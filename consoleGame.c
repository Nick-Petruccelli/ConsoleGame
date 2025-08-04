#include <stdio.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned int uint32;
typedef unsigned long uint64;
typedef unsigned short uint16;
typedef unsigned char uint8;
typedef unsigned char byte;

void clear_screen(){
	struct winsize term_size;
	ioctl(0, TIOCGWINSZ, &term_size);
	uint16 term_height = term_size.ws_row;
	uint16 term_width = term_size.ws_col;
	char* empty_row = malloc(sizeof(char)*term_width + 1);
	memset(empty_row, ' ', term_width);
	empty_row[term_width] = '\0';
	for(int row=0; row<term_height; row++){
		printf("%s", empty_row);
		printf("\n");
	}
	free(empty_row);
}

void print_frame(){
	struct winsize term_size;
	ioctl(0, TIOCGWINSZ, &term_size);
	uint16 term_height = term_size.ws_row;
	uint16 term_width = term_size.ws_col;

	uint32 window_width = 30;
	uint32 window_height = 30;

	for(uint32 row=0; row<term_height; row++){
		printf("\033[F");
	}

	uint32 vertical_padding = (term_height - window_height) / 2;
	uint32 horizontal_padding = (term_width - window_width) / 2;
	char* empty_row = malloc(sizeof(char)*term_width + 1);
	memset(empty_row, ' ', term_width);
	empty_row[term_width] = '\0';
	for(uint32 row=0; row<vertical_padding; row++){
		printf("%s", empty_row);
		printf("\n");
	}

	char *filled_row = malloc(sizeof(char)*window_width + 1);
	free(empty_row);
	memset(filled_row, '#', window_width);
	filled_row[window_width] = '\0';
	empty_row = malloc(sizeof(char)*horizontal_padding+1);
	memset(empty_row, ' ', horizontal_padding);
	empty_row[horizontal_padding] = '\0';
	for(uint32 row=0; row<window_height; row++){
		printf("%s", empty_row);
		printf("%s", filled_row);
		printf("\n");
	}
	
	for(uint32 row=0; row<vertical_padding; row++){
		printf("%s", empty_row);
		printf("\n");
	}
}

int main(int argc, char** argv){
	clear_screen();
	print_frame();
}


