// 회로 연결(UNO 기준)
// 없음

#include <SoftwareSerial.h> // 시리얼 통신 라이브러리
#define LEN_OF_SENSOR_ARRAY 20 // 센서 값 배열의 길이
#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
StaticJsonDocument<500> doc;  // JSON 



void setup() {
  Serial.begin(2400);   // 출력용 Serial
 }

 void loop() {


  // STX ~ ETX 사이의 데이터 문자열 가져오기
  String st = "3664.00|15740.00|-2432.00|-166.00|152.00|-151.00|0.75|1.11|-4.29|37.632904|127.077423|100.39|126.21|0.00|0.00|";
   Serial.print(st);
   Serial.println();
  //st = "3664.00|15740.00|-2432.00|-166.00|152.00|-151.00|0.75|1.11|-4.29|37.632904|127.078423|100.39|126.21|0.00|0.00|";
  // Serial.print(st);
  // Serial.println();
   st = "3664.00|15740.00|-2432.00|-166.00|152.00|-151.00|0.75|1.11|-4.29|37.633904|127.077423|100.39|126.21|0.00|0.00|";
   Serial.print(st);
   Serial.println();
 }
