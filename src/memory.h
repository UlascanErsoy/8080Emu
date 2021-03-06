#ifndef MEMORY_H
#define MEMORY_H

#include "emulator.h"

#define MEM_SIZE 0xFFFF

	typedef uint8_t* MEMORY;
	MEMORY mem_init();
	uint8_t read_mem(MEMORY mem , uint16_t addr);
	uint16_t read_mem16(MEMORY mem, uint16_t addr);
	uint16_t mem_check(MEMORY mem);
	void write_mem(MEMORY mem , uint16_t addr , uint8_t val);
	void write_mem16(MEMORY me, uint16_t addr , uint16_t val);
	void load_rom(MEMORY mem, const char* dir);
#endif 
