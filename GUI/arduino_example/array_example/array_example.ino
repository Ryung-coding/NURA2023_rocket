// 회로 연결(UNO 기준)
// 없음

#include <SoftwareSerial.h> // 시리얼 통신 라이브러리
#define LEN_OF_SENSOR_ARRAY 20 // 센서 값 배열의 길이
#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
StaticJsonDocument<500> doc;  // JSON 

int XBeeTx = 0;   // Tx (보내는 핀 설정)
int XBeeRx = 1;   // Rx (받는 핀 설정)
SoftwareSerial XBeeSerial(XBeeTx, XBeeRx);  // 시리얼 통신을 위한 객체 선언
String Buff;          // 버퍼 문자열
const char cSTX = 2, cETX = 3;

String sensorData[LEN_OF_SENSOR_ARRAY]; // 센서 값 배열

void setup() {
  Serial.begin(2400);   // 출력용 Serial
  XBeeSerial.begin(2400); // XBee를 통해 값을 입력받는 Serial
}

 void loop() {
  if (XBeeSerial.available() <= 0) return;

  // 수신내용 버퍼에 추가
  char c = XBeeSerial.read();
  Buff += c;

  // STX ~ ETX 찾기
  int ipos0 = Buff.indexOf(cSTX);
  if (ipos0 < 0) return;
  int ipos1 = Buff.indexOf(cETX, ipos0);
  if (ipos1 < 0) return;

  // STX ~ ETX 사이의 데이터 문자열 가져오기
  String st = Buff.substring(ipos0 + 1, ipos1);

  // 버퍼 업데이트
  Buff = Buff.substring(ipos1 + 1);



  
   Serial.print(st);
    
  
   Serial.println();
 }
