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
   @Date 2/04/2019

   Method for creating a display string for scrolling across the screen. It needs a buffer of 2 spaces at the start and one at the end.
*/
char* getDisplayString(char* inputString, int numChar)
{
  char* displayString = (char*)malloc(sizeof(char) * (numChar + 4));
  displayString[0] = ' ';
  displayString[1] = ' ';

  for (int i = 0; i < numChar; i++)
  {
    memcpy(&displayString[i + 2], &inputString[i], sizeof(char));
  }

  displayString[numChar + 2] = ' ';
  displayString[numChar + 3] = ' ';

  return displayString;
}

/*
   @Author Jesse Sieunarine
   @Date 2/04/2019

   Method for scrolling text across one display, would be possibly to modify it to work with multiple.
   TODO: Modify the method to use the extended bitmap to allow for different sized characters
*/
void LedControlExtended::writeScrollingStringOld(int mtx, char* inputString, long delayMS) {

  int numChar = strlen(inputString);
  char* displayString = getDisplayString(inputString, numChar);
  int charPos = 0;
  int currChar = 0;
  
  while (currChar < numChar + 2) //As we add a buffer of 2 space characters to each side of the string but don't want to complete scrolling the last space otherwise we have a memory leak as there is no next char to display
  {
    int charNum = currChar; //The char we want to display the rows from
    int arrayPos = getCharArrayPositionExtended(displayString[charNum]); //the position in the aplabetbitmap
    int len = alphabetBitmapExtended[arrayPos][0]; //The first element in the bitmap is the length of that array
    int shiftLeft = charPos % len; 
    int shiftRight = 0;
    int index = shiftLeft;
    int i = charPos;


    //Sends the data to the LED Matrix row by row
    for (int j = 0; j < 8; j++)
    {
      index = j + shiftLeft - shiftRight;

      if (index == len) //If we the index is  equal to the length of the array then we can start displaying rows from the next
      {
        charNum++;
        shiftRight = j; //We need to shift right the number of rows we have already displayed, so we can start display from the first row of the new char
        arrayPos = getCharArrayPositionExtended(displayString[charNum]);
        len = alphabetBitmapExtended[arrayPos][0];
        shiftLeft = 0; //We no longer need to shift left
        index = j + shiftLeft - shiftRight; //Calc the new index of the row
      }

      setRow(mtx, j, alphabetBitmapExtended[arrayPos][index + 1]); //+1 to the index as the first element is the array size
    }

    charPos++; //The char needs to be shifted one row across the display

    //Checking if the current char is currently off the screen, if so then move onto the next char
    arrayPos = getCharArrayPositionExtended(displayString[currChar]);
    len = alphabetBitmapExtended[arrayPos][0];
    if (charPos == len) //Change to the next character
    {
      currChar++;
      charPos = 0;
    }
    
    delay(delayMS);
    clearAll();
  }
  
  free(displayString);
}
