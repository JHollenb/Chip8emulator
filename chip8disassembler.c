#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define OFFSET 0x200

// See [Chip 8](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#0.1)

void p_addr(char * call, uint8_t byte0, uint8_t byte1)
{
	printf("%-10s $%01x%02x", call, byte0, byte1);
}

void p_reg_byte(char * call, uint8_t byte0, uint8_t byte1)
{
	printf("%-10s V%01X,#%02x", call, byte0, byte1);
}

void p_reg_reg(char * call, uint8_t byte0, uint8_t byte1)
{
	printf("%-10s V%01X,V%01X", call, byte0, byte1);
}

void p_reg(char * call, uint8_t byte)
{
	printf("%-10s V%01X", call, byte);
}

int disassemble (unsigned char * buffer, int pc)
{
	int retval = 1;
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
	return retval;
}

int main (int argc, char ** argv)
{
	FILE * f = fopen(argv[1], "rb");

	if (!f)
	{
		printf("[ERROR] Failed to open %s\n", argv[1]);
		exit(1);
	}

	fseek(f, 0L, SEEK_END);
	int fsize = ftell(f);
	fseek(f, 0L, SEEK_SET);

	// Chip8 puts programs in memory at 0x200
	unsigned char * buffer = malloc (fsize + OFFSET);
	fread (buffer + OFFSET, fsize, 1, f);
	fclose (f);
	int pc = OFFSET;
	while (pc < (fsize + OFFSET))
	{
		disassemble (buffer, pc);
		pc+=2;
		printf("\n");
	}
	return 0;
}
