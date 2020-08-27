package com.ohanacode.mm_control;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.text.SpannableString;
import android.text.method.LinkMovementMethod;
import android.text.util.Linkify;
import android.util.Log;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.TextView;

import java.util.List;

public class MainActivity extends AppCompatActivity {
    private final String TAG = "Main";

    // Code types
    private final short CODE_MOUSE = 3;
    // Mouse clicks
    private final short KEY_MOUSE_LEFT = 1;
    private final short KEY_MOUSE_RIGHT = 2;

    private ThreadedCommunication comms;
    public TextView TouchSurface;
    int lastX = 0;
    int lastY = 0;
    private AlertDialog aboutDialog;
    private Handler deviceProcessorHandler;
    private int REQUEST_CODE = 13;
    String serverIP = "";


    @SuppressLint("ClickableViewAccessibility")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

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

        Button btnMouseLeft = findViewById(R.id.button_left);
        btnMouseLeft.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // Send left click
            }
        });

        Button btnMouseRight = findViewById(R.id.button_right);
        btnMouseRight.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // Send right click
            }
        });

        // TCP communication
        comms = new ThreadedCommunication(this);

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

        deviceProcessorHandler = new Handler();

    }


    @Override
    protected void onStop() {
        comms.stopTcpServer();
        aboutDialog.dismiss();
        super.onStop();
    }

    @Override
    protected void onPause() {
        comms.stopTcpServer();
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
            comms.stopTcpServer();

            Intent scanIntent = new Intent(MainActivity.this, ScanActivity.class);
            startActivityForResult(scanIntent, REQUEST_CODE);


//            comms.discoverDevices();
//            deviceProcessorHandler.postDelayed(deviceResponseProcessor, 500);
        }

        return true;
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (resultCode == RESULT_OK && requestCode == REQUEST_CODE) {
            if (data.hasExtra("selected")) {
                serverIP = data.getExtras().getString("selected");
                Log.i(TAG, "SELECTED:" + serverIP);
            }
        }

        super.onActivityResult(requestCode, resultCode, data);
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
            short codeType = CODE_MOUSE;

            Log.i(TAG, "TOUCH OFFSET:" + offset_X + ", " + offset_Y);

        }
    }

    Runnable deviceResponseProcessor = new Runnable() {
        @Override
        public void run() {
            List<String> deviceList = comms.getDeviceList();

            for(int i = 0; i < deviceList.size(); i++){

                Log.d(TAG, "FOUND server at:" + deviceList.get(i));
            }
        }
    };

}
