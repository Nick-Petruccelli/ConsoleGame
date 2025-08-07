#include "mylib.h"
#include "arena.h"
#include <assert.h>
#include <stddef.h>
#include <string.h>

#ifndef DEFAULT_ALIGN
#define DEFAULT_ALIGN (2*sizeof(void*))
#endif


bool32 is_power_of_two(uintptr x){
	return x & (x-1);
}

void arena_init(Arena *arena, void *backing_buffer, size_t buffer_size){
	arena->buffer = (byte*)backing_buffer;
	return;
	arena->buffer_len = buffer_size;
	arena->cur_offset = 0;
	arena->prev_offset = 0;
}

uintptr align_foward(uintptr ptr, size_t align){
	uintptr a, p, modulo;
	assert(is_power_of_two(align));

	p = ptr;
	a = align;
	modulo = p & (a-1);

	if(modulo){
		p += a - modulo;
	}
	return p;
}

void *arena_alloc_align(Arena *arena, size_t size, size_t align){
	uintptr cur_offset = arena->cur_offset;
	uintptr offset = align_foward(cur_offset+size, align);
	offset -= (uintptr)arena->buffer;

	if(offset+size > (uintptr)arena->buffer + arena->buffer_len){
		return NULL;
	}
	void *ptr = &(arena->buffer[offset]);
	memset(ptr, 0, size);
	arena->prev_offset = offset;
	arena->cur_offset = offset+size;
	return ptr;
}

void *arena_alloc(Arena *arena, size_t size){
	return arena_alloc_align(arena, size, DEFAULT_ALIGN);
}


void arena_free_all(Arena *arena){
	arena->cur_offset = 0;
	arena->prev_offset = 0;
}

void *arena_realloc_align(Arena *arena, void* old_ptr, size_t old_size, size_t size, size_t align){
	if(size <= old_size){
		return old_ptr;
	}
	if((uintptr)old_ptr != arena->prev_offset){
		return arena_alloc(arena, size);
	}
	uintptr offset = align_foward(arena->cur_offset+size, align);
	offset -= (uintptr)arena->buffer;
	if(offset+size > (uintptr)arena->buffer + arena->buffer_len){
		return NULL;
	}
	void *ptr = &(arena->buffer[offset]);
	memset(ptr, 0, size-old_size);
	arena->prev_offset = offset;
	arena->cur_offset = offset+size;
	return ptr;
}
