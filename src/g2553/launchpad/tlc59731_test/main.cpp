#include "main.h"

int main(void)
{
  clock(DCO_F_16MHz);

  easyset tlc59731 = easyset(p1_0, 3);

  for (;;) //ever
  {
    for (uint16_t i=0; i<255; i++)
    {
      tlc59731.rgbData(0, i, 0, 0);
      tlc59731.rgbData(1, i, 0, 0);
      tlc59731.rgbData(2, i, 0, 0);
      tlc59731.update();
      clock::delayMS(20);
    }
    //clock::delayS(1);
    for (uint16_t i=0; i<255; i++)
    {
      tlc59731.rgbData(0, 0, i, 0);
      tlc59731.rgbData(1, 0, i, 0);
      tlc59731.rgbData(2, 0, i, 0);
      tlc59731.update();
      clock::delayMS(20);
    }
    //clock::delayS(1);
    for (uint16_t i=0; i<255; i++)
    {
      tlc59731.rgbData(0, 0, 0, i);
      tlc59731.rgbData(1, 0, 0, i);
      tlc59731.rgbData(2, 0, 0, i);
      tlc59731.update();
      clock::delayMS(20);
    }
    for (uint16_t i=0; i<255; i++)
	{
	  tlc59731.rgbData(0, i, 0, 0);
	  tlc59731.rgbData(1, 0, i, 0);
	  tlc59731.rgbData(2, 0, 0, i);
	  tlc59731.update();
	  clock::delayMS(20);
	}
	//clock::delayS(1);
	for (uint16_t i=0; i<255; i++)
	{
	  tlc59731.rgbData(0, 0, i, 0);
	  tlc59731.rgbData(1, 0, 0, i);
	  tlc59731.rgbData(2, i, 0, 0);
	  tlc59731.update();
	  clock::delayMS(20);
	}
	//clock::delayS(1);
	for (uint16_t i=0; i<255; i++)
	{
	  tlc59731.rgbData(0, 0, 0, i);
	  tlc59731.rgbData(1, i, 0, 0);
	  tlc59731.rgbData(2, 0, i, 0);
	  tlc59731.update();
	  clock::delayMS(20);
	}
	for (uint16_t i=0; i<255; i++)
	{
	  tlc59731.rgbData(0, i, 0, i);
	  tlc59731.rgbData(1, i, 0, 0);
	  tlc59731.rgbData(2, 0, i, 0);
	  tlc59731.update();
	  clock::delayMS(20);
	}
    //clock::delayS(1);
  }
}
