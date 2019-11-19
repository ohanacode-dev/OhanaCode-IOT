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
#include "email_sender.h"

static const char HTML_BEGIN[] PROGMEM = R"(
<!DOCTYPE HTML>
<html>
  <head>
    <meta name = "viewport" content = "width = device-width, initial-scale = 1.0, maximum-scale = 1.0, user-scalable=0">
    <title>Ujagaga IOT Router</title>
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
  .btn_cfg{border:0;border-radius:0.3rem;color:#fff;line-height:1.4rem;font-size:0.8rem;margin:1ch;height:2rem;width:30ch;background-color:#ff3300;}
  h1{text-align: center;font-size:1.5rem;}
  h2{text-align: center;font-size:1.2rem;}
  #m{-webkit-transform:rotate(-90deg);-moz-transform:rotate(-90deg);-o-transform:rotate(-90deg);transform:rotate(-90deg);
  position:absolute;right:-7.5rem;bottom:8rem; width:16rem;height:5%;}
  #temp{text-align: center;font-size:9em;padding: 0; margin:0;font-weight:bold;color:#1fa3ec;}
</style>
<div class="contain">
  <div class="center_div">
)";

static const char INDEX_HTML_1[] PROGMEM = R"(
    <h1>Ujagaga IOT Router</h1> 
    <span id="devl"></span> 
  </div>
)";

static const char INDEX_HTML_2[] PROGMEM = R"(
  <hr>
  <p id='status'></p>  
  <br><button class="btn_cfg" type="button" onclick="location.href='/selectap';">Configure wifi</button>
  <br><button class="btn_cfg" type="button" onclick="location.href='/cfgauth';">Configure credentials</button>
)";

static const char INDEX_HTML_3[] PROGMEM = R"(
</div>
<script>  
  var cn=new WebSocket('ws://'+location.hostname+':81/');
  cn.onopen=function(){
    cn.send('{"DEVICES":,"STATUS":}');    
  };
  cn.onmessage=function(e){
    try{
      var data=JSON.parse(e.data);
  
      if(data.hasOwnProperty('STATUS')){       
        document.getElementById('status').innerHTML=data.STATUS;       
      }
      if(data.hasOwnProperty('DEVICES')){       
        var devList = data.DEVICES.split('|');
        var html = "";
        for (i = 0; i < devList.length; i++) { 
          if(devList[i].length > 11){
            html += "<button class='btn_b btn2' type='button' onclick='location.href=\"/" + devList[i].replace(/:/g,"" ) + "/\";'>" + devList[i] + "</button><br/>";
          }
        }
        document.getElementById('devl').innerHTML=html;       
      }    
    }
    catch(err){
      console.log("Err:" + err + ":" + e.data);
    }
  }; 
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
        <input id='s' name='s' length=32 placeholder='SSID'><br>      
        <input id='p' name='p' length=32 placeholder='password'><br>
        <input id='a' name='a' length=16 placeholder='static IP address (x.x.x.x)' required><br>       
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

static const char AUTH_HTML_1[] PROGMEM = R"( 
        <h1>Change Credentials</h1>  

        <form method='post' action='saveauth'>
          <h2>Admin Login</h2>        
          <input name='u' length=32 placeholder='New user name' required><br>      
          <input name='p' length=32 placeholder='New password' required><br>            
        
          <br><h2>External IP</h2>
          <p>Use this url to check external IP</p><br>  
          <input name='url' length=32 placeholder='http://ident.me' required><br> 
          
          <br><h2>Setup Email</h2>
          <p>This will be used to send notification when external IP address changes.</p><br>            
          <input name='ss' length=32 placeholder='smtp server' required><br>      
          <input name='sp' length=32 placeholder='smtp port' required><br>
          <input name='sl' length=32 placeholder='login' required><br>
          <input name='spw' length=32 placeholder='password' required><br>
          <input name='st' length=32 placeholder='recipient' required><br>
          <input name='ssub' length=32 placeholder='subject' required><br>            
          <br><button type='submit'>save</button>
        </form>
     </div>
  </div>
)";


