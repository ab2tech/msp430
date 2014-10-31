#include "main.h"

#define BLINK_PIN p5_3

// Initialize the clock to F_CPU
clock clk = clock();

// blink function declaration
void blink(msp_pin_t pin);
int main (void)
{
  // Blink the blink pin
  blink(BLINK_PIN);
}

void blink(msp_pin_t pin)
{
  // Enable the blink pin as an output
  pinOutput(pin);
  // Now blink,
  for (;;) //ever
  {
    pinToggle(pin);
    clk.delayMS(1000);
  }
}
