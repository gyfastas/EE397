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

/*************************************************************************/
// Task1 :           WiFi Configuration and Remote Control
/*************************************************************************/

String htmlGenerateOneParameter(uint8_t id, String name, uint8_t precision)
{
  String form = name;
  form += "<form id='";
  form += String(id);
  form += "' name='";
  form += String(id);
  form += "' method ='post' action='Tuning'>";
  form += "<input id='";
  form += String(id); 
  form += "' name='";
  form += String(id);
  form += "' type='text'> <input id='tuning' name ='submit' type='submit' style='width:70px;' value=";
  form += ((id < 12) ? String(myBala.getParaK(id-1),precision) : ((id == 12) ? String(safe_distance_cm) : ((id == 13) ? String(backward_time) : String(turnleft_time))));
  form += "></form>";
   
  return form;  
}

String htmlIndex(uint8_t avoidance, uint8_t track)
{
  String htmlIndex = "<!DOCTYPE html><html><head><h5 align='center' style='font-size:20px;'>Bala Remote Control</h5>\
  <style>\
    .no-select{\
      -webkit-touch-callout:none;\
      -webkit-user-select:none;\
      -khtml-user-select:none;\
      -moz-user-select:none;\
      -ms-user-select:none;\
      user-select:none;\
    }\
    .change_direction{\
      text-align: center;\
      position: relative;\
      width: 200px;\
      height: 200px;\
    }\
    .direction_content{\
      position: relative;\
      width: 100%;\
      height: 70%;\
    }\
    .direction_div{\
      position: relative;\
      width: 30px;\
      height: 30px;\
    }\
    .left_direction{\
      top: 30%;\
      left: 12%; \
    }\
    .bottom_direction{\
      top: 34%;\
      left: 40%;\
    }\
    .right_direction{\
      top: -13%;\
      left: 68%;\
    }\
    .top_direction{\
      top: 28%;\
      left: 40%;\
    }\
    .button {\
        border: 1;\
        padding: 4px 20px;\
        text-align: center;\
        display: inline-block;\
        font-size: 16px;\
        margin: 4px 2px;\
        cursor: pointer;\
    }\
  </style></head><body>";

  htmlIndex += "<div>";

  htmlIndex += "<div style='margin-top:30px;margin-bottom:30px;' align='center'>\
    <input"; 
  htmlIndex += (avoidance ? String(" checked ") : String(" "));
  htmlIndex +=    "type ='checkbox' id = 'Avoidance' onClick = 'onCheckboxClick()'>Avoidance Mode\
      <input";
  htmlIndex += (track ? String(" checked ") : String(" "));
  htmlIndex +=    "style = 'margin-left:50px;' type = 'checkbox' id = 'Track' onClick = 'onCheckboxClick()'>Tracking Mode\
  </div>";

  htmlIndex += "<div>\
    <table style='margin-top:10px;margin-bottom:30px;' align='center' border ='1'>\
        <tr>\
      <td width = '100'>Battery:<span id = 'Battery'></span></td>\
      <td width = '100'>Angle:<span id = 'Angle'></span></td>\
      <td width = '100'>SpeedL:<span id = 'SpeedL'></span></td>\
      <td width = '100'>SpeedR:<span id = 'SpeedR'></span></td>\
      <td width = '100'>DistForward:<span id = 'DistanceForward'></span></td>\
      <td width = '100'>DistLeft:<span id = 'DistanceLeft'></span></td>\
      <td width = '100'>Command:<span id = 'Command'></span></td>\
        </tr>\
    </table>\
  </div>";

  htmlIndex += "<div class='no-select'>\
    <div style='margin: 0 auto;' class='change_direction'>\
      <div class='direction_content'>\
        <div class='direction_div top_direction'>\
          <button id='F' class='button' ontouchstart='sendMotion()' ontouchend='sendStop()' onmousedown='sendMotion()' onmouseup='sendStop()'>F</button>\
        </div>\
        <div class='direction_div left_direction'>\
          <button id='L' class='button' ontouchstart='sendMotion()' ontouchend='sendStop()' onmousedown='sendMotion()' onmouseup='sendStop()'>L</button>\
        </div>\
        <div class='direction_div bottom_direction'>\
          <button id='B' class='button' ontouchstart='sendMotion()' ontouchend='sendStop()' onmousedown='sendMotion()' onmouseup='sendStop()'>B</button>\
        </div>\
        <div class='direction_div right_direction'>\
          <button id='R' class='button' ontouchstart='sendMotion()' ontouchend='sendStop()' onmousedown='sendMotion()' onmouseup='sendStop()'>R</button>\
        </div>\
      </div>\
    </div>\
  </div>";

  htmlIndex += "<table align='center' style='margin-bottom:50px;'><tr><td>Move Certain Distance (m)\
              <form id='MCD' name='MCD' method='post' action='Motion'>\
                <input id='MCD' name='MCD' type='text'>\
                <input id='MCD' name='submit' type='submit' style='width:70px;' value='Go'>\
                <span id='targetDist'></span>\
              </form></td></tr></table>";

  htmlIndex += "<table align='center' style='margin-bottom:50px;'><tr><td>Rotate Certain Angle (degree)\
              <form id='RCA' name='RCA' method='post' action='Motion'>\
                <input id='RCA' name='RCA' type='text'>\
                <input id='RCA' name='submit' type='submit' style='width:70px;' value='Go'>\
                <span id='targetYaw'></span>\
              </form></td></tr></table>";

  htmlIndex += "<table align='center'><tr><td><div style='float:left;margin-bottom:20px;'>";
  htmlIndex += "<div style='float:left;'>";
  htmlIndex += htmlGenerateOneParameter(1, String("BKP"), 4);
  htmlIndex += "</div><div style='float:left;margin-left:50px'>";
  htmlIndex += htmlGenerateOneParameter(2, String("BKD"), 4);
  htmlIndex += "</div></div></td></tr></table>";

  htmlIndex += "<table align='center'><tr><td><div style='float:left;margin-bottom:20px;'>";
  htmlIndex += "<div style='float:left;'>";
  htmlIndex += htmlGenerateOneParameter(3, String("VKP"), 4);
  htmlIndex += "</div><div style='float:left;margin-left:50px'>";
  htmlIndex += htmlGenerateOneParameter(4, String("VKI"), 4);
  htmlIndex += "</div></div></td></tr></table>";

  htmlIndex += "<table align='center'><tr><td><div style='float:left;margin-bottom:20px;'>";
  htmlIndex += "<div style='float:left;'>";
  htmlIndex += htmlGenerateOneParameter(5, String("TKP"), 4);
  htmlIndex += "</div><div style='float:left;margin-left:50px'>";
  htmlIndex += htmlGenerateOneParameter(6, String("TKD"), 4);
  htmlIndex += "</div></div></td></tr></table>";

  htmlIndex += "<table align='center'><tr><td><div style='float:left;margin-bottom:20px;'>";
  htmlIndex += "<div style='float:left;'>";
  htmlIndex += htmlGenerateOneParameter(7, String("MDZ"), 2);
  htmlIndex += "</div><div style='float:left;margin-left:50px'>";
  htmlIndex += htmlGenerateOneParameter(8, String("Target_Angle"), 2);
  htmlIndex += "</div></div></td></tr></table>";

  htmlIndex += "<table align='center'><tr><td><div style='float:left;margin-bottom:20px;'>";
  htmlIndex += "<div style='float:left;'>";
  htmlIndex += htmlGenerateOneParameter(9, String("Movement_Step"), 2);
  htmlIndex += "</div><div style='float:left;margin-left:50px'>";
  htmlIndex += htmlGenerateOneParameter(10, String("Turn_Base"), 2);
  htmlIndex += "</div></div></td></tr></table>";

  htmlIndex += "<table align='center'><tr><td><div style='float:left;margin-bottom:20px;'>";
  htmlIndex += "<div style='float:left;'>";
  htmlIndex += htmlGenerateOneParameter(11, String("Turn_Step"), 2); 
  htmlIndex += "</div><div style='float:left;margin-left:50px'>";
  htmlIndex += htmlGenerateOneParameter(12, String("Safe_Distance"), 0);
  htmlIndex += "</div></div></td></tr></table>";

  htmlIndex += "<table align='center'><tr><td><div style='float:left;margin-bottom:20px;'>";
  htmlIndex += "<div style='float:left;'>";
  htmlIndex += htmlGenerateOneParameter(13, String("Backward_Time"), 0);
  htmlIndex += "</div><div style='float:left;margin-left:50px'>";
  htmlIndex += htmlGenerateOneParameter(14, String("Turnleft_Time"), 0);
  htmlIndex += "</div></div></td></tr></table>";

  htmlIndex += "</div>";

  htmlIndex += " <script>\
  function onCheckboxClick(event){\
    event = event ? event : window.event;\
    var obj = event.srcElement ? event.srcElement : event.target;\
    var xhr = new XMLHttpRequest();\
    xhr.open('POST', '/Mode');\
    xhr.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');\
    if (document.getElementById(obj.id).checked==true){ \
      xhr.send(obj.id+'=on');\
    } else {\
      xhr.send(obj.id+'=off');\
    }\
  }\
  function sendMotion(event){ \
    event = event ? event : window.event;\
    var obj = event.srcElement ? event.srcElement : event.target;\
    var xhr = new XMLHttpRequest();\
    xhr.open('POST', '/Motion');\
    xhr.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');\
    xhr.send('Motion='+obj.id);\
  }\
  function sendStop(event){ \
    event = event ? event : window.event;\
    event.preventDefault(); /*Prevent click event after touch event on mobile*/\
    var obj = event.srcElement ? event.srcElement : event.target;\
    var xhr = new XMLHttpRequest();\
    xhr.open('POST', '/Motion');\
    xhr.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');\
    xhr.send('Stop='+obj.id); \
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
        document.getElementById('Battery').innerText = data.Battery;\
        document.getElementById('Angle').innerText = data.Angle;\
        document.getElementById('SpeedL').innerText = data.SpeedL;\
        document.getElementById('SpeedR').innerText = data.SpeedR;\
        document.getElementById('DistanceForward').innerText = data.DistanceForward;\
        document.getElementById('DistanceLeft').innerText = data.DistanceLeft;\
        document.getElementById('Command').innerText = data.Command;\
        document.getElementById('targetDist').innerText = data.targetDist;\
        document.getElementById('targetYaw').innerText = data.targetYaw;\
      } else {\
        document.getElementById('Battery').innerText = '0.0000';\
        document.getElementById('Angle').innerText = '0.0000';\
        document.getElementById('SpeedL').innerText = '0.0000';\
        document.getElementById('SpeedR').innerText = '0.0000';\
        document.getElementById('DistanceForward').innerText = '0.0000';\
        document.getElementById('DistanceLeft').innerText = '0.0000';\
        document.getElementById('Command').innerText = 'None';\
        document.getElementById('targetDist').innerText = '0.0000';\
        document.getElementById('targetYaw').innerText = '0.0000';\
      }\
    } else {\
      document.getElementById('Battery').innerText = '0.0000';\
      document.getElementById('Angle').innerText = '0.0000';\
      document.getElementById('SpeedL').innerText = '0.0000';\
      document.getElementById('SpeedR').innerText = '0.0000';\
      document.getElementById('DistanceForward').innerText = '0.0000';\
      document.getElementById('DistanceLeft').innerText = '0.0000';\
      document.getElementById('Command').innerText = 'None';\
      document.getElementById('targetDist').innerText = '0.0000';\
      document.getElementById('targetYaw').innerText = '0.0000';\
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
    + "\"SpeedL\": " + String(myBala.getSpeedL(),2) + "," 
    + "\"SpeedR\": " + String(myBala.getSpeedR(),2) + ","
    + "\"DistanceForward\": " + String(distance_forward_cm) + ","
    + "\"DistanceLeft\": " + String(distance_left_cm) + ","
    + "\"Command\": \"" + command + "\","
    + "\"targetDist\": " + String(target_dist, 2) + ","
    + "\"targetYaw\": " + String(target_yaw, 2) + "}");
}

