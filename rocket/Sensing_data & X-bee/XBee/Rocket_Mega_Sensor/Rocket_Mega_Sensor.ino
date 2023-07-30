// 회로 연결(MEGA 기준) 
// Rocket_Mega : 0(RX0) - 18(TX1), 1(TX0) - 19(RX1) (Rocket_Mega_Sensor의 0 1을 Rocket_Mega의 18 19와 연결)
// Rocket_Due  : 16(TX2) - 19(RX1), 17(RX2) - 18(TX1) (Rocket_Mega_Sensor의 16 17을 Rocket_Due의 19 18과 연결)
// BMP280 : SCL - 21(SCL), SDA - 20(SDA)
// DHT22  : SIG - 2
// IMU    : SCL - SCL, SDA - SDA
// GPS    : TXD - 19(RX1), RXD - 18(TX1) 
// MPXV7002DP : ANALOG - A0
// MicroSD : MISO - 50, MOSI - 51, SCK - 52, SC - 53

// ===================== 헤더파일 선언 =====================
#include <avr/wdt.h>            // Watchdog Timer 라이브러리
#include <Wire.h>               // I2C 통신용 라이브러리
#include <Adafruit_BMP280.h>    // BMP280 라이브러리
#include <DHT.h>                // DHT22 라이브러리
#include <TinyGPS.h>            // GPS 라이브러리
#include <SD.h>                 // MicroSDCard 라이브러리
#define LEN_OF_SENSOR_ARRAY 15  // 센서 값 배열의 길이

#define Criteria_for_evaluating_altitude 17000   // 고도 안전 사출 기준 높이
#define Criteria_for_evaluating_time 9000       // 타이머 사출 기준 시간 


// ===================== 센서 객체 선언 ====================
Adafruit_BMP280 bmp;  // BMP280 객체
DHT dht(2, DHT22);    // DHT22 객체
TinyGPS gps;          // GPS 객체
File myFile;          // MicroSDCard 객체
File GPSFile;         // GPS SDCard 객체


// ================= 센서 값 배열, 변수 선언 ================
double sensorData[LEN_OF_SENSOR_ARRAY];     // 센서 값 배열

double Pressure, Altitude;                  // 기압, 고도
double Temperature, Humidity;               // 온도, 습도
int16_t ax, ay, az, gx, gy, gz;             // 3축 가속도, 각속도
float flat, flon, speedGPS, speedGPSx, speedGPSy, courseDegree;   // 위도, 경도, 속도, x축 속도, y축 속도, 각도
unsigned long age;                          // GPS 정확도
double speedDP, speedZ;                     // 차압 센서 속도
double prevPressure, prevAltitude;                             
double prevTemperature, prevHumidity;               
int16_t prevax, prevay, prevaz, prevgx, prevgy, prevgz;             
float prevFlat, prevFlon, prevSpeedGPS, prevCourseDegree,DegreeYaw; 
double prevSpeedDP;  
unsigned long prevAge;  

double count =0 ;                              //로켓 안전 사출 변수

double Par_Pretime=0;                       //로켓 타이머 사출용 변수
double Par_Curtime=0;

double Par_Endtime=0;


bool Par_flag=true;                        //로켓 타이머 사출용 flag
bool Par_Endflag=true;
bool Par_Safeflag=true;
bool Par_Safeflag2=true;



bool sendData;                              // 데이터를 보낼지 말지 결정하는 변수
const char cSTX = 2, cETX = 3;              // 데이터의 시작과 끝을 구분하는 문자
unsigned long curTime;                      // 현재 시간을 저장하는 변수             

// ================ MPXV7002DP 관련 상수  =================
int offset = 0;
float alpha = 0.1;
double air = 1.1839;                        //공기밀도 kg/m3

