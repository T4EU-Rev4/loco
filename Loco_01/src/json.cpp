#include <ArduinoJSON.h>
#include "json.h"
#include "mqtt.h"


String Location    = "DE";     //The Countrycode, where the showcase is located

String MAC         = "C8:C9:A3:C5:E5:A4";       //preset: MAC-Adress of my ESP
String TopicPrefix = "t4eu_";                   //prefix of all topics  -> namespace
String TopicSend   = TopicPrefix + "Server";    //topic, where the server is listening to
String TopicRec0   = TopicPrefix + MAC;         //
String TopicRec1   = TopicPrefix + Location;    //default reply from Server
String ClientName;  

boolean handshakeRequest = false;
uint16_t handshakeCount = 0;

char  toPublish[BUFFERSIZE];        //buffer for messages to send

String msgTypes[NR_OF_MSGTYPES] = { 
    "Boot",
    "Topic",
    "Device",
    "Handshake"
};

String msgParam[NR_OF_PARAMS] = {       //keep inline with enum jsParam 
    "country",
    "sender",
    "client",
    "topic",
    "wagon",
    "pin",
    "status"
};

void json_setMAC( String mac ){
    MAC = mac;
    TopicRec0   = TopicPrefix + MAC;
}


//a very useful tool to do this can be found here:
//https://arduinojson.org/v6/assistant/#/step4

/**
 * @brief Create a Server Msg object and serializes the msg 
 *          to the String toPublish
 * 
 * @param msgTyp 
 *        Supported types are:      
 *        boot, topic, device, handshake
 */
void json_createServerMsg( enum jsMsgTyp msgTyp ){
    StaticJsonDocument<256> doc;       

    doc["msgType"] = msgTypes[msgTyp];        //"Boot";

    JsonObject msgData = doc.createNestedObject("msgData");
    switch ( msgTyp ) {
     case mt_boot:  
                msgData[ msgParam[Country] ] = Location;
                msgData[ msgParam[Client] ]  = MAC;
                msgData[ msgParam[Topic] ]   = TopicRec0;
                break;
     case mt_topic: 
                msgData[ msgParam[Country] ] = Location;
                msgData[ msgParam[Client] ]  = ClientName;
                msgData[ msgParam[Topic] ]   = TopicRec1;
                break;
     case mt_device:
                break;
     case mt_handshake:
                msgData[ msgParam[Sender] ] = "client";
                msgData[ msgParam[Country] ] = Location;
                break;
    default:
        break;
    }
    serializeJson(doc, toPublish, BUFFERSIZE );
}



void json_mqtt_callback(char *topic, byte *payload, unsigned int length) {
  StaticJsonDocument<256> doc;
  DeserializationError error = deserializeJson(doc, payload, length);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  //-----
  if ( doc["msgType"] == msgTypes[mt_boot] ) {
    //brings new values for client and topic
    TopicRec1 =  String(  (const char*)doc["msgData"][ msgParam[Topic] ]  );
    ClientName=  String(  (const char*)doc["msgData"][ msgParam[Client] ]  );

        Serial.println( "   new topic=" + TopicRec1 );
        Serial.println( "   new client=" + ClientName );
    if (mqttState == MQTT_Connecting) {
            mqtt_enterState( MQTT_Connected );
    }
  }
  if ( doc["msgType"] == msgTypes[mt_topic] ) {
    //we do not expect to receive a message with this messagetype
  }
  if ( doc["msgType"] == msgTypes[mt_device] ) {
    //command for the device received
    //We accept this command without teking the actual MQTT-State into account
    uint16_t adr = convertCountryCode( String(  (const char*)doc["msgData"][ msgParam[Wagon] ]  ).c_str() );
    uint8_t  cmd = atoi( String(  (const char*)doc["msgData"][ msgParam[Pin] ]  ).c_str() ); 
    uint8_t  val = atoi( String(  (const char*)doc["msgData"][ msgParam[Status] ]  ).c_str() ); 
    mqtt_Received( adr, cmd, val );

  }
  if ( doc["msgType"] == msgTypes[mt_handshake] ) {
    //handshake request received
    //Attribute sender is actuelly not needed 
    handshakeRequest = true;
    handshakeCount++;
    //Serial.print("Handshake request  ");
  }


  //Serial.println("-----------------------");
}



