//#include "LowPower.h"

void setup() {
  
  for(int i = 0; i < 20; i++) {
    pinMode(i, OUTPUT);
  }
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  //LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);  
  SMCR |= (1<<2);
  SMCR |= 1;
  __asm__ __volatile__("sleep");
}

