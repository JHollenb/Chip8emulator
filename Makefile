CC = gcc            # default is CC = cc
CFLAGS = -g -Wall   # default is CFLAGS = [blank]
CPPFLAGS =          # default is CPPFLAGS = [blank]
LDFLAGS = 

# default compile command: $(CC) $(CFLAGS) $(CPPFLAGS) -c -o <foo>.o <foo>.c

all: chip8disassembler chip8emulator

chip8disassembler: chip8disassembler.c
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

chip8emulator: chip8emulator.c
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

clean:
	$(RM) -f *.o *.exe chip8disassembler chip8emulator
