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

String htmlGenerateOneParameter(uint8_t id, String name, uint8_t precision)
{
  String div = String("<div style = \"margin-bottom:30px;\"><div style = \"float:left;margin-right:10px;\"><form id=\"")
  + String(id)
  + String("\" name=\"")
  + String(id)
  + String("\" method =\"post\" action=\"Tuning\">")
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

String htmlGenerateSwitch(uint8_t checked, String id, String label)
{
  String sw = String("<div style = \"margin-top:10px;margin-left:300px;\">")
  + String("<input type = \"checkbox\"") 
  + ((checked) ? String(" checked ") : String(" "))
  + String("id = \"")
  + id
  + String("\" onClick = \"checkboxOnclick()\">")
  + label
  + String("</div></br>");
  return sw;
}

String htmlIndex(uint8_t avoidance, uint8_t track)
{
  String htmlIndex = "<!DOCTYPE html><html><head><h5>Bala Remote Control</h5></head><body>";

  htmlIndex += "<div>";

  htmlIndex += htmlGenerateSwitch(avoidance, String("Avoidance"), String("Avoidance Mode"));

  htmlIndex += "<div style = \"margin-top:10px;margin-bottom:10px;\"><table border =\"2\"><tr> \
  <td width = \"100\">Battery:<span id = 'Battery'></span></td> \
  <td width = \"100\">Angle:<span id = 'Angle'></span></td> \
  <td width = \"100\">SpeedL:<span id = 'SpeedL'></span></td> \
  <td width = \"100\">SpeedR:<span id = 'SpeedR'></span></td> \
  <td width = \"100\">Dist:<span id = 'Distance'></span></td> \
  </tr></table></div></br>";

  htmlIndex += "<div style = \"margin-top:10px;margin-bottom:30px;\"><form name=\"input\" action=\"Motion\" method=\"post\"> \
  <input name=\"F\" type=\"submit\" value=\"Forward\"> \
  <input name=\"B\" type=\"submit\" value=\"Backward\"> \
  <input name=\"L\" type=\"submit\" value=\"Left\"> \
  <input name=\"R\" type=\"submit\" value=\"Right\"> \
  <input name=\"S\" type=\"submit\" value=\"Stop\"> \
  </form></div></br>";

  htmlIndex += htmlGenerateOneParameter(1, String("BKP"), 4);
  htmlIndex += htmlGenerateOneParameter(2, String("BKD"), 4);
  htmlIndex += htmlGenerateOneParameter(3, String("VKP"), 4);
  htmlIndex += htmlGenerateOneParameter(4, String("VKI"), 4);
  htmlIndex += htmlGenerateOneParameter(5, String("TKP"), 4);
  htmlIndex += htmlGenerateOneParameter(6, String("TKD"), 4);
  htmlIndex += htmlGenerateOneParameter(7, String("MDZ"), 2);
  htmlIndex += htmlGenerateOneParameter(8, String("Target_Angle"), 2);
  htmlIndex += htmlGenerateOneParameter(9, String("Movement_Step"), 2);
  htmlIndex += htmlGenerateOneParameter(10, String("Turn_Base"), 2);
  htmlIndex += htmlGenerateOneParameter(11, String("Turn_Step"), 2);  

  htmlIndex += "</div>";

  htmlIndex += " <script>\
  function checkboxOnclick(){ \
    var xhr = new XMLHttpRequest();\
    xhr.open('POST', '/Mode');\
    xhr.setRequestHeader(\"Content-type\", \"application/x-www-form-urlencoded\");\
    if (document.getElementById(\"Avoidance\").checked==true){ \
      xhr.send(\"Avoidance=on\");\
    } else {\
      xhr.send(\"Avoidance=off\");\
    }\
  }\
  requestData(); \
  setInterval(requestData, 100);\
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

void handleRoot(AsyncWebServerRequest *request)
{
  if (!request->authenticate(www_username, www_password))
    return request->requestAuthentication();

  request->send(200, "text/html", htmlIndex(avoidance_en, false));
}

void handleUpdate(AsyncWebServerRequest *request)
{
  if (!request->authenticate(www_username, www_password))
    return request->requestAuthentication();

  request->send(200, "application/json",
     "{\"Battery\": " + String(myBala.getBatteryVoltage(),2) + ","
    + "\"Angle\": " + String(myBala.getRoll(),2) + "," 
    + "\"SpeedL\": " + String(myBala.getSpeedL()) + "," 
    + "\"SpeedR\": " + String(myBala.getSpeedR()) + ","
    + "\"Distance\": " + String(dist_cm) + "}");
}

void handleTuning(AsyncWebServerRequest *request)
{
  if (!request->authenticate(www_username, www_password))
    return request->requestAuthentication();

  myBala.setParaK((uint8_t)(request->argName((size_t)0)).toInt()-1, request->arg((size_t)0).toFloat());
  myFlash.updateEEPROM(myBala);
  
  request->redirect("/");
}

void handleMotion(AsyncWebServerRequest *request)
{
  if (!request->authenticate(www_username, www_password))
    return request->requestAuthentication();

  if (request->argName((size_t)0) == "F")
    myBala.move(1);
  else if (request->argName((size_t)0) == "B")
    myBala.move(2);
  else if (request->argName((size_t)0) == "L")
    myBala.turn(1);
  else if (request->argName((size_t)0) == "R")
    myBala.turn(2);
  else if (request->argName((size_t)0) == "S") 
    myBala.stop();

  request->redirect("/");    
}

void handleMode(AsyncWebServerRequest *request) 
{
  if (!request->authenticate(www_username, www_password))
    return request->requestAuthentication();
   
  if (request->argName((size_t)0) == "Avoidance") 
  {
    avoidance_en = (String(request->arg((size_t)0)) == String("on"));
    if (!avoidance_en) myFlash.initEEPROM(myBala);  // if quit avoidance mode, reset original parameters (especially 'movement_step')
  }

  request->redirect("/"); 
}

void handleNotFound(AsyncWebServerRequest *request) 
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += request->url();
  message += "\nMethod: ";
  message += (request->method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += request->args();
  message += "\n";
  for (uint8_t i = 0; i < request->args(); i++)
    message += " " + request->argName(i) + ": " + request->arg(i) + "\n";
  request->send(404, "text/plain", message);
}

void WiFiControl(void *parameter)
{
  WiFi.softAP(ssid, password);

  server.on("/", handleRoot);
  server.on("/Update", handleUpdate);
  server.on("/Mode", HTTP_POST, handleMode);
  server.on("/Tuning", HTTP_GET, handleRoot);
  server.on("/Tuning", HTTP_POST, handleTuning);
  server.on("/Motion", HTTP_GET, handleRoot);
  server.on("/Motion", HTTP_POST, handleMotion);
  server.onNotFound(handleNotFound);
  server.begin();

  // Start Serial for diplay debug message
  Serial.begin(115200);
  delay(500);

  Serial.println(String("Open http://") + WiFi.softAPIP() + " in your browser to see it working.");

  vTaskDelete(NULL);
}

