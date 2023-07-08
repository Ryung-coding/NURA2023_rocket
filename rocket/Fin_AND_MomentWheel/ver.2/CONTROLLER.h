#ifndef CONTROLLER_H
#define CONTROLLER_H

//##############################################################################################################//

#include <Servo.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h> 

#include "I2Cdev.h"
#include "MPU9250.h"

I2Cdev I2C_M; MPU9250 mpu;
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

Servo servo_roll ; 
Servo servo_pitch;  

//----------------------------------< calibration Value >----------------------------------//

//               <---range_duty--->
//---------------|ㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁ|-----------------> BLDC pwm duty range [pca9685-unit] 
//            set_duty

#define set_duty 1700                     // setting zero angular velocity[pca9685-unit] 
#define range_duty 300                    // control duty range angular velocity[pca9685-unit] 

#define set_servo_angle 0                 // setting zero angle of servo [micro-sec]
                                          
#define sampling_time 0.015               //sampling time[s]

//----------------------------------< port setting >----------------------------------//

#define SERVO_pin_roll 9
#define SERVO_pin_pitch 10

#define BLDC_pin_cw 15
#define BLDC_pin_ccw 14

//----------------------------------< PID & filter gain setting >----------------------------------//

//roll gain                           
#define Kp_r 1  
#define Kd_r 0           

// pitch gain
#define Kp_p 1 
#define Kd_p 0 

//yaw gain
#define Kp_y 10    
#define Kd_y 0

#define bangbang_r 0    
#define bangbang_p 0  
#define bangbang_y 0

#define filter_factor_r 0.9   
#define filter_factor_p 0.9   
#define filter_factor_y 0.9

#define off_set_roll -4
#define off_set_pitch -4 

//----------------------------------< Desired attitude >----------------------------------//

#define goal_roll 0         // Goal pitch [angle of velocity]
#define goal_pitch 0        // Goal pitch [deg]
#define goal_yaw 0          // Goal yaw    [deg]

//----------------------------------< value define >----------------------------------//

//roll value                   pitch value                    yaw value
double past_roll = 0;          double past_pitch = 0;         double past_yaw = 0;
double sensor_roll = 0;        double sensor_pitch = 0;       double sensor_yaw = 0;
double u_roll = 0;             double u_pitch = 0;            double u_yaw = 0;

 
double dt = 0;         // delta_time[ms]
double start_time = 0; // Initial Time Measurement[ms]
double end_time = 0;   // Final time measurement[ms]


double imu[6] = {0}; int16_t ax = 0, ay = 0, az = 0, gx = 0, gy = 0, gz = 0;

//##################################################################################################################################################//

void Setting_sampling_time()
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
}//_______________________________________________________________________________________________________________________________________________________

void Compute_Complmentary(double ax, double ay, double az, double gx, double gy, double gz, double dt)
{
  double roll_a = atan2(ay, az) * 180 / PI;
  double pitch_a = atan2(ax, az) * 180 / PI;

  sensor_roll = filter_factor_r * (sensor_roll + gx * dt) + (1 - filter_factor_r) * roll_a;
  sensor_pitch = filter_factor_p * (sensor_pitch + gy * dt) + (1 - filter_factor_p) * pitch_a;
  sensor_yaw = filter_factor_y * sensor_yaw + (1 - filter_factor_y) * imu[5];
 
}//_______________________________________________________________________________________________________________________________________________________

void Input_data()
{
   mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    
   imu[0] = (double) ax / 16384;
   imu[1] = (double) ay / 16384;
   imu[2] = (double) az / 16384;
   imu[3] = (double) gx * 250 / 32768;
   imu[4] = (double) gy * 250 / 32768;
   imu[5] = (double) gz * 250 / 32768;
 
   Compute_Complmentary(imu[0], imu[1], imu[2], imu[3], imu[4], imu[5], dt);  
}//_______________________________________________________________________________________________________________________________________________________

double computePD_BLDC(double goal, double sensor, double sensor_past, double dt, double Kp, double Kd, double bangbang_control_range)
{
  double error = goal - sensor;                                    
  
  double P = Kp * error;  
  
  double D = Kd *(sensor - sensor_past) / dt;
  double u = P - D;
  
  if ( abs(error) <= bangbang_control_range) u = set_duty;
  
  u = constrain(set_duty + abs(u), set_duty, set_duty + range_duty);
  
  u = error > 0 ? u : -u;
 
  return u; //[us]  
}//_______________________________________________________________________________________________________________________________________________________

void control_BLDC()
{
  if( u_yaw > 0)                           // ccw case -> rotate ccw
  {
    pwm.setPWM(BLDC_pin_ccw,0,abs(u_yaw));
    //Serial.println(String(u_yaw)+",5,");
  }
  else                                      // cw case -> rotate cw
  {
    pwm.setPWM(BLDC_pin_cw,0,abs(u_yaw));   
    //Serial.println(String(u_yaw)+",-5,");
  }
}//_______________________________________________________________________________________________________________________________________________________

double computePD_SERVO(double goal, double sensor, double sensor_past, double dt, double Kp, double Kd, double bangbang_control_range)
{
  double error = goal - sensor;               // angular velocity increasing -> PID u decreasing
  
  double P = Kp * error;  
  double D = Kd *(sensor - sensor_past) / dt;
  
  double u = P - D;
  if ( abs(error) <= bangbang_control_range) u = set_servo_angle;

  return u;
}//_______________________________________________________________________________________________________________________________________________________

void control_SERVO()
{
  servo_roll.writeMicroseconds(u_roll);
  servo_pitch.writeMicroseconds(u_pitch);
  delay(10);
}

//##############################################################################################################//
#endif
