#ifndef CPU_H
#define CPU_H

#include "emulator.h"

enum registers{ S, P , A , B , C , D , E , H , L };
	
struct flags{
	
	/**Flag Bits**/
	uint8_t S : 1;	//Sign (1 == negative)
	uint8_t Z : 1;	//Zero (1 => result is naught)
	uint8_t P : 1;	//Parity(even bits => 1)
	uint8_t C : 1;	//Set if the last addition resulted in a carry or the last subtraction required a borrow
	uint8_t H : 1;	//Auxilary carry
	uint8_t I : 1;  //Interrupt Enable
	uint8_t padding_: 2;
};

struct cpu_state{
	
	/**16 bit pointers**/
	uint16_t stack_ptr;
	uint16_t program_ptr;

	/**8 bit registers**/
	uint8_t A;	//Accumulator
	struct flags* flag;

	uint8_t B;	//B
	uint8_t C;	//C 16 bit pair
	uint8_t D;	//D
	uint8_t E;	//E 16 bit pair
	uint8_t H;	//H
	uint8_t L;	//L 16 bit pair

	uint8_t* mem_unit;
	uint8_t port[8];

}__attribute__((packed));
	
	struct cpu_state* cpu_init();
	unsigned int execute(struct cpu_state* state);
	
#endif
