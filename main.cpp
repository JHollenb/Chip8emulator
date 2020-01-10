#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>
#include <GLUT/glut.h>
#include "chip8.h"

chip8 myChip8;

/////////////////////////////////////////////////////////////////
// OpenGL functions
/////////////////////////////////////////////////////////////////
int modifier = 15;
int display_width = SCREEN_WIDTH * modifier;
int display_height = SCREEN_HEIGHT * modifier;
void display();
void reshape_window(GLsizei w, GLsizei h);
void keyboardUp(unsigned char key, int x, int y);
void keyboardDown(unsigned char key, int x, int y);

typedef uint8_t u8;
u8 screenData[SCREEN_HEIGHT][SCREEN_WIDTH][3];
void setupTexture();
void launchOpenGL(int argc, char ** argv);
/////////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{
	char * flag = argv[1];

	if (argc == 2)
	{
		myChip8.init(argv[1]);
		myChip8.printDisassembly();
	}
	else if (argc == 3)
	{
		if (strcmp(flag, "--disassemble") == 0)
		{
			myChip8.init(argv[2]);
			myChip8.printDisassembly();
		}
		else if (strcmp(flag, "-d") == 0)
		{
			myChip8.init(argv[2]);
			myChip8.printDisassembly();
		}
		else if (strcmp(flag, "--play") == 0)
		{
			launchOpenGL(argc, argv);
		}
		else if (strcmp(flag, "-p") == 0)
		{
			launchOpenGL(argc, argv);
		}
		else
		{
			printf("How to use: ./emulator [-d disassemble | --disassemble] [-p play | --play] [path to ROMs]\n");
			printf("        or  ./emulator [path to ROMS] will default to the disassemble option.\n");
			return 1;
		}
	}
	else
	{
		printf("How to use: ./emulator [-d disassemble | --disassemble] [-p play | --play] [path to ROMs]\n");
		printf("        or  ./emulator [path to ROMS] will default to the disassemble option.\n");
		return 1;
	}

	return 0;
}

/////////////////////////////////////////////////////////////////
// OpenGL functions
/////////////////////////////////////////////////////////////////
void launchOpenGL(int argc, char ** argv)
{
	myChip8.init(argv[2]);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowSize(display_width, display_height);
    glutInitWindowPosition(320, 320);
	glutCreateWindow("Chip8 Emulator");

	glutDisplayFunc(display);
	glutIdleFunc(display);
    glutReshapeFunc(reshape_window);
	glutKeyboardFunc(keyboardDown);
	glutKeyboardUpFunc(keyboardUp);

	glutMainLoop(); 
}

// Setup Texture
void setupTexture()
{
	// Clear screen
	for(int y = 0; y < SCREEN_HEIGHT; ++y)
		for(int x = 0; x < SCREEN_WIDTH; ++x)
			screenData[y][x][0] = screenData[y][x][1] = screenData[y][x][2] = 0;

	// Create a texture
	glTexImage2D(GL_TEXTURE_2D, 0, 3, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)screenData);

	// Set up the texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	// Enable textures
	glEnable(GL_TEXTURE_2D);
}

void updateTexture(const chip8 & c8)
{
	// Update pixels
	for(int y = 0; y < SCREEN_HEIGHT; ++y)
		for(int x = 0; x < SCREEN_WIDTH; ++x)
			if(c8.screen[(y * SCREEN_WIDTH) + x] == 0)
				screenData[y][x][0] = screenData[y][x][1] = screenData[y][x][2] = 0;	// Disabled
			else
				screenData[y][x][0] = screenData[y][x][1] = screenData[y][x][2] = 255;  // Enabled

	// Update Texture
	glTexSubImage2D(GL_TEXTURE_2D, 0 ,0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)screenData);

	glBegin( GL_QUADS );
		glTexCoord2d(0.0, 0.0);		glVertex2d(0.0,			  0.0);
		glTexCoord2d(1.0, 0.0); 	glVertex2d(display_width, 0.0);
		glTexCoord2d(1.0, 1.0); 	glVertex2d(display_width, display_height);
		glTexCoord2d(0.0, 1.0); 	glVertex2d(0.0,			  display_height);
	glEnd();
}

