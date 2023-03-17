// Roll, Pitch 각도
double prevPhi = 0;
double prevTheta = 0;

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
  getAccel_Data();
  EulerAccel(Axyz[0], Axyz[1], Axyz[2]);
  
  Serial.print(prevPhi);
  Serial.print(",");
  Serial.println(prevTheta);
}


void EulerAccel(double ax, double ay, double az){
  prevPhi = atan(ay/az);
  prevTheta = atan(ax/sqrt(ay*ay + az*az));

  prevPhi = prevPhi * 180 / PI;
  prevTheta = prevTheta * 180 / PI;
}


void getAccel_Data(void)
{
    accelgyro.getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);
    Axyz[0] = (double) ax / 16384;
    Axyz[1] = (double) ay / 16384;
    Axyz[2] = (double) az / 16384;
}
