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
#include <animationSelf.h>
#include <animationStartStop.h>
#include <log.h>
#include <ezButton.h>
#include <stepper.h>

#define SM_DIR      17
#define SM_PUL      16
#define SM_EN       14
#define SM_SYNC     4     //Stepper motor sync switch

#define Mot1En1     18
#define Mot1En2     19  
#define Mot1PWM     5
#define SCANDELAY   10
#define SCREENDELAY 5
uint8_t manCamPos = 0;
bool    bLed = false;
uint8_t scan = SCANDELAY;
uint8_t screen = SCREENDELAY;
I2CDevice scanner = I2CDevice(0);

extern TWagon allWagons[ NR_OF_WAGONS ];      //Assigning I2C-Busadresses to Country-Code
extern TTopic topics[ NR_OF_TOPICS ];         //Assigning Topics to Country-Code
TAnimationBasic* animation[ NR_OF_WAGONS ];   //Animations for each wagon

extern union TTimer timerFlags;               //keep all the flags in one byte

 ezButton button1(BUTTON1);  // create ezButton object that attach to pin BUTTON1;
 ezButton button2(BUTTON2);  // create ezButton object that attach to pin BUTTON2;
 //ezButton button3(BUTTON3);  // create ezButton object that attach to pin BUTTON2;

Stepper camera( SM_PUL, SM_DIR, SM_EN, SM_SYNC );

void timerEvent_100ms() {                 //called every 100ms (round about)
   for (uint8_t i=0; i<NR_OF_WAGONS; i++) {
    if ( animation[i] != NULL )  {
      animation[i]->trigger();
    }
  }
}

void timerEvent_1s() {                  //called every second
  digitalWrite( LED1, bLed );
  bLed = !bLed;
  if (screen > 0) { screen--; }
  //  mqtt_MessageReceived( convertCountryCode( countries[DE] ), 1, 30 );
}

void timerEvent_10s() {                 //called every 10 seconds
  mqtt_showWiFiStatus();
  mqtt_checkWiFiStatus();
  if (scan > 0) { scan--; }
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

        //i is now an indicator for the camera position
        if (i < SHOWCASE_LIMIT) {
          camera.moveTo( i );  
        } else {
          Serial.print( i );
          Serial.println(" not reachable by camera.");
        }
        break;
      }
    }
  }
}

#ifdef TRAIN_GERMANY
void assignAnimations() {
  animation[ DE ] = new TAnimationSelf();         //create an animation object
  animation[ DE ]->setWagon( &allWagons[ DE ] );    //assign a wagon to this animation

  animation[ DK ] = new TAnimationSelf();         
  animation[ DK ]->setWagon( &allWagons[ DK ] );    

  animation[ IS ] = new TAnimationSelf();         
  animation[ IS ]->setWagon( &allWagons[ IS ] );    

  animation[ SK ] = new TAnimationSelf();         
  animation[ SK ]->setWagon( &allWagons[ SK ] );   

  animation[ RO ] = new TAnimationSelf();         
  animation[ RO ]->setWagon( &allWagons[ RO ] );   

  animation[ FR ] = new TAnimationSelf();         
  animation[ FR ]->setWagon( &allWagons[ FR ] );   

  animation[ AT ] = new TAnimationSelf();         
  animation[ AT ]->setWagon( &allWagons[ AT ] );    

  animation[ HR ] = new TAnimationSelf();         
  animation[ HR ]->setWagon( &allWagons[ HR ] );   

  animation[ BE ] = new TAnimationSelf();         
  animation[ BE ]->setWagon( &allWagons[ BE ] );    

  animation[ CH ] = new TAnimationSelf();         //create an animation object
  animation[ CH ]->setWagon( &allWagons[ CH ] );    //assign a wagon to this animation



//  animation[ BE ] = new TAnimationKnightRider();     //create an animation object
//  animation[ BE ]->setWagon( &allWagons[ BE ] );     //assign a wagon to this animation

//  animation[ SE ] = new TAnimationStartStop();         //create an animation object
//  animation[ SE ]->setWagon( &allWagons[ SE ] );    //assign a wagon to this animation
  
//  animation[ RO ] = new TAnimationUpDown();           //create an animation object
//  animation[ RO ]->setWagon( &allWagons[ RO ] );      //assign a wagon to this animation

//  animation[ L1 ] = new TAnimationLoco( Mot1En1, Mot1En2, Mot1PWM);           //create an animation object
//  animation[ L1 ]->setWagon( &allWagons[ L1 ] );      //assign a wagon to this animatio

}
#endif

