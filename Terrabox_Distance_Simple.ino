#include "Arduino.h"
#include "A02_UART_Auto.h"


A02_UART_Auto sensor;

void setup()
{
  sensor.begin();
}

// The loop function is called in an endless loop
void loop()
{
  sensor.exec();
}
