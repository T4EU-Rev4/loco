#include <mqtt.h>
#include <wagon.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <json.h>

// WiFi
const char *ssid          = "IBB"; // Enter your WiFi name
const char *password      = "";  // Enter WiFi password

// MQTT Broker
// const char *mqtt_broker   = "broker.emqx.io";
// const char *mqtt_username = "emqx";
// const char *mqtt_password = "public";
// const int   mqtt_port     = 1883;

const char *mqtt_broker   = "broker.hivemq.com";
const char *mqtt_username = "";
const char *mqtt_password = "";
const int   mqtt_port     = 1883;

// const char *mqtt_broker   = "192.168.1.21";
// const char *mqtt_username = "train4euIT";
// const char *mqtt_password = "italy";
// const int   mqtt_port     = 1883;


char TopicName[NR_OF_TOPICS][15] = { 
    "T4EU/DE",
    "T4EU/DE/DK",
    "T4EU/DE/DE",
    "T4EU/DE/IT",
    "T4EU/DE/HR",
    "T4EU/DE/LT",
    "T4EU/DE/IR"
};

/**
 * @brief Initialize the array of structure.
 *        First parameter is the topic for the dedicated wagon
 *        Second parameter is a decimal representation of the 2-letter country code
 *        used to adress the wagon. You can find the I2CDevice with this code.
 */
TTopic topics[ NR_OF_TOPICS ] = { { TopicName[ 0 ],        0  }, 
                                  { TopicName[ cDK +1 ],   convertCountryCode( countries[cDK] )  }, 
                                  { TopicName[ cDE +1 ],   convertCountryCode( countries[cDE] )  }, 
                                  { TopicName[ cIT +1 ],   convertCountryCode( countries[cIT] )  }, 
                                  { TopicName[ cHR +1 ],   convertCountryCode( countries[cHR] )  }, 
                                  { TopicName[ cLT +1 ],   convertCountryCode( countries[cLT] )  }, 
                                  { TopicName[ cIR +1 ],   convertCountryCode( countries[cIR] )  }
                                };

mqtt_callback_t	CB_ON_RECEIVE = NULL;			//- Variablen für die Callback-Funktionen

WiFiClient   espClient;
PubSubClient client( espClient );
MQTT_State mqttState = MQTT_None;   //start value for the statemachine

//https://forum.arduino.cc/t/int-aus-string-extrahieren/143685    thanks to jurs
//local method
int getIntFromString (char *stringWithInt, int num) {
  char *tail; 
  while (num>0)  {
    num--;
    while ((!isdigit (*stringWithInt))&&(*stringWithInt!=0)) stringWithInt++;
    tail=stringWithInt;

    while ((isdigit(*tail))&&(*tail!=0)) tail++;
    if (num>0) stringWithInt=tail;
  }
  return atoi(stringWithInt);
}


/**
 * @brief This method will be called, if an MQTT message is received
 * 
 * @param adr This is the decimal code, derived from the 2 letter country code
 * @param cmd The first byte (Port a of the port expander or command)
 * @param val The second byte (Port b of the port expander or value)
 */
void mqtt_Received( uint16_t adr, uint8_t cmd, uint8_t val ) {
  //dispatch the received bytes to the listener.
  //Serial.println("rec:");
  if (CB_ON_RECEIVE != NULL) { CB_ON_RECEIVE( adr, cmd, val ); } 
}


// void mqtt_callback(char *topic, byte *payload, unsigned int length) {
//   for (int i=0; i < NR_OF_TOPICS; i++ ) {
//     uint8_t cmd, val;
//     if ( strcmp( topic, topics[i].topic )==0 ){
//       cmd = getIntFromString( (char*)payload, 1 );   
//       val = getIntFromString( (char*)payload, 2 );
//       mqtt_Received( topics[i].wagon, cmd, val );
//       break;
//     }
//   }
//  Serial.println("-----------------------");
// }

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  json_setMAC( WiFi.macAddress() );    //override the preset with the actual value
  Serial.print("Connecting to WIFI network");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

void mqtt_setup() {
  uint8_t res;
  
  initWiFi();
  mqtt_enterState( MQTT_None ) ;
  client.setServer(mqtt_broker, mqtt_port);    //connecting to a mqtt broker
  client.setCallback( json_mqtt_callback );
  while (!client.connected()) {
     

     Serial.printf("The client %s connects to the public mqtt broker\n", MAC.c_str() );
     if (client.connect( MAC.c_str(), mqtt_username, mqtt_password)) {
        Serial.println("Connected to MQTT broker");
        if (mqttState == MQTT_None) {
          mqtt_enterState( MQTT_Connecting );

        }
     } else {
        Serial.print("failed with state ");
        Serial.print(client.state());
        mqttState = MQTT_None;
        delay(2000);
     }
  }
  // publish and subscribe


  // this is the code for my version. It will be removed after the other one is working...
  // client.publish( topics[0].topic, "Hi EMQX I'm ESP32 ^^");
  // Serial.println( topics[0].topic );
  // for( int i=0; i < NR_OF_TOPICS; i++ ) {
  //   client.subscribe( topics[i].topic ); 
  //   Serial.println( topics[i].topic );
  // }
 }

void mqtt_loop() {
  switch (mqttState) {
    case MQTT_None:   handshakeRequest = false;
                      break;
    case MQTT_Connecting:  //sent the messages to the broker,now waiting for the response
                      //hier ggf. eine timeout-überwachung einfügen
                      handshakeRequest = false;
                      break;
     case MQTT_Connected:  //response from server received
                      handshakeRequest = false;
                      mqtt_enterState( MQTT_Run );
                      break;

    case MQTT_Run:    if (handshakeRequest) { //stable 
                        Serial.print("-  Handshake response  " );
                        Serial.println( handshakeCount );
                        json_createServerMsg( mt_handshake );
                        client.publish( TopicSend.c_str(), toPublish  );
                        handshakeRequest = false;
                      }
                      break;
  }
  client.loop();
}

void mqtt_register_Callback(  mqtt_callback_t cbf ) {
  CB_ON_RECEIVE  = cbf;
}

/**
 * @brief Do andy steps neccesary to enter the appropiate state
 * 
 * @param mqs target state
 */
void mqtt_enterState( MQTT_State mqs ) {
  switch (mqs) {
    case MQTT_None:   Serial.println( "MQTT_None:" );
                      break;
    case MQTT_Connecting:  
                      Serial.println( "MQTT_Connecting:" );
                      client.subscribe( TopicRec0.c_str() );   
                      Serial.println( "  subscribing: " + TopicRec0) ;
                      //send a JSON-Msg to the Server
                      json_createServerMsg( mt_boot );    
                      client.publish( TopicSend.c_str(), toPublish  );
                      Serial.println( "  publishing: " + String(toPublish) ) ;
                      break;
    case MQTT_Connected:   //will be entered, if boot message was received @ topic TopicRec0
                      Serial.println( "MQTT_Connected:" );
                      Serial.println( "  unsubscribing: " + TopicRec0 ) ;
                      client.unsubscribe( TopicRec0.c_str()  );
                      Serial.println( "  subscribing: " + TopicRec1 ) ;
                      client.subscribe( TopicRec1.c_str() );
                      //send a JSON-Msg to the Server
                      json_createServerMsg( mt_topic );    
                      Serial.println( "  publishing: " + String(toPublish) ) ;
                      client.publish( TopicSend.c_str(), toPublish  );
                      break;
    case MQTT_Run:    Serial.println( "MQTT_Run:" );
                      break;
  }
  mqttState = mqs;
}
