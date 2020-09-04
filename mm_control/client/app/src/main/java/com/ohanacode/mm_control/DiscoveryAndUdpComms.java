package com.ohanacode.mm_control;

import android.content.Context;
import android.net.DhcpInfo;
import android.net.wifi.WifiManager;
import android.os.StrictMode;
import android.util.Log;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketException;
import java.util.ArrayList;
import java.util.List;


public class DiscoveryAndUdpComms {

    private final String TAG = "DiscoveryAndUdpComms";

    private static final int TCP_RX_PORT = 4211;
    private static final int UDP_TX_PORT = 4210;         /* Port on which to broadcast the ping message */
    private static final String pingMsg = "ujagaga ping";   /* Message to broadcast */
    private static final int SOCKET_TIMEOUT = 500;
    private ServerSocket serverSocket;
    private Thread serverThread;
    private boolean tcpServerEnabledFlag = false;
    private final int DEV_ID_INDEX = 6;
    private final int DEV_ID_MEDIA_TYPE = 80;
    private List<String> deviceList = new ArrayList<>();
    private DatagramSocket udpSocket = null;
    private static DiscoveryAndUdpComms instance;
    private InetAddress broadcastAddr;


    private DiscoveryAndUdpComms(Context c){
        try {
            broadcastAddr = getBroadcastAddress(c);
        } catch (IOException e) {
            Log.e(TAG, "IOException: " + e.getMessage());
        }

        try {
            udpSocket = new DatagramSocket();
            udpSocket.setBroadcast(true);
            udpSocket.setSoTimeout(SOCKET_TIMEOUT);

        }catch (SocketException e) {
            Log.e(TAG, "SocketException: " + e.getMessage());
        }

        startTcpServer();
    }

    public static DiscoveryAndUdpComms getInstance(Context c)
    {
        if (instance== null) {
            synchronized(TcpClient.class) {
                if (instance == null) {
                    instance = new DiscoveryAndUdpComms(c);
                }
            }
        }

        // Return the instance
        return instance;
    }

    private class ServerThread extends Thread {

        @Override
        public void run() {
            try {
                serverSocket = new ServerSocket(TCP_RX_PORT);

                while (tcpServerEnabledFlag) {
                    // block the call until connection is created and return
                    // Socket object
                    Socket socket = serverSocket.accept();

                    CommunicationThread commThread = new CommunicationThread(socket);
                    new Thread(commThread).start();
                }

            } catch (Exception e) {
                Log.d(TAG, e.getMessage());
            }
        }

    }

    class CommunicationThread implements Runnable {

        private Socket clientSocket;

        private BufferedReader input;

        public CommunicationThread(Socket clientSocket) {

            this.clientSocket = clientSocket;

            try {
                this.input = new BufferedReader(new InputStreamReader(this.clientSocket.getInputStream()));
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        public void run() {
            while (!Thread.currentThread().isInterrupted()) {

                try {

                    String read = input.readLine();

                    if (read == null ){
                        Thread.currentThread().interrupt();
                    }else{
                        if(read.length() > 1) {
                            Log.i(TAG, "Message from " + this.clientSocket.getInetAddress() + ":" + this.clientSocket.getPort() + ": " + read);

                            String[] data = read.split(":");
                            if (data.length > DEV_ID_INDEX) {
                                int deviceId = 0;
                                try {
                                    deviceId = Integer.parseInt(data[DEV_ID_INDEX], 16);

                                    if(deviceId == DEV_ID_MEDIA_TYPE){
                                        String serverIp = this.clientSocket.getInetAddress().toString();
                                        serverIp = serverIp.substring(1);
                                        deviceList.add(serverIp);
                                    }
                                } catch(NumberFormatException nfe) {
                                    Log.i(TAG, "ERROR: could not get device ID. " + nfe.getMessage());
                                }
                            }
                        }
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }

    }

    /* Calculates current IP address or a broadcast address */
    private static InetAddress getBroadcastAddress(Context c) throws IOException {
        WifiManager wifi = (WifiManager) c.getApplicationContext().getSystemService(Context.WIFI_SERVICE);
        DhcpInfo dhcp = wifi.getDhcpInfo();

        int addr = dhcp.ipAddress;

        byte[] quads = new byte[4];
        for (int k = 0; k < 4; k++) {
            quads[k] = (byte) ((addr >> k * 8) & 0xFF);
        }

        quads[3] = (byte)255;

        return InetAddress.getByAddress(quads);
    }

    private void startTcpServer(){
        if(!tcpServerEnabledFlag) {
            Log.d("TC_Start", "Starting");
            tcpServerEnabledFlag = true;
            serverThread = new Thread(new ServerThread());
            serverThread.start();
        }else{
            Log.d("TC_Start", "Already started");
        }
    }

    public void stopTcpServer(){
        Log.d("TC_Stop", "Stopping");
        try {
            if(serverSocket != null) {
                serverSocket.close();
            }else{
                Log.d(TAG, "Already stopped");
            }
        } catch (Exception e) {
            Log.e(TAG, "SocketException: " + e.getMessage());
        }

        tcpServerEnabledFlag = false;
    }

    public void sendUdpMsg(byte[] msg, String destinationIP){
        StrictMode.ThreadPolicy policy = new StrictMode.ThreadPolicy.Builder().permitAll().build();
        StrictMode.setThreadPolicy(policy);
        try {
            DatagramPacket sendPacket = new DatagramPacket(msg, msg.length, InetAddress.getByName(destinationIP), UDP_TX_PORT);
            udpSocket.send(sendPacket);
        }catch (Exception e) {
            Log.e(TAG, "SocketException1: " + e.getMessage());
        }
    }

    /* Refresh all devices on the same network. To do this, we broadcast a UDP ping message,
     * and all devices will respond via TCP. */
    public void discoverDevices(){
        deviceList.clear();

        startTcpServer();

        StrictMode.ThreadPolicy policy = new StrictMode.ThreadPolicy.Builder().permitAll().build();
        StrictMode.setThreadPolicy(policy);
        try {
            DatagramSocket socket = new DatagramSocket();
            socket.setBroadcast(true);
            socket.setSoTimeout(SOCKET_TIMEOUT);
            byte[] sendData = pingMsg.getBytes();

            /* UDP package is not guarantied to arrive, so we send several of them and hope that at least one will arrive. */
            for (int i = 0; i < 5; i++) {
                try {
                    DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, broadcastAddr, UDP_TX_PORT);
                    socket.send(sendPacket);

                } catch (IOException e) {
                    Log.e("TC_discoverDevices", "IOException: " + e.getMessage());
                }
            }
        }catch (SocketException e) {
            Log.e("TC_discoverDevices", "SocketException: " + e.getMessage());
        }
    }

    public List<String> getDeviceList(){
        return deviceList;
    }

    public void closeUdpSocket(){
        try {
            udpSocket.close();
        }catch (Exception e){

        }
    }
}
