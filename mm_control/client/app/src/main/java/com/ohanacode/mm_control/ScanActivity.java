package com.ohanacode.mm_control;

import androidx.appcompat.app.ActionBar;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;

import java.util.ArrayList;
import java.util.List;

public class ScanActivity extends AppCompatActivity {

    private final String TAG = "ScanActivity";
    private DiscoveryAndUdpComms comms;
    private Handler deviceProcessorHandler;
    private ListView lv;
    private List<String> deviceArrayList;
    ArrayAdapter<String> arrayAdapter;
    private boolean scanInProgress = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_scan);

        ActionBar actionBar = getSupportActionBar();
        if(actionBar != null)
        {
            actionBar.setTitle("Select server");
        }

        lv = findViewById(R.id.DeviceList);
        deviceArrayList = new ArrayList<String>();
        arrayAdapter = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1, deviceArrayList );

        lv.setAdapter(arrayAdapter);
        lv.setClickable(true);
        lv.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {

                String selectedIp =  arrayAdapter.getItem(position);

                Intent data = new Intent();
                data.putExtra("selected", selectedIp);
                setResult(RESULT_OK, data);
                finish();
            }
        });

        // Setup scan button
        Button refreshBtn = findViewById(R.id.button_scan);
        refreshBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                scanNetworkForServer();
            }
        });

        // Initiate scan
        comms = DiscoveryAndUdpComms.getInstance(this);
        deviceProcessorHandler = new Handler();
        scanNetworkForServer();
    }

    @Override
    protected void onPause() {
        comms.stopTcpServer();
        super.onPause();
    }

    void scanNetworkForServer(){
        if(!scanInProgress) {
            scanInProgress = true;
            comms.discoverDevices();
            deviceProcessorHandler.postDelayed(deviceResponseProcessor, 500);
        }
    }

    Runnable deviceResponseProcessor = new Runnable() {
        @Override
        public void run() {
            List<String> response = comms.getDeviceList();
            deviceArrayList.clear();

            for(int i = 0; i < response.size(); i++){
                deviceArrayList.add(response.get(i));
                Log.d(TAG, "FOUND server at:" + response.get(i));
            }
            arrayAdapter.notifyDataSetChanged();

            scanInProgress = false;
        }
    };
}
