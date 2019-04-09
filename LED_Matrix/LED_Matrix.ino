
#include "LedExtended.h"

#define DATA_PIN 2
#define CS_PIN  3
#define CLK_PIN 4
#define NBR_MTX 1 //number of matrices attached is one

LedControlExtended lc = LedControlExtended(DATA_PIN, CLK_PIN, CS_PIN, NBR_MTX);

void setup()
{
  for (int i = 0; i < NBR_MTX; i++)
  {
    lc.shutdown(i, false);

    /* Set the brightness to a medium values */
    lc.setIntensity(i, 5);

    /* and clear the display */
    lc.clearDisplay(i);
    delay(100);
  }
}



void loop()
{
  lc.writeScrollingStringOld(0, "Welcome to super mario world!", 69); //sending characters to display
  lc.clearAll();//clearing the display
  delay(1000);
}
