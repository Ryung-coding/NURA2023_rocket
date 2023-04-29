// 회로 연결(UNO 기준)
// 없음

#include <SoftwareSerial.h> // 시리얼 통신 라이브러리
#define LEN_OF_SENSOR_ARRAY 10 // 센서 값 배열의 길이

int XBeeTx = 0;   // Tx (보내는핀 설정)
int XBeeRx = 1;   // Rx (받는핀 설정)
SoftwareSerial XBeeSerial(XBeeTx, XBeeRx);  // 시리얼 통신을 위한 객체선언
String myString = ""; // 받는 문자열

String sensorData[LEN_OF_SENSOR_ARRAY]; // 센서 값 배열
bool writeValue = false; // 센서 값을 배열에 넣을지 안 넣을지를 저장하는 변수
int idx; // 센서 값 배열의 인덱스
 
void setup() {
  Serial.begin(2400);   // 출력용 Serial
  XBeeSerial.begin(2400); // XBee를 통해 값을 입력받는 Serial
}
 
void loop() {
  while(XBeeSerial.available()){                                      // Xbee를 통해 값이 들어오면
    char c = XBeeSerial.read();                                       // 데이터를 1바이트 읽어옴
                                               
    if (c >= 'a'){                                                    // 만약 데이터가 알파벳(구분자)이면 myString에 쌓은 데이터 값을 알맞은 인덱스에 입력함.
      if (c == 'a') sensorData[LEN_OF_SENSOR_ARRAY - 1] = myString;                                                      
      else sensorData[c - 'a' - 1] = myString;
      myString = "";                                                                     
    }
    else
      if (c != '\n') myString += c;                                               // 데이터가 알파벳이 아니면(실제 데이터값) myString에 한 글자씩 쌓음.                                                  
  }

  printValueOfArray(sensorData,LEN_OF_SENSOR_ARRAY);                  // 데이터를 Serial 출력한다.
}

void printValueOfArray(String Array[], int LenOfArray){ // String 배열과 배열의 길이를 매개변수로 받아 배열 값을 Serial 출력한다.
  for (int i = 0; i < LenOfArray; i++){
    Serial.print(Array[i]);
    Serial.print(',');
  }
  Serial.println();
}
