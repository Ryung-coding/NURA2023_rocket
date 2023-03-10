/*
   라이브러리를 설치 하고 시작해야 한다.
   1. https://github.com/adafruit/Adafruit_Sensor
   2. https://www.arduino.cc/reference/en/libraries/dht-sensor-library/
   3. json 라이브러리 arduinojson 을 검색 설치 
*/

#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

StaticJsonDocument<200> doc;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i = 0; i < 200; i++)
  {
  doc["x"] = i;
  doc["y"] = i;
  doc["z"] = i;
  doc["ex"] = 50.3;
  doc["ey"] = 30;
  doc["pitch"] = float(i)/100;
  doc["roll"] = float(i)/100;
  doc["yaw"] = float(i)/100;
  doc["t"] = 17;
  doc["h"] = 40.5;
  serializeJson(doc, Serial);
  Serial.println();
  }
  

  delay(100);
}
