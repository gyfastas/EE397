#ifndef _FLASH_H_
#define _FLASH_H_

#include <Arduino.h>
#include <EEPROM.h>
#include "Bala.h"

class Flash
{
private:
    uint8_t EEPROM_SIZE;
	uint8_t EEPROM_FLAG;
	template <class T> uint8_t EEPROM_write(uint8_t addr, const T& value)
	{
		const byte* p = (const byte*)(const void*)&value;
		for (uint8_t i = 0; i < sizeof(value); i++)
			EEPROM.write(addr++, *p++);
		return addr;
	}

	template <class T> uint8_t EEPROM_read(uint8_t addr, T& value)
	{
		byte* p = (byte*)(void*)&value;
		for (uint8_t i = 0; i < sizeof(value); i++)
			*p++ = EEPROM.read(addr++);
		return addr;
	}

public:
	Flash(uint8_t size, uint8_t flag = 0x01): EEPROM_SIZE(size), EEPROM_FLAG(flag) {}
	uint8_t initEEPROM(Bala &b);
	void updateEEPROM(Bala &b);
};

#endif // _FLASH_H_