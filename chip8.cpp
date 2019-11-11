#include "chip8.h"

// TODO: Move to header
#define WIDTH 8

chip8::chip8(const char * rom) :
	drawFlag(false),
	m_romPath(rom),
	m_file(NULL),
	m_fsize(0),
	v(),
	i(0),
	pc(0),
	delay(0),
	sound(0),
	memory(),
	screen(),
	sp(0),
	stack(),
	key()
{
}

void chip8::p_addr(const char * call, uint8_t byte0, uint8_t byte1)
{
	printf("%-10s $%01x%02x", call, byte0, byte1);
}

void chip8::p_reg_byte(const char * call, uint8_t byte0, uint8_t byte1)
{
	printf("%-10s V%01X,$%02x", call, byte0, byte1);
}

void chip8::p_reg_reg(const char * call, uint8_t byte0, uint8_t byte1)
{
	printf("%-10s V%01X,V%01X", call, byte0, byte1);
}

void chip8::p_reg(const char * call, uint8_t byte)
{
	printf("%-10s V%01X", call, byte);
}

void chip8::disassemble (uint8_t * code)
{
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
		case 0xd: printf("%-10s V%01X,V%01X,$%01x", "SPRITE", x, y, n); break;
		case 0xe: 
			switch (kk)
			{
				case 0xa2: p_reg("SKP", x); break;
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

void chip8::printDisassembly()
{
	while (pc < (m_fsize + OFFSET))
	{
		disassemble(&memory[pc]);
		pc+=2;
		printf("\n");
	}
}

int chip8::openROM()
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

int chip8::loadROM()
{
	int retval = SUCCESS;
	if (openROM() == SUCCESS)
	{
		fseek(m_file, 0L, SEEK_END);
		m_fsize = ftell(m_file);

		if (m_fsize == 0)
		{
			printf("[WARNING] File size is 0!\n");
			retval = ERROR;
		}

		fseek(m_file, 0L, SEEK_SET);

		// Chip8 puts programs in memory at 0x200
		//unsigned char * buffer = (unsigned char *) malloc (m_fsize + OFFSET);
		fread (memory + OFFSET, m_fsize, 1, m_file);
		fclose (m_file);
	}
	else
	{
		printf("[ERROR] No file found!\n");
		retval = ERROR;
	}
	return retval;
}

void chip8::init()
{
	// TODO: Makesure sizes align
	//memory = (uint8_t *)calloc (1024 * 4, 1);
	//screen = &memory[0xf00];
	//sp = 0xfa0;
	sp = 0;
	pc = OFFSET;

	// TODO: init other arrays to 0
	bzero(&stack, sizeof(stack));

	// load ROM into memory at designated offset
	loadROM();
}

void chip8::loop()
{
	uint16_t opcode = memory[pc] << 8 | memory[pc + 1];
	uint8_t * code = &memory[pc];

	uint8_t x = (code[0] & 0xf);	// (opcode & 0x0F00) >> 8
	uint8_t y = (code[1] >> 4);		// (opcode & 0x00F0) >> 4
	uint8_t n = (code[1] & 0xf);	// (opcode & 0x000F) 
	uint8_t kk = code[1];			// (opcode & 0x00FF)
	uint16_t addr = (x << 8 | kk);

	disassemble(&memory[pc]);
//#define DEBUG
#ifdef DEBUG
	printf(" [DEBUG] x: 0x%X ", x);
	printf("y: 0x%X ", y);
	printf("n: 0x%X ", n);
	printf("kk: 0x%X ", kk);
	printf("addr: 0x%X ", addr);
#endif

	switch (code[0] >> 4)
	{
		case 0x0: 
		{
			switch (kk)
			{
				case 0xe0: unimplementedInstruction(); break;
				case 0xee:
				{
					// RET
					pc = stack[sp];
					--sp;
					break;
				}
				default:
				{
					// TODO "This instruction is ignored by modern interpreters"
					// SYS addr
					pc = addr;
					break;
				}
			}
			break;
		}
		case 0x1:
		{
			// JP addr
			pc = addr;
			break;
		}
		case 0x2: 
		{
			// CALL addr
			stack[sp] = pc;
			sp++;
			pc = addr;
			break;
		}
		case 0x3:
		{
			// SE Vx, Byte
			if (v[x] == kk)
			{
				pc += 2;
			}
			pc += 2;
			break;
		}
		case 0x4: unimplementedInstruction(); break;
		case 0x5: unimplementedInstruction(); break;
		case 0x6: 
		{
			// LD Vx, byte
			v[x] = kk;
			pc += 2;
			break;
		}
		case 0x7:
		{
			// ADD Vx, byte
			v[x] += kk;

			// TODO: No mention of carry flag in this?
			pc += 2;
			break;
		}
		case 0x8: 
		{
			switch (kk & 0x0f)
			{
				case 0x0:
				{
					// LD Vx, Vy
					v[x] = v[y];
					break;
				}
				case 0x1:
				{
					// OR Vx, Vy
					v[x] |= v[y];
					break;
				}
				case 0x2:
				{
					// AND Vx, Vy
					v[x] &= v[y];
					break;
				}
				case 0x3:
				{
					// XOR Vx, Vy
					v[x] ^= v[y];
					break;
				}
				case 0x4: 
				{
					// ADD Vx, Vy
					v[x] += v[y];

					// Set carry flag if result is greater than 8 bits
					v[0xf] = (v[x] + v[y] > 255);
					break;
				}
				case 0x5:
				{
					// SUB Vx, Vy
					v[x] = v[x] - v[y];

					// TODO: Make sure interpretation is correct
					// Set NOT borrow flag
					v[0xf] = (v[x] > v[y]);
					break;
				}
				case 0x6:
				{
					// SHR Vx {, Vy}
					v[0xf] = (v[x] & 0x1);
					v[x] = (v[x] >> 1);
					break;
				}
				case 0x7:
				{
					// SUBN Vx, Vy
					v[x] = v[y] - v[x];

					// Set NOT borrow flag
					v[0xf] = (v[y] > v[x]);
					break;
				}
				case 0xe:
				{
					// SHL Vx {, Vy}
					v[0xf] = (v[x] & 0x80); // TODO: Is 0x80 correct value?
					v[x] = (v[x] << 1);
					break;
				}
				default: printf("UNKNOWN 8"); break;
			}
			printf("HIT PC!\n");
			pc += 2;
			break;
		}
		case 0x9: unimplementedInstruction(); break;
		case 0xa:
		{
			// TODO: Verify this is correct
			i = addr;
			pc += 2;
			break;
		}
		case 0xb: unimplementedInstruction(); break;
		case 0xc: unimplementedInstruction(); break;
		case 0xd:
		{
			// TODO: Can prob move this into a function
			// Get coordinates from v-registers
			uint8_t tmp_x = v[x];
			uint8_t tmp_y = v[y];
			uint8_t height = n;
			uint8_t pixel = 0;

			// v[0xf] is used a test for collisions.
			v[0xf] = 0;
			for (int yline = 0; yline < height; yline++)
			{
				pixel = memory[i + yline];
				for (int xline = 0; xline < WIDTH; xline++)
				{
					// Scan each bit, check for a one
					if ((pixel & (0x80 >> xline)) != 0)
					{
						// If any pixels changed from 1 to 0, set collision flag
						if (screen[(x + xline + ((y + yline) * 64))] == 1)
						{
							v[0xf] = 1;
						}

						// Set pixel via XOR op
						screen[x + xline + ((y + yline) * 64)] ^= 1;
					}
				}
			}

			drawFlag = true;
			pc += 2;
			break;
		}
		case 0xe: unimplementedInstruction(); break;
		case 0xf: unimplementedInstruction(); break;
		default: printf("Unknown opcode: %X\n", code[0] & 0xf);
	}
	printf("\n");
}

void chip8::unimplementedInstruction()
{
	uint16_t opcode = memory[pc] << 8 | memory[pc + 1];
	printf("\t - Unimplemented instruction: %x", opcode);
}
