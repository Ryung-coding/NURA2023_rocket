#include <Eigen30.h>
#include <EigenAVR.h>
#include <Eigen/Dense>
#include "Wire.h"
#include "I2Cdev.h"
#include "MPU9250.h"

MPU9250 accelgyro;
I2Cdev   I2C_M;

using Eigen::MatrixXd;

//**************** Define Matrix ****************//    
MatrixXd Axis_Calibration(3, 3);
MatrixXd Accel(3, 1);
MatrixXd Accel_bias(3, 1);
MatrixXd Gyro(3, 1);
MatrixXd Gyro_bias(3, 1);
MatrixXd unit(3, 1);


MatrixXd Gyro_origin_change(3, 3);

//**************** Calibration factor ****************//           
#define ax_bias 0
#define ay_bias 0
#define az_bias 0

#define gx_bias 0
#define gy_bias 0
#define gz_bias 0

#define Gxx 1
#define Gxy 0
#define Gxz 0
#define Gyx 0
#define Gyy 1
#define Gyz 0
#define Gzx 0
#define Gzy 0
#define Gzz 1

//**************** Define value ****************//    
int16_t ax, ay, az;
int16_t gx, gy, gz;
int16_t mx, my, mz;

double IMU_data[6];                            // ax ay az gx gy gz

double millisTime_i;                           // Initial Time Measurement[ms]
double millisTime_f;                           // Final time measurement[ms]
double dt = 0;                                 // Tiny time[s]
double runtime = 0;

double roll, pitch, yaw;
double roll_estimate, pitch_estimate, yaw_estimate;


//**************** Physical parameters ****************// 
#define Accel_thrust_time 1
#define Done_thrust_time 3

#define Accel_thrust_gradient 400
#define Done_thrust_gradient -30
#define decreasing_mass_gradient 10

#define thrust_max 400 //[N]
#define rocket_mass 4.5 //[kg]

double mass = 0;
double thrust = 0;

//**************** estimate parameters ****************// 
#define factor_H 1
#define factor_Q 10
#define factor_R 1

MatrixXd X(4, 1);
MatrixXd X_past(4, 1);

MatrixXd P(4, 4);
MatrixXd P_past(4, 4);

MatrixXd H(4, 4); 
MatrixXd Q(4, 4);
MatrixXd R(4, 4);



void Read_data()
{ 
   accelgyro.getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);
    
   IMU_data[0] = (double) ax / 16384;
   IMU_data[1] = (double) ay / 16384;
   IMU_data[2] = (double) az / 16384;
   IMU_data[3] = (double) gx * 250 / 32768;
   IMU_data[4] = (double) gy * 250 / 32768;
   IMU_data[5] = (double) gz * 250 / 32768;
}

void Calibration(double ax, double ay, double az, double gx, double gy, double gz)
{
  Accel(0,0) = ax;  Accel(1,0) = ay;  Accel(2,0) = az;
  Gyro(0,0) = gx;  Gyro(1,0) = gy;  Gyro(2,0) = gz;

  Accel = Axis_Calibration * Accel + Accel_bias;
  Gyro = Gyro + Gyro_bias;
}

void Estimate_thrustANDmass(double Tiny_time)
{
  //running time calculation 
  runtime += Tiny_time;

  //Thrust model
  if(runtime < Accel_thrust_time)
    thrust = Accel_thrust_gradient * runtime;
  else if(runtime > Accel_thrust_time and runtime < Done_thrust_time)
    thrust = thrust_max; 
  else
  {
    thrust = Done_thrust_gradient * runtime + thrust_max;
    thrust = thrust < 0 ? 0 : thrust; 
  }

  //mass model
  mass = runtime > Done_thrust_time ? rocket_mass : rocket_mass + decreasing_mass_gradient * (Done_thrust_time - Tiny_time);
}

