#include "main.h"

// Initialize the clock
clock clk = clock(F_25MHz);
spi   test_spi = spi(SPI_B1);

int main (void)
{
  uint8_t i = 0;
  for (;;) //ever
  {
    for (i=0; i<0xFF; i++)
    {
      test_spi.write(i);
      clk.delayMS(10);
    }
    clk.delayMS(100);
  }
}
