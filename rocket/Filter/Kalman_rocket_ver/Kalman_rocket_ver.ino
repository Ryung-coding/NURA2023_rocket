#include "kalman_coefficient.h"

double roll_filter = 0, pitch_filter = 0, yaw_filter = 0;
double  roll_g = 0, pitch_g = 0, yaw_g = 0;


double Thrust(double Thrust_start_time, double now_time)
{
  #define Thrust_end_time 1.6958 //s
  #define Thrust_slope 71.586 //N/s
  #define Thrust_Intercept 351.78 //N

//                      Thrust[N]
//                        ^  
//                        |    Thrust_slope
//                        |       /|
//                        |      / |
//                        |     /  | 
//                        |    /   |
//                        |   /    |
//                        |  /     |
//                        | /      |
//     Thrust_Intercept   |/       |
//                        |        |  
//                        |        |  Thrust_end_time
//                      - - - - - - - - - - - - - - - - - >time[s]
//                 Thrust_start_time      

  double Thrust = now_time - Thrust_start_time > 0 and now_time - Thrust_start_time < Thrust_end_time ? Thrust_slope * (now_time - Thrust_start_time) + Thrust_Intercept : 0;
  return Thrust;
}


 void Compute_Kalman(double ax,double ay, double az, double gx,double gy, double gz, double dt)
 {
  
 }




 

void setup()
{
  Serial.begin(2400);
  Serial1.begin(2400);  // Rocket_Mega_Sensor와의 Serial 통신
  Serial2.begin(2400);  // Rocket_Mega_Actuator와의 Serial 통신
  Input_data();
}

void loop()     
{
  end_time = millis();
  dt = (end_time - start_time)*0.001; //[s] 1000ms -> 0.001 = 1s
  start_time = millis();

  while(dt < sampling_time)
  {
    delayMicroseconds(1);
    end_time = millis();
    dt = (end_time - start_time)*0.001; 
  }
  
  if (Serial1.available() <= 0) return;   // Serial1으로 들어온 값이 없으면 종료

  char c = Serial1.read();
  buff += c;

  // STX ~ ETX 찾기
  int ipos0 = buff.indexOf(cSTX);
  if (ipos0 < 0) return;
  int ipos1 = buff.indexOf(cETX, ipos0);
  if (ipos1 < 0) return;

  // STX ~ ETX 빼고 내부만 얻기
  input_data = buff.substring(ipos0+1, ipos1);

  // gBuff 업데이트
  buff = buff.substring(ipos1 + 1);

  Input_data();
  
  Compute_Kalman(ax, ay, az, gx, gy, gz, dt);
  
      Serial.print(ax);
      Serial.print("|");
      Serial.print(ay);
      Serial.print("|");
      Serial.print(az);
      Serial.print("|");
      Serial.print(gx);
      Serial.print("|");
      Serial.print(gy);
      Serial.print("|");
      Serial.println(gz);
  
  Output_data(roll_filter, pitch_filter, yaw_filter);
}
