void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
}

void loop() {

  if (Serial1.available()) {
     int inByte = Serial1.read();
       Serial.write(inByte);
  }
  

  if (Serial2.available()) {
     int inByte1 = Serial2.read();
     Serial.write(inByte1);
  }
 

}
    


