// 회로 연결(MEGA 기준) 
// Rocket_MEGA : 0(RX0) - 18(TX1), 1(TX0) - 19(RX1) (Rocket_MEGA_SENSOR의 0 1을 Rocket_MEGA의 18 19와 연결)
// BMP280 : SCL - 21(SCL), SDA - 20(SDA)
// DHT22  : SIG - 2
// IMU    : SCL - SCL, SDA - SDA
// GPS    : TXD - 19(RX1), RXD - 20(TX1) 
// MicroSD : MISO - 50, MOSI - 51, SCK - 52, SC - 53


#include <Wire.h>               // I2C 통신용 라이브러리
#include <Adafruit_BMP280.h>    // BMP280 라이브러리
#include <DHT.h>                // DHT22 라이브러리
#include <TinyGPS.h>            // GPS 라이브러리
#include <SD.h>                 // MicroSDCard 라이브러리
#define LEN_OF_SENSOR_ARRAY 14  // 센서 값 배열의 길이


Adafruit_BMP280 bmp;  // BMP280 객체
DHT dht(2, DHT22);    // DHT22 객체
TinyGPS gps;          // GPS 객체
File myFile;          // MicroSDCard 객체

double sensorData[LEN_OF_SENSOR_ARRAY];     // 센서 값 배열

double Pressure, Altitude;                  // 기압, 고도
double Temperature, Humidity;               // 온도, 습도
int16_t ax, ay, az, gx, gy, gz;             // 3축 가속도, 각속도
float flat, flon, speedMPS, courseDegree;  // 위도, 경도, 속도, 각도
unsigned long age;                          // GPS 정확도


void setup() {
  Serial.begin(2400);
  Wire.begin();
  bmp.begin(0x76);
  dht.begin();
  Serial1.begin(9600);  // GPS 보드레이트는 9600으로 고정
  initializeMicroSD();  
}// Setup End


void loop() {
//  BMP280에서 기압, 고도 값 읽어오기
  Pressure = bmp.readPressure()/1000;
  Altitude = bmp.readAltitude(1019.1);


// DHT22에서 온도, 습도 값 읽어오기
  Temperature = dht.readTemperature();
  Humidity = dht.readHumidity();
  
  
//  MPU9250에서 3축 가속도, 각속도 값 읽어오기
  Wire.beginTransmission(0x68);
  Wire.write(0x3B);                     // 시작 주소 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(0x68, 14, true);     // 총 14개 레지스터 요청
  ax = Wire.read() << 8 | Wire.read();  // X축 가속도계 값 읽어오기
  ay = Wire.read() << 8 | Wire.read();  // Y축 가속도계 값 읽어오기
  az = Wire.read() << 8 | Wire.read();  // Z축 가속도계 값 읽어오기
  Wire.read();  // 온도 값 건너뛰기
  Wire.read();  // 온도 값 건너뛰기
  gx = Wire.read() << 8 | Wire.read();  // X축 자이로스코프 값 읽어오기
  gy = Wire.read() << 8 | Wire.read();  // Y축 자이로스코프 값 읽어오기
  gz = Wire.read() << 8 | Wire.read();  // Z축 자이로스코프 값 읽어오기


//  GPS 센서에서 위도, 경도, 속도, 각도 값 읽어오기
  gps.f_get_position(&flat, &flon, &age);   // 위도, 경도, 정확도 얻음
  speedMPS = gps.f_speed_mps();             // 속도 얻음
  courseDegree = gps.f_course();            // 각도 얻음


//  배열에 센서 값 저장하기
  sensorData[0] = Pressure;
  sensorData[1] = Altitude;
  sensorData[2] = Temperature;
  sensorData[3] = Humidity;
  sensorData[4] = ax;
  sensorData[5] = ay;
  sensorData[6] = az;
  sensorData[7] = gx;
  sensorData[8] = gy;
  sensorData[9] = gz;
  sensorData[10] = flat;
  sensorData[11] = flon;
  sensorData[12] = speedMPS;
  sensorData[13] = courseDegree;


//  시리얼 통신으로 센서 값 배열 보내기, MicroSD에 입력하기
  for (int i = 0; i < LEN_OF_SENSOR_ARRAY; i++){
    Serial.print(char('a' + i));
    Serial.print(sensorData[i]);
    writeValue(String(sensorData[i]));
    writeValue(",");
  }
  Serial.println();
  writeValue("\n");
  smartdelay(100);
}// Loop End




// GPS용 딜레이
static void smartdelay(unsigned long ms){ 
  unsigned long start = millis();
  do 
  {
    while (Serial1.available())
      gps.encode(Serial1.read());
  } while (millis() - start < ms);
}


// MicroSD 모듈을 초기화
void initializeMicroSD(){   
  Serial.print("Initializing SD card...");

  // Mega 기준 MISO 50, MOSI 51, SCK 52, SC 53
  if (!SD.begin(53)) { // SD카드 모듈을 초기화합니다, 매개변수에 SC 핀 번호 입력
    Serial.println("initialization failed!"); // SD카드 모듈 초기화에 실패하면 에러를 출력합니다.
    while(1);
  }
  Serial.println("initialization done.");
}


// string 변수를 매개변수로 받아 MicroSD의 txt 파일에 작성
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


// MicroSD 안의 txt 파일 값을 읽어 Serial 출력
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
