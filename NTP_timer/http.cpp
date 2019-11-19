#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <ESP8266HTTPClient.h>
#include <pgmspace.h>
#include "logo.h"
#include "http.h"
#include "udp.h"
#include "wifi_connection.h"
#include "web_socket.h"
#include "config.h"
#include "NTP_timer.h"
#include "ntp.h"

static const char HTML_BEGIN[] PROGMEM = R"(
<!DOCTYPE HTML>
<html>
  <head>
    <meta name = "viewport" content = "width = device-width, initial-scale = 1.0, maximum-scale = 1.0, user-scalable=0">
    <title>Ujagaga WiFi LED Control</title>
    <style>
      body { background-color: white; font-family: Arial, Helvetica, Sans-Serif; Color: #000000; }
      .contain{width: 100%;}.center_div{margin:0 auto; max-width: 400px;position:relative;}
    </style>
  </head>
  <body>
)";

static const char HTML_END[] PROGMEM = "</body></html>";

static const char INDEX_HTML_0[] PROGMEM = R"(
<style>
  .btn_b{border:0;border-radius:0.3rem;color:#fff;line-height:2.4rem;font-size:1.2rem;margin:2%;height:2.4rem;background-color:#1fa3ec;}
  .btn1{width:25%;}.btn2{width:85%;}
  .btn_cfg{border:0;border-radius:0.3rem;color:#fff;line-height:1.4rem;font-size:0.8rem;margin:1ch;height:2rem;width:15ch;background-color:#ff3300;}
  h1{text-align: center;font-size:1.5rem;}
  #time{font-size:6rem;color:#1fa3ec;}
</style>
<div class="contain">
  <div class="center_div">
)";

// TODO: add timezone adjusting
const char INDEX_HTML_1[] PROGMEM = R"(
    <h1>Ujagaga WiFi NTP clock</h1>   
    <p id="time">--:--:--</p>
    <br>
    <select id="tz" onchange="setZone();">
  <option value="-12">(GMT-12:00) International Date Line West</option>
  <option value="-11">(GMT-11:00) Midway Island, Samoa</option>
  <option value="-10">(GMT-10:00) Hawaii</option>
  <option value="-9">(GMT-09:00) Alaska</option>
  <option value="-8">(GMT-08:00) Pacific Time (US & Canada)</option>
  <option value="-8">(GMT-08:00) Tijuana, Baja California</option>
  <option value="-7">(GMT-07:00) Arizona</option>
  <option value="-7">(GMT-07:00) Chihuahua, La Paz, Mazatlan</option>
  <option value="-7">(GMT-07:00) Mountain Time (US & Canada)</option>
  <option value="-6">(GMT-06:00) Central America</option>
  <option value="-6">(GMT-06:00) Central Time (US & Canada)</option>
  <option value="-6">(GMT-06:00) Guadalajara, Mexico City, Monterrey</option>
  <option value="-6">(GMT-06:00) Saskatchewan</option>
  <option value="-5">(GMT-05:00) Bogota, Lima, Quito, Rio Branco</option>
  <option value="-5">(GMT-05:00) Eastern Time (US & Canada)</option>
  <option value="-5">(GMT-05:00) Indiana (East)</option>
  <option value="-4">(GMT-04:00) Atlantic Time (Canada)</option>
  <option value="-4">(GMT-04:00) Caracas, La Paz</option>
  <option value="-4">(GMT-04:00) Manaus</option>
  <option value="-4">(GMT-04:00) Santiago</option>
  <option value="-3.5">(GMT-03:30) Newfoundland</option>
  <option value="-3">(GMT-03:00) Brasilia</option>
  <option value="-3">(GMT-03:00) Buenos Aires, Georgetown</option>
  <option value="-3">(GMT-03:00) Greenland</option>
  <option value="-3">(GMT-03:00) Montevideo</option>
  <option value="-2">(GMT-02:00) Mid-Atlantic</option>
  <option value="-1">(GMT-01:00) Cape Verde Is.</option>
  <option value="-1">(GMT-01:00) Azores</option>
  <option value="0">(GMT+00:00) Casablanca, Monrovia, Reykjavik</option>
  <option value="0">(GMT+00:00) Greenwich Mean Time : Dublin, Edinburgh, Lisbon, London</option>
  <option value="1">(GMT+01:00) Amsterdam, Berlin, Bern, Rome, Stockholm, Vienna</option>
  <option value="1">(GMT+01:00) Belgrade, Bratislava, Budapest, Ljubljana, Prague</option>
  <option value="1">(GMT+01:00) Brussels, Copenhagen, Madrid, Paris</option>
  <option value="1">(GMT+01:00) Sarajevo, Skopje, Warsaw, Zagreb</option>
  <option value="1">(GMT+01:00) West Central Africa</option>
  <option value="2">(GMT+02:00) Amman</option>
  <option value="2">(GMT+02:00) Athens, Bucharest, Istanbul</option>
  <option value="2">(GMT+02:00) Beirut</option>
  <option value="2">(GMT+02:00) Cairo</option>
  <option value="2">(GMT+02:00) Harare, Pretoria</option>
  <option value="2">(GMT+02:00) Helsinki, Kyiv, Riga, Sofia, Tallinn, Vilnius</option>
  <option value="2">(GMT+02:00) Jerusalem</option>
  <option value="2">(GMT+02:00) Minsk</option>
  <option value="2">(GMT+02:00) Windhoek</option>
  <option value="3">(GMT+03:00) Kuwait, Riyadh, Baghdad</option>
  <option value="3">(GMT+03:00) Moscow, St. Petersburg, Volgograd</option>
  <option value="3">(GMT+03:00) Nairobi</option>
  <option value="3">(GMT+03:00) Tbilisi</option>
  <option value="3.5">(GMT+03:30) Tehran</option>
  <option value="4">(GMT+04:00) Abu Dhabi, Muscat</option>
  <option value="4">(GMT+04:00) Baku</option>
  <option value="4">(GMT+04:00) Yerevan</option>
  <option value="4.5">(GMT+04:30) Kabul</option>
  <option value="5">(GMT+05:00) Yekaterinburg</option>
  <option value="5">(GMT+05:00) Islamabad, Karachi, Tashkent</option>
  <option value="5.5">(GMT+05:30) Sri Jayawardenapura</option>
  <option value="5.5">(GMT+05:30) Chennai, Kolkata, Mumbai, New Delhi</option>
  <option value="5.75">(GMT+05:45) Kathmandu</option>
  <option value="6">(GMT+06:00) Almaty, Novosibirsk</option>
  <option value="6">(GMT+06:00) Astana, Dhaka</option>
  <option value="6.5">(GMT+06:30) Yangon (Rangoon)</option>
  <option value="7">(GMT+07:00) Bangkok, Hanoi, Jakarta</option>
  <option value="7">(GMT+07:00) Krasnoyarsk</option>
  <option value="8">(GMT+08:00) Beijing, Chongqing, Hong Kong, Urumqi</option>
  <option value="8">(GMT+08:00) Kuala Lumpur, Singapore</option>
  <option value="8">(GMT+08:00) Irkutsk, Ulaan Bataar</option>
  <option value="8">(GMT+08:00) Perth</option>
  <option value="8">(GMT+08:00) Taipei</option>
  <option value="9">(GMT+09:00) Osaka, Sapporo, Tokyo</option>
  <option value="9">(GMT+09:00) Seoul</option>
  <option value="9">(GMT+09:00) Yakutsk</option>
  <option value="9.5">(GMT+09:30) Adelaide</option>
  <option value="9.5">(GMT+09:30) Darwin</option>
  <option value="10">(GMT+10:00) Brisbane</option>
  <option value="10">(GMT+10:00) Canberra, Melbourne, Sydney</option>
  <option value="10">(GMT+10:00) Hobart</option>
  <option value="10">(GMT+10:00) Guam, Port Moresby</option>
  <option value="10">(GMT+10:00) Vladivostok</option>
  <option value="11">(GMT+11:00) Magadan, Solomon Is., New Caledonia</option>
  <option value="12">(GMT+12:00) Auckland, Wellington</option>
  <option value="12">(GMT+12:00) Fiji, Kamchatka, Marshall Is.</option>
  <option value="13">(GMT+13:00) Nuku'alofa</option>
    </select>
    <input type="checkbox" id="ds" onclick="setDs() ">Use daylight savings<br>
  </div>
  <hr>
  <p id='status'></p>  
  <br><button class="btn_cfg" type="button" onclick="location.href='/selectap';">Configure wifi</button><br/>
</div>
<script>
  var cn=new WebSocket('ws://'+location.hostname+':81/');
  cn.onopen=function(){
    cn.send('{"CURRENT":"","STATUS":"","TIMEZONE":"","DLSAVE":""}');    
  };
  cn.onmessage=function(e){
    var data=JSON.parse(e.data);
    console.log(data);
    if(data.hasOwnProperty('CURRENT')){       
      document.getElementById('time').innerHTML=data.CURRENT;       
    } 
    if(data.hasOwnProperty('STATUS')){       
      document.getElementById('status').innerHTML=data.STATUS;       
    }
    if(data.hasOwnProperty('TIMEZONE')){       
      document.getElementById('tz').value=data.TIMEZONE;       
    }
    if(data.hasOwnProperty('DLSAVE')){       
      document.getElementById('ds').checked=data.DLSAVE;       
    }
  }; 
  function setZone(){
    var e = document.getElementById("tz");
    var tz = e.options[e.selectedIndex].value;
    var dls = e.options[e.selectedIndex].getAttribute('dlSave');
    cn.send('{"TIMEZONE":' + tz + ', "DLSAVE":' + dls + '}');
  }  
  function setDs(){
    var e = document.getElementById("ds");
    if(e.checked == true){
      cn.send('{"DLSAVE":1}');
    }else{
      cn.send('{"DLSAVE":0}');  
    }
  }  
  
</script>
)";

