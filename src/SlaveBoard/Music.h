#ifndef SdInclude
#define SdInclude 1
#include <SPI.h>

//Add the SdFat Libraries
#include <SdFat.h>
#include <SdFatUtil.h>

//and the MP3 Shield Library
#include <SFEMP3Shield.h>
#endif

// Below is not needed if interrupt driven. Safe to remove if not using.
#if defined(USE_MP3_REFILL_MEANS) && USE_MP3_REFILL_MEANS == USE_MP3_Timer1
#include <TimerOne.h>
#elif defined(USE_MP3_REFILL_MEANS) && USE_MP3_REFILL_MEANS == USE_MP3_SimpleTimer
#include <SimpleTimer.h>
#endif

#define NBR_TRACKS 10
#define NBR_PAUSABLE_TRACKS 2

#define HERE_WE_GO 1
#define WARP_PIPE 2
#define UNDERGROUND 3
#define SUPER_STAR 4
#define ENEMY_DEATH 5
#define WAAH 6
#define OOF 7
#define SO_LONG_BOWSER 8
#define OUTRO 9
#define THANKS_FOR_PLAYING 10

/**
   \brief Setup the Arduino Chip's feature for our use.

   After Arduino's kernel has booted initialize basic features for this
   application, such as Serial port and MP3player objects with .begin.
   Along with displaying the Help Menu.

*/
void setupMusic();

void stopMusic();

void playMusic(int track);

char* getTrackTitle();

void resetTrackPositions();
