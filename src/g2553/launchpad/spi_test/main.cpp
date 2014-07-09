#include "main.h"

#define LED_PIN p1_0
#define LED_PIN2 p1_6

int main(void)
{
  basic_clock clock = basic_clock();
  spi spi_test = spi(SPI_B0);

  pinOutput(LED_PIN);
  pinOutput(LED_PIN2);
  pinOn(LED_PIN2);

  for (;;) //ever
  {
    for (uint8_t i=0; i<255; i++)
    {
      spi_test.write(i);
    }
    pinToggle(LED_PIN);
    pinToggle(LED_PIN2);
    _delay_s(1);
  }
}
