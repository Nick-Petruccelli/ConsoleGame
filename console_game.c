#include "terminal_renderer.h"
#include "mylib.h"
#include <stdio.h>
#include "arena.h"

void run(){
	TerminalRendererHandel *terminal_renderer_h;
	terminal_renderer_h = terminal_renderer_init(terminal_renderer_h, 50, 50);

	bool32 running = 1;
	uint32 frame = 0;
	uint32 sprite_id = terminal_renderer_load_sprite(terminal_renderer_h, "resources/card.txt");
	uint32 sprite_id2 = terminal_renderer_load_sprite(terminal_renderer_h, "resources/circle.txt");

	while(running){
		terminal_renderer_clear_window(terminal_renderer_h);
		terminal_renderer_blit_sprite(terminal_renderer_h, sprite_id, frame/10, frame/10);
		terminal_renderer_blit_sprite(terminal_renderer_h, sprite_id2, 40 - (frame/10), 40 - (frame/10));
		terminal_renderer_print_frame(terminal_renderer_h);
		frame++;
		if(frame == 1000)running = 0;
	}
	terminal_renderer_shutdown(terminal_renderer_h);
}
