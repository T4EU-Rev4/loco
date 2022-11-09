#include <WiFi.h>
#include <PubSubClient.h>

#define NROFTOPICS 5

typedef struct {
  char    *topic;    //the topic, we want to register
  uint8_t adr;       //the assigned adress of the bus device
} TTopic;

TTopic allTopics[NROFTOPICS];

 
void initTopics() {
  allTopics[0].topic = "T4EU/DE";      allTopics[0].adr = 0;
  allTopics[1].topic = "T4EU/DE/DE";   allTopics[1].adr = 49;
  allTopics[2].topic = "T4EU/DE/DK";   allTopics[2].adr = 94;
  allTopics[3].topic = "T4EU/DE/HR";   allTopics[3].adr = 17;
  allTopics[4].topic = "T4EU/DE/UA";   allTopics[4].adr = 21;
}

// WiFi
const char *ssid = ""; // Enter your WiFi name
const char *password = "";  // Enter WiFi password

// MQTT Broker
const char *mqtt_broker = "broker.emqx.io";

const char *mqtt_username = "emqx";
const char *mqtt_password = "public";
const int mqtt_port = 1883;


WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);         // Set software serial baud to 115200;
  initTopics();
  WiFi.begin(ssid, password);   // connecting to a WiFi network
  while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
 
  client.setServer(mqtt_broker, mqtt_port);    //connecting to a mqtt broker
  client.setCallback(callback);
  while (!client.connected()) {
     String client_id = "esp32-client-";
     client_id += String(WiFi.macAddress());
     Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
     if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
         Serial.println("Public emqx mqtt broker connected");
     } else {
         Serial.print("failed with state ");
         Serial.print(client.state());
         delay(2000);
     }
 }
 // publish and subscribe
 client.publish( allTopics[0].topic, "Hi EMQX I'm ESP32 ^^");
 for( int i=0; i<NROFTOPICS; i++ ) {
  client.subscribe( allTopics[i].topic ); 
 }

 }

void myMQTTReceived( uint8_t adr, int cmd, int val ) {
  Serial.print( "Adr=" );   Serial.print( adr );
  Serial.print( "  cmd=" );   Serial.print( cmd );
  Serial.print( "  val=" );   Serial.println( val );
}

//https://forum.arduino.cc/t/int-aus-string-extrahieren/143685
//thanks to jurs
int getIntFromString (char *stringWithInt, int num)
{
  char *tail; 
  while (num>0)
  {
    num--;
    while ((!isdigit (*stringWithInt))&&(*stringWithInt!=0)) stringWithInt++;
    tail=stringWithInt;

    while ((isdigit(*tail))&&(*tail!=0)) tail++;
    if (num>0) stringWithInt=tail;
  }

  return atoi(stringWithInt);
}


void callback(char *topic, byte *payload, unsigned int length) {
// Serial.print("Message arrived in topic: ");
// Serial.println(topic);
// Serial.print("Message:");
// for (int i = 0; i < length; i++) {
//     Serial.print((char) payload[i]);
// }
// Serial.println(length);

  for (int i=0; i<NROFTOPICS; i++ ) {
    int cmd, val;
    if ( strcmp( topic, allTopics[i].topic )==0 ){
//      Serial.print("Adr=");
//      Serial.println( allTopics[i].adr );
      cmd = getIntFromString( (char*)payload, 1 );   
      val = getIntFromString( (char*)payload, 2 );
      myMQTTReceived( allTopics[i].adr, cmd, val );
      break;
    }
  }
 Serial.println("-----------------------");
}

void loop() {
 client.loop();
}