static const char LED_LAMP_HTML[] PROGMEM = R"(
    <h1>Ujagaga WiFi LED Control</h1>      
    <button class="btn_b btn2" type="button" onclick="setLevel(0);">OFF</button><br>
    <button class="btn_b btn1" type="button" onclick="setLevel(10);">1</button>
    <button class="btn_b btn1" type="button" onclick="setLevel(20);">2</button>
    <button class="btn_b btn1" type="button" onclick="setLevel(30);">3</button><br>
    <button class="btn_b btn1" type="button" onclick="setLevel(40);">4</button>
    <button class="btn_b btn1" type="button" onclick="setLevel(50);">5</button>
    <button class="btn_b btn1" type="button" onclick="setLevel(60);">6</button><br>
    <button class="btn_b btn1" type="button" onclick="setLevel(70);">7</button>
    <button class="btn_b btn1" type="button" onclick="setLevel(80);">8</button>
    <button class="btn_b btn1" type="button" onclick="setLevel(90);">9</button><br>
    <button class="btn_b btn2" type="button" onclick="setLevel(100);">ON</button>
    <meter id="m" value="" min="0" max="100"></meter> 
  </div> 
</div>
<script>
  var cn=new WebSocket('ws://'+location.hostname+':81/');
  cn.onopen=function(){
    cn.send('{"CURRENT":""}');    
  };
  cn.onmessage=function(e){
    var data=JSON.parse(e.data);
    if(data.hasOwnProperty('CURRENT')){       
      document.getElementById('m').value=parseInt(data.CURRENT);       
    }   
  };
  function setLevel(lev){  
    cn.send('{"CURRENT":' + lev + '}');
  }
</script>
)";

static const char THERMOMETER[] PROGMEM = R"( 
    <h1>Ujagaga WiFi Thermometer</h1>
    <p><span id="temp"></span><span style=\"font-size:2em;\">&#8451;</span></p><br>
  </div>
</div>
<script> 
  var cn=new WebSocket('ws://'+location.hostname+':81/');
  cn.onopen=function(){
    cn.send('{"CURRENT":""}');    
  };
  cn.onmessage=function(e){
    var data=JSON.parse(e.data);  
    if(data.hasOwnProperty('CURRENT')){ 
      document.getElementById('temp').innerHTML=data.CURRENT;     
    }  
  };  
</script>
)";

static const char THERMOSTAT03[] PROGMEM = R"( 
  <h1>Ujagaga WiFi Thermostat</h1>
  <p><span id="temp"></span><span style="font-size:2em;">&#8451;</span></p><br>
  <button class="btn_temp" id="btn_tgl" type="button" onclick="toggleState();">Activate</button><br>
  <div id="cfg_temp" class="contain">
    <p id='target'></p><br>
    <input id='num' type='number' step='0.1' min='-50' max='84'>
    <button class="btn_temp" type="button" onclick="setTarget();">Set</button><br>
  </div>  
</div>
<script>
  var cn=new WebSocket('ws://'+location.hostname+':81/');
  cn.onopen=function(){
    cn.send('{"CURRENT":""}');    
  };
  cn.onmessage=function(e){
    var data=JSON.parse(e.data);  
    if(data.hasOwnProperty('CURRENT')){ 
      document.getElementById('temp').innerHTML=data.CURRENT;
    } 
    if(data.hasOwnProperty('TARGET')){
      temp=parseFloat(data.TARGET);  
      var btn=document.getElementById('btn_tgl');
      if(temp<-1000){
        btn.innerHTML='Activate';
        btn.style.background='#efeae8';
        btn.style.color='black';
        document.getElementById('cfg_temp').style.height='0';
      }else{
        btn.innerHTML='Deactivate';
        btn.style.background='#f7b34f';
        btn.style.color='white';
        document.getElementById('cfg_temp').style.height='8em';
        document.getElementById('target').innerHTML='Target temperature: '+data.TARGET;
        document.getElementById('num').value=temp;
      }       
    }
  };
  function setTarget(){
    var number=document.getElementById('num');
    if(number.value>84){
      number.value=84;
    };
    if(number.value<-50){
      number.value=-50;
    };
    cn.send('{"TARGET":' + number.value + '}');
  }
  function toggleState(){
    if(document.getElementById('btn_tgl').innerHTML=='Activate'){
      cn.send('{"TARGET":2000}');
    }else{
      cn.send('{"TARGET":-2000}');
    }        
  }
</script>
)";

