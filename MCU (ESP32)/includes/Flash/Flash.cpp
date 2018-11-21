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

#include "Flash.h"

uint8_t Flash::initEEPROM(Bala &b)
{
  Serial.println("Start EEPROM...");
  if (!EEPROM.begin(2 + EEPROM_SIZE * sizeof(double)))
  {
    Serial.println("Failed to initialise EEPROM");
    return 1;
  }
  if (byte(EEPROM.read(0)) == EEPROM_FLAG && byte(EEPROM.read(1)) == EEPROM_FLAG)
  {
    Serial.println("Read PID parameters from EEPROM ...");
    double val;
    uint8_t addr = 2;
    for (uint8_t i = 0; i < EEPROM_SIZE; ++i)
    {
      addr = EEPROM_read(addr, val);
      b.setParaK(i, val);
    }      
  }
  else
  {
    Serial.println("First time to use EEPROM ... ");
    EEPROM.write(0, EEPROM_FLAG); EEPROM.write(1, EEPROM_FLAG);
    uint8_t addr = 2;
    for (uint8_t i = 0; i < EEPROM_SIZE; ++i)
      addr = EEPROM_write(addr, b.getParaK(i));   
    EEPROM.commit();
  }
  return 0;
}

void Flash::updateEEPROM(Bala &b)
{
  Serial.println("Update EEPROM ... ");
  uint8_t addr = 2;
  for (uint8_t i = 0; i < EEPROM_SIZE; ++i)
    addr = EEPROM_write(addr, b.getParaK(i));    
  EEPROM.commit();
}