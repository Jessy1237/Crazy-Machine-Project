#include "LedExtended.h"
#include <string.h>
/*
 * LedExtended.cpp - An extended library of LedControlMS for controling Leds with a MAX7219/MAX7221
 *
 */
LedControlExtended::LedControlExtended(int dataPin, int clkPin, int csPin, int numDevices) : LedControl(dataPin, clkPin, csPin, numDevices)
{
	
}

/*
 * @Author Jesse Sieunarine
 * @Date 2/04/2019
 * 
 * Method for getting the index value of the char in the extended bitmap
 */
int LedControlExtended::getCharArrayPositionExtended(char input){
     if ((input==' ')||(input=='+')) return 10;
     if (input==':') return 11;
     if (input=='-') return 12;
     if (input=='.') return 13;
     if ((input =='(')) return  14;  //replace by '�'
     if (input=='!') return 15;	 
	 if (input=='\'') return 16;
     if(input=='=') return 16;//Added in the position for the '='
     if ((input >='0')&&(input <='9')) return (input-'0');
     if ((input >='A')&&(input <='Z')) return (input-'A' + 18);
     if ((input >='a')&&(input <='z')) return (input-'a' + 18);     
     return 13;
} 

/*
 * @Author Jesse Sieunarine
 * @Date 2/04/2019
 * 
 * Method for creating a display string for scrolling across the screen. It needs a buffer of 2 spaces at the start and one at the end.
 */
char* getDisplayString(char* inputString, int numChar)
{
  char* displayString = (char*)malloc(sizeof(char)*(numChar+3));
  displayString[0] = ' ';
  displayString[1] = ' ';
  
  for(int i=0; i < numChar; i++)
  {
    memcpy(&displayString[i+2], &inputString[i], sizeof(char));
  }
  
  displayString[numChar+2] = ' ';

  return displayString;
}

/*
 * @Author Jesse Sieunarine
 * @Date 2/04/2019
 * 
 * Method for scrolling text across one display, would be possibly to modify it to work with multiple.
 * TODO: Modify the method to use the extended bitmap to allow for different sized characters
 */
void LedControlExtended::writeScrollingStringOld(int mtx, char* inputString, long delayMS) {

  int numChar = strlen(inputString);
  char* displayString = getDisplayString(inputString, numChar);
  
  int scrollLength = (numChar+2)*6;
  
  for(int i=0; i<scrollLength-1; i++)
  {
	  int pos;
	  int nextPos;
	  int nextNextPos;
	  char c;
	  
	  c = displayString[i/6];
	  pos = getCharArrayPosition(c);
	  c = displayString[i/6+1];
      nextPos = getCharArrayPosition(c);
	  c = displayString[i/6+2];
	  nextNextPos = getCharArrayPosition(c);

	  /* Old Code for the Vertical Scrolling
	  byte currChar[6] = {0x0,0x0,0x0,0x0,0x0,0x0};
	  byte nextChar[6] = {0x0,0x0,0x0,0x0,0x0,0x0};
	  memcpy(&currChar, &alphabetBitmap[pos], sizeof(byte)*6);
	  memcpy(&nextChar, &alphabetBitmap[nextPos], sizeof(byte)*6);
	  */
	  
	  for(int j=0; j<8; j++)
	  {

		  int shiftLeft =  i % 6;
		  int index = j + shiftLeft;
		  
		  /* Old Code for the Vertical Scrolling
		  byte data = ((currChar[j] << shiftLeft) | (nextChar[j] >> shiftRight)) & 0xFF;
		  setRow(mtx, j, data);
		  */
		  
		  if(index < 6)
		  {
		    setRow(mtx, j, alphabetBitmap[pos][index]);
		  }
		  //Do the next next character
		  else if ( index - 6 >= 6)
		  {
			  setRow(mtx, j, alphabetBitmap[nextNextPos][index-12]);
		  }
		  //do the next character
		  else
		  {
			setRow(mtx, j, alphabetBitmap[nextPos][index - 6]);
		  }
	  }
	  
	  delay(delayMS);
      clearAll();
  }
  free(displayString);
}
