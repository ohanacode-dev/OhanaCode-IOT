package com.ohanacode.mm_control;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.os.Handler;
import android.text.SpannableString;
import android.text.method.LinkMovementMethod;
import android.text.util.Linkify;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.TextView;


public class MainActivity extends AppCompatActivity {

    private static final String TAG = "MAIN_ACTIVITY";
    private static final int TAP_TIMEOUT = 100;
    private static final int TAP_PROCESS_TIMEOUT = 300;
    private static final int TOUCH_PROCESS_TIMEOUT = 50;
    private static final int REQUEST_CODE = 13;
    private String serverIP = "";
    private int prevX = 0;
    private int prevY = 0;
    private int nextX = 0;
    private int nextY = 0;
    private boolean surfaceTouchedFlag = false;
    private boolean mouseDownFlag = false;
    private Long touchTimestamp;
    private Long tapTimestamp;
    private AlertDialog aboutDialog;
    private Handler tapProcessorHandler;
    private Handler touchSenderHandler;
    private DiscoveryAndUdpComms udpSender;
    private TcpClient tcpSender;

    @SuppressLint("ClickableViewAccessibility")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        serverIP = readServerIp();
        tcpSender = TcpClient.getInstance();
        tcpSender.startSender(serverIP);

        udpSender = DiscoveryAndUdpComms.getInstance(this);
        touchSenderHandler = new Handler();
        touchSenderHandler.postDelayed(touchSenderProcessor, TOUCH_PROCESS_TIMEOUT);

        // Setup touchpad surface
        touchTimestamp = System.currentTimeMillis();
        tapTimestamp = touchTimestamp;
        tapProcessorHandler = new Handler();
        TextView TouchSurface = findViewById(R.id.textView_touch);
        TouchSurface.setOnTouchListener(new View.OnTouchListener(){
            @Override
            public boolean onTouch(View v, MotionEvent event) {

                switch (event.getAction()) {
                    case MotionEvent.ACTION_DOWN:
                        //pointer down.
                        prevX = (int)event.getX();
                        prevY = (int)event.getY();
                        touchTimestamp = System.currentTimeMillis();
                        surfaceTouchedFlag = true;
                        break;
                    case MotionEvent.ACTION_UP:
                        surfaceTouchedFlag = false;
                        Long releaseTimestamp = System.currentTimeMillis();
                        if((releaseTimestamp - touchTimestamp) < TAP_TIMEOUT){
                            // Tap detected
                            if((releaseTimestamp - tapTimestamp) < TAP_PROCESS_TIMEOUT) {
                                // Last tap happened recently. This is a double tap, so cancel touch processing and just send a double click.
                                tapProcessorHandler.removeCallbacksAndMessages(null);
                                mouseDownFlag = false;
                                byte[] msg = new byte[3];
                                msg[0] = CommandData.CODE_SPECIAL;
                                msg[1] = CommandData.KEY_MOUSE_LEFT_DOUBLECLICK;
                                sendTcpMsg(msg);
                            }else{
                                // Last tap was a long time ago
                                tapProcessorHandler.postDelayed(tapProcessor, TAP_PROCESS_TIMEOUT);
                            }

                            tapTimestamp = releaseTimestamp;
                        }

                        if(mouseDownFlag){
                            mouseDownFlag = false;
                            byte[] msg = new byte[3];
                            msg[0] = CommandData.CODE_SPECIAL;
                            msg[1] = CommandData.KEY_MOUSE_LEFT_UP;
                            sendTcpMsg(msg);
                        }

                        break;
                    case MotionEvent.ACTION_CANCEL:
                    case MotionEvent.ACTION_OUTSIDE:
                        //event has finished, pointer is up or event was canceled or the pointer is outside of the view's bounds
                        break;
                }

                nextX = (int)event.getX();
                nextY = (int)event.getY();

                return true;
            }
        });

