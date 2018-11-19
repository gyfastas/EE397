#include <WiFi.h>
#include <ESPmDNS.h>
#include <ArduinoOTA.h>
#include <WebServer.h>

// Replace with your network credentials
const char* ssid     = "ESP32-Access-Point";
const char* password = "123456789";

WebServer server(80);

const char* www_username = "admin";
const char* www_password = "esp32";
// allows you to set the realm of authentication Default:"Login Required"
const char* www_realm = "Custom Auth Realm";
// the Content of the HTML response in case of Unautherized Access Default:empty
String authFailResponse = "Authentication Failed";

String printPage()
{ 
  String message = "<form id=\"login\" name=\"login\" method =\"post\"  action=\"Post.aspx\">  \
        <p>UserName:<input id=\"txtUserName\" name=\"txtUserName\" type=\"text\" /></p>   \
        <p>PassWord:<input id=\"txtPWD\" name=\"txtPWD\" type=\"text\" /></p>  \
        <p><input id=\"subLogin\"  name =\"subLogin\" type=\"submit\" value=\"Submit\" /></p> \
        </form> ";
  return message;
}

void handleRoot()
{
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication(DIGEST_AUTH, www_realm, authFailResponse);
  server.send(200, "text/html", printPage());
}

void handlePost()
{
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication(DIGEST_AUTH, www_realm, authFailResponse); 
  String message = "";
  for (uint8_t i = 0; i < server.args(); i++)
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n"; 
  server.send(200, "test/plain", message); 
  Serial.print(message);
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

void setup() 
{
  Serial.begin(115200);
  
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  server.on("/", handleRoot);
  server.on("/Post.aspx", HTTP_POST, handlePost);
  server.onNotFound(handleNotFound);
  
  server.begin();

  Serial.print("Open http://");
  Serial.print(WiFi.softAPIP());
  Serial.println("/ in your browser to see it working");
}

void loop() 
{
  server.handleClient();
}



