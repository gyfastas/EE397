#include <WiFi.h>
#include <WebServer.h>

const char* ssid     = "ESP32-Access-Point";
const char* password = "123456789";

WebServer server(80);

const char* www_username = "admin";
const char* www_password = "LWYGN";
// allows you to set the realm of authentication Default:"Login Required"
const char* www_realm = "Custom Auth Realm";
// the Content of the HTML response in case of Unautherized Access Default:empty
String authFailResponse = "Authentication Failed";

uint16_t testData = 0;

const String htmlIndex = "<!DOCTYPE html><html><head>Charles</head><body>\
  <p>testData: <span id='testData'></span> sec</p>\
  <script>\
    requestData(); \
    setInterval(requestData, 1000);\
    function requestData() {\
      var xhr = new XMLHttpRequest();\
      xhr.open('GET', '/update');\
      xhr.onload = function() {\
        if (xhr.status === 200) {\
          if (xhr.responseText) {\
            var data = JSON.parse(xhr.responseText);\
            document.getElementById(\"testData\").innerText = data.testData;\
          } else {\
            document.getElementById(\"testData\").innerText = \"?\";\
          }\
        } else {\
          document.getElementById(\"testData\").innerText = \"?\";\
        }\
      };\
      xhr.send();\
    }\
  </script>\
</body>\
</html>";

void handleRoot()
{
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication(DIGEST_AUTH, www_realm, authFailResponse);
  server.send(200, "text/html", htmlIndex);
}

void handleUpdate()
{
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication(DIGEST_AUTH, www_realm, authFailResponse); 
  server.send(200, "application/json", "{\"testData\": " + String(testData) + "}");  
}

void handleNotFound() 
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void inc(void *parameter)
{
  while(1)
  {
    testData += 1;
    vTaskDelay(1000);
  }
  vTaskDelete(NULL);  
}

void setup() 
{
  Serial.begin(115200);

  // Init WiFi in AP mode
  Serial.print("Setting AP (Access Point)…");
  WiFi.softAP(ssid, password);

  server.on("/", handleRoot);
  server.on("/update", handleUpdate);
  server.onNotFound(handleNotFound);
  
  server.begin();

  Serial.println(String("Open http://") + WiFi.softAPIP() + " in your browser to see it working.");

  xTaskCreatePinnedToCore(
    inc,          /* Task function. */
    "inc",        /* String with name of task. */
    1000,         /* Stack size in words. */
    NULL,         /* Parameter passed as input of the task */
    2,            /* Priority of the task. */
    NULL,         /* Task handle. */
    0);           /* Run on core 0. */
}

void loop() 
{
  server.handleClient();
}

