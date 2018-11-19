#include <Ultrasonic.h>

#define TRIG 25
#define ECHO 39
#define MAX_DISTANCE 200        // Maximum sensor distance is set as 200cm.

Ultrasonic sonic(TRIG, ECHO, MAX_DISTANCE);

void UltraSound(void * parameter)
{
	static uint16_t dist_cm;
	while(1)
	{
	    static uint32_t ranging_interval = millis() + 100;
		if (millis() > ranging_interval) 
		{
			ranging_interval = millis() + 100;
			dist_cm = sonic.ping_cm();
			Serial.println("Distance = " + dist_cm + " cm.");
		}  
		vTaskDelay(10); 
	}
	vTaskDelete(NULL);  
}

void setup()
{
	Serial.begin(115200);
	Serial.println("Begin");
	sonic.begin();

	xTaskCreatePinnedToCore(
		UltraSound,           /* Task function. */
		"UltraSound",         /* String with name of task. */
		10000,                /* Stack size in words. */
		NULL,                 /* Parameter passed as input of the task */
		2,                    /* Priority of the task. */
		NULL,                 /* Task handle. */
		0);                   /* Run on core 0. */
}

void loop()
{

}