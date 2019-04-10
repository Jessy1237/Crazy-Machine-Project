
#include "LedExtended.h"

#define DATA_PIN 2
#define CS_PIN  3
#define CLK_PIN 4
#define NBR_MTX 1 //number of matrices attached is one

const char* WELCOME_MSG = "Welcome to super mario world!";
const char* ANOTHER_MSG = "another string";

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
  /*if (i < 200)
    {
    displayString = WELCOME_MSG;
    }
    else if ( i < 320)
    {
    displayString = ANOTHER_MSG;
    }
    else
    {
    i = 0;
    }*/
  if (lc.writeScrollingString(0, displayString, 69)) //sending characters to display
  {
    if (displayString == WELCOME_MSG) //above statement returns true once the string has finished scrolling so time to switch displays
    {
      displayString = ANOTHER_MSG;
    }
    else
    {
        displayString = WELCOME_MSG;
    }
  }
  //i++;
}
