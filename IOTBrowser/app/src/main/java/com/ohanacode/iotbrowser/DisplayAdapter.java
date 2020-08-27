package com.ohanacode.iotbrowser;

import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;


public class DisplayAdapter extends BaseAdapter {
    private ArrayList<String> mKeyList = new ArrayList<>();
    /* MAC, {IP, label, props} */
    private Map<String, DeviceData> mPropertyMap =  new HashMap<>();

    public DisplayAdapter() {
        mKeyList.clear();
        mPropertyMap.clear();
    }

    public void put(DeviceData data) {
        if(!mPropertyMap.containsKey(data.mac)){
            mKeyList.add(data.mac);
        }

        mPropertyMap.put(data.mac, data);
    }

    public void clear(){
        mKeyList.clear();
        mPropertyMap.clear();
    }

    /* Extracts current value from device data string */
    private String getCurrent(String devData){
        String retVal = "";
        int i;
        String[] props = devData.split("\n");

        for(i = 0; i < props.length; i++){
            String[] values = props[i].split(":");

            if(values[0].equals("CURRENT") && (values.length > 1)){
                retVal = values[1];
                break;
            }
        }

        while(retVal.length() < 3){
            retVal += " ";
        }

        return retVal;
    }

    @Override
    public int getCount() {
        return mPropertyMap.size();
    }

    @Override
    public DeviceData getItem(int position) {
        String key = mKeyList.get(position);

        return mPropertyMap.get(key);
    }

    @Override
    public long getItemId(int position) {
        // TODO implement you own logic with ID
        return 0;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        final View result;

        if (convertView == null) {
            result = LayoutInflater.from(parent.getContext()).inflate(R.layout.my_adapter_item, parent, false);
        } else {
            result = convertView;
        }

        DeviceData item = getItem(position);
        String ip = "IP:" + item.ip;
        String mac = "MAC:" + item.mac;
        String label;

        Log.i("PROPS", item.properties);

        if(item.label.length() > 2){
            label = item.label;
            ((TextView) result.findViewById(R.id.text_value)).setText(getCurrent(item.properties));
        }else{
            label = item.properties;
            ((TextView) result.findViewById(R.id.text_value)).setText("");
        }

        ((TextView) result.findViewById(R.id.text_title)).setText(label);
        ((TextView) result.findViewById(R.id.text_mac)).setText(mac);
        ((TextView) result.findViewById(R.id.text_ip)).setText(ip);

        return result;
    }
}

