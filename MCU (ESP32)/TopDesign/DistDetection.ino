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
// Task2-a :                Ultrasonic Distance Detection
/*************************************************************************/

void distForwardDetection(void *parameter)
{
  // Initialize ...
  mySonic1.begin();
  delay(500);

  while(1)
  {
    static uint32_t detect_interval = millis() + 10;
    if (millis() > detect_interval) 
    {
      detect_interval = millis() + 10;
      distance_forward_cm = mySonic1.ping_cm();
    }
    vTaskDelay(1); 
  }
  vTaskDelete(NULL);  
}

/*************************************************************************/
// Task2-b :                Ultrasonic Distance Detection
/*************************************************************************/

void distLeftDetection(void *parameter)
{
  // Initialize ...
  mySonic2.begin();
  delay(500);

  while(1)
  {
    static uint32_t detect_interval = millis() + 10;
    if (millis() > detect_interval) 
    {
      detect_interval = millis() + 10;
      distance_left_cm = mySonic2.ping_cm();
    }
    vTaskDelay(1); 
  }
  vTaskDelete(NULL);  
}
