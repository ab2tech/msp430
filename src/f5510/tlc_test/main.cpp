#include "main.h"

// Initialize the clock
clock clk = clock();



int main (void)
{
  tlc5925 tlc_red = tlc5925(&clk, p4_0);
  tlc5925 tlc_green = tlc5925(&clk, p4_2);
  tlc5925 tlc_blue = tlc5925(&clk, p4_6);
  uint16_t ch = TLC5925_CH00;
  for (;;) //ever
  {
    ch = rrc(ch);
    tlc_red.write(ch);
    tlc_green.write(ch);
    tlc_blue.write(ch);
    clk.delayMS(100);
  }
}
