package com.ohanacode.mm_control;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.Color;
import android.os.Bundle;
import android.text.SpannableString;
import android.text.method.LinkMovementMethod;
import android.text.util.Linkify;
import android.util.Log;
import android.view.GestureDetector;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.TextView;


public class MainActivity extends AppCompatActivity {

    private final String TAG = "Main";
    public TextView TouchSurface;
    int lastX = 0;
    int lastY = 0;
    private AlertDialog aboutDialog;
    private int REQUEST_CODE = 13;
    String serverIP = "";
    TcpClient sender;
    static final String mouseBtnColorUp = "#DBF0EA";
    static final String mouseBtnColorDown = "#BCCCC8";


    @SuppressLint("ClickableViewAccessibility")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        serverIP = readServerIp();
        sender = TcpClient.getInstance();
        sender.startSender(serverIP);

        // Setup touchpad surface
        TouchSurface = findViewById(R.id.textView_touch);
        TouchSurface.setOnTouchListener(new View.OnTouchListener(){
            @Override
            public boolean onTouch(View v, MotionEvent event) {

                switch (event.getAction()) {
                    case MotionEvent.ACTION_DOWN:
                        //pointer down.
                        lastX = 0;
                        lastY = 0;
                        break;
                    case MotionEvent.ACTION_UP:
                    case MotionEvent.ACTION_CANCEL:
                    case MotionEvent.ACTION_OUTSIDE:
                        //event has finished, pointer is up or event was canceled or the pointer is outside of the view's bounds
                        break;
                }

                float screenX = event.getX();
                float screenY = event.getY();

                processTouch((int)screenX, (int)screenY);
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
        btnMouseLeft.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                byte[] msg = new byte[3];
                msg[0] = CommandData.CODE_SPECIAL;
                msg[1] = CommandData.KEY_MOUSE_LEFT;
                sendTcpMsg(msg);
            }
        });
//        btnMouseLeft.setBackgroundColor(Color.parseColor(mouseBtnColorUp));
//        btnMouseLeft.setOnTouchListener(new View.OnTouchListener() {
//            @Override
//            public boolean onTouch(View v, MotionEvent event) {
//                byte[] msg = new byte[3];
//                msg[0] = CommandData.CODE_SPECIAL;
//
//                switch ( event.getAction() ) {
//                    case MotionEvent.ACTION_DOWN:
//                        msg[1] = CommandData.KEY_MOUSE_LEFT_DOWN;
//                        sendTcpMsg(msg);
//                        btnMouseLeft.setBackgroundColor(Color.parseColor(mouseBtnColorDown));
//                        break;
//                    case MotionEvent.ACTION_UP:
//                        msg[1] = CommandData.KEY_MOUSE_LEFT_UP;
//                        sendTcpMsg(msg);
//                        btnMouseLeft.setBackgroundColor(Color.parseColor(mouseBtnColorUp));
//                        break;
//                }
//                return true;
//            }
//        });

        final Button btnMouseRight = findViewById(R.id.button_right);
        btnMouseRight.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                byte[] msg = new byte[3];
                msg[0] = CommandData.CODE_SPECIAL;
                msg[1] = CommandData.KEY_MOUSE_RIGHT;
                sendTcpMsg(msg);
            }
        });
//        btnMouseRight.setBackgroundColor(Color.parseColor(mouseBtnColorUp));
//        btnMouseRight.setOnTouchListener(new View.OnTouchListener() {
//            @Override
//            public boolean onTouch(View v, MotionEvent event) {
//                byte[] msg = new byte[3];
//                msg[0] = CommandData.CODE_SPECIAL;
//
//                switch ( event.getAction() ) {
//                    case MotionEvent.ACTION_DOWN:
//                        msg[1] = CommandData.KEY_MOUSE_RIGHT_DOWN;
//                        sendTcpMsg(msg);
//                        btnMouseRight.setBackgroundColor(Color.parseColor(mouseBtnColorDown));
//                        break;
//                    case MotionEvent.ACTION_UP:
//                        msg[1] = CommandData.KEY_MOUSE_RIGHT_UP;
//                        sendTcpMsg(msg);
//                        btnMouseRight.setBackgroundColor(Color.parseColor(mouseBtnColorUp));
//                        break;
//                }
//                return true;
//            }
//        });

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
                sender.startSender(serverIP);
            }
        }

        super.onActivityResult(requestCode, resultCode, data);
    }

    public void sendTcpMsg(byte[] msg){
        if(!sender.sendMsg(msg)){
            sender.startSender(serverIP);
            sender.sendMsg(msg);
        }
    }

    private void processTouch(int x, int y){
        int offset_X = 0;
        int offset_Y = 0;

        if(lastX != 0){
            offset_X = x - lastX;
            if(offset_X > 127){
                offset_X = 127;
            }else if(offset_X < -127){
                offset_X = -127;
            }
        }
        if(lastY != 0){
            offset_Y = y - lastY;
            if(offset_Y > 127){
                offset_Y = 127;
            }else if(offset_Y < -127){
                offset_Y = -127;
            }
        }

        lastX = x;
        lastY = y;

        if((offset_X != 0) || (offset_Y != 0)){
            byte[] msg = new byte[3];
            msg[0] = CommandData.CODE_MOUSE;
            msg[1] = (byte) (offset_X & 0xFF);
            msg[2] = (byte) (offset_Y & 0xFF);

            sendTcpMsg(msg);
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
}
