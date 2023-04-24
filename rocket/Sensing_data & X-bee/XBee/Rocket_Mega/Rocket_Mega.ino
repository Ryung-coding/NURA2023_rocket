// 회로 연결(MEGA 기준)
// Rocket_MEGA : 18(TX1) - 0(RX0) , 19(RX1) - 1(TX0) (Rocket_MEGA의 18 19을 Rocket_MEGA_Sensor의 0 1와 연결)

void setup() {
  Serial.begin(2400);
  Serial1.begin(2400);  // 센서 달린 Mega와의 Serial 통신
}

void loop() {
  if (Serial1.available()){         // 만약 센서 Mega로부터 값이 들어오면
     int inByte = Serial1.read();   // 값을 읽어서
     Serial.write(inByte);          // Serial 출력(XBee로도 출력됨)
  }
}
    
