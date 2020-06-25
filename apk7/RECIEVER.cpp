
#include <dos.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>

#include "com.c"

#define BASE 0x3f8


int main() {
	char* str;
	while(1) {
		printf("Waiting for sygnal...\n");
		str = com_receive_string(BASE);
		if(strcmp(str,"exit") == 0) {
			printf("Recieved exit sygnal");
			break;
		}
		printf("Recieved string: %s\n", str);
	}
	return 0;
}