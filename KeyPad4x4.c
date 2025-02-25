#include "KeyPad4x4.h"

char KeyPad4x4() {
	P0 = 0xf0;

	// find the row and column of the key
	// then return the index of the key
	for (char i = 0; i < 4; i++)
	{
		P0 = ~(1 << i);

		for (char j = 0; j < 4; j++)
		{
			if (!(P0 & (0x10 << j))) return j * 4 + i;
		}
	}

	return -1; //if key no found return -1
}
