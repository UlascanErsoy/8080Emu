#include "emulator.h"

/**Little = 1 | Big = 0 **/
uint8_t test_endianness(){volatile uint32_t i=0x01234567; return (*((uint8_t*)(&i))) == 0x67;}

char* int_str(unsigned int num){
	
	static char rep[] = "0123456789ABCDEF";
	static char buf[50];
	char* ptr;

	ptr = &buf[49];
	*ptr = '\0';

	do{
		*--ptr = rep[num%10];
		num/=10;
	}while(num != 0);

	return(ptr);
}

void emu_message(enum verboseness p ,char* str, ...){
								
	enum verboseness c;
	if(p >= (c = DEBUG_MODE)){
		
		fprintf(stdout , "[8080Emulator]:%s\n", str);

	}

}

void emu_error(char* str, bool fatal){
	
	fprintf(stderr , "\x1b[%c;31m[8080Emulator-%sError]:%s\n" , fatal==1?'1':'0',fatal==1?"Fatal ":"" , str);
	
	if(fatal){
		exit(1);
	}
}

void emu_warning(enum verboseness p , char* str){
	
	enum verboseness c;
	if(p >= (c = DEBUG_MODE)){
		
		fprintf(stdout , "\x1b[0;33m[8080Emulator-Warning]:%s\n", str);
		
	}

}

char* getVersion(char* _v){
	
	_v = malloc(sizeof(char) * 8);
	snprintf(_v , 8 ,"%d.%d" , VERSION_MAJOR , VERSION_MINOR);

return _v;
}
