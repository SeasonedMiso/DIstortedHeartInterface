// #include <SoftwareSerial.h>

#include <DS1804.h>
// #include <EEPROM.h>

DS1804 digipot = DS1804( 6, 5, 4, DS1804_HUNDRED);

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  digipot.unlock();
  digipot.setToZero();
  unsigned long actualRes = digipot.setResistance( 10000 );
  Serial.println(actualRes);
  
  // Serial.println("hello");
}

void loop() {
  // put your main code here, to run repeatedly:
  // Serial.println("pok");

}
