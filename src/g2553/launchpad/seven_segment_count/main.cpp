#include "main.h"

#define LED_PIN  p1_0

int main(void)
{
  clock(DCO_F_16MHz);
  seven_seg ss = seven_seg(p1_4);
  bool colon = false;

  pinOutput(LED_PIN);

  ss.write((uint16_t)TLC5925_CH0_15);
  clock::delayS(2);
  pinToggle(LED_PIN);

  for (;;) //ever
  {
    for (uint8_t i=0;i<100;i++)
    {
      ss.num(i);
      pinToggle(LED_PIN);
      clock::delayMS(100);
    }
    ss.num(0xFF, 16);
    pinToggle(LED_PIN);
    clock::delayMS(100);
    ss.blank(true, true);
    ss.num(0);
    pinToggle(LED_PIN);
    clock::delayMS(100);
    ss.blank(false, false);
    ss.num(0xFF, 16);
    pinToggle(LED_PIN);
    clock::delayMS(200);
    ss.blank(true, true);
    ss.num(0);
    pinToggle(LED_PIN);
    clock::delayS(1);
    ss.blank(false, false);
    for (uint8_t i=0;i<0xFF;i++)
    {
      ss.num(i, 16);
      pinToggle(LED_PIN);
      clock::delayMS(100);
    }
    colon = !colon;
    ss.dp(colon, colon);
  }
}
