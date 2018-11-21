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