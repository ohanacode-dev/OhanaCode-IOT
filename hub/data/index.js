var cn=new WebSocket('ws://'+location.hostname+':81/');

cn.onopen=function(){
	cn.send('{"CURRENT":""}');    
};

cn.onmessage=function(e){
	var data=JSON.parse(e.data);
	if(data.hasOwnProperty('CURRENT')){       
	  document.getElementById('m').value=parseInt(data.CURRENT);       
	} 
	  
	document.getElementById('logo').src="/logo.png";
	
};

function setLevel(lev){  
	cn.send('{"CURRENT":' + lev + '}');
}
