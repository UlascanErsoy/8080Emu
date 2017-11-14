#include "timer.h"

void reset(struct emu_millis_timer* t){
	
	t->start = clock();
	
}

struct emu_millis_timer* init_timer(struct emu_millis_timer* t){
	
	t = malloc(sizeof(struct emu_millis_timer));
	t->start = clock();

return t;
}

uint64_t millis_since(struct emu_millis_timer* t){return (clock() - t->start)/(CLOCKS_PER_SEC/1000);}

void emu_delay(double millis){
	
	clock_t s = clock();

	while((clock() - (double)s)/(CLOCKS_PER_SEC/1000) <= millis){}
}


