#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "terminal_renderer.h"
#include "mylib.h"

typedef struct Board {
	uint32 *tile_data;
	uint32 width;
	uint32 height;
	uint32 tile_width;
	uint32 tile_height;
	uint32 cur_x;
	uint32 cur_y;
}Board;

typedef struct Hand {
	uint32 *card_id_data;
	uint32 max_cards_in_hand;
	int32 selected_card;
}Hand;

typedef struct HandDisplay {
	uint32 width;
	uint32 height;
	uint32 tile_width;
	uint32 tile_height;
	uint32 cur_x;
	uint32 cur_y;
}HandDisplay;

typedef struct GameState {
	Board board;
	HandDisplay hand_display;
	Hand hand;
	uint32 cursor_focus; //NOTE: curently I am only considering focus on board and focus on hand but left as int so I could extend to somthing like settings
}GameState;

//NOTE: cursot_focus defines
#define FOCUS_BOARD 0
#define FOCUS_HAND 1

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
	GameState game_state = {};
	game_state.hand.selected_card = -1;
	game_state.cursor_focus = FOCUS_BOARD;
	game_state.board.width = 7;
	game_state.board.height = 5;
	game_state.board.tile_data = malloc(sizeof(uint32) * game_state.board.width * game_state.board.height);
	while(running){
		KeyState key_state = terminal_renderer_get_key_state();
		if(key_state.q){
			break;
		}
		if(game_state.cursor_focus == FOCUS_BOARD){
			if(key_state.h){
				game_state.board.cur_x--;
			}else if(key_state.j){
				game_state.board.cur_y++;
			}else if(key_state.k){
				game_state.board.cur_y--;
			}else if(key_state.l){
				game_state.board.cur_x++;
			}else if(key_state.ctrl_h){
			}else if(key_state.ctrl_j){
			}else if(key_state.ctrl_k){
			}else if(key_state.ctrl_l){
				game_state.cursor_focus = FOCUS_HAND;
			}else if(key_state.i){
				if(game_state.hand.selected_card != -1)
					game_state.board.tile_data[game_state.board.cur_y * game_state.board.width + game_state.board.cur_x] = game_state.hand.selected_card;
				game_state.hand.selected_card = -1;
			}
		}else if(game_state.cursor_focus == FOCUS_HAND){
			if(key_state.h){
			}else if(key_state.j){
			}else if(key_state.k){
			}else if(key_state.l){
			}else if(key_state.ctrl_h){
				game_state.cursor_focus = FOCUS_BOARD;
			}else if(key_state.ctrl_j){
			}else if(key_state.ctrl_k){
			}else if(key_state.ctrl_l){
			}else if(key_state.i){
				game_state.hand.selected_card = card_on_board_sprite_id;
				game_state.cursor_focus = FOCUS_BOARD;
			}
		}

		
		terminal_renderer_clear_window(terminal_renderer_h);
		terminal_renderer_blit_sprite(terminal_renderer_h, board_sprite_id, 2, 2);
		terminal_renderer_blit_sprite(terminal_renderer_h, card_on_board_sprite_id, card_pos_x, card_pos_y);
		for(int i=0; i<game_state.board.width * game_state.board.height; i++){
			if(game_state.board.tile_data[i] == 0)
				continue;
			uint32 offset_x = (i % game_state.board.width * 14) + 3;
			uint32 offset_y = (i/game_state.board.width * 9) + 3;
			terminal_renderer_blit_sprite(terminal_renderer_h, card_on_board_sprite_id, offset_x, offset_y);
		}
		if(game_state.cursor_focus == FOCUS_BOARD){
			uint32 cursor_screen_pos_x = (game_state.board.cur_x * 14) + 3;
			uint32 cursor_screen_pos_y = (game_state.board.cur_y * 9) + 3;
			terminal_renderer_blit_sprite(terminal_renderer_h, cursor_sprite_id, cursor_screen_pos_x, cursor_screen_pos_y);
		}
		terminal_renderer_print_frame(terminal_renderer_h);
		frame++;
	}
	terminal_renderer_shutdown(terminal_renderer_h);
}