static const char APLIST_HTML_0[] PROGMEM = R"(
<style>
  .c{text-align: center;}
  div,input{padding:5px;font-size:1em;}
  input{width:95%;}
  body{text-align: left;}
  button{width:100%;border:0;border-radius:0.3rem;color:#fff;line-height:2.4rem;font-size:1.2rem;height:40px;background-color:#1fa3ec;}
  .q{float: right;width: 64px;text-align: right;}
  .radio{width:2em;}
  #vm{width:100%;height:50vh;overflow-y:auto;margin-bottom:1em;}
</style>
</head><body>  
  <div class="contain">
    <div class="center_div">
)";    

/* Placeholder for the wifi list */
static const char APLIST_HTML_1[] PROGMEM = R"(   
      <h1 id='ttl'>Networks found:</h1>
      <div id='vm'>
)";  

static const char APLIST_HTML_2[] PROGMEM = R"( 
      </div>
      <form method='get' action='wifisave'>
        <button type='button' onclick='refresh();'>Rescan</button><br/><br/>
        <input id='s' name='s' length=32 placeholder='SSID (Leave blank for AP mode)'><br>      
        <input id='p' name='p' length=32 placeholder='password'><br>
        <input id='a' name='a' length=16 placeholder='static IP address (optional)'><br>       
        <br><button type='submit'>save</button></form>
     </div>
  </div>
