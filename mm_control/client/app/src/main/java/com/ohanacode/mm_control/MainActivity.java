package com.ohanacode.mm_control;

import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.widget.ImageButton;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {
    private final String TAG = "Main";
    private ThreadedCommunication comms;
    public TextView TouchSurface;
    int lastX = 0;
    int lastY = 0;

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

                float screenX = event.getX();
                float screenY = event.getY();

                int xOffset = xMotion(screenX);
                int yOffset = yMotion(screenY);
                Log.i(TAG, "TOUCH OFFSET:" + xOffset + ", " + yOffset);

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

        // TCP communication
        comms = new ThreadedCommunication(this);

    }


    @Override
    protected void onStop() {
        comms.stopTcpServer();
        super.onStop();
    }

    @Override
    protected void onPause() {
        comms.stopTcpServer();
        super.onPause();
    }

    @Override
    public void onResume(){
        super.onResume();
    }


    int xMotion(float x ){
        int offset_X = 0;

        if(lastX != 0){
            offset_X = (int)x - lastX;
        }

        lastX = (int)x;

        return offset_X;
    }

    int yMotion(float y){

        int offset_Y = 0;

        if(lastY != 0){
            offset_Y = (int)y - lastY;
        }

        lastY = (int)y;

        return offset_Y;
    }

}
