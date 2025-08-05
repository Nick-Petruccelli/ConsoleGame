#include "terminal_renderer.h"
#include "mylib.h"
#include <stdio.h>

void run(){
	printf("\e[?25l");
	char* card = "/==============\\|+3            ||              ||  ___    ___  || /   \\__/   \\ || |          | ||  |        |  ||   \\_    _/   ||     \\__/     ||              ||              ||              ||            +3|\\==============/";
	TerminalRendererHandel *terminal_renderer_h = terminal_renderer_init(100, 80);


	uint32 running = 1;
	uint32 frame = 0;
	uint32 sprite_id = terminal_renderer_load_sprite(terminal_renderer_h, "card.txt");
	while(frame < 5000){
		terminal_renderer_clear_window(terminal_renderer_h);
		terminal_renderer_blit_sprite(terminal_renderer_h, sprite_id, frame/100, frame/100);
		terminal_renderer_print_frame(terminal_renderer_h);
		frame++;
	}
	terminal_renderer_shutdown(terminal_renderer_h);
}
