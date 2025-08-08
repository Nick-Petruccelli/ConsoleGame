#include <stdio.h>
#include <unistd.h>
#include "terminal_renderer.h"
#include "mylib.h"
#include "arena.h"

void run(){
	TerminalRendererHandel *terminal_renderer_h = NULL;
	terminal_renderer_h = terminal_renderer_init(terminal_renderer_h, 140, 50);

	bool32 running = 1;
	uint32 frame = 0;
	uint32 board_sprite_id = terminal_renderer_load_sprite(terminal_renderer_h, "resources/game_board.txt");
	uint32 card_on_board_sprite_id = terminal_renderer_load_sprite(terminal_renderer_h, "resources/card_on_board.txt");

	char input_buff[100];
	uint32 card_pos_x = 1;
	uint32 card_pos_y = 1;
	while(running){
		size_t bytes_read = read(STDIN_FILENO, input_buff, sizeof(input_buff));
		input_buff[sizeof(input_buff)-1] = '\0';
		if(input_buff[0] == 'j'){
			card_pos_y += 9;
		}
		/*
		*/
		terminal_renderer_clear_window(terminal_renderer_h);
		terminal_renderer_blit_sprite(terminal_renderer_h, board_sprite_id, 2, 2);
		terminal_renderer_blit_sprite(terminal_renderer_h, card_on_board_sprite_id, card_pos_x, card_pos_y);
		terminal_renderer_print_frame(terminal_renderer_h);
		frame++;
		if(frame == 1000)running = 0;
	}
	terminal_renderer_shutdown(terminal_renderer_h);
}
