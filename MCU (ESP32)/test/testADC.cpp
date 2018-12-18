#define BATTERY_VOLTAGE_TEST 36

void setup() 
{
   Serial.begin(115200);
   adcAttachPin(BATTERY_VOLTAGE_TEST);
   adcStart(BATTERY_VOLTAGE_TEST);
   analogReadResolution(10);       // 10-bits resolution, maximum raw value is 1023
   analogSetAttenuation(ADC_6db);  // at 6dB attenuation, the maximum voltage is 2.2V
}

void loop() {
  static uint8_t Voltage_Count;
  static uint32_t Voltage_Sum;
  double battery_voltage;
  
  // Battery voltage sample
  Voltage_Count++;                                   // counter for average
  Voltage_Sum += analogRead(BATTERY_VOLTAGE_TEST);   // sample battery voltage and integrate
  if(Voltage_Count == 200)                           // calculate average
  {
    Voltage_Sum /= 200; 
    // voltage = sample / 1024 * VRef * 11, where VRef is 2.2V(ideal) at 6dB attenuation, in real, we set VRef = 1.75V
    battery_voltage = Voltage_Sum * 0.0187988;
    Voltage_Sum = 0;
    Voltage_Count = 0;
    Serial.println(String("Read: ") + String(battery_voltage));
  }
}
