var cn=new WebSocket('ws://'+location.hostname+':81/');
cn.onopen=function(){
	cn.send('{"STATUS":""}');    
};
cn.onmessage=function(e){
	var data=JSON.parse(e.data);

	if(data.hasOwnProperty('STATUS')){       
	  document.getElementById('status').innerHTML=data.STATUS;       
	}	
};

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
