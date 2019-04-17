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
#define CONGRATS_MSG "congratulations"

LedControlExtended lc = LedControlExtended(DATA_PIN, CLK_PIN, CS_PIN, NBR_MTX);
char* displayString = NULL;

int prevState = -1;
long numCycles = 0;
int numCoins = 0;
int numLifes = 2;

void neutralState(int state, int prevState);
void aboveGroundState(int state, int prevState);
void superStarState(int state, int prevState);
void elevatorToFunnelState(int state, int prevState);
void funnelState(int state, int prevState);
void finalElevatorState(int state, int prevState);
void finishedState(int state, int prevState);

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
      aboveToUndergroundState(state, prevState);
      break;
    case 2:
      superStarState(state, prevState);
      break;
    case 3:
      elevatorToFunnelState(state, prevState);
      break;
    case 4:
      funnelState(state, prevState);
      break;
    case 5:
      finalElevatorState(state, prevState);
      break;
    case 6:
      finishedState(state, prevState);
      break;
    case 7:
      //Do NOTHING as there is no state 7. Could possibly just make it go to neutral state.
      break;
    default:
      neutralState(state, prevState);
      break;
  }

  numCycles++;
  prevState = state;
}

void neutralState(int state, int prevState)
{
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
}

void aboveToUndergroundState(int state, int prevState)
{
  if (prevState != state)
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
}

void superStarState(int state, int prevState)
{
  if (prevState != state)
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
}

void elevatorToFunnelState(int state, int prevState)
{
  if (prevState != state) //Here we do a reset to new state 1 i.e. things to run once per state change
  {
    //stopMusic();
    //playMusic(ENEMY_DEATH);
    numCoins++;
    numCycles = 0;
  }

  if (numCycles == 250)
  {
    //stopMusic();
    //playMusic(UNDERGROUND);
  }

  //lc.displayPlayerStats(numLifes, numCoins);
}

void funnelState(int state, int prevState)
{
  if (prevState != state) //Here we do a reset to new state 1 i.e. things to run once per state change
  {
    //stopMusic();
    numCycles = 0;
  }

  if(numCycles == 1000)
  {
    //playMusic(WAAH);
  }
  else if(numCycles == 4000)
  {
    //stopMusic();
    //playMusic(OOF);
  }
}

void finalElevatorState(int state, int prevState)
{
  if (prevState != state) //Here we do a reset to new state 1 i.e. things to run once per state change
  {
    //stopMusic();
    //playMusic(SO_LONG_BOWSER);
    numCycles = 0;
    displayString = CONGRATS_MSG;
  }

  if(numCycles >= 1000)
  {
    lc.writeScrollingString(0, NBR_MTX, displayString, 69);
  }

  if(numCycles == 2000) //Possibly combine this music file with the so long bowser
  {
    //stopMusic();
    //playMusic(OUTRO);
  }
}

void finishedState(int state, int prevState)
{
  if (prevState != state) //Here we do a reset to new state 1 i.e. things to run once per state change
  {
    //stopMusic();
    //playMusic(THANKS_FOR_PLAYING);
    numCycles = 0;
  }
}
