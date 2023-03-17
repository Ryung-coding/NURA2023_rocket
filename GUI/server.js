const { SerialPort } = require('serialport')
const { ReadlineParser } = require('@serialport/parser-readline')
const port = new SerialPort({ path: 'COM6', baudRate: 9600 })
const parser = new ReadlineParser()
port.pipe(parser)


const express = require('express');
const app = express();
const http = require('http');
const server = http.createServer(app);
const { Server } = require("socket.io");
const io = new Server(server);
var fs = require('fs')


app.get('/', (req, res) => {
	res.sendFile(__dirname + "/public/index.html"); // html 문서를 지정
});

app.get('/image1', function(req, res) {
	fs.readFile('./public/1.jpg', function(error, data) {
		res.writeHead(200, {'Context-Type' : 'text/html'});
		res.end(data);
	});
});
app.get('/image2', function(req, res) {
	fs.readFile('./public/rocket_position.png', function(error, data) {
		res.writeHead(200, {'Context-Type' : 'text/html'});
		res.end(data);
	});
});
//소켓 연결시
io.on('connection', (socket) => {
	console.log('a user connected');
	socket.on('disconnect', () => {
		console.log('user disconnected');
	});

	socket.emit('result', `${socket.id}로 연결 되었습니다.`);
	parser.on('data', function(data) {
			console.log(data);
			//data를 가공
			//여기부터
			// var spawn = require('child_process').spawn;
			// // 2. spawn을 통해 "python 파이썬파일.py" 명령어 실행
			// const result = spawn('py', ['./pypy.py', 'param1', 'param2']);
			// // 3. stdout의 'data'이벤트리스너로 실행결과를 받는다.
			// result.stdout.on('data', function(data) {
			// 		console.log("111", data.toString());
			// })

			// // 4. 에러 발생 시, stderr의 'data'이벤트리스너로 실행결과를 받는다.
			// result.stderr.on('data', function(data) {
			// 		console.log("222", data.toString());
			// });
			//여기까지 파이썬 실행코드
			//소켓 발신
			socket.emit('data', data);
	});


	socket.on('message', (msg) => { //받고
		console.log("클라이언트의 요청이 있습니다.");
		console.log(msg);
		socket.emit('result', `수신된 메세지는 "${ msg }" 입니다.`);
	});
});


server.listen(3000, () => {
    console.log("server is listening at localhost: 3000"); //localhost:3000으로 접속
});