#ifdef TRAIN_NORWAY
void assignAnimations() {
  animation[ IT ] = new TAnimationStartStop();     //create an animation object
  animation[ IT ]->setWagon( &allWagons[ IT ] );     //assign a wagon to this animation

  animation[ FR ] = new TAnimationStartStop();         
  animation[ FR ]->setWagon( &allWagons[ FR ] );    
  
  animation[ NO ] = new TAnimationStartStop();           
  animation[ NO ]->setWagon( &allWagons[ NO ] );      

  animation[ PT ] = new TAnimationStartStop( );          
  animation[ PT ]->setWagon( &allWagons[ PT ] );      

  animation[ NL ] = new TAnimationStartStop();        
  animation[ NL ]->setWagon( &allWagons[ NL ] );    

  animation[ LX ] = new TAnimationStartStop();        
  animation[ LX ]->setWagon( &allWagons[ LX ] );   
}
#endif

void setup() {
  pinMode(SM_DIR, OUTPUT);
  pinMode(SM_PUL, OUTPUT);
  pinMode(SM_EN, OUTPUT);


  pinMode(LED1, OUTPUT);
  digitalWrite( LED1, bLed );
  pinMode(LED2, OUTPUT);
  digitalWrite( LED2, true );
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

  scanner.begin( SDA, SCL );    //only needed for scanning for I2C devices

  camera.begin();
  camera.home();
  camera.moveTo( 0 );
  Serial.println( camera.error() );
  Serial.println("Setup done");
}

void loop() {  

  timer_Trigger();   //keep the timer running

  if(timerFlags.flags.bMilli) {     //keine call-back für diese schnelle 
    camera.clock();
    timerFlags.flags.bMilli = 0;
  }

  button1.loop();
  button2.loop();
  // button3.loop();
  
  mqtt_loop();

  if (button1.isPressed()) {
    Serial.println("button 1");
    //mqtt_MessageReceived( convertCountryCode( countries[DE] ), 1, 30 );
//    digitalWrite( SM_DIR, HIGH );
//    for (int i=0; i<200; i++) {
//      digitalWrite( SM_PUL, HIGH );
//      delay(1);
//      digitalWrite( SM_PUL, LOW);
//      delay(1);  
//    }
    manCamPos++;
    if (manCamPos > 9) { manCamPos = 0; }
    camera.moveTo( manCamPos );
  }

  if (button2.isPressed()) {
    Serial.println("button 2");
    //mqtt_MessageReceived( convertCountryCode( countries[BE] ), 1, 30 );    
//    digitalWrite( SM_DIR, LOW );
//    for (int i=0; i<200; i++) {
//      digitalWrite( SM_PUL, HIGH );
//      delay(1);
//      digitalWrite( SM_PUL, LOW);
//      delay(1);  
//    }    

//    camera.moveTo( 9 );
    scan = 0;

  }

  // if (button3.isPressed()) {
  //   Serial.println("button 3");
  //   mqtt_MessageReceived( convertCountryCode( countries[DK] ), 1, 0 );    
  // }

  if (scan == 0) {
    scanner.scan();
    scan = SCANDELAY;
  }

  if (screen == 0 ) {
    
    screen = SCREENDELAY;
  }




}



