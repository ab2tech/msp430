#include "main.h"

// Initialize the clock
clock clk = clock(F_4MHz);
tlc5925 tlc = tlc5925(&clk, p5_2, p5_3, 50);

int main (void)
{
  for (;;) //ever
  {
    tlc.flash(10, CH0_15);
  }
}
