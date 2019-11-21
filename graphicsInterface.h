#include <GLUT/glut.h>
#include "chip8.h"

typedef uint8_t u8;

class graphicsInterface
{
public:
	graphicsInterface(const char *, int);
	~graphicsInterface(){}

	void initGraphics(int *, char **);
	void display();
	void reshape_window(GLsizei w, GLsizei h);
	void keyboardUp(unsigned char key, int x, int y);
	void keyboardDown(unsigned char key, int x, int y);
	void setupTexture();

protected:
private:
	const char * m_name;
	int m_modifier;
	int m_display_width;
	int m_display_height;
	u8 screenData[SCREEN_HEIGHT][SCREEN_WIDTH][3];

	// TODO Make this generic
	chip8 m_emulator;
};

