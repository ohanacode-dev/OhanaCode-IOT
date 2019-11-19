package com.ohanacode-dev.oc-iot-browser;

public class DeviceData {
    public String ip;
    public String mac;
    public String properties;
    public String label;

    public DeviceData(){
        ip = "";
        mac = "";
        properties = "";
        label = "";
    }

    public DeviceData(DeviceData data){
        ip = data.ip;
        mac = data.mac;
        properties = data.properties;
        label = data.label;
    }

}
