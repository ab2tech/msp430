#include "main.h"

#define LED_PIN  p1_0
#define LED_PIN2 p1_6

int main(void)
{
  clock(DCO_F_16MHz);

  pinOutput(LED_PIN);

  //clock::clk2PinEnable(CLK_PIN_ACLK);
  clock::clk2PinEnable(CLK_PIN_SMCLK);

  for (;;)
  {
    pinOn(LED_PIN);
    clock::delayMS(10);
    pinOff(LED_PIN);
    clock::delayMS(10);
  }
}
