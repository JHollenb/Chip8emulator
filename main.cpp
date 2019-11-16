#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>
#include "chip8.h"

char pressedKey;
chip8 myChip;

void clearGFXMemory();
void outputGFXBuffer();
int kbhit();
void unsetKeys();
void handleKeyboardInput();


int main(int argc, char **argv)
{

	if(argc < 2)
	{
		printf("How to use: ./emu8 chip8application\n\n");
		return 1;
	}


	initscr();
	myChip.init(argv[1]);
    clearGFXMemory();
	nodelay(stdscr, TRUE);	
	cbreak();
	noecho();	
	while(1) // main loop
	{

		pressedKey = getch();
		handleKeyboardInput();
		myChip.loop();
		if(myChip.drawFlag)
			outputGFXBuffer();
		refresh();	
		usleep(2200);
		if(kbhit())
			unsetKeys();
	}
	getch();
	endwin();

 return 0;
}


void clearGFXMemory()
{
	for(int i = 0; i < 2048; ++i)
	{	
		myChip.screen[i] = 0;
	}
}

void outputGFXBuffer()
{
	int y = 0;
	int x = 0;

		for(y = 0; y < 32; y++)
		{
			for(x = 0; x < 64; x++)
			{
				move(y,x);

				if(myChip.screen[x + (y * 64)])
				{
					attron(A_REVERSE);
					printw(" ");
					attroff(A_REVERSE);
				}else{
					printw(" ");
				}
			}
		}
}


int kbhit(void)
{
    int ch = getch();

    if (ch != ERR) {
        ungetch(ch);
        return 1;
    } else {
        return 0;
    }
}

void unsetKeys()
{
	int k;

	for(k = 0; k < 16; k++)
	{
		myChip.key[k] = 0;
	}

}

void handleKeyboardInput()
{
	switch(pressedKey)
	{
		case 49:
			myChip.key[0] = 1;		
			break;	
		case 50:
			myChip.key[1] = 1;
			break;
		case 51:
			myChip.key[2] = 1;
			break;
		case 52:
			myChip.key[3] = 1;
			break;
		case 113:
			myChip.key[4] = 1;		
			break;
		case 119:
			myChip.key[5] = 1;		
			break;
		case 101:
			myChip.key[6] = 1;		
			break;
		case 114:
			myChip.key[7] = 1;		
			break;
		case 97:
			myChip.key[8] = 1;		
			break;
		case 115:
			myChip.key[9] = 1;		
			break;
		case 100:
			myChip.key[10] = 1;		
			break;
		case 102:
			myChip.key[11] = 1;		
			break;
		case 121:
			myChip.key[12] = 1;		
			break;
		case 120:
			myChip.key[13] = 1;		
			break;
		case 99:
			myChip.key[14] = 1;		
			break;
		case 118:
			myChip.key[15] = 1;		
			break;
		
		default:
			break;
	}

}
