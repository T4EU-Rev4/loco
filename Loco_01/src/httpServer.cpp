#include <httpServer.h>
//#include <WebServer.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

// Sources:
// https://randomnerdtutorials.com/esp32-esp8266-input-data-html-form/
// https://techtutorialsx.com/2017/12/17/esp32-arduino-http-server-getting-query-parameters/ 
// https://lastminuteengineers.com/creating-esp32-web-server-arduino-ide/
// 


AsyncWebServer server( HTTP_SERVER_PORT );

//These are the names of the parameters we expect with the GET-Request
const char* PARAM_WAGON = "wagon";
const char* PARAM_CMD   = "cmd";
const char* PARAM_VAL   = "val";

//These variables will contain the received values
String  http_Wagon;
String  http_Cmd;
String  http_Value;
boolean http_NewValues = false;

String reply;

//char reply[80];     //Place to create the response to any request

//---- HTTP Source stored in FLASH ----
const char HTML_Page[] PROGMEM = 
#include "index.html"
;


void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}





// void http_Setup() {
//   server.on("/",      handle_OnConnect);
//   server.on("/local", handle_Local);
//   server.begin();
//   Serial.println("HTTP server started");
// }

void http_Setup() {

  // Send web page with input fields to client when accessing the root
  server.on("/", HTTP_GET, 
    [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", HTML_Page);
    }
  );

  // Send a GET request to <ESP_IP>/set?wagon=DK&cmd=2&val=1>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    // GET wagon value on <ESP_IP>/set?wagon=DK&cmd=2&val=1>
    if (request->hasParam(PARAM_WAGON)) {
      http_Wagon = request->getParam(PARAM_WAGON)->value();
    } else {
      http_Wagon = "";  
    }
    // GET input2 value on <ESP_IP>/set?wagon=DK&cmd=2&val=1>
    if (request->hasParam(PARAM_CMD)) {
      http_Cmd = request->getParam(PARAM_CMD)->value();
    } else {
        http_Cmd = "0"; 
    }
    // GET input3 value on <ESP_IP>/set?wagon=DK&cmd=2&val=1>
    if (request->hasParam(PARAM_VAL)) {
      http_Value = request->getParam(PARAM_VAL)->value();
    } else {
        http_Value = "0";
    }
    
    http_NewValues = true;

    reply = "Request received:  wagon=" + http_Wagon +
            "  Cmd="+ http_Cmd + "  Val=" + http_Value;
    Serial.println(reply);

    request->send( 200, "text/html", reply + "<br><a href=\"/\">Return</a>" );
  }
  );

  server.onNotFound(notFound);

  server.begin();
}

boolean http_NewValueAvail() {
    return http_NewValues;
}

void http_Clear() {
    http_NewValues = false;    
}

String  http_getWagon() {
    return http_Wagon;
}

uint8_t http_getCmd() {
    return atoi( http_Cmd.c_str() );
}

uint8_t http_getValue() {
    return atoi( http_Value.c_str() );
}


