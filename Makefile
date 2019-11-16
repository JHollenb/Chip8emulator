CC = g++
CFLAGS = -g -Wall
TARGET = main

CPPFLAGS =
LDFLAGS =
SOURCES = main.cpp chip8.cpp 
LIBS = -lncurses
# default compile command: $(CC) $(CFLAGS) $(CPPFLAGS) -c -o <foo>.o <foo>.c

$(TARGET) : $(TARGET).o chip8.o
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $(TARGET) $(TARGET).o chip8.o $(LIBS)

chip8.o: chip8.cpp chip8.h
	$(CC) $(CFLAGS) -c chip8.cpp

clean:
	$(RM) -rf *.o main
