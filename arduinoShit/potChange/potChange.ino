
#include <DS1804.h>
	
const byte wiperPin = A5;
// DS1804 digipot = DS1804( 6, 5, 4, DS1804_FIFTY );
DS1804 digipot = DS1804( 6, 5, 4, DS1804_TEN );
	
int reading = 0;
unsigned long new_resistance = 0;
String inString;
	
void setup() {
  Serial.begin(9600);
  digipot.setToZero();

  Serial.println( "~~ testing all positions:");
for (int i = 0; i < 100; i++) {
  digipot.setWiperPosition((byte)i);
  delay(10);
  reading = analogRead( wiperPin );
  String temp = "";
  Serial.println(temp + digipot.getWiperPosition() +","+temp + digipot.getResistance()+  "ohms"+"," + temp + 4.9*reading + "mV");

}
Serial.println( "~~");
}
	
void loop() {
  // if ( Serial.available() > 0 ) {
  //   new_resistance = Serial.parseFloat();
  //   digipot.setResistance( new_resistance );
  //   delay(100);	// give DS1804 time to change, and for analogue reading to settle
  //   reading = analogRead( wiperPin );
  //   String wiperPositionText = "DS1804 Wiper at ";
  //   Serial.println( wiperPositionText + digipot.getWiperPosition() );
  //   String resistanceText = "DS1804 Resistance set to ";
  //   Serial.println( resistanceText + digipot.getResistance() + " ohms (requested " + new_resistance + " ohms)" );
  //   String wiperPinVoltageText = "Wiper pin voltage: ";
  //   Serial.println( wiperPinVoltageText + 4.9*reading + " mV" );
  //     delay(100);	// give DS1804 time to change, and for analogue reading to settle
  // inString = Serial.readString();
  // Serial.println(inString);
  // }
}