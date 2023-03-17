#include <SoftwareSerial.h>
#include "TinyGPS.h"

TinyGPS gps;  // GPS 클래스 객체
SoftwareSerial GPS(7, 6);  // TX : 7 , RX : 6

void setup() {
  Serial.begin(115200);
  GPS.begin(9600);  // GPS 보더레이트는 9600으로 고정
}

void loop() {
  float latitude, longitude, speedMPS, courseDegree;  // 위도, 경도, 속도, 방향
  unsigned long age;

  gps.f_get_position(&latitude, &longitude, &age);
  speedMPS = gps.f_speed_mps();
  courseDegree = gps.f_course();
  
  Serial.print("Age : ");
  Serial.print(age);
  Serial.print("   Latitude : ");
  Serial.print(latitude, 6);;
  Serial.print("   Longitude : ");
  Serial.print(longitude, 6);
  Serial.print("   Speed : ");
  Serial.print(speedMPS);
  Serial.print("   Course : ");
  Serial.print(courseDegree);
  Serial.println();

  smartdelay(100);
}


static void smartdelay(unsigned long ms){  // GPS 전용 딜레이
  unsigned long start = millis();
  do 
  {
    while (GPS.available())
      gps.encode(GPS.read());
  } while (millis() - start < ms);
}
