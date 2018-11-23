/* Copyright (C) 2018 gyfastas, Charles. All rights reserved.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <WiFi.h>
#include <WebServer.h>

String htmlGenerateOneParameter(uint8_t id, String name, uint8_t precision)
{
  String div = String("<div style = \"margin-bottom:30px;\"><div style = \"float:left;margin-right:10px;\"><form id=\"")
  + String(id)
  + String("\" name=\"")
  + String(id)
  + String("\" method =\"post\" action=\"Control\">")
  + name 
  + String("  <input id=\"")
  + String(id)
  + String("\" name=\"")
  + String(id)
  + String("\" type=\"text\" /><input id=\"subControl\" name =\"subControl\" type=\"submit\" value=\"Submit\" /></form></div>")
  + String("<div style = \"float:left;\"><span id = 'K")
  + String(id-1)
  + String("'></span>") 
  + String(myBala.getParaK(id-1),precision) 
  + String("</div></div></br>");
  return div;  
}

String htmlIndex()
{
  String htmlIndex = "<!DOCTYPE html><html><head><h5>Bala Remote Control</h5></head><body>";

  htmlIndex += "<div>";

  htmlIndex += "<div style = \"margin-top:10px;margin-bottom:10px;\"><table border =\"2\"><tr> \
  <td width = \"100\">Battery:<span id = 'Battery'></span></td> \
  <td width = \"100\">Angle:<span id = 'Angle'></span></td> \
  <td width = \"100\">SpeedL:<span id = 'SpeedL'></span></td> \
  <td width = \"100\">SpeedR:<span id = 'SpeedR'></span></td> \
  <td width = \"100\">Dist:<span id = 'Distance'></span></td> \
  </tr></table></div></br>";

  htmlIndex += htmlGenerateOneParameter(1, String("BKP"), 4);
  htmlIndex += htmlGenerateOneParameter(2, String("BKD"), 4);
  htmlIndex += htmlGenerateOneParameter(3, String("VKP"), 4);
  htmlIndex += htmlGenerateOneParameter(4, String("VKI"), 4);
  htmlIndex += htmlGenerateOneParameter(5, String("VKD"), 4);
  htmlIndex += htmlGenerateOneParameter(6, String("TKP"), 4);
  htmlIndex += htmlGenerateOneParameter(7, String("TKI"), 4);
  htmlIndex += htmlGenerateOneParameter(8, String("TKD"), 4);
  htmlIndex += htmlGenerateOneParameter(9, String("SDK"), 4);
  htmlIndex += htmlGenerateOneParameter(10, String("TarAngle"), 2);
  htmlIndex += htmlGenerateOneParameter(11, String("VecPeriod"), 0);
  htmlIndex += htmlGenerateOneParameter(12, String("Cardown"), 0);
  htmlIndex += htmlGenerateOneParameter(13, String("MDZ"), 2);

  htmlIndex += "</div>";

  htmlIndex += " <script>\
   requestData(); \
   setInterval(requestData, 200);\
   function requestData() {\
     var xhr = new XMLHttpRequest();\
     xhr.open('GET', '/Update');\
     xhr.onload = function() {\
       if (xhr.status === 200) {\
         if (xhr.responseText) {\
           var data = JSON.parse(xhr.responseText);\
           document.getElementById(\"Battery\").innerText = data.Battery;\
           document.getElementById(\"Angle\").innerText = data.Angle;\
           document.getElementById(\"SpeedL\").innerText = data.SpeedL;\
           document.getElementById(\"SpeedR\").innerText = data.SpeedR;\
           document.getElementById(\"Distance\").innerText = data.Distance;\
         } else {\
           document.getElementById(\"Battery\").innerText = \"0.0000\";\
           document.getElementById(\"Angle\").innerText = \"0.0000\";\
           document.getElementById(\"SpeedL\").innerText = \"0.0000\";\
           document.getElementById(\"SpeedR\").innerText = \"0.0000\";\
           document.getElementById(\"Distance\").innerText = \"0.0000\";\
         }\
       } else {\
           document.getElementById(\"Battery\").innerText = \"0.0000\";\
           document.getElementById(\"Angle\").innerText = \"0.0000\";\
           document.getElementById(\"SpeedL\").innerText = \"0.0000\";\
           document.getElementById(\"SpeedR\").innerText = \"0.0000\";\
           document.getElementById(\"Distance\").innerText = \"0.0000\";\
       }\
     };\
     xhr.send();\
   }\
 </script>";

    htmlIndex += "</body></html>";

  return htmlIndex;
}

void handleRoot()
{
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication(DIGEST_AUTH, www_realm, authFailResponse);

  server.send(200, "text/html", htmlIndex());
}

void handleUpdate()
{
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication(DIGEST_AUTH, www_realm, authFailResponse);

  server.send(200, "application/json",
     "{\"Battery\": " + String(myBala.getBatteryVoltage(),2) + ","
    + "\"Angle\": " + String(myBala.getRoll(),2) + "," 
    + "\"SpeedL\": " + String(myBala.getSpeedL()) + "," 
    + "\"SpeedR\": " + String(myBala.getSpeedR()) + ","
    + "\"Distance\": " + String(dist_cm) + "}");
}

void handleControl()
{
  if (!server.authenticate(www_username, www_password))
    return server.requestAuthentication(DIGEST_AUTH, www_realm, authFailResponse);

  myBala.setParaK((uint8_t)(server.argName(0)).toInt()-1,server.arg(0).toFloat());
  myFlash.updateEEPROM(myBala);
  server.send(200, "text/html", htmlIndex());
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
  for (uint8_t i = 0; i < server.args(); i++)
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  server.send(404, "text/plain", message);
}

void WiFiControl(void *parameter)
{
  WiFi.softAP(ssid, password);

  server.on("/", handleRoot);
  server.on("/Update", handleUpdate);
  server.on("/Control",HTTP_POST,handleControl);
  server.onNotFound(handleNotFound);
  server.begin();

  // Start Serial for diplay debug message
  Serial.begin(115200);
  delay(500);

  Serial.println(String("Open http://") + WiFi.softAPIP() + " in your browser to see it working.");

  while(1)
  {
    server.handleClient();
    vTaskDelay(10);
  }
  vTaskDelete(NULL);
}

