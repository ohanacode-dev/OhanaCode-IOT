<<<<<<< HEAD:IOTBrowser/app/src/main/java/com/ohanacode/iotbrowser/DeviceData.java
package com.ohanacode.iotbrowser;
=======
package com.ohanacode.ociotbrowser;
>>>>>>> New IOT app version:OC-IOT-Browser/app/src/main/java/com/ohanacode/ociotbrowser/DeviceData.java

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
