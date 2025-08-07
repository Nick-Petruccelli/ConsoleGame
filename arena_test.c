#include <stdio.h>
#include <stdlib.h>
#include "arena.h"
#include "arena.c"
#include "mylib.h"


typedef struct Foo {
	Arena arena;
}Foo;

int main(){
	byte *backing_buf = malloc(1028);
	Arena arena = {0};
	arena_init(&arena, backing_buf, 1028);

	printf("is_power_of_two(%d): %b\n", 2, is_power_of_two(2));
	printf("is_power_of_two(%d): %b\n", 4, is_power_of_two(4));
	printf("is_power_of_two(%d): %b\n", 8, is_power_of_two(8));
	printf("is_power_of_two(%d): %b\n", 16, is_power_of_two(16));
	printf("is_power_of_two(%d): %b\n", 32, is_power_of_two(32));
	printf("is_power_of_two(%d): %b\n", 64, is_power_of_two(64));
	printf("is_power_of_two(%d): %b\n", 7, is_power_of_two(7));
	printf("is_power_of_two(%d): %b\n", 37, is_power_of_two(37));
	printf("is_power_of_two(%d): %b\n", 84, is_power_of_two(84));

	printf("\n%ld\n", DEFAULT_ALIGN);


	char *str = arena_alloc(&arena, sizeof(char)*100);

	printf("&backing_buf[0]: %p\n", backing_buf);
	printf("&str[0]: %p\n", str);
	str[0] = 'h'; str[1] = 'e'; str[2] = 'l'; str[3] = 'l'; str[4] = 'o'; str[5] = ' ';
	str[6] = 'w'; str[7] = 'o'; str[8] = 'r'; str[9] = 'l'; str[10] = 'd'; str[11] = '!';
	printf("%s", str);

	Foo *foo;
	foo = malloc(sizeof(Foo));
	byte *foo_back_buf = malloc(256);
	arena_init(&foo->arena, foo_back_buf, 256);
	char *s2 = arena_alloc(&foo->arena, sizeof(char)*100);
	printf("&foo_back_buf[0]: %p\n", foo_back_buf);
	printf("&str2[0]: %p\n", s2);



	free(arena.buffer);
}
