#include <main.h>
#include <i2cdevice.h>
#include <wagon.h>
#include <mqtt.h>
#include <timer.h>
#include <animationUpDown.h>
#include <animationBlink.h>
#include <animationKnightRider.h>

bool    bLed = false;

extern TWagon allWagons[ NR_OF_WAGONS ];      //Assigning I2C-Busadresses to Country-Code
extern TTopic topics[ NR_OF_TOPICS ];         //Assigning Topics to Country-Code
TAnimationBasic* animation[ NR_OF_WAGONS ];   //Animations for each wagon

 

void timerEvent_100ms() {                 //called every 100ms (round about)
   for (uint8_t i=0; i<NR_OF_WAGONS; i++) {
    if ( animation[i] != NULL )  {
      animation[i]->trigger();
    }
  }
}

void timerEvent_1s() {                  //called every second
  digitalWrite( LED, bLed );
  bLed = !bLed;
}

/**
 * @brief This method will be called, if an MQTT message to any of the registered topics arrived.
 * 
 * @param adr Decimal representation of the 2 letter country code. This will adress the wagon in the train
 * @param cmd Byte 0 or Port A of the PCF8577 port expander
 * @param val Byte 1 or Port B of the PCF8577 port expander
 */
void mqtt_MessageReceived( uint16_t adr, uint8_t cmd, uint8_t val ) {
  Serial.print( "Adr=" );     Serial.print( adr );
  Serial.print( "  cmd=" );   Serial.print( cmd );
  Serial.print( "  val=" );   Serial.println( val );

  for (uint8_t i=0; i < NR_OF_WAGONS; i++) {
    if ( animation[i] != NULL ) {     //there is an animation...
      if (animation[i]->getWagon()->wagon == adr ) {   //and the adress is correct
        switch (cmd)  {
          case 1:  animation[i]->start( val );
                   break;
          case 3:  animation[i]->stop();
                   break;  
          default: animation[i]->command( cmd, val );
                   break;
        }
        break;
      }
    }
  }
}


void setup() {
  pinMode(LED, OUTPUT);
  digitalWrite( LED, bLed );
  Serial.begin(115200);

  wagon_setup();
  mqtt_setup();
  mqtt_register_Callback( &mqtt_MessageReceived );

  timer_register_Callback( tCB_100MS, &timerEvent_100ms );    //set the callback methods
  timer_register_Callback( tCB_1SEK, &timerEvent_1s );
  timer_Setup();            //start a software based timer to avoid delay()
    
  for( uint8_t i=0; i < NR_OF_WAGONS; i++) {          //init array as empty
    animation[ NR_OF_WAGONS ] = NULL;
  }  

  animation[ cDK ] = new TAnimationKnightRider();     //create an animation object
  animation[ cDK ]->setWagon( &allWagons[ cDK ] );    //assign a wagon to this animatio
  animation[ cDK ]->start(0);


  animation[ cIT ] = new TAnimationBlink();           //create an animation object
  animation[ cIT ]->setWagon( &allWagons[ cIT ] );    //assign a wagon to this animatio
  animation[ cIT ]->start(0);

  animation[ cLT ] = new TAnimationUpDown();           //create an animation object
  animation[ cLT ]->setWagon( &allWagons[ cLT ] );    //assign a wagon to this animatio
  animation[ cLT ]->start(0);

  Serial.println("Setup done");
}

void loop() {  


  timer_Trigger();   //keep the timer running

 
  mqtt_loop();
}