static const char THERMOSTAT04[] PROGMEM = R"( 
  <h1>Ujagaga WiFi Thermostat</h1>
  <p><span id="temp"></span><span style="font-size:2em;">&#8451;</span></p><br>
  <button class="btn_temp" id="btn_tgl" type="button" onclick="toggleState();">Activate</button><br>
  <div id="cfg_temp" class="contain">
    <p id='target'></p><br>
    <input id='num' type='number' step='0.5' min='0' max='1000'>"
    <button class="btn_temp" type="button" onclick="setTarget();">Set</button><br>
  </div> 
</div>
<script>
  var cn=new WebSocket('ws://'+location.hostname+':81/');
  cn.onopen=function(){
    cn.send('{"CURRENT":}');    
  };
  cn.onmessage=function(e){
    var data=JSON.parse(e.data);  
    if(data.hasOwnProperty('CURRENT')){ 
      document.getElementById('temp').innerHTML=data.CURRENT;"
    } 
    if(data.hasOwnProperty('TARGET')){
      temp=parseFloat(data.TARGET);  
      var btn=document.getElementById('btn_tgl');
      if(temp<-1000){
        btn.innerHTML='Activate';
        btn.style.background='#efeae8';
        btn.style.color='black';
        document.getElementById('cfg_temp').style.height='0';
      }else{
        btn.innerHTML='Deactivate';
        btn.style.background='#f7b34f';
        btn.style.color='white';
        document.getElementById('cfg_temp').style.height='8em';
        document.getElementById('target').innerHTML='Target temperature: '+data.TARGET;
        document.getElementById('num').value=temp;
      }       
    }
  };
  function setTarget(){
    var number=document.getElementById('num');
    if(number.value>1000){
      number.value=1000;
    };
    if(number.value<0){
      number.value=0;
    };
    cn.send('{"TARGET":' + number.value + '}');
  }
  function toggleState(){
    if(document.getElementById('btn_tgl').innerHTML=='Activate'){
      cn.send('{"TARGET":2000}');
    }else{
      cn.send('{"TARGET":-2000}');
    }        
  }
</script>
)";

static char usr_name[USR_NAME_SIZE];    
static char usr_pass[USR_PASS_SIZE];  
static ESP8266WebServer webServer(80);
static String http_statusMessage = "";   
static String ipIdentUrl = "http://ident.me";   /* TODO: create config page for this */  
/* TODO: create config page for email sending */

String HTTP_getExternalIP(void){
  String result = "";
  
  HTTPClient http;
  http.begin(ipIdentUrl);
  int httpCode = http.GET();
  
  if (httpCode == HTTP_CODE_OK) {  
    result = http.getString();              
  }else{
    result = "";
  }
  
  http.end();
  
  return result;
}

static void showNotFound(void){
  webServer.send(404, "text/html; charset=iso-8859-1","<html><head> <title>404 Not Found</title></head><body><h1>Not Found</h1></body></html>");
}

static void showLoginPage(void) { 
  
  if(!webServer.authenticate(usr_name, usr_pass)){
    Serial.print("Expecting user:");
    Serial.print(usr_name);
    Serial.print("  pass: ");
    Serial.println(usr_pass);
    UDP_devCountClear(); 
    UDP_pingDevices();   
    return webServer.requestAuthentication();
  }

  WiFiClient client = webServer.client();  
  IPAddress clientIP = client.remoteIP();
    
  String response = FPSTR(HTML_BEGIN);
  response += FPSTR(INDEX_HTML_0);
  response += FPSTR(LOGO);
  response += FPSTR(INDEX_HTML_1);
  
  if( WIFIC_IPIsLocal(clientIP)){
    response += FPSTR(INDEX_HTML_2); 
  }
  
  response += FPSTR(INDEX_HTML_3); 
  response += FPSTR(HTML_END);
  webServer.send(200, "text/html", response);  

  UDP_pingDevices();
}

