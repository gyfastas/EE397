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
      myBala.stop();
      myBala.dist_en(1);
      myBala.move(direction + 1);
      while (abs(myBala.getDistance()) < abs(target)) 
      {
        target_dist = target - myBala.getDistance();
        vTaskDelay(15);
      }
      target_dist = 0;
      myBala.stop();
      myBala.dist_en(0);
      move_dist_en = 0;
    }
    vTaskDelay(1);
  }
  vTaskDelete(NULL);  
}

/*************************************************************************/
// Task5 :                Rotate Certain Yaw Event
/*************************************************************************/

void rotateCertainYaw(void *parameter)
{
  while (1)
  {
    static double target;
    static uint8_t direction;
    if (rotate_yaw_en)
    {
      target = target_yaw;
      direction = (target < 0);   // 0-left, 1-right
      myBala.turn(0);
      myBala.yaw_en(1);
      myBala.turn(direction + 1);
      while (abs(myBala.getYaw()) < abs(target)) 
      {
        target_yaw = target - myBala.getYaw();
        vTaskDelay(5);
      }
      target_yaw = 0;
      myBala.turn(0);
      myBala.yaw_en(0);
      rotate_yaw_en = 0;
    }
    vTaskDelay(1);
  }
  vTaskDelete(NULL);  
}