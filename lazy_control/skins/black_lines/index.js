var longpress = false;
var presstimer = null;
var buttons = document.getElementsByTagName('button');

var cancel = function(e) {
    if (presstimer !== null) {
        clearInterval(presstimer);
        presstimer = null;
    }
};

var click = function(e){
    if (presstimer !== null) {
        clearInterval(presstimer);
        presstimer = null;
    }
    if(!longpress) {
        cmdSend(this.className);
    };
    longpress = false;
}

var start = function(e) {
    var btnName = this.className;
    presstimer = setInterval(function() {
        longpress = true;
        cmdSend(btnName);
    }, 500);
};

for (var i = 0; i < buttons.length; i++) {
    var node = buttons[i];
    node.addEventListener("mousedown", start);
    node.addEventListener("touchstart", start);
    node.addEventListener("click", click);
    node.addEventListener("mouseout", cancel);
    node.addEventListener("touchend", cancel);
    node.addEventListener("touchleave", cancel);
    node.addEventListener("touchcancel", cancel);
}


function cmdSend(name){
    var val = -2;
    switch(name) {
        case "off":
            val=1;
            break;
        case "stop":
            val=3;
            break;
        case "vol_up":
            val=5;
            break;
        case "rw":
            val=7;
            break;
        case "playpause":
            val=9;
            break;
        case "ff":
            val=11;
            break;
        case "vol_down":
            val=13;
            break;
        case "media":
            val=15;
            break;
        case "msg":
            window.location.href = "/msgbox.html";
            break;
        }
        if(longpress){
            val++;
        }
    if(val > 0){
        var xH=new XMLHttpRequest();
        var url='/ctrl?CMD=' + val;
        xH.open('GET',url, true);
        xH.send(null);
        xH.onreadystatechange=function(){
          if(xH.readyState==4 && xH.status==200){
             if(this.responseText!='OK'){
               alert(this.responseText);
             }
          }
        }
    }
}
setInterval(function(){
    console.log(document.visibilityState);
    if(document.visibilityState == "visible"){
        var wt=new XMLHttpRequest();
        var url='/id';
        wt.open('GET',url, true);
        wt.send(null);
        wt.onreadystatechange=function(){
            if(wt.readyState==4){
                if(wt.status==200){
                    var curTitle=JSON.parse(this.responseText);
                    document.getElementById('cur_title').textContent=curTitle.CURRENT;
                }else{
                    document.getElementById('cur_title').textContent="ERROR: no connection...";
                }
            }

        }}}, 1000);
