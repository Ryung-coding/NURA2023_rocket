/*
 * mpu9250 I2C addr = 0x68
 * pca9685 I2c addr = 0x40
 * 
 * download 
 * Adafruit_PWMServoDriver.h ! -> my git-Hub NURA2023/rocket/momentwheel
 * 
 * using filter & control system
 * LPF(Low Pass Filer)
 * Bang-Bang control(ON-OFF)
 * PD
 * 
 * 
 */

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h> //scl = A5 / sda = A4

Adafruit_PWMServoDriver pwm=Adafruit_PWMServoDriver();

int16_t ax, ay, az, gx, gy, gz; // we use only 'gz'(yaw angular velocity)

//calibration Value
#define bangbang_control_range 7        // Bang-Bang control range factor
#define set_duty 20                     // setting zero angular velocity[%]
#define range_duty 15                   // control duty range angular velocity[%]

//PD gain
#define Kp 1.5
#define Kd 0.87

//Control & PID & LPF(Low Pass Filter) Value
double P, D;
double r = 0;                                  // Desired angular velocity
double y = 0;                                  // measurement angular velocity
double u = 0;                                  // Control Value-PWM duty [%]
double millisTime_i;                           // Initial Time Measurement[ms]
double millisTime_f;                           // Final time measurement[ms]
double dt = 0;                                 // Tiny time[ms]
double y_past = 0;                             // Past input data

double motor_speed, motor_speed_past, motor_speed_filter;    // setting BLDC value

bool state = true;                                           // on-off swithing state value

void Input_data()
{
  Wire.beginTransmission(0x68);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(0x68, 14, true);
  ax = Wire.read() << 8 | Wire.read();
  ay = Wire.read() << 8 | Wire.read();
  az = Wire.read() << 8 | Wire.read();
  Wire.read()<< 8 | Wire.read(); //temp data
  gx = Wire.read() << 8 | Wire.read();
  gy = Wire.read() << 8 | Wire.read();
  gz = Wire.read() << 8 | Wire.read();
  
  gz = (double) gz * 250 / 32768; //change 16bit -> degree/s
}

float lowpassfilter(float filter, float data, float lowpass_constant)
{
  filter = filter * (1 - lowpass_constant) + data * lowpass_constant;
  return filter;   
}

float computePD(float r, float y, float dt)
{
  double error = r - y;               // angular velocity increasing -> PID u decreasing
  
  P = Kp * error;  
  D = Kd *(y - y_past) / dt;
  
  u = P - D;
  
  if ( abs(error) <= bangbang_control_range) u = 0;
  
  u = constrain(map(set_duty, 0, 100, 1613, 3226) + u, map(set_duty - range_duty, 0, 100, 1613, 3226), map(set_duty + range_duty, 0, 100, 1613, 3226));
  u = error < 0 ? u : -u;            //define ccw = r < y case = u | cw = r > y case = -u
   
  return u; //[us]  
}

void control_motor()
{
  if( u > 0) // ccw case
  {
    pwm.setPWM(0,0,abs(u));
    pwm.setPWM(1,0,0);
  }
  else      // cw case
  {
    pwm.setPWM(0,0,0);
    pwm.setPWM(1,0,abs(u));    
  }
}

void BLDC_setting(int rotate, float duty)
{
  if(duty > 0)
  {
    pwm.setPWM(rotate,0,map(0, 0, 100, 1613, 3226)); //set BLDC
    delay(3000);
  }

  do
  {
    motor_speed = constrain(map(duty, 0, 100, 1613, 3226), 1613, 3226);
    motor_speed_filter = lowpassfilter(motor_speed_past, motor_speed, 0.9);
    //Serial.println(motor_speed_filter);
    pwm.setPWM(rotate,0,motor_speed_filter);
    motor_speed_past = motor_speed_filter;
    delay(100);
    }while(abs(motor_speed - motor_speed_filter) > 3);
}

void setup() 
{
  Serial.begin(9600);
  pinMode(2, INPUT);   // on-off swithing line
  Wire.begin();
  pwm.begin();
  pwm.setPWMFreq(380); //->measurement 400hz -> 2.5ms / so pca9685 input unit mean 0.620us
  
  BLDC_setting(0, set_duty);  
  BLDC_setting(1, set_duty);  
}


void loop() 
{ 
  
  if(digitalRead(2) == HIGH)
  {
    BLDC_setting(0, 0);
    BLDC_setting(1, 0);
    
    while(1)
    {
      if(digitalRead(2) == LOW)
       {
        delay(100);
        break; 
       }
    }
  }
    
    millisTime_f = millis();                             //measurement time
    dt = (millisTime_f - millisTime_i)*0.001;            //calculate time
    millisTime_i = millis();                             //measurement time
  
    Input_data();                                        //Load raw data
  
    y = lowpassfilter(y_past , gz, 0.9);                 //filtering raw data
    r = 0;
    u = computePD(r, y, dt);                             //calculate PD
    
    Serial.print(gz);
    Serial.print(",");
    Serial.println(y);
    
    control_motor();                                     //control BLDC using 'u' factor
  
    y_past = y;                                          //up-date    
}
