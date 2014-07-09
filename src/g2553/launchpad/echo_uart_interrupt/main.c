#include "main.h"

void main (void)
{
  // Stop watchdog timer
  WDTCTL = WDTPW | WDTHOLD;
  // Initialize the clock to 16MHz
  clockInitDefaults();
  uartInitDefaults();

  // Make p1_0 an output so we can flash our LED every time we interrupt :)
  pinOutput(p1_0);
  pinOff(p1_0);

  // Block until we get a character from UART
  uartGetc();
  // Print a welcome message
  uartPuts("Welcome to the AB2 UART Echo Example - Any Character to Begin");
  // Block until another character gets us started
  uartGetc();
  uartNewLine();

  // Enter low power mode 3
  _BIS_SR(LPM3_bits + GIE);
}

// One interrupt vector for both A&B USCI on G2553
#pragma vector=USCIAB0RX_VECTOR
__interrupt void UART_RX_ISR(void)
{
  char c = UCA0RXBUF;
  if (c == '\r')
    uartNewLine();
  else
    uartPutc(c);

  pinPulseDuration(p1_0, 1000);
}
