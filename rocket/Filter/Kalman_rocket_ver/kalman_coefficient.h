#ifndef KALMAN_COEFFICIENT_H 
#define KALMAN_COEFFICIENT_H

//##############################################################################################################//


//**************** Eigen 라이브러리 설정 ****************// 
//https://github.com/Ryung-coding/NURA2023_rocket/tree/master/rocket/Filter 에 있는 zip파일로 추가

#include <Eigen30.h>    //Eigen 라이브러리 선언
#include <EigenAVR.h>   //Eigen 라이브러리 선언
#include <Eigen/Dense>  //Eigen 라이브러리 선언

using Eigen::MatrixXd;  //Eigen 매트릭스 객체 선언

//********************* Matrix 선언 ********************// 
   
extern MatrixXd Axis_Calibration(3, 3);         // 가속도값 축 calibration matrix
extern MatrixXd Accel(3, 1);                    // 가속도 상태변수 vector
extern MatrixXd Accel_bias(3, 1);               // 가속도값 bias calibration matrix
extern MatrixXd Gyro(3, 1);                     // 각속도 상태변수 vector
extern MatrixXd Gyro_bias(3, 1);                // 자이로값 bias calibration matrix

// Note* 외부 .h 에 변수를 선언할때는 extern을 해야한다

//**************** Calibration factor ****************//    
        
#define ax_bias 0 #define gx_bias 0
#define ay_bias 0 #define gy_bias 0
#define az_bias 0 #define gz_bias 0
  
#define Gxx 1 #define Gxy 0 #define Gxz 0
#define Gyx 0 #define Gyy 1 #define Gyz 0
#define Gzx 0 #define Gzy 0 #define Gzz 1

#define sampling_time 0.003 //s

//****************************************************//
String input_data;
const char cSTX = 2;
const char cETX = 3;
String buff;

double ax = 0, ay = 0, az = 0, gx = 0, gy = 0, gz = 0, speedGPSx = 0, speedGPSy = 0, speedDP = 0;

double dt = 0;         // delta_time[ms]
double start_time = 0; // Initial Time Measurement[ms]
double end_time = 0;   // Final time measurement[ms]


void Calibration()
{
  //16bit -> SI unit & Origin setting
   Accel(0,0) = (double) ax / 16384;        Accel(1,0) = (double) az / 16384;        Accel(2,0) = (double) -ay / 16384;
   Gyro(0,0) = (double) gx * 250 / 32768;   Gyro(1,0) = (double) gz * 250 / 32768;   Gyro(2,0) = (double) gy * -250 / 32768;

   Accel = Axis_Calibration * Accel + Accel_bias;
   Gyro = Gyro + Gyro_bias;
}



 void Input_data()
 {   
 int data_len = input_data.length();
 String temp = "";
 int cnt = 1;

 for (int i = 0; i < data_len; i++)
 {
   if (input_data[i] == ','){
     if (cnt == 1) ax = temp.toDouble();
     else if (cnt == 2) ay = temp.toDouble();
     else if (cnt == 3) az = temp.toDouble();
     else if (cnt == 4) gx = temp.toDouble();
     else if (cnt == 5) gy = temp.toDouble();
     else if (cnt == 6) gz = temp.toDouble();
     else if (cnt == 7) speedGPSx = temp.toDouble();
     else if (cnt == 8) speedGPSy = temp.toDouble();
     else speedDP = temp.toDouble();
     temp = "";
     cnt++;
   }
   else temp += input_data[i];
 } 
}

void Output_data(double roll, double pitch, double yaw)
{
  Serial2.print(char(cSTX));
  Serial2.print(roll);
  Serial2.print(",");
  Serial2.print(pitch);
  Serial2.print(",");
  Serial2.print(yaw);
  Serial2.print(",");
  Serial2.print(char(cETX));
  Serial2.println();
}


//##############################################################################################################//
#endif
