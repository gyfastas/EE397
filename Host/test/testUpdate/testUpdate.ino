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
String htmlIndex(double value = 0,int index = 0)
{
  
  String htmlIndex = "";
  htmlIndex += "<!DOCTYPE html><html><head>Charles</head><body>";
  htmlIndex += "<form id=\"ControlBKP\" name=\"ControlBKP\" method =\"post\" action=\"Control\" >  \
        <p>BKP:<input id=\"ControlBKP\" name=\"ControlBKP\" type=\"text\" /></p>  \
        <p><input id=\"subControl\"  name =\"subControl\" type=\"submit\" value=\"Submit\" /></p> \
    </form>\ ";
  htmlIndex +=  "<form id=\"ControlBKD\" name=\"ControlBKD\" method =\"post\"  action=\"Control\">  \
        <p>BKD:<input id=\"ControlBKD\" name=\"ControlBKD\" type=\"text\" /></p>  \
        <p><input id=\"subControl\"  name =\"subControl\" type=\"submit\" value=\"Submit\" /></p> \
    </form>";

  htmlIndex += "<form id=\"ControlVKP\" name=\"ControlVKP\" method =\"post\"  action=\"Control\">  \
        <p>VKP:<input id=\"ControlVKP\" name=\"ControlVKP\" type=\"text\" /></p>  \
        <p><input id=\"subControl\"  name =\"subControl\" type=\"submit\" value=\"Submit\" /></p> \
    </form>";
    
  htmlIndex += "<form id=\"ControlVKI\" name=\"ControlVKI\" method =\"post\"  action=\"Control\">  \
        <p>VKI:<input id=\"ControlVKI\" name=\"ControlVKI\" type=\"text\" /></p>  \
        <p><input id=\"subControl\"  name =\"subControl\" type=\"submit\" value=\"Submit\" /></p> \
    </form>\ ";

  htmlIndex += "<form id=\"ControlVKD\" name=\"ControlVKD\" method =\"post\"  action=\"Control\">  \
        <p>VKD:<input id=\"ControlVKD\" name=\"ControlVKD\" type=\"text\" /></p>  \
        <p><input id=\"subControl\"  name =\"subControl\" type=\"submit\" value=\"Submit\" /></p> \
    </form>\ ";

  htmlIndex += "    <form id=\"ControlTKP\" name=\"ControlTKP\" method =\"post\"  action=\"Control\">  \
        <p>TKP:<input id=\"ControlTKP\" name=\"ControlTKP\" type=\"text\" /></p>  \
        <p><input id=\"subControl\"  name =\"subControl\" type=\"submit\" value=\"Submit\" /></p> \
    </form>\ ";
  htmlIndex += " <form id=\"ControlTKI\" name=\"ControlTKI\" method =\"post\"  action=\"Control\">  \
        <p>TKI:<input id=\"ControlTKI\" name=\"ControlTKI\" type=\"text\" /></p>  \
        <p><input id=\"subControl\"  name =\"subControl\" type=\"submit\" value=\"Submit\" /></p> \
    </form>";
  htmlIndex += "<form id=\"ControlTKD\" name=\"ControlTKD\" method =\"post\"  action=\"Control\">  \
        <p>TKD:<input id=\"ControlTKD\" name=\"ControlTKD\" type=\"text\" /></p>  \
        <p><input id=\"subControl\"  name =\"subControl\" type=\"submit\" value=\"Submit\" /></p> \
    </form>\ ";
  htmlIndex +="<table border = 2> <tr>" +"<td>" + myBala.getRoll() + "</td> <td>" + myBala.getSpeedL() + "</td> <td>"+myBala.getSpeedR()+"</td> </tr> </table>\";
  
  htmlIndex += "</body>\
</html>";
  
  




  return htmlIndex;
}
//const String htmlIndex = "<!DOCTYPE html><html><head>Charles</head><body>\
//		<form id=\"ControlBKP\" name=\"ControlBKP\" method =\"post\" action=\"Control\" >  \
//        <p>BKP:<input id=\"ControlBKP\" name=\"ControlBKP\" type=\"text\" /></p>  \
//        <p><input id=\"subControl\"  name =\"subControl\" type=\"submit\" value=\"Submit\" /></p> \
//		</form>\ 
//		<form id=\"ControlBKD\" name=\"ControlBKD\" method =\"post\"  action=\"Control.aspx\">  \
//        <p>BKD:<input id=\"ControlBKD\" name=\"ControlBKD\" type=\"text\" /></p>  \
//        <p><input id=\"subControl\"  name =\"subControl\" type=\"submit\" value=\"Submit\" /></p> \
//		</form>\
//		<form id=\"ControlVKP\" name=\"ControlVKP\" method =\"post\"  action=\"Control.aspx\">  \
//        <p>VKP:<input id=\"ControlVKP\" name=\"ControlVKP\" type=\"text\" /></p>  \
//        <p><input id=\"subControl\"  name =\"subControl\" type=\"submit\" value=\"Submit\" /></p> \
//		</form>\
//		<form id=\"ControlVKI\" name=\"ControlVKI\" method =\"post\"  action=\"Control.aspx\">  \
//        <p>VKI:<input id=\"ControlVKI\" name=\"ControlVKI\" type=\"text\" /></p>  \
//        <p><input id=\"subControl\"  name =\"subControl\" type=\"submit\" value=\"Submit\" /></p> \
//		</form>\ 
//		<form id=\"ControlVKD\" name=\"ControlVKD\" method =\"post\"  action=\"Control.aspx\">  \
//        <p>VKD:<input id=\"ControlVKD\" name=\"ControlVKD\" type=\"text\" /></p>  \
//        <p><input id=\"subControl\"  name =\"subControl\" type=\"submit\" value=\"Submit\" /></p> \
//		</form>\ 
//		<form id=\"ControlTKP\" name=\"ControlTKP\" method =\"post\"  action=\"Control.aspx\">  \
//        <p>TKP:<input id=\"ControlTKP\" name=\"ControlTKP\" type=\"text\" /></p>  \
//        <p><input id=\"subControl\"  name =\"subControl\" type=\"submit\" value=\"Submit\" /></p> \
//		</form>\ 		
//		<form id=\"ControlTKI\" name=\"ControlTKI\" method =\"post\"  action=\"Control.aspx\">  \
//        <p>TKI:<input id=\"ControlTKP\" name=\"ControlTKP\" type=\"text\" /></p>  \
//        <p><input id=\"subControl\"  name =\"subControl\" type=\"submit\" value=\"Submit\" /></p> \
//		</form>\
//		<form id=\"ControlTKD\" name=\"ControlTKD\" method =\"post\"  action=\"Control.aspx\">  \
//        <p>TKD:<input id=\"ControlTKD\" name=\"ControlTKD\" type=\"text\" /></p>  \
//        <p><input id=\"subControl\"  name =\"subControl\" type=\"submit\" value=\"Submit\" /></p> \
//		</form>\ 	
//  <p>BKP: <span id='BKP'></span> </p>\
//  <p>BKD: <span id='BKD'></span> </p>\
//  <p>VKP: <span id='VKP'></span> </p>\
//  <p>VKI: <span id='VKI'></span> </p>\
//  <p>VKD: <span id='VKD'></span> </p>\
//  <p>\
//  <input type = 'range',min = '-100', max = '100', value= '0'/>Test value:\
//  <span id = 'test'>\
//  </span>\
//  </p>\
//  <script>\
//    requestData(); \
//    setInterval(requestData, 1000);\
//    function requestData() {\
//      var xhr = new XMLHttpRequest();\
//      xhr.open('GET', '/update');\
//      xhr.onload = function() {\
//        if (xhr.status === 200) {\
//          if (xhr.responseText) {\
//            var data = JSON.parse(xhr.responseText);\
//            document.getElementById(\"BKP\").innerText = data.BKP;\
//            document.getElementById(\"BKD\").innerText = data.BKD;\
//            document.getElementById(\"VKP\").innerText = data.VKP;\
//            document.getElementById(\"VKI\").innerText = data.VKI;\
//            document.getElementById(\"VKD\").innerText = data.VKD;\
//          } else {\
//            document.getElementById(\"BKP\").innerText = \"?\";\
//			      document.getElementById(\"BKD\").innerText = \"?\";\
//            document.getElementById(\"VKP\").innerText = \"?\";\
//            document.getElementById(\"VKI\").innerText = \"?\";\
//            document.getElementById(\"VKD\").innerText = \"?\";\
//          }\
//        } else {\
//            document.getElementById(\"BKP\").innerText = \"?\";\
//			document.getElementById(\"BKD\").innerText = \"?\";\
//            document.getElementById(\"VKP\").innerText = \"?\";\
//            document.getElementById(\"VKI\").innerText = \"?\";\
//            document.getElementById(\"VKD\").innerText = \"?\";\
//        }\
//      };\
//      xhr.send();\
//    }\
//  </script>\
//</body>\
//</html>";

void handleRoot()
{
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication(DIGEST_AUTH, www_realm, authFailResponse);
  server.send(200, "text/html", htmlIndex(0,0));
}

void handleUpdate()
{
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication(DIGEST_AUTH, www_realm, authFailResponse); 
  server.send(200, "application/json", "{\"testData\": " + String(testData) + "}");  
  

}
void handleControl()
{
	if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication(DIGEST_AUTH, www_realm, authFailResponse); 

	String mess = "";
	mess +=server.uri();
	Serial.println(mess);
	
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

void handlePost()
{
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication(DIGEST_AUTH, www_realm, authFailResponse); 
  String message = "";
  message += " " + server.argName(0) + ": " + server.arg(0) + "\n"; 
  server.send(200, "text/html", htmlIndex(0,0));
  Serial.print(message);
}


void setup() 
{
  Serial.begin(115200);

  // Init WiFi in AP mode
  Serial.print("Setting AP (Access Point)…");
  WiFi.softAP(ssid, password);
  
  server.on("/", handleRoot);
  server.on("/update", handleUpdate);
  server.on("/Control",HTTP_POST,handlePost);
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

