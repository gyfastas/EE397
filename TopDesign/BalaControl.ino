/*************************************************************************/
// Task0 :                Self-balance Control
/*************************************************************************/

void balaControl(void *parameter)
{
  // Initialize bala ...
  myBala.begin();
  delay(500);

  // Update PID parameters ...
  if (myFlash.initEEPROM(myBala))
  {
    Serial.println("Restart ...");
    ESP.restart();
  }

  while(1)
  {
    static uint32_t control_interval = millis() + 5;
    if (millis() > control_interval) 
    {
      control_interval = millis() + 5;
      myBala.run();
    }  
    vTaskDelay(10); 
  }
  vTaskDelete(NULL);  
}