void setup() {
  Serial.begin(2400);   // XBee Mega와의 Serial 통신
  Serial2.begin(2400);  // Due와의 Serial 통신
  Wire.begin();   
  bmp.begin(0x76);
  dht.begin();
  Serial1.begin(9600);  // GPS 보드레이트는 9600으로 고정
  initializeMicroSD();
  wdt_enable(WDTO_1S);  // Watchdog Timer 1초
  sendData = true;
  pinMode(9,OUTPUT);
  for(int i = 0; i < 10; i++){
    offset += analogRead(A0) - 512;
  } offset = offset / 10;
}// Setup End


 
void loop() {
// Watchdog Timer 리셋
  wdt_reset();  

// MicroSD txt파일 열기
  myFile = SD.open("value.txt", FILE_WRITE);
  GPSFile = SD.open("GPS.txt", FILE_WRITE);

// BMP280에서 기압, 고도 값 읽어오기
  Pressure = bmp.readPressure()/1000;
  Altitude = bmp.readAltitude(1019.1);

// DHT22에서 온도, 습도 값 읽어오기
  Temperature = dht.readTemperature();
  Humidity = dht.readHumidity();
  curTime = millis();
  
// MPU9250에서 3축 가속도, 각속도 값 읽어오기
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

// GPS 센서에서 위도, 경도, 속도, 각도 값 읽어오기
  gps.f_get_position(&flat, &flon, &age);               // 위도, 경도, 정확도 얻음
  speedGPS = gps.f_speed_mps();                         // 속도 얻음
  courseDegree = gps.f_course();                        // 각도 얻음
  speedGPSx = speedGPS * sin(courseDegree * PI / 180);  // 속도와 각도로부터 x축 속도 얻어냄
  speedGPSy = speedGPS * cos(courseDegree * PI / 180);  // 속도와 각도로부터 y축 속도 얻어냄

// MPXV7002DP에서 속도 값 읽어오기
  float adc = 0;
  for(int i = 0; i < 100; i++){
    adc += analogRead(A0) - offset;
  } adc = adc/100;
  //  512
  if (adc > 514)      speedDP = sqrt((2000*(5*adc/1023.0 - 2.5))/air);   
  else if (adc < 510) speedDP = -sqrt((-2000*(5*adc/1023.0 - 2.5))/air);


// 센서 값들이 이상하지 않은지 체크하기
  checkSensorData();
  Parachute();

// 센서 값들 배열에 저장하기
  putSensorDataIntoArray();
  
// 시리얼 통신으로 센서 값 배열 보내기, MicroSD에 입력하기
  if (sendData){
    Serial.print(char(cSTX));
    Serial2.print(char(cSTX));

    for (int i = 0; i < LEN_OF_SENSOR_ARRAY; i++){
      
      // 데이터가 위도 혹은 경도이면 소수점 아래 6자리까지 전송
      if (i == 9 || i == 10){                    
        Serial.print(sensorData[i], 6);
        myFile.print(sensorData[i], 6);
      }
      
      // 그 외에는 원래대로 전송
      else{                                      
        Serial.print(sensorData[i]);
        myFile.print(sensorData[i]);
      }
      Serial.print('|');
      myFile.print('|');
    }

    Serial.print(getCheckSum(&sensorData[0]));  // 체크섬 계산하여 전송
    Serial.print('|');

    myFile.print(curTime);
    myFile.print('|');

    // GPS SDCard에 위도, 경도 값 작성
    GPSFile.print("{latlng: new kakao.maps.LatLng(");
    GPSFile.print(sensorData[9], 6);
    GPSFile.print(", ");
    GPSFile.print(sensorData[10], 6);
    GPSFile.println(")},");    

    // DUE에는 각,가속도, 속도 값만 전송
    for(int i = 0; i < 9; i++){
      Serial2.print(sensorData[i]);
      Serial2.print(',');
    }
    
    Serial.print(char(cETX));
    Serial2.print(char(cETX));
    Serial.println();
    Serial2.println();
    myFile.println();
    smartdelay(100);

    myFile.close(); // 파일을 닫습니다.
    GPSFile.close();
  }
}// Loop End



void checkSensorData(){
  // BMP280 데이터 체크
  if (isnan(Altitude)){
    Pressure = prevPressure;
    Altitude = prevAltitude;
  }
  if (Altitude > 1462){
    Pressure = prevPressure;
    Altitude = prevAltitude;
    bmp.begin(0x76);
  }
  if (Pressure == 0 && Altitude == 0){
    Pressure = prevPressure;
    Altitude = prevAltitude;
    bmp.begin(0x76);
  }

  // DHT22 데이터 체크
  if (isnan(Temperature)){
    Temperature = prevTemperature;
    Humidity = prevHumidity;
  }
  
  // MPXV7002DP 데이터 체크
  if (isnan(speedDP)){
    speedDP = prevSpeedDP;
  }
    
  // MPU9250 데이터 체크
  if (ax == -1 && ay == -1){
    ax = prevax;  ay = prevay;  az = prevaz;  gx = prevgx;  gy = prevgy;  gz = prevgz;
  }
  if (ax == 0 && ay == 0) sendData = false;
  else sendData = true;
}

// Low Pass Filter(float 자료형)
float floatLowPassFilter(float curData, float prevData, float sensitivity)
{
  return curData * sensitivity + prevData * (1 - sensitivity);
}

