#include <stdio.h>

#define OFFSET 0x200

// See [Chip 8](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#0.1)

class chip8
{
public:
	chip8();
	~chip8(){};
	int open_file (const char * file);
	void print_file ();
protected:
private:
	void p_addr(const char * call, uint8_t byte0, uint8_t byte1);
	void p_reg_byte(const char * call, uint8_t byte0, uint8_t byte1);
	void p_reg_reg(const char * call, uint8_t byte0, uint8_t byte1);
	void p_reg(const char * call, uint8_t byte);
	void disassemble (unsigned char * buffer, int pc);

	FILE * m_file;
	int m_fsize;
};
