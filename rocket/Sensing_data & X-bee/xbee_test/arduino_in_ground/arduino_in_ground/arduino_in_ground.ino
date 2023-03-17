
#include <SoftwareSerial.h> //시리얼 통신 라이브러리 호출
 
int blueTx=0;   //Tx (보내는핀 설정)
int blueRx=1;   //Rx (받는핀 설정)
SoftwareSerial mySerial(blueTx, blueRx);  //시리얼 통신을 위한 객체선언
String myString=""; //받는 문자열
 
void setup() {
  Serial.begin(4800);   //시리얼모니터 
  mySerial.begin(4800); //시리얼 개방
  pinMode(13,OUTPUT);   //Pin 13을 OUTPUT으로 설정 (LED ON/OFF)
}
 
void loop() {
  while(mySerial.available())  //mySerial 값이 있으면
  {
    char c = mySerial.read(); // 데이터를 1바이트 읽어옴
    if (c == ']') { // 수신이 완료되었을 때
      myString+="]";
      Serial.println(myString); // 수신된 문자열을 시리얼 모니터에 출력
      myString = ""; // myString 변수 초기화
    } else { // 수신 중일 때
      myString += c; // myString 변수에 문자를 추가
    }
   
}}
