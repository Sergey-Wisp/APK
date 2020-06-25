#include <dos.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>

#include "com.c"

#define BASE 0x2f8

int main() {
	char* str;
	while(1){
		printf("Enter string...\n");
		scanf("%s",str);
		com_initializer(BASE);
		com_send_string(str, BASE);
		if(strcmp(str,"exit") == 0) break;
	}
	return 0;
}