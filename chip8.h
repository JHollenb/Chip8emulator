#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define OFFSET 0x200
#define SUCCESS 0
#define ERROR -1

// See [Chip 8](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#0.1)

class chip8
{
public:
	typedef struct chipStates
	{
		uint8_t V[16];
		uint16_t i;
		uint16_t sp;
		uint16_t pc;
		uint8_t delay;
		uint8_t sound;
		uint8_t * memory;
		uint8_t * screen;
	} chipStates;

	chip8(const char * rom);
	~chip8(){};
	void printDisassembly ();
	void init();
	void emulate();
protected:
private:
	void p_addr(const char * call, uint8_t byte0, uint8_t byte1);
	void p_reg_byte(const char * call, uint8_t byte0, uint8_t byte1);
	void p_reg_reg(const char * call, uint8_t byte0, uint8_t byte1);
	void p_reg(const char * call, uint8_t byte);
	int openRom ();
	void disassemble (unsigned char * buffer, int pc);
	void unimplementedInstruction();
	

	// Member vars
	const char * m_romPath;
	FILE * m_file;
	int m_fsize;
	chipStates m_states;
};
