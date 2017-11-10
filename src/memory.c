#include "memory.h"


MEMORY mem_init(){

	MEMORY mem = calloc(MEM_SIZE , sizeof(uint8_t));

return mem;
}

uint8_t read_mem(MEMORY mem , uint16_t addr){
	
	return mem[addr];

}

void write_mem(MEMORY mem , uint16_t addr , uint8_t val){

	mem[addr] = val;

}

uint16_t read_mem16(MEMORY mem , uint16_t addr){
	
	return mem[addr] + (mem[addr+1] << 8);

}
void write_mem16(MEMORY mem , uint16_t addr , uint16_t val){
	
	*(mem+addr) = val;	
}
