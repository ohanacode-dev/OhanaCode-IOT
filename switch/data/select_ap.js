function c(l){
	document.getElementById('s').value=l.innerText||l.textContent;
	document.getElementById('p').focus();
}

var cn=new WebSocket('ws://'+location.hostname+':81/');
	cn.onopen=function(){
	cn.send('{"APLIST":""}');
}
cn.onmessage=function(e){
	var data=JSON.parse(e.data);
	if(data.hasOwnProperty('APLIST')){
		rsp=data.APLIST.split('|');
		document.getElementById('vm').innerHTML='';
		for(var i=0;i<(rsp.length);i++){
			document.getElementById('vm').innerHTML+='<span>'+(i+1)+": </span><a href='#p' onclick='c(this)'>" + rsp[i] + '</a><br>';
		}
		if(!document.getElementById('vm').innerHTML.replace(/\\s/g,'').length){
			document.getElementById('ttl').innerHTML='No networks found.'
		} 
	}
	if(data.hasOwnProperty('DAP')){
		if(data.DAP == "0"){
			document.getElementById('dap').checked = false;
		}else{
			document.getElementById('dap').checked = true;
		}
	}
};
function refresh(){
	document.getElementById('vm').innerHTML='Please wait...'
	cn.send('{"APLIST":""}');
}

function updateDap()
{
  if (document.getElementById('dap').checked) {
	cn.send('{"DAP":"1"}');
  } else {
	cn.send('{"DAP":"0"}');
  }
}
