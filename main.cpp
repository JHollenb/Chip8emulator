#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "chip8.h"

int main (int argc, char ** argv)
{
	chip8 myChip(argv[1]);

	myChip.printDisassembly();

	return 0;
}

