
#define TRIG 25
#define ECHO 39

double temp, cm;

void UltraSound(void * parameter)
{
	while(1)
	{
		digitalWrite(TRIG, LOW); 
		delayMicroseconds(2);
		digitalWrite(TRIG, HIGH);
		delayMicroseconds(10);
		digitalWrite(TRIG, LOW);

		temp = double(pulseIn(ECHO, HIGH));

		cm = (temp * 17)/1000;
		vTaskDelay(10);
	}
	vTaskDelete(NULL); 
}

void setup()
{
	Serial.begin(115200);
	Serial.println("Begin");
	pinMode(TRIG, OUTPUT);
	pinMode(ECHO, INPUT);

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
	Serial.print("Echo =");
	Serial.print(temp);
	Serial.print(" | | Distance = ");
	Serial.print(cm);
	Serial.println("cm");
	delay(100);
}