// 전송 데이터의 체크섬을 계산
String getCheckSum(double* Array){
  String st = "";
  
  for (int i = 0; i < 15; i++){
    if (i == 9 || i == 10) st += String(Array[i], 6);
    else st += String(Array[i]);
    st += '|';
  }

  unsigned char chksum = 0xff;
  int len = st.length();
  for (int i = 0; i < len; i++){
    chksum -= st[i];
  }
  return String(chksum, HEX);
}


void putSensorDataIntoArray(){
  // 이전 센서 값에 현재 센서 값 저장
  prevPressure = Pressure;  prevAltitude = Altitude;  // BMP280
  prevTemperature = Temperature;  prevHumidity = Humidity;  // DHT22
  prevax = ax;  prevay = ay;  prevaz = az;  prevgx = gx;  prevgy = gy;  prevgz = gz;  // MPU9250
  prevSpeedDP = speedDP; // MPXV7002DP
  
  // 센서 값 배열에 각 센서 값 저장
  sensorData[0] = ax;
  sensorData[1] = ay;
  sensorData[2] = az;
  sensorData[3] = gx;
  sensorData[4] = gy;
  sensorData[5] = gz;
  sensorData[6] = speedGPSx;
  sensorData[7] = speedGPSy;
  sensorData[8] = speedDP;
  sensorData[9] = flat;
  sensorData[10] = flon;
  sensorData[11] = Pressure;
  sensorData[12] = Altitude;
  sensorData[13] = Temperature;
  sensorData[14] = Humidity;
}


// GPS용 딜레이
static void smartdelay(unsigned long ms){ 
//  unsigned long start = millis();
//  do 
//  {
//    while (Serial1.available())
//      gps.encode(Serial1.read());
//  } while (millis() - start < ms);
  while (Serial1.available()) gps.encode(Serial1.read());
}



// MicroSD 모듈을 초기화
void initializeMicroSD(){   
  Serial.print("Initializing SD card...");

  // Mega 기준 MISO 50, MOSI 51, SCK 52, SC 53
  if (!SD.begin(53)) { // SD카드 모듈을 초기화합니다, 매개변수에 SC 핀 번호 입력
    Serial.println("initialization failed!"); // SD카드 모듈 초기화에 실패하면 에러를 출력합니다.
  }
  Serial.println("initialization done.");
}


//낙하산 사출 함수
void Parachute(){

Par_Curtime = millis();                                                                  //로켓 전원 인가 후 현재 시간 

if(((ay>15000)&&(Par_flag)))
{
  Par_Pretime=millis();                                                                    //그 때의 시간을 기록한다.
  Par_flag = false;                                                                        //다시 시간을 기록하는 일이 없도록 off 한다.
}

if(((!Par_flag)&&((Par_Curtime-Par_Pretime>Criteria_for_evaluating_time)&&(Par_Curtime-Par_Pretime<(Criteria_for_evaluating_time+3000))))&&(Par_Safeflag))
{                                                                                        //[현재 시간 - 로켓 발사 시간]이 [기준 시간] 보다 크고 3000ms 동안만
  
 digitalWrite(9,HIGH);                                                                   //니크롬선을 가열한다.
 Par_Safeflag2=false;

}                                                                                        //속도가 처음으로 5m/s를 통과했다면(로켓이 발사된 시간) 
else
{
  if(!Par_Safeflag2)digitalWrite(9,LOW); 
}



if(Altitude>Criteria_for_evaluating_altitude) //일정고도 이상이 아니면 타이머 사출 함수를 제외한 낙하산 사출 함수는 작동하지 않는다.
{
  
  if((Altitude-prevAltitude<0))                 //하강 중이라면 count 변수를 1 증가시킨다.
  {
    count++;
  }
  else                                        //상승 중이라면 count 변수를 1 이상일 때만 1 감소시킨다.
  {
    if(count>0)count--;
  }
  

  if((count>5)&&(Par_Endflag))                                                           //count 변수가 20 이상 되었다면 시간을 기록하고 니크롬선을 가열한다.
  {
    Par_Endtime=millis();
    Par_Endflag=false;
  }           
  
  
  if(((!Par_Endflag))&&((Par_Curtime-Par_Endtime)<3000))
  {
    digitalWrite(9,HIGH);
    Par_Safeflag=false;
  }                          // 니크롬선을 가열했고 이후 시간이 3000ms 지났다면 니크롬선 가열을 멈춰라

  else
  {
    if(!Par_Safeflag)digitalWrite(9,LOW);

  }



}




}                                             //Parachute 함수 끝
