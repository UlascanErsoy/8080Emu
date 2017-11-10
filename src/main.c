#include "emulator.h"


int main(int argc , char** argv){
	
	emu_message(EMU_DETAILED , test_endianness()==1?"Little Endian!":"Big Endian!");
	cpu_st* cpu = cpu_init();
/**
	emu_message("Verbose" , EMU_VERBOSE);
	emu_message("Detailed", EMU_DETAILED);
	emu_message("Normal"  , EMU_TRIVIAL);
	emu_warning("Warning Test!" , EMU_VERBOSE);
	emu_error("Non-fatal Error!" , false);
	emu_error("Fatal Error!" , true);

	write_mem(cpu->mem_unit , 0xF1 , 12);
	printf("%d\n" , read_mem(cpu->mem_unit , 0xF1));
**/
	emu_message(EMU_TRIVIAL, getVersion());
return 0;
}

