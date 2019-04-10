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

/**
   \brief Setup the Arduino Chip's feature for our use.

   After Arduino's kernel has booted initialize basic features for this
   application, such as Serial port and MP3player objects with .begin.
   Along with displaying the Help Menu.

*/
void setupMusic();

/**
   \brief Decode the Menu.

   Parses through the characters of the users input, executing corresponding
   MP3player library functions and features then displaying a brief menu and
   prompting for next input command.
*/

void parse_menu(byte key_command);

/**
   \brief Print Help Menu.

   Prints a full menu of the commands available along with descriptions.
*/
void help();
