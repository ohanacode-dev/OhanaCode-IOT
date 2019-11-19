function changeSkin() {

    var msg=new XMLHttpRequest();
    var url='/change_skin';
    msg.open('GET',url, true);
    msg.timeout = 2000;
    msg.send(null);
    msg.ontimeout = function (e) {
        alert("No server response");
    }
    msg.onreadystatechange=function(){
        if(msg.readyState==4 && msg.status==200){
           window.location.href = "/";
        }
    }
}
