#include "main.h"

#define LED_PIN  p1_0

int main(void)
{
  // Initialize the clock
  clock(DCO_F_16MHz);
  // Initialize the seven segment instance
  seven_seg ss = seven_seg(p1_4);
  // Variable for tracking the colon status
  bool colon = false;

  // Make the LED_PIN an output
  pinOutput(LED_PIN);

  // Turn on all the digits initially
  ss.write((uint16_t)TLC5925_CH0_15);
  // Delay for 2 seconds to demonstrate this
  clock::delayS(2);
  // Toggle the LED
  pinToggle(LED_PIN);

  for (;;) //ever
  {
    // Count from 0 to 99 at 10Hz in decimal
    for (uint8_t i=0;i<100;i++)
    {
      ss.num(i);
      pinToggle(LED_PIN);
      clock::delayMS(100);
    }
    // Set the seven segment to display 0xFF in hex
    ss.num(0xFF, 16);
    pinToggle(LED_PIN);
    clock::delayMS(100);
    // Blank the seven segment
    ss.blank(true, true);
    ss.num(0);
    pinToggle(LED_PIN);
    clock::delayMS(100);
    // Unblank the seven segment
    ss.blank(false, false);
    // Set to 0xFF hex
    ss.num(0xFF, 16);
    pinToggle(LED_PIN);
    clock::delayMS(200);
    // Blank the seven segment
    ss.blank(true, true);
    ss.num(0);
    pinToggle(LED_PIN);
    clock::delayS(1);
    // Unblank the seven segment
    ss.blank(false, false);
    // Count from 0 to 0xFF at 10Hz in hex
    for (uint8_t i=0;i<0xFF;i++)
    {
      ss.num(i, 16);
      pinToggle(LED_PIN);
      clock::delayMS(100);
    }
    // Toggle the colon
    colon = !colon;
    // Set the DPs to represent the colon
    ss.dp(colon, colon);
  }
}
