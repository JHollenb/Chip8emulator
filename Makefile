CC = g++
CFLAGS = -g -Wall 
TARGET = main

CPPFLAGS =       
LDFLAGS = 
SOURCES = main.cpp chip8.cpp 

# default compile command: $(CC) $(CFLAGS) $(CPPFLAGS) -c -o <foo>.o <foo>.c

$(TARGET) : $(TARGET).o chip8.o
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).o chip8.o

chip8: chip8.cpp chip8.h
	$(CC) $(CFLAGS) -c chip8.cpp

clean:
	$(RM) -rf *.o main
