#include <stdio.h>
//TODO: Want to find another way to get terminal size without library
#include <sys/ioctl.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef unsigned int uint32;
typedef unsigned long uint64;
typedef unsigned short uint16;
typedef unsigned char uint8;
typedef unsigned char byte;

typedef struct {
	char* data;
	uint16 width;
	uint16 height;
}Sprite;

typedef struct {
	char* data;
	uint16 width;
	uint16 height;
}ScreenBuffer;

void clear_screen(){
	struct winsize term_size;
	ioctl(0, TIOCGWINSZ, &term_size);
	uint16 term_height = term_size.ws_row;
	uint16 term_width = term_size.ws_col;
	char* empty_row = malloc(sizeof(char)*term_width + 1);
	memset(empty_row, ' ', term_width);
	empty_row[term_width] = '\0';
	for(int row=0; row<term_height-1; row++){
		printf("%s", empty_row);
		printf("\n");
	}
	free(empty_row);
}

void blit_sprite(ScreenBuffer *screen_buffer, Sprite *sprite, uint32 x, uint32 y){
	uint32 screen_row = y;
	for(uint32 sprite_row=0; sprite_row<sprite->height; sprite_row++){
		uint32 screen_col = x;
		for(uint32 sprite_col=0; sprite_col<sprite->width; sprite_col++){
			screen_buffer->data[screen_row*screen_buffer->width + screen_col] = sprite->data[sprite_row*sprite->width + sprite_col];
			screen_col++;
		}
		screen_row++;
	}
}

void screen_buffer_clear(ScreenBuffer *screen_buffer){
	for(uint32 i=0; i<screen_buffer->width*screen_buffer->height; i++){
		screen_buffer->data[i] = ' ';
	}
}

void print_frame(ScreenBuffer *screen_buffer){
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

	free(empty_row);
	empty_row = malloc(sizeof(char)*horizontal_padding+1);
	memset(empty_row, ' ', horizontal_padding);
	empty_row[horizontal_padding] = '\0';
	for(uint32 row=0; row<window_height; row++){
		printf("%s", empty_row);
		for(uint32 col=0; col<screen_buffer->width; col++){
			printf("%c", screen_buffer->data[row*screen_buffer->width + col]);
		}
		printf("\n");
	}
	
	for(uint32 row=0; row<vertical_padding; row++){
		printf("%s", empty_row);
		printf("\n");
	}
}

void screen_buffer_init(ScreenBuffer *screen_buffer, uint32 width, uint32 height){
	screen_buffer->data = malloc(sizeof(char)*width*height);
	screen_buffer->width = width;
	screen_buffer->height =height;
	for(uint32 row=0; row<screen_buffer->height; row++){
		for(uint32 col=0; col<screen_buffer->width; col++){
			screen_buffer->data[row*screen_buffer->width + col] = ' ';
		}
	}
}

int main(int argc, char** argv){
	printf("\e[?25l");
	char* card = "/==============\\|+3            ||              ||  ___    ___  || /   \\__/   \\ || |          | ||  |        |  ||   \\_    _/   ||     \\__/     ||              ||              ||              ||            +3|\\==============/";
	ScreenBuffer screen_buffer;
	Sprite sprite;
	sprite.width = 16;
	sprite.height = 14;
	sprite.data = malloc(sizeof(char)*sprite.width*sprite.height);
	for(uint32 i=0; i<sprite.height*sprite.width; i++){
		sprite.data[i] = card[i];	
	}
	printf("%s", sprite.data);
	screen_buffer_init(&screen_buffer, 60, 60);


	clear_screen();
	uint32 running = 1;
	uint32 frame = 0;
	while(running){
		screen_buffer_clear(&screen_buffer);
		blit_sprite(&screen_buffer, &sprite, frame/100, frame/100);
		print_frame(&screen_buffer);
		frame++;
	}
}


