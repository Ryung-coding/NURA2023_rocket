#define sampling_time 0.003 //s
#define filter_factor 0.99
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
String input_data;

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

 void Compute_Complmentary(double ax,double ay, double az, double gx,double gy, double gz, double dt)
 {
  double roll_a  = atan2(ay, az) * 180 / PI;
  double pitch_a  = atan2(ax, az) * 180 / PI;
  double yaw_a  = atan2(ax, ay) * 180 / PI;

  roll_g += gx * dt;
  pitch_g += gy * dt;
  yaw_g += gz * dt;

  roll_filter = filter_factor * (roll_filter + gx * dt) + (1 - filter_factor) * roll_a;
  pitch_filter = filter_factor  * (pitch_filter + gy * dt) + (1 - filter_factor) * pitch_a;
  yaw_filter = filter_factor  * (yaw_filter + gz * dt) + (1 - filter_factor) * yaw_a;
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

  // ================= Sensor Mega로부터 데이터 받는 부분 ======================
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

  // gBuff 업데이트
  buff = buff.substring(ipos1 + 1);

  Input_data();
  // ================= Sensor Mega로부터 데이터 받는 부분 ======================
  
  Compute_Complmentary(ax, ay, az, gx, gy, gz, dt);
  
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
      Serial.print(gz);
      Serial.print("|");
      Serial.print(speedGPSx);
      Serial.print("|");
      Serial.print(speedGPSy);
      Serial.print("|");
      Serial.println(speedDP);
      
  
  Output_data(roll_filter, pitch_filter, yaw_filter);
}