        ImageButton btnKbd = findViewById(R.id.button_keyboard);
        btnKbd.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent keyboardIntent = new Intent(MainActivity.this, KeyboardActivity.class);
                MainActivity.this.startActivity(keyboardIntent);
            }
        });

        final Button btnMouseLeft = findViewById(R.id.button_left);
        btnMouseLeft.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                byte[] msg = new byte[3];
                msg[0] = CommandData.CODE_SPECIAL;

                switch ( event.getAction() ) {
                    case MotionEvent.ACTION_DOWN:
                        msg[1] = CommandData.KEY_MOUSE_LEFT_DOWN;
                        sendTcpMsg(msg);
                        btnMouseLeft.getBackground().setAlpha(150);

                        break;
                    case MotionEvent.ACTION_UP:
                        msg[1] = CommandData.KEY_MOUSE_LEFT_UP;
                        sendTcpMsg(msg);
                        btnMouseLeft.getBackground().setAlpha(255);
                        break;
                }
                return true;
            }
        });

        final Button btnMouseRight = findViewById(R.id.button_right);
        btnMouseRight.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                byte[] msg = new byte[3];
                msg[0] = CommandData.CODE_SPECIAL;

                switch ( event.getAction() ) {
                    case MotionEvent.ACTION_DOWN:
                        msg[1] = CommandData.KEY_MOUSE_RIGHT_DOWN;
                        sendTcpMsg(msg);
                        btnMouseRight.getBackground().setAlpha(150);
                        break;
                    case MotionEvent.ACTION_UP:
                        msg[1] = CommandData.KEY_MOUSE_RIGHT_UP;
                        sendTcpMsg(msg);
                        btnMouseRight.getBackground().setAlpha(255);
                        break;
                }
                return true;
            }
        });


        /* Create the About dialog as a globally accessible object so we can close it when the app goes in the background */
        final String aboutString = "Author: Rada Berar\ne-mail: rada.berar@ohanacode-dev.com\n\n" +
                "http://ohanacode-dev.com\nhttp://radinaradionica.com";

        final SpannableString msg = new SpannableString(aboutString);
        Linkify.addLinks(msg, Linkify.ALL);
        aboutDialog = new AlertDialog.Builder(this)
                .setIcon(android.R.drawable.ic_dialog_info)
                .setMessage( msg )
                .setTitle("About")
                .create();
    }


    @Override
    protected void onStop() {
        aboutDialog.dismiss();
        super.onStop();
    }

    @Override
    protected void onPause() {
        aboutDialog.dismiss();
        super.onPause();
    }

    @Override
    public void onResume(){
        super.onResume();
    }

    @Override
    public void onBackPressed() {
        super.onBackPressed();
    }

    /* Create options menu */
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.main_menu, menu);
        return true;
    }

    /* Handle menu item click event */
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        if(item.getItemId() == R.id.about){
            aboutDialog.show();
            ((TextView)aboutDialog.findViewById(android.R.id.message)).setMovementMethod(LinkMovementMethod.getInstance());
        }else if(item.getItemId() == R.id.scan){
            Intent scanIntent = new Intent(MainActivity.this, ScanActivity.class);
            startActivityForResult(scanIntent, REQUEST_CODE);
        }

        return true;
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (resultCode == RESULT_OK && requestCode == REQUEST_CODE) {
            if (data.hasExtra("selected")) {
                serverIP = data.getExtras().getString("selected");
                saveServerIp(serverIP);
                tcpSender.startSender(serverIP);
            }
        }

        super.onActivityResult(requestCode, resultCode, data);
    }

    public void sendTcpMsg(byte[] msg){
        if(!tcpSender.sendMsg(msg)){
            tcpSender.startSender(serverIP);
            tcpSender.sendMsg(msg);
        }
    }


    private void processTouch(int x, int y){
        int offset_X = 0;
        int offset_Y = 0;

        offset_X = nextX - prevX;
        if(offset_X > 127){
            offset_X = 127;
        }else if(offset_X < -127){
            offset_X = -127;
        }

        prevX = nextX;

        offset_Y = nextY - prevY;
        if(offset_Y > 127){
            offset_Y = 127;
        }else if(offset_Y < -127){
            offset_Y = -127;
        }

        prevY = nextY;

        if((offset_X != 0) || (offset_Y != 0)){
            byte[] msg = new byte[3];
            msg[0] = CommandData.CODE_MOUSE;
            msg[1] = (byte) (offset_X & 0xFF);
            msg[2] = (byte) (offset_Y & 0xFF);

            udpSender.sendUdpMsg(msg, serverIP);
        }
    }

    private void saveServerIp(String ipAddr){
        SharedPreferences prefs = getSharedPreferences("srv_prefs", this.MODE_PRIVATE);
        SharedPreferences.Editor editor = prefs.edit();

        editor.putString("serverip", ipAddr);
        editor.apply();
    }

    public String readServerIp() {
        SharedPreferences prefs = getSharedPreferences("srv_prefs", this.MODE_PRIVATE);
        return prefs.getString("serverip", "");
    }

    Runnable tapProcessor = new Runnable() {
        @Override
        public void run() {
            if(surfaceTouchedFlag){
                // Mouse down
                mouseDownFlag = true;
                byte[] msg = new byte[3];
                msg[0] = CommandData.CODE_SPECIAL;
                msg[1] = CommandData.KEY_MOUSE_LEFT_DOWN;
                sendTcpMsg(msg);
            }else{
                // click
                byte[] msg = new byte[3];
                msg[0] = CommandData.CODE_SPECIAL;
                msg[1] = CommandData.KEY_MOUSE_LEFT_CLICK;
                sendTcpMsg(msg);
            }
        }
    };

    Runnable touchSenderProcessor = new Runnable() {
        @Override
        public void run() {
            processTouch(nextX, nextY);
            touchSenderHandler.postDelayed(touchSenderProcessor, TOUCH_PROCESS_TIMEOUT);
        }
    };
}
