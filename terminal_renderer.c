#include <stdio.h>
//TODO: Want to find another way to get terminal size without library
#include <sys/ioctl.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "mylib.h"
#include "arena.h"

#define TERMINAL_RENDERER_ARENA_SIZE 100000 
#define SCRATCH_ARENA_SIZE 1024

global Arena terminal_renderer_arena = {};
global Arena scratch_arena = {};

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

typedef struct _terminal_renderer_handel {
	ScreenBuffer screen_buffer;
	SpriteContainer sprite_container;
}TerminalRendererHandel;

internal void clear_screen(TerminalRendererHandel *terminal_renderer_h){
	struct winsize term_size;
	ioctl(0, TIOCGWINSZ, &term_size);
	uint16 term_height = term_size.ws_row;
	uint16 term_width = term_size.ws_col;
	char* empty_row = arena_alloc(&scratch_arena, sizeof(char)*term_width + 1);
	memset(empty_row, ' ', term_width);
	empty_row[term_width] = '\0';
	for(int row=0; row<term_height-1; row++){
		printf("%s", empty_row);
		printf("\n");
	}
	arena_free_all(&scratch_arena);
}


internal uint32 terminal_renderer_load_sprite(TerminalRendererHandel *terminal_renderer_h, char* path){
	FILE *fp = fopen(path, "rb");
	fseek(fp, 0L, SEEK_END);
	uint64 size = ftell(fp);
	rewind(fp);

	char *sprite_txt = arena_alloc(&terminal_renderer_arena, size+1);
	sprite_txt[size] = 0;
	if(sprite_txt == NULL){
		printf("ERROR: OUT OF MEM\n");
		return 0;
	}
	fread(sprite_txt, size, 1, fp);


	terminal_renderer_h->sprite_container.sprites[terminal_renderer_h->sprite_container.sprite_count].data = sprite_txt;
	return terminal_renderer_h->sprite_container.sprite_count++;
}

internal void terminal_renderer_blit_sprite(TerminalRendererHandel *terminal_renderer_h, uint32 sprite_id, uint32 x, uint32 y){
	ScreenBuffer *screen_buffer = &(terminal_renderer_h->screen_buffer);
	Sprite sprite = terminal_renderer_h->sprite_container.sprites[sprite_id];
	uint32 screen_row = y;
	uint32 screen_col = x;
	
	bool32 writeing = 0;
	char *cur_char = sprite.data;
	while(*cur_char){
		if(screen_row > screen_buffer->height){
			break;
		}
		if(*cur_char == '\n'){
			screen_row++;
			screen_col = x;
			writeing = 0;
			cur_char++;
			continue;
		}
		if(*cur_char != ' '){
			writeing = 1;
		}
		if(!writeing){
			cur_char++;
			screen_col++;
			continue;
		}
		if(screen_col >= screen_buffer->width){
			cur_char++;
			continue;
		}
		screen_buffer->data[screen_row*screen_buffer->width + screen_col] = *cur_char;
		screen_col++;
		cur_char++;
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
	char* empty_row = arena_alloc(&scratch_arena, sizeof(char)*term_width + 1);
	memset(empty_row, ' ', term_width);
	empty_row[term_width] = '\0';
	for(uint32 row=0; row<vertical_padding-1; row++){
		printf("%s", empty_row);
		printf("\n");
	}


	empty_row = arena_alloc(&scratch_arena, sizeof(char)*horizontal_padding+1);
	memset(empty_row, ' ', horizontal_padding);
	empty_row[horizontal_padding-1] = '/';
	empty_row[horizontal_padding] = '\0';
	char *top_bot_border = arena_alloc(&scratch_arena, sizeof(char)*screen_buffer.width+1);
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

	arena_free_all(&scratch_arena);
}

internal void screen_buffer_init(TerminalRendererHandel *terminal_renderer_h, uint32 width, uint32 height){
	terminal_renderer_h->screen_buffer.data = arena_alloc(&terminal_renderer_arena, sizeof(char)*width*height);
	terminal_renderer_h->screen_buffer.width = width;
	terminal_renderer_h->screen_buffer.height =height;
	for(uint32 row=0; row<terminal_renderer_h->screen_buffer.height; row++){
		for(uint32 col=0; col<terminal_renderer_h->screen_buffer.width; col++){
			terminal_renderer_h->screen_buffer.data[row*terminal_renderer_h->screen_buffer.width + col] = ' ';
		}
	}
}

internal TerminalRendererHandel *terminal_renderer_init(TerminalRendererHandel *terminal_renderer_h, uint32 width, uint32 height){
	terminal_renderer_h = malloc(sizeof(TerminalRendererHandel));
	void *terminal_renderer_arena_buffer = malloc(TERMINAL_RENDERER_ARENA_SIZE * sizeof(byte));	
	void *scratch_arena_buffer = malloc(SCRATCH_ARENA_SIZE * sizeof(byte));
	arena_init(&terminal_renderer_arena, terminal_renderer_arena_buffer, TERMINAL_RENDERER_ARENA_SIZE);
	arena_init(&scratch_arena, scratch_arena_buffer, SCRATCH_ARENA_SIZE);
	screen_buffer_init(terminal_renderer_h, width, height);
	
	terminal_renderer_h->sprite_container.sprites = arena_alloc(&terminal_renderer_arena, sizeof(Sprite)*100);
	terminal_renderer_h->sprite_container.sprite_count = 0;

	clear_screen(terminal_renderer_h);
	printf("\e[?25l");
	return terminal_renderer_h;
}

internal void terminal_renderer_shutdown(TerminalRendererHandel *terminal_renderer_h){
	free(terminal_renderer_arena.buffer);
	free(scratch_arena.buffer);
	free(terminal_renderer_h);
	printf("\e[?25h");
}
