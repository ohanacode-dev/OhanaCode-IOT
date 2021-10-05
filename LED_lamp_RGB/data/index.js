var clr_r = document.getElementById('c_r');
var clr_g = document.getElementById('c_g');
var clr_b = document.getElementById('c_b');
var clr_a = document.getElementById('c_a');
var ts=0;

var cn=new WebSocket('ws://'+location.hostname+':81/');

clr_r.addEventListener('change', function(e){
	updateSlideColor();
}); 
clr_g.addEventListener('change', function(e){
	updateSlideColor();
});
clr_b.addEventListener('change', function(e){
	updateSlideColor();
});
clr_a.addEventListener('change', function(e){
	updateSlideColor();
}); 

cn.onopen=function(){
	cn.send('{"CURRENT":"","STATUS":""}');    
};
cn.onmessage=function(e){
	var data=JSON.parse(e.data);

	if(data.hasOwnProperty('CURRENT')){ 
		if((Date.now() - ts) > 100){
			clr_r.value = parseInt(data.CURRENT[0]) / 10;
			clr_g.value = parseInt(data.CURRENT[1]) / 10; 
			clr_b.value = parseInt(data.CURRENT[2]) / 10;
			clr_a.value = parseInt(data.CURRENT[3]) / 10;
		}		
	}
	
	if(data.hasOwnProperty('STATUS')){       
		document.getElementById('status').innerHTML=data.STATUS;       
	}
	
	document.getElementById('logo').src="/logo.png";
}

function updateSlideColor(){
	ts = Date.now();
	cn.send('{"CURRENT":[' + (clr_r.value * 10) + ',' + (clr_g.value * 10) + ',' + (clr_b.value * 10) + ',' + (clr_a.value * 10) +']}');
}
  
