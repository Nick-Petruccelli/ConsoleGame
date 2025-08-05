#include <stdio.h>
//TODO: Want to find another way to get terminal size without library
#include <sys/ioctl.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "mylib.h"


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

typedef struct {
	Sprite *sprites;
	uint32 sprite_count;
}SpriteContainer;

typedef struct TerminalRendererHandel{
	ScreenBuffer screen_buffer;
	SpriteContainer sprite_container;
}TerminalRendererHandel;

internal void clear_screen(){
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

internal TerminalRendererHandel *terminal_renderer_init(uint32 width, uint32 height){
	TerminalRendererHandel *terminal_renderer_h = malloc(sizeof(TerminalRendererHandel));	
	terminal_renderer_h->screen_buffer.width = width;
	terminal_renderer_h->screen_buffer.height = height;
	terminal_renderer_h->screen_buffer.data = malloc(sizeof(char)*width*height);
	
	terminal_renderer_h->sprite_container.sprites = malloc(sizeof(Sprite)*100);
	terminal_renderer_h->sprite_container.sprite_count = 0;

	clear_screen();
	printf("\e[?25l");
	return terminal_renderer_h;
}

internal uint32 terminal_renderer_load_sprite(TerminalRendererHandel *terminal_renderer_h, char* path){
	FILE *fp = fopen(path, "rb");
	fseek(fp, 0L, SEEK_END);
	uint64 size = ftell(fp);
	rewind(fp);

	char *sprite_txt = calloc(1, size+1);
	fread(sprite_txt, size, 1, fp);

	uint32 newlinecount = 0;
	char *cur_char = sprite_txt;
	uint32 height = 0;
	while(*cur_char != '\0'){
		if(*cur_char == '\n'){
			char *temp = cur_char;
			while(*temp){
				*temp = *(temp+1);
				temp++;
			}
			height++;
			continue;
		}
		cur_char++;
	}

	terminal_renderer_h->sprite_container.sprites[terminal_renderer_h->sprite_container.sprite_count].data = sprite_txt;
	terminal_renderer_h->sprite_container.sprites[terminal_renderer_h->sprite_container.sprite_count].height = height;
	terminal_renderer_h->sprite_container.sprites[terminal_renderer_h->sprite_container.sprite_count].width = (size/height)-1;
	return terminal_renderer_h->sprite_container.sprite_count++;
}

internal void terminal_renderer_blit_sprite(TerminalRendererHandel *terminal_renderer_h, uint32 sprite_id, uint32 x, uint32 y){
	ScreenBuffer screen_buffer = terminal_renderer_h->screen_buffer;
	Sprite sprite = terminal_renderer_h->sprite_container.sprites[sprite_id];
	uint32 screen_row = y;
	uint32 screen_col = x;
	
	char *cur_char = sprite.data;
	for(uint32 sprite_row=0; sprite_row<sprite.height; sprite_row++){
		if(screen_row >= screen_buffer.height){
			break;
		}
		uint32 screen_col = x;
		for(uint32 sprite_col=0; sprite_col<sprite.width; sprite_col++){
			if(screen_col >= screen_buffer.width){
				break;
			}
			screen_buffer.data[screen_row*screen_buffer.width + screen_col] = sprite.data[sprite_row*sprite.width + sprite_col];
			screen_col++;
		}
		screen_row++;
	}
}

internal void terminal_renderer_clear_window(TerminalRendererHandel *terminal_renderer_h){
	ScreenBuffer screen_buffer = terminal_renderer_h->screen_buffer;
	for(uint32 i=0; i<screen_buffer.width*screen_buffer.height; i++){
		screen_buffer.data[i] = '#';
	}
}

internal void terminal_renderer_print_frame(TerminalRendererHandel *terminal_renderer_h){
	ScreenBuffer screen_buffer = terminal_renderer_h->screen_buffer;
	struct winsize term_size;
	ioctl(0, TIOCGWINSZ, &term_size);
	uint16 term_height = term_size.ws_row;
	uint16 term_width = term_size.ws_col;

	for(uint32 row=0; row<term_height; row++){
		printf("\033[F");
	}

	uint32 vertical_padding = (term_height - screen_buffer.height) / 2;
	uint32 horizontal_padding = (term_width - screen_buffer.width) / 2;
	char* empty_row = malloc(sizeof(char)*term_width + 1);
	memset(empty_row, ' ', term_width);
	empty_row[term_width] = '\0';
	for(uint32 row=0; row<vertical_padding-1; row++){
		printf("%s", empty_row);
		printf("\n");
	}


	free(empty_row);
	empty_row = malloc(sizeof(char)*horizontal_padding+1);
	memset(empty_row, ' ', horizontal_padding);
	empty_row[horizontal_padding-1] = '/';
	empty_row[horizontal_padding] = '\0';
	char *top_bot_border = malloc(sizeof(char)*screen_buffer.width+1);
	memset(top_bot_border, '=', screen_buffer.width);
	top_bot_border[screen_buffer.width] = '\0';

	printf("%s", empty_row);
	printf("%s\\\n", top_bot_border);
	empty_row[horizontal_padding-1] = '|';
	for(uint32 row=0; row<screen_buffer.height; row++){
		printf("%s", empty_row);
		for(uint32 col=0; col<screen_buffer.width; col++){
			printf("%c", screen_buffer.data[row*screen_buffer.width + col]);
		}
		printf("|\n");
	}
	

	empty_row[horizontal_padding-1] = '\\';
	printf("%s", empty_row);
	printf("%s/\n", top_bot_border);
	empty_row[horizontal_padding-1] = ' ';
	for(uint32 row=0; row<vertical_padding-1; row++){
		printf("%s", empty_row);
		printf("\n");
	}

	free(empty_row);
	free(top_bot_border);
}

internal void screen_buffer_init(ScreenBuffer *screen_buffer, uint32 width, uint32 height){
	screen_buffer->data = malloc(sizeof(char)*width*height);
	screen_buffer->width = width;
	screen_buffer->height =height;
	for(uint32 row=0; row<screen_buffer->height; row++){
		for(uint32 col=0; col<screen_buffer->width; col++){
			screen_buffer->data[row*screen_buffer->width + col] = ' ';
		}
	}
}

internal void terminal_renderer_shutdown(TerminalRendererHandel *terminal_renderer_h){
	for(uint32 i=0; i<terminal_renderer_h->sprite_container.sprite_count; i++){
		free(terminal_renderer_h->sprite_container.sprites[i].data);
	}
	free(terminal_renderer_h->sprite_container.sprites);
	free(terminal_renderer_h->screen_buffer.data);
	printf("\e[?25h");
}
