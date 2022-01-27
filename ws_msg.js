const express = require('express')
const app = express()
const server = require('http').createServer(app);
const WebSocket = require('ws');

const wss = new WebSocket.Server({ server:server });

var client_info={};
var i=0
var lookup={};
//int numkey;
var mssg={};
var name={};
var data={}
wss.on('connection', function connection(ws,req) {
  console.log('A new client Connected!');
  ws.id= req.headers['sec-websocket-key'];
	client_info[ws.id]=i;
	lookup[i]=ws;
	i=i+1;
//  ws.send('Welcome New Client!');

//  console.log("id:",req.headers);
	ws.on('message', function incoming(message) {
		console.log('received: %s', message);
		mssg=JSON.parse(message);
		if(mssg[1]==="0"){
		console.log('client name:',mssg[0]);
		console.log('userid:',mssg[1]);
		name[client_info[ws.id]]=mssg[0];
		if(mssg[0]==='chat'){
		chatkey=client_info[ws.id];
		lookup[chatkey].send(JSON.stringify(mssg));
		}}else{
		dest=mssg[0];
			//mssg[0]=name[client_info[ws.id]];
			data[name[client_info[ws.id]]]=mssg[1];
		lookup[dest].send(JSON.stringify(data));
			data={};
//		lookup[chatkey].send(JSON.stringify(mssg));
		};
		console.log('client number:',client_info[ws.id]);
		//lookup[chatkey].send(name,client_info[ws.id]);
    wss.clients.forEach(function each(client) {
      if (client !== ws && client.readyState === WebSocket.OPEN) {
	      if(mssg[1]==="0"){
		      //bc=mssg[0]+":"+client_info[client.id];
		      //bc[client_info[client.id]]=name[];
		      console.log("broadcast:",client_info[client.id])
	      client.send(JSON.stringify(name));
	      }
	//client.on('message', function incoming(message) {
	//	data=JSON.parse(message);
	//	if(data[1]!=="0"){
	//	console.log("chat condition");
	//	console.log("to userid:",name[data[0]]);
	//	lookup[data[0]].send(message);
	//	};
      //});
      } else if(client===ws){
      
	      ws.send(JSON.stringify(name));
      }

    });
    });
});

app.get("/",function(req,res){
res.sendFile(__dirname+"/chat.html");
console.log("sent");

});
app.get("/client1",function(req,res){
res.sendFile(__dirname+"/client1.html");
console.log("sent");

});

server.listen(3000, () => console.log(`Lisening on port :3000`))
