package com.ohanacode.mm_control;

import android.content.Context;
import android.net.DhcpInfo;
import android.net.wifi.WifiManager;
import android.os.Handler;
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


public class ServerDiscovery {

    private final String TAG = "COMMS";

    private static final int RCV_SERVERPORT = 4211;
    private static final int PING_PORT = 4210;         /* Port on which to broadcast the ping message */
    private static final String pingMsg = "ujagaga ping";   /* Message to broadcast */
    private static final int SOCKET_TIMEOUT = 500;
    private ServerSocket serverSocket;
    private Thread serverThread;
    private boolean tcpServerEnabledFlag = false;
    private Context context;
    private final int DEV_ID_INDEX = 6;
    private final int DEV_ID_MEDIA_TYPE = 80;
    private List<String> deviceList = new ArrayList<>();
    private Handler deviceProcessorHandler;

    public ServerDiscovery(Context c){
        context = c;
        startTcpServer();
    }

    private class ServerThread extends Thread {

        @Override
        public void run() {
            try {
                serverSocket = new ServerSocket(RCV_SERVERPORT);

                while (tcpServerEnabledFlag) {
                    // block the call until connection is created and return
                    // Socket object
                    Socket socket = serverSocket.accept();

                    CommunicationThread commThread = new CommunicationThread(socket);
                    new Thread(commThread).start();
                }

            } catch (Exception e) {
                Log.d("TC_ServerThreadRun", e.getMessage());
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
        Log.d("TC_Start", "Starting");
        if(!tcpServerEnabledFlag) {
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
                Log.d("TC_Stop", "Already stopped");
            }
        } catch (Exception e) {
            Log.e("TC_stopTcpServer", "SocketException: " + e.getMessage());
        }

        tcpServerEnabledFlag = false;
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
                    DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, getBroadcastAddress(context), PING_PORT);
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
}