static void showStatusPage(bool goToHome = false) {    
  Serial.println("showStatusPage");
  String response = FPSTR(HTML_BEGIN);
  response += "<h1>Connection Status</h1><p>";
  response += http_statusMessage + "</p>";
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

static void configAuth(void) {   
  String response = FPSTR(HTML_BEGIN);
  response += FPSTR(APLIST_HTML_0);  
  response += FPSTR(LOGO);
  response += FPSTR(AUTH_HTML_1); 
  response += FPSTR(HTML_END);
  webServer.send(200, "text/html", response);  
}

static void routeToDevice(void){
  String uri = webServer.uri();
  //Serial.println("RCV:" + uri );

  if(uri.length() > 11){
    String deviceMAC = uri.substring(1, uri.indexOf('/', 1));

    int devId = UDP_getDevListId(deviceMAC);
    
    if(devId > -1){
      String devTypeStr = deviceMAC.substring(12); /* Last two characters for a 14 character string */
      int devType = devTypeStr.toInt();   
 
      /* Send http request to device and Forward response to client */          
      String response = FPSTR(HTML_BEGIN);
      response += FPSTR(INDEX_HTML_0);
      response += FPSTR(LOGO);
      
      switch(devType){
        case DEVT_LED_DIMMER_LAMP:                 
          response += FPSTR(LED_LAMP_HTML);       
          break;
        
        case DEVT_THERMOMETER:
          response += FPSTR(THERMOMETER);      
          break;

        case DEVT_THERMOSTAT03:
          response += FPSTR(THERMOSTAT03);      
          break;

        case DEVT_THERMOSTAT04:
          response += FPSTR(THERMOSTAT04);      
          break;
          
        default:
          showNotFound();
          return;
      }
             
      response += FPSTR(HTML_END);
      webServer.send(200, "text/html", response); 
          
      WS_connectAsWsClient(devId);
      
    }else{
      showNotFound();  
    }
  }  
}

/* Saves wifi settings to EEPROM */
static void saveWiFi(void){
  String ssid = webServer.arg("s");
  String pass = webServer.arg("p");
  String ipaddr = webServer.arg("a");
  
  if((ssid.length() > 63) || (pass.length() > 63)){
      http_statusMessage = "Sorry, this module can only remember SSID and a PASSWORD up to 63 bytes long.";
      showStatusPage(); 
      return;
  } 

  IPAddress newStationIP;

  if(!newStationIP.fromString(ipaddr)){
    http_statusMessage = "The specified IP address is not in expected format: xxx.xxx.xxx.xxx (i.e. 192.168.1.100). Please specify a valid IP address";
    showStatusPage(); 
    return;
  }

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
      http_statusMessage = "All parameters are already set as requested.";
      showStatusPage(true);      
      return;
    }   
  }
  
  WIFIC_setStSSID(ssid);
  WIFIC_setStPass(pass);
  WIFIC_setStIP(newStationIP);
    
  if(ssid.length() > 3){    
    http_statusMessage = "Saving settings and connecting to SSID: ";
    http_statusMessage += st_ssid; 
    http_statusMessage += " ,IP: ";
    http_statusMessage += ipaddr;
  }else{       
    http_statusMessage = "Saving settings and switching to AP mode only.";    
  }
  http_statusMessage += "<br>If you can not connect to this device 20 seconds from now, please, reset it.";
  showStatusPage();
     
  for(int i = 0; i < 1000; i++){
    webServer.handleClient(); 
    ESP.wdtFeed();
  }

  if(ssid.length() > 3){
    WIFIC_stationtMode(); 
  }else{    
    WIFIC_APMode();
  }
}

