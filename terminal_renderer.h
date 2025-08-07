#pragma once

#include "mylib.h"

typedef struct _terminal_renderer_handel TerminalRendererHandel;


TerminalRendererHandel *terminal_renderer_init(TerminalRendererHandel *terminal_renderer_h, uint32 width, uint32 height);
void terminal_renderer_shutdown(TerminalRendererHandel *terminal_renderer_h);
uint32 terminal_renderer_load_sprite(TerminalRendererHandel *terminal_renderer_h, char* path);
void terminal_renderer_blit_sprite(TerminalRendererHandel *terminal_rederer_h, uint32 sprite_id, uint32 x, uint32 y);
void terminal_renderer_clear_window(TerminalRendererHandel *terminal_renderer_h);
void terminal_renderer_print_frame(TerminalRendererHandel *terminal_renderer_h);
