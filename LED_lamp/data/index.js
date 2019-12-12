var oldAngle = 0;
var savedLevel = 0;
var current = 0;
var sliderChgTimestamp;

var input = document.querySelector('.plain-angle-input');
var outlet = document.querySelector('.plain-angle-outlet');

var options = {
	max: 185,       // maximum value
	min: 0,         // minimum value
	step: 2,        // [min, min+step, ..., max]
	name: 'angle',  // used for <input name>
  };

var angle = AngleInput(input, options);
  
input.oninput = function(e) {
	
	var pwr = Math.round(((180 - angle()) * 20) / 170) * 5;
	if(pwr > 100){
		pwr = 100;
	}
	if(pwr < 0){
		pwr = 0;
	}	
	
	if(oldAngle != angle()){
		console.log("in");
		oldAngle = angle();
		outlet.innerText = pwr;
		setLevel(pwr);
	}	
}

input.onchange = function(e) {
	var pwr = Math.round(((180 - angle()) * 20) / 170) * 5;
	if(pwr > 100){
		pwr = 100;
	}
	if(pwr < 0){
		pwr = 0;
	}
	if(oldAngle != angle()){		
		oldAngle = angle();
		console.log("ch to:" + pwr);
		outlet.innerText = pwr;
		setLevel(pwr);
	}	
}

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
		}else{
			document.getElementById('tgl').classList.remove('lightBtnOff');
			document.getElementById('tgl').classList.add('lightBtnOn');
			savedLevel = data.CURRENT;
		}	
		
		if((Math.floor(Date.now() / 1000) - sliderChgTimestamp) > 1){
			if(data.CURRENT > 0){
				var newAngle = 180 - ((data.CURRENT *170) / 100);
				angle(newAngle); 
			}
		}else{
			sliderChgTimestamp = Math.floor(Date.now()) / 1000;
		}
	} 
	
	if(data.hasOwnProperty('STATUS')){       
	  document.getElementById('status').innerHTML=data.STATUS;       
	}	
	  
	document.getElementById('logo').src="/logo.png";
	
};
function setLevel(lev){  
	cn.send('{"CURRENT":' + lev + '}');
}

function toggle(){  
	if(savedLevel == 0){
		savedLevel = 100;
	}

	if(current == 0){
		current = savedLevel;
	}else{
		current = 0;
	}

	// console.log("Sending:" + current);
	sliderChgTimestamp = Math.floor(Date.now() / 1000);

	cn.send('{"CURRENT":' + current + '}');
}
