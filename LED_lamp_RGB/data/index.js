var opacity = document.getElementById('opac');
var clr_r = document.getElementById('c_r');
var clr_g = document.getElementById('c_g');
var clr_b = document.getElementById('c_b');
var ts=0;

var cn=new WebSocket('ws://'+location.hostname+':81/');

opacity.addEventListener('change', function(e){
	setOpacity(opacity.value*12.5);
}); 

clr_r.addEventListener('change', function(e){
	updateSlideColor();
}); 
clr_g.addEventListener('change', function(e){
	updateSlideColor();
});
clr_b.addEventListener('change', function(e){
	updateSlideColor();
});

cn.onopen=function(){
	cn.send('{"CURRENT":"","STATUS":""}');    
};
cn.onmessage=function(e){
	var data=JSON.parse(e.data);

	if(data.hasOwnProperty('CURRENT')){       
		opacity.value = data.CURRENT.OPA*0.08;

		if((Date.now() - ts) > 100){
			clr_r.value = Math.round(parseInt(data.CURRENT.CLR.substring(1, 3),16) / 25.5);
			clr_g.value = Math.round(parseInt(data.CURRENT.CLR.substring(3, 5),16) / 25.5); 
			clr_b.value = Math.round(parseInt(data.CURRENT.CLR.substring(5, 7),16) / 25.5);
		}		
	}
	
	if(data.hasOwnProperty('STATUS')){       
		document.getElementById('status').innerHTML=data.STATUS;       
	}
	
	document.getElementById('logo').src="/logo.png";
}

function setOpacity(lev){  
  cn.send('{"CURRENT":{OPA:' + lev + '}}');
}
function updateSlideColor(){  
	var R=Math.round(clr_r.value*25.5);
	var G=Math.round(clr_g.value*25.5);
	var B=Math.round(clr_b.value*25.5);
	var color="#" + ("00" + R.toString(16)).substr(-2) + ("00" + G.toString(16)).substr(-2) + ("00" + B.toString(16)).substr(-2);
	ts = Date.now();
	cn.send('{"CURRENT":{CLR:"' + color + '"}}');
}
  
