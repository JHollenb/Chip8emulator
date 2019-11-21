#include "graphicsInterface.h"

graphicsInterface::graphicsInterface(const char * name, int modifier): 
	m_name(name),
	m_modifier(modifier),
	m_display_width(SCREEN_WIDTH * m_modifier),
	m_display_height(SCREEN_HEIGHT * m_modifier),
	m_emulator()
{
}

void graphicsInterface::initGraphics(int * argc, char ** argv)
{
	m_emulator.init(argv[1]);

	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowSize(m_display_width, m_display_height);
    glutInitWindowPosition(320, 320);
	glutCreateWindow(m_name);

	glutDisplayFunc(&graphicsInterface::display);
	glutIdleFunc(&graphicsInterface::display);
    glutReshapeFunc(reshape_window);
	glutKeyboardFunc(keyboardDown);
	glutKeyboardUpFunc(keyboardUp);

#ifdef DRAWWITHTEXTURE
	setupTexture();			
#endif	
}

void graphicsInterface::setupTexture()
{
	// Clear screen
	for(int y = 0; y < SCREEN_HEIGHT; ++y)
	{
		for(int x = 0; x < SCREEN_WIDTH; ++x)
		{
			screenData[y][x][0] = screenData[y][x][1] = screenData[y][x][2] = 0;
		}
	}

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

void graphicsInterface::updateTexture(const chip8 & c8)
{
	// Update pixels
	for(int y = 0; y < 32; ++y)
	{
		for(int x = 0; x < 64; ++x)
		{
			if(c8.screen[(y * 64) + x] == 0)
			{
				// Disabled
				screenData[y][x][0] = screenData[y][x][1] = screenData[y][x][2] = 0;
			}
			else
			{
				// Enabled
				screenData[y][x][0] = screenData[y][x][1] = screenData[y][x][2] = 255; 
			}
		}
	}

	// Update Texture
	glTexSubImage2D(GL_TEXTURE_2D, 0 ,0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)screenData);

	glBegin( GL_QUADS );
		glTexCoord2d(0.0, 0.0);		glVertex2d(0.0,			  0.0);
		glTexCoord2d(1.0, 0.0); 	glVertex2d(m_display_width, 0.0);
		glTexCoord2d(1.0, 1.0); 	glVertex2d(m_display_width, m_display_height);
		glTexCoord2d(0.0, 1.0); 	glVertex2d(0.0,			  m_display_height);
	glEnd();
}

// Old screen code
void graphicsInterface::drawPixel(int x, int y)
{
	glBegin(GL_QUADS);
		glVertex3f((x * m_modifier) + 0.0f,     (y * m_modifier) + 0.0f,	 0.0f);
		glVertex3f((x * m_modifier) + 0.0f,     (y * m_modifier) + m_modifier, 0.0f);
		glVertex3f((x * m_modifier) + m_modifier, (y * m_modifier) + m_modifier, 0.0f);
		glVertex3f((x * m_modifier) + m_modifier, (y * m_modifier) + 0.0f,	 0.0f);
	glEnd();
}

void graphicsInterface::updateQuads(const chip8& c8)
{
	// Draw
	for(int y = 0; y < 32; ++y)
	{
		for(int x = 0; x < 64; ++x)
		{
			if(c8.screen[(y*64) + x] == 0)
				glColor3f(0.0f,0.0f,0.0f);
			else
				glColor3f(1.0f,1.0f,1.0f);

			drawPixel(x, y);
		}
	}
}

void graphicsInterface::display()
{
	m_emulator.loop();

	if(m_emulator.drawFlag)
	{
		// Clear framebuffer
		glClear(GL_COLOR_BUFFER_BIT);

#ifdef DRAWWITHTEXTURE
		updateTexture(m_emulator);
#else
		updateQuads(m_emulator);
#endif
		// Swap buffers!
		glutSwapBuffers();

		// Processed frame
		m_emulator.drawFlag = false;
	}
}

void graphicsInterface::reshape_window(GLsizei w, GLsizei h)
{
	glClearColor(0.0f, 0.0f, 0.5f, 0.0f);
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, h, 0);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, w, h);

	// Resize quad
	m_display_width = w;
	m_display_height = h;
}

void graphicsInterface::keyboardDown(unsigned char key, int x, int y)
{
	if(key == 27)    // esc
		exit(0);

	if(key == '1')		m_emulator.key[0x1] = 1;
	else if(key == '2')	m_emulator.key[0x2] = 1;
	else if(key == '3')	m_emulator.key[0x3] = 1;
	else if(key == '4')	m_emulator.key[0xC] = 1;

	else if(key == 'q')	m_emulator.key[0x4] = 1;
	else if(key == 'w')	m_emulator.key[0x5] = 1;
	else if(key == 'e')	m_emulator.key[0x6] = 1;
	else if(key == 'r')	m_emulator.key[0xD] = 1;

	else if(key == 'a')	m_emulator.key[0x7] = 1;
	else if(key == 's')	m_emulator.key[0x8] = 1;
	else if(key == 'd')	m_emulator.key[0x9] = 1;
	else if(key == 'f')	m_emulator.key[0xE] = 1;

	else if(key == 'z')	m_emulator.key[0xA] = 1;
	else if(key == 'x')	m_emulator.key[0x0] = 1;
	else if(key == 'c')	m_emulator.key[0xB] = 1;
	else if(key == 'v')	m_emulator.key[0xF] = 1;

	printf("Press key %c\n", key);
}

void graphicsInterface::keyboardUp(unsigned char key, int x, int y)
{
	if(key == '1')		m_emulator.key[0x1] = 0;
	else if(key == '2')	m_emulator.key[0x2] = 0;
	else if(key == '3')	m_emulator.key[0x3] = 0;
	else if(key == '4')	m_emulator.key[0xC] = 0;

	else if(key == 'q')	m_emulator.key[0x4] = 0;
	else if(key == 'w')	m_emulator.key[0x5] = 0;
	else if(key == 'e')	m_emulator.key[0x6] = 0;
	else if(key == 'r')	m_emulator.key[0xD] = 0;

	else if(key == 'a')	m_emulator.key[0x7] = 0;
	else if(key == 's')	m_emulator.key[0x8] = 0;
	else if(key == 'd')	m_emulator.key[0x9] = 0;
	else if(key == 'f')	m_emulator.key[0xE] = 0;

	else if(key == 'z')	m_emulator.key[0xA] = 0;
	else if(key == 'x')	m_emulator.key[0x0] = 0;
	else if(key == 'c')	m_emulator.key[0xB] = 0;
	else if(key == 'v')	m_emulator.key[0xF] = 0;
}

