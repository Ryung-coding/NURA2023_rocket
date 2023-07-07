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
  String st = "3664.00|15740.00|-2432.00|-166.00|152.00|-151.00|10|5|20|37.632904|127.077423|1000.39|126.21|30.00|40.00|";
   Serial.print(st);
   Serial.println();
  st = "3664|15740.00|-2432.00|-166.00|152.00|-151.00|5|10|50|37.633904|127.078423|900.39|140.21|20.00|50.00|";
   Serial.print(st);
   Serial.println();
   st = "3664|15740.00|-2432.00|-166.00|152.00|-151.00|10|20|30|37.631904|127.079423|400.39|150.21|10.00|50.00|";
   Serial.print(st);
   Serial.println();
 }
