// 회로 연결(Due 기준)
// Rocket_Due : 18(TX1) - 17(RX2) , 19(RX1) - 16(TX2) (Rocket_Due의 18 19을 Rocket_Mega_Sensor의 17 16와 연결)

void setup() {
  Serial.begin(2400);
  Serial1.begin(2400);  // Rocket_Mega_Sensor와의 Serial 통신
}

void loop() {
  if (Serial1.available()){         // 만약 Rocket_Mega_Sensor로부터 값이 들어오면
     int inByte = Serial1.read();   // 값을 읽어서
     Serial.write(inByte);          // Serial 출력
  }
}
