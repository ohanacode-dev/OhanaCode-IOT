/* 
 *  Author: Rada Berar
 *  email: ujagaga@gmail.com
 *  
 *  HTTP server which generates the web browser pages. Scrool to the bottom to see all the links contained in this HTTP_init routine.
 */
 
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <ESP8266HTTPClient.h>
#include <pgmspace.h>
#include "logo.h"
#include "http.h"
#include "udp_ping.h"
#include "wifi_connection.h"
#include "web_socket.h"
#include "config.h"
#include "LED_lamp_RGB.h"
#include "led.h"
#include "ota.h"

/* If we were writing HTML files, this would be the content. Here we use char arrays. */
static const char HTML_BEGIN[] PROGMEM = R"(
<!DOCTYPE HTML>
<html>
  <head>
    <meta name = "viewport" content = "width = device-width, initial-scale = 1.0, maximum-scale = 1.0, user-scalable=0">
    <title>Ujagaga WiFi RGB LED Control</title>
    <style>
      body { background-color: white; font-family: Arial, Helvetica, Sans-Serif; Color: #000000; }
      .contain{width: 100%;}
      .center_div{margin:0 auto; max-width: 400px;position:relative;}
    </style>
  </head>
  <body>
)";

static const char HTML_END[] PROGMEM = "</body></html>";

static const char INDEX_HTML_0[] PROGMEM = R"(
<style>
.btn_b{border:0;border-radius:0.3rem;color:#fff;line-height:2.4rem;font-size:1.2rem;margin:2%;height:2.4rem;background-color:#1fa3ec;}
.btn_cfg{border:0;border-radius:0.3rem;color:#fff;line-height:1.4rem;font-size:0.8rem;margin:1ch;height:2rem;width:15ch;background-color:#ff3300;}
h1{text-align: center;font-size:1.5rem;}
.slider{-webkit-appearance:none;appearance:none;width:90%;height:0.8em;border-radius:0.4em;background:#d3d3d3;outline:none;-webkit-transition:.2s;margin:10% 4%;padding:0;}
.slider::-webkit-slider-thumb{-webkit-appearance:none;appearance:none;width:3em;height:3em;background:white;border-radius:50%;box-shadow:inset -1em -1em 2em rgba(0,0,0,.5);}
.slider::-moz-range-thumb{appearance:none;width:3em;height:3em;background:white;border-radius:50%;box-shadow: inset -1em -1em 2em rgba(0,0,0,.5);}
.kolor{width:18%;margin:1%;padding-top:17%;float:left;border-radius:50%;}
.row{width:100%;overflow: auto;}
#cpick{width:100%;overflow: auto;}
#samp{margin:1em auto;background:black;padding:0;overflow:hidden;width:102px;height:52px;border-radius:2em;border:2px solid black;}
#sampc{margin:auto;padding:0;overflow:hidden;width:102px;text-align:center;}
input[type="color"]{height:70px;width:150px;margin-left:-20px;margin-top:-10px;} 
</style>
<div class="contain">
  <div class="center_div">
)";

const char INDEX_HTML_1[] PROGMEM = R"(
    <h1>Ujagaga WiFi RGB LED Control</h1>      
    <div id='cpick'></div>  
    <div class="row">  
        <div id='samp'>        
          <div id='sampc'>
            <input type="color" id='choose' onchange="setColor(this.value);"> 
          </div>          
        </div>
    </div> 
    <input type="range" min="0" max="8" value="8" class="slider" id="opac">
  </div>
  <hr>
  <p id='status'></p>  
  <br><button class="btn_cfg" type="button" onclick="location.href='/selectap';">Configure wifi</button><br/>
</div>
<script>
var colArr = ["#1abc9c","#2ecc71","#0000FF","#9b59b6","#34495e","#00FF00",
"#27ae60","#2980b9","#8e44ad","#2c3e50","#f1c40f","#e67e22","#e74c3c",
"#ecf0f1","#95a5a6","#f39c12","#d35400","#FF0000","#bdc3c7","#7f8c8d"];

var cpDiv = document.getElementById('cpick');
var opacity = document.getElementById('opac');
var sample = document.getElementById('sampc');
var choose = document.getElementById('choose');
var p = [255, 255, 255];
var cn=new WebSocket('ws://'+location.hostname+':81/');

for(var i=0;i<colArr.length;i++){
cDiv = document.createElement('div');
cDiv.className = "kolor";
cDiv.id = i;
cDiv.style.background = colArr[i];
cDiv.onclick = function() { setColor(colArr[this.id]); };
cpDiv.append(cDiv);
}

opacity.addEventListener('change', function(e){
setOpacity(opacity.value*12.5);
}); 

cn.onopen=function(){
cn.send('{"CURRENT":"","STATUS":""}');    
};
cn.onmessage=function(e){
var data=JSON.parse(e.data);
if(data.hasOwnProperty('CURRENT')){       
  opacity.value = data.CURRENT.OPA*0.08;  
  sample.style.opacity = opacity.value*0.125;
  choose.value = data.CURRENT.CLR;
}
if(data.hasOwnProperty('STATUS')){       
document.getElementById('status').innerHTML=data.STATUS;       
}}

function setOpacity(lev){  
  cn.send('{"CURRENT":{OPA:' + lev + '}}');
}
function setColor(clr){  
  cn.send('{"CURRENT":{CLR:"' + clr + '"}}');
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
        <br><button type='submit'>save</button>        
      </form>      
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

static void startOtaUpdate(void){
  String statusMsg = "{\"STATUS\":\"Starting OTA update...\"}";               
  WS_ServerBroadcast(statusMsg); 
   
  OTA_init();    
}

String HTTP_getFeatures( void ){
  Serial.println("returnFeatures");
  String response = "{\"MAC\":\"";
  response += WiFi.macAddress();
  response += ":";
  response += DEV_ID;
  response += "\",\"MODEL\":\"ujagaga WiFi LED\"";
  response += ",\"CURRENT\":";
  response += String(LED_getCurrentVal());  
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

/* Setup links to which device will respond to and routines which will send response. */
void HTTP_init(void){
 
  webServer.on("/", showStartPage);
  webServer.on("/id", showID);
  webServer.on("/favicon.ico", showNotFound);
  webServer.on("/selectap", selectAP);
  webServer.on("/wifisave", saveWiFi);
  webServer.on("/start_ota_update", startOtaUpdate);
  webServer.onNotFound(showStartPage);
  
  webServer.begin();
}
