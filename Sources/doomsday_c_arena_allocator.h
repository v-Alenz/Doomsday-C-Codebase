/******************************************************************************/
/* DoomsDay C                                                                 */
/* Copyright (C) 2025 - v-Alenz                                               */
/*                                                                            */
/* This program is free software: you can redistribute it and/or modify       */
/* it under the terms of the GNU Affero General Public License as published by*/
/* the Free Software Foundation, either version 3 of the License, or          */
/* (at your option) any later version.                                        */
/*                                                                            */
/* This program is distributed in the hope that it will be useful,            */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of             */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              */
/* GNU Affero General Public License for more details.                        */
/*                                                                            */
/* You should have received a copy of the GNU Affero General Public License   */
/* along with this program.  If not, see <http://www.gnu.org/licenses/>.      */
/******************************************************************************/

/* Debug */
#include <stdio.h>
#include <string.h>
#define DOOMSDAY_C_ARENA_ALLOCATOR_IMPLEMENTATION
#define DOOMSDAY_C_ARENA_ALLOCATOR_STRIP_PREFIX


/*****************************************************************************/
/* DOOMSDAY C ARENA ALLOCATOR                                                */
/*****************************************************************************/
#ifndef DOOMSDAY_C_ARENA_ALLOCATOR_H
#define  DOOMSDAY_C_ARENA_ALLOCATOR_H


#include <stdint.h>
#include <stdlib.h>


typedef uint8_t byte;


typedef struct doomsday_c_memory_arena_t {
    uint64_t _memory_size;
    uint64_t _memory_used_size;
    int64_t _last_chunk_id;
    uint64_t _last_chunk_size;
    byte * _memory;
    byte * _base_ptr;
} doom_memory_arena;

typedef struct doomsday_c_memory_arena_chunk_t {
    int64_t * _id;
    uint64_t * _size;
    uint64_t * _prev_size;
    byte * _memory;
} doom_memory_arena_chunk;


int doom_memory_arena_init( doom_memory_arena * arena, void * memory, uint64_t size );
doom_memory_arena doom_memory_arena_new( void * memory, uint64_t size );
int doom_memory_arena_get_chunk_info( doom_memory_arena_chunk * chunk, void * memory );
void * doom_memory_arena_malloc( doom_memory_arena * arena, uint64_t size );
void doom_memory_arena_free( doom_memory_arena * arena, void * memory );

/*****************************************************************************/
/* DOOMSDAY C ARENA ALLOCATOR STRIP PREFIX                                   */
/*****************************************************************************/
#ifdef DOOMSDAY_C_ARENA_ALLOCATOR_STRIP_PREFIX
#endif /* DOOMSDAY_C_ARENA_ALLOCATOR_STRIP_PREFIX */


/*****************************************************************************/
/* DOOMSDAY C ARENA ALLOCATOR IMPLEMENTATION                                 */
/*****************************************************************************/
#ifdef DOOMSDAY_C_ARENA_ALLOCATOR_IMPLEMENTATION
#ifndef DOOMSDAY_C_ARENA_ALLOCATOR_IMPLEMENTATION_BARRIER
#define DOOMSDAY_C_ARENA_ALLOCATOR_IMPLEMENTATION_BARRIER

int doom_memory_arena_init( doom_memory_arena * arena, void * memory, uint64_t size ) {
    if (arena == NULL || memory == NULL || size == 0) {
        return -1; 
    }
    arena->_memory = (byte *)memory;
    arena->_base_ptr = (byte *)memory;
    arena->_last_chunk_id = -1;
    arena->_last_chunk_size = 0;
    arena->_memory_size = size;
    arena->_memory_used_size = 0;

    return 0;
}

doom_memory_arena doom_memory_arena_new( void * memory, uint64_t size ) {
    doom_memory_arena arena;
    memset(&arena, 0, sizeof(doom_memory_arena));
    if (memory == NULL || size == 0) {
        return arena; 
    }
    arena._memory = (byte *)memory;
    arena._base_ptr = (byte *)memory;
    arena._last_chunk_id = -1;
    arena._last_chunk_size = 0;
    arena._memory_size = size;
    arena._memory_used_size = 0;

    return arena;
}

