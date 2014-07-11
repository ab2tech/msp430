#include "main.h"

int main(void)
{
  clock(DCO_F_16MHz);

  easyset tlc59731 = easyset(p1_0, 1);

  for (;;) //ever
  {
    for (uint16_t i=0; i<255; i++)
    {
      tlc59731.data(ES_OUT_0, 0, i);
      tlc59731.update();
    }
    for (uint16_t i=0; i<255; i++)
    {
      tlc59731.data(ES_OUT_1, 0, i);
      tlc59731.update();
    }
    for (uint16_t i=0; i<255; i++)
    {
      tlc59731.data(ES_OUT_2, 0, i);
      tlc59731.update();
    }
  }
}
