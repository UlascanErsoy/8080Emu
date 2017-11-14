#include "emulator.h"


int main(int argc , char** argv){
	
	struct emu_millis_timer* timer = init_timer(timer);

	emu_message(EMU_DETAILED , test_endianness()==1?"Little Endian!":"Big Endian!");
	struct cpu_state* cpu = cpu_init(cpu);

	
/**
	emu_message("Verbose" , EMU_VERBOSE);
	emu_message("Detailed", EMU_DETAILED);
	emu_message("Normal"  , EMU_TRIVIAL);
	emu_warning("Warning Test!" , EMU_VERBOSE);
	emu_error("Non-fatal Error!" , false);
	emu_error("Fatal Error!" , true);
**/
//	write_mem(cpu->mem_unit , 0xF1 , 12);
//	printf("%d\n" , read_mem(cpu->mem_unit , 0/emu_message(EMU_TRIVIAL, getVersion());
 	char* _v = getVersion(_v);
	emu_message(EMU_VERBOSE , _v);
	
	cpu->mem_unit[0] = 0x00;
	cpu->mem_unit[1] = 0xF6;
	cpu->mem_unit[2] = 2;
	cpu->mem_unit[3] = 0xF5;
	
	printf("%d \n" , cpu->program_ptr);
	printf("%d %s\n" , cpu->stack_ptr , mem_check(cpu->mem_unit)==0?"No Memory Error":"Memory Error Occured!");
	

	for(int i = 0 ; i < 0x50 ; i++){
	
		execute(cpu);
		cpu->program_ptr++;
		emu_delay(100);
		}
 	printf("%d \n" , millis_since(timer));
return 0;
}

