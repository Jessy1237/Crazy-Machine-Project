/**
 * \file MP3Shield_Library_Demo.ino
 *
 * \brief Example sketch of using the MP3Shield Arduino driver, demonstrating all methods and functions.
 * \remarks comments are implemented with Doxygen Markdown format
 *
 * \author Bill Porter
 * \author Michael P. Flaga
 *
 * This sketch listens for commands from a serial terminal (like the Serial
 * Monitor in the Arduino IDE). If it sees 1-9 it will try to play an MP3 file
 * named track00x.mp3 where x is a number from 1 to 9. For eaxmple, pressing
 * 2 will play 'track002.mp3'. A lowe case 's' will stop playing the mp3.
 * 'f' will play an MP3 by calling it by it's filename as opposed to a track
 * number.
 *
 * Sketch assumes you have MP3 files with filenames like "track001.mp3",
 * "track002.mp3", etc on an SD card loaded into the shield.
 */

#include <SPI.h>

//Add the SdFat Libraries
#include <SdFat.h>
#include <SdFatUtil.h>

//and the MP3 Shield Library
#include <SFEMP3Shield.h>

// Below is not needed if interrupt driven. Safe to remove if not using.
#if defined(USE_MP3_REFILL_MEANS) && USE_MP3_REFILL_MEANS == USE_MP3_Timer1
  #include <TimerOne.h>
#elif defined(USE_MP3_REFILL_MEANS) && USE_MP3_REFILL_MEANS == USE_MP3_SimpleTimer
  #include <SimpleTimer.h>
#endif

/**
 * \brief Object instancing the SdFat library.
 *
 * principal object for handling all SdCard functions.
 */
SdFat sd;

/**
 * \brief Object instancing the SFEMP3Shield library.
 *
 * principal object for handling all the attributes, members and functions for the library.
 */
SFEMP3Shield MP3player;

//------------------------------------------------------------------------------
/**
 * \brief Setup the Arduino Chip's feature for our use.
 *
 * After Arduino's kernel has booted initialize basic features for this
 * application, such as Serial port and MP3player objects with .begin.
 * Along with displaying the Help Menu.
 *
 */
void setup() {

  uint8_t result; //result code from some function as to be tested at later time.

  Serial.begin(115200);

  // Assign the pin for an input device
  // pinMode(2, INPUT);
  
  //Initialize the SdCard.
  if(!sd.begin(SD_SEL, SPI_FULL_SPEED)) sd.initErrorHalt();
  // depending upon your SdCard environment, SPI_HAVE_SPEED may work better.
  if(!sd.chdir("/")) sd.errorHalt("sd.chdir");

  //Initialize the MP3 Player Shield
  result = MP3player.begin();

#if defined(__BIOFEEDBACK_MEGA__) // or other reasons, of your choosing.
  // Typically not used by most shields, hence commented out.
  Serial.println(F("Applying ADMixer patch."));
  if(MP3player.ADMixerLoad("admxster.053") == 0) {
    Serial.println(F("Setting ADMixer Volume."));
    MP3player.ADMixerVol(-3);
  }
#endif

  Serial.println(F("Arduino Music Selector:"));
  help();
}

//------------------------------------------------------------------------------
/**
 * \brief Main Loop the Arduino Chip
 *
 * This is called at the end of Arduino kernel's main loop before recycling.
 * And is where the user's serial input of bytes are read and analyzed by
 * parsed_menu.
 *
 * Additionally, if the means of refilling is not interrupt based then the
 * MP3player object is serviced with the availaible function.
 *
 * \note Actual examples of the libraries public functions are implemented in
 * the parse_menu() function.
 */
