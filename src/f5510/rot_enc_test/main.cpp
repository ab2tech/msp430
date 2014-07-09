#include "main.h"

#define LED_PIN p5_3
#define MIN_ENC_DIVS 16
#define MAX_ENC_DIVS 112
#define NUM_TLC_LEDs 16

// Button callback function declaration
void doBtn(void);
// Encoder callback function declaration
void doEncoder(rot_enc_int *enc);
//void doEncoder(rot_enc_poll *enc);

// Initialize the clock to F_CPU
clock clk = clock();
tlc5925 tlc_red = tlc5925(p4_0);
tlc5925 tlc_green = tlc5925(p4_2);
tlc5925 tlc_blue = tlc5925(p4_6);

rot_enc_int enc = rot_enc_int(p1_0, p1_1, &doEncoder);
//rot_enc_poll enc = rot_enc_poll(p1_0, p1_1, &doEncoder);

int_sw red_sw = int_sw(p1_6, &doBtn);

uint16_t divisor = (ROT_ENC_DIVISIONS / NUM_TLC_LEDs);

int main (void)
{
  pinOutput(LED_PIN);

  tlc_red.write((uint16_t)(1<<((uint16_t)enc.getVal()/divisor)));
  tlc_green.write(divisor);
  tlc_blue.write((uint16_t)ROT_ENC_DIVISIONS);

  LPM0;
}

void doBtn(void)
{
  int8_t divs = enc.getDivs();
  if (divs < MAX_ENC_DIVS)
    divs += MIN_ENC_DIVS;
  else
    divs = MIN_ENC_DIVS;

  enc.setDivs(divs);
  tlc_blue.write((uint16_t)divs);

  divisor = (divs / NUM_TLC_LEDs);
  tlc_green.write(divisor);
  enc.clearVal();
  tlc_red.write((uint16_t)1);
}

void doEncoder(rot_enc_int *enc)
//void doEncoder(rot_enc_poll *enc)
{
  tlc_red.write((uint16_t)(1<<((uint16_t)enc->getVal()/divisor)));
}
