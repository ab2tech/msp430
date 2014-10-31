#include "main.h"

// Initialize the clock
clock clk = clock();

int main (void)
{
  // Initialize the TLC5925 devices
  tlc5925 tlc_red = tlc5925(p4_0);
  tlc5925 tlc_green = tlc5925(p4_2);
  tlc5925 tlc_blue = tlc5925(p4_6);
  // Start with CH00
  uint16_t ch = TLC5925_CH00;
  for (;;) //ever
  {
    // Iterate through the channels by RRC
    ch = rrc(ch);
    tlc_red.write(ch);
    tlc_green.write(ch);
    tlc_blue.write(ch);
    // 100ms delay between each change for demonstration purposes
    clk.delayMS(100);
  }
}
