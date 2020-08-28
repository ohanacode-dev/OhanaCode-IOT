package com.ohanacode.mm_control;

import android.content.Context;

import java.io.DataOutputStream;
import java.io.IOException;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;

public class TcpClient {

    private final String TAG = "TCP_CLIENT";
    private Socket socket;
    private Context context;

    private static final int SERVERPORT = 4213;
    private static String SERVER_IP = "";

    public TcpClient(Context c, String serverIP){
        context = c;
        SERVER_IP = serverIP;
        new Thread(new ClientThread()).start();
    }

    public void sendMsg(byte[] buf) {
        try {
            DataOutputStream s_out = new DataOutputStream( socket.getOutputStream());
            s_out.write(buf);
//
//            PrintWriter out = new PrintWriter(new BufferedWriter(new OutputStreamWriter(socket.getOutputStream())),true);
//            out.println(message);
        } catch (UnknownHostException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    class ClientThread implements Runnable {
        @Override
        public void run() {
            try {
                InetAddress serverAddr = InetAddress.getByName(SERVER_IP);
                socket = new Socket(serverAddr, SERVERPORT);
            } catch (UnknownHostException e1) {
                e1.printStackTrace();
            } catch (IOException e1) {
                e1.printStackTrace();
            }
        }
    }
}
