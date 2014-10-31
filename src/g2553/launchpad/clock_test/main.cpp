#include "main.h"

#define LED_PIN  p1_0

int main(void)
{
  // Initialize the clock
  clock();

  // Make the LED_PIN an output
  pinOutput(LED_PIN);

  // Enable SMCLK pin output
  //clock::clk2PinEnable(CLK_PIN_ACLK);
  clock::clk2PinEnable(CLK_PIN_SMCLK);

  // Loop
  for (;;)//ever
  {
    // Blink the LED (5Hz)
    pinOn(LED_PIN);
    clock::delayMS(100);
    pinOff(LED_PIN);
    clock::delayMS(100);
  }
}
