#include "cpu.h"
/**16-bit registers**/
inline static uint16_t reg_HL(struct cpu_state* state){return state->H + (state->L << 8);}
inline static uint16_t reg_BC(struct cpu_state* state){return state->B + (state->C << 8);}
inline static uint16_t reg_DE(struct cpu_state* state){return state->D + (state->E << 8);}
inline static uint32_t reg_SP(struct cpu_state* state){return state->stack_ptr + (state->program_ptr<<16);}

inline static void write_16(uint8_t *ptr , uint16_t val){ *ptr = val; }

inline static void JUMP(struct cpu_state* state, uint16_t addr){
	
	state->program_ptr = read_mem(state->mem_unit , addr);
}

/**Slightly Dubious**/
static void CALL(struct cpu_state* state, uint16_t addr){
	
	write_16(state->stack_ptr-2 , state->program_ptr);
	state->stack_ptr-=2;
	state->program_ptr = addr;
}

static void RET(struct cpu_state* state){
	
	state->program_ptr = read_mem16(state , state->stack_ptr);
	state->stack_ptr+=2;
	
}

struct cpu_state* cpu_init(){
	
	struct cpu_state* state = calloc(1 , sizeof(struct cpu_state));
	state->stack_ptr = 0xFFFF;/**Stack_ptr starts at the top**/
	state->mem_unit = mem_init();

return state;

}

static void updateFlags(struct cpu_state* state){
	
	state->flag->S = 1 - (state->A >> 8);
	if(state->A == 0)
		state->flag->Z = 1;
	else
	       	state->flag->Z = 0;
	
	state->flag->P = __builtin_parity(state->A);
	state->flag->H = state->A >> 5;/**Auxiliary Carry Flag**/
	/**Carry is set during the Addition - Subtraction**/

}

static void DAA(struct cpu_state* state){
	
	uint8_t l = state->A && 0xF;
	uint8_t u = state->A >> 4;
	if(l > 9){state->flag->H = 1; l+=6;}else state->flag->H = 0;
	if(u > 9){state->flag->C = 1; u+=6;}else state->flag->C = 0;
	
	state->A = l + (u << 4);
}

