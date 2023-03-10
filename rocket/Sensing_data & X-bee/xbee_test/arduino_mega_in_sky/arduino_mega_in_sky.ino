void setup() {
  Serial.begin(4800);
  Serial1.begin(4800);
 
}

void loop() {

  if (Serial1.available()) {
     int inByte = Serial1.read();
       
       Serial.write(inByte);
      
  }
  

}
    
