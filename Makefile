CC = g++
CFLAGS = -g -Wall
TARGET = emulator

CPPFLAGS = -DGL_SILENCE_DEPRECATION
LDFLAGS =
SOURCES = main.cpp chip8.cpp 
terminal_libs := -lncurses
opengl_libs := -framework OpenGL -framework GLUT
LIBS = $(opengl_libs) $(terminal_libs)
# default compile command: $(CC) $(CFLAGS) $(CPPFLAGS) -c -o <foo>.o <foo>.c

$(TARGET) : main.o chip8.o
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $(TARGET) main.o chip8.o $(LIBS)

chip8.o: chip8.cpp chip8.h
	$(CC) $(CFLAGS) -c chip8.cpp

clean:
	$(RM) -rf *.o emulator
