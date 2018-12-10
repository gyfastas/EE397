/* Copyright (C) 2018 Charles. All rights reserved.

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
// Task3 :               Communication with Raspberry
/*************************************************************************/

void raspberryControl(void *parameter)
{
  MySerial.begin(115200, SERIAL_8N1, /*RX*/HWRX, HWTX);

  static uint8_t idx;
  static char cmd[16];

  while(1)
  {
    if (MySerial.available())
    {
      if (!raspberry_en)
      {
        while (MySerial.available()) MySerial.read();
        continue;
      }
      if (MySerial.read() == 0x80)
      {
        idx = 0;
        while (1)
        {
          if (MySerial.available())
            cmd[idx++] = MySerial.read();
          else break;
          if (cmd[idx-1] == 0x81)
          {
            if (MySerial.available() && MySerial.read() == 0x81)
            {
              cmd[idx-1] = 0x00;
              command = String(cmd);
              if (command == "F")       myBala.move(1);
              else if (command == "B")  myBala.move(2);
              else if (command == "L")  myBala.turn(1);
              else if (command == "R")  myBala.turn(2);
              else if (command == "S")  myBala.stop();
              else if (command == "E")  myBala.stop(); // error 
              Serial.println(command);
              break;
            }
            else break;
          }
        }
      }
    }
    else
      command = "N";
    vTaskDelay(1); 
  }
  vTaskDelete(NULL);  
}

