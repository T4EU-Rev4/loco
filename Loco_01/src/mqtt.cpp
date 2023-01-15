#include <mqtt.h>
#include <wagon.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <json.h>
#include <log.h>


// WiFi
const char *ssid          = "TPMC"; // Enter your WiFi name
const char *password      = "kaun_Technikos_PMC_device";  // Enter WiFi password

// MQTT Broker
// const char *mqtt_broker   = "broker.emqx.io";
// const char *mqtt_username = "emqx";
// const char *mqtt_password = "public";
// const int   mqtt_port     = 1883;

const char *mqtt_broker   = "broker.hivemq.com";
const char *mqtt_username = "";
const char *mqtt_password = "";
const int   mqtt_port     = 1883;

// const char *mqtt_broker   = "192.168.1.21";    //my local Mosquitto
// const char *mqtt_username = "train4euIT";
// const char *mqtt_password = "italy";
// const int   mqtt_port     = 1883;



#define WiFiRestartTimeout  20
uint8_t timeout = WiFiRestartTimeout;         //try to reboot after an unexpected disconnect


mqtt_callback_t	CB_ON_RECEIVE = NULL;			//- Variablen für die Callback-Funktionen

WiFiClient   espClient;
PubSubClient client( espClient );
MQTT_State mqttState = MQTT_None;   //start value for the statemachine

int mqtt_LastClientstate = -100;    //a unused value for init

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

uint8_t mqtt_getWiFiStatus(){
  return  (uint8_t)WiFi.status();
}

void initWiFi() {
  uint8_t timeout = WiFiRestartTimeout;  //Timeout in seconds
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  json_setMAC( WiFi.macAddress() );    //override the preset with the actual value
  Serial.print("Connecting to WIFI network");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');        //If there is not successfull connection to WiFi network
    mqtt_showWiFiStatus();    //after typ. 20 sec, the ESP will be restarted
    delay(1000);              
    timeout--;
    if (timeout == 0) {
      ESP.restart();
    }
  }
  Serial.println(WiFi.localIP());
  log_print( WiFi.localIP().toString()  );
  mqtt_showWiFiStatus();
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
 }

void mqtt_loop() {
  int clientstate;
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
                        //Serial.print("-  Handshake response  " );
                        //Serial.println( handshakeCount );
                        String s = "Handshake: " + String(handshakeCount);
                        log_line( 2, s.c_str() );
                        json_createServerMsg( mt_handshake );
                        client.publish( TopicSend.c_str(), toPublish  );
                        handshakeRequest = false;
                      }
                      break;
  }
  clientstate = client.state() ;
  if (clientstate != mqtt_LastClientstate) {
    //Do not flood logfile
    mqtt_LastClientstate = clientstate;
    switch ( clientstate ) {
      case -4 : Serial.println("MQTT_CONNECTION_TIMEOUT"); break;
      case -3 : Serial.println("MQTT_CONNECTION_LOST"); break; 
      case -2 : Serial.println("MQTT_CONNECT_FAILED");  break;
      case -1 : Serial.println("MQTT_DISCONNECTED");  break;
      case  0 : break;  //Serial.println("MQTT_CONNECTED");  break;
      case  1 : Serial.println("MQTT_CONNECT_BAD_PROTOCOL");  break;
      case  2 : Serial.println("MQTT_CONNECT_BAD_CLIENT_ID");  break;
      case  3 : Serial.println("MQTT_CONNECT_UNAVAILABLE");  break;
      case  4 : Serial.println("MQTT_CONNECT_BAD_CREDENTIALS");  break;
      case  5 : Serial.println("MQTT_CONNECT_UNAUTHORIZED");  break;
      default : Serial.print("unknown state: "); Serial.println( client.state() );
    }
  }
  client.loop();
}

void mqtt_register_Callback(  mqtt_callback_t cbf ) {
  CB_ON_RECEIVE  = cbf;
}

/**
 * @brief Do any steps neccesary to enter the appropiate state
 *        BeforeEnter()
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


void mqtt_showWiFiStatus() {                 //called every 10 seconds
  String s;
  switch( WiFi.status() ) {
    case WL_NO_SHIELD:       s = "WL_NO_SHIELD"; break;
    case WL_IDLE_STATUS:     s = "WL_IDLE_STATUS"; break;
    case WL_NO_SSID_AVAIL:   s = "WL_NO_SSID_AVAIL"; break;
    case WL_SCAN_COMPLETED:  s = "WL_SCAN_COMPLETED"; break;
    case WL_CONNECTED:       s = "WL_CONNECTED"; break;
    case WL_CONNECT_FAILED:  s = "WL_CONNECT_FAILED"; break;
    case WL_CONNECTION_LOST: s = "WL_CONNECTION_LOST"; break;
    case WL_DISCONNECTED:    s = "WL_DISCONNECTED"; break;
  default: s = "Unknown WiFi state";
  }
  log_line( 7, s.c_str() );
}


void mqtt_checkWiFiStatus() {
  switch( WiFi.status() ) {
    case WL_NO_SHIELD:        break;
    case WL_IDLE_STATUS:      mqtt_enterState( MQTT_None );  //recognize a potential connection loss
                              break;
    case WL_NO_SSID_AVAIL:    break;
    case WL_SCAN_COMPLETED:   break;
    case WL_CONNECTED:        break;
    case WL_CONNECT_FAILED:   break;
    case WL_CONNECTION_LOST:  break;
    case WL_DISCONNECTED:     mqtt_enterState( MQTT_None );   //recognize a potential connection loss
                              break;
  default: ;
  }
  if (mqttState == MQTT_None) {
    Serial.println("Reboot...");
    ESP.restart();
  }
}