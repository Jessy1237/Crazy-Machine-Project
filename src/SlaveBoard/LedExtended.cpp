#include "LedExtended.h"
#include <string.h>
/*
   LedExtended.cpp - An extended library of LedControlMS for controling Leds with a MAX7219/MAX7221

*/
LedControlExtended::LedControlExtended(int dataPin, int clkPin, int csPin, int numDevices) : LedControl(dataPin, clkPin, csPin, numDevices)
{

}

/*
   @Author Jesse Sieunarine
   @Date 2/04/2019

   Method for getting the index value of the char in the extended bitmap
*/
int LedControlExtended::getCharArrayPositionExtended(char input) {
  if ((input == ' ') || (input == '+')) return 10;
  if (input == ':') return 11;
  if (input == '-') return 12;
  if (input == '.') return 13;
  if ((input == '(')) return  14; //replace by '�'
  if (input == '!') return 15;
  if (input == '\'') return 16;
  if (input == '=') return 16; //Added in the position for the '='
  if ((input >= '0') && (input <= '9')) return (input - '0');
  if ((input >= 'A') && (input <= 'Z')) return (input - 'A' + 18);
  if ((input >= 'a') && (input <= 'z')) return (input - 'a' + 18);
  return 13;
}

/*
   @Author Jesse Sieunarine
   @Date 10/04/2019

   Method for creating a display string for scrolling across the screen. It needs a buffer of 2 spaces at the start and two at the end.
*/
char* getDisplayString(char* inputString, int numDevices)
{
  char* displayString = NULL;

  if (inputString != NULL)
  {
    short numChar = strlen(inputString);
    displayString = (char*)malloc(sizeof(char) * (numChar + 4 * numDevices));

    for (int i = 0; i < numDevices * 2; i++)
    {
      displayString[i] = ' ';
    }

    for (int i = 0; i < numChar; i++)
    {
      displayString[i + 2 * numDevices] = inputString[i];
    }

    for (int i = numDevices * 2; i < numDevices * 4; i++)
    {
      displayString[numChar + i] = ' ';
    }
  }

  return displayString;
}

/*
   @Author Jesse Sieunarine
   @Date 10/04/2019

   Method for scrolling text across one display. The method uses static variables to allow for quick response times to allow for polling in the main loop.
   The return bool allows for switching of input strings once a string has been completely scrolled as it will return true.
   @return true if the string has been completely scrolled across the screen.
*/
bool LedControlExtended::writeScrollingString(int mtx, int numDevices, char* inputString, long delayMS)
{

  static char* scrollString = NULL;
  static char* oldInputString = NULL;
  static short charPos = 0;
  static short currChar = 0;
  static short numChar = 0;
  bool finishedCycle = false;

  //Need to reset the positioning if between polling the string was changed or if we've finished scrolling the current string.
  if (oldInputString != inputString || currChar >= numChar)
  {
    charPos = 0;
    currChar = 0;
  }

  //Need to change the scroll string if between polling the inputstring has changed.
  if (strcmp(oldInputString, inputString) != 0)
  {
    oldInputString = inputString;
    numChar = strlen(inputString) + 2 * numDevices;
    if (scrollString != NULL)
    {
      free(scrollString);
    }
    scrollString = getDisplayString(inputString, numDevices);
  }

  if (currChar < numChar && scrollString != NULL) //As we add a buffer of 2 space characters to each side of the string but don't want to complete scrolling the last space otherwise we have a memory leak as there is no next char to display
  {
    short charNum = currChar; //The char we want to display the rows from
    short arrayPos = getCharArrayPositionExtended(scrollString[charNum]); //the position in the aplabetbitmap
    short len = alphabetBitmapExtended[arrayPos][0]; //The first element in the bitmap is the length of that array
    short shiftLeft = charPos % len;
    short shiftRight = 0;
    short index = shiftLeft;

    //Sends the data to the LED Matrix row by row
    for (int j = mtx * 8; j < 8 * numDevices; j++)
    {
      index = j + shiftLeft - shiftRight;

      if (index == len) //If we the index is  equal to the length of the array then we can start displaying rows from the next
      {
        charNum++;
        shiftRight = j; //We need to shift right the number of rows we have already displayed, so we can start display from the first row of the new char
        arrayPos = getCharArrayPositionExtended(scrollString[charNum]);
        len = alphabetBitmapExtended[arrayPos][0];
        shiftLeft = 0; //We no longer need to shift left
        index = j + shiftLeft - shiftRight; //Calc the new index of the row
      }

      setRow(j / 8, j % 8, alphabetBitmapExtended[arrayPos][index + 1]); //+1 to the index as the first element is the array size
    }

    charPos++; //The char needs to be shifted one row across the display

    //Checking if the current char is currently off the screen, if so then move onto the next char
    arrayPos = getCharArrayPositionExtended(scrollString[currChar]);
    len = alphabetBitmapExtended[arrayPos][0];
    if (charPos == len) //Change to the next character
    {
      currChar++;
      charPos = 0;
    }

    delay(delayMS);
    //clearAll(); //Makes it jittery with clearAll being used
  }

  if (currChar >= numChar)
  {
    finishedCycle = true;
  }

  return finishedCycle;
}

void LedControlExtended::writeScrollingStars(int mtx, int numDevices, long delayMS)
{
  static bool second = false;
  static short starPos = 0;
  const short len = 16;

  if (starPos >= len)
  {
    starPos = 0;
  }

  short shiftLeft = starPos % len;
  short index = shiftLeft;
  short shiftRight = 0;

  //Sends the data to the LED Matrix row by row
  for (int j = mtx * 8; j < 8 * numDevices; j++)
  {
    index = j + shiftLeft - shiftRight;

    if (index == len) //If we the index is  equal to the length of the array then we can start displaying rows from the next
    {
      shiftRight = j; //We need to shift right the number of rows we have already displayed, so we can start display from the first row of the new char
      shiftLeft = 0; //We no longer need to shift left
      index = j + shiftLeft - shiftRight; //Calc the new index of the row
    }

    setRow(j / 8, j % 8, starBitmap[second][index % 16]);
  }

  if (second)
  {
    second = false;
    starPos += 8; //The char needs to be shifted one row across the display
  }
  else
  {
    second = true;
  }

  delay(delayMS);
  clearAll(); //Makes it jittery with clearAll being used



}
