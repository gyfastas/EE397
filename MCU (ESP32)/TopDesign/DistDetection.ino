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
// Task2 :                Ultrasonic Distance Detection
/*************************************************************************/

void distDetection(void *parameter)
{
  // Initialize ...
  mySonic.begin();
  delay(500);

  while(1)
  {
    static uint32_t detect_interval = millis() + 20;
    if (millis() > detect_interval) 
    {
      detect_interval = millis() + 20;
      distance_cm = mySonic.ping_cm();
    }
    
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
        if (distance_cm < safe_distance_cm)
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
        // if (distance_cm > safe_distance_cm)    // if no obstacle, return to state 0
        // {
        //   myBala.turn(0);
        //   state = 0;
        // }
        if (millis() - state_timer > turnleft_time)
        {
          myBala.turn(0);
          state = 0;          
        }
        break;
      default: break;
      }
    }  
    vTaskDelay(10); 
  }
  vTaskDelete(NULL);  
}


/*************************************************************************/
// Task4 :                Move Certain Distance Event
/*************************************************************************/

void moveCertainDist(void *parameter)
{
  while (1)
  {
    static double target;
    static uint8_t direction;
    if (move_dist_en)
    {
      target = target_dist;
      direction = (target < 0);   // 0-forward, 1-backward
      myBala.dist(1);
      myBala.move(direction + 1);
      while (abs(myBala.getDistance()) < abs(target)) 
      {
        target_dist = target - myBala.getDistance();
        vTaskDelay(15);
      }
      target_dist = 0;
      myBala.dist(0);
      move_dist_en = 0;
    }
    vTaskDelay(1);
  }
  vTaskDelete(NULL);  
}