#include "main.h"

// Initialize the clock
clock clk = clock(F_1MHz);

tlc5925 tlc_red = tlc5925(&clk, p4_0);
tlc5925 tlc_green = tlc5925(&clk, p4_2);
tlc5925 tlc_blue = tlc5925(&clk, p4_6);

int main (void)
{
  uint8_t loop = 1;
  uint8_t i;
  for(loop;loop>0;loop--)
  {
    tlc_red.channelScanUp(1);
    tlc_red.channelScanDown(1);

    tlc_red.shiftUp(16,BOLD_CURSOR);
    tlc_red.shiftDown(15,BOLD_CURSOR>>1);

    tlc_red.shiftUp(13,BOTTOM_LEFT_QUADRANT);
    tlc_red.shiftDown(13,BOTTOM_RIGHT_QUADRANT);

    tlc_red.shiftUp(9,LEFT_HALF);
    tlc_red.shiftDown(9,RIGHT_HALF);

    i = 4;
    for(i;i>0;i--)
    {
      tlc_red.flash(1,CHAAAA);
      tlc_red.flash(1,CH5555);
    }

    i = 4;
    for(i;i>0;i--)
    {
      tlc_red.flash(1,CRASH_DUMMY_LEFT);
      clk.delayMS(400);
      tlc_red.flash(1,CRASH_DUMMY_RIGHT);
      clk.delayMS(400);
    }

    i = 16;
    for(i;i>0;i--)
    {
      tlc_red.write(0xAAAA);
      clk.delayMS(10);
      tlc_red.write(0x5555);
      clk.delayMS(10);
    }

    tlc_red.flash(4,CH0_15);
    tlc_red.outputDisable();
  }
}

// Port 1 Interrupt Service Routine
#pragma vector = PORT1_VECTOR
__interrupt void PORT_1_ISR(void)
{
  if(ROT_ENC_WIPER_IFG & ROT_ENC_WIPER_A_BIT)
  {
    if(ROT_ENC_WIPER_IN & ROT_ENC_WIPER_B_BIT)
    {
      // B is High -> CCW
      encoder_position = (encoder_position>>1)|(encoder_position<<15);
      drv8825_direction_counter_clockwise();
    }
    else
    {
      // B is Low -> CW
      encoder_position = (encoder_position<<1)|(encoder_position>>15);
      drv8825_direction_clockwise();
    }
    tlc5925_write(encoder_position);
    ROT_ENC_WIPER_IFG &= ~ROT_ENC_WIPER_A_BIT;
  }
  else if(ROT_ENC_SWITCH_IFG & ROT_ENC_SWITCH_BIT)
  {
    microstepping_mode = get_drv8825_microstepping_value();
    if((microstepping_mode << 1) > 32)
      microstepping_mode = 1;
    else
      microstepping_mode = microstepping_mode << 1;
    drv8825_set_microstepping(microstepping_mode);
    ROT_ENC_SWITCH_IFG &= ~ROT_ENC_SWITCH_BIT;
  }
}

/*
  pinOutput(p5_2);
  pinOutput(p5_3);
  pinOff(p5_2);
  pinOff(p5_3);
  serial.risingEdge();
  serial.write(0xFF);
  serial.write(0xFF);
  pinOn(p5_3);
  pinOff(p5_3);
  pinOn(p5_2);
  pinOff(p5_2);
  //tlc.channelScanUp(1);
*/
/*
  for(;;)
  {
    tlc_red.channelScanUp(1);
    //tlc_red.flash(10,CH07);
    //tlc_red.flash(10,CH04);
    //tlc_red.flash(10,CH06);
    //tlc_red.flash(10,CH08);
  }
*/
