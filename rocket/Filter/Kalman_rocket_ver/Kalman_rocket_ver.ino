#include <Eigen30.h>
#include <EigenAVR.h>
#include <Eigen/Dense>

using Eigen::MatrixXd;

//**************** Define Matrix ****************//    
MatrixXd Axis_Calibration(3, 3);
MatrixXd Accel(3, 1);
MatrixXd Accel_bias(3, 1);
MatrixXd Gyro(3, 1);
MatrixXd Gyro_bias(3, 1);

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




#define sampling_time 0.003 //s

double ax = 0, ay = 0, az = 0, gx = 0, gy = 0, gz = 0, speedGPSx = 0, speedGPSy = 0, speedDP = 0;

double dt = 0;         // delta_time[ms]
double start_time = 0; // Initial Time Measurement[ms]
double end_time = 0;   // Final time measurement[ms]

double roll_filter = 0;
double pitch_filter = 0;
double yaw_filter = 0;




double  roll_g = 0;
double  pitch_g = 0;
double  yaw_g = 0;

const char cSTX = 2;
const char cETX = 3;
String buff;

 void Input_data()
 {   
  if (Serial1.available() <= 0) return;   // Serial1으로 들어온 값이 없으면 종료
  char c = Serial1.read();
  buff += c;

  // STX ~ ETX 찾기
  int ipos0 = buff.indexOf(cSTX);
  if (ipos0 < 0) return;
  int ipos1 = buff.indexOf(cETX, ipos0);
  if (ipos1 < 0) return;

  // STX ~ ETX 빼고 내부만 얻기
  String input_data = buff.substring(ipos0+1, ipos1);

  // gBuff 업데이트
  buff = buff.substring(ipos1 + 1);

  int data_len = input_data.length();
  String temp = "";
  int cnt = 1;

  for (int i = 0; i < data_len; i++)
  {
    if (input_data[i] == ',')
    {
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


void Calibration()
{
  //16bit -> SI unit & Origin setting
   Accel(0,0) = (double) ax / 16384;        Accel(1,0) = (double) az / 16384;        Accel(2,0) = (double) -ay / 16384;
   Gyro(0,0) = (double) gx * 250 / 32768;   Gyro(1,0) = (double) gz * 250 / 32768;   Gyro(2,0) = (double) gy * -250 / 32768;

   Accel = Axis_Calibration * Accel + Accel_bias;
   Gyro = Gyro + Gyro_bias;
}

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

  Thrust = now_time - Thrust_start_time > 0 and now_time - Thrust_start_time < Thrust_end_time ? Thrust_slope * (now_time - Thrust_start_time) + Thrust_Intercept : 0;
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
