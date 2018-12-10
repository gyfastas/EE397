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
// Task6 :               Avoidance Control
/*************************************************************************/

void avoidanceControl(void *parameter)
{
  while(1)
  { 
    // finite state machine
    static uint32_t state_timer = millis();
    static uint8_t state = 0;
    // Avoidance Mode
    if (avoidance_en)
    {
      myBala.setParaK(8, 50); // in avoidance mode, run at a slow speed
      switch (state)
      {
      case 0:  // state 0 : move forward (if no obstacle)
        myBala.move(1);      // move forward
        if (distance_forward_cm < safe_distance_cm)
        {
          myBala.move(2);    // move backward to avoid hitting the obstacle
          state_timer = millis();
          state = 1;
        }
        break;
      case 1:  // state 1 : move backward (to avoid hitting the obstacle)
        if (millis() - state_timer > backward_time)
        {
          myBala.move(0);
          myBala.turn(1);    // turn left to bypass the obstacle
          state_timer = millis();
          state = 2;
        }
        break;
      case 2:  // state 2 : turn left (to bypass the obstacle)
        if (millis() - state_timer > turnleft_time)
        {
          myBala.turn(0);
          state = 0;          
        }
        break;
      default: break;
      }
    }  
    vTaskDelay(1); 
  }
  vTaskDelete(NULL);  
}