var cn=new WebSocket('ws://'+location.hostname+':81/');
cn.onopen=function(){
	cn.send('{"CURRENT":"","STATUS":""}');    
};
cn.onmessage=function(e){
	var data=JSON.parse(e.data);
	
	if(data.hasOwnProperty('STATUS')){       
	  document.getElementById('status').innerHTML=data.STATUS;       
	}	
	  
	document.getElementById('logo').src="/logo.png";	
};

function send(){  
	var tx = document.getElementById('tx').value;
	cn.send('{"TX":' + tx + '}');	
}


