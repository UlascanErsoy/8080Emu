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

void load_rom(MEMORY mem, const char* dir){
	
	FILE* f = fopen(dir , "rb");
	if(!f){
		
		emu_error("Couldn't open the file!" , true);
		exit(1);

	}	

	fseek(f , 0 , SEEK_END);
	uint32_t _t;
	if((_t = ftell(f)) > 0xFFFF){
		
		emu_error("File too large! > 0xFFFF bytes!" , true);

	}

	rewind(f);

	fread(mem , sizeof(uint8_t) , _t , f);
	

}
