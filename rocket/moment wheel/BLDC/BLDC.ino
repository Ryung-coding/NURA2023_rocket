#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm=Adafruit_PWMServoDriver();

int duty = 0; //(%) 1000~2000us 
int motor_speed = 0;

void setup() 
{
  Serial.begin(9600);
  pinMode(4,INPUT);
  pwm.begin();
  pwm.setPWMFreq(380); //->400hz로 측정됨 = 2.5ms
  Serial.println("setting time");
  
  pwm.setPWM(0,0,3225); //1당 0.620us로 계산됨, 2000us
  delay(1000);
  Serial.println("HIGH set");
  pwm.setPWM(0,0,1613); //1당 0.620us 1000us
  delay(1000);
  Serial.println("LOW set");
  
}


void loop() 
{
  //int PWM = constrain(map(duty, 0, 100, 0, ), 0, 4032); 
  for (motor_speed = 1613; motor_speed<1935;motor_speed+=100)// 1000us에서 1935us 까지 증가
  {
     pwm.setPWM(0,0,motor_speed); //1당 0.620us
    delay(100);
    Serial.print(motor_speed);
    Serial.print(" = ");
    Serial.println(map(motor_speed, 1613, 3225, 0, 100));
  }

    for (motor_speed = 1935; motor_speed>1613;motor_speed-=100)
  {
    pwm.setPWM(0,0,motor_speed); //1당 0.620us
    delay(100);
    Serial.print(motor_speed);
    Serial.print(" = ");
    Serial.println(map(motor_speed, 1613, 3225, 0, 100));
  }


}