void loop() {

// Below is only needed if not interrupt driven. Safe to remove if not using.
#if defined(USE_MP3_REFILL_MEANS) \
    && ( (USE_MP3_REFILL_MEANS == USE_MP3_SimpleTimer) \
    ||   (USE_MP3_REFILL_MEANS == USE_MP3_Polled)      )

  MP3player.available();
#endif

//  // Assign the pins for an input device
//  int state001 = 2;
//  int state010 = 4;
//  int state100 = 5;
//  pinMode(state001, INPUT);
//  pinMode(state010, INPUT);
//  pinMode(state100, INPUT);
//  
//
//  // Code for reading the state based off digital pins
//    if (digitalRead(state001) == HIGH && digitalRead(state010) == LOW && digitalRead(state100) == LOW) {
//      parse_menu(1);
//      
//    } else if (digitalRead(state001) == HIGH && digitalRead(state010) == LOW && digitalRead(state100) == LOW) {
//      parse_menu(2);
//      
//    } else if (digitalRead(state001) == LOW && digitalRead(state010) == HIGH && digitalRead(state100) == LOW) {
//      parse_menu(3);
//      
//    } else if (digitalRead(state001) == HIGH && digitalRead(state010) == HIGH && digitalRead(state100) == LOW) {
//      parse_menu(4);
//      
//    } else if (digitalRead(state001) == HIGH && digitalRead(state010) == LOW && digitalRead(state100) == HIGH) {
//      parse_menu(5);
//      
//    } else if (digitalRead(state001) == LOW && digitalRead(state010) == HIGH && digitalRead(state100) == HIGH) {
//      parse_menu(6);
//      
//    } else if (digitalRead(state001) == HIGH && digitalRead(state010) == HIGH && digitalRead(state100) == HIGH) {
//      parse_menu(7);
//      
//    }

  if(Serial.available()) {
    // parse_menu(digitalRead(PIN_NO)); // get input from attached input at PIN_NO
    parse_menu(Serial.read()); // get command from serial input
  }

  delay(100);
}

//------------------------------------------------------------------------------
/**
 * \brief Decode the Menu.
 *
 * Parses through the characters of the users input, executing corresponding
 * MP3player library functions and features then displaying a brief menu and
 * prompting for next input command.
 */

void parse_menu(byte key_command) {

  uint8_t result;
  char title[30];

  Serial.print(F("Received command: "));
  Serial.write(key_command);
  Serial.println(F(" "));

  //if s, stop the current track
  if(key_command == 's') {
    Serial.println(F("Stopping"));
    MP3player.stopTrack();

  //if 1-9, play corresponding track
  } else if(key_command >= '1' && key_command <= '9') {
    //convert ascii numbers to real numbers
    key_command = key_command - 48;

    result = MP3player.playTrack(key_command);

    //check result, see readme for error codes.
    if(result != 0) {
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
  } else if((key_command == '-') || (key_command == '+')) {
    union twobyte mp3_vol; // create key_command existing variable that can be both word and double byte of left and right.
    mp3_vol.word = MP3player.getVolume(); // returns a double uint8_t of Left and Right packed into int16_t

    if(key_command == '-') { // note dB is negative
      // assume equal balance and use byte[1] for math
      if(mp3_vol.byte[1] >= 254) { // range check
        mp3_vol.byte[1] = 254;
      } else {
        mp3_vol.byte[1] += 2; // keep it simpler with whole dB's
      }
    } else {
      if(mp3_vol.byte[1] <= 2) { // range check
        mp3_vol.byte[1] = 2;
      } else {
        mp3_vol.byte[1] -= 2;
      }
    }
    // push byte[1] into both left and right assuming equal balance.
    MP3player.setVolume(mp3_vol.byte[1], mp3_vol.byte[1]); // commit new volume
    Serial.print(F("Volume changed to -"));
    Serial.print(mp3_vol.byte[1]>>1, 1);
    Serial.println(F("[dB]"));

  // if p, pause track
  } else if(key_command == 'p') {
    if( MP3player.getState() == playback) {
      MP3player.pauseMusic();
      Serial.println(F("Pausing"));
    } else {
      Serial.println(F("Not Playing!"));
    }
    
  // if r, resume track
  } else if(key_command == 'r') {
    if( MP3player.getState() == paused_playback) {
      MP3player.resumeMusic();
      Serial.println(F("Resuming"));
    } else {
      Serial.println(F("No Track to Resume!"));
    }
    
  } else if(key_command == 'g') {
    int32_t offset_ms = 20000; // Note this is just an example, try your own number.
    Serial.print(F("jumping to "));
    Serial.print(offset_ms, DEC);
    Serial.println(F("[milliseconds]"));
    result = MP3player.skipTo(offset_ms);

  } else if(key_command == 'k') {
    int32_t offset_ms = -1000; // Note this is just an example, try your own number.
    Serial.print(F("moving = "));
    Serial.print(offset_ms, DEC);
    Serial.println(F("[milliseconds]"));
    result = MP3player.skip(offset_ms);

  // if h, print help
  } else if(key_command == 'h') {
    help();

  // if any other command, pring error
  } else {
    Serial.println(F("ERROR: Invalid Command."));
    
  }
  // print prompt after key stroke has been processed.
  Serial.print(F("Enter: 1-9, s, p, r, +, -, g, k, h\n"));
}

//------------------------------------------------------------------------------
/**
 * \brief Print Help Menu.
 *
 * Prints a full menu of the commands available along with descriptions.
 */
void help() {
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
