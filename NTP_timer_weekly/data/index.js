var cn=new WebSocket('ws://'+location.hostname+':81/');
var activated = false;

cn.onopen=function(){
  cn.send('{"CURRENT":"","STATUS":"","TIMEZONE":"","SCHEDULE":"","SCHENABLED":""}');   
};

cn.onmessage=function(e){  
  console.log(e);   
  var data=JSON.parse(e.data);  
  
  if(data.hasOwnProperty('CURRENT')){ 
    var timeDate =  data.CURRENT.split("|");   
      
    document.getElementById('time').innerHTML=timeDate[0]; 
    document.getElementById('date').innerHTML=timeDate[1];     
  } 
  if(data.hasOwnProperty('STATUS')){       
    document.getElementById('status').innerHTML=data.STATUS;       
  }
  if(data.hasOwnProperty('ACTIVE')){   
    var result = data.ACTIVE.split(',')
    
    activated = parseInt(result[0]) > 0;
    var enBtn = document.getElementById('etgl_btn');

    if(activated){        
      enBtn.innerHTML="Force off";
    }else{
      enBtn.innerHTML="Force on";
    }
  }
  if(data.hasOwnProperty('SCHENABLED')){  
    document.getElementById('useSch').checked=parseInt(data.SCHENABLED);
  }
  if(data.hasOwnProperty('TIMEZONE')){       
    document.getElementById('tz').value=data.TIMEZONE;       
  }
  if(data.hasOwnProperty('DLSAVE')){       
    document.getElementById('ds').checked=parseInt(data.DLSAVE);       
  }

  if(data.hasOwnProperty('SCHEDULE')){        
    var schedule =  data.SCHEDULE.split(",");
    if(schedule.length > 13){
      document.getElementById("d0_start").value = schedule[0];
      document.getElementById("d0_stop").value = schedule[1]; 
      document.getElementById("d1_start").value = schedule[2];
      document.getElementById("d1_stop").value = schedule[3]; 
      document.getElementById("d2_start").value = schedule[4];
      document.getElementById("d2_stop").value = schedule[5]; 
      document.getElementById("d3_start").value = schedule[6];
      document.getElementById("d3_stop").value = schedule[7]; 
      document.getElementById("d4_start").value = schedule[8];
      document.getElementById("d4_stop").value = schedule[9]; 
      document.getElementById("d5_start").value = schedule[10];
      document.getElementById("d5_stop").value = schedule[11]; 
      document.getElementById("d6_start").value = schedule[12];
      document.getElementById("d6_stop").value = schedule[13]; 
    }else{
      console.log("ERR: wrong schedule len:" + schedule.length);
    }    
  }

  document.getElementById('logo').src="/logo.png";
}; 

function setZone(){
  var e = document.getElementById("tz");
  var tz = e.options[e.selectedIndex].value;
  var dls = e.options[e.selectedIndex].getAttribute('dlSave');
  cn.send('{"TIMEZONE":"' + tz + '", "DLSAVE":"' + dls + '"}');
}  

function setDs(){
  if(document.getElementById("ds").checked == true){
    cn.send('{"DLSAVE":1}');
  }else{
    cn.send('{"DLSAVE":0}');  
  }
}  

function pad(number) {   
  var str = '' + number;
  if (str.length < 2) {
      str = '0' + str;
  }   
  return str;
}

function scheduleSave(){
var msg = '{"SCHEDULE":"'
+ pad(document.getElementById("d0_start").value) + "," + pad(document.getElementById("d0_stop").value) + "," 
+ pad(document.getElementById("d1_start").value) + "," + pad(document.getElementById("d1_stop").value) + ","
+ pad(document.getElementById("d2_start").value) + "," + pad(document.getElementById("d2_stop").value) + ","  
+ pad(document.getElementById("d3_start").value) + "," + pad(document.getElementById("d3_stop").value) + "," 
+ pad(document.getElementById("d4_start").value) + "," + pad(document.getElementById("d4_stop").value) + "," 
+ pad(document.getElementById("d5_start").value) + "," + pad(document.getElementById("d5_stop").value) + "," 
+ pad(document.getElementById("d6_start").value) + "," + pad(document.getElementById("d6_stop").value) + "," 
+ '"}';

console.log("Sending" + msg);
  cn.send(msg);
}
  
function toggle(){
  if(activated){
    cn.send('{"ACTIVE":"0"}');
  }else{
    cn.send('{"ACTIVE":"1"}');
  }
}

function scheduleSet(){
  if(document.getElementById('useSch').checked == true){
    cn.send('{"SCHENABLED":"1"}');
  }else{
    cn.send('{"SCHENABLED":"0"}');
  }  
}

