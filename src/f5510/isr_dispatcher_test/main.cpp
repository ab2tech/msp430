#include "main.h"

#define LED_PIN p5_3

// Initialize the clock
clock clk = clock();

isr_t toggleLED(void *ptr)
{
  pinToggle(LED_PIN);
}

int main (void)
{
  msp_timerA_t timer = ta1_0;
  // Make the LED PIN and output
  pinOutput(LED_PIN);
  // Install the TA1_0 ISR to toggle the LED
  isr_d::installISR(isr_d::taVector(ta1_0), (void *) 0, &toggleLED);
  // Configure timerA for up mode and assign ACLK as its source
  set(tactl(timer), (TASSEL__ACLK | MC__UP | TACLR));
  // Enable the timer interrupt
  on (tacctl(timer), CCIE);
  // Set the CCR at 32768 to equal a second
  set(taccr(timer), 32768);

  // Enable interrupts and enter LPM3
  _EINT();
  LPM3;
}
