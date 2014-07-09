#include "main.h"

void main (void)
{
  // Declare a counter variable
  uint8_t spi_cnt;
  // Stop watchdog timer
  WDTCTL = WDTPW | WDTHOLD;
  // Initialize the clock to 16MHz
  clockInitDefaults();

  spiInit(SPI_B0);

  for (;;) //ever
  {
    for (spi_cnt = 0; spi_cnt < 255; spi_cnt++)
      spiWrite(SPI_B0, spi_cnt);
  }
}

