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
    static uint8_t state_forward = 0;
    static uint8_t state_left = 0;
    static uint8_t mode = 0;
    static uint16_t pathIndex = 0;
    if (maze_solver_en || maze_opt_switch)
    {
      // Judge the mode
      state_forward = distance_forward_cm > forward_distance_threshold_cm;
      state_left = distance_left_cm > left_distance_threshold_cm;
      MSmode = mode = (state_left) ? 1 : ((state_forward) ? 0 : 2); // 0-forward, 1-left, 2-right

      // First pass
      if (!maze_opt_switch)
      {
        pathIndex = 0;
        switch (mode)
        {
        case 0: // go straight
          myBala.move(1);
        break;
        case 1: // turn left
          if (distance_forward_cm > forward_distance_threshold_cm)
            goCertainDist(maze_solver_en, buff_dist);  // move forward some distance to get enough space to turn 
          myBala.stop(); 
          if (distance_left_cm > left_distance_threshold_cm)  // check the mode again
          {
            turnCertainAngle(maze_solver_en, target_yaw_left);
            myBala.move(1);             // move forward until touching the wall on the left again
            while (maze_solver_en && distance_left_cm > left_distance_threshold_cm) vTaskDelay(1); 
            path += "L";
          }
          simplifyPath();
          mode = 0;                                                    
        break;
        case 2: // turn right 
          if (distance_forward_cm > forward_distance_threshold_cm)
            goCertainDist(maze_solver_en, buff_dist);  // move forward some distance to get enough space to turn 
          myBala.stop();
          if (distance_left_cm > left_distance_threshold_cm)    // check the left wall
          {
            turnCertainAngle(maze_solver_en, target_yaw_left);
            myBala.move(1);             // move forward until touching the wall on the left again
            while (maze_solver_en && distance_left_cm > left_distance_threshold_cm) vTaskDelay(1); 
            path += "L";
          }
          else
          {
            turnCertainAngle(maze_solver_en, target_yaw_right);
            if (distance_forward_cm < forward_distance_threshold_cm)  // if the right side is also block (twice), turn right again
            {
              turnCertainAngle(maze_solver_en, target_yaw_right);
              if (distance_forward_cm < forward_distance_threshold_cm)  // if the right side is once again block (trice), reach maze end
              {
                myBala.stop();
                // trap this task until the host turn off maze solver or turn on maze optimization
                while (maze_solver_en && !maze_opt_switch) vTaskDelay(1);  
              }
              else
              {
                if(path.endsWith("B"))
                {
                  path += "F";
                  simplifyPath();
                }
                path += "B";
              }
            }
            else
            {
              if (path.endsWith("R")) path[path.length()-1] = 'B';
              else path += "R";
            }
          }
          simplifyPath();
          mode = 0; 
        break;
        default: break;
        }
      }
      else // Second pass: optimization
      {
        switch (mode)
        {
        case 0: // go straight
          myBala.move(1);
        break;
        case 1: case 2:
          myBala.stop();
          if (pathIndex < path.length())
          {
            switch (path[pathIndex++])
            {
            case 'F':
              myBala.move(1);             // move forward until touching the wall on the left again
              while (maze_opt_switch && distance_left_cm > left_distance_threshold_cm) vTaskDelay(1); 
            break;
            case 'B':
              target_yaw = 2 * target_yaw_right;
              rotate_yaw_en = 1;
              while (maze_opt_switch && rotate_yaw_en) vTaskDelay(1); // wait for turning to finish
              myBala.move(1);             // move forward until touching the wall on the left again
              while (maze_opt_switch && distance_left_cm > left_distance_threshold_cm) vTaskDelay(1);             
            break;
            case 'L':
              target_dist = buff_dist;    // move forward some distance to get enough space to turn
              move_dist_en = 1;
              while (maze_opt_switch && move_dist_en) vTaskDelay(1); // wait for moving to finish
              target_yaw = target_yaw_left;
              rotate_yaw_en = 1;
              while (maze_opt_switch && rotate_yaw_en) vTaskDelay(1); // wait for turning to finish
              myBala.move(1);             // move forward until touching the wall on the left again
              while (maze_opt_switch && distance_left_cm > left_distance_threshold_cm) vTaskDelay(1);           
            break;
            case 'R':
              target_dist = buff_dist;    // move forward some distance to get enough space to turn
              move_dist_en = 1;
              while (maze_opt_switch && move_dist_en) vTaskDelay(1); // wait for moving to finish
              target_yaw = target_yaw_right;
              rotate_yaw_en = 1;
              while (maze_opt_switch && rotate_yaw_en) vTaskDelay(1); // wait for turning to finish   
              myBala.move(1);             // move forward until touching the wall on the left again
              while (maze_opt_switch && distance_left_cm > left_distance_threshold_cm) vTaskDelay(1);    
            break;
            default: myBala.stop(); break;
            }
          }
        break;
        default: break;
        }
      }
    }
    else
      MSmode = mode = pathIndex = 0;
    vTaskDelay(1); 
  }
  vTaskDelete(NULL);  
}

void goCertainDist(uint8_t &flag, uint16_t target_cm)
{
  target_dist = target_cm;                      // set target distance
  move_dist_en = 1;                             // enable move-certain-distance event
  while (flag && move_dist_en) vTaskDelay(1);   // wait for moving to finish  
}

void turnCertainAngle(uint8_t &flag, double target_degree)
{
  target_yaw = target_degree;                  // set target angle
  rotate_yaw_en = 1;                           // enable turn-certain-yaw event
  while (flag && rotate_yaw_en) vTaskDelay(1); // wait for turning to finish   
}

void simplifyPath()
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
  case   0: path[pathLength - 3] = 'F'; break;
  case  90: path[pathLength - 3] = 'R'; break;
  case 180: path[pathLength - 3] = 'B'; break;
  case 270: path[pathLength - 3] = 'L'; break;
  default: break;
  }

  // The path is now two steps shorter.
  path.remove(pathLength - 2, 2);
} 
