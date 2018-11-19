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
	return (uint16_t)(NewPingConvert(this->ping()));
}
