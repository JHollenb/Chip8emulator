#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define OFFSET 0x200

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
	uint8_t firstnib = (code[0] >> 4);

	printf("%04x %02x %02x ", pc, code[0], code[1]);
	switch (firstnib)
	{
		case 0x0: 
		{
			switch (code[1])
			{
				case 0xe0: printf("%-10s", "CLS"); break;
				case 0xee: printf("%-10s", "RTS"); break;
				default: printf("UNKNOWN 0"); break;
			}
			break;
		}

		// 1xxx, 2xxx, Bxxx ops, jumps and calls
		case 0x1: p_addr("JUMP", code[0]&0xf, code[1]); break;
		case 0x2: p_addr("CALL", code[0]&0xf, code[1]); break;

		// 3xxx, 4xxx, 5xxx, 9xxx ops, compare and skip: These instructions 
		// compare 2 values and skip the next instruciton if the test passes
		case 0x3: p_reg_byte("SE",  code[0]&0xf, code[1]); break;
		case 0x4: p_reg_byte("SNE", code[0]&0xf, code[1]); break;
		case 0x5: p_reg_reg("SE",   code[0]&0xf, code[1]>>4); break;
		case 0x6: p_reg_byte("LD",  code[0]&0xf, code[1]); break;
		case 0x7: p_reg_byte("ADD", code[0]&0xf, code[1]); break;
		// TODO
		case 0x8: printf("Not implemented"); break;
		case 0x9: p_reg_reg("SNE", code[0]&0xf, code[1]>>4); break;
		case 0xa: p_addr("LD I", code[0]&0xf, code[1]); break;
		case 0xb: p_addr("JUMP V0", code[0]&0xf, code[1]); break;
		case 0xc: p_reg_byte("RND", code[0]&0xf, code[1]); break;
		case 0xd: printf("Not implemented"); break;
		case 0xe: 
			switch (code[1])
			{
				case 0x9e: p_reg("SKP",  code[0]&0xf); break;
				case 0xa1: p_reg("SKNP", code[0]&0xf); break;
				default: printf("UNKNOWN E"); break;
			}
			break;
		case 0xf: 
			switch (code[1])
			{
				case 0x07: printf("%-10s V%01X,DELAY", "MOV", code[0]&0xf); break;
				case 0x0a: printf("%-10s V%01X", "KEY", code[0]&0xf); break;
				case 0x15: printf("%-10s DELAY,V%01X", "MOV", code[0]&0xf); break;
				case 0x18: printf("%-10s SOUND,V%01X", "MOV", code[0]&0xf); break;
				case 0x1e: printf("%-10s I,V%01X", "ADI", code[0]&0xf); break;
				case 0x29: printf("%-10s I,V%01X", "SPRITECHAR", code[0]&0xf); break;
				case 0x33: printf("%-10s (I),V%01X", "MOVBCD", code[0]&0xf); break;
				case 0x55: printf("%-10s (I),V0-V%01X", "MOVM", code[0]&0xf); break;
				case 0x65: printf("%-10s V0-V%01X,(I)", "MOVM", code[0]&0xf); break;
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