int doom_memory_arena_get_chunk_info( doom_memory_arena_chunk * chunk, void * memory ) {
    if (chunk == NULL || memory == NULL) {
        return -1;
    }
    
    chunk->_id = (int64_t *)(((byte *)memory)-24);
    chunk->_size = (uint64_t *)(((byte *)memory)-16);
    chunk->_prev_size = (uint64_t *)(((byte *)memory)-8);
    chunk->_memory = (byte *)memory;

    return 0;
}

void * doom_memory_arena_malloc( doom_memory_arena * arena, uint64_t size ) {
    if (arena == NULL) {
        return NULL;
    }
    if (size == 0) {
        return NULL;
    }

    if (arena->_memory_size - arena->_memory_used_size < size+32) {
        return NULL;
    }
    arena->_memory_used_size = arena->_memory_used_size + size + 32;

    doom_memory_arena_chunk allocation_chunk;

    allocation_chunk._id = (int64_t *)arena->_base_ptr;
    arena->_base_ptr = arena->_base_ptr + 8;
    allocation_chunk._size = (uint64_t *)arena->_base_ptr;
    arena->_base_ptr = arena->_base_ptr + 8;
    allocation_chunk._prev_size = (uint64_t *)arena->_base_ptr;
    arena->_base_ptr = arena->_base_ptr + 8;
    allocation_chunk._memory = arena->_base_ptr; 
    arena->_base_ptr = arena->_base_ptr + size;

    *allocation_chunk._id = arena->_last_chunk_id + 1;
    *allocation_chunk._size = size;
    *allocation_chunk._prev_size = arena->_last_chunk_size;
    arena->_last_chunk_id = arena->_last_chunk_id + 1;
    arena->_last_chunk_size = size;

    return (void *)allocation_chunk._memory;
}

void doom_memory_arena_free( doom_memory_arena * arena, void * memory ) {
    if (arena == NULL || memory == NULL) {
        return;
    }

    doom_memory_arena_chunk memory_chunk_info;
    doom_memory_arena_get_chunk_info(&memory_chunk_info, memory);

    /* if this chunk is not on top of the heap */
    if (*memory_chunk_info._id != arena->_last_chunk_id) {
        doom_memory_arena_chunk next_chunk_info;
        doom_memory_arena_get_chunk_info(&next_chunk_info, (void *)(((byte *) memory)+*memory_chunk_info._size+24));
        *memory_chunk_info._id = *next_chunk_info._id;
        return;
    }
    /* else */
    arena->_last_chunk_size = *memory_chunk_info._prev_size;
    arena->_last_chunk_id = arena->_last_chunk_id-1;
    arena->_base_ptr = (byte *)memory_chunk_info._id;
    arena->_memory_used_size = arena->_memory_used_size - (*memory_chunk_info._size + 32);

    /* if this was the last chunk in the memory arena we are done*/
    if (arena->_memory_used_size == 0 || arena->_last_chunk_id == -1 || arena->_memory == arena->_base_ptr) {
        return;
    }
    /* else we trace back for any free memory hanging for deletion */
    doom_memory_arena_chunk prev_chunk_info;
    doom_memory_arena_get_chunk_info(&prev_chunk_info, memory_chunk_info._memory - (*memory_chunk_info._prev_size + 24));
    while (*memory_chunk_info._id == *prev_chunk_info._id) {
        arena->_last_chunk_size = *prev_chunk_info._prev_size;
        arena->_last_chunk_id = arena->_last_chunk_id-1;
        arena->_base_ptr = (byte *)prev_chunk_info._id;
        arena->_memory_used_size = arena->_memory_used_size - (*prev_chunk_info._size + 32);
        if (arena->_memory_used_size == 0 || arena->_last_chunk_id == -1 || arena->_memory == arena->_base_ptr) {
            return;
        }
        doom_memory_arena_get_chunk_info(&memory_chunk_info, memory_chunk_info._memory - (*memory_chunk_info._prev_size + 24));
        doom_memory_arena_get_chunk_info(&prev_chunk_info, memory_chunk_info._memory - (*memory_chunk_info._prev_size + 24));
    }
}

#endif /* DOOMSDAY_C_ARENA_ALLOCATOR_IMPLEMENTATION_BARRIER */
#endif /* DOOMSDAY_C_ARENA_ALLOCATOR_IMPLEMENTATION */

#endif /* DOOMSDAY_C_ARENA_ALLOCATOR_H */
