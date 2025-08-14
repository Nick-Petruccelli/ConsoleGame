#pragma once

#include "mylib.h"

typedef struct _terminal_renderer_handel TerminalRendererHandel;

typedef struct KeyState {
	bool32 h;
	bool32 j;
	bool32 k;
	bool32 l;
	bool32 i;
	bool32 q;
	bool32 ctrl_h;
	bool32 ctrl_j;
	bool32 ctrl_k;
	bool32 ctrl_l;
}KeyState;

internal TerminalRendererHandel *terminal_renderer_init(TerminalRendererHandel *terminal_renderer_h, uint32 width, uint32 height);
internal void terminal_renderer_shutdown(TerminalRendererHandel *terminal_renderer_h);
internal uint32 terminal_renderer_load_sprite(TerminalRendererHandel *terminal_renderer_h, char* path);
internal void terminal_renderer_blit_sprite(TerminalRendererHandel *terminal_rederer_h, uint32 sprite_id, uint32 x, uint32 y);
internal void terminal_renderer_clear_window(TerminalRendererHandel *terminal_renderer_h);
internal void terminal_renderer_print_frame(TerminalRendererHandel *terminal_renderer_h);
internal KeyState terminal_renderer_get_key_state();
