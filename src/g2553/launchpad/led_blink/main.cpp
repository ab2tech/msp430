#include "main.h"

#define LED_PIN p1_0

int main(void)
{
  basic_clock clock = basic_clock();

  pinOutput(LED_PIN);
  for (;;) //ever
  {
    pinOn(LED_PIN);
    _delay_s(1);
    pinOff(LED_PIN);
    _delay_s(1);
  }
}
