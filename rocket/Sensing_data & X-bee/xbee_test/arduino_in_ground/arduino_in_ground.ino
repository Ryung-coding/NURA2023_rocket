
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
   
    char myChar = mySerial.read();  //mySerial int형식의 값을 char형식으로 변환
    myString+=myChar;   //수신되는 문자열을 myString에 모두 붙임 (1바이트씩 전송되는 것을 모두 붙임)
    delay(10);           //수신 문자열 끊김 방지
  }
   if(!myString.equals(""))  //myString 값이 있다면
  {
    
    Serial.print(myString); //시리얼모니터에 myString값 출력
   

  }
  myString="";
}
