#include "main.h"

void main (void)
{
  char c;
  // Stop watchdog timer
  WDTCTL = WDTPW | WDTHOLD;
  // Initialize the clock to 16MHz
  clockInitDefaults();
  uartInitDefaults();

  for(;;)
  {
    // Block waiting for a character and echo it once it arrives
    while (c = uartGetc())
    {
      if (c == '\r')
        uartNewLine();
      else
        uartPutc(c);
    }
  }
}
