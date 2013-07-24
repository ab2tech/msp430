#include "main.h"

#define BLINK_PIN p5_3

// Initialize the clock to F_CPU
clock clk = clock();

// blink function declaration
void blink(msp_pin_t pin);
int main (void)
{
  clk_ret_t status;

  pinOutput(pJ_0);

  //status = clk.cfgCLK(CLK_ACLK, CLK_SEL_DCO, CLK_DIV_1);
  status = clk.clk2PinSetAclkDivider(CLK_DIV_8);
  status = clk.clk2PinEnable(CLK_PIN_ACLK);
  // check status as appropriate
//  clk.delayMS(2000);
//  //clk.cfgSysFreq(F_1MHz);

//  for (;;)
//  {
//    pinToggle(pJ_0);
//    clk.delayMS(50);
//  }

  // Blink the blink pin
  blink(BLINK_PIN);
}

void blink(msp_pin_t pin)
{
  // Enable the blink pin as an output
  pinOutput(pin);
  // Now blink,
  for (;;) //ever
  {
    pinToggle(pin);
    clk.delayMS(1000);
  }
}
