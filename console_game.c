#include <stdio.h>
#include <unistd.h>
#include "terminal_renderer.h"
#include "mylib.h"

typedef struct Board {
	uint32 *tile_data;
	uint32 width;
	uint32 height;
	uint32 tile_width;
	uint32 tile_height;
}Board;

typedef struct Hand {
	uint32 *card_id_data;
	uint32 max_cards_in_hand;
}Hand;

void run(){
	TerminalRendererHandel *terminal_renderer_h = NULL;
	terminal_renderer_h = terminal_renderer_init(terminal_renderer_h, 180, 50);

	bool32 running = 1;
	uint32 frame = 0;
	uint32 board_sprite_id = terminal_renderer_load_sprite(terminal_renderer_h, "resources/game_board.txt");
	uint32 card_on_board_sprite_id = terminal_renderer_load_sprite(terminal_renderer_h, "resources/card_on_board.txt");
	uint32 cursor_sprite_id = terminal_renderer_load_sprite(terminal_renderer_h, "resources/circle.txt");

	char input;
	uint32 card_pos_x = 3;
	uint32 card_pos_y = 3;

	bool32 player_turn = 1;
	uint32 cur_cursor_x = 0;
	uint32 cur_cursor_y = 0;
	while(running){
		KeyState key_state = terminal_renderer_get_key_state();
		if(key_state.h){
			cur_cursor_x--;
		}else if(key_state.j){
			cur_cursor_y++;
		}else if(key_state.k){
			cur_cursor_y--;
		}else if(key_state.l){
			cur_cursor_x++;
		}else if(key_state.i){
		}else if(key_state.q){
			break;
		}

		uint32 cursor_screen_pos_x = (cur_cursor_x * 14) + 3;
		uint32 cursor_screen_pos_y = (cur_cursor_y * 9) + 3;
		terminal_renderer_clear_window(terminal_renderer_h);
		terminal_renderer_blit_sprite(terminal_renderer_h, board_sprite_id, 2, 2);
		terminal_renderer_blit_sprite(terminal_renderer_h, card_on_board_sprite_id, card_pos_x, card_pos_y);
		terminal_renderer_blit_sprite(terminal_renderer_h, cursor_sprite_id, cursor_screen_pos_x, cursor_screen_pos_y);
		terminal_renderer_print_frame(terminal_renderer_h);
		frame++;
	}
	terminal_renderer_shutdown(terminal_renderer_h);
}
