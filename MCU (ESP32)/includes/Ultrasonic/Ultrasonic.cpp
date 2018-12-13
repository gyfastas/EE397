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

#include "Ultrasonic.h"

Ultrasonic::Ultrasonic(uint8_t trigger_pin, uint8_t echo_pin, uint16_t max_distance_cm)
	: pin_trig(trigger_pin), pin_echo(echo_pin) 
{
	max_timeout_us = min(max_distance_cm, MAX_SENSOR_DISTANCE) * US_ROUNDTRIP_CM + (US_ROUNDTRIP_CM / 2);	
}

void Ultrasonic::begin()
{
	pinMode(pin_trig, OUTPUT);
	pinMode(pin_echo, INPUT);
}

uint32_t Ultrasonic::ping()
{
	// Triger
	digitalWrite(this->pin_trig, LOW); 
	delayMicroseconds(4);
	digitalWrite(this->pin_trig, HIGH);
	delayMicroseconds(10);
	digitalWrite(this->pin_trig, LOW);

	// Wait for echo
	return (uint32_t)(pulseIn(this->pin_echo, HIGH, this->max_timeout_us));
}
uint16_t Ultrasonic::ping_cm()
{
    uint32_t t = this->ping();
	return (t == 0) ? (uint16_t)(MAX_SENSOR_DISTANCE) : (uint16_t)(NewPingConvert(t));
}
