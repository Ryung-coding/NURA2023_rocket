#include <Eigen30.h>
#include <EigenAVR.h>
#include <Eigen/Dense>

#define sampling_time 0.003 //s
#define filter_factor 0.99
double ax = 0, ay = 0, az = 0, gx = 0, gy = 0, gz = 0;

double dt = 0;         // delta_time[ms]
double start_time = 0; // Initial Time Measurement[ms]
double end_time = 0;   // Final time measurement[ms]

double roll_filter = 0;
double pitch_filter = 0;


 void Input_data()
 {
  //read list = Serial.read; -> a12345b12345c123456
  // 촵촵 짤라
 }

 void Compute_Complmentary(double ax,double ay, double az, double gx,double gy, double gz, double dt)
 {
  double roll_a  = atan2(ay, az) * 180 / PI;
  double pitch_a  = atan2(ax, az) * 180 / PI;
  
//  double yaw_a = atan2(ax, ay) * 180 / PI;
//  double  roll_g += gx * dt;
//  double  pitch_g += gy * dt;
//  double  yaw_g += gz * dt;

  roll_filter = filter_factor * (roll_filter + gx * dt) + (1 - filter_factor) * roll_a;
  pitch_filter = filter_factor  * (pitch_filter + gy * dt) + (1 - filter_factor) * pitch_a;
 }

void setup()
{
  Serial.begin(9600);
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
  
  Input_data();
  
  compute_Euler_Angle(ax, ay, az, gx, gy, gz, dt);
  
  
  delay(10);
}
