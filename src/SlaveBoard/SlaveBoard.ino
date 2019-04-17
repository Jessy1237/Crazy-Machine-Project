#include "LedExtended.h"
#include "Music.h"

//LED Matrix Pins
#define DATA_PIN A0
#define CS_PIN  A1
#define CLK_PIN A2
#define NBR_MTX 5 //number of matrices attached is one

//State pins
#define STATE_PIN0 A3
#define STATE_PIN1 A4
#define STATE_PIN2 A5

#define WELCOME_MSG "Welcome to super mario world!"
#define PRESS_START_MSG "Press START to Play"

LedControlExtended lc = LedControlExtended(DATA_PIN, CLK_PIN, CS_PIN, NBR_MTX);
char* displayString = NULL;

int prevState = -1;
long numCycles = 0;
int numCoins = 0;
int numLifes = 2;

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

  //Initialize the SdCard.
  setupMusic();
  displayString = PRESS_START_MSG;

  //Setup pins to read state changes
  pinMode(STATE_PIN0, INPUT);
  pinMode(STATE_PIN1, INPUT);
  pinMode(STATE_PIN2, INPUT);
}

void loop()
{
  int state = digitalRead(STATE_PIN2) * 4 + digitalRead(STATE_PIN1) * 2 + digitalRead(STATE_PIN0);

  switch (state)
  {
    case 1:
      if (prevState != state) //Here we do a reset to new state 1 i.e. things to run once per state change
      {
        //stopMusic();
        displayString = WELCOME_MSG;
        //playMusic(HERE_WE_GO);
        numCycles = 0;
      }

      if (numCycles == 1000)
      {
        //playMusic(WARP_PIPE);
      }
      else if (numCycles == 2000)
      {
        //playMusic(UNDERGROUND);
      }

      lc.writeScrollingString(0, NBR_MTX, displayString, 69);
      break;
    case 2:
      if (prevState != state) //Here we do a reset to new state 1 i.e. things to run once per state change
      {
        //stopMusic();
        //playMusic(SUPER_STAR);
        numCycles = 0;
      }

      if (numCycles == 1000)
      {
        //stopMusic();
        //playMusic(ENEMY_DEATH);
        numCoins++;
      }
      else if (numCycles == 1250)
      {
        //stopMusic();
        //playMusic(SUPER_STAR);
      }

      //lc.displaySuperStar();
      //lc.displayPlayerStats(numLifes, numCoins);
      break;
    case 3:
      if (prevState != state) //Here we do a reset to new state 1 i.e. things to run once per state change
      {
        //stopMusic();
        //playMusic(ENEMY_DEATH);
        numCoins++;
        numCycles = 0;
      }

      if(numCycles == 250)
      {
        //stopMusic();
        //playMusic(UNDERGROUND);
      }

      //lc.displayPlayerStats(numLifes, numCoins);
      break;
    case 4:

      break;
    case 5:

      break;
    case 6:

      break;
    case 7:

      break;
    default:
      if (prevState != state) //Here we do a reset to new state 0 i.e. things to run once per state change
      {
        displayString = PRESS_START_MSG;
        //stopMusic()
        numCycles = 0;
        numCoins = 0;
        numLifes = 2;
      }

      //Here we write things that need to happen every cycle in a state
      lc.writeScrollingString(0, NBR_MTX, displayString, 69);
      break;
  }

  numCycles++;
  prevState = state;
}