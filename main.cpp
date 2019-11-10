#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "chip8.h"

#define EMULATOR

int main (int argc, char ** argv)
{
	// TODO: set up graphics
	// TODO: set up input

	// Initialize chip8 and load ROM into memory
	chip8 myChip(argv[1]);
	myChip.init();

#ifdef EMULATOR
	int iter = 15;
	for (int i = 0 ; i < iter; ++i)
	{
		myChip.loop();
	}
#else
    myChip.printDisassembly();
#endif

	//while (1)
	//{
		// loop one cycle
		//myChip.loop();

		// TODO: update graphics
		// TODO: update key press state
	//}

	return 0;
}

