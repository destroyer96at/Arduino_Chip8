#ifndef _CHIP_8_H_
#define _CHIP_8_H_

#include<avr/io.h>

struct CHIP8_machine
{
	uint8_t ucRegister[16], ucStackPointer; //Registers
	uint16_t PC, uiStack[16],I;	//Index 16bit Register
	uint16_t (*memory)(uint16_t);
  uint8_t (*dispDriver)(uint8_t, uint8_t, uint8_t);
  void (*cls)(void);
};
//functions
void init_Chip8(struct CHIP8_machine*);
void run_Chip8(struct CHIP8_machine*);

uint16_t getAdress(uint16_t);
uint8_t getXRegister(uint16_t);
uint8_t getYRegister(uint16_t);
uint8_t getKK(uint16_t);
#endif
