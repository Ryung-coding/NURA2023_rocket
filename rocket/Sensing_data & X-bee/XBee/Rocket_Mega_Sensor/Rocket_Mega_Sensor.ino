// 회로 연결(MEGA 기준)
// Rocket_MEGA : 0(RX0) - 18(TX1) , 1(TX0) - 19(RX1) (Rocket_MEGA_SENSOR의 0 1을 Rocket_MEGA의 18 19와 연결)
// IMU : SCL - SCL , SDA - SDA
// GPS : TXD - RX1(19) , RXD - TX1(20) 
// MicroSD : MISO 50, MOSI 51, SCK 52, SC 53

#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>
#define LEN_OF_SENSOR_ARRAY 10 // 센서값 배열의 길이

int16_t ax, ay, az, gx, gy, gz;  // 가속도계 및 자이로스코프 값 저장 변수

TinyGPS gps;  // GPS 객체

File myFile;  // MicroSDCard 객체

void setup() {
  Serial.begin(2400);
  Wire.begin();
  Serial1.begin(9600);  // GPS 보드레이트는 9600으로 고정 , TX : 19  RX : 18
  initializeMicroSD();  // MicroSDCard 초기화
}

void loop() {
  
//   MPU6050에서 가속도계 및 자이로스코프 값 읽어오기
  Wire.beginTransmission(0x68);
  Wire.write(0x3B);  // 시작 주소 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(0x68, 14, true);  // 총 14개 레지스터 요청
  ax = Wire.read() << 8 | Wire.read();  // X축 가속도계 값 읽어오기
  ay = Wire.read() << 8 | Wire.read();  // Y축 가속도계 값 읽어오기
  az = Wire.read() << 8 | Wire.read();  // Z축 가속도계 값 읽어오기
  Wire.read();  // 온도 값 건너뛰기
  Wire.read();  // 온도 값 건너뛰기
  gx = Wire.read() << 8 | Wire.read();  // X축 자이로스코프 값 읽어오기
  gy = Wire.read() << 8 | Wire.read();  // Y축 자이로스코프 값 읽어오기
  gz = Wire.read() << 8 | Wire.read();  // Z축 자이로스코프 값 읽어오기


//  GPS 센서로 위도, 경도, 속도, 각도 읽어오기
  float flat, flon, speedMPS, courseDegree;   // 위도, 경도, 속도, 각도
  unsigned long age;  // 정확도
  
  gps.f_get_position(&flat, &flon, &age);   // 위도, 경도, 정확도 얻음
  speedMPS = gps.f_speed_mps();             // 속도 얻음
  courseDegree = gps.f_course();            // 각도 얻음


//   센서 값 배열 만들기
  int sensorData[LEN_OF_SENSOR_ARRAY];
  sensorData[0] = ax;
  sensorData[1] = ay;
  sensorData[2] = az;
  sensorData[3] = gx;
  sensorData[4] = gy;
  sensorData[5] = gz;
  sensorData[6] = flat;
  sensorData[7] = flon;
  sensorData[8] = speedMPS;
  sensorData[9] = courseDegree;


//   시리얼 통신으로 센서 값 배열 보내기, MicroSD카드에 입력하기
  for (int i = 0; i < LEN_OF_SENSOR_ARRAY; i++){
    Serial.print(char('a' + i));
    Serial.print(sensorData[i]);
    writeValue(String(sensorData[i]));
    writeValue(",");
  }
  writeValue("\n");
  smartdelay(100);
} // Loop End


static void smartdelay(unsigned long ms){   // GPS용 딜레이
  unsigned long start = millis();
  do 
  {
    while (Serial1.available())
      gps.encode(Serial1.read());
  } while (millis() - start < ms);
}


void initializeMicroSD(){   // MicroSD카드 모듈을 초기화한다.
  Serial.print("Initializing SD card...");

  // 아두이노 메가 기준 MISO 50, MOSI 51, SCK 52, SC 53
  if (!SD.begin(53)) { // SD카드 모듈을 초기화합니다, 매개변수에 SC 핀 번호 입력
    Serial.println("initialization failed!"); // SD카드 모듈 초기화에 실패하면 에러를 출력합니다.
    while(1);
  }
  Serial.println("initialization done.");
}


void writeValue(String val){  // string 변수를 매개변수로 받아 MicroSD카드의 텍스트 파일에 작성한다.
  // 파일을 열어 쓸 준비를 합니다. 한 번에 하나의 파일만 열 수 있습니다.
  myFile = SD.open("value.txt", FILE_WRITE); // 두 번째 인자가 있으면 쓰기모드입니다.

  if (myFile) { // 파일이 정상적으로 열리면 파일에 문자를 작성(추가)합니다.
    myFile.print(val);
    myFile.close(); // 파일을 닫습니다.
  } else {
    // 파일이 열리지 않으면 에러를 출력합니다.
    Serial.println("error opening value.txt");
  }
}


void openValueFile(){   // MicroSD카드 안의 텍스트 파일 값을 읽어 Serial 출력한다.
  // 파일을 읽기 위해 다시 엽니다. 두 번째 인자가 없으면 읽기모드입니다.
  myFile = SD.open("value.txt");
  if (myFile) {
    Serial.println("value.txt:");

    // while문을 통해 파일을 EOF(End-Of-File)까지 읽습니다.
    while (myFile.available()) {
      Serial.write(myFile.read()); // 읽을 파일이 있다면 시리얼로 출력합니다.
    }
    myFile.close(); // 파일을 닫습니다.
  } else {
    // 파일이 열리지 않으면 에러를 출력합니다.
    Serial.println("error opening value.txt");
  }
}
