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
};
function refresh(){
	document.getElementById('vm').innerHTML='Please wait...'
	cn.send('{"APLIST":""}');
}
