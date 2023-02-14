#include <Wire.h>

const int trigPin = 2;      // 초음파 센서의 Trig 핀
const int echoPin = 3;      // 초음파 센서의 Echo 핀

int16_t ax, ay, az, gx, gy, gz;  // 가속도계 및 자이로스코프 값 저장 변수

void setup() {
  Serial.begin(9600);
  Wire.begin();
}

void loop() {
  // 초음파 센서에서 거리 읽어오기
  long duration, distance;
  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  // MPU6050에서 가속도계 및 자이로스코프 값 읽어오기
  Wire.beginTransmission(0x68);
  Wire.write(0x3B);  // 시작 주소 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(0x68, 14, true);  // 총 14개 레지스터 요청
  ax = Wire.read() << 8 | Wire.read();  // X축 가속도계 값 읽어오기
  ay = Wire.read() << 8 | Wire.read();  // Y축 가속도계 값 읽어오기
  az = Wire.read() << 8 | Wire.read();  // Z축 가속도계 값 읽어오기
  Wire.read();  // 온도 값 건너뛰기
  Wire.read();  // 온도 값 건너뛰기
  gx = Wire.read() << 8 | Wire.read();  // X축 자이로스코프 값 읽어오기
  gy = Wire.read() << 8 | Wire.read();  // Y축 자이로스코프 값 읽어오기
  gz = Wire.read() << 8 | Wire.read();  // Z축 자이로스코프 값 읽어오기

  // 센서 값 배열 만들기
  int sensorData[7];
  sensorData[0] = distance;
  sensorData[1] = ax;
  sensorData[2] = ay;
  sensorData[3] = az;
  sensorData[4] = gx;
  sensorData[5] = gy;
  sensorData[6] = gz;

  // 시리얼 통신으로 센서 값 배열 보내기
  Serial.print(sensorData[0]);
  Serial.print(",");
  Serial.print(sensorData[1]);
  Serial.print(",");
  Serial.print(sensorData[2]);
  Serial.print(",");,...................................................................................................
  Serial.print(sensorData[3]);
  Serial.print(",");
  Serial.print(sensorData[4]);
  Serial.print(",");
  Serial.print(sensorData[5]);
  Serial.print(",");
  Serial.println(sensorData[6]);
}
