#include "LedExtended.h"
#include "Music.h"

#define DATA_PIN A0
#define CS_PIN  A1
#define CLK_PIN A2
#define NBR_MTX 5 //number of matrices attached is one

#define WELCOME_MSG "Welcome to super mario world!"

LedControlExtended lc = LedControlExtended(DATA_PIN, CLK_PIN, CS_PIN, NBR_MTX);
char* displayString = NULL;

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

  Serial.begin(115200);

  //Initialize the SdCard.
  setupMusic();
  displayString = WELCOME_MSG;
}

void loop()
{
  if (Serial.available()) {
    // parse_menu(digitalRead(PIN_NO)); // get input from attached input at PIN_NO
    parse_menu(Serial.read()); // get command from serial input
    char title[30];
    MP3player.trackTitle((char*)&title);
    displayString = title;
  }
  lc.writeScrollingString(0, NBR_MTX, displayString, 69);

}
