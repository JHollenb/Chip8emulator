#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "graphicsInterface.h"

int main (int argc, char ** argv)
{
	graphicsInterface graphics("Jake's Emulator", 10);
	graphics.initGraphics(&argc, argv);
	glutMainLoop(); 

	return 0;
}

