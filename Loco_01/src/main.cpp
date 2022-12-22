#include <main.h>
#include <i2cdevice.h>
#include <wagon.h>
#include <mqtt.h>
#include <timer.h>
#include <animationUpDown.h>
#include <animationBlink.h>
#include <animationKnightRider.h>
#include <animationDirect.h>
#include <animationLoco.h>
#include <log.h>

#define Mot1En1 18
#define Mot1En2 19  
#define Mot1PWM 5

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

void timerEvent_10s() {                 //called every 10 seconds
  mqtt_showWiFiStatus();
  mqtt_checkWiFiStatus();
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
        animation[i]->command( cmd, val );
        break;
      }
    }
  }
}


void assignAnimations() {
  animation[ BE ] = new TAnimationKnightRider();     //create an animation object
  animation[ BE ]->setWagon( &allWagons[ BE ] );     //assign a wagon to this animatio

  // animation[ SE ] = new TAnimationBlink();           //create an animation object
  animation[ SE ] = new TAnimationDirect();         //create an animation object
  animation[ SE ]->setWagon( &allWagons[ SE ] );    //assign a wagon to this animatio
  
  animation[ RO ] = new TAnimationUpDown();           //create an animation object
  animation[ RO ]->setWagon( &allWagons[ RO ] );      //assign a wagon to this animatio

  animation[ DE ] = new TAnimationLoco( Mot1En1, Mot1En2, Mot1PWM);           //create an animation object
  animation[ DE ]->setWagon( &allWagons[ DE ] );      //assign a wagon to this animatio

}


void setup() {
  pinMode(LED, OUTPUT);
  digitalWrite( LED, bLed );
  Serial.begin(115200);

  log_setup();

  wagon_setup();
  mqtt_setup();
  mqtt_register_Callback( &mqtt_MessageReceived );

  timer_register_Callback( tCB_100MS, &timerEvent_100ms );    //set the callback methods
  timer_register_Callback( tCB_1SEK, &timerEvent_1s );
  timer_register_Callback( tCB_10SEK, &timerEvent_10s );
  timer_Setup();            //start a software based timer to avoid delay()
    
  for( uint8_t i=0; i < NR_OF_WAGONS; i++) {          //init array as empty
    animation[ NR_OF_WAGONS ] = NULL;
  }  
  assignAnimations();

  Serial.println("Setup done");
}

void loop() {  

  timer_Trigger();   //keep the timer running
 
  mqtt_loop();
}



