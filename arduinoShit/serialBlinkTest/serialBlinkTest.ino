byte freq;
String inByte;

void setup() {
  // start serial port at 9600 bps:
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
  pinMode(LED_BUILTIN, OUTPUT);
  // establishContact();  // send a byte to establish contact until receiver responds
  Serial.println("setup completed\n");
  delay(300);
}

void loop() {
  // if we get a valid byte, read analog ins:
  // Serial.println("Enter your name.");  
  while(Serial.available() == 0){
      inByte = Serial.read();
      Serial.println(inByte);
  }
  while (Serial.available() == 0) {}  
  // if (Serial.available() > 0) {
  //   // get incoming byte:
  //   Serial.println("Getting byte...");
  //   delay(300);
  //   inByte = Serial.read();
  // }
  // digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  // delay(500/freq);                       // wait for a second
  // digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  // delay(500/freq);                       // wait for a second
}

void establishContact() {
  while (Serial.available() <= 0) {
    Serial.print("ABCD");   // send a capital A
    delay(300);
  }
}
