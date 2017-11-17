#ifndef EMULATOR_H
#define EMULATOR_H

#include "common.h"
#include "cpu.h"
#include "memory.h"
#include "timer.h"

#define DEBUG_MODE EMU_VERBOSE/** EMU_DETAILED EMU_TRIVIAL**/

typedef enum _bool{false , true}bool;

enum verboseness{EMU_VERBOSE , EMU_DETAILED , EMU_TRIVIAL};

uint8_t test_endianness();
void emu_message(enum verboseness , char* str);
void emu_warning(enum verboseness p , char* str);
void emu_error(char* str  , bool fatal);
char* getVersion(char* _v);



typedef struct cpu_state cpu_st;

#endif
