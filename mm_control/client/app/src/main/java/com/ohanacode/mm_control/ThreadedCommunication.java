package com.ohanacode.mm_control;

import android.content.Context;
import android.net.DhcpInfo;
import android.net.wifi.WifiManager;
import android.os.StrictMode;
import android.util.Log;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.HttpURLConnection;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketException;
import java.net.URL;
import java.util.ArrayList;


public class ThreadedCommunication {

    private static final int RCV_SERVERPORT = 4211;
    private static final int PING_PORT = 4210;         /* Port on which to broadcast the ping message */
    private static final String pingMsg = "ujagaga ping";   /* Message to broadcast */
    private static final int SOCKET_TIMEOUT = 500;
    private ServerSocket serverSocket;
    private Thread serverThread;
    private boolean tcpServerEnabledFlag = false;
    private ArrayList<String> responseList = new ArrayList<>();
    private Context context;

    public ThreadedCommunication(Context c){
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

//                    Log.d("RCV", "Message from " + socket.getInetAddress() + ":" + socket.getPort());

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

        private CommunicationThread(Socket clientSocket) {

            this.clientSocket = clientSocket;
        }

        public void run() {

            try {
                String IPAddr = clientSocket.getInetAddress().toString();
                String idUrl = "http:/" + IPAddr + "/id";
                /* Now query id */
                String response = queryUrl(idUrl);

                if(response.length() < 2){
                    /* No data received. Try alternate URL. */

                    IPAddr += ":" + String.valueOf(PING_PORT + 1);
                    idUrl = "http:/" + IPAddr + "/id";

                    response = queryUrl(idUrl);
                }

                if(response.length() > 2){
                    String devResponse = "IP:" + IPAddr.replace("/", "") + "\n" + response;
                    responseList.add(devResponse);
                    Log.d("TC_CommsThreadRun", "Response: " + devResponse);
                }

            } catch (Exception e) {
                Log.e("TC_CommsThreadRun", e.getMessage());
            }

        }

    }

    private String queryUrl(String urlStr){

        URL url;
        HttpURLConnection urlConnection = null;
        String response = "";

        try {
            url = new URL(urlStr);

            urlConnection = (HttpURLConnection) url.openConnection();

            InputStream in = urlConnection.getInputStream();
            InputStreamReader isw = new InputStreamReader(in);

            int data = isw.read();
            while (data != -1) {
                response += (char) data;
                data = isw.read();
            }

//            Log.d("TC_queryUrl", "response from " + urlStr + ":" + response);

        } catch (Exception e) {
            Log.e("TC_queryUrl", e.getMessage());
            response = "";
        } finally {
            if (urlConnection != null) {
                urlConnection.disconnect();
            }
        }

        return response;
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

    public ArrayList<String> getResponseList(){
        Log.d("TC_getResponseList", "Responding with message count: " + String.valueOf(responseList.size()));
        return responseList;
    }

    /* Refresh all devices on the same network. To do this, we broadcast a UDP ping message,
     * and all devices will respond via TCP. */
    public void discoverDevices(){
        responseList.clear();

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
}
