#ifndef TIMER_H
#define TIMER_H

#include <time.h>
#include "emulator.h"

struct emu_millis_timer{
	clock_t start;
};

struct emu_millis_timer* init_timer(struct emu_millis_timer* t);
uint64_t millis_since(struct emu_millis_timer* t);
void reset(struct emu_millis_timer* t);
void emu_delay(uint64_t millis);

#endif 