unsigned int execute(struct cpu_state* state){

	uint8_t opcode = read_mem(state->mem_unit , state->program_ptr);

	switch(opcode){
		
		/**MOV DEST , SRC**/
		case 0x7F: //MOV A , A 01111111
			emu_message(EMU_VERBOSE , "Executing Mov A , A | A <- A" );
			state->A = state->A;
			break;
		case 0x78: //MOV A , B 01111000 
			emu_message(EMU_VERBOSE , "Executing Mov A , B | A <- B");
			state->A = state->B;
			break;
		case 0x79: //MOV A , C 01111001
			emu_message(EMU_VERBOSE , "Executing Mov A , C | A <- C");
			state->A = state->C;
			break;
		case 0x7A: //MOV A , D 01111010
			emu_message(EMU_VERBOSE , "Executing Mov A , D | A <- D");
			state->A = state->D;
			break;
		case 0x7B: //MOV A , E 01111011
			emu_message(EMU_VERBOSE , "Executing Mov A , E | A <- E");
			state->A = state->E;
			break;
		case 0x7C: //MOV A , H 01111100
			emu_message(EMU_VERBOSE , "Executing Mov A , H | A <- H");
			state->A = state->H;
			break;
		case 0x7D: //MOV A , L 01111101
			emu_message(EMU_VERBOSE , "Executing Mov A , L | A <- L");
			state->A = state->A;
			break;
		case 0x7E: //MOV A , M 01111110
			emu_message(EMU_VERBOSE , "Executing Mov A , M | A <- M");
			state->A = read_mem(state->mem_unit , reg_HL(state));
			break;
		case 0x47: //MOV B , A 01000111
			emu_message(EMU_VERBOSE , "Executing Mov B , A | B <- A");
			state->B = state->A;
			break;
		case 0x40: //MOV B , B 01000000
			emu_message(EMU_VERBOSE , "Executing Mov B , A | B <- B");
			state->B = state->B;
			break;
		case 0x41: //MOV B , C 01000001
			emu_message(EMU_VERBOSE , "Executing Mov B , C | B <- C");
			state->B = state->C;
			break;
		case 0x42: //MOV B , D 01000010
			emu_message(EMU_VERBOSE , "Executing Mov B , D | B <- D");
			state->B = state->D;
			break;
		case 0x43: //MOV B , E 01000011
			emu_message(EMU_VERBOSE , "Executing Mov B , E | B <- E");
			state->B = state->E;
			break;
		case 0x44: //MOV B , H 01000100
			emu_message(EMU_VERBOSE , "Executing Mov B , H | B <- H");
			state->B = state->H;
			break;
		case 0x45: //MOV B , L 01000101
			emu_message(EMU_VERBOSE , "Executing Mov B , L | B <- L");
			state->B = state->L;
			break;
		case 0x46: //MOV B , M 01000110
			emu_message(EMU_VERBOSE , "Executing Mov B , M | B <- M");
			state->B = read_mem(state->mem_unit , reg_HL(state));
			break;
		case 0x4F: //MOV C , A 01001111
			emu_message(EMU_VERBOSE , "Executing Mov C , A | C <- A");
			state->C = state->A;
			break;
		case 0x48: //MOV C , B 01001000
			emu_message(EMU_VERBOSE , "Executing Mov C , B | C <- B");
			state->C = state->B;
			break;
		case 0x49: //MOV C , C 01001001
			emu_message(EMU_VERBOSE , "Executing Mov C , C | C <- C");
			state->C = state->C;
			break;
		case 0x4A: //MOV C , D 01001010
			emu_message(EMU_VERBOSE , "Executing Mov C , D | C <- D");
			state->C = state->D;
			break;
		case 0x4B: //MOV C , E 01001011
			emu_message(EMU_VERBOSE , "Executing Mov C , E | C <- E");
			state->C = state->E;
			break;
		case 0x4C: //MOV C , H 01001100
			emu_message(EMU_VERBOSE , "Executing Mov C , H | C <- H");
			state->C = state->H;
			break;
		case 0x4D: //MOV C , L 01001101
			emu_message(EMU_VERBOSE , "Executing Mov C , L | C <- L");
			state->C = state->L;
			break;
		case 0x4E: //MOV C , M 01001110
			emu_message(EMU_VERBOSE , "Executing Mov C , M | C <- M");
			state->C = read_mem(state->mem_unit , reg_HL(state));
			break;
		case 0x57: //MOV D , A 01010111
			emu_message(EMU_VERBOSE , "Executing Mov D , A | D <- A");
			state->D = state->A;
			break;
		case 0x50: //MOV D , B 01010000
			emu_message(EMU_VERBOSE , "Executing Mov D , B | D <- B");
			state->D = state->B;
			break;
		case 0x51: //MOV D , C 01010001
			emu_message(EMU_VERBOSE , "Executing Mov D , C | D <- C");
			state->D = state->C;
			break;
		case 0x52: //MOV D , D 01010010
			emu_message(EMU_VERBOSE , "Executing Mov D , D | D <- D");
			state->D = state->D;
			break;
		case 0x53: //MOV D , E 01010011
			emu_message(EMU_VERBOSE , "Executing Mov D , E | D <- E");
			state->D = state->E;
			break;
		case 0x54: //MOV D , H 01010100
			emu_message(EMU_VERBOSE , "Executing Mov D , H | D <- H");
			state->D = state->H;
			break;
		case 0x55: //MOV D , L 01010101
			emu_message(EMU_VERBOSE , "Executing Mov D , L | D <- L");
			state->D = state->L;
			break;
		case 0x56: //MOV D , M 01010110
			emu_message(EMU_VERBOSE , "Executing Mov D , M | D <- M");
			state->D = read_mem(state->mem_unit , reg_HL(state));
			break;
		case 0x5F: //MOV E , A 01011111
			emu_message(EMU_VERBOSE , "Executing Mov E , A | E <- A");
			state->E = state->A;
			break;
		case 0x58: //MOV E , B 01011000
			emu_message(EMU_VERBOSE , "Executing Mov E , B | E <- B");
			state->E = state->B;
			break;
		case 0x59: //MOV E , C 01011001
			emu_message(EMU_VERBOSE , "Executing Mov E , C | E <- C");
			state->E = state->C;
			break;
		case 0x5A: //MOV E , D 01011010
			emu_message(EMU_VERBOSE , "Executing Mov E , D | E <- D");
			state->E = state->D;
			break;
		case 0x5B: //MOV E , E 01011011
			emu_message(EMU_VERBOSE , "Executing Mov E , E | E <- E");
			state->E = state->E;
			break;
		case 0x5C: //MOV E , H 01011100
			emu_message(EMU_VERBOSE , "Executing Mov E , H | E <- H");
			state->E = state->H;
			break;
		case 0x5D: //MOV E , L 01011101
			emu_message(EMU_VERBOSE , "Executing Mov E , L | E <- L");
			state->E = state->L;
			break;
		case 0x5E: //MOV E , M 01011110
			emu_message(EMU_VERBOSE , "Executing Mov E , M | E <- M");
			state->E = read_mem(state->mem_unit , reg_HL(state));
			break;
		case 0x67: //MOV H , A 01100111
			emu_message(EMU_VERBOSE , "Executing Mov H , A | H <- A");
			state->H = state->A;
			break;
		case 0x60: //MOV H , B 01100000	
			emu_message(EMU_VERBOSE , "Executing Mov H , B | H <- B");
			state->H = state->B;
			break;
		case 0x61: //MOV H , C 01100001
			emu_message(EMU_VERBOSE , "Executing Mov H , C | H <- C");
			state->H = state->C;
			break;
		case 0x62: //MOV H , D 01100010
			emu_message(EMU_VERBOSE , "Executing Mov H , D | H <- D");
			state->H = state->D;
			break;
		case 0x63: //MOV H , E 01100011
			emu_message(EMU_VERBOSE , "Executing Mov H , E | H <- E");
			state->H = state->E;
			break;
		case 0x64: //MOV H , H 01100100
			emu_message(EMU_VERBOSE , "Executing Mov H , H | H <- H");
			state->H = state->H;
			break;
		case 0x65: //MOV H , L 01100101
			emu_message(EMU_VERBOSE , "Executing Mov H , L | H <- L");
			state->H = state->L;
			break;
		case 0x66: //MOV H , M 01100110
			emu_message(EMU_VERBOSE , "Executing Mov H , M | H <- M");
			state->H = read_mem(state->mem_unit , reg_HL(state));
			break;
		case 0x6F: //MOV L , A 01101111
			emu_message(EMU_VERBOSE , "Executing Mov L , A | L <- A");
			state->L = state->A;
			break;
		case 0x68: //MOV L , B 01101000	
			emu_message(EMU_VERBOSE , "Executing Mov L , B | L <- B");
			state->L = state->B;
			break;
		case 0x69: //MOV L , C 01101001
			emu_message(EMU_VERBOSE , "Executing Mov L , C | L <- C");
			state->L = state->C;
			break;
		case 0x6A: //MOV L , D 01101010
			emu_message(EMU_VERBOSE , "Executing Mov L , D | L <- D");
			state->L = state->D;
			break;
		case 0x6B: //MOV L , E 01101011
			emu_message(EMU_VERBOSE , "Executing Mov L , E | L <- E");
			state->L = state->E;
			break;
		case 0x6C: //MOV L , H 01101100
			emu_message(EMU_VERBOSE , "Executing Mov L , H | L <- H");
			state->L = state->H;
			break;
		case 0x6D: //MOV L , L 01101101
			emu_message(EMU_VERBOSE , "Executing Mov L , L | L <- L");
			state->L = state->L;
			break;
		case 0x6E: //MOV L , M 01101110
			emu_message(EMU_VERBOSE , "Executing Mov L , M | L <- M");
			state->L = read_mem(state->mem_unit , reg_HL(state));
			break;
		case 0x77: //MOV M , A 01110111
			emu_message(EMU_VERBOSE , "Executing Mov M , A | M <- A");
			write_mem(state->mem_unit, reg_HL(state) , state->A);
			break;
		case 0x70: //MOV M , B 01110000
			emu_message(EMU_VERBOSE , "Executing Mov M , B | M <- B");
			write_mem(state->mem_unit, reg_HL(state) , state->B);
			break;
		case 0x71: //MOV M , C 01110001
			emu_message(EMU_VERBOSE , "Executing Mov M , C | M <- C");
			write_mem(state->mem_unit, reg_HL(state) , state->C);
			break;
		case 0x72: //MOV M , D 01110010
			emu_message(EMU_VERBOSE , "Executing Mov M , D | M <- D");
			write_mem(state->mem_unit, reg_HL(state) , state->D);
			break;
		case 0x73: //MOV M , E 01110011
			emu_message(EMU_VERBOSE , "Executing Mov M , E | M <- E");
			write_mem(state->mem_unit, reg_HL(state) , state->E);
			break;
		case 0x74: //MOV M , H 01110100
			emu_message(EMU_VERBOSE , "Executing Mov M , E | M <- E");
			write_mem(state->mem_unit, reg_HL(state) , state->E);
			break;
		case 0x75: //MOV M , L 01110101
			emu_message(EMU_VERBOSE , "Executing Mov M , C | M <- E");
			write_mem(state->mem_unit, reg_HL(state) , state->E);
			break;
		
		case 0x0A: //LDAX B(BC) 
			emu_message(EMU_VERBOSE , "Executing Ldax B(BC) | A <-(BC)");
			state->A = read_mem(state->mem_unit, reg_BC(state));
			break;
		case 0x1A: //LDAX D(DE) 
			emu_message(EMU_VERBOSE , "Executing Ldax D(DE) | A <-(DE)");
			state->A = read_mem(state->mem_unit, reg_DE(state));
			break;
		case 0x3A: //LDA 
			emu_message(EMU_VERBOSE , "Executing Lda | A<-(Word)");
			state->A = read_mem(state->mem_unit, read_mem16(state->mem_unit , ++state->program_ptr));
			++state->program_ptr;
			break;		
		case 0x3E: //MVI A,byte
			emu_message(EMU_VERBOSE , "Executing MVI A,Byte | A<-(byte)");
			state->A = read_mem(state->mem_unit, read_mem(state->mem_unit , ++state->program_ptr));
			break;

		case 0x06: //MVI B,byte
			emu_message(EMU_VERBOSE , "Executing MVI B,Byte | B<-(byte)");
			state->B = read_mem(state->mem_unit, read_mem(state->mem_unit , ++state->program_ptr));
			break;
		case 0x0E: //MVI C,byte
			emu_message(EMU_VERBOSE , "Executing MVI C,Byte | C<-(byte)");
			state->C = read_mem(state->mem_unit, read_mem(state->mem_unit , ++state->program_ptr));
			break;
		case 0x16: //MVI D,byte
			emu_message(EMU_VERBOSE , "Executing MVI D,Byte | D<-(byte)");
			state->D = read_mem(state->mem_unit, read_mem(state->mem_unit , ++state->program_ptr));
			break;
		case 0x1E: //MVI E,byte
			emu_message(EMU_VERBOSE , "Executing MVI E,Byte | E<-(byte)");
			state->E = read_mem(state->mem_unit, read_mem(state->mem_unit , ++state->program_ptr));
			break;
		case 0x26: //MVI H,byte
			emu_message(EMU_VERBOSE , "Executing MVI H,Byte | H<-(byte)");
			state->H = read_mem(state->mem_unit, read_mem(state->mem_unit , ++state->program_ptr));
			break;
		case 0x2E: //MVI L,byte
			emu_message(EMU_VERBOSE , "Executing MVI L,Byte | L<-(byte)");
			state->L = read_mem(state->mem_unit, read_mem(state->mem_unit , ++state->program_ptr));
			break;
		case 0x36: //MVI A,byte
			emu_message(EMU_VERBOSE , "Executing MVI M(HL),Byte | M(HL)<-(byte)");
			write_mem(state->mem_unit , reg_HL(state) , read_mem(state->mem_unit, read_mem(state->mem_unit , ++state->program_ptr)));
			break;
		case 0x02: //STAX B(BC)
			emu_message(EMU_VERBOSE , "Executing STAX B(BC) | (BC) <- A");
			write_mem(state->mem_unit , reg_BC(state) , state->A);
			break;
		case 0x12: //STAX D(DE)
			emu_message(EMU_VERBOSE , "Executing STAX D(DE) | (DE) <- A");
			write_mem(state->mem_unit , reg_DE(state) , state->A);
			break;
		case  0x32: //STA word
			emu_message(EMU_VERBOSE , "Executing STA (word) | (word) <- A");
			write_mem(state->mem_unit , read_mem16(state->mem_unit, ++state->program_ptr) , state->A);
			++state->program_ptr;
			break;
		case 0x01: //LXI BC,word
			emu_message(EMU_VERBOSE , "Executing LXI BC,word | BC <- word");
			write_16(&state->B , read_mem16(state->mem_unit, ++state->program_ptr));
			++state->program_ptr;
			break;
		case 0x11: //LXI DE,word
			emu_message(EMU_VERBOSE , "Executing LXI DE,word | DE <- word");
			write_16(&state->D , read_mem16(state->mem_unit, ++state->program_ptr));
			++state->program_ptr;
			break;
		case 0x21: //LXI HL,word
			emu_message(EMU_VERBOSE , "Executing LXI HL,word | HL <- word");
			write_16(&state->H , read_mem16(state->mem_unit, ++state->program_ptr));
			++state->program_ptr;
			break;
		case 0x31: //LXI SP,word
			emu_message(EMU_VERBOSE , "Executing LXI SP,word | SP <- word");
			write_16(&state->stack_ptr , read_mem16(state->mem_unit, ++state->program_ptr));
			++state->program_ptr;
			break;
		case 0x2A: //LHLD word
			emu_message(EMU_VERBOSE , "Executing LHLD word | HL <- (word)");
			write_16(&state->H , read_mem(state->mem_unit , read_mem16(state->mem_unit, ++state->program_ptr)));
			break;
		case 0x22: //SHLD word
			emu_message(EMU_VERBOSE , "Executing SHLD word | (word) <- HL");
			write_mem16(state->mem_unit , read_mem16(state->mem_unit,++state->program_ptr) , reg_HL(state));
			break;
		case 0xF9: //SPHL
			emu_message(EMU_VERBOSE , "Executing SPHL | SP <- HL");
			write_16(&state->stack_ptr , reg_HL(state));
			break;	
		case 0xEB: //XCHG
			emu_message(EMU_VERBOSE , "Executing XCHG | HL <-> DE");
			uint16_t _tmp = reg_DE(state);
			write_16(&state->D , reg_HL(state));
			write_16(&state->H , _tmp);
			break;
		case 0xE3: //XTHL TOP of the stack swapped with HL
			emu_message(EMU_VERBOSE , "Executing XTHL | SP <-> HL");
			write_mem(state->mem_unit , state->stack_ptr  , state->H);
			write_mem(state->mem_unit , state->stack_ptr+1, state->L);
			break;
		case 0x87: //ADD A
			emu_message(EMU_VERBOSE , "Executing ADD A | A+=A");
			state->flag->C = __builtin_add_overflow(state->A , state->A, &state->A);
			updateFlags(state);
			break;
		case 0x80: //ADD B
			emu_message(EMU_VERBOSE , "Executing ADD B | A+=B");
			state->flag->C = __builtin_add_overflow(state->A , state->B, &state->A);
			updateFlags(state);
			break;
		case 0x81: //ADD C
			emu_message(EMU_VERBOSE , "Executing ADD C | A+=C");
			state->flag->C = __builtin_add_overflow(state->A , state->C, &state->A);
			updateFlags(state);
			break;
		case 0x82: //ADD D
			emu_message(EMU_VERBOSE , "Executing ADD D | A+=D");
			state->flag->C = __builtin_add_overflow(state->A , state->D, &state->A);
			updateFlags(state);
			break;
		case 0x83: //ADD E
			emu_message(EMU_VERBOSE , "Executing ADD E | A+=E");
			state->flag->C = __builtin_add_overflow(state->A , state->E, &state->A);
			updateFlags(state);
			break;
		case 0x84: //ADD H
			emu_message(EMU_VERBOSE , "Executing ADD H | A+=H");
			state->flag->C = __builtin_add_overflow(state->A , state->H, &state->A);
			updateFlags(state);
			break;
		case 0x85: //ADD L
			emu_message(EMU_VERBOSE , "Executing ADD L | A+=L");
			state->flag->C = __builtin_add_overflow(state->A , state->L, &state->A);
			updateFlags(state);
			break;
		case 0x86: //ADD M
			emu_message(EMU_VERBOSE , "Executing ADD (HL) | A+=(HL)");
			state->flag->C = __builtin_add_overflow(state->A , read_mem(state->mem_unit,reg_HL(state)), &state->A);
			updateFlags(state);
			break;
		case 0xC6: //ADI byte
			emu_message(EMU_VERBOSE , "Executing ADI byte | A+=byte");
			state->flag->C = __builtin_add_overflow(state->A , read_mem(state->mem_unit,++state->program_ptr) , &state->A);
			updateFlags(state);
			break;
		case 0x8F: //ADC A
			emu_message(EMU_VERBOSE , "Executing ADC A | A+=A + Carry");
			state->flag->C = __builtin_add_overflow(state->A , state->flag->C , &state->A);	
			state->flag->C = __builtin_add_overflow(state->A , state->A , &state->A);
			updateFlags(state);
			break;
		case 0x88: //ADC B
			emu_message(EMU_VERBOSE , "Executing ADC B | A+=B + Carry");
			state->flag->C = __builtin_add_overflow(state->A , state->flag->C , &state->A);	
			state->flag->C = __builtin_add_overflow(state->A , state->B , &state->A);
			updateFlags(state);
			break;
		case 0x89: //ADC C
			emu_message(EMU_VERBOSE , "Executing ADC C | A+=C + Carry");
			state->flag->C = __builtin_add_overflow(state->A , state->flag->C , &state->A);	
			state->flag->C = __builtin_add_overflow(state->A , state->C , &state->A);
			updateFlags(state);
			break;
		case 0x8A: //ADC D
			emu_message(EMU_VERBOSE , "Executing ADC D | A+=D + Carry");
			state->flag->C = __builtin_add_overflow(state->A , state->flag->C , &state->A);	
			state->flag->C = __builtin_add_overflow(state->A , state->D , &state->A);
			updateFlags(state);
			break;
		case 0x8B: //ADC E
			emu_message(EMU_VERBOSE , "Executing ADC E | A+=E + Carry");
			state->flag->C = __builtin_add_overflow(state->A , state->flag->C , &state->A);	
			state->flag->C = __builtin_add_overflow(state->A , state->E , &state->A);
			updateFlags(state);
			break;
		case 0x8C: //ADC H
			emu_message(EMU_VERBOSE , "Executing ADC H | A+=H + Carry");
			state->flag->C = __builtin_add_overflow(state->A , state->flag->C , &state->A);	
			state->flag->C = __builtin_add_overflow(state->A , state->H , &state->A);
			updateFlags(state);
			break;
		case 0x8D: //ADC L
			emu_message(EMU_VERBOSE , "Executing ADC L | A+=L + Carry");
			state->flag->C = __builtin_add_overflow(state->A , state->flag->C , &state->A);	
			state->flag->C = __builtin_add_overflow(state->A , state->L , &state->A);
			updateFlags(state);
			break;
		case 0x8E: //ADC M(HL)
			emu_message(EMU_VERBOSE , "Executing ADC (HL) | A+=(HL) + Carry");
			state->flag->C = __builtin_add_overflow(state->A , state->flag->C , &state->A);	
			state->flag->C = __builtin_add_overflow(state->A , read_mem(state->mem_unit , reg_HL(state)) , &state->A);
			updateFlags(state);
			break;
		case 0x97: //SUB A
			emu_message(EMU_VERBOSE , "Executing SUB A | A-=A");
			state->flag->C = __builtin_sub_overflow(state->A , state->A , &state->A);	
			updateFlags(state);
			break;
		case 0x90: //SUB B
			emu_message(EMU_VERBOSE , "Executing SUB B | A-=B");
			state->flag->C = __builtin_sub_overflow(state->A , state->B , &state->A);	
			updateFlags(state);
			break;
		case 0x91: //SUB C
			emu_message(EMU_VERBOSE , "Executing SUB C | A-=C");
			state->flag->C = __builtin_sub_overflow(state->A , state->C , &state->A);	
			updateFlags(state);
			break;
		case 0x92: //SUB D
			emu_message(EMU_VERBOSE , "Executing SUB D | A-=D");
			state->flag->C = __builtin_sub_overflow(state->A , state->D , &state->A);	
			updateFlags(state);
			break;
		case 0x93: //SUB E
			emu_message(EMU_VERBOSE , "Executing SUB E | A-=E");
			state->flag->C = __builtin_sub_overflow(state->A , state->E , &state->A);	
			updateFlags(state);
			break;
		case 0x94: //SUB H
			emu_message(EMU_VERBOSE , "Executing SUB H | A-=H");
			state->flag->C = __builtin_sub_overflow(state->A , state->H , &state->A);	
			updateFlags(state);
			break;
		case 0x95: //SUB L
			emu_message(EMU_VERBOSE , "Executing SUB L | A-=L");
			state->flag->C = __builtin_sub_overflow(state->A , state->L , &state->A);	
			updateFlags(state);
			break;
		case 0x96: //SUB M
			emu_message(EMU_VERBOSE , "Executing SUB (HL) | A-=(HL)");
			state->flag->C = __builtin_sub_overflow(state->A , read_mem(state->mem_unit, reg_HL(state)) , &state->A);	
			updateFlags(state);
			break;
		case 0xD6: //SUI byte
			emu_message(EMU_VERBOSE , "Executing SUI byte | A-=byte");
			state->flag->C = __builtin_sub_overflow(state->A , read_mem(state->mem_unit, ++state->program_ptr), &state->A);
			updateFlags(state);
			break;	
		case 0x9F: //SBB A
			emu_message(EMU_VERBOSE , "Executing SBB A | A-=A - Carry");
			state->flag->C = __builtin_sub_overflow(state->A , state->flag->C , &state->A);
			state->flag->C = __builtin_sub_overflow(state->A , state->A , &state->A);
			updateFlags(state);
			break;
		case 0x98: //SBB B
			emu_message(EMU_VERBOSE , "Executing SBB B | A-=B - Carry");
			state->flag->C = __builtin_sub_overflow(state->A , state->flag->C , &state->A);
			state->flag->C = __builtin_sub_overflow(state->A , state->B , &state->A);
			updateFlags(state);
			break;
		case 0x99: //SBB C
			emu_message(EMU_VERBOSE , "Executing SBB C | A-=C - Carry");
			state->flag->C = __builtin_sub_overflow(state->A , state->flag->C , &state->A);
			state->flag->C = __builtin_sub_overflow(state->A , state->C , &state->A);
			updateFlags(state);
			break;
		case 0x9A: //SBB D
			emu_message(EMU_VERBOSE , "Executing SBB D | A-=D - Carry");
			state->flag->C = __builtin_sub_overflow(state->A , state->flag->C , &state->A);
			state->flag->C = __builtin_sub_overflow(state->A , state->D , &state->A);
			updateFlags(state);
			break;
		case 0x9B: //SBB E
			emu_message(EMU_VERBOSE , "Executing SBB E | A-=E - Carry");
			state->flag->C = __builtin_sub_overflow(state->A , state->flag->C , &state->A);
			state->flag->C = __builtin_sub_overflow(state->A , state->E , &state->A);
			updateFlags(state);
			break;
		case 0x9C: //SBB H
			emu_message(EMU_VERBOSE , "Executing SBB H | A-=H - Carry");
			state->flag->C = __builtin_sub_overflow(state->A , state->flag->C , &state->A);
			state->flag->C = __builtin_sub_overflow(state->A , state->H , &state->A);
			updateFlags(state);
			break;
		case 0x9D: //SBB L
			emu_message(EMU_VERBOSE , "Executing SBB L | A-=L - Carry");
			state->flag->C = __builtin_sub_overflow(state->A , state->flag->C , &state->A);
			state->flag->C = __builtin_sub_overflow(state->A , state->L , &state->A);
			updateFlags(state);
			break;
		case 0x9E: //SBB M
			emu_message(EMU_VERBOSE , "Executing SBB (HL) | A-=(HL) - Carry");
			state->flag->C = __builtin_sub_overflow(state->A , state->flag->C , &state->A);
			state->flag->C = __builtin_sub_overflow(state->A , read_mem(state->mem_unit , reg_HL(state)) , &state->A);
			updateFlags(state);
			break;
		case 0xDE: //SBI byte
			emu_message(EMU_VERBOSE , "Executing SBI byte | A-=byte - Carry");
			state->flag->C = __builtin_sub_overflow(state->A , state->flag->C , &state->A);
			state->flag->C = __builtin_sub_overflow(state->A , read_mem(state->mem_unit , ++state->program_ptr) , &state->A);
			updateFlags(state);
			break;
		case 0x09: //DAD B(BC)
			emu_message(EMU_VERBOSE , "Executing DAD BC | HL+=BC");
			state->flag->C = __builtin_sub_overflow(reg_HL(state) , reg_BC(state) , (uint16_t*)&state->H);
			updateFlags(state);
			break;
	
		case 0x19: //DAD D(DE)
			emu_message(EMU_VERBOSE , "Executing DAD DE | HL+=DE");
			state->flag->C = __builtin_sub_overflow(reg_HL(state) , reg_DE(state) , (uint16_t*)&state->H);
			updateFlags(state);
			break;
		case 0x29: //DAD H(HL)
			emu_message(EMU_VERBOSE , "Executing DAD HL | HL+=HL");
			state->flag->C = __builtin_sub_overflow(reg_HL(state) , reg_HL(state) , (uint16_t*)&state->H);
			updateFlags(state);
			break;
		case 0x39: //DAD S(SP)
			emu_message(EMU_VERBOSE , "Executing DAD SP | HL+=SP");
			state->flag->C = __builtin_sub_overflow(reg_HL(state) , state->stack_ptr , (uint16_t*)&state->H);
			updateFlags(state);
			break;
		case 0xF3: //DI
			emu_message(EMU_VERBOSE , "Executing DI | Disable Interrupts");
			state->flag->I = 0;
			break;
		case 0xFB: //EI
			emu_message(EMU_VERBOSE , "Executing EI | Enable Interrupts");
			state->flag->I = 1;
			break;
		case 0x00: //NOP
			break;
		case 0x76:
			emu_message(EMU_TRIVIAL , "Halted!");
			--state->program_ptr;
			break;
		case 0x3C: //INR A
			emu_message(EMU_VERBOSE , "Executing INR A | A++");
			state->flag->C = __builtin_add_overflow(state->A , 1 , &state->A);
			updateFlags(state);
			break;
		case 0x04: //INR B
			emu_message(EMU_VERBOSE , "Executing INR B | B++");
			state->flag->C = __builtin_add_overflow(state->B , 1 , &state->B);
			updateFlags(state);
			break;
		case 0x0C: //INR C
			emu_message(EMU_VERBOSE , "Executing INR C | C++");
			state->flag->C = __builtin_add_overflow(state->C , 1 , &state->C);
			updateFlags(state);
			break;
		case 0x14: //INR D
			emu_message(EMU_VERBOSE , "Executing INR D | D++");
			state->flag->C = __builtin_add_overflow(state->D , 1 , &state->D);
			updateFlags(state);
			break;
		case 0x1C: //INR E
			emu_message(EMU_VERBOSE , "Executing INR E | E++");
			state->flag->C = __builtin_add_overflow(state->D , 1 , &state->D);
			updateFlags(state);
			break;
		case 0x24: //INR H
			emu_message(EMU_VERBOSE , "Executing INR H | H++");
			state->flag->C = __builtin_add_overflow(state->E , 1 , &state->E);
			updateFlags(state);
			break;
		case 0x2C: //INR L
			emu_message(EMU_VERBOSE , "Executing INR L | L++");
			state->flag->C = __builtin_add_overflow(state->L , 1 , &state->L);
			updateFlags(state);
			break;
		case 0x34: //INR (HL)
			emu_message(EMU_VERBOSE , "Executing INR (HL) | (HL)++");
			state->flag->C = __builtin_add_overflow(read_mem(state->mem_unit , reg_HL(state)) , 1 , (uint8_t*)(&state->mem_unit + reg_HL(state)));
			updateFlags(state);
			break;
		case 0x3D: //DCR A
			emu_message(EMU_VERBOSE , "Executing DCR A | A--");
			state->flag->C = __builtin_sub_overflow(state->A , 1 , &state->A);
			updateFlags(state);
			break;
			
		case 0x05: //DCR B 
			emu_message(EMU_VERBOSE , "Executing DCR B | B--");
			state->flag->C = __builtin_sub_overflow(state->B , 1 , &state->B);
			updateFlags(state);
			break;
		case 0x0D: //DCR C
			emu_message(EMU_VERBOSE , "Executing DCR C | C--");
			state->flag->C = __builtin_sub_overflow(state->C , 1 , &state->C);
			updateFlags(state);
			break;
		case 0x15: //DCR D
			emu_message(EMU_VERBOSE , "Executing DCR D | D--");
			state->flag->C = __builtin_sub_overflow(state->D , 1 , &state->D);
			updateFlags(state);
			break;
		case 0x1D: //DCR E
			emu_message(EMU_VERBOSE , "Executing DCR E | E--");
			state->flag->C = __builtin_sub_overflow(state->E , 1 , &state->E);
			updateFlags(state);
			break;
		case 0x25: //DCR H
			emu_message(EMU_VERBOSE , "Executing DCR H | H--");
			state->flag->C = __builtin_sub_overflow(state->H , 1 , &state->H);
			updateFlags(state);
			break;
		case 0x2D: //DCR L
			emu_message(EMU_VERBOSE , "Executing DCR L | L--");
			state->flag->C = __builtin_sub_overflow(state->L , 1 , &state->L);
			updateFlags(state);
			break;
		case 0x35: //DCR M
			emu_message(EMU_VERBOSE , "Executing DCR (HL) | (HL)--");
			state->flag->C = __builtin_sub_overflow(read_mem(state->mem_unit , reg_HL(state)), 1 , (uint8_t*)(&state->mem_unit + reg_HL(state)));
			updateFlags(state);
			break;
		case 0x03: //INX B(BC)
			emu_message(EMU_VERBOSE , "Executing INX B(BC) | BC++");
			state->flag->C = __builtin_add_overflow(reg_BC(state) , 1 , (uint16_t*)state->B);
			updateFlags(state);
			break;
		
		case 0x13: //INX D(DE)
			emu_message(EMU_VERBOSE , "Executing INX D(DE) | DE++");
			state->flag->C = __builtin_add_overflow(reg_DE(state) , 1 , (uint16_t*)state->D);
			updateFlags(state);
			break;
		case 0x23: //INX H(HL)
			emu_message(EMU_VERBOSE , "Executing INX H(HL) | HL++");
			state->flag->C = __builtin_add_overflow(reg_BC(state) , 1 , (uint16_t*)state->H);
			updateFlags(state);
			break;
		case 0x33: //INX SP(stack_ptr)
			emu_message(EMU_VERBOSE , "Executing INX SP | SP++");
			state->flag->C = __builtin_add_overflow(state->stack_ptr , 1 , &state->stack_ptr);
			updateFlags(state);
			break;
		case 0x0B: //DCX B(BC)
			emu_message(EMU_VERBOSE , "Executing DCX B(BC) | BC--");
			state->flag->C = __builtin_sub_overflow(reg_BC(state) , 1 , (uint16_t*)state->B);
			updateFlags(state);
			break;
		
		case 0x1B: //DCX D(DE)
			emu_message(EMU_VERBOSE , "Executing DCX D(DE) | DE--");
			state->flag->C = __builtin_sub_overflow(reg_DE(state) , 1 , (uint16_t*)state->D);
			updateFlags(state);
			break;
		case 0x2B: //INX H(HL)
			emu_message(EMU_VERBOSE , "Executing DCX H(HL) | HL--");
			state->flag->C = __builtin_sub_overflow(reg_BC(state) , 1 , (uint16_t*)state->H);
			updateFlags(state);
			break;
		case 0x3B: //INX SP(stack_ptr)
			emu_message(EMU_VERBOSE , "Executing DCX SP | SP--");
			state->flag->C = __builtin_sub_overflow(state->stack_ptr , 1 , &state->stack_ptr);
			updateFlags(state);
			break;
		case 0x27: //DAA 
			emu_message(EMU_VERBOSE , "Executing DAA | Binary to BCD of A");
			DAA(state);
			break;
		case 0x2F: //CMA A!=A
			emu_message(EMU_VERBOSE , "Executing CMA | A!=A");
			A!=A;
			break;
		case 0x37: //STC CARRY = 1;
			emu_message(EMU_VERBOSE , "Executing STC | Carry=1");
			state->flag->C = 1;
			break;
		case 0x3F: //CMC CARRY=!CARRY
			emu_message(EMU_VERBOSE , "Executing CMC | Carry!=Carry");
			state->flag->C != state->flag->C;
			break;
		case 0x07: //RLC A<<1
			emu_message(EMU_VERBOSE , "Executing RLC | A<<1");
			state->flag->C = state->A >> 7;
			state->A = state->A << 1;
			break;
		case 0x0F: //RRC A>>1
			emu_message(EMU_VERBOSE , "Executing RRC | A>>1");
			state->flag->C = state->A % 2;
			state->A = state->A >> 1;
			break;
		case 0x17: //RAL 
			emu_message(EMU_VERBOSE , "Executing RAL | A<<1");
			state->flag->C = state->A >> 7;
			state->A = state->A << 1;
			break;
		case 0x1F: //RAR
			emu_message(EMU_VERBOSE , "Executing RAR | A>>1");
			state->flag->C = state->A % 2;
			state->A = state->A >> 1;
			break;
		case 0xA7: //ANA A
			emu_message(EMU_VERBOSE , "Executing ANA A | A=A&&A");
			state->A &= state->A;
			break;
		case 0xA0: //ANA B
			emu_message(EMU_VERBOSE , "Executing ANA B | A=B&&A");
			state->A &= state->B;
			break;
		case 0xA1: //ANA C
			emu_message(EMU_VERBOSE , "Executing ANA C | A=C&&A");
			state->A &= state->C;
			break;
		case 0xA2: //ANA D
			emu_message(EMU_VERBOSE , "Executing ANA D | A=D&&A");
			state->A &= state->D;
			break;
		case 0xA3: //ANA E
			emu_message(EMU_VERBOSE , "Executing ANA E | A=E&&A");
			state->A &= state->E;
			break;
		case 0xA4: //ANA H
			emu_message(EMU_VERBOSE , "Executing ANA H | A=H&&A");
			state->A &= state->H;
			break;
		case 0xA5: //ANA L
			emu_message(EMU_VERBOSE , "Executing ANA L | A=L&&A");
			state->A &= state->L;
			break;
		case 0xA6: //ANA M
			emu_message(EMU_VERBOSE , "Executing ANA M | A=(HL)&&A");
			state->A &= read_mem(state->mem_unit , reg_HL(state));
			break;
		case 0xE6: //ANI byte
			emu_message(EMU_VERBOSE , "Executing ANI byte | A=byte&&A");
			state->A &= read_mem(state->mem_unit , ++state->program_ptr);
			break;
		case 0xAF: //XRA A
			emu_message(EMU_VERBOSE , "Executing XRA A | A=A ^ A");
			state->A ^= state->A;
			break;
		case 0xA8: //XRA B
			emu_message(EMU_VERBOSE , "Executing XRA B | A=B ^ A");
			state->A ^= state->B;
			break;
		case 0xA9: //XRA C
			emu_message(EMU_VERBOSE , "Executing XRA C | A=C ^ A");
			state->A ^= state->C;
			break;
		case 0xAA: //XRA D
			emu_message(EMU_VERBOSE , "Executing XRA D | A=D ^ A");
			state->A ^= state->D;
			break;
		case 0xAB: //XRA E
			emu_message(EMU_VERBOSE , "Executing XRA E | A=E ^ A");
			state->A ^= state->E;
			break;
		case 0xAC: //XRA H
			emu_message(EMU_VERBOSE , "Executing XRA H | A=H ^ A");
			state->A ^= state->H;
			break;
		case 0xAD: //XRA L
			emu_message(EMU_VERBOSE , "Executing XRA L | A=L ^ A");
			state->A ^= state->L;
			break;
		case 0xAE: //XRA M
			emu_message(EMU_VERBOSE , "Executing XRA M | A=(HL) ^ A");
			state->A ^= read_mem(state->mem_unit , reg_HL(state));
			break;
		case 0xEE: //XRI byte
			emu_message(EMU_VERBOSE , "Executing XRI byte | A=byte ^ A");
			state->A ^= read_mem(state->mem_unit , ++state->program_ptr);
			break;
		case 0xB7: //ORA A
			emu_message(EMU_VERBOSE , "Executing ORA A | A=A || A");
			state->A |= state->A;
			break;
		case 0xB0: //ORA B
			emu_message(EMU_VERBOSE , "Executing ORA B | A=B || A");
			state->A |= state->B;
			break;
		case 0xB1: //ORA C
			emu_message(EMU_VERBOSE , "Executing ORA C | A=C || A");
			state->A |= state->C;
			break;
		case 0xB2: //ORA D
			emu_message(EMU_VERBOSE , "Executing ORA D | A=D || A");
			state->A |= state->D;
			break;
		case 0xB3: //ORA E
			emu_message(EMU_VERBOSE , "Executing ORA E | A=E || A");
			state->A |= state->E;
			break;
		case 0xB4: //ORA H
			emu_message(EMU_VERBOSE , "Executing ORA H | A=H || A");
			state->A |= state->H;
			break;
		case 0xB5: //ORA L
			emu_message(EMU_VERBOSE , "Executing ORA L | A=L || A");
			state->A |= state->L;
			break;
		case 0xB6: //ORA M
			emu_message(EMU_VERBOSE , "Executing ORA M | A=(HL) || A");
			state->A |= read_mem(state->mem_unit , reg_HL(state));
			break;
		case 0xF6: //ORI byte
			emu_message(EMU_VERBOSE , "Executing ORI byte | A=byte || A");
			state->A |= read_mem(state->mem_unit , ++state->program_ptr);
			break;
		case 0xBF: //CMP A
			emu_message(EMU_VERBOSE , "Executing CMP A | A?=A");
			state->flag->Z = 1;
			break;
		case 0xB8: //CMP B
			emu_message(EMU_VERBOSE , "Executing CMP B | A?=B");
			state->flag->Z = state->B-state->A==0?1:0;
			break;
		case 0xB9: //CMP C
			emu_message(EMU_VERBOSE , "Executing CMP C | A?=C");
			state->flag->Z = state->C-state->A==0?1:0;
			break;
		case 0xBA: //CMP D
			emu_message(EMU_VERBOSE , "Executing CMP D | A?=D");
			state->flag->Z = state->D-state->A==0?1:0;
			break;
		case 0xBB: //CMP E
			emu_message(EMU_VERBOSE , "Executing CMP E | A?=E");
			state->flag->Z = state->E-state->A==0?1:0;
			break;
		case 0xBC: //CMP H
			emu_message(EMU_VERBOSE , "Executing CMP H | A?=H");
			state->flag->Z = state->H-state->A==0?1:0;
			break;
		case 0xBD: //CMP L
			emu_message(EMU_VERBOSE , "Executing CMP L | A?=L");
			state->flag->Z = state->L-state->A==0?1:0;
			break;
		case 0xBE: //CMP M
			emu_message(EMU_VERBOSE , "Executing CMP (HL) | A?=(HL)");
			state->flag->Z = read_mem(state->mem_unit , reg_HL(state))-state->A==0?1:0;
			break;
		case 0xFE: //CPI byte
			emu_message(EMU_VERBOSE , "Executing CPI byte | A?=Byte");
			state->flag->Z = read_mem(state->mem_unit , ++state->program_ptr)-state->A==0?1:0;
			break;
		case 0xC3: //JMP address
			emu_message(EMU_VERBOSE , "Executing JMP addr | program_ptr = address");
			JUMP(state , read_mem16(state->mem_unit , ++state->program_ptr));
			++state->program_ptr;
			break;
		case 0xC2: //JNZ address
			emu_message(EMU_VERBOSE , "Executing JNZ addr | if NZ->program_ptr = address");
			if(!state->flag->Z){
				JUMP(state , read_mem16(state->mem_unit , ++state->program_ptr));
				++state->program_ptr;
				}
			break;
		case 0xCA: //JZ address
			emu_message(EMU_VERBOSE , "Executing JZ addr | if Z->program_ptr = address");
			if(state->flag->Z){
				JUMP(state , read_mem16(state->mem_unit , ++state->program_ptr));
				++state->program_ptr;
				}
			break;
		case 0xD2: //JNC address
			emu_message(EMU_VERBOSE , "Executing JNC addr | if NC->program_ptr = address");
			if(!state->flag->C){
				JUMP(state , read_mem16(state->mem_unit , ++state->program_ptr));
				++state->program_ptr;
				}
			break;
		case 0xDA: //JC  address
			emu_message(EMU_VERBOSE , "Executing JC addr | if C->program_ptr = address");
			if(state->flag->C){
				JUMP(state , read_mem16(state->mem_unit , ++state->program_ptr));
				++state->program_ptr;
				}
			break;
		case 0xE2: //JPO address
			emu_message(EMU_VERBOSE , "Executing JPO addr | if PO->program_ptr = address");
			if(!state->flag->P){
				JUMP(state , read_mem16(state->mem_unit , ++state->program_ptr));
				++state->program_ptr;
				}
			break;
		case 0xEA: //JPE address
			emu_message(EMU_VERBOSE , "Executing JPE addr | if PE->program_ptr = address");
			if(state->flag->P){
				JUMP(state , read_mem16(state->mem_unit , ++state->program_ptr));
				++state->program_ptr;
				}
			break;
		case 0xF2: //JP address
			emu_message(EMU_VERBOSE , "Executing JP addr | if S->program_ptr = address");
			if(!state->flag->S){
				JUMP(state , read_mem16(state->mem_unit , ++state->program_ptr));
				++state->program_ptr;
				}
			break;
		case 0xFA: //JM address
			emu_message(EMU_VERBOSE , "Executing JM addr | if S->program_ptr = address");
			if(state->flag->S){
				JUMP(state , read_mem16(state->mem_unit , ++state->program_ptr));
				++state->program_ptr;
				}
			break;
		case 0xE9: //PCHL
			emu_message(EMU_VERBOSE , "Executing PCHL | PC <- HL");
			state->program_ptr = reg_HL(state);
			break;
		case 0xCD: //CALL address
			emu_message(EMU_VERBOSE , "Executing CALL addr | program_ptr = address");
			CALL(state , read_mem16(state->mem_unit , ++state->program_ptr));
			++state->program_ptr;
			break;
		case 0xC4: //CNZ address
			emu_message(EMU_VERBOSE , "Executing CNZ addr | if NZ->program_ptr = address");
			if(!state->flag->Z){
				CALL(state , read_mem16(state->mem_unit , ++state->program_ptr));
				++state->program_ptr;
				}
			break;
		case 0xCC: //CZ address
			emu_message(EMU_VERBOSE , "Executing CZ addr | if Z->program_ptr = address");
			if(state->flag->Z){
				CALL(state , read_mem16(state->mem_unit , ++state->program_ptr));
				++state->program_ptr;
				}
			break;
		case 0xD4: //CNC address
			emu_message(EMU_VERBOSE , "Executing CNC addr | if NC->program_ptr = address");
			if(!state->flag->C){
				CALL(state , read_mem16(state->mem_unit , ++state->program_ptr));
				++state->program_ptr;
				}
			break;
		case 0xDC: //CC  address
			emu_message(EMU_VERBOSE , "Executing CC addr | if C->program_ptr = address");
			if(state->flag->C){
				CALL(state , read_mem16(state->mem_unit , ++state->program_ptr));
				++state->program_ptr;
				}
			break;
		case 0xE4: //CPO address
			emu_message(EMU_VERBOSE , "Executing CPO addr | if PO->program_ptr = address");
			if(!state->flag->P){
				CALL(state , read_mem16(state->mem_unit , ++state->program_ptr));
				++state->program_ptr;
				}
			break;
		case 0xEC: //CPE address
			emu_message(EMU_VERBOSE , "Executing CPE addr | if PE->program_ptr = address");
			if(state->flag->P){
				CALL(state , read_mem16(state->mem_unit , ++state->program_ptr));
				++state->program_ptr;
				}
			break;
		case 0xF4: //CP address
			emu_message(EMU_VERBOSE , "Executing CP addr | if S->program_ptr = address");
			if(!state->flag->S){
				JUMP(state , read_mem16(state->mem_unit , ++state->program_ptr));
				++state->program_ptr;
				}
			break;
		case 0xFC: //CM address
			emu_message(EMU_VERBOSE , "Executing CM addr | if S->program_ptr = address");
			if(state->flag->S){
				CALL(state , read_mem16(state->mem_unit , ++state->program_ptr));
				++state->program_ptr;
				}
			break;
		case 0xC9: //RET
			emu_message(EMU_VERBOSE , "Executing RET | RETURN from subroutine");
			RET(state);
			break;	
		case 0xC0: //RNZ
			emu_message(EMU_VERBOSE , "Executing RNZ | RETURN from subroutine if NZ");
			if(!state->flag->Z)RET(state);
			break;
		case 0xC8: //RZ
			emu_message(EMU_VERBOSE , "Executing RZ | RETURN from subroutine if Z");
			if(state->flag->Z)RET(state);
			break;
		case 0xD0: //RNC
			emu_message(EMU_VERBOSE , "Executing RNC | RETURN from subroutine if NC");
			if(!state->flag->C)RET(state);
			break;
		case 0xD8: //RC
			emu_message(EMU_VERBOSE , "Executing RC | RETURN from subroutine if C");
			if(state->flag->C)RET(state);
			break;
		case 0xE0: //RPO
			emu_message(EMU_VERBOSE , "Executing RPO | RETURN from subroutine if PO");
			if(!state->flag->P)RET(state);
			break;
		case 0xE8: //RPE
			emu_message(EMU_VERBOSE , "Executing RPE | RETURN from subroutine if PE");
			if(state->flag->P)RET(state);
			break;
		case 0xF0: //RP
			emu_message(EMU_VERBOSE , "Executing RP | RETURN from subroutine if P(ositive)");
			if(!state->flag->S)RET(state);
			break;
		case 0xF8: //RM
			emu_message(EMU_VERBOSE , "Executing RM | RETURN from subroutine if M(inus)");
			if(state->flag->S)RET(state);
			break;
		case 0xC7: //RST 0
			emu_message(EMU_VERBOSE , "Executing RST 0");
			CALL(state , 0 * 8);
			break;
		case 0xCF: //RST 8
			emu_message(EMU_VERBOSE , "Executing RST 8");
			CALL(state , 1 * 8);
			break;
		case 0xD7: //RST 16
			emu_message(EMU_VERBOSE , "Executing RST 16");
			CALL(state , 2 * 8);
			break;
		case 0xDF: //RST 24
			emu_message(EMU_VERBOSE , "Executing RST 24");
			CALL(state , 3 * 8);
			break;
		case 0xE7: //RST 32
			emu_message(EMU_VERBOSE , "Executing RST 32");
			CALL(state , 4 * 8);
			break;
		case 0xEF: //RST 40
			emu_message(EMU_VERBOSE , "Executing RST 40");
			CALL(state , 5 * 8);
			break;
		case 0xF7: //RST 48
			emu_message(EMU_VERBOSE , "Executing RST 48");
			CALL(state , 6 * 8);
			break;
		case 0xFF: //RST 56
			emu_message(EMU_VERBOSE , "Executing RST 56");
			CALL(state , 7 * 8);
			break;
		
		
		default:
			emu_error("Execution Error!",true);

	}	
	
	++state->program_ptr;

return 0;
}

