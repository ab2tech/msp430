#include "main.h"

#define LED_PIN p5_3
#define SW_PIN  p1_4

// Initialize the clock
clock clk = clock();

void toggleLED()
{
  pinToggle(LED_PIN);
}

int main (void)
{
  // Set up the switch
  int_sw sw = int_sw(SW_PIN, &toggleLED);
  // Configure the LED_PIN as an output
  pinOutput(LED_PIN);
  pinOn(LED_PIN);
  // Already done by clock library, but do it again anyway
  _EINT();
  // Enter low power mode 3
  LPM3;
}
