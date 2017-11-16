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
	cpu->mem_unit[1] = 0x3A;
	cpu->mem_unit[2] = 0x3;
	write_mem16(cpu->mem_unit , 3 , 51);
	write_mem16(cpu->mem_unit , 51, 255);
//	printf("%d \n" , read_mem16(cpu->mem_unit , 41));

//	cpu->mem_unit[4] = 0x47;
//	cpu->mem_unit[5] = 0x80;
//	cpu->mem_unit[6] = 0xC3;
//	write_mem16(cpu->mem_unit , 7 , 30);
//	cpu->mem_unit[30] = 0x76;
	
//	printf("%d \n" , cpu->program_ptr);
//	printf("%d %s\n" , cpu->stack_ptr , mem_check(cpu->mem_unit)==0?"No Memory Error":"Memory Error Occured!");
	

	for(int i = 0 ; i < 20 ; i++){
	
		execute(cpu);
		cpu->program_ptr++;
		printf("%d %d\n" , cpu->program_ptr , cpu->A);
		emu_delay(0.1);
		}
	
 	printf("%d %d %d\n" , millis_since(timer) , cpu->A , cpu->program_ptr);
return 0;
}

