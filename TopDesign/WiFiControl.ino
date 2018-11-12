/*************************************************************************/
// Task1 :                WiFi Remote Control
/*************************************************************************/

void showParaK(uint8_t K)
{  
  String info = "";
  switch(K)
  {
    case 0 : info += "\r\nKp_B: ";        info += myBala.getParaK(0);   break;
    case 1 : info += "\r\nKd_B: ";        info += myBala.getParaK(1);   break;
    case 2 : info += "\r\nKp_V: ";        info += myBala.getParaK(2);   break;
    case 3 : info += "\r\nKi_V: ";        info += myBala.getParaK(3);   break;
    case 4 : info += "\r\nKd_V: ";        info += myBala.getParaK(4);   break;
    case 5 : info += "\r\nKp_T: ";        info += myBala.getParaK(5);   break;
    case 6 : info += "\r\nKi_T: ";        info += myBala.getParaK(6);   break;
    case 7 : info += "\r\nKd_T: ";        info += myBala.getParaK(7);   break;
    case 8 : info += "\r\nSDK: ";         info += myBala.getParaK(8);   break;
    case 9 : info += "\r\nTargetAngle: "; info += myBala.getParaK(9);   break;
    case 10: info += "\r\nVecPeriod: ";   info += myBala.getParaK(10);  break;
    case 11: info += "\r\nCarDown: ";     info += myBala.getParaK(11);  break;
    default:                              break; 
  }
  client.println(info.c_str());
}

void WiFiConfig()
{
  static uint8_t connection_timeout_cnt = 0;
  for (uint8_t idx = 0; idx < NUMSSID; ++idx)
  {
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid[idx]);

    WiFi.begin(ssid[idx].c_str(), pswd[idx].c_str());

    connection_timeout_cnt = 0;

    while (WiFi.status() != WL_CONNECTED) 
    {
      delay(500);
      if (++connection_timeout_cnt >= 4)
        break;
      Serial.print(".");
    }

    if (WiFi.status() == WL_CONNECTED)
      break;
  }

  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println();
    Serial.println("WiFi connection failed.");
    return;
  }

  Serial.println();
  Serial.println("WiFi connected");

  Serial.print("Connecting to remote TCP server ");
  Serial.println(host);

  if (!client.connect(host, hostPort)) 
  {
    Serial.println("TCP Server connection failed");
    return;
  }

  // Send username and password to login
  client.print(TCPCert);
}

void remoteControl(void *parameter)
{
  // Connect to WiFi route and the remote TCP server
  WiFiConfig();
  while(1)
  {
  // Get command from remote controller via WiFi
  if (client.available())
  {
    String cmd = client.readStringUntil('\r');
    cmd.trim();
    Serial.print("\nGet command: ");
    Serial.println(cmd);
    if (cmd.substring(0,1) == String("#"))
    { 
      if (cmd.length() < 2)
      {
        String info = "Angle: ";     info += myBala.getRoll();
        info += "\r\nKp_B: ";        info += myBala.getParaK(0);
        info += "\r\nKd_B: ";        info += myBala.getParaK(1);
        info += "\r\nKp_V: ";        info += myBala.getParaK(2);
        info += "\r\nKi_V: ";        info += myBala.getParaK(3);
        info += "\r\nKd_V: ";        info += myBala.getParaK(4);
        info += "\r\nKp_T: ";        info += myBala.getParaK(5);
        info += "\r\nKi_T: ";        info += myBala.getParaK(6);
        info += "\r\nKd_T: ";        info += myBala.getParaK(7);
        info += "\r\nSDK: ";         info += myBala.getParaK(8);
        info += "\r\nTargetAngle: "; info += myBala.getParaK(9);
        info += "\r\nVecPeriod: ";   info += myBala.getParaK(10);
        info += "\r\nCarDown: ";     info += myBala.getParaK(11);
        info += "\r\nSpeedL: ";      info += myBala.getSpeedL();
        info += "\r\nSpeedR: ";      info += myBala.getSpeedR();
        client.println(info.c_str());
        for (uint8_t i = 0; i < EEPROM_SIZE; ++i)
          Serial.println(myBala.getParaK(i));
      }
      else
      {
        showParaK(cmd.substring(1).toInt()-1);
      }
    }
    
    else
    {
    uint8_t ty = cmd.indexOf('#') - 1;
    if (ty == 0)
      myBala.setParaK((uint8_t)(cmd.substring(0,1).toInt() - 1), cmd.substring(1).toFloat());
    else if (ty == 1)
      myBala.setParaK((uint8_t)(cmd.substring(0,2).toInt() - 1), cmd.substring(2).toFloat());
    myFlash.updateEEPROM(myBala);    
    }
  } 
  vTaskDelay(10);
  }
  vTaskDelete(NULL);  
}
