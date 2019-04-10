
#include "LedExtended.h"

#define DATA_PIN 2
#define CS_PIN  3
#define CLK_PIN 4
#define NBR_MTX 1 //number of matrices attached is one

LedControlExtended lc = LedControlExtended(DATA_PIN, CLK_PIN, CS_PIN, NBR_MTX);
char* displayString = NULL;
int stringLen = 0;
int i = 0;

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

  //A loop to display the switching between strings whilst polling for state changes from main arduino
  if (i < 200)
  {
    displayString = "Welcome to super mario world!";
  }
  else if ( i < 320)
  {
    displayString = "Another String";
  }
  else
  {
    i = 0;
  }
  lc.writeScrollingString(0, displayString, 69); //sending characters to display
  i++;
}
