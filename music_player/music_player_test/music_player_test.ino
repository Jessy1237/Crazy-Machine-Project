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

#include "Music.h"
void setup() {

  uint8_t result; //result code from some function as to be tested at later time.

  Serial.begin(115200);

  // Assign the pin for an input device
  // pinMode(2, INPUT);
  
  //Initialize the SdCard.
  setupMusic();
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
