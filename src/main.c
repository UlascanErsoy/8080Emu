#include "emulator.h"


int main(int argc , char** argv){
	
	struct emu_millis_timer* timer = init_timer(timer);

	emu_message(EMU_DETAILED , test_endianness()==1?"Little Endian!":"Big Endian!");
	struct cpu_state* cpu = cpu_init(cpu);
	
	uint8_t i = 10;
	int8_t a = (int8_t) i;
	printf("%d %c\n" , a,1 - (i << 8)==0?'+':'-');

	load_rom(cpu->mem_unit, "../rom/cpudiag.bin");
	cpu->program_ptr = 0x64;
//	cpu->mem_unit[0] = 0xC3;
//	write_mem16(cpu->mem_unit , 1 , 32000);
//	for(int i = 0; i < 20 ; i++)
//	printf("%.x\n" , read_mem(cpu->mem_unit , 100+i));
	while(true){
	
		execute(cpu);
		printf("\t**stack_ptr: %.x program_ptr: %d Opcode: 0x%.2x\n" , cpu->stack_ptr , cpu->program_ptr , read_mem(cpu->mem_unit , cpu->program_ptr));
		emu_delay(100);
		++cpu->program_ptr;
	}

 	printf("%d %d %d\n" , millis_since(timer) , cpu->A , cpu->program_ptr);
return 0;
}

