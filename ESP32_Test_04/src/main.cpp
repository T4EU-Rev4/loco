#include <main.h>
#include <i2cdevice.h>

uint8_t a = 0;                  //count upwards
uint8_t b = 0xFF;               //count downwards

bool    bLed = false;

I2CDevice i2cdev( DEV ); 

void setup() {
  pinMode(LED, OUTPUT);
  digitalWrite( LED, bLed );
  Serial.begin(115200);
  Serial.println("1");
  i2cdev.begin( );  
  Serial.println("2");
}

void loop() {  
  uint8_t x;

  i2cdev.set( PCF_PORT_A, a );
  i2cdev.set( PCF_PORT_B, b );
  i2cdev.write();


  a++;  b--;

  digitalWrite( LED, bLed );
  bLed = !bLed;

  delay( 500 );

}