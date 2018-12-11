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
// Task7 :                  Maze Solver
/*************************************************************************/

void mazeSolver(void *parameter)
{
  while(1)
  { 
    static String path = "";  // buffer to store all actions: L, R, F, B
    static uint8_t state_forward = 0;
    static uint8_t state_left = 0;
    static uint8_t mode = 0;
    static uint8_t turn_state = 0;
    static uint8_t buff_before_turn_state = 0;
    static uint8_t buff_after_turn_state = 0;
    if (maze_solver_en)
    {
      // Judge the mode
      state_forward = distance_forward_cm > forward_distance_threshold_cm;
      state_left = distance_left_cm > left_distance_threshold_cm;
      if (turn_state != 1) // only change mode when not in turn
        mode = (state_left) ? 1 : ((state_forward) ? 0 : 2); // 0-forward, 1-left, 2-right

      // First pass
      switch (mode)
      {
      case 0: // go straight
        myBala.move(1);
        turn_state = 0;
      break;
      case 1: // turn left
        switch (turn_state)
        {
        case 0:
          switch (buff_before_turn_state)
          {
          case 0:
            target_dist = buff_dist;    // move forward some distance to get enough space to turn
            move_dist_en = 1;
            buff_before_turn_state = 1;                 
          break;
          case 1:
            if (!move_dist_en)
            {
              buff_before_turn_state = 0; 
              target_yaw = target_yaw_left; // start to turn
              rotate_yaw_en = 1;
              turn_state = 1;
            }
          break;
          default: break;
          }
        break;
        case 1:
          if (!rotate_yaw_en)
          {   // right after turning
            if (state_forward)
            {
              // move forward until touching the wall on the left again
              myBala.move(1);
              if (state_left == 0)
              {
                turn_state = 0;
                mode = 0;                        
                buff_after_turn_state = 0;                               
              }
            }
            else
            {
              turn_state = 0;
              mode = 0;
            }
          }
        break;
        default: break;
        }
      break;
      case 2: // turn right
        switch (turn_state)
        {
        case 0:
          switch (buff_before_turn_state)
          {
          case 0:
            target_dist = buff_dist;    // move forward some distance to get enough space to turn
            move_dist_en = 1;
            buff_before_turn_state = 1;                 
          break;
          case 1:
            if (!move_dist_en)
            {
              buff_before_turn_state = 0; 
              target_yaw = target_yaw_right; // start to turn
              rotate_yaw_en = 1;
              turn_state = 1;
            }
          break;
          default: break;
          }
        break;
        case 1:
          if (!rotate_yaw_en)
          {
            turn_state = 0;
            mode = 0;
          }
        break;
        default: break;
        }
      break;
      default: break;
      }

      // Second pass: optimization
      // TODO
    }
    else
    {
      mode = 0;
      turn_state = 0;
      buff_before_turn_state = 0;
      buff_after_turn_state = 0;
    }
    vTaskDelay(1); 
  }
  vTaskDelete(NULL);  
}

void actIntersection(String &path, char direction)
{
  path += direction;
  simplifyPath(path);
}

void simplifyPath(String &path)
{
  uint16_t pathLength = path.length();

  // only simplify the path if the second-to-last turn was a 'B'
  if (pathLength < 3 || path[pathLength-2] != 'B')
    return;

  uint16_t totalAngle = 0;
  for (uint8_t i = 1; i <= 3; i++)
  {
    switch (path[pathLength-i])
    {
    case 'R': totalAngle += 90; break;
    case 'L': totalAngle += 270; break;
    case 'B': totalAngle += 180; break;
    default: break;
    }
  }

  // Get the angle as a number between 0 and 360 degrees.
  totalAngle = totalAngle % 360;

  // Replace all of those turns with a single one.
  switch(totalAngle)
  {
  case   0: path[pathLength - 3] = 'S'; break;
  case  90: path[pathLength - 3] = 'R'; break;
  case 180: path[pathLength - 3] = 'B'; break;
  case 270: path[pathLength - 3] = 'L'; break;
  default: break;
  }

  // The path is now two steps shorter.
  path[pathLength - 2] = 0;
} 
