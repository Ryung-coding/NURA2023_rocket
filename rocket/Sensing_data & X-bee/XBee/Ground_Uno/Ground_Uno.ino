// 회로 연결(UNO 기준)
// 없음

#include <SoftwareSerial.h> // 시리얼 통신 라이브러리
#define LEN_OF_SENSOR_ARRAY 14 // 센서 값 배열의 길이

int XBeeTx = 0;   // Tx (보내는핀 설정)
int XBeeRx = 1;   // Rx (받는핀 설정)
SoftwareSerial XBeeSerial(XBeeTx, XBeeRx);  // 시리얼 통신을 위한 객체선언
String Buff;          // Buffer 문자열
String myString = ""; // 받는 문자열
const char cSTX = 2, cETX = 3;

String sensorData[LEN_OF_SENSOR_ARRAY]; // 센서 값 배열
bool writeValue = false; // 센서 값을 배열에 넣을지 안 넣을지를 저장하는 변수
int idx; // 센서 값 배열의 인덱스



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
  if(ipos0 < 0) return;
  int ipos1 = Buff.indexOf(cETX, ipos0);
  if(ipos1 < 0) return;

  // STX ~ ETX 빼고 내부만 얻기
  String st = Buff.substring(ipos0+1, ipos1);

  // gBuff 업데이트
  Buff = Buff.substring(ipos1 + 1);

  // 데이터 문자열을 ,로 구분하여 각 데이터를 배열에 저장하기
  stSplit(st);

  printValueOfArray(sensorData,LEN_OF_SENSOR_ARRAY);    // 데이터를 Serial 출력한다.
}


void printValueOfArray(String Array[], int LenOfArray){ // String 배열과 배열의 길이를 매개변수로 받아 배열 값을 Serial 출력한다.
  for (int i = 0; i < LenOfArray; i++){
    Serial.print(Array[i]);
    Serial.print(',');
    Serial.print(' ');
  }
  Serial.println();
}

void stSplit(String& stOrigin) {
  
  int length = stOrigin.length();
  String temp = "";
  int indexOfSensorData = 0;

  for (int i = 0; i < length; i++){
    if (stOrigin[i] == ','){
      sensorData[indexOfSensorData++] = temp.toDouble();
      temp = "";
    }
    else temp += stOrigin[i];
  }
}
