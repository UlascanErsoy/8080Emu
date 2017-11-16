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
	
	return ((uint16_t)(mem[addr]<<8))+ (mem[addr+1]);

}
void write_mem16(MEMORY mem , uint16_t addr , uint16_t val){
	
	*(mem+addr)  = (val >> 8) & 0xFF;
	*(mem+addr+1)= (val) & 0xFF; 
		
}

/**Returns the erroneous memory address **/
uint16_t mem_check(MEMORY mem){
	
	for(int i = 0; i < MEM_SIZE && mem[i]==0; i++)
	
		if(i != MEM_SIZE - 1)return i;

return 0;
}
