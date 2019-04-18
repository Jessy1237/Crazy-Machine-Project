#include "LedExtended.h"
#include "Music.h"

//LED Matrix Pins
#define DATA_PIN A0
#define CS_PIN  A1
#define CLK_PIN A2
#define NBR_MTX 4 //number of matrices attached is one
#define LED_DELAY 69

//State pins
#define STATE_PIN0 A3
#define STATE_PIN1 A4
#define STATE_PIN2 A5

#define WELCOME_MSG "Welcome to super mario world!"
#define PRESS_START_MSG "Press START to Play"
#define CONGRATS_MSG "congratulations"

LedControlExtended lc = LedControlExtended(DATA_PIN, CLK_PIN, CS_PIN, NBR_MTX);
char* displayString = NULL;

short prevState = -1;
long numCycles = 0;
short numCoins = 0;
short numLifes = 2;

void neutralState(short state, short prevState);
void aboveGroundState(short state, short prevState);
void superStarState(short state, short prevState);
void elevatorToFunnelState(short state, short prevState);
void funnelState(short state, short prevState);
void finalElevatorState(short state, short prevState);
void finishedState(short state, short prevState);

void setup()
{
  for (int i = 0; i < NBR_MTX; i++)
  {
    lc.shutdown(i, false);

    /* Set the brightness to a medium values */
    lc.setIntensity(i, 8);

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
  short state = digitalRead(STATE_PIN2) * 4 + digitalRead(STATE_PIN1) * 2 + digitalRead(STATE_PIN0);

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

void neutralState(short state, short prevState)
{
  if (prevState != state) //Here we do a reset to new state 0 i.e. things to run once per state change
  {
    displayString = PRESS_START_MSG;
    stopMusic();
    numCycles = 2000;
    numCoins = 0;
    numLifes = 2;
    resetTrackPositions();
    playMusic(PRESS_START_TO_PLAY);
  }

  if(numCycles == 2000)
  {
    playMusic(PRESS_START_TO_PLAY);
    numCycles = 0; //Set to 0 to loop the sound effect
  }

  //Here we write things that need to happen every cycle in a state
  lc.writeScrollingString(0, NBR_MTX, displayString, LED_DELAY);
}

void aboveToUndergroundState(short state, short prevState)
{
  if (prevState != state)
  {
    stopMusic();
    displayString = WELCOME_MSG;
    playMusic(HERE_WE_GO);
    numCycles = 0;
  }

  if (numCycles == 1000)
  {
    playMusic(WARP_PIPE);
  }
  else if (numCycles == 2000)
  {
    playMusic(UNDERGROUND);
  }

  lc.writeScrollingString(0, NBR_MTX, displayString, LED_DELAY);
}

void superStarState(short state, short prevState)
{
  if (prevState != state)
  {
    stopMusic();
    playMusic(SUPER_STAR);
    numCycles = 0;
  }

  if (numCycles == 1000)
  {
    stopMusic();
    playMusic(ENEMY_DEATH);
    numCoins++;
  }
  else if (numCycles == 1250)
  {
    stopMusic();
    playMusic(SUPER_STAR);
  }

  lc.writeScrollingStars(0, NBR_MTX, LED_DELAY * 1.5);
  //lc.displayPlayerStats(numLifes, numCoins);
}

void elevatorToFunnelState(short state, short prevState)
{
  if (prevState != state) //Here we do a reset to new state 1 i.e. things to run once per state change
  {
    stopMusic();
    playMusic(ENEMY_DEATH);
    numCoins++;
    numCycles = 0;
  }

  if (numCycles == 250)
  {
    stopMusic();
    playMusic(UNDERGROUND);
  }

  //lc.displayPlayerStats(numLifes, numCoins);
}

void funnelState(short state, short prevState)
{
  if (prevState != state) //Here we do a reset to new state 1 i.e. things to run once per state change
  {
    stopMusic();
    numCycles = 0;
  }

  if (numCycles == 1000)
  {
    playMusic(WAAH);
  }
  else if (numCycles == 4000)
  {
    stopMusic();
    playMusic(OOF);
  }
}

void finalElevatorState(short state, short prevState)
{
  if (prevState != state) //Here we do a reset to new state 1 i.e. things to run once per state change
  {
    stopMusic();
    playMusic(SO_LONG_BOWSER);
    numCycles = 0;
    displayString = CONGRATS_MSG;
  }

  if (numCycles >= 1000)
  {
    lc.writeScrollingString(0, NBR_MTX, displayString, LED_DELAY);
  }

  if (numCycles == 2000) //Possibly combine this music file with the so long bowser
  {
    stopMusic();
    playMusic(OUTRO);
  }
}

void finishedState(short state, short prevState)
{
  if (prevState != state) //Here we do a reset to new state 1 i.e. things to run once per state change
  {
    stopMusic();
    playMusic(THANKS_FOR_PLAYING);
    numCycles = 0;
    displayString = CONGRATS_MSG;
  }

  lc.writeScrollingString(0, NBR_MTX, displayString, LED_DELAY);
}
