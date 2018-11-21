/* Copyright (C) 2018 gyfastas. All rights reserved.

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
	for (uint8_t i = 0; i < server.args(); i++)
		message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
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