void handleTuning(AsyncWebServerRequest *request)
{
  if (!request->authenticate(www_username, www_password))
    return request->requestAuthentication();

  uint8_t id = (uint8_t)(request->argName((size_t)0)).toInt();
  if (id < 12)
  {
    myBala.setParaK(id-1, request->arg((size_t)0).toFloat());
    myFlash.updateEEPROM(myBala);
  }
  else if (id == 12) safe_distance_cm = (uint16_t)(request->arg((size_t)0).toFloat());
  else if (id == 13) backward_time = (uint16_t)(request->arg((size_t)0).toFloat());
  else if (id == 14) turnleft_time = (uint16_t)(request->arg((size_t)0).toFloat());
  
  request->redirect("/");
}

void handleMotion(AsyncWebServerRequest *request)
{
  if (!request->authenticate(www_username, www_password))
    return request->requestAuthentication();

  if (request->argName((size_t)0) == "Motion")
  {
    if (request->arg((size_t)0) == "F")       myBala.move(1);
    else if (request->arg((size_t)0) == "B")  myBala.move(2);
    else if (request->arg((size_t)0) == "L")  myBala.turn(1);
    else if (request->arg((size_t)0) == "R")  myBala.turn(2);
  }
  else if (request->argName((size_t)0) == "Stop")
  {
    if (request->arg((size_t)0) == "F")       myBala.move(0);
    else if (request->arg((size_t)0) == "B")  myBala.move(0);
    else if (request->arg((size_t)0) == "L")  myBala.turn(0);
    else if (request->arg((size_t)0) == "R")  myBala.turn(0);    
  }
  else if (request->argName((size_t)0) == "MCD")
  {
    target_dist = request->arg((size_t)0).toFloat() * 100;
    move_dist_en = 1;
  }
  else if (request->argName((size_t)0) == "RCA")
  {
    target_yaw = request->arg((size_t)0).toFloat();
    rotate_yaw_en = 1;
  }

  Serial.println(request->arg((size_t)0));

  request->redirect("/");    
}

void handleMode(AsyncWebServerRequest *request) 
{
  if (!request->authenticate(www_username, www_password))
    return request->requestAuthentication();
   
  if (request->argName((size_t)0) == "Avoidance") 
  {
    avoidance_en = (String(request->arg((size_t)0)) == String("on"));
    if (!avoidance_en) 
    {
      myBala.stop();
      myFlash.initEEPROM(myBala);  // if quit avoidance mode, reset original parameters (especially 'movement_step')
    }
  }
  else if (request->argName((size_t)0) == "Track")
  {
    raspberry_en = (String(request->arg((size_t)0)) == String("on"));
    if (raspberry_en)
    {
      MySerial.write(0x80); MySerial.write(0x01); MySerial.write(0x81); MySerial.write(0x81);
      Serial.println("Raspberry On!");
    }
    else
    {
      MySerial.write(0x80); MySerial.write(0x02); MySerial.write(0x81); MySerial.write(0x81);
      Serial.println("Raspberry Off!");
    }
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
  WiFi.mode(WIFI_AP_STA);
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

  Serial.print("Open http://");
  Serial.print(WiFi.softAPIP());
  Serial.println(" in your browser to see it working.");

  vTaskDelete(NULL);
}
