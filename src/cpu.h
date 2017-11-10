#ifndef CPU_H
#define CPU_H

#include "common.h"
#include "memory.h"
#include "emulator.h"

	enum registers{ S, P , A , B , C , D , E , H , L };
	
struct flags{
	
	/**Flag Bits**/
	uint8_t S : 1;	//Sign (1 == negative)
	uint8_t Z : 1;	//Zero (1 => result is naught)
	uint8_t P : 1;	//Parity(even bits => 1)
	uint8_t C : 1;	//Set if the last addition resulted in a carry or the last subtraction required a borrow
	uint8_t H : 1;	//Auxilary carry
	uint8_t I : 1;
};

struct cpu_state{
	
	/**16 bit pointers**/
	
	uint16_t stack_ptr;
	uint16_t program_ptr;

	/**8 bit registers**/
	uint8_t A;	//Accumulator
	uint8_t B;	//B
	uint8_t C;	//C 16 bit pair
	uint8_t D;	//D
	uint8_t E;	//E 16 bit pair
	uint8_t H;	//H
	uint8_t L;	//L 16 bit pair

	struct flags* flag;
	MEMORY mem_unit;

}__attribute__((packed));
	
	struct cpu_state* cpu_init();
	unsigned int execute(struct cpu_state* state);
	/**16-bit registers**/
	inline static uint16_t reg_HL(struct cpu_state* state){return state->H + (state->L << 8);}
	inline static uint16_t reg_BC(struct cpu_state* state){return state->B + (state->C << 8);}
	inline static uint16_t reg_DE(struct cpu_state* state){return state->D + (state->E << 8);}
	inline static uint32_t reg_SP(struct cpu_state* state){return state->stack_ptr + (state->program_ptr<<16);}

	inline static void write_16(uint8_t *ptr , uint16_t val){ *ptr = val; }
	
	static void updateFlags(struct cpu_state* state);
	static void DAA(struct cpu_state* state);	
#endif
