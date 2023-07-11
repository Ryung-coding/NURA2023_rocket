const { SerialPort } = require('serialport');
const { ReadlineParser } = require('@serialport/parser-readline');
const port = new SerialPort({ path: 'COM6', baudRate: 2400 }); // 9600원래 6번
const parser = new ReadlineParser();
port.pipe(parser);

const express = require('express');
const app = express();
const http = require('http');
const server = http.createServer(app);
const { Server } = require("socket.io");
const io = new Server(server);
var fs = require('fs');
var RecordTF = 0;
var replayTF = 0;
let today = new Date();
let year = today.getFullYear(); // 년도
let month = today.getMonth() + 1; // 월
let date = today.getDate(); // 날짜
let hours = today.getHours(); // 시
let minutes = today.getMinutes(); // 분

app.use(express.static('public'));
app.get('/', (req, res) => {
  res.sendFile(__dirname + "/public/index.html"); // html 문서를 지정
});


// 소켓 연결시
io.on('connection', (socket) => {
  console.log('a user connected');
  socket.on('disconnect', () => {
    console.log('user disconnected');
  });

  socket.emit('result', `${socket.id}로 연결 되었습니다.`);

  parser.on('data', function (data) {
	if(replayTF == 0) {
    if (RecordTF == 1) {
      fs.appendFile(year + '_' + month + '_' + date + '_' + hours + minutes + '.txt', data + '\n', function (err) {
        if (err) throw err;
        console.log('Data saved!');
      });
    }
    socket.emit('data', data);
	}
  });

  socket.on('message', (msg) => {
    console.log("클라이언트의 요청이 있습니다.");
    console.log(msg);
    if (msg.startsWith('replay')) {
      socket.emit('flag', '1');
		  replayTF = 1;
      const replayDate = msg.split(' ')[1];
      const filePath = `${replayDate}.txt`;
      let lines = [];

      fs.readFile(filePath, 'utf-8', function (err, fileData) {
        if (err) {
          socket.emit('result', 'Error reading data file');
        } else {
          lines = fileData.split('\n');
          let lineIndex = 0;
         
          const lineInterval = setInterval(() => {
            if (lineIndex < lines.length) {
              const lineData = lines[lineIndex];
              socket.emit('data', lineData);
              lineIndex++;
            } else {
              clearInterval(lineInterval);
              socket.emit('flag', '2');
			  replayTF = 0;
            }
          }, 1000); // 일정 시간 간격으로 전송. 원래 500
        }
      });
    } else if (msg === 'record') {
      RecordTF = 1;
    } else if (msg === 'stop') {
      RecordTF = 0;
    }
    socket.emit('result', `수신된 메세지는 "${msg}" 입니다.`);
  });
});

server.listen(3000, '0.0.0.0',() => {
  console.log("server is listening at localhost: 3000"); //localhost:3000으로 접속
});