// Old screen code
void drawPixel(int x, int y)
{
	glBegin(GL_QUADS);
		glVertex3f((x * modifier) + 0.0f,     (y * modifier) + 0.0f,	 0.0f);
		glVertex3f((x * modifier) + 0.0f,     (y * modifier) + modifier, 0.0f);
		glVertex3f((x * modifier) + modifier, (y * modifier) + modifier, 0.0f);
		glVertex3f((x * modifier) + modifier, (y * modifier) + 0.0f,	 0.0f);
	glEnd();
}

void updateQuads(const chip8& c8)
{
	// Draw
	for(int y = 0; y < SCREEN_HEIGHT; ++y)
		for(int x = 0; x < SCREEN_WIDTH; ++x)
		{
			if(c8.screen[(y*SCREEN_WIDTH) + x] == 0)
				glColor3f(0.0f,0.0f,0.0f);
			else
				glColor3f(1.0f,1.0f,1.0f);

			drawPixel(x, y);
		}
}

void display()
{
	myChip8.loop();

	if(myChip8.drawFlag)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		updateQuads(myChip8);
		glutSwapBuffers();
		myChip8.drawFlag = false;
	}
}

void reshape_window(GLsizei w, GLsizei h)
{
	glClearColor(0.0f, 0.0f, 0.5f, 0.0f);
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, h, 0);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, w, h);

	// Resize quad
	display_width = w;
	display_height = h;
}

void keyboardDown(unsigned char key, int x, int y)
{
	if(key == 27)    // esc
		exit(0);

	if(key == '1')		myChip8.key[0x1] = 1;
	else if(key == '2')	myChip8.key[0x2] = 1;
	else if(key == '3')	myChip8.key[0x3] = 1;
	else if(key == '4')	myChip8.key[0xC] = 1;

	else if(key == 'q')	myChip8.key[0x4] = 1;
	else if(key == 'w')	myChip8.key[0x5] = 1;
	else if(key == 'e')	myChip8.key[0x6] = 1;
	else if(key == 'r')	myChip8.key[0xD] = 1;

	else if(key == 'a')	myChip8.key[0x7] = 1;
	else if(key == 's')	myChip8.key[0x8] = 1;
	else if(key == 'd')	myChip8.key[0x9] = 1;
	else if(key == 'f')	myChip8.key[0xE] = 1;

	else if(key == 'z')	myChip8.key[0xA] = 1;
	else if(key == 'x')	myChip8.key[0x0] = 1;
	else if(key == 'c')	myChip8.key[0xB] = 1;
	else if(key == 'v')	myChip8.key[0xF] = 1;

	//printf("Press key %c\n", key);
}

void keyboardUp(unsigned char key, int x, int y)
{
	if(key == '1')		myChip8.key[0x1] = 0;
	else if(key == '2')	myChip8.key[0x2] = 0;
	else if(key == '3')	myChip8.key[0x3] = 0;
	else if(key == '4')	myChip8.key[0xC] = 0;

	else if(key == 'q')	myChip8.key[0x4] = 0;
	else if(key == 'w')	myChip8.key[0x5] = 0;
	else if(key == 'e')	myChip8.key[0x6] = 0;
	else if(key == 'r')	myChip8.key[0xD] = 0;

	else if(key == 'a')	myChip8.key[0x7] = 0;
	else if(key == 's')	myChip8.key[0x8] = 0;
	else if(key == 'd')	myChip8.key[0x9] = 0;
	else if(key == 'f')	myChip8.key[0xE] = 0;

	else if(key == 'z')	myChip8.key[0xA] = 0;
	else if(key == 'x')	myChip8.key[0x0] = 0;
	else if(key == 'c')	myChip8.key[0xB] = 0;
	else if(key == 'v')	myChip8.key[0xF] = 0;
}

