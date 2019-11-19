var cn=new WebSocket('ws://'+location.hostname+':81/');
cn.onopen=function(){
	cn.send('{"CURRENT":"","STATUS":""}');    
};
cn.onmessage=function(e){
	var data=JSON.parse(e.data);

	if(data.hasOwnProperty('CURRENT')){
		document.getElementById('temp').innerHTML=data.CURRENT;   
	}

	document.getElementById('logo').src="/logo.png";
};
