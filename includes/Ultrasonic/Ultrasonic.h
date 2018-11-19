#ifndef _ULTRASONIC_H_
#define _ULTRASONIC_H_

#define MAX_SENSOR_DISTANCE 500  // Maximum sensor distance can be up to 500cm.
#define US_ROUNDTRIP_CM 59      // Microseconds (uS) it takes sound to travel round-trip 1cm (2cm total).
// Conversion from uS to distance (round result to nearest cm).
#define NewPingConvert(echoTime) (max((echoTime + US_ROUNDTRIP_CM / 2) / US_ROUNDTRIP_CM, (echoTime ? 1 : 0)))

#include <Arduino.h>

class Ultrasonic
{
	uint8_t pin_trig;
	uint8_t pin_echo;
	uint16_t max_timeout_us;
public:
	Ultrasonic(uint8_t trigger_pin, uint8_t echo_pin, uint16_t max_distance_cm = MAX_SENSOR_DISTANCE);
	void begin();
	uint32_t ping();
	uint16_t ping_cm();
};

#endif // _ULTRASONIC_H_