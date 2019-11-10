#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "chip8.h"

int main (int argc, char ** argv)
{
	chip8 myChip;
	myChip.open_file(argv[1]);
	myChip.print_file();

	return 0;
}

