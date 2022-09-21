
#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
byte freq;
String inString;
unsigned long last = millis();

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW); 
  
  lcd.begin(16, 2);
  lcd.print(".");
  lcd.setCursor(0,1);
  lcd.print("");
  delay(500);
  lcd.begin(16, 2);
  lcd.print("..");
  lcd.setCursor(0,1);
  lcd.print("");
  delay(500);
  lcd.begin(16, 2);
  lcd.print(".");
  lcd.setCursor(0,1);
  lcd.print("");
  delay(500);
  lcd.begin(16, 2);
  lcd.print("...");
  lcd.setCursor(0,1);
  lcd.print("");
  delay(500);
  // start serial port at 9600 bps:
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
  
  // Serial.println("test\n");
  // Serial.write("test\n");


  
  freq = 15;
  lcd.begin(16, 2);
  lcd.print("initArd");
  lcd.setCursor(0,1);
  lcd.print("");
  
}

void loop() {
  // while(Serial.available() == 0){
  //   // digitalWrite(LED_BUILTIN, LOW); 
  //   // if(last<millis()-1000){
  //   //   last = millis();
  //   //   digitalWrite(LED_BUILTIN, HIGH);
  //   // }
  //   // if(last<millis()-500){
  //   //   digitalWrite(LED_BUILTIN, LOW);
  //   // }
  //   // digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    
  //   // delay(500/freq);                       // wait for a second
  //   // digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  //   // delay(500/freq);                       // wait for a second
    
  // }
    while(Serial.available() > 0){
      digitalWrite(LED_BUILTIN, HIGH); 
      inString = Serial.readString();
      // Serial.println(inString);
      // Serial.write("sent message\n");
      lcd.begin(16, 2);
      lcd.print("received");
      lcd.setCursor(0,1);
      lcd.print("something");
      // if(inString.toInt()>0&&inString.toInt()<20){
      //   freq = inString.toInt();
      // }
    }
    
} 


void establishContact() {
  while (Serial.available() <= 0) {
    Serial.print("ABCD");   // send a capital A
    delay(300);
  }
}
