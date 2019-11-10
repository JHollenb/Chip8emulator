#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "chip8.h"

int main (int argc, char ** argv)
{
	// TODO: set up graphics
	// TODO: set up input

	// Initialize chip8 and load ROM into memory
	chip8 myChip(argv[1]);
	myChip.init();
    //myChip.printDisassembly();

	int iter = 5;
	for (int i = 0 ; i < iter; ++i)
	{
		myChip.loop();
	}

	//while (1)
	//{
		// loop one cycle
		//myChip.loop();

		// TODO: update graphics
		// TODO: update key press state
	//}

	return 0;
}

