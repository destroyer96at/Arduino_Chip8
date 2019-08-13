#include"chip8.h"

void init_Chip8(struct CHIP8_machine* machine)
{
	machine->PC = 0x200;
	machine->ucStackPointer = 0;
	for(int i = 0; i < 16; i++)
	{
		machine->ucRegister[i] = 0;
	}
}
void run_Chip8(struct CHIP8_machine* machine)
{
	uint16_t uiOpCode, nnn, cache;
	uint8_t x,y,kk;
	uiOpCode = machine->memory(machine->PC);
	x = getXRegister(uiOpCode);
	y = getYRegister(uiOpCode);
	nnn = getAdress(uiOpCode);
	kk = getKK(uiOpCode);
	
	if((uiOpCode & 0xF000) == 0)
	{
		switch(uiOpCode)
		{
			case 0x00E0: //CLS
			//TODO insert clear routine here 130819
        machine->cls();
				machine->PC += 1;
			break;
			case 0x00EE: //RET
				machine->PC = machine->uiStack[machine->ucStackPointer];
				machine->ucStackPointer = machine->ucStackPointer - 1;
			break;
			default:
				machine->PC = uiOpCode & 0x0FFF;
		}	
	}
	else if( (uiOpCode & 0xF000) == 0x1000) //JP
	{
		machine->PC = uiOpCode & 0x0FFF;
	}
	else if( (uiOpCode & 0xF000) == 0x2000) 
	{
		machine->ucStackPointer = machine->ucStackPointer + 1;
		machine->uiStack[machine->ucStackPointer] = machine->PC;
		machine->PC = uiOpCode & 0x0FFF;
	}
	else if( (uiOpCode & 0xF000) == 0x3000) 
	{
		unsigned char regIndex = ((uiOpCode & 0x0F00)>>8);
		cache = (unsigned char)uiOpCode & 0x0FF;
		if(cache == machine->ucRegister[regIndex])
		{
			machine->PC += 2;
		}	
		else 
		{
			machine->PC += 1;
		}
	}
	else if( (uiOpCode & 0xF000) == 0x4000) 
	{
		cache = (unsigned char)uiOpCode & 0x0FF;
		uint16_t regIndex = ((uiOpCode & 0x0F00)>>8);
		if(cache != machine->ucRegister[regIndex])
		{
			machine->PC += 2;
		}	
		else 
		{
			machine->PC += 1;
		}
	}
	else if( (uiOpCode & 0xF000) == 0x5000) 
	{
		if(machine->ucRegister[x] == machine->ucRegister[y])
		{
			machine->PC += 2;
		}
		else 
		{
			machine->PC += 1;
		}
	}
	else if( (uiOpCode & 0xF000) == 0x6000) 
	{
		machine->ucRegister[x] = uiOpCode&0x00FF;
	}
	else if( (uiOpCode & 0xF000) == 0x7000) 
	{
		machine->ucRegister[x] = machine->ucRegister[x] + uiOpCode&0x00FF;
	}
	else if( (uiOpCode & 0xF000) == 0x8000) 
	{
    uint8_t ucSubIndex = uiOpCode & 0xf;
		switch(ucSubIndex)
		{
			case 0x00:
				machine->ucRegister[x] = machine->ucRegister[y];
			break;
			case 0x01:
				machine->ucRegister[x] = machine->ucRegister[x] | machine->ucRegister[y];
			break;
			case 0x02:
				machine->ucRegister[x] = machine->ucRegister[x] & machine->ucRegister[y];
			break;
			case 0x03:
				machine->ucRegister[x] = machine->ucRegister[x] ^ machine->ucRegister[y];
			break;
			case 0x04:
				
				cache = machine->ucRegister[x] + machine->ucRegister[y];
				if(cache > 255) 
				{
					machine->ucRegister[0x0F] = 1;
				}
				machine->ucRegister[x] = (uint8_t)cache;
			break;
			case 0x05:
				if(machine->ucRegister[x] > machine->ucRegister[y])
				{
					machine->ucRegister[0x0F] = 1;
				}
				else
				{
					machine->ucRegister[0x0F] = 0;
				}
				machine->ucRegister[x] = machine->ucRegister[x] - machine->ucRegister[y];
			break;
			case 0x06:
				if(machine->ucRegister[x] & 0x01)
				{
					machine->ucRegister[0x0F] = 1;
				}
				else
				{
					machine->ucRegister[0x0F] = 0;
				}
				machine->ucRegister[x] = (machine->ucRegister[x]>>1);
			break;
			case 0x07:
				if(machine->ucRegister[y] > machine->ucRegister[x])
				{
					machine->ucRegister[0x0F] = 1;
				}
				else
				{
					machine->ucRegister[0x0F] = 0;
				}
				machine->ucRegister[x] = machine->ucRegister[y] - machine->ucRegister[x];
			break;
			case 0x0E:
				if(machine->ucRegister[x] & 0x80)
				{
					machine->ucRegister[0x0F] = 1;
				}
				else
				{
					machine->ucRegister[0x0F] = 0;
				}
				machine->ucRegister[x] = (machine->ucRegister[x]<<1);
			break;
		}
			machine->PC += 1;
	}
	else if( (uiOpCode & 0xF000) == 0x9000) 
	{
		if(machine->ucRegister[x] != machine->ucRegister[y])
		{
			machine->PC += 2;
		}
		else 
		{
			machine->PC += 1;
		}
	}
	else if( (uiOpCode & 0xF000) == 0xA000) 
	{
		machine->PC = nnn;
	}
	else if( (uiOpCode & 0xF000) == 0xB000) 
	{
		machine->PC = machine->ucRegister[0] + nnn;
	}
	else if( (uiOpCode & 0xF000) == 0xC000) 
	{
		uint8_t rnd = 15; //TODO implement rnd-gen
		machine->ucRegister[x] = rnd & kk;
		machine->PC += 1;
	}
	else if( (uiOpCode & 0xF000) == 0xD000) 
	{
		uint8_t dispByte, n = uiOpCode&0xF;
		for(uint8_t i = 0; i < n;i++)
		{
			dispByte = machine->memory(machine->PC + i);
			machine->dispDriver(x, y+i, dispByte); 
		}
		machine->PC += 1;
	}
	else if( (uiOpCode & 0xF000) == 0xE000)  //TODO is this right ??? 130819
	{
		uint8_t status = 0,cmp;
		switch(uiOpCode&0xff)
		{
			case 0x9E:
				cmp = 1;
			break;
			case 0xA1:
				cmp = 0;
			break;
		}
		if(status == cmp) 
		{
			machine->PC += 2;
		}
		else 
		{
			machine->PC += 1;
		}
	}
	else if( (uiOpCode & 0xF000) == 0xF000) 
	{
		
	}	
}
/*Helpers*/
uint16_t getAdress(uint16_t uiOpCode)
{
	return ((uiOpCode & (0x0FFF)));
}
uint8_t getXRegister(uint16_t uiOpCode)
{
	return ((uiOpCode & (0x0F00))>>8);
}
uint8_t getYRegister(uint16_t uiOpCode)
{
	return ((uiOpCode & (0x00F0))>>4);
}
uint8_t getKK(uint16_t uiOpCode)
{
	return (uint8_t)(uiOpCode & (0x00FF));
}
