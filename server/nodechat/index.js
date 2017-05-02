var app = require('express')();
var http = require('http').Server(app);
var io = require('socket.io')(http);

app.get('/', function(req, res){
	res.sendFile(__dirname+'/index.html');
})

io.on('connection', function(socket){
	console.log('one user connected');

	socket.on('disconnect', function(){
		console.log('one user disconnected');
	});


	socket.on('message', function(data){
		console.log(data);
		Object.keys(io.sockets.sockets).forEach(function(id) {
			io.to(id).emit('message', data);
		});
	});
})


http.listen(3000, function(){
	console.log('server listening port 3000');
})
