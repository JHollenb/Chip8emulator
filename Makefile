CC = g++
CFLAGS = -g -Wall
TARGET = main

CPPFLAGS = -DGL_SILENCE_DEPRECATION
LDFLAGS =
SOURCES = main.cpp chip8.cpp 
opengl_libs := -framework OpenGL -framework GLUT
# default compile command: $(CC) $(CFLAGS) $(CPPFLAGS) -c -o <foo>.o <foo>.c

$(TARGET) : $(TARGET).o chip8.o
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $(TARGET) $(TARGET).o chip8.o $(opengl_libs)

chip8.o: chip8.cpp chip8.h
	$(CC) $(CFLAGS) -c chip8.cpp

clean:
	$(RM) -rf *.o main
