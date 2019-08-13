#ifndef _CHIP_8_H_
#define _CHIP_8_H_
struct CHIP8_machine
{
	uint8_t ucRegister[16], ucStackPointer; //Registers
	uint16_t I, ucStack[16];	//Index 16bit Register
}
//functions
void init_Chip8(struct CHIP8_machine*);
void run_Chip8(struct CHIP8_machine*);

uint16_t getAdress(uint16_t);
uint8_t getXRegister(uint16_t);
uint8_t getYRegister(uint16_t);
#endif