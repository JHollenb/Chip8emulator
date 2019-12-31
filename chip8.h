#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include <stdint.h>

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32
#define OFFSET 0x200
#define SUCCESS 0
#define ERROR -1

// See [Chip 8](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#0.1)

class chip8
{
public:
	chip8(const char * rom);
	chip8();
	~chip8(){};
	void printDisassembly();
	//void init();
	void init(const char * rom);
	void loop();
	void disassemble (uint8_t * code);

	// Member vars
	bool drawFlag;
	const char * m_romPath;
	FILE * m_file;
	int m_fsize;

	// States
	uint8_t v[16];				// CPU registers
	uint16_t I;					// Index register
	uint16_t pc;				// Program counter
	uint8_t delay;				// delay timer @ 60Hz
	uint8_t sound;				// sound timer @ 60Hz
	uint8_t memory[4096];		// Chip8 has 4k memory
	uint8_t screen[SCREEN_WIDTH * SCREEN_HEIGHT];	// Graphics array (screen is 64 x 32)
	uint16_t sp;			    // Stack pointer
	uint16_t stack[16];			// 16 levels of stack
	uint8_t key[16];			// Hex keypad 0x0 - 0xf
protected:
private:
	void instructions0(uint8_t kk);
	void instructions8(uint8_t x, uint8_t y, uint8_t kk);
	void instructionsf(uint8_t x, uint8_t y, uint8_t kk);
	void unimplementedInstruction();
	void p_addr(const char * call, uint8_t byte0, uint8_t byte1);
	void p_reg_byte(const char * call, uint8_t byte0, uint8_t byte1);
	void p_reg_reg(const char * call, uint8_t byte0, uint8_t byte1);
	void p_reg(const char * call, uint8_t byte);
	int openROM();
	int loadROM();
};
