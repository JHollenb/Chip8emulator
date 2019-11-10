#include "chip8.h"

chip8::chip8(const char * rom) :
	m_romPath(rom),
	m_file(NULL),
	m_fsize(0)
{
}

void chip8::p_addr(const char * call, uint8_t byte0, uint8_t byte1)
{
	printf("%-10s $%01x%02x", call, byte0, byte1);
}

void chip8::p_reg_byte(const char * call, uint8_t byte0, uint8_t byte1)
{
	printf("%-10s V%01X,#%02x", call, byte0, byte1);
}

void chip8::p_reg_reg(const char * call, uint8_t byte0, uint8_t byte1)
{
	printf("%-10s V%01X,V%01X", call, byte0, byte1);
}

void chip8::p_reg(const char * call, uint8_t byte)
{
	printf("%-10s V%01X", call, byte);
}

void chip8::disassemble (unsigned char * buffer, int pc)
{
	// 0000     0000
	// code[1]	code[0]
	uint8_t * code = &buffer[pc];
	uint8_t x = (code[0] & 0xf);
	uint8_t y = (code[1] >> 4);
	uint8_t n = (code[1] & 0xf);
	uint8_t kk = code[1];
	// x, kk == addr

	printf("%04x %02x %02x ", pc, code[0], kk);
	switch (code[0] >> 4)
	{
		case 0x0: 
		{
			switch (kk)
			{
				case 0xe0: printf("%-10s", "CLS"); break;
				case 0xee: printf("%-10s", "RTS"); break;
				default: p_addr("SYS", x, kk); break;
			}
			break;
		}

		// 1xxx, 2xxx, Bxxx ops, jumps and calls
		case 0x1: p_addr("JUMP", x, kk); break;
		case 0x2: p_addr("CALL", x, kk); break;

		// 3xxx, 4xxx, 5xxx, 9xxx ops, compare and skip: These instructions 
		// compare 2 values and skip the next instruciton if the test passes
		case 0x3: p_reg_byte("SE", x, kk); break;
		case 0x4: p_reg_byte("SNE", x, kk); break;
		case 0x5: p_reg_reg("SE", x, y); break;
		case 0x6: p_reg_byte("LD", x, kk); break;
		case 0x7: p_reg_byte("ADD", x, kk); break;
		case 0x8: 
			switch (kk & 0x0f)
			{
				case 0x0: p_reg_reg("LD", x, y); break;
				case 0x1: p_reg_reg("OR", x, y); break;
				case 0x2: p_reg_reg("AND", x, y); break;
				case 0x3: p_reg_reg("XOR", x, y); break;
				case 0x4: p_reg_reg("ADD", x, y); break;
				case 0x5: printf("%-10s V%01X,V%01X,V%01X", "SUB", x, x, y); break;
				case 0x6: p_reg_reg("SHR", x, y); break;
				case 0x7: printf("%-10s V%01X,V%01X,V%01X", "SUB", x, y, y); break;
				case 0xe: p_reg_reg("SHL", x, y); break;
				default: printf("UNKNOWN 8"); break;
			}
			break;
		case 0x9: p_reg_reg("SNE", x, y); break;
		case 0xa: p_addr("LD I", x, kk); break;
		case 0xb: p_addr("JUMP V0", x, kk); break;
		case 0xc: p_reg_byte("RND", x, kk); break;
		case 0xd: printf("%-10s V%01X,V%01X,#%01x", "SPRITE", x, y, n); break;
		case 0xe: 
			switch (kk)
			{
				case 0x9e: p_reg("SKP", x); break;
				case 0xa1: p_reg("SKNP", x); break;
				default: printf("UNKNOWN E"); break;
			}
			break;
		case 0xf: 
			switch (kk)
			{
				case 0x07: printf("%-10s V%01X,DELAY", "MOV", x); break;
				case 0x0a: printf("%-10s V%01X", "KEY", x); break;
				case 0x15: printf("%-10s DELAY,V%01X", "MOV", x); break;
				case 0x18: printf("%-10s SOUND,V%01X", "MOV", x); break;
				case 0x1e: printf("%-10s I,V%01X", "ADI", x); break;
				case 0x29: printf("%-10s I,V%01X", "SPRITECHAR", x); break;
				case 0x33: printf("%-10s (I),V%01X", "MOVBCD", x); break;
				case 0x55: printf("%-10s (I),V0-V%01X", "MOVM", x); break;
				case 0x65: printf("%-10s V0-V%01X,(I)", "MOVM", x); break;
				default: printf("UNKNOWN F"); break;
			}
			break;
		default: printf("Not implemented");
	}
}

int chip8::openRom()
{
	int retval = SUCCESS;

	m_file = fopen(m_romPath, "rb");

	if (!m_file)
	{
		printf("[ERROR] Failed to open %s\n", m_romPath);
		retval = ERROR;
	}

	return retval;
}

void chip8::printDisassembly()
{
	if (openRom() == SUCCESS)
	{
		fseek(m_file, 0L, SEEK_END);
		m_fsize = ftell(m_file);

		if (m_fsize == 0)
		{
			printf("[WARNING] File size is 0!\n");
		}

		fseek(m_file, 0L, SEEK_SET);

		// Chip8 puts programs in memory at 0x200
		unsigned char * buffer = (unsigned char *)malloc (m_fsize + OFFSET);
		fread (buffer + OFFSET, m_fsize, 1, m_file);
		fclose (m_file);

		// process file
		int pc = OFFSET;
		while (pc < (m_fsize + OFFSET))
		{
			disassemble (buffer, pc);
			pc+=2;
			printf("\n");
		}
	}
	else
	{
		printf("[ERROR] No file found!\n");
		exit(1);
	}

}

void chip8::init()
{
	m_states.memory = (uint8_t *)calloc (1024 * 4, 1);
	m_states.screen = &m_states.memory[0xf00];
	m_states.sp = 0xfa0;
	// TODO: OFFSET
	m_states.pc = 0x200;
}

void chip8::emulate()
{
	// 0000     0000
	// code[1]	code[0]
	//uint8_t * code = &buffer[pc];
	//uint8_t x = (code[0] & 0xf);
	//uint8_t y = (code[1] >> 4);
	//uint8_t n = (code[1] & 0xf);
	//uint8_t kk = code[1];
	// x, kk == addr
	uint8_t * op = &m_states.memory[m_states.pc];
	int highnib = (*op & 0xf0) >> 4;
	switch (highnib)
	{
		case 0x00: unimplementedInstruction(); break;
		case 0x01: unimplementedInstruction(); break;
		case 0x02: unimplementedInstruction(); break;
		case 0x03: unimplementedInstruction(); break;
		case 0x04: unimplementedInstruction(); break;
		case 0x05: unimplementedInstruction(); break;
		case 0x06: unimplementedInstruction(); break;
		case 0x07: unimplementedInstruction(); break;
		case 0x08: unimplementedInstruction(); break;
		case 0x09: unimplementedInstruction(); break;
		case 0x0a: unimplementedInstruction(); break;
		case 0x0b: unimplementedInstruction(); break;
		case 0x0c: unimplementedInstruction(); break;
		case 0x0d: unimplementedInstruction(); break;
		case 0x0e: unimplementedInstruction(); break;
		case 0x0f: unimplementedInstruction(); break;
	}
}

void chip8::unimplementedInstruction()
{
	printf("[DEBUG] Unimplemented instruction\n");
}
