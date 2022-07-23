#ifndef HTML_UI_H
#define HTML_UI_H

static const char index_html[] PROGMEM = R"(
<!DOCTYPE HTML>
<html>
<head>
    <meta name = "viewport" content = "width = device-width, initial-scale = 1.0, maximum-scale = 1.0, user-scalable=0">
    <title>Ohana RGBA LED Control</title>    
    <link rel="shortcut icon" href="">
<style>
body{background-color: white; font-family: Arial, Helvetica, Sans-Serif; Color:#000000; }
.contain{width: 100%;}
.center_div{margin:0 auto; max-width: 400px;position:relative;}
.btn_b{border:0;border-radius:0.3rem;color:#fff;line-height:2.4rem;font-size:1.2rem;margin:2%;height:2.4rem;background-color:#1fa3ec;}
.btn_cfg{border:0;border-radius:0.3rem;color:#fff;line-height:1.4rem;font-size:0.8rem;margin:1ch;height:2rem;width:15ch;background-color:#ff3300;}
h1{text-align: center;font-size:1.5rem;}
.slider, .r_slide, .g_slide, .b_slide, .a_slide{-webkit-appearance:none;appearance:none;width:90%;height:0.8em;border-radius:0.4em;background:#d3d3d3;outline:none;
-webkit-transition:.2s;margin:10% 4%;padding:0;}
.slider::-webkit-slider-thumb{-webkit-appearance:none;appearance:none;width:3em;height:3em;background:white;border-radius:50%;box-shadow:inset -1em -1em 2em rgba(0,0,0,.5);}
.slider::-moz-range-thumb{appearance:none;width:3em;height:3em;background:white;border-radius:50%;box-shadow: inset -1em -1em 2em rgba(0,0,0,.5);}
.r_slide::-webkit-slider-thumb, .g_slide::-webkit-slider-thumb, .b_slide::-webkit-slider-thumb, .a_slide::-webkit-slider-thumb{-webkit-appearance:none;appearance:none;width:3em;height:3em;}
.r_slide::-moz-range-thumb, .g_slide::-moz-range-thumb, .b_slide::-moz-range-thumb, .a_slide::-moz-range-thumb{appearance:none;width:3em;height:3em;background:white;}
.r_slide::-webkit-slider-thumb{background:rgb(243, 100, 100);border-radius:50%;box-shadow:inset -1em -1em 2em rgba(112, 2, 2, 0.5);}
.r_slide::-moz-range-thumb{border-radius:50%;box-shadow: inset -1em -1em 2em rgba(112, 2, 2, 0.5);}
.g_slide::-webkit-slider-thumb{background:rgb(141, 243, 100);border-radius:50%;box-shadow:inset -1em -1em 2em rgba(2, 112, 11, 0.5);}
.g_slide::-moz-range-thumb{border-radius:50%;box-shadow: inset -1em -1em 2em rgba(2, 112, 11, 0.5);}
.b_slide::-webkit-slider-thumb{background:rgb(126, 100, 243);border-radius:50%;box-shadow:inset -1em -1em 2em rgba(13, 2, 112, 0.5);}
.b_slide::-moz-range-thumb{border-radius:50%;box-shadow: inset -1em -1em 2em rgba(13, 2, 112, 0.5);}
.a_slide::-webkit-slider-thumb{background:rgb(255, 255, 255);border-radius:50%;box-shadow:inset -1em -1em 2em rgba(27, 27, 27, 0.5);}
.a_slide::-moz-range-thumb{border-radius:50%;box-shadow: inset -1em -1em 2em rgba(182, 181, 184, 0.5);}
.kolor{width:18%;margin:1%;padding-top:17%;float:left;border-radius:50%;}
.row{width:100%;overflow: auto;}
#cpick{width:100%;overflow: auto;}
#samp{margin:1em auto;background:black;padding:0;overflow:hidden;width:102px;height:52px;border-radius:2em;border:2px solid black;}
#sampc{margin:auto;padding:0;overflow:hidden;width:102px;text-align:center;}
input[type="color"]{height:70px;width:150px;margin-left:-20px;margin-top:-10px;} 
#logo{display:block;margin:0 0;width:4em;height:4em;}
</style>
</head>
<body>
<div class="contain">
  <div class="center_div">
	<img id="logo" src="" alt="Logo">
    <h1>Ohana RGBA LED Control</h1> 
    <div id="spick">
      <input type="range" min="0" max="10" class="r_slide" id="c_r">
      <input type="range" min="0" max="10" class="g_slide" id="c_g">
      <input type="range" min="0" max="10" class="b_slide" id="c_b">
      <input type="range" min="0" max="10" class="a_slide" id="c_a">
    </div>
  </div>
  <hr>
  <p id='status'></p>  
  <br><button class="btn_cfg" type="button" onclick="location.href='/select_ap';">Configure wifi</button>
  <br/>
</div>
<script>
var clr_r = document.getElementById('c_r');
var clr_g = document.getElementById('c_g');
var clr_b = document.getElementById('c_b');
var clr_a = document.getElementById('c_a');
var ts=0;
var cn=new WebSocket('ws://'+location.hostname+':81/');
clr_r.addEventListener('change', function(e){updateSlideColor();});
clr_g.addEventListener('change', function(e){updateSlideColor();});
clr_b.addEventListener('change', function(e){updateSlideColor();});
clr_a.addEventListener('change', function(e){updateSlideColor();});
cn.onopen=function(){cn.send('{"CURRENT":"","STATUS":""}');};
cn.onmessage=function(e){
var data=JSON.parse(e.data);if(data.hasOwnProperty('CURRENT')){
if((Date.now() - ts) > 100){
clr_r.value = parseInt(data.CURRENT[0]) / 10;
clr_g.value = parseInt(data.CURRENT[1]) / 10;
clr_b.value = parseInt(data.CURRENT[2]) / 10;
clr_a.value = parseInt(data.CURRENT[3]) / 10;
}}
if(data.hasOwnProperty('STATUS')){document.getElementById('status').innerHTML=data.STATUS; }
document.getElementById('logo').src="/logo.png";
}
function updateSlideColor(){
ts=Date.now();
cn.send('{"CURRENT":['+(clr_r.value*10)+','+(clr_g.value*10)+','+(clr_b.value * 10)+','+(clr_a.value*10)+']}');
}
</script>
</body>
</html>
)"


static const char redirect_html[] PROGMEM = R"(
<!DOCTYPE HTML>
<html>
<head>
<meta name = "viewport" content = "width = device-width, initial-scale = 1.0, maximum-scale = 1.0, user-scalable=0">
<title>Ohana WiFi LED Control</title>
<link rel="stylesheet" href="redirect.css">
<link rel="shortcut icon" href="">
<style>
body{background-color: #210D03; font-family: Arial, Helvetica, Sans-Serif; Color: #EDEBD1;}
.contain{width: 100%;}
.center_div{margin:0 auto; max-width: 400px;position:relative;}
</style>
</head>
<body>
<div class="contain">
<div class="center_div">
<h1>Connection Status</h1>
<p id="status"></p>
<p id="tmr"></p>
</div>  
</div>
</body>
<script>
var cn=new WebSocket('ws://'+location.hostname+':81/');
cn.onopen=function(){cn.send('{"STATUS":""}');};
cn.onmessage=function(e){var data=JSON.parse(e.data);
if(data.hasOwnProperty('STATUS')){       
document.getElementById('status').innerHTML=data.STATUS;       
}};
var c=6;   
function count(){
var tmr=document.getElementById('tmr');   
if(c>0){c--;tmr.innerHTML="You will be redirected to home page in "+c+" seconds.";setTimeout('count()',1000);}else{
window.location.href="/";}}count();
</script>
</html>
)";

static const char select_ap_html[] PROGMEM = R"(
<!DOCTYPE HTML>
<html>
<head>
<meta name = "viewport" content = "width = device-width, initial-scale = 1.0, maximum-scale = 1.0, user-scalable=0">
<title>Ohana WiFi LED Control</title>
<link rel="stylesheet" href="select_ap.css">
<link rel="shortcut icon" href="">
<style>
body{background-color: #210D03; font-family: Arial, Helvetica, Sans-Serif; color: #EDEBD1;}
.contain{width: 100%;}
.center_div{margin:0 auto; max-width: 400px;position:relative;}
.c{text-align: center;}
div,input{padding:5px;font-size:1em;}
input{width:95%;background-color: black;
border: 2px solid #210D03;color: #EDEBD1;}
body{text-align: left;}
button{width:100%;border:0;border-radius:0.3rem;color:black;line-height:2.4rem;font-size:1.2rem;height:40px;background-color:#8a8a5c;}
.q{float: right;width: 64px;text-align: right;}
.radio{width:2em;}
#vm{width:100%;height:50vh;overflow-y:auto;margin-bottom:1em;}
a, a:link, a:visited, a:hover, a:active {color: #66ff66;}
</style>
</head>
<body>
<div class="contain">
<div class="center_div">
<h1 id='ttl'>Networks found:</h1>
<div id='vm'><p>Please wait...</p></div>
<form method='get' action='wifisave'>
<button type='button' onclick='refresh();'>Rescan</button><br/><br/>
<input id='s' name='s' length=32 placeholder='SSID (Leave blank for AP mode)'><br>      
<input id='p' name='p' length=32 placeholder='password'><br>
<input id='a' name='a' length=16 placeholder='static IP address (optional)'><br>       
<br>
<button type='submit'>save</button>
</form>
</div>
</div>
<script>
function c(l){
document.getElementById('s').value=l.innerText||l.textContent;
document.getElementById('p').focus();}
var cn=new WebSocket('ws://'+location.hostname+':81/');
cn.onopen=function(){
cn.send('{"APLIST":""}');
}
cn.onmessage=function(e){
var data=JSON.parse(e.data);
if(data.hasOwnProperty('APLIST')){
rsp=data.APLIST.split('|');
document.getElementById('vm').innerHTML='';
for(var i=0;i<(rsp.length);i++){document.getElementById('vm').innerHTML+='<span>'+(i+1)+": </span><a href='#p' onclick='c(this)'>" + rsp[i] + '</a><br>';}
if(!document.getElementById('vm').innerHTML.replace(/\\s/g,'').length){document.getElementById('ttl').innerHTML='No networks found.'} 
}
};
function refresh(){document.getElementById('vm').innerHTML='Please wait...'cn.send('{"APLIST":""}');}
</script>
</body></html>
)";


#endif
