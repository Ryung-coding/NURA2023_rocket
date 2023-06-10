#include <Servo.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h> //scl = A5 / sda = A4

Adafruit_PWMServoDriver pwm=Adafruit_PWMServoDriver();



//calibration Value
#define sampling_time 0.01 
#define set_duty 1750                     // setting zero angular velocity[pca9685-cell]
#define range_duty 500                    // control duty range angular velocity[pca9685-cell] 변동가능성 있음
// 1800~2300us을 기준으로 moment wheel이 작동함


// Servo pin
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


unsigned long dt = 0;         // delta_time[ms]
unsigned long start_time = 0; // Initial Time Measurement[ms]
unsigned long end_time = 0;   // Final time measurement[ms]

double sensor_roll = 0;
double sensor_pitch = 0;
double sensor_yaw = 0;

double u_roll = 0;
double u_pitch = 0;
double u_yaw = 0;

 void Input_data()
 {
  //read list = Serial.read; -> a12345b12345c123456
  // 촵촵 짤라
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
  Serial.begin(115200);
  
  servo_pitch.attach(servopin_pitch);
  servo_yaw.attach(servopin_yaw);

  pwm.begin();
  pwm.setPWMFreq(380); //->measurement 400hz -> 2.5ms / so pca9685 input unit mean 0.620us
  pwm.setPWM(15,0,set_duty);
  pwm.setPWM(14,0,set_duty); 

  Input_data();
}

void loop()
{
  start_time = millis();
  dt = (start_time - end_time) * 0.001;
  end_time = millis();
  
  while(dt < sampling_time)
  {
    delay(1);
  }
  
  Input_data();

  u_roll = computePD_BLDC(goal_roll, sensor_roll, past_roll, sampling_time, Kp_r, Kd_r, 0);
  u_pitch = computePD_SERVO(goal_pitch, sensor_pitch, past_pitch, sampling_time, Kp_p, Kd_p);
  u_yaw = computePD_SERVO(goal_yaw, sensor_yaw, past_yaw, sampling_time, Kp_y, Kd_y);

  control_BLDC();
  control_SERVO();
 

}
