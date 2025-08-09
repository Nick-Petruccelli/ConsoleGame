#include <stdio.h>
#include <unistd.h>
#include "terminal_renderer.h"
#include "mylib.h"

void run(){
	TerminalRendererHandel *terminal_renderer_h = NULL;
	terminal_renderer_h = terminal_renderer_init(terminal_renderer_h, 140, 50);

	bool32 running = 1;
	uint32 frame = 0;
	uint32 board_sprite_id = terminal_renderer_load_sprite(terminal_renderer_h, "resources/game_board.txt");
	uint32 card_on_board_sprite_id = terminal_renderer_load_sprite(terminal_renderer_h, "resources/card_on_board.txt");

	char input;
	uint32 card_pos_x = 3;
	uint32 card_pos_y = 3;
	uint32 j_pressed_n_times = 0;
	while(running){
		KeyState key_state = terminal_renderer_get_key_state();
		if(key_state.j){
			j_pressed_n_times++;
		}else if(key_state.q){
			break;
		}
		//terminal_renderer_clear_window(terminal_renderer_h);
		terminal_renderer_blit_sprite(terminal_renderer_h, board_sprite_id, 2, 2);
		terminal_renderer_blit_sprite(terminal_renderer_h, card_on_board_sprite_id, card_pos_x, card_pos_y);
		terminal_renderer_print_frame(terminal_renderer_h);
		frame++;
		if(frame == 1000)running = 0;
	}
	terminal_renderer_shutdown(terminal_renderer_h);
	printf("Frames run: %d\n", frame);
	printf("J pressed n times: %d\n", j_pressed_n_times);
}
