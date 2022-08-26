byte freq;
String inString;

void setup() {
  // start serial port at 9600 bps:
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
  pinMode(LED_BUILTIN, OUTPUT);
  // Serial.println("test\n");
  // Serial.write("test\n");
  freq = 15;
}

void loop() {
  while(Serial.available() == 0){
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(500/freq);                       // wait for a second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(500/freq);                       // wait for a second
  }
    while(Serial.available() > 0){
      inString = Serial.readString();
      Serial.println(inString);
      Serial.write("sent message\n");
      if(inString.toInt()>0&&inString.toInt()<20){
        freq = inString.toInt();
      }
    }
    
} 


void establishContact() {
  while (Serial.available() <= 0) {
    Serial.print("ABCD");   // send a capital A
    delay(300);
  }
}
