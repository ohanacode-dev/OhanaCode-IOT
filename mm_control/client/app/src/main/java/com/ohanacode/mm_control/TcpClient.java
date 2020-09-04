package com.ohanacode.mm_control;

import android.util.Log;

import java.io.DataOutputStream;
import java.net.InetAddress;
import java.net.Socket;
import java.net.SocketException;

public class TcpClient{

    private final String TAG = "TCP_CLIENT";
    private static TcpClient instance;
    private Socket socket;

    private static final int SERVERPORT = 4213;
    private static String SERVER_IP = "";
    private static byte[] msg;
    private Thread clientThread;

    private TcpClient()
    {
        // Constructor hidden because this is a singleton
    }

    public static TcpClient getInstance()
    {
        if (instance== null) {
            synchronized(TcpClient.class) {
                if (instance == null) {
                    instance = new TcpClient();
                }
            }
        }
        // Return the instance
        return instance;
    }

    public void startSender(String serverIP){
        endClient();
        SERVER_IP = serverIP;
        clientThread = new Thread(new ClientThread());
        clientThread.start();
    }

    public void restartSender(){
        endClient();
        clientThread = new Thread(new ClientThread());
        clientThread.start();
    }

    public boolean sendMsg(byte[] buf) {
        try {
            DataOutputStream s_out = new DataOutputStream(socket.getOutputStream());
            s_out.write(buf);
//            Log.i(TAG, "Sending:" + buf[0] + ' ' + buf[1] + ' ' + buf[2]);
            return true;
        } catch (SocketException se){
            Log.e(TAG, "ERROR1 sending data to:" + SERVER_IP + " " + se.getMessage());
        } catch (Exception e) {
            Log.e(TAG, "ERROR2 sending data:" + e.getMessage());
            e.printStackTrace();
        }

        return false;
    }

    public void endClient(){
        try {
            clientThread.interrupt();
        } catch (Exception e1) {}

        try {
            socket.close();
        } catch (Exception e1) {}
    }

    class ClientThread implements Runnable {
        @Override
        public void run() {
            try {
                InetAddress serverAddr = InetAddress.getByName(SERVER_IP);
                socket = new Socket(serverAddr, SERVERPORT);
            } catch (Exception e1) {
                Log.e(TAG, "ERROR connecting:" + e1.getMessage());
            }
        }
    }
}