<script>
  function c(l){
    document.getElementById('s').value=l.innerText||l.textContent;
    document.getElementById('p').focus();
  }
  
  var cn=new WebSocket('ws://'+location.hostname+':81/');
  cn.onopen=function(){
    cn.send('{"APLIST":""}');
  }
  cn.onmessage=function(e){
    var data=JSON.parse(e.data);
    if(data.hasOwnProperty('APLIST')){
      rsp=data.APLIST.split('|');
      document.getElementById('vm').innerHTML='';
      for(var i=0;i<(rsp.length);i++){
        document.getElementById('vm').innerHTML+='<span>'+(i+1)+": </span><a href='#p' onclick='c(this)'>" + rsp[i] + '</a><br>';
      }
      if(!document.getElementById('vm').innerHTML.replace(/\\s/g,'').length){
        document.getElementById('ttl').innerHTML='No networks found.'
      } 
    }
  };
  function refresh(){
    document.getElementById('vm').innerHTML='Please wait...'
    cn.send('{"APLIST":""}');
  }
</script>
)";

static const char REDIRECT_HTML[] PROGMEM = R"(
<p id="tmr"></p>
<script>
  var c=6;   
  function count(){
    var tmr=document.getElementById('tmr');   
    if(c>0){
      c--;
      tmr.innerHTML="You will be redirected to home page in "+c+" seconds.";
      setTimeout('count()',1000);
    }else{
      window.location.href="/";
    }
  }
  count();
</script> 
)";

/* Declaring a web server object. */
static ESP8266WebServer webServer(80);

static void showNotFound(void){
  webServer.send(404, "text/html; charset=iso-8859-1","<html><head> <title>404 Not Found</title></head><body><h1>Not Found</h1></body></html>");
}

String HTTP_getFeatures( void ){
  Serial.println("returnFeatures");
  String response = "{\"MAC\":\"";
  response += WiFi.macAddress();
  response += ":";
  response += DEV_ID;
  response += "\",\"MODEL\":\"ujagaga WiFi LED\"";
  response += ",\"CURRENT\":";
  response += NTP_getTimeString();  
  response += "}";  
  return response;     
}

