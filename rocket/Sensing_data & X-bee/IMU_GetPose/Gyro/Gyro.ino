// Roll, Pitch 각도
double prevPhi = 0;
double prevTheta = 0;
double dt = 0.02;  // 자이로센서 측정 간격

// IMU 센서 관련 코드
#include "Wire.h"
#include "I2Cdev.h"
#include "MPU9250.h"

MPU9250 accelgyro;
I2Cdev   I2C_M;

uint8_t buffer_m[6];

int16_t ax, ay, az;
int16_t gx, gy, gz;
int16_t   mx, my, mz;

float heading;
float tiltheading;

float Axyz[3];
float Gxyz[3];
float Mxyz[3];

#define sample_num_mdate  5000

volatile float mx_sample[3];
volatile float my_sample[3];
volatile float mz_sample[3];

static float mx_centre = 0;
static float my_centre = 0;
static float mz_centre = 0;

volatile int mx_max = 0;
volatile int my_max = 0;
volatile int mz_max = 0;

volatile int mx_min = 0;
volatile int my_min = 0;
volatile int mz_min = 0;

float temperature;
float pressure;
float atm;
float altitude;


void setup() {
  Wire.begin();
  Serial.begin(115200);
}


void loop() {
  getGyro_Data();
  EulerGyro(Gxyz[0], Gxyz[1], Gxyz[2]);
  
  Serial.print(prevPhi);
  Serial.print(",");
  Serial.println(prevTheta);
}


void EulerGyro(double gx, double gy, double gz){
  double sinPhi = sin(prevPhi / 180 * PI);
  double cosPhi = cos(prevPhi / 180 * PI);
  double cosTheta = cos(prevTheta / 180 * PI);
  double tanTheta = tan(prevTheta / 180 * PI);

  prevPhi = prevPhi + dt*(gx + gy*sinPhi*tanTheta + gz*cosPhi*tanTheta);
  prevTheta = prevTheta + dt*(gy*cosPhi - gz*sinPhi);
}


void getGyro_Data(void)
{
    accelgyro.getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);

    Gxyz[0] = (double) gx * 250 / 32768;
    Gxyz[1] = (double) gy * 250 / 32768;
    Gxyz[2] = (double) gz * 250 / 32768;
}
