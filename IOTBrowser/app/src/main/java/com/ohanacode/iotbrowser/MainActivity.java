package com.ohanacode.iotbrowser;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;
import androidx.constraintlayout.widget.ConstraintLayout;

import android.net.wifi.SupplicantState;
import android.os.Bundle;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.drawable.ColorDrawable;
import android.net.Uri;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Handler;
import android.text.SpannableString;
import android.text.method.LinkMovementMethod;
import android.text.util.Linkify;
import android.util.Log;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.TextView;

import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

public class MainActivity extends AppCompatActivity {

    private static final String TAG = "MAIN";
    private TextView statusText;
    private Handler wifiRefreshHandler;
    private Handler deviceProcessorHandler;
    private DisplayAdapter adapter;
    private ThreadedCommunication comms;
    private AlertDialog aboutDialog;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        /* Set custom title bar color */
        if(getSupportActionBar() != null){
            getSupportActionBar().setBackgroundDrawable(new ColorDrawable(getColor(R.color.colorDarker)));
        }

        statusText = findViewById(R.id.textView_status);

        Button btnAllOff = findViewById(R.id.button_allOff);

        btnAllOff.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                allDevsOff();
            }
        });

        Button btnRescan = findViewById(R.id.button_rescan);

        btnRescan.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                refreshDevs();
            }
        });

        ListView deviceList = findViewById(R.id.device_list);

        adapter = new DisplayAdapter();

        deviceList.setAdapter(adapter);

        /* If item is clicked, this should take us to the devices web UI */
        deviceList.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {

                view.setBackgroundColor(getColor(R.color.colorSelected));

                DeviceData item =  adapter.getItem(position);

                Log.i("SELECT", item.mac);

                String urlStr = "http://" + item.ip;

                Intent browserIntent = new Intent(Intent.ACTION_VIEW, Uri.parse(urlStr));
                startActivity(browserIntent);
            }
        });

        /* On long click we are setting up the label */
        deviceList.setOnItemLongClickListener(new AdapterView.OnItemLongClickListener() {
            @Override
            public boolean onItemLongClick(AdapterView<?> adapterView, final View view, final int position, long l) {

                final DeviceData item =  adapter.getItem(position);

                final String mac = item.mac;
                final String label = item.label;
                final String props = item.properties;

                AlertDialog.Builder alertDialog = new AlertDialog.Builder(MainActivity.this);
                alertDialog.setTitle("Set Label");
                alertDialog.setMessage( props );

                final EditText input = new EditText(MainActivity.this);
                ConstraintLayout.LayoutParams lp = new ConstraintLayout.LayoutParams(
                        ConstraintLayout.LayoutParams.MATCH_PARENT,
                        ConstraintLayout.LayoutParams.MATCH_PARENT);
                input.setLayoutParams(lp);
                input.setText(label, TextView.BufferType.EDITABLE);

                alertDialog.setView(input);

                alertDialog.setPositiveButton("Apply",
                        new DialogInterface.OnClickListener(){
                            public void onClick (DialogInterface dialog,int which) {

                                String labelToSet = input.getText().toString();

                                if(labelToSet.length() < 3){
                                    labelToSet = "";
                                }

                                saveLabel(mac, labelToSet);
                            }
                        });

                alertDialog.setOnDismissListener(new DialogInterface.OnDismissListener() {
                    public void onDismiss(final DialogInterface dialog) {
                        refreshDevs();
                    }
                });

                input.requestFocus();
                alertDialog.show();

                return true;
            }
        });

        /* Create the About dialog as a globally accessible object so we can close it when the app goes in the background */
        final String aboutString = "Author: Rada Berar\ne-mail: rada.berar@ohanacode-dev.com\n\n" +
                "http://ohanacode-dev.com\nhttp://radinaradionica.com\n\nPersonal IOT project device locator via " +
                "UDP broadcast ping.\nTip: try long click on listed device to set label.";

        final SpannableString msg = new SpannableString(aboutString);
        Linkify.addLinks(msg, Linkify.ALL);

        aboutDialog = new AlertDialog.Builder(this)
                .setIcon(android.R.drawable.ic_dialog_info)
                .setMessage( msg )
                .setTitle("About")
                .create();

        comms = new ThreadedCommunication(this);

        deviceProcessorHandler = new Handler();
        wifiRefreshHandler = new Handler();
        WiFiStatusChecker.run();

    }

    @Override
    protected void onStop() {
        comms.stopTcpServer();
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
        refreshDevs();
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
        }

        return true;
    }

    private DeviceData parseMResponseMessage(String msg){
        DeviceData result = new DeviceData();
        int i;
        String devProperties = "";
        String[] props = msg.split("\n");

        for(i = 0; i < props.length; i++){
            if(props[i].split(":")[0].equals("MAC")){
                result.mac = props[i].substring(4);
            }else if(props[i].split(":")[0].equals("IP")){
                result.ip = props[i].substring(3);
            }else{
                devProperties += props[i] + "\n";
            }
        }

        if(devProperties.length() > 0) {
            devProperties = devProperties.substring(0, devProperties.length() - 3);
        }

        Map<String, String> escapeSequences = new HashMap<String, String>(){{
            put("\\u017e", "ž");
            put("\\u017d", "Ž");
            put("\\u0161", "š");
            put("\\u0160", "Š");
            put("\\u010d", "č");
            put("\\u010c", "Č");
            put("\\u0107", "ć");
            put("\\u0106", "Ć");
            put("\\u0111", "đ");
            put("\\u0110", "Đ");
        }};

        Iterator it = escapeSequences.entrySet().iterator();
        while (it.hasNext()) {
            Map.Entry pair = (Map.Entry)it.next();

            devProperties = devProperties.replace(pair.getKey().toString(), pair.getValue().toString());

            it.remove(); // avoids a ConcurrentModificationException
        }

        result.properties = devProperties;

        return result;
    }

    /* Turn all devices off. Sends a zero to control url of each device */
    private void allDevsOff(){
        int i, j;
        for(i=0; i<adapter.getCount(); i++){

            DeviceData item = adapter.getItem(i);

            String[] props = item.properties.split("\n");

            String urlStr = "http://" + item.ip;

            for(j = 0; j < props.length; j++){
                String[] prIdVal = props[j].split(":");

                if(prIdVal[0].equals("CTRL_URL")){
                    urlStr += prIdVal[1] + "-2000";
                    break;
                }
            }

            URL url;
            HttpURLConnection urlConnection = null;
            try {
                url = new URL(urlStr);

                urlConnection = (HttpURLConnection) url.openConnection();

                InputStream in = urlConnection.getInputStream();
                InputStreamReader isw = new InputStreamReader(in);

                String urlResp = "";
                int data = isw.read();
                while (data != -1) {
                    urlResp += (char) data;
                    data = isw.read();
                }
                Log.d("MAIN_allDevsOff", urlResp);

            } catch (Exception e) {
                Log.e("MAIN_allDevsOff", e.getMessage());
            } finally {
                if (urlConnection != null) {
                    urlConnection.disconnect();
                }
            }
        }

        refreshDevs(); /* To update current state */
    }

    Runnable deviceResponseProcessor = new Runnable() {
        @Override
        public void run() {
            ArrayList<String> responseList = comms.getResponseList();

            for(int i = 0; i < responseList.size(); i++){
                String message = responseList.get(i).replace("{", "").replace("}", "").replace(",", "\n").replace("\"", "");

                DeviceData data = parseMResponseMessage(message);
                data.label = readLabel(data.mac);

                adapter.put(data);

                Log.d("MAIN_viewRefresher", message);
            }

            adapter.notifyDataSetChanged();
        }
    };

    private void refreshDevs(){
//        Log.d("Main_refreshDevs", "Refreshing device list");

        adapter.clear();

        comms.discoverDevices();

        deviceProcessorHandler.postDelayed(deviceResponseProcessor, 500);
    }

    private Boolean CheckWiFiAvailable() {
        WifiManager manager = (WifiManager) getApplicationContext().getSystemService(Context.WIFI_SERVICE);
        if (manager.isWifiEnabled()) {
            WifiInfo wifiInfo = manager.getConnectionInfo();
            if (wifiInfo != null && wifiInfo.getSupplicantState() == SupplicantState.COMPLETED) {
                statusText.setText( String.format(getResources().getString(R.string.connected_to), wifiInfo.getSSID()));
                refreshDevs();
                return true;
            }
        }
        statusText.setText( getResources().getString(R.string.no_wifi_connection));
        return false;
    }


    Runnable WiFiStatusChecker = new Runnable() {
        @Override
        public void run() {
            if(!CheckWiFiAvailable()){
                wifiRefreshHandler.postDelayed(WiFiStatusChecker, 2000);
            }
        }
    };


    private void saveLabel(String MAC, String label){
        SharedPreferences prefs = getSharedPreferences("srv_prefs", this.MODE_PRIVATE);
        SharedPreferences.Editor editor = prefs.edit();

        if(label.length() < 3){
            editor.remove(MAC);
        }else {
            editor.putString(MAC, label);
        }

        editor.apply();
    }

    public String readLabel(String MAC) {
        SharedPreferences prefs = getSharedPreferences("srv_prefs", this.MODE_PRIVATE);
        return prefs.getString(MAC, "");
    }
}
