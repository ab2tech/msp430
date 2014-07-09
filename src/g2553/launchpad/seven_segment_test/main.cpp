#include "main.h"

#define LED_PIN  p1_0
#define LED_PIN2 p1_6

int main(void)
{
  basic_clock clock = basic_clock(DCO_F_1MHz);
  tlc5925 tlc = tlc5925(p1_4);

  pinOutput(LED_PIN);
  pinOutput(LED_PIN2);
  pinOn(LED_PIN2);

  tlc.write((uint16_t)TLC5925_CH01);
  _delay_s(2);
  tlc.write((uint16_t)TLC5925_CH0_15);
  _delay_s(5);

  for (;;) //ever
  {
    tlc.shiftDown(16,TLC5925_CH00);
    pinToggle(LED_PIN);
    pinToggle(LED_PIN2);
    _delay_s(1);
  }
}
