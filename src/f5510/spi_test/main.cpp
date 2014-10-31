#include "main.h"

// Initialize the clock
clock clk = clock(F_25MHz);
// Initialize the SPI object
spi test_spi = spi(SPI_B1);

int main (void)
{
  uint8_t i = 0;
  for (;;) //ever
  {
    // Spew 0x0-0xFF for reading on a logic analyzer -- use a delay to make it
    // easier to look at in the analyzer
    for (i=0; i<0xFF; i++)
    {
      test_spi.tx(i);
      clk.delayMS(10);
    }
    clk.delayMS(100);
  }
}
