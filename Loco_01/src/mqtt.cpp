#include <mqtt.h>
#include <wagon.h>
#include <WiFi.h>
#include <PubSubClient.h>

// WiFi
const char *ssid          = "IBB"; // Enter your WiFi name
const char *password      = "pc6sWPuV2YyFO5kSJKpW";  // Enter WiFi password

// MQTT Broker
// const char *mqtt_broker   = "broker.emqx.io";
// const char *mqtt_username = "emqx";
// const char *mqtt_password = "public";
// const int   mqtt_port     = 1883;

// const char *mqtt_broker   = "192.168.1.21";
// const char *mqtt_username = "train4euIT";
// const char *mqtt_password = "italy";
// const int   mqtt_port     = 1883;

const char *mqtt_broker   = "michael.tuxcity.de";
const char *mqtt_username = "Paul";
const char *mqtt_password = "test1234";
const int   mqtt_port     = 1883;

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
PubSubClient client(espClient);

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


void mqtt_callback(char *topic, byte *payload, unsigned int length) {
  for (int i=0; i < NR_OF_TOPICS; i++ ) {
    uint8_t cmd, val;
    if ( strcmp( topic, topics[i].topic )==0 ){
      cmd = getIntFromString( (char*)payload, 1 );   
      val = getIntFromString( (char*)payload, 2 );
      mqtt_Received( topics[i].wagon, cmd, val );
      break;
    }
  }
 Serial.println("-----------------------");
}

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
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
 
  client.setServer(mqtt_broker, mqtt_port);    //connecting to a mqtt broker
  client.setCallback( mqtt_callback );
  while (!client.connected()) {
     String client_id = "esp32-client-";
     client_id += String(WiFi.macAddress());
     Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
     if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
         Serial.println("Connected to MQTT broker");
     } else {
         Serial.print("failed with state ");
         Serial.print(client.state());
         delay(2000);
     }
  }
  // publish and subscribe
  client.publish( topics[0].topic, "Hi EMQX I'm ESP32 ^^");
  Serial.println( topics[0].topic );
  for( int i=0; i < NR_OF_TOPICS; i++ ) {
    client.subscribe( topics[i].topic ); 
    Serial.println( topics[i].topic );
  }
 }

void mqtt_loop() {
 client.loop();
}

void mqtt_register_Callback(  mqtt_callback_t cbf ) {
  CB_ON_RECEIVE  = cbf;
}