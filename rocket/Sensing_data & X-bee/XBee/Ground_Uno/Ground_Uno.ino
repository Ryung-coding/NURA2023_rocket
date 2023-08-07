// 회로 연결(UNO 기준)
// 없음

#include <SoftwareSerial.h> // 시리얼 통신 라이브러리 

int XBeeRx = 0;   // Rx (받는 핀 설정)
int XBeeTx = 1;   // Tx (보내는 핀 설정)
SoftwareSerial XBeeSerial(XBeeRx, XBeeTx);  // 시리얼 통신을 위한 객체 선언

String Buff;                        // 버퍼 문자열
const char cSTX = 2, cETX = 3;      // 데이터의 시작과 끝을 구분하는 문자

void setup() {
  Serial.begin(2400);     // 출력용 Serial
  XBeeSerial.begin(2400); // XBee를 통해 값을 입력받는 Serial
}

void loop() {
  if (XBeeSerial.available() <= 0) return;  // XBee를 통해 받은 데이터가 없으면 return
  
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

  // 체크섬 검사를 하여 검사를 통과하면 데이터를 알맞게 받았다고 판단하고 st 전송
  int len = st.length();
  if (checkSum(st, len) == true){
    if (st[len - 4] == '|') st = st.substring(0,len-3);
    else st = st.substring(0,len-2);
    Serial.println(st);
  }
}

// 체크섬 검사하는 함수
bool checkSum(String dataString, int len){
  unsigned char chksum = 0xff;

  if (dataString[len - 4] == '|') for (int i = 0; i < len - 3; i++) chksum -= dataString[i];
  else for (int i = 0; i < len - 2; i++) chksum -= dataString[i];

  if (String(chksum, HEX) == (String(dataString[len-3]) + dataString[len-2]) || String(chksum, HEX) == String(dataString[len-2])) return true;
  else return false;
}
