var cn=new WebSocket('ws://'+location.hostname+':81/');
  cn.onopen=function(){
    cn.send('{"CURRENT":"","STATUS":"","TIMEZONE":""}');    
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
    if(data.hasOwnProperty('TIMEZONE')){       
      document.getElementById('tz').value=data.TIMEZONE;       
    }
    if(data.hasOwnProperty('DLSAVE')){       
      document.getElementById('ds').checked=data.DLSAVE;       
    }

    document.getElementById('logo').src="/logo.png";
  }; 

  function setZone(){
    var e = document.getElementById("tz");
    var tz = e.options[e.selectedIndex].value;
    var dls = e.options[e.selectedIndex].getAttribute('dlSave');
    cn.send('{"TIMEZONE":' + tz + ', "DLSAVE":' + dls + '}');
  }  

  function setDs(){
    var e = document.getElementById("ds");
    if(e.checked == true){
      cn.send('{"DLSAVE":1}');
    }else{
      cn.send('{"DLSAVE":0}');  
    }
  }  
  
