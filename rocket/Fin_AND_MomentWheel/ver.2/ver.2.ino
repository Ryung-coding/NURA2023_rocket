#include "CONTROLLER.h"

void setup()
{
  Serial.begin(9600);
  servo_roll.attach(SERVO_pin_roll);
  servo_pitch.attach(SERVO_pin_pitch);


  pwm.begin();
  pwm.setPWMFreq(380); //->measurement 400hz -> 2.5ms / so pca9685 input unit mean 0.620us
  pwm.setPWM(BLDC_pin_cw, 0, set_duty);
  pwm.setPWM(BLDC_pin_ccw, 0, set_duty); 
  delay(1000);
}

void loop()
{   
  Setting_sampling_time();
  
  Input_data();

  Serial.println(String(sensor_roll - off_set_roll)+","+String(sensor_pitch - off_set_pitch)+","+String(sensor_yaw));
  
  u_roll  = computePD_SERVO(goal_roll, sensor_roll, past_roll, dt, Kp_r, Kd_r, bangbang_r);
  u_pitch = computePD_SERVO(goal_pitch, sensor_pitch, past_pitch, dt, Kp_p, Kd_p, bangbang_p);
  u_yaw   = computePD_BLDC(goal_yaw, sensor_yaw, past_yaw, dt, Kp_y, Kd_y, bangbang_y);

  control_BLDC();
  control_SERVO();

  past_roll = sensor_roll;
  past_pitch = sensor_pitch;
  past_yaw = sensor_yaw;

}
