function sendMessage() {
    var msgText = document.getElementById('return_value').value.replace(/\\r?\\n/g, '%3');
    if (msgText != null && msgText != "") {
        var msg=new XMLHttpRequest();
        var url='/msg?' + msgText;
        msg.open('GET',url, true);
        msg.timeout = 2000;
        msg.send(null);
        msg.ontimeout = function (e) {
            alert("No server response");
        }
        msg.onreadystatechange=function(){
            if(msg.readyState==4 && msg.status==200){
               window.history.back();
            }
        }
    }
}
