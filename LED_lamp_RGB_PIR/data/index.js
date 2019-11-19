var colArr = ["#1abc9c","#2ecc71","#0000FF","#9b59b6","#34495e","#00FF00",
"#27ae60","#2980b9","#8e44ad","#2c3e50","#f1c40f","#e67e22","#e74c3c",
"#ecf0f1","#95a5a6","#f39c12","#d35400","#FF0000","#bdc3c7","#7f8c8d"];

var cpDiv = document.getElementById('cpick');
var opacity = document.getElementById('opac');
var sample = document.getElementById('sampc');
var choose = document.getElementById('choose');
var p = [255, 255, 255];
var cn=new WebSocket('ws://'+location.hostname+':81/');

for(var i=0;i<colArr.length;i++){
	cDiv = document.createElement('div');
	cDiv.className = "kolor";
	cDiv.id = i;
	cDiv.style.background = colArr[i];
	cDiv.onclick = function() { 
		setColor(colArr[this.id]); 
	};
	cpDiv.append(cDiv);
}

opacity.addEventListener('change', function(e){
	setOpacity(opacity.value*12.5);
}); 

cn.onopen=function(){
	cn.send('{"CURRENT":"","STATUS":""}');    
};
cn.onmessage=function(e){
	var data=JSON.parse(e.data);
	if(data.hasOwnProperty('CURRENT')){       
	  opacity.value = data.CURRENT.OPA*0.08;  
	  sample.style.opacity = opacity.value*0.125;
	  choose.value = data.CURRENT.CLR;
	}
	
	if(data.hasOwnProperty('STATUS')){       
		document.getElementById('status').innerHTML=data.STATUS;       
	}
	
	document.getElementById('logo').src="/logo.png";
}

function setOpacity(lev){  
  cn.send('{"CURRENT":{OPA:' + lev + '}}');
}
function setColor(clr){  
  cn.send('{"CURRENT":{CLR:"' + clr + '"}}');
}
