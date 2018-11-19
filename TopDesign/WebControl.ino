#include <WiFi.h>
#include <WebServer.h>


uint16_t testData = 0;
String htmlIndex(double value = 0,int index = 0)
{
  
  String htmlIndex = "";
  htmlIndex += "<!DOCTYPE html><html><head>Charles</head><body>";
  htmlIndex += "<form id=\"1\" name=\"1\" method =\"post\" action=\"Control\" >  \
        <p>BKP:<input id=\"1\" name=\"1\" type=\"text\" /></p>  \
        <p><input id=\"subControl\"  name =\"subControl\" type=\"submit\" value=\"Submit\" /></p> \
    </form>\ ";
  htmlIndex +=  "<form id=\"2\" name=\"2\" method =\"post\"  action=\"Control\">  \
        <p>BKD:<input id=\"2\" name=\"2\" type=\"text\" /></p>  \
        <p><input id=\"subControl\"  name =\"subControl\" type=\"submit\" value=\"Submit\" /></p> \
    </form>";

  htmlIndex += "<form id=\"3\" name=\"3\" method =\"post\"  action=\"Control\">  \
        <p>VKP:<input id=\"3\" name=\"3\" type=\"text\" /></p>  \
        <p><input id=\"subControl\"  name =\"subControl\" type=\"submit\" value=\"Submit\" /></p> \
    </form>";
    
  htmlIndex += "<form id=\"4\" name=\"4\" method =\"post\"  action=\"Control\">  \
        <p>VKI:<input id=\"4\" name=\"4\" type=\"text\" /></p>  \
        <p><input id=\"subControl\"  name =\"subControl\" type=\"submit\" value=\"Submit\" /></p> \
    </form>\ ";

  htmlIndex += "<form id=\"5\" name=\"5\" method =\"post\"  action=\"Control\">  \
        <p>VKD:<input id=\"5\" name=\"5\" type=\"text\" /></p>  \
        <p><input id=\"subControl\"  name =\"subControl\" type=\"submit\" value=\"Submit\" /></p> \
    </form>\ ";

  htmlIndex += "    <form id=\"6\" name=\"6\" method =\"post\"  action=\"Control\">  \
        <p>TKP:<input id=\"6\" name=\"6\" type=\"text\" /></p>  \
        <p><input id=\"subControl\"  name =\"subControl\" type=\"submit\" value=\"Submit\" /></p> \
    </form>\ ";
  htmlIndex += " <form id=\"7\" name=\"7\" method =\"post\"  action=\"Control\">  \
        <p>TKI:<input id=\"7\" name=\"7\" type=\"text\" /></p>  \
        <p><input id=\"subControl\"  name =\"subControl\" type=\"submit\" value=\"Submit\" /></p> \
    </form>";
  htmlIndex += "<form id=\"8\" name=\"8\" method =\"post\"  action=\"Control\">  \
        <p>TKD:<input id=\"8\" name=\"8\" type=\"text\" /></p>  \
        <p><input id=\"subControl\"  name =\"subControl\" type=\"submit\" value=\"Submit\" /></p> \
    </form>\ ";
 htmlIndex +="<table border =\"2\">  <tr> <td> Angle:" + String(myBala.getRoll()) + "</td> <td>SpeedL:" + String(myBala.getSpeedL()) + "</td> <td> SpeedR:"+String(myBala.getSpeedR())+"</td> </tr> <tr> <td>"
 +String(myBala.getParaK(0))+"</td> <td>"+String(myBala.getParaK(1))+"</td> <td>"+String(myBala.getParaK(2))+"</td> </tr> <tr> <td>"
 +String(myBala.getParaK(3))+"</td> <td>"+String(myBala.getParaK(4))+"</td> <td>"+String(myBala.getParaK(5))+"</td> </tr> <tr> <td>"
 +String(myBala.getParaK(6))+"</td> <td>"+String(myBala.getParaK(7))+"</td> <td>"+String(myBala.getParaK(8))+"</td> </tr> <tr> <td>"
 +String(myBala.getParaK(9))+"</td> <td>"+String(myBala.getParaK(10))+"</td> <td>"+String(myBala.getParaK(11))+"</td> </tr> <tr> <td>"
 +"</table>";
  
  htmlIndex += "</body>\
</html>";
  

  return htmlIndex;
}
//const String htmlIndex = "<!DOCTYPE html><html><head>Charles</head><body>\
//		<form id=\"ControlBKP\" name=\"1\" method =\"post\" action=\"Control\" >  \
//        <p>BKP:<input id=\"ControlBKP\" name=\"ControlBKP\" type=\"text\" /></p>  \
//        <p><input id=\"subControl\"  name =\"subControl\" type=\"submit\" value=\"Submit\" /></p> \
//		</form>\ 
//		<form id=\"ControlBKD\" name=\"2\" method =\"post\"  action=\"Control.aspx\">  \
//        <p>BKD:<input id=\"ControlBKD\" name=\"ControlBKD\" type=\"text\" /></p>  \
//        <p><input id=\"subControl\"  name =\"subControl\" type=\"submit\" value=\"Submit\" /></p> \
//		</form>\
//		<form id=\"ControlVKP\" name=\"3\" method =\"post\"  action=\"Control.aspx\">  \
//        <p>VKP:<input id=\"ControlVKP\" name=\"ControlVKP\" type=\"text\" /></p>  \
//        <p><input id=\"subControl\"  name =\"subControl\" type=\"submit\" value=\"Submit\" /></p> \
//		</form>\
//		<form id=\"ControlVKI\" name=\"4\" method =\"post\"  action=\"Control.aspx\">  \
//        <p>VKI:<input id=\"ControlVKI\" name=\"ControlVKI\" type=\"text\" /></p>  \
//        <p><input id=\"subControl\"  name =\"subControl\" type=\"submit\" value=\"Submit\" /></p> \
//		</form>\ 
//		<form id=\"ControlVKD\" name=\"5\" method =\"post\"  action=\"Control.aspx\">  \
//        <p>VKD:<input id=\"ControlVKD\" name=\"ControlVKD\" type=\"text\" /></p>  \
//        <p><input id=\"subControl\"  name =\"subControl\" type=\"submit\" value=\"Submit\" /></p> \
//		</form>\ 
//		<form id=\"ControlTKP\" name=\"6\" method =\"post\"  action=\"Control.aspx\">  \
//        <p>TKP:<input id=\"ControlTKP\" name=\"ControlTKP\" type=\"text\" /></p>  \
//        <p><input id=\"subControl\"  name =\"subControl\" type=\"submit\" value=\"Submit\" /></p> \
//		</form>\ 		
//		<form id=\"ControlTKI\" name=\"7\" method =\"post\"  action=\"Control.aspx\">  \
//        <p>TKI:<input id=\"ControlTKP\" name=\"ControlTKP\" type=\"text\" /></p>  \
//        <p><input id=\"subControl\"  name =\"subControl\" type=\"submit\" value=\"Submit\" /></p> \
//		</form>\
//		<form id=\"ControlTKD\" name=\"8\" method =\"post\"  action=\"Control.aspx\">  \
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
  server.send(200, "text/html",htmlIndex());  
  

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


void handlePost()
{
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication(DIGEST_AUTH, www_realm, authFailResponse);
  
  myBala.setParaK((uint8_t)(server.argName(0)).toInt()-1,server.arg(0).toFloat());
  String message = "";
  message += " " + server.argName(0) + ": " + server.arg(0) + "\n"; 
  server.send(200, "text/html", htmlIndex(0,0));
  Serial.print(message);
}


void WiFiControl(void *parameter)
{
  WiFi.softAP(ssid, password);
  
  server.on("/", handleRoot);
  server.on("/update", handleUpdate);
  server.on("/Control",HTTP_POST,handlePost);
  server.onNotFound(handleNotFound);
  
  server.begin();

  Serial.println(String("Open http://") + WiFi.softAPIP() + " in your browser to see it working.");
  // Start Serial for diplay debug message
  Serial.begin(115200);
  delay(500);

  while(1)
  {
    server.handleClient();
    vTaskDelay(10);
  }
  vTaskDelete(NULL);
}

