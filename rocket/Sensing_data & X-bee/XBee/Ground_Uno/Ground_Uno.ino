// 회로 연결(UNO 기준)
// 없음

#include <SoftwareSerial.h> // 시리얼 통신 라이브러리
#define LEN_OF_SENSOR_ARRAY 15 // 센서 값 배열의 길이
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

  // 문자열 파싱
  int currentIndex = 0; // sensorData 배열의 현재 인덱스
  int delimiterIndex = st.indexOf('|'); // 첫 번째 구분자(,)의 인덱스
  while (delimiterIndex >= 0) {
    // 구분자 이전의 문자열을 추출하여 sensorData 배열에 저장
    String data = st.substring(0, delimiterIndex);
    sensorData[currentIndex] = data;

    // 다음 구분자 이전의 문자열을 추출하기 위해 인덱스 업데이트
    st = st.substring(delimiterIndex + 1);
    delimiterIndex = st.indexOf('|');

    currentIndex++;
    if (currentIndex >= LEN_OF_SENSOR_ARRAY) {
      // sensorData 배열의 크기를 초과하는 경우 루프를 종료
      break;
    }
  }

  // 센서 값을 초기화
  for (int i = currentIndex; i < LEN_OF_SENSOR_ARRAY; i++) {   
    sensorData[i] = "";
  }
   doc["roll"] = sensorData[0];    //ax값
   doc["pitch"] = sensorData[1];   //ay값
   doc["yaw"] = sensorData[2];     //az값
 
    doc["airx"] = sensorData[6];   //gps의 x속도
    doc["airy"] = sensorData[7];   //gps의 y속도
    doc["airz"] =sensorData[8];   //차압센서 속도값
    doc["x"] = sensorData[9];  // 위도값
    doc["y"] = sensorData[10]; // 경도값
    doc["at"] = sensorData[11]; // pressure값
    doc["z"] = sensorData[12]; // 해발고도 
    doc["t"] = sensorData[13]; // temperature값
    doc["h"] = sensorData[14]; // humidity값
  
   // JSON 직렬화 후 출력
    serializeJson(doc, Serial);

  
   
   Serial.println();
   

   

 }