void showID( void ) {    
  Serial.println("show ID");
  webServer.send(200, "text/plain", HTTP_getFeatures());  
}

void showStartPage() {    
  Serial.println("showStartPage");
  String response = FPSTR(HTML_BEGIN);
  response += FPSTR(INDEX_HTML_0);
  response += FPSTR(LOGO);
  response += FPSTR(INDEX_HTML_1); 
  response += FPSTR(HTML_END);
  webServer.send(200, "text/html", response);  
}

static void showStatusPage(bool goToHome = false) {    
  Serial.println("showStatusPage");
  String response = FPSTR(HTML_BEGIN);
  response += "<h1>Connection Status</h1><p>";
  response += MAIN_getStatusMsg() + "</p>";
  if(goToHome){
    /* Add redirect timer. */
    response += FPSTR(REDIRECT_HTML);
  }
  response += FPSTR(HTML_END);
  webServer.send(200, "text/html", response);   
}

/* Sends response to http request for configuring AP to connect to */
static void selectAP(void) {   
  Serial.println("selectAP");
  String response = FPSTR(HTML_BEGIN);
  response += FPSTR(APLIST_HTML_0);  
  response += FPSTR(LOGO);
  response += FPSTR(APLIST_HTML_1);
  response += "Please wait...";  
  response += FPSTR(APLIST_HTML_2);   
  response += FPSTR(HTML_END);
  webServer.send(200, "text/html", response);  
}

extern bool reset_flag;

/* Saves wifi settings to EEPROM */
static void saveWiFi(void){
  String ssid = webServer.arg("s");
  String pass = webServer.arg("p");
  String ipaddr = webServer.arg("a");
  
  if((ssid.length() > 63) || (pass.length() > 63)){
      MAIN_setStatusMsg("Sorry, this module can only remember SSID and a PASSWORD up to 63 bytes long.");
      showStatusPage(); 
      return;
  } 

  IPAddress newStationIP;
  newStationIP.fromString(ipaddr);

  String st_ssid = "";
  String st_pass = "";
  IPAddress stationIP;

  if(ssid.length() > 0){
    bool cmpFlag = true;

    st_ssid = WIFIC_getStSSID();
    st_pass = WIFIC_getStPass();
    stationIP = WIFIC_getStIP();

    if(!st_ssid.equals(ssid) || !st_pass.equals(pass)){
      cmpFlag = false;
    }
      
    if(cmpFlag){
       if((newStationIP[0] != stationIP[0]) || (newStationIP[1] != stationIP[1]) || (newStationIP[2] != stationIP[2]) || (newStationIP[3] != stationIP[3])){
          cmpFlag = false;
       }
    }
  
    if(cmpFlag){
      MAIN_setStatusMsg("All parameters are already set as requested.");
      showStatusPage(true);      
      return;
    }   
  }
  
  WIFIC_setStSSID(ssid);
  WIFIC_setStPass(pass);
  WIFIC_setStIP(newStationIP);

  String http_statusMessage;
  
  if(ssid.length() > 3){    
    http_statusMessage = "Saving settings and connecting to SSID: ";
    http_statusMessage += ssid; 
    http_statusMessage += " ,IP: ";
    
    if(WIFIC_checkValidIp(newStationIP)){      
      http_statusMessage += ipaddr;
    }else{
      http_statusMessage += "dynamically assigned by DHCP";
    }
    
  }else{       
    http_statusMessage = "Saving settings and switching to AP mode only.";    
  }
  http_statusMessage += "<br>If you can not connect to this device 20 seconds from now, please, reset it.";

  MAIN_setStatusMsg(http_statusMessage);
  showStatusPage();

  volatile int i;

  /* Keep serving http to display the status page*/
  for(i = 0; i < 100000; i++){
    webServer.handleClient(); 
    ESP.wdtFeed();
  } 

  /* WiFI config changed. Restart to apply. 
   Note: ESP.restart is buggy after programming the chip. 
   Just reset once after programming to get stable results. */
  ESP.restart();
}

void HTTP_process(void){
  webServer.handleClient(); 
}

void HTTP_init(void){
 
  webServer.on("/", showStartPage);
  webServer.on("/id", showID);
  webServer.on("/favicon.ico", showNotFound);
  webServer.on("/selectap", selectAP);
  webServer.on("/wifisave", saveWiFi);
  webServer.onNotFound(showStartPage);
  
  webServer.begin();
}
