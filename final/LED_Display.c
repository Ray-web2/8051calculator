#include "LED_Display.h"
const char a[17] = { 0b00111111,
0b00000110,
0b01011011,
0b01001111,
0b01100110,
0b01101101,
0b01111101,
0b00000111,
0b01111111,
0b01101111,
0b01110111,
0b01111100,
0b00111001,
0b01011110,
0b01111001,
0b01110001,
0b00000000 }; // led display for 0~f and space

void LED_Display(unsigned char digit, unsigned char num)
{
	P1 = digit;		// set which digit to display
	P2 = a[num];	// set 7-seg display
}