static void saveAuth(void){
  bool cmpFlag = true;
  String uname = webServer.arg("u");
  String pass = webServer.arg("p");
  String url = webServer.arg("url");

  smtp_setup reqData;
  reqData.port = webServer.arg("sp").toInt();
  reqData.smtp_server = webServer.arg("ss");
  reqData.login = webServer.arg("sl");
  reqData.passwd = webServer.arg("spw");
  reqData.to = webServer.arg("st");
  reqData.subject = webServer.arg("ssub");
   
  if((uname.length() > USR_NAME_SIZE) || (pass.length() > USR_PASS_SIZE)){
      http_statusMessage = "Sorry, this module can only remember USERNAME and a PASSWORD up to 32 bytes long.";
      showStatusPage(); 
      return;
  } 

  if(url.length() > IPCHK_URL_SIZE){
      http_statusMessage = "Sorry, this module can only remember url 64 bytes long.";
      showStatusPage(); 
      return;
  } 

  if( (reqData.smtp_server.length() > SMTP_SERVER_SIZE) || (reqData.login.length() > SMTP_LOGIN_SIZE) ||
      (reqData.passwd.length() > SMTP_PASS_SIZE) || (reqData.to.length() > SMTP_TO_SIZE) || 
      (reqData.subject.length() > SMTP_SUBJECT_SIZE)){
    http_statusMessage = "Sorry, this module can only remember smtp strings up to 32 bytes long.";
    showStatusPage(); 
    return;      
  }
  
  if(uname.length() > 0){  
    for(int i=0; i<uname.length(); i++){
      if(uname[i] != usr_name[i]){
        cmpFlag = false;
        break;
      }
    }
  
    if(cmpFlag){
      for(int i=0; i<pass.length(); i++){
        if(pass[i] != usr_pass[i]){
          cmpFlag = false;
          break;
        }       
      }    
    }    
  }

  if(url.length() > 0){
    if(!ipIdentUrl.equals(url)){
      cmpFlag = false;
    }
  }
  
  if(cmpFlag){
    smtp_setup data;
    EMAIL_getSmtpSetup(&data);

    if((data.port != reqData.port) || !data.smtp_server.equals(reqData.smtp_server) || \
    !data.login.equals(reqData.login) || !data.passwd.equals(reqData.passwd) || \
    !data.to.equals(reqData.to) || !data.subject.equals(reqData.subject)){
      cmpFlag = false;
    }    
  }  

  if(!cmpFlag){
    EEPROM.begin(EEPROM_SIZE);
    
    uint16_t addr;
    for(addr = 0; addr < pass.length(); addr++){
      EEPROM.write(addr + USR_PASS_EEPROM_ADDR, pass[addr]);
      usr_pass[addr] = pass[addr];
    }
    EEPROM.write(addr + USR_PASS_EEPROM_ADDR, 0);
    usr_pass[addr] = 0;
  
    for(addr = 0; addr < uname.length(); addr++){
      EEPROM.write(addr + USR_NAME_EEPROM_ADDR, uname[addr]);
      usr_name[addr] = uname[addr];
    }
    EEPROM.write(addr + USR_NAME_EEPROM_ADDR, 0);  
    usr_name[addr] = 0;

    for(addr = 0; addr < uname.length(); addr++){
      EEPROM.write(addr + IPCHK_URL_EEPROM_ADDR, ipIdentUrl[addr]);
      ipIdentUrl[addr] = url[addr];
    }
    EEPROM.write(addr + IPCHK_URL_EEPROM_ADDR, 0);  
    ipIdentUrl[addr] = 0;
     
    EEPROM.commit();
    EEPROM.end();

    EMAIL_config(&reqData);
  }

  if(cmpFlag){
    http_statusMessage = "All settings are already as requested.";    
  }else{
    http_statusMessage = "Saving changes.";    
  }  
  showStatusPage(true);
}

void HTTP_process(void){
  webServer.handleClient(); 
}

String HTTP_getStatus(void)
{
  return http_statusMessage;
}

void HTTP_init(void){
  /* Read authentication data from EEPROM */
  EEPROM.begin(EEPROM_SIZE);
  uint16_t i = 0;

  do{
    usr_pass[i] = EEPROM.read(i + USR_PASS_EEPROM_ADDR);
    if((usr_pass[i] < 32) || (usr_pass[i] > 126)){
      /* Non printable character */
      break;
    }
    i++;
  }while(i < USR_PASS_SIZE);
  usr_pass[i] = 0;

    i = 0;
  do{
    usr_name[i] = EEPROM.read(i + USR_NAME_EEPROM_ADDR);
    if((usr_name[i] < 32) || (usr_name[i] > 126)){
      /* Non printable character */
      break;
    }
    i++;
  }while(i < USR_NAME_SIZE);
  usr_name[i] = 0;
   
  EEPROM.end();  

  
  webServer.on("/", showLoginPage);
  webServer.on("/favicon.ico", showNotFound);
  webServer.on("/selectap", selectAP);
  webServer.on("/wifisave", saveWiFi);
  webServer.on("/cfgauth", configAuth);
  webServer.on("/saveauth", saveAuth);
  webServer.onNotFound(routeToDevice);
  
  webServer.begin();
}



