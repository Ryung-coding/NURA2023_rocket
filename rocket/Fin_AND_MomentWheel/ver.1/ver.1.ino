#include <Servo.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h> //scl = A5 / sda = A4

Adafruit_PWMServoDriver pwm=Adafruit_PWMServoDriver();



//calibration Value
 
#define set_duty 1750                     // setting zero angular velocity[pca9685-cell]
#define range_duty 500                    // control duty range angular velocity[pca9685-cell] 변동가능성 있음
// 1800~2300us을 기준으로 moment wheel이 작동함


// Servo pin
#define sampling_time 0.003 //s
#define servopin_pitch 9
#define servopin_yaw 10

// Servo object
Servo servo_pitch;
Servo servo_yaw ;

//About Shin
//PID gain_Roll
#define Kp_r 1
#define Kd_r 0
#define bangbang_r 0        // Bang-Bang control range factor

#define Kp_p 1
#define Kd_p 0

#define Kp_y 1
#define Kd_y 0

#define goal_roll 0         // Goal pitch[angle of velocity]
#define goal_pitch 0        // Goal pitch[deg]
#define goal_yaw 0         // Goal yaw[deg]


double past_roll = 0; 
double past_pitch = 0; 
double past_yaw = 0;

double dt = 0;         // delta_time[ms]
double start_time = 0; // Initial Time Measurement[ms]
double end_time = 0;   // Final time measurement[ms]

double sensor_roll = 0;
double sensor_pitch = 0;
double sensor_yaw = 0;

double u_roll = 0;
double u_pitch = 0;
double u_yaw = 0;



const char cSTX = 2;
const char cETX = 3;
String buff;
String input_data;

void Input_data()
{
  int data_len = input_data.length();
  String temp = "";
  int cnt = 1;

  for (int i = 0; i < data_len; i++)
  {
    if (input_data[i] == ',')
    {
      if (cnt == 1) sensor_roll = temp.toDouble();
      else if (cnt == 2) sensor_pitch = temp.toDouble();
      else sensor_yaw = temp.toDouble();
      
      temp = "";
      cnt++;
    }
    else temp += input_data[i];
  }
}


double computePD_BLDC(double goal, double sensor, double sensor_past, double dt, double Kp, double Kd, double bangbang_control_range)
{
  double error = goal - sensor;               // angular velocity increasing -> PID u decreasing
  
  double P = Kp * error;  
  double D = Kd *(sensor - sensor_past) / dt;
  
  double u = P - D;
  
  if ( abs(error) <= bangbang_control_range) u = set_duty;
  
  u = constrain(set_duty + u, set_duty, set_duty + range_duty);
  u = error < 0 ? u : -u;            //define ccw = r < y case = u | cw = r > y case = -u
   
  return u; //[us]  
}

void control_BLDC()
{
  if( u_roll > 0) // ccw case
  {
    pwm.setPWM(15,0,abs(u_roll));
    pwm.setPWM(14,0,0);
  }
  else      // cw case
  {
    pwm.setPWM(15,0,0);
    pwm.setPWM(14,0,abs(u_roll));    
  }
}

double computePD_SERVO(double goal, double sensor, double sensor_past, double dt, double Kp, double Kd)
{
  double error = goal - sensor;               // angular velocity increasing -> PID u decreasing
  
  double P = Kp * error;  
  double D = Kd *(sensor - sensor_past) / dt;
  
  double u = P - D;

  return u;
}

void control_SERVO()
{
  servo_pitch.writeMicroseconds(u_pitch);
  servo_yaw.writeMicroseconds(u_yaw);
}




void setup()
{
  Serial.begin(2400);
  Serial1.begin(2400);
  
  servo_pitch.attach(servopin_pitch);
  servo_yaw.attach(servopin_yaw);

  pwm.begin();
  pwm.setPWMFreq(380); //->measurement 400hz -> 2.5ms / so pca9685 input unit mean 0.620us
  pwm.setPWM(15,0,set_duty);
  pwm.setPWM(14,0,set_duty); 

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
  buff = buff.substring(ipos1 + 1);
  
  Input_data();

  Serial.print(sensor_roll);
  Serial.print(" ");
  Serial.print(sensor_pitch);
  Serial.print(" ");
  Serial.println(sensor_yaw);
  




  u_roll = computePD_BLDC(goal_roll, sensor_roll, past_roll, dt, Kp_r, Kd_r, 0);
  u_pitch = computePD_SERVO(goal_pitch, sensor_pitch, past_pitch, dt, Kp_p, Kd_p);
  u_yaw = computePD_SERVO(goal_yaw, sensor_yaw, past_yaw, dt, Kp_y, Kd_y);

  control_BLDC();
  control_SERVO();

  past_roll = sensor_roll;
  past_pitch = sensor_pitch;
  past_yaw = sensor_yaw;

}
