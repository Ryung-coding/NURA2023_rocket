#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>

const int trigPin = 2;      // 초음파 센서의 Trig 핀
const int echoPin = 3;      // 초음파 센서의 Echo 핀

int16_t ax, ay, az, gx, gy, gz;  // 가속도계 및 자이로스코프 값 저장 변수

TinyGPS gps;
SoftwareSerial GPS(7, 6);

File myFile;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  GPS.begin(9600);
  initializeMicroSD();
}

void loop() {
//  초음파 센서로 거리 값 읽어오기
  long duration, distance;
  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  
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
  float flat, flon, speedMPS, courseDegree;
  unsigned long age, date, time;
  
  gps.f_get_position(&flat, &flon, &age);
  speedMPS = gps.f_speed_kmph() / 3.6;
  courseDegree = gps.f_course();

//   센서 값 배열 만들기
  int sensorData[7];
  sensorData[0] = distance;
  sensorData[1] = ax;
  sensorData[2] = ay;
  sensorData[3] = az;
  sensorData[4] = gx;
  sensorData[5] = gy;
  sensorData[6] = gz;
  sensorData[7] = flat;
  sensorData[8] = flon;
  sensorData[9] = speedMPS;
  sensorData[10] = courseDegree;
  

//   시리얼 통신으로 센서 값 배열 보내기

  for (int i = 0; i < 7; i++){
    Serial.print(sensorData[i]);
    Serial.print(",");
   writeValue(String(sensorData[i]));
    writeValue(",");
  }
  Serial.println();
  writeValue("\n");
}

void initializeMicroSD(){
  Serial.print("Initializing SD card...");

  // 아두이노 메가 기준 MISO 50, MOSI 51, SCK 52, SC 53
  if (!SD.begin(53)) { // SD카드 모듈을 초기화합니다, 매개변수에 SC 핀 번호 입력
    Serial.println("initialization failed!"); // SD카드 모듈 초기화에 실패하면 에러를 출력합니다.
    while (1);
  }
  Serial.println("initialization done.");
}
void writeValue(String val){
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

void openValueFile(){
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
