#include "Music.h"

/**
   \brief Setup the Arduino Chip's feature for our use.

   After Arduino's kernel has booted initialize basic features for this
   application, such as Serial port and MP3player objects with .begin.
   Along with displaying the Help Menu.

*/

/**
   \brief Object instancing the SdFat library.

   principal object for handling all SdCard functions.
*/
static SdFat sd;

/**
   \brief Object instancing the SFEMP3Shield library.

   principal object for handling all the attributes, members and functions for the library.
*/
static SFEMP3Shield MP3player;

static uint8_t result;

void setupMusic()
{

  // Below is only needed if not interrupt driven. Safe to remove if not using.
#if defined(USE_MP3_REFILL_MEANS) \
    && ( (USE_MP3_REFILL_MEANS == USE_MP3_SimpleTimer) \
    ||   (USE_MP3_REFILL_MEANS == USE_MP3_Polled)      )

  MP3player.available();
#endif

  if (!sd.begin(SD_SEL, SPI_FULL_SPEED)) sd.initErrorHalt();
  // depending upon your SdCard environment, SPI_HAVE_SPEED may work better.
  if (!sd.chdir("/")) sd.errorHalt("sd.chdir");

  //Initialize the MP3 Player Shield
  result = MP3player.begin();

#if defined(__BIOFEEDBACK_MEGA__) // or other reasons, of your choosing.
  // Typically not used by most shields, hence commented out.
  Serial.println(F("Applying ADMixer patch."));
  if (MP3player.ADMixerLoad("admxster.053") == 0) {
    Serial.println(F("Setting ADMixer Volume."));
    MP3player.ADMixerVol(-3);
  }
#endif

  Serial.println(F("Arduino Music Selector:"));
  help();
}

/**
   \brief Decode the Menu.

   Parses through the characters of the users input, executing corresponding
   MP3player library functions and features then displaying a brief menu and
   prompting for next input command.
*/

void parse_menu(byte key_command)
{
  char title[30];

  Serial.print(F("Received command: "));
  Serial.write(key_command);
  Serial.println(F(" "));

  //if s, stop the current track
  if (key_command == 's') {
    Serial.println(F("Stopping"));
    MP3player.stopTrack();

    //if 1-9, play corresponding track
  } else if (key_command >= '1' && key_command <= '9') {
    //convert ascii numbers to real numbers
    key_command = key_command - 48;

    result = MP3player.playTrack(key_command);

    //check result, see readme for error codes.
    if (result != 0) {
      Serial.print(F("Error code: "));
      Serial.print(result);
      Serial.println(F(" when trying to play track"));
    } else {
      Serial.println(F("Playing:"));

      //we can get track info by using the following functions and arguments
      //the functions will extract the requested information, and put it in the array we pass in
      MP3player.trackTitle((char*)&title);
      Serial.write((byte*)&title, 30);
      Serial.println();
    }

    //if +/- to change volume
  } else if ((key_command == '-') || (key_command == '+')) {
    union twobyte mp3_vol; // create key_command existing variable that can be both word and double byte of left and right.
    mp3_vol.word = MP3player.getVolume(); // returns a double uint8_t of Left and Right packed into int16_t

    if (key_command == '-') { // note dB is negative
      // assume equal balance and use byte[1] for math
      if (mp3_vol.byte[1] >= 254) { // range check
        mp3_vol.byte[1] = 254;
      } else {
        mp3_vol.byte[1] += 2; // keep it simpler with whole dB's
      }
    } else {
      if (mp3_vol.byte[1] <= 2) { // range check
        mp3_vol.byte[1] = 2;
      } else {
        mp3_vol.byte[1] -= 2;
      }
    }
    // push byte[1] into both left and right assuming equal balance.
    MP3player.setVolume(mp3_vol.byte[1], mp3_vol.byte[1]); // commit new volume
    Serial.print(F("Volume changed to -"));
    Serial.print(mp3_vol.byte[1] >> 1, 1);
    Serial.println(F("[dB]"));

    // if p, pause track
  } else if (key_command == 'p') {
    if ( MP3player.getState() == playback) {
      MP3player.pauseMusic();
      Serial.println(F("Pausing"));
    } else {
      Serial.println(F("Not Playing!"));
    }

    // if r, resume track
  } else if (key_command == 'r') {
    if ( MP3player.getState() == paused_playback) {
      MP3player.resumeMusic();
      Serial.println(F("Resuming"));
    } else {
      Serial.println(F("No Track to Resume!"));
    }

  } else if (key_command == 'g') {
    int32_t offset_ms = 20000; // Note this is just an example, try your own number.
    Serial.print(F("jumping to "));
    Serial.print(offset_ms, DEC);
    Serial.println(F("[milliseconds]"));
    result = MP3player.skipTo(offset_ms);

  } else if (key_command == 'k') {
    int32_t offset_ms = -1000; // Note this is just an example, try your own number.
    Serial.print(F("moving = "));
    Serial.print(offset_ms, DEC);
    Serial.println(F("[milliseconds]"));
    result = MP3player.skip(offset_ms);

    // if h, print help
  } else if (key_command == 'h') {
    help();

    // if any other command, pring error
  } else {
    Serial.println(F("ERROR: Invalid Command."));

  }
  // print prompt after key stroke has been processed.
  Serial.print(F("Enter: 1-9, s, p, r, +, -, g, k, h\n"));
}

/**
   \brief Print Help Menu.

   Prints a full menu of the commands available along with descriptions.
*/
void help()
{
  Serial.println(F("COMMANDS:"));
  Serial.println(F(" [1-9] to play a track"));
  Serial.println(F(" [s] to stop playing"));
  Serial.println(F(" [p] to pause"));
  Serial.println(F(" [r] to resume"));
  Serial.println(F(" [+ or -] to change volume"));
  Serial.println(F(" [g] Skip to a predetermined offset of ms in current track."));
  Serial.println(F(" [k] Skip a predetermined number of ms in current track."));
  Serial.println(F(" [h] to see commands"));
}
