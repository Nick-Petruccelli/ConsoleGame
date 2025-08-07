#pragma once

#include "mylib.h"
#include <stddef.h>

typedef struct {
	byte *buffer;
	size_t buffer_len;
	size_t prev_offset;
	size_t cur_offset;
}Arena;

void arena_init(Arena *arena, void *backing_buffer, size_t buffer_size);

void *arena_alloc(Arena *arena, size_t size);

void *arena_realloc(Arena *arena, void *old_ptr, size_t old_size, size_t size);

void arena_free_all(Arena *arena);
