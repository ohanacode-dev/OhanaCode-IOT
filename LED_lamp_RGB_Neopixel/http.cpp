/* 
 *  Author: Rada Berar
 *  email: ujagaga@gmail.com
 *  
 *  HTTP server which generates the web browser pages. 
 */
 
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <ESP8266HTTPClient.h>
#include <pgmspace.h>
#include "http.h"
#include "wifi_connection.h"
#include "web_socket.h"
#include "config.h"
#include "LED_lamp_RGB_Neopixel.h"
#include "led.h"
#include "ota.h"
#include "logo.h"

static const char HTML_BEGIN[] PROGMEM = R"(
<!DOCTYPE HTML>
<html>
  <head>
    <meta name = "viewport" content = "width = device-width, initial-scale = 1.0, maximum-scale = 1.0, user-scalable=0">
    <title>WiFi RGB LED Control</title>
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
body {background-color: white; font-family: Arial, Helvetica, Sans-Serif; Color:#000000; }
.contain{
width: 100%;
}
.center_div{
margin:0 auto; max-width: 400px;position:relative;
}
.btn_b{
border:0;border-radius:0.3rem;color:#fff;line-height:2.4rem;
font-size:1.2rem;margin:2%;height:2.4rem;background-color:#1fa3ec;
}
.btn_cfg{
border:0;border-radius:0.3rem;color:#fff;line-height:1.4rem;
font-size:0.8rem;margin:1ch;height:2rem;width:15ch;background-color:#ff3300;
}
h1{
text-align: center;font-size:1.5rem;
}
.slider, .r_slide, .g_slide, .b_slide, .a_slide{
-webkit-appearance:none;appearance:none;width:90%;height:0.8em;
border-radius:0.4em;background:#d3d3d3;outline:none;
-webkit-transition:.2s;margin:10% 4%;padding:0;
}
.slider::-webkit-slider-thumb{
-webkit-appearance:none;appearance:none;width:3em;height:3em;
background:white;border-radius:50%;box-shadow:inset -1em -1em 2em rgba(0,0,0,.5);
}
.slider::-moz-range-thumb{
appearance:none;width:3em;height:3em;background:white;
border-radius:50%;box-shadow: inset -1em -1em 2em rgba(0,0,0,.5);
}
.r_slide::-webkit-slider-thumb, .g_slide::-webkit-slider-thumb, .b_slide::-webkit-slider-thumb, .a_slide::-webkit-slider-thumb{
-webkit-appearance:none;appearance:none;width:3em;height:3em;
}
.r_slide::-moz-range-thumb, .g_slide::-moz-range-thumb, .b_slide::-moz-range-thumb, .a_slide::-moz-range-thumb{
appearance:none;width:3em;height:3em;background:white;
}
.r_slide::-webkit-slider-thumb{background:rgb(243, 100, 100);border-radius:50%;box-shadow:inset -1em -1em 2em rgba(112, 2, 2, 0.5);}
.r_slide::-moz-range-thumb{border-radius:50%;box-shadow: inset -1em -1em 2em rgba(112, 2, 2, 0.5);}
.g_slide::-webkit-slider-thumb{background:rgb(141, 243, 100);border-radius:50%;box-shadow:inset -1em -1em 2em rgba(2, 112, 11, 0.5);}
.g_slide::-moz-range-thumb{border-radius:50%;box-shadow: inset -1em -1em 2em rgba(2, 112, 11, 0.5);}
.b_slide::-webkit-slider-thumb{background:rgb(126, 100, 243);border-radius:50%;box-shadow:inset -1em -1em 2em rgba(13, 2, 112, 0.5);}
.b_slide::-moz-range-thumb{border-radius:50%;box-shadow: inset -1em -1em 2em rgba(13, 2, 112, 0.5);}
.a_slide::-webkit-slider-thumb{background:rgb(255, 255, 255);border-radius:50%;box-shadow:inset -1em -1em 2em rgba(27, 27, 27, 0.5);}
.a_slide::-moz-range-thumb{border-radius:50%;box-shadow: inset -1em -1em 2em rgba(182, 181, 184, 0.5);}

.kolor{
width:18%;margin:1%;padding-top:17%;float:left;border-radius:50%;
}
.row{
width:100%;overflow: auto;
}
#cpick{
width:100%;overflow: auto;
}
#samp{
margin:1em auto;background:black;padding:0;overflow:hidden;
width:102px;height:52px;border-radius:2em;border:2px solid black;
}
#sampc{
margin:auto;padding:0;overflow:hidden;width:102px;text-align:center;
}
input[type="color"]{
height:70px;width:150px;margin-left:-20px;margin-top:-10px;
} 
#logo{
display:block;margin:0 0;width:4em;height:4em;
}

</style>
<div class="contain">
  <div class="center_div">
)";

const char INDEX_HTML_1[] PROGMEM = R"(
    <h1>RGBA LED Control</h1> 
    <div id="spick">
      <input type="range" min="0" max="10" class="r_slide" id="c_r">
      <input type="range" min="0" max="10" class="g_slide" id="c_g">
      <input type="range" min="0" max="10" class="b_slide" id="c_b">
      <input type="range" min="0" max="10" class="a_slide" id="c_a">
    </div>
  </div> 
</div>
<script>
var clr_r = document.getElementById('c_r');
var clr_g = document.getElementById('c_g');
var clr_b = document.getElementById('c_b');
var clr_a = document.getElementById('c_a');
var ts=0;

var cn=new WebSocket('ws://'+location.hostname+':81/');

clr_r.addEventListener('change', function(e){
	updateSlideColor();
}); 
clr_g.addEventListener('change', function(e){
	updateSlideColor();
});
clr_b.addEventListener('change', function(e){
	updateSlideColor();
});
clr_a.addEventListener('change', function(e){
	updateSlideColor();
}); 

cn.onopen=function(){
	cn.send('{"CURRENT":""}');    
};
cn.onmessage=function(e){
	var data=JSON.parse(e.data);

	if(data.hasOwnProperty('CURRENT')){ 
		if((Date.now() - ts) > 100){
			clr_r.value = parseInt(data.CURRENT[0]) / 10;
			clr_g.value = parseInt(data.CURRENT[1]) / 10; 
			clr_b.value = parseInt(data.CURRENT[2]) / 10;
			clr_a.value = parseInt(data.CURRENT[3]) / 10;
		}		
	}		
}

function updateSlideColor(){
	ts = Date.now();
	cn.send('{"CURRENT":[' + (clr_r.value * 10) + ',' + (clr_g.value * 10) + ',' + (clr_b.value * 10) + ',' + (clr_a.value * 10) +']}');
}

</script>
)";


/* Declaring a web server object. */
static ESP8266WebServer webServer(80);

void showStartPage() {    
  String response = FPSTR(HTML_BEGIN);
  response += FPSTR(INDEX_HTML_0);
  response += FPSTR(LOGO);
  response += FPSTR(INDEX_HTML_1); 
  response += FPSTR(HTML_END);
  webServer.send(200, "text/html", response);  
}


static void showNotFound(void){
  webServer.send(404, "text/html; charset=iso-8859-1","<html><head> <title>404 Not Found</title></head><body><h1>Not Found</h1></body></html>");
}

static void startOtaUpdate(void){
  OTA_init();    
}

void HTTP_process(void){
  webServer.handleClient(); 
}

void HTTP_init(void){ 
  webServer.on("/", showStartPage);
  webServer.on("/favicon.ico", showNotFound);
  webServer.on("/start_ota_update", startOtaUpdate);  
  webServer.onNotFound(showNotFound);
  
  webServer.begin();
}
