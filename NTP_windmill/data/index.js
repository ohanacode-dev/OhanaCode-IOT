var cn=new WebSocket('ws://'+location.hostname+':81/');
cn.onopen=function(){
  cn.send('{"CURRENT":"","STATUS":""}');    
};
cn.onmessage=function(e){
  var data=JSON.parse(e.data);
  console.log(data);
  if(data.hasOwnProperty('CURRENT')){     
    document.getElementById('time').innerHTML=data.CURRENT; 
  } 
  if(data.hasOwnProperty('STATUS')){       
    document.getElementById('status').innerHTML=data.STATUS;       
  } 
  document.getElementById('logo').src="/logo.png";
}; 

function light_tgl(){
  cn.send('{"LIGHT":1}');  
}

function tick(){
  cn.send('{"TICK":1}');  
}
