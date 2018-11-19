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