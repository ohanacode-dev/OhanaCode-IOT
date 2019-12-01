var level = 0;

var cn=new WebSocket('ws://'+location.hostname+':81/');
cn.onopen=function(){
	cn.send('{"CURRENT":"","STATUS":""}');    
};
cn.onmessage=function(e){
	var data=JSON.parse(e.data);
	if(data.hasOwnProperty('CURRENT')){   
		if(data.CURRENT == 0){
			document.getElementById('tgl').classList.add('lightBtnOff');
			document.getElementById('tgl').classList.remove('lightBtnOn');
			level = 0;
		}else{
			document.getElementById('tgl').classList.remove('lightBtnOff');
			document.getElementById('tgl').classList.add('lightBtnOn');
			level = 100;
		}
	} 
	if(data.hasOwnProperty('STATUS')){       
	  document.getElementById('status').innerHTML=data.STATUS;       
	}	
	  
	document.getElementById('logo').src="/logo.png";
	
};

function toggle(){  
	if(level == 0){
		level = 100;
	}else{
		level = 0;
	}
	cn.send('{"CURRENT":' + level + '}');
}
