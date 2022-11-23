#include <main.h>
/*
  This little programm will test a PCF8575 portexpander with 16 Leds connected
  One after the other will be switched on and off.

  Disadvantage of this library:  You can only set pin by pin ans every pin change causes
  a transfer of 16 Bits over the I2C bus.

  Setting 16 pins will result in transering 16 x 16 bit

*/
// Set i2c address
PCF8575 pcf8575( DEV );
uint8_t n = 0;

void setup() {
	Serial.begin(115200);
  
	// Set pinMode to OUTPUT:  all Pins to output
	for (uint8_t i=P0; i<=P15; i++) {
	  pcf8575.pinMode(i, OUTPUT);
	}
	pcf8575.begin();
}

void loop() {
  if (n==0) { 
    pcf8575.digitalWrite(P15, HIGH);    //vorhergehener Pin aus
  } else {
    pcf8575.digitalWrite(n-1, HIGH);
  }
  delay(250);
  pcf8575.digitalWrite( n, LOW);        //neuer Pin ein
  n = (n + 1) % 16;  //nächstes bit

  delay(500);
  
}