void Extended_Kalman_Filter()
{

  //accel
  //accel = accel - (thrust/mass) * unit - (Coriolis_force / mass);
  Accel = Accel - (thrust/mass) * unit ; //only gravity value

  roll_estimate = atan(Accel(1,0)/sqrt(Accel(0,0)*Accel(0,0) + Accel(2,0)*Accel(2,0))); //[rad]
  pitch_estimate= atan(Accel(0,0)/sqrt(Accel(1,0)*Accel(1,0) + Accel(2,0)*Accel(2,0))); //[rad]
  yaw_estimate = 0;


  //Gyro 
  Gyro_origin_change(0,0) = 1;   Gyro_origin_change(0,1) = sin(roll) * tan(pitch);  Gyro_origin_change(0,2) = cos(roll) * tan(pitch);
  Gyro_origin_change(1,0) = 0;   Gyro_origin_change(1,1) = cos(roll);               Gyro_origin_change(1,2) = -sin(roll);
  Gyro_origin_change(2,0) = 0;   Gyro_origin_change(2,1) = sin(roll) / cos(pitch);  Gyro_origin_change(2,2) = cos(roll) / cos(pitch);
  
  Gyro = Gyro_origin_change * Gyro;
  
  //change Euler -> Quaternion
  MatrixXd Euler1(4, 1);
  MatrixXd Euler2(4, 1);
  MatrixXd Euler3(4, 1);
  
  Euler1(0,0) = cos(yaw_estimate/2);    Euler2(0,0) = cos(pitch_estimate/2);    Euler3(0,0) = cos(roll_estimate/2);
  Euler1(1,0) = 0;                      Euler2(1,0) = 0;                        Euler3(1,0) = sin(roll_estimate/2);
  Euler1(2,0) = 0;                      Euler2(2,0) = sin(pitch_estimate/2);    Euler3(2,0) = 0;
  Euler1(3,0) = sin(yaw_estimate/2);    Euler2(3,0) = 0;                        Euler3(3,0) = 0;

  MatrixXd Quaternion_measure(4, 1);
  Quaternion_measure = Euler1 * Euler2 * Euler3;

  //model
  MatrixXd Model(4, 4);
  
  Model(0,0) = 1;                      Model(1,0) = 0.5*dt *(Gyro(0,0));    Model(2,0) = 0.5*dt *(Gyro(1,0));   Model(3,0) = 0.5*dt *(Gyro(2,0));
  Model(0,1) = 0.5*dt *(-Gyro(0,0));   Model(1,1) = 1;                      Model(2,1) = 0.5*dt *(-Gyro(2,0));  Model(3,1) = 0.5*dt *(Gyro(1,0));
  Model(0,2) = 0.5*dt *(-Gyro(1,0));   Model(1,2) = 0.5*dt *(Gyro(2,0));    Model(2,2) = 1;                     Model(3,2) = 0.5*dt *(-Gyro(0,0)); 
  Model(0,3) = 0.5*dt *(Gyro(2,0));    Model(1,3) = 0.5*dt *(-Gyro(1,0));   Model(2,3) = 0.5*dt *(Gyro(0,0));   Model(3,3) = 1;

}

void setup() 
{
  Serial.begin(9600);
  Wire.begin();

  //Setting matrix
  unit(0, 0) = 0; unit(1, 0) = 0; unit(2, 0) = 1;
  for(int i=0; i<5;i++)
  {
    for(int j=0; j<5; j++)
    {
      H(i,j) = 0;
      Q(i,j) = 0;
      R(i,j) = 0;
      
      P = 0;
      P_past =0;
      
      if(i == j)
      {
        H(i,j) = factor_H;
        Q(i,j) = factor_Q;
        R(i,j) = factor_R;

        P = 1;
        P_past = 1;
      }
    }
  }

  Accel_bias(0,0) = ax_bias; Accel_bias(1,0) = ay_bias; Accel_bias(2,0) = az_bias;
  Gyro_bias(0,0) = gx_bias; Gyro_bias(1,0) = gy_bias; Gyro_bias(2,0) = gz_bias;

  Axis_Calibration(0,0) = Gxx;   Axis_Calibration(0,1) = Gxy;   Axis_Calibration(0,2) = Gxz;
  Axis_Calibration(1,0) = Gyx;   Axis_Calibration(1,1) = Gyy;   Axis_Calibration(1,2) = Gyz;
  Axis_Calibration(2,0) = Gzx;   Axis_Calibration(2,1) = Gzy;   Axis_Calibration(2,2) = Gzz;

  X(0, 0) = 1;        X(1, 0) = 0;        X(2, 0) = 0;        X(3, 0) = 0;
  X_past(0, 0) = 1;   X_past(1, 0) = 0;   X_past(2, 0) = 0;   X_past(3, 0) = 0; 
}

void loop()
{
  millisTime_f = millis();                        
  dt = (millisTime_f - millisTime_i)*0.001;          
  millisTime_i = millis();                         
    
  Read_data();
  Calibration(IMU_data[0], IMU_data[1], IMU_data[2], IMU_data[3], IMU_data[4], IMU_data[5]);
  Estimate_thrustANDmass(dt);
}
