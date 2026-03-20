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

#ifndef DOOMSDAY_C
#define DOOMSDAY_C


#include <alloca.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>


/*****************************************************************************/
/* DOOMSDAY C DEFINITIONS                                                    */
/*****************************************************************************/

#define DOOMSDAY_C_IMPLEMENTATION
#define DOOMSDAY_C_STRIP_PREFIX
#define DOOMSDAY_C_ENABLE_COMPILER_MAGIC

#ifdef DOOMSDAY_C_DEFAULT_ALLOCATOR
#ifndef DOOMSDAY_C_DEFAULT_DEALLOCATOR
#error "Custom default allocator without deallocator provided"
#endif
#else
#define DOOMSDAY_C_DEFAULT_ALLOCATOR malloc
#define DOOMSDAY_C_DEFAULT_DEALLOCATOR free
#endif /* DOOM_STRING_DEALLOCATOR */

typedef uint8_t byte;

/*****************************************************************************/
/* DOOMSDAY C ERROR                                                          */
/*****************************************************************************/
typedef enum doomsday_c_errno_t {
    DOOM_SUCCESS = 0,
    DOOM_OUT_OF_MEMORY = -1,
    DOOM_ARGUMENT_IS_NULL = -2,
    DOOM_INVALID_ARGUMENT = -3,
} doom_errno;


/*****************************************************************************/
/* DOOMSDAY C ARENA ALLOCATOR                                                */
/*****************************************************************************/

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
#ifdef DOOMSDAY_C_STRIP_PREFIX

#define memory_arena_init           doom_memory_arena_init
#define memory_arena_new            doom_memory_arena_new
#define memory_arena_get_chunk_info doom_memory_arena_get_chunk_info
#define memory_arean_malloc         doom_memory_arena_malloc
#define memory_arena_free           doom_memory_arena_free

#endif /* DOOMSDAY_C_STRIP_PREFIX */

/*****************************************************************************/
/* DOOMSDAY C ARENA ALLOCATOR IMPLEMENTATION                                 */
/*****************************************************************************/
#ifdef DOOMSDAY_C_IMPLEMENTATION

int doom_memory_arena_init( doom_memory_arena * arena, void * memory, uint64_t size ) {
    if (arena == NULL || memory == NULL) {
        return DOOM_ARGUMENT_IS_NULL; 
    }
    if (size == 0) {
        return DOOM_INVALID_ARGUMENT;
    }
    arena->_memory = (byte *)memory;
    arena->_base_ptr = (byte *)memory;
    arena->_last_chunk_id = -1;
    arena->_last_chunk_size = 0;
    arena->_memory_size = size;
    arena->_memory_used_size = 0;

    return DOOM_SUCCESS;
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

#endif /* DOOMSDAY_C_IMPLEMENTATION */


/*****************************************************************************/
/* DOOMSDAY C THREAD                                                         */
/*****************************************************************************/

typedef pthread_t doom_thread;

int doom_thread_create( doom_thread * restrict newthread, void *(*start_routine)(void *), void *restrict arg );
int doom_thread_create_detached( doom_thread * restrict newthread, void *(*start_routine)(void *), void *restrict arg );
doom_thread * doom_thread_new( void *(*start_routine)(void *), void *restrict arg );
doom_thread * doom_thread_new_detached( void *(*start_routine)(void *), void *restrict arg );
void * doom_thread_join( doom_thread * thread );
int doom_thread_detach( doom_thread * thread );
void doom_thread_deinit( void * result );
void doom_thread_result_deinit( void * result );

/*****************************************************************************/
/* DOOMSDAY C THREAD MACROS                                                  */
/*****************************************************************************/

/* NOTE: I DON'T LIKE THIS, but i can't figure out a bether way, for now */
#ifndef DOOM_THREAD_ALLOCATOR
    #ifdef DOOMSDAY_C_DEFAULT_ALLOCATOR
        #define DOOM_THREAD_ALLOCATOR(x) DOOMSDAY_C_DEFAULT_ALLOCATOR(x)
    #else
        #define DOOM_THREAD_ALLOCATOR(x) malloc(x)
    #endif /* DOOMSDAY_C_DEFAULT_ALLOCATOR */
#endif /* DOOM_THREAD_ALLOCATOR */

#ifndef DOOM_THREAD_DEALLOCATOR
    #ifdef DOOMSDAY_C_DEFAULT_DEALLOCATOR
        #define DOOM_THREAD_DEALLOCATOR(x) DOOMSDAY_C_DEFAULT_DEALLOCATOR(x)
    #else
        #define DOOM_THREAD_DEALLOCATOR(x) free(x)
    #endif /* DOOMSDAY_C_DEFAULT_DEALLOCATOR */
#endif /* DOOM_THREAD_DEALLOCATOR */

/* Auxillitary Macros */
#define DOOM_THREAD_FUNCNAME( fname )                                       \
fname##_doom_thread_wrapper

#define DOOM_THREAD_FUNCARGS( fname, args )                                 \
fname##_doom_thread_args_pack args

/* Procedure Wrapping Macros */
#define DOOM_THREAD_PROCEDURE_WRAP( fname )                                 \
void * fname##_doom_thread_wrapper( void * args ) {                         \
    fname();                                                                \
    return NULL;                                                            \
}  

#define DOOM_THREAD_PROCEDURE_WRAP_1( fname, t_0 )                          \
struct fname##_doom_thread_args_decoder {                                   \
    t_0 arg0;                                                               \
};                                                                          \
struct fname##_doom_thread_args_decoder *                                   \
fname##_doom_thread_args_pack(                                              \
        t_0 arg0                                                            \
        ) {                                                                 \
    struct fname##_doom_thread_args_decoder * str =                         \
        (struct fname##_doom_thread_args_decoder *)DOOM_THREAD_ALLOCATOR(   \
                sizeof(struct fname##_doom_thread_args_decoder)             \
                );                                                          \
    str->arg0 = arg0;                                                       \
    return str;                                                             \
}                                                                           \
void * fname##_doom_thread_wrapper( void * args ) {                         \
    struct fname##_doom_thread_args_decoder * f_args =                      \
        (struct fname##_doom_thread_args_decoder *) args;                   \
    fname(                                                                  \
            f_args->arg0                                                    \
         );                                                                 \
    DOOM_THREAD_DEALLOCATOR(args);                                          \
    return NULL;                                                            \
} 

#define DOOM_THREAD_PROCEDURE_WRAP_2( fname, t_0, t_1 )                     \
struct fname##_doom_thread_args_decoder {                                   \
    t_0 arg0;                                                               \
    t_1 arg1;                                                               \
};                                                                          \
struct fname##_doom_thread_args_decoder *                                   \
fname##_doom_thread_args_pack(                                              \
        t_0 arg0,                                                           \
        t_1 arg1                                                            \
        ) {                                                                 \
    struct fname##_doom_thread_args_decoder * str =                         \
        (struct fname##_doom_thread_args_decoder *)DOOM_THREAD_ALLOCATOR(   \
                sizeof(struct fname##_doom_thread_args_decoder)             \
                );                                                          \
    str->arg0 = arg0;                                                       \
    str->arg1 = arg1;                                                       \
    return str;                                                             \
}                                                                           \
void * fname##_doom_thread_wrapper( void * args ) {                         \
    struct fname##_doom_thread_args_decoder * f_args =                      \
        (struct fname##_doom_thread_args_decoder *) args;                   \
    fname(                                                                  \
            f_args->arg0,                                                   \
            f_args->arg1                                                    \
         );                                                                 \
    DOOM_THREAD_DEALLOCATOR(args);                                          \
    return NULL;                                                            \
} 

#define DOOM_THREAD_PROCEDURE_WRAP_3( fname, t_0, t_1, t_2 )                \
struct fname##_doom_thread_args_decoder {                                   \
    t_0 arg0;                                                               \
    t_1 arg1;                                                               \
    t_2 arg2;                                                               \
};                                                                          \
struct fname##_doom_thread_args_decoder *                                   \
fname##_doom_thread_args_pack(                                              \
        t_0 arg0,                                                           \
        t_1 arg1,                                                           \
        t_2 arg2                                                            \
        ) {                                                                 \
    struct fname##_doom_thread_args_decoder * str =                         \
        (struct fname##_doom_thread_args_decoder *)DOOM_THREAD_ALLOCATOR(   \
                sizeof(struct fname##_doom_thread_args_decoder)             \
                );                                                          \
    str->arg0 = arg0;                                                       \
    str->arg1 = arg1;                                                       \
    str->arg2 = arg2;                                                       \
    return str;                                                             \
}                                                                           \
void * fname##_doom_thread_wrapper( void * args ) {                         \
    struct fname##_doom_thread_args_decoder * f_args =                      \
        (struct fname##_doom_thread_args_decoder *) args;                   \
    fname(                                                                  \
            f_args->arg0,                                                   \
            f_args->arg1,                                                   \
            f_args->arg2                                                    \
         );                                                                 \
    DOOM_THREAD_DEALLOCATOR(args);                                          \
    return NULL;                                                            \
} 

#define DOOM_THREAD_PROCEDURE_WRAP_4( fname, t_0, t_1, t_2, t_3 )           \
struct fname##_doom_thread_args_decoder {                                   \
    t_0 arg0;                                                               \
    t_1 arg1;                                                               \
    t_2 arg2;                                                               \
    t_3 arg3;                                                               \
};                                                                          \
struct fname##_doom_thread_args_decoder *                                   \
fname##_doom_thread_args_pack(                                              \
        t_0 arg0,                                                           \
        t_1 arg1,                                                           \
        t_2 arg2,                                                           \
        t_3 arg3                                                            \
        ) {                                                                 \
    struct fname##_doom_thread_args_decoder * str =                         \
        (struct fname##_doom_thread_args_decoder *)DOOM_THREAD_ALLOCATOR(   \
                sizeof(struct fname##_doom_thread_args_decoder)             \
                );                                                          \
    str->arg0 = arg0;                                                       \
    str->arg1 = arg1;                                                       \
    str->arg2 = arg2;                                                       \
    str->arg3 = arg3;                                                       \
    return str;                                                             \
}                                                                           \
void * fname##_doom_thread_wrapper( void * args ) {                         \
    struct fname##_doom_thread_args_decoder * f_args =                      \
        (struct fname##_doom_thread_args_decoder *) args;                   \
    fname(                                                                  \
            f_args->arg0,                                                   \
            f_args->arg1,                                                   \
            f_args->arg2,                                                   \
            f_args->arg3                                                    \
         );                                                                 \
    DOOM_THREAD_DEALLOCATOR(args);                                          \
    return NULL;                                                            \
} 

#define DOOM_THREAD_PROCEDURE_WRAP_5( fname, t_0, t_1, t_2, t_3, t_4 )      \
struct fname##_doom_thread_args_decoder {                                   \
    t_0 arg0;                                                               \
    t_1 arg1;                                                               \
    t_2 arg2;                                                               \
    t_3 arg3;                                                               \
    t_4 arg4;                                                               \
};                                                                          \
struct fname##_doom_thread_args_decoder *                                   \
fname##_doom_thread_args_pack(                                              \
        t_0 arg0,                                                           \
        t_1 arg1,                                                           \
        t_2 arg2,                                                           \
        t_3 arg3,                                                           \
        t_4 arg4                                                            \
        ) {                                                                 \
    struct fname##_doom_thread_args_decoder * str =                         \
        (struct fname##_doom_thread_args_decoder *)DOOM_THREAD_ALLOCATOR(   \
                sizeof(struct fname##_doom_thread_args_decoder)             \
                );                                                          \
    str->arg0 = arg0;                                                       \
    str->arg1 = arg1;                                                       \
    str->arg2 = arg2;                                                       \
    str->arg3 = arg3;                                                       \
    str->arg4 = arg4;                                                       \
    return str;                                                             \
}                                                                           \
void * fname##_doom_thread_wrapper( void * args ) {                         \
    struct fname##_doom_thread_args_decoder * f_args =                      \
        (struct fname##_doom_thread_args_decoder *) args;                   \
    fname(                                                                  \
            f_args->arg0,                                                   \
            f_args->arg1,                                                   \
            f_args->arg2,                                                   \
            f_args->arg3,                                                   \
            f_args->arg4                                                    \
         );                                                                 \
    DOOM_THREAD_DEALLOCATOR(args);                                          \
    return NULL;                                                            \
} 

#define DOOM_THREAD_PROCEDURE_WRAP_6( fname, t_0, t_1, t_2, t_3, t_4, t_5 ) \
struct fname##_doom_thread_args_decoder {                                   \
    t_0 arg0;                                                               \
    t_1 arg1;                                                               \
    t_2 arg2;                                                               \
    t_3 arg3;                                                               \
    t_4 arg4;                                                               \
    t_5 arg5;                                                               \
};                                                                          \
struct fname##_doom_thread_args_decoder *                                   \
fname##_doom_thread_args_pack(                                              \
        t_0 arg0,                                                           \
        t_1 arg1,                                                           \
        t_2 arg2,                                                           \
        t_3 arg3,                                                           \
        t_4 arg4,                                                           \
        t_5 arg5                                                            \
        ) {                                                                 \
    struct fname##_doom_thread_args_decoder * str =                         \
        (struct fname##_doom_thread_args_decoder *)DOOM_THREAD_ALLOCATOR(   \
                sizeof(struct fname##_doom_thread_args_decoder)             \
                );                                                          \
    str->arg0 = arg0;                                                       \
    str->arg1 = arg1;                                                       \
    str->arg2 = arg2;                                                       \
    str->arg3 = arg3;                                                       \
    str->arg4 = arg4;                                                       \
    str->arg5 = arg5;                                                       \
    return str;                                                             \
}                                                                           \
void * fname##_doom_thread_wrapper( void * args ) {                         \
    struct fname##_doom_thread_args_decoder * f_args =                      \
        (struct fname##_doom_thread_args_decoder *) args;                   \
    fname(                                                                  \
            f_args->arg0,                                                   \
            f_args->arg1,                                                   \
            f_args->arg2,                                                   \
            f_args->arg3,                                                   \
            f_args->arg4,                                                   \
            f_args->arg4                                                    \
         );                                                                 \
    DOOM_THREAD_DEALLOCATOR(args);                                          \
    return NULL;                                                            \
} 

/* Function Wrapping Macros */
#define DOOM_THREAD_FUNCTION_WRAP( t_ret, fname )                           \
void * fname##_doom_thread_wrapper( void * args ) {                         \
    t_ret * result = (t_ret *) DOOM_THREAD_ALLOCATOR(sizeof(t_ret));        \
    *result = fname();                                                      \
    return (void *) result;                                                 \
}  

#define DOOM_THREAD_FUNCTION_WRAP_1( t_ret, fname, t_0 )                    \
struct fname##_doom_thread_args_decoder {                                   \
    t_0 arg0;                                                               \
};                                                                          \
struct fname##_doom_thread_args_decoder *                                   \
fname##_doom_thread_args_pack(                                              \
        t_0 arg0                                                            \
        ) {                                                                 \
    struct fname##_doom_thread_args_decoder * str =                         \
        (struct fname##_doom_thread_args_decoder *)DOOM_THREAD_ALLOCATOR(   \
                sizeof(struct fname##_doom_thread_args_decoder)             \
                );                                                          \
    str->arg0 = arg0;                                                       \
    return str;                                                             \
}                                                                           \
void * fname##_doom_thread_wrapper( void * args ) {                         \
    struct fname##_doom_thread_args_decoder * f_args =                      \
        (struct fname##_doom_thread_args_decoder *) args;                   \
    t_ret * result = (t_ret *) DOOM_THREAD_ALLOCATOR(sizeof(t_ret));        \
    *result = fname(                                                        \
            f_args->arg0                                                    \
         );                                                                 \
    DOOM_THREAD_DEALLOCATOR(args);                                          \
    return (void *) result;                                                 \
} 

#define DOOM_THREAD_FUNCTION_WRAP_2( t_ret, fname, t_0, t_1 )               \
struct fname##_doom_thread_args_decoder {                                   \
    t_0 arg0;                                                               \
    t_1 arg1;                                                               \
};                                                                          \
struct fname##_doom_thread_args_decoder *                                   \
fname##_doom_thread_args_pack(                                              \
        t_0 arg0,                                                           \
        t_1 arg1                                                            \
        ) {                                                                 \
    struct fname##_doom_thread_args_decoder * str =                         \
        (struct fname##_doom_thread_args_decoder *)DOOM_THREAD_ALLOCATOR(   \
                sizeof(struct fname##_doom_thread_args_decoder)             \
                );                                                          \
    str->arg0 = arg0;                                                       \
    str->arg1 = arg1;                                                       \
    return str;                                                             \
}                                                                           \
void * fname##_doom_thread_wrapper( void * args ) {                         \
    struct fname##_doom_thread_args_decoder * f_args =                      \
        (struct fname##_doom_thread_args_decoder *) args;                   \
    t_ret * result = (t_ret *) DOOM_THREAD_ALLOCATOR(sizeof(t_ret));        \
    *result = fname(                                                        \
            f_args->arg0,                                                   \
            f_args->arg1                                                    \
         );                                                                 \
    DOOM_THREAD_DEALLOCATOR(args);                                          \
    return result;                                                          \
} 

#define DOOM_THREAD_FUNCTION_WRAP_3( t_ret, fname, t_0, t_1, t_2 )          \
struct fname##_doom_thread_args_decoder {                                   \
    t_0 arg0;                                                               \
    t_1 arg1;                                                               \
    t_2 arg2;                                                               \
};                                                                          \
struct fname##_doom_thread_args_decoder *                                   \
fname##_doom_thread_args_pack(                                              \
        t_0 arg0,                                                           \
        t_1 arg1,                                                           \
        t_2 arg2                                                            \
        ) {                                                                 \
    struct fname##_doom_thread_args_decoder * str =                         \
        (struct fname##_doom_thread_args_decoder *)DOOM_THREAD_ALLOCATOR(   \
                sizeof(struct fname##_doom_thread_args_decoder)             \
                );                                                          \
    str->arg0 = arg0;                                                       \
    str->arg1 = arg1;                                                       \
    str->arg2 = arg2;                                                       \
    return str;                                                             \
}                                                                           \
void * fname##_doom_thread_wrapper( void * args ) {                         \
    struct fname##_doom_thread_args_decoder * f_args =                      \
        (struct fname##_doom_thread_args_decoder *) args;                   \
    t_ret * result = (t_ret *) DOOM_THREAD_ALLOCATOR(sizeof(t_ret));        \
    *result = fname(                                                        \
            f_args->arg0,                                                   \
            f_args->arg1,                                                   \
            f_args->arg2                                                    \
         );                                                                 \
    DOOM_THREAD_DEALLOCATOR(args);                                          \
    return result;                                                          \
} 

#define DOOM_THREAD_FUNCTION_WRAP_4( t_ret, fname, t_0, t_1, t_2, t_3 )     \
struct fname##_doom_thread_args_decoder {                                   \
    t_0 arg0;                                                               \
    t_1 arg1;                                                               \
    t_2 arg2;                                                               \
    t_3 arg3;                                                               \
};                                                                          \
struct fname##_doom_thread_args_decoder *                                   \
fname##_doom_thread_args_pack(                                              \
        t_0 arg0,                                                           \
        t_1 arg1,                                                           \
        t_2 arg2,                                                           \
        t_3 arg3                                                            \
        ) {                                                                 \
    struct fname##_doom_thread_args_decoder * str =                         \
        (struct fname##_doom_thread_args_decoder *)DOOM_THREAD_ALLOCATOR(   \
                sizeof(struct fname##_doom_thread_args_decoder)             \
                );                                                          \
    str->arg0 = arg0;                                                       \
    str->arg1 = arg1;                                                       \
    str->arg2 = arg2;                                                       \
    str->arg3 = arg3;                                                       \
    return str;                                                             \
}                                                                           \
void * fname##_doom_thread_wrapper( void * args ) {                         \
    struct fname##_doom_thread_args_decoder * f_args =                      \
        (struct fname##_doom_thread_args_decoder *) args;                   \
    t_ret * result = (t_ret *) DOOM_THREAD_ALLOCATOR(sizeof(t_ret));        \
    *result = fname(                                                        \
            f_args->arg0,                                                   \
            f_args->arg1,                                                   \
            f_args->arg2,                                                   \
            f_args->arg3                                                    \
         );                                                                 \
    DOOM_THREAD_DEALLOCATOR(args);                                          \
    return result;                                                          \
} 

#define DOOM_THREAD_FUNCTION_WRAP_5( t_ret, fname, t_0, t_1, t_2, t_3, t_4 )\
struct fname##_doom_thread_args_decoder {                                   \
    t_0 arg0;                                                               \
    t_1 arg1;                                                               \
    t_2 arg2;                                                               \
    t_3 arg3;                                                               \
    t_4 arg4;                                                               \
};                                                                          \
struct fname##_doom_thread_args_decoder *                                   \
fname##_doom_thread_args_pack(                                              \
        t_0 arg0,                                                           \
        t_1 arg1,                                                           \
        t_2 arg2,                                                           \
        t_3 arg3,                                                           \
        t_4 arg4                                                            \
        ) {                                                                 \
    struct fname##_doom_thread_args_decoder * str =                         \
        (struct fname##_doom_thread_args_decoder *)DOOM_THREAD_ALLOCATOR(   \
                sizeof(struct fname##_doom_thread_args_decoder)             \
                );                                                          \
    str->arg0 = arg0;                                                       \
    str->arg1 = arg1;                                                       \
    str->arg2 = arg2;                                                       \
    str->arg3 = arg3;                                                       \
    str->arg4 = arg4;                                                       \
    return str;                                                             \
}                                                                           \
void * fname##_doom_thread_wrapper( void * args ) {                         \
    struct fname##_doom_thread_args_decoder * f_args =                      \
        (struct fname##_doom_thread_args_decoder *) args;                   \
    t_ret * result = (t_ret *) DOOM_THREAD_ALLOCATOR(sizeof(t_ret));        \
    *result = fname(                                                        \
            f_args->arg0,                                                   \
            f_args->arg1,                                                   \
            f_args->arg2,                                                   \
            f_args->arg3,                                                   \
            f_args->arg4                                                    \
         );                                                                 \
    DOOM_THREAD_DEALLOCATOR(args);                                          \
    return result;                                                          \
} 

#define DOOM_THREAD_FUNCTION_WRAP_6( t_ret, fname, t_0, t_1, t_2, t_3, t_4, t_5 )\
struct fname##_doom_thread_args_decoder {                                   \
    t_0 arg0;                                                               \
    t_1 arg1;                                                               \
    t_2 arg2;                                                               \
    t_3 arg3;                                                               \
    t_4 arg4;                                                               \
    t_5 arg5;                                                               \
};                                                                          \
struct fname##_doom_thread_args_decoder *                                   \
fname##_doom_thread_args_pack(                                              \
        t_0 arg0,                                                           \
        t_1 arg1,                                                           \
        t_2 arg2,                                                           \
        t_3 arg3,                                                           \
        t_4 arg4,                                                           \
        t_5 arg5                                                            \
        ) {                                                                 \
    struct fname##_doom_thread_args_decoder * str =                         \
        (struct fname##_doom_thread_args_decoder *)DOOM_THREAD_ALLOCATOR(   \
                sizeof(struct fname##_doom_thread_args_decoder)             \
                );                                                          \
    str->arg0 = arg0;                                                       \
    str->arg1 = arg1;                                                       \
    str->arg2 = arg2;                                                       \
    str->arg3 = arg3;                                                       \
    str->arg4 = arg4;                                                       \
    str->arg5 = arg5;                                                       \
    return str;                                                             \
}                                                                           \
void * fname##_doom_thread_wrapper( void * args ) {                         \
    struct fname##_doom_thread_args_decoder * f_args =                      \
        (struct fname##_doom_thread_args_decoder *) args;                   \
    t_ret * result = (t_ret *) DOOM_THREAD_ALLOCATOR(sizeof(t_ret));        \
    *result = fname(                                                        \
            f_args->arg0,                                                   \
            f_args->arg1,                                                   \
            f_args->arg2,                                                   \
            f_args->arg3,                                                   \
            f_args->arg4,                                                   \
            f_args->arg5                                                    \
         );                                                                 \
    DOOM_THREAD_DEALLOCATOR(args);                                          \
    return result;                                                          \
} 



/*****************************************************************************/
/* DOOMSDAY C THREAD STRIP PREFIX                                            */
/*****************************************************************************/
#ifdef DOOMSDAY_C_STRIP_PREFIX

#define thread_create doom_thread_create
#define thread_create_detached doom_thread_create_detached
#define thread_new doom_thread_new
#define thread_new_detached doom_thread_new_detached
#define thread_join doom_thread_join
#define thread_detach doom_thread_detach
#define thread_deinit  doom_thread_deinit
#define thread_result_deinit  doom_thread_result_deinit

#endif /* DOOMSDAY_C_STRIP_PREFIX */

/*****************************************************************************/
/* DOOMSDAY C THREAD IMPLEMENTATIN                                           */
/*****************************************************************************/
#ifdef DOOMSDAY_C_IMPLEMENTATION

int doom_thread_create( doom_thread * restrict newthread, void *(*start_routine)(void *), void *restrict arg ) {
    int result = 0;
    result = pthread_create(newthread, NULL, start_routine, arg);
    return result;
}

int doom_thread_create_detached( doom_thread * restrict newthread, void *(*start_routine)(void *), void *restrict arg ) {
    int result = 0;  
    result = pthread_create(newthread, NULL, start_routine, arg);
    if (result != 0) {
        return result;
    }
    result = pthread_detach(*newthread);
    return result;
}

doom_thread * doom_thread_new( void *(*start_routine)(void *), 
        void *restrict arg ) {
    doom_thread  * newthread = NULL;
    int result = 0;
    newthread = (doom_thread *)DOOM_THREAD_ALLOCATOR(sizeof(doom_thread));
    if (newthread == NULL) {
        return NULL;
    }
    result = pthread_create(newthread, NULL, start_routine, arg);
    if (result != 0) {
        return NULL;
    }
    return newthread;
}

doom_thread * doom_thread_new_detached( void *(*start_routine)(void *), void *restrict arg ) {
    doom_thread  * newthread = NULL;
    int result = 0;
    newthread = (doom_thread *)DOOM_THREAD_ALLOCATOR(sizeof(doom_thread));
    if (newthread == NULL) {
        return NULL;
    }
    result = pthread_create(newthread, NULL, start_routine, arg);
    if (result != 0) {
        return NULL;
    }
    result = pthread_detach(*newthread);
    if (result != 0) {
        return NULL;
    }
    return newthread;
}

void * doom_thread_join( doom_thread * thread ) {
    void * result = NULL;
    int ret = 0;
    ret = pthread_join(*thread, &result);
    if (ret != 0) {
        return NULL;
    }
    return result;
}

int doom_thread_detach( doom_thread * thread ) {
    int ret = 0;
    ret = pthread_detach(*thread);
    return ret;
}

void doom_thread_deinit(void *result) {
    DOOM_THREAD_DEALLOCATOR(result);
}

void doom_thread_result_deinit( void * result ) {
    DOOM_THREAD_DEALLOCATOR(result);
}

#endif /* DOOMSDAY_C_IMPLEMENTATION */


/*****************************************************************************/
/* DOOMSDAY C STRING                                                         */
/*****************************************************************************/

typedef struct doomsday_c_string_header_t {
    void *(*_allocator)(size_t);
    void (*_deallocator)(void *);
    int64_t _size;
} doom_string_header;

typedef struct doomsday_c_string_t {
    void *(**_allocator)(size_t);
    void (**_deallocator)(void *);
    int64_t *_size;
    char * _char_array;
} doom_string_struct;

typedef char * doom_string ;

/* basic functionality */
int     doom_string_get_struct( doom_string_struct * string_struct, char const * string);
void *  doom_string_base_pointer( char * string );
int     doom_string_init( char ** string );
int     doom_string_init_size ( char ** string, size_t const size );
int     doom_string_init_copy ( char ** string, char const * restrict c_string );
int     doom_string_init_alloc( char ** string, void *(*custom_allocator)(size_t), void (*custom_deallocator)(void *));
int     doom_string_init_size_alloc ( char ** string, size_t const size, void *(*custom_allocator)(size_t), void (*custom_deallocator)(void *));
int     doom_string_init_copy_alloc ( char ** string, char const * restrict c_string, void *(*custom_allocator)(size_t), void (*custom_deallocator)(void *));
void    doom_string_deinit( char * string );
void    doom_string_deinit_ptr( char ** string );
char *  doom_string_new( void );
char *  doom_string_new_size( size_t const size );
char *  doom_string_new_copy ( char const * restrict c_string );
char *  doom_string_new_alloc( void *(*custom_allocator)(size_t), void (*custom_deallocator)(void *));
char *  doom_string_new_size_alloc ( size_t const size, void *(*custom_allocator)(size_t), void (*custom_deallocator)(void *));
char *  doom_string_new_copy_alloc ( char const * restrict c_string, void *(*custom_allocator)(size_t), void (*custom_deallocator)(void *));
int     doom_string_resize( char ** string, size_t const size );
int     doom_string_safe_resize( char ** string, size_t const size );
int     doom_string_fit( char ** string );
int64_t doom_string_get_size( char const * string );
/* string.h funcitons weappers */
char *  doom_string_stpcpy( char ** dst, char const * restrict src );
char *  doom_string_strcat( char ** dst, char const * restrict src );
char *  doom_string_strchr( char * dst, int c );
int     doom_string_strcmp( char const * s1, char const * s2 );
int     doom_string_strcoll( char const * s1, char const * s2 );
int     doom_string_strcoll_l( char const * s1, char const * s2, locale_t locale );
char *  doom_string_strcpy( char ** dst, char const * restrict src );
size_t  doom_string_strcspn( char const * s, char const * reject );
char *  doom_string_strdup( char const * s );
size_t  doom_string_strlen( char const * s );
char *  doom_string_strpbrk( char const * s, char const * accept );
size_t  doom_string_strspn( char const * s, char const * accept );
char *  doom_string_strstr( char const * haystack, char const * needle );
char *  doom_string_strtok( char * restrict str, char const * restrict delim);
char *  doom_string_strtok_r( char * restrict str, char const * restrict delim,
                                                   char ** restrict saveptr);

/*****************************************************************************/
/* DOOMSDAY C STRING MACROS                                                  */
/*****************************************************************************/
#ifdef DOOMSDAY_C_ENABLE_COMPILER_MAGIC
#ifndef __has_attribute
#   define __has_attribute(x) 0
#endif
#if __has_attribute(cleanup)
#   define DOOM_STRING_DEFER_FREE __attribute__((cleanup(doom_string_deinit_ptr)))
#else
#   warning "COMPILER MISSING CLEANUP FEATURE! Memory leacks are possible!"
#   define DOOM_STRING_DEFER_FREE 
#endif
#endif /* ifdef DOOMSDAY_C_ENABLE_COMPILER_MAGIC */


/*****************************************************************************/
/* DOOMSDAY C STRING STRIP PREFIX                                            */
/*****************************************************************************/
#ifdef DOOMSDAY_C_STRIP_PREFIX

#define string_to_struct doom_string_get_struct
#define string_base_pointer doom_string_base_pointer
#define string_init doom_string_init
#define string_init_s doom_string_init_size
#define string_init_c doom_string_init_copy
#define string_init_a doom_string_init_alloc
#define string_init_s_a doom_string_init_size_alloc
#define string_init_c_a doom_string_init_copy_alloc
#define string_deinit doom_string_deinit
#define string_new doom_string_new
#define string_new_s doom_string_new_size
#define string_new_c doom_string_new_copy
#define string_new_a doom_string_new_alloc
#define string_new_s_a doom_string_new_size_alloc
#define string_new_c_a doom_string_new_copy_alloc
#define string_resize doom_string_resize
#define string_safe_resize doom_string_safe_resize
#define string_fit doom_string_fit
#define string_get_size doom_string_get_size
#define string_srpcpy doom_string_stpcpy
#define string_strcat doom_string_strcat
#define string_strchr doom_string_strchr
#define string_strcmp doom_string_strcmp
#define string_strcoll doom_string_strcoll
#define string_strcoll_l doom_string_strcoll_l
#define string_strcpy doom_string_strcpy
#define string_strcspn doom_string_strcspn
#define string_strdup doom_string_strdup
#define string_strlen doom_string_strlen
#define string_strpbrk doom_string_strpbrk
#define string_strspn doom_string_strspn
#define string_strstr doom_string_strstr
#define string_strtok doom_string_strtok
#define string_strtok_r doom_string_strtok_r

#endif /* DOOMSDAY_C_STRIP_PREFIX */


/*****************************************************************************/
/* DOOMSDAY C STRING IMPLEMENTATIN                                           */
/*****************************************************************************/
#ifdef DOOMSDAY_C_IMPLEMENTATION

int doom_string_get_struct(doom_string_struct * string_struct, char const * string) {
    if (string_struct == NULL || string == NULL) {
        return DOOM_ARGUMENT_IS_NULL;
    }
    doom_string_header * aux_header = ((doom_string_header *)string) - 1;
    string_struct->_size = &aux_header->_size;
    string_struct->_allocator = &aux_header->_allocator;
    string_struct->_deallocator = &aux_header->_deallocator;
    string_struct->_char_array = (char *)string;
    return DOOM_SUCCESS;
}

void * doom_string_base_pointer( char * string ) {
    if (string == NULL) {
        return NULL;
    }
    return (void *)((doom_string_header *)string - 1);
}

int doom_string_init( char ** string ) {
    doom_string_header * aux_str = NULL;
    aux_str = (doom_string_header *)DOOMSDAY_C_DEFAULT_ALLOCATOR(
            sizeof(doom_string_header) + 
            sizeof(char)
            ); 
    if(aux_str == NULL) {
        return DOOM_OUT_OF_MEMORY; 
    }
    aux_str->_size = 0; 
    aux_str->_allocator = DOOMSDAY_C_DEFAULT_ALLOCATOR;
    aux_str->_deallocator = DOOMSDAY_C_DEFAULT_DEALLOCATOR;
    *string = (char *)(aux_str + 1);
    memset((void *)*string, 0, 1);
    return DOOM_SUCCESS;
}

int doom_string_init_size ( char ** string, size_t const size ) {
    doom_string_header * aux_str = NULL;
    aux_str = (doom_string_header *)DOOMSDAY_C_DEFAULT_ALLOCATOR(
            sizeof(doom_string_header) + 
            (sizeof(char) * size+1)
             ); 
    if(aux_str == NULL) {
        return DOOM_OUT_OF_MEMORY; 
    }
    aux_str->_size = size; 
    aux_str->_allocator = DOOMSDAY_C_DEFAULT_ALLOCATOR;
    aux_str->_deallocator = DOOMSDAY_C_DEFAULT_DEALLOCATOR;
    *string = (char *)(aux_str + 1);
    memset((void *)*string, 0, size+1);
    return DOOM_SUCCESS;
}

int doom_string_init_copy ( char ** string, char const * restrict c_string ) {
    if (c_string == NULL) {
        return DOOM_ARGUMENT_IS_NULL;
    }
    size_t c_string_size = strlen(c_string);
    doom_string_header * aux_str = NULL;
    aux_str = (doom_string_header *)DOOMSDAY_C_DEFAULT_ALLOCATOR(
            sizeof(doom_string_header) + 
            sizeof(char) * (c_string_size + 1)
            ); 
    if(aux_str == NULL) {
        return DOOM_OUT_OF_MEMORY; 
    }
    aux_str->_size = c_string_size; 
    aux_str->_allocator = DOOMSDAY_C_DEFAULT_ALLOCATOR;
    aux_str->_deallocator = DOOMSDAY_C_DEFAULT_DEALLOCATOR;
    *string = (char *)(aux_str + 1);
    memset((void *)*string, 0, c_string_size+1);
    memcpy((void *)*string, c_string, c_string_size);
    printf("%s\n", (*string));
    return DOOM_SUCCESS;
}

int doom_string_init_alloc( char ** string, void *(*custom_allocator)(size_t), void (*custom_deallocator)(void *)) {
    if (custom_allocator == NULL || custom_deallocator == NULL) {
        return DOOM_ARGUMENT_IS_NULL;
    }
    doom_string_header * aux_str = NULL;
    aux_str = (doom_string_header *)custom_allocator(
            sizeof(doom_string_header) + 
            sizeof(char)
            ); 
    if(aux_str == NULL) {
        return DOOM_OUT_OF_MEMORY; 
    }
    aux_str->_size = 0; 
    aux_str->_allocator = custom_allocator;
    aux_str->_deallocator = custom_deallocator;
    *string = (char *)(aux_str + 1);
    memset((void *)*string, 0, 1);
    return DOOM_SUCCESS;
}

int doom_string_init_size_alloc ( char ** string, size_t const size, void *(*custom_allocator)(size_t), void (*custom_deallocator)(void *)) {
    if (custom_allocator == NULL || custom_deallocator == NULL) {
        return DOOM_ARGUMENT_IS_NULL;
    }
    doom_string_header * aux_str = NULL;
    aux_str = (doom_string_header *)custom_allocator(
            sizeof(doom_string_header) + 
            (sizeof(char) * size+1)
             ); 
    if(aux_str == NULL) {
        return DOOM_OUT_OF_MEMORY; 
    }
    aux_str->_size = size; 
    aux_str->_allocator = custom_allocator;
    aux_str->_deallocator = custom_deallocator;
    *string = (char *)(aux_str + 1);
    memset((void *)*string, 0, size+1);
    return DOOM_SUCCESS;
}

int doom_string_init_copy_alloc ( char ** string, char const * restrict c_string, void *(*custom_allocator)(size_t), void (*custom_deallocator)(void *)) {
    if (custom_allocator == NULL || custom_deallocator == NULL) {
        return DOOM_ARGUMENT_IS_NULL;
    }
    if (c_string == NULL) {
        return DOOM_ARGUMENT_IS_NULL;
    }
    size_t c_string_size = strlen(c_string);
    doom_string_header * aux_str = NULL;
    aux_str = (doom_string_header *)custom_allocator(
            sizeof(doom_string_header) + 
            sizeof(char) * (c_string_size + 1)
            ); 
    if(aux_str == NULL) {
        return DOOM_OUT_OF_MEMORY; 
    }
    aux_str->_size = c_string_size; 
    aux_str->_allocator = custom_allocator;
    aux_str->_deallocator = custom_deallocator;
    *string = (char *)(aux_str + 1);
    memset((void *)*string, 0, c_string_size+1);
    memcpy((void *)*string, c_string, c_string_size);
    return DOOM_SUCCESS;
}

void doom_string_deinit( char * string ) {
    if (string == NULL) {
        return;
    }
    doom_string_header * string_head = (doom_string_header *)doom_string_base_pointer(string);
    if (string_head == NULL) {
        return;
    }
    string_head->_deallocator((void *)string_head);
}

void doom_string_deinit_ptr( char ** string_ptr ) {
    doom_string_deinit(*string_ptr);
}

char * doom_string_new( void ) {
    char * aux_str = NULL;
    if((doom_string_init(&aux_str)) != DOOM_SUCCESS) {
        return NULL;
    }
    return aux_str;
}

char * doom_string_new_size( size_t const size ) {
    char * aux_str = NULL;
    if((doom_string_init_size(&aux_str, size)) != DOOM_SUCCESS) {
        return NULL;
    }
    return aux_str;
}

char * doom_string_new_copy ( char const * restrict c_string ) {
    char * aux_str = NULL;
    if((doom_string_init_copy(&aux_str, c_string)) != DOOM_SUCCESS) {
        return NULL;
    }
    return aux_str;
}

char * doom_string_new_alloc( void *(*custom_allocator)(size_t), void (*custom_deallocator)(void *)) {
    char * aux_str = NULL;
    if((doom_string_init_alloc(&aux_str, custom_allocator, custom_deallocator)) != DOOM_SUCCESS) {
        return NULL;
    }
    return aux_str;
}

char * doom_string_new_size_alloc ( size_t const size, void *(*custom_allocator)(size_t), void (*custom_deallocator)(void *)) {
    char * aux_str = NULL;
    if((doom_string_init_size_alloc(&aux_str, size, custom_allocator, custom_deallocator)) != DOOM_SUCCESS) {
        return NULL;
    }
    return aux_str;
}

char * doom_string_new_copy_alloc ( char const * restrict c_string, void *(*custom_allocator)(size_t), void (*custom_deallocator)(void *)) {
    char * aux_str = NULL;
    if((doom_string_init_copy_alloc(&aux_str, c_string, custom_allocator, custom_deallocator)) != DOOM_SUCCESS) {
        return NULL;
    }
    return aux_str;
}

int doom_string_resize( char ** string, size_t const size ) {
    if (string == NULL) {
        return DOOM_ARGUMENT_IS_NULL;
    }
    if (*string == NULL) {
        return DOOM_ARGUMENT_IS_NULL;
    }
    char * aux_str = doom_string_new_size(size);
    int64_t aux_len = doom_string_get_size(*string);
    memcpy(aux_str, *string, (aux_len < size) ? aux_len : size);
    doom_string_deinit(*string);

    *string = aux_str;
    return DOOM_SUCCESS;
}

int doom_string_safe_resize( char ** string, size_t const size ) {
    if (string == NULL) {
        return DOOM_ARGUMENT_IS_NULL;
    }
    if (*string == NULL) {
        return DOOM_ARGUMENT_IS_NULL;
    }
    if (size <= strlen(*string)) {
        return DOOM_INVALID_ARGUMENT;
    }

    doom_string_resize(string, size);
    return DOOM_SUCCESS;
}

int doom_string_fit( char ** string ) {
    if (string == NULL) {
        return DOOM_ARGUMENT_IS_NULL;
    }
    if (*string == NULL) {
        return DOOM_ARGUMENT_IS_NULL;
    }

    return doom_string_resize(string, strlen(*string));
}

int64_t doom_string_get_size( char const * string ) {
    if (string == NULL) {
        return DOOM_ARGUMENT_IS_NULL;
    }
    
    doom_string_struct str;
    int64_t result = doom_string_get_struct(&str, string);
    if (result != DOOM_SUCCESS) {
        return result;
    }
    return *str._size;
}

char * doom_string_stpcpy( char ** dst, char const * restrict src ) {
    if (dst == NULL || src == NULL || *dst == NULL) {
        return NULL;
    }
    if (doom_string_get_size(*dst) < doom_string_strlen(src)) {
        doom_string_resize(dst, doom_string_strlen(src));
    }

    return stpcpy(*dst, src);
}

char * doom_string_strcat( char ** dst, char const * restrict src ) {
    if (dst == NULL || src == NULL || *dst == NULL) {
        return NULL;
    }
    if (doom_string_get_size(*dst) <
        doom_string_strlen(*dst) + doom_string_strlen(src)) {
        doom_string_resize(
                dst,
                doom_string_strlen(*dst)+doom_string_strlen(src)
                );
    }

    return strcat(*dst, src);
}

char * doom_string_strchr( char * dst, int c ) {
    if (dst == NULL) {
        return NULL;
    }

    return strchr(dst, c);
}

int doom_string_strcmp( char const * s1, char const * s2 ) {
    if (s1 == NULL || s2 == NULL) {
        return -2;
    }
    int aux = strcmp(s1, s2);

    return (aux<0) ? -1 : (aux>0) ? 1 : 0;
}

int doom_string_strcoll( char const * s1, char const * s2 ) {
    if (s1 == NULL || s2 == NULL) {
        return -2;
    }
    int aux = strcoll(s1, s2);

    return (aux<0) ? -1 : (aux>0) ? 1 : 0;
}

int doom_string_strcoll_l( char const * s1, char const * s2,
                           locale_t locale ) {
    if (s1 == NULL || s2 == NULL) {
        return -2;
    }
    int aux = strcoll_l(s1, s2, locale);

    return (aux<0) ? -1 : (aux>0) ? 1 : 0;
}

char * doom_string_strcpy( char ** dst, char const * restrict src ) {
    if (dst == NULL || src == NULL || *dst == NULL) {
        return NULL;
    }
    if (doom_string_get_size(*dst) < doom_string_strlen(src)) {
        doom_string_resize(dst, doom_string_strlen(src));
    }

    return strcpy(*dst, src);
}

size_t doom_string_strcspn( char const * s, char const * reject ) {
    if (s == NULL || reject == NULL) {
        return 0;
    }

    return strcspn(s, reject);
}

char * doom_string_strdup( char const * s ) {
    return doom_string_new_copy(s);
}

size_t doom_string_strlen( char const * s ) {
    if (s == NULL) {
        return 0;
    }

    return strlen(s);
}

char * doom_string_strpbrk( char const * s, char const * accept ) {
    if (s == NULL || accept == NULL) {
        return 0;
    }

    return (char *)strpbrk(s, accept);
}

size_t doom_string_strspn( char const * s, char const * accept ) {
    if (s == NULL || accept == NULL) {
        return 0;
    }

    return strspn(s, accept);
}

char * doom_string_strstr( char const * haystack, char const * needle ) {
    if (haystack == NULL || needle == NULL) {
        return NULL;
    }
    if (doom_string_strlen(needle) > doom_string_strlen(haystack)) {
        return NULL;
    }

    return (char *)strstr(haystack, needle);
}

char * doom_string_strtok( char * restrict str, char const * restrict delim) {
    return strtok(str, delim);
}

char * doom_string_strtok_r( char * restrict str, char const * restrict delim,
                                                  char ** restrict saveptr) {
    return strtok_r(str, delim, saveptr);
}

#endif /* DOOMSDAY_C_IMPLEMENTATION */


/*****************************************************************************/
/* DOOMSDAY C DYNAMIC ARRAY                                                  */
/*****************************************************************************/

#define DOOM_ARRAY_INIT_DEFAULT_SIZE 5

typedef void * doom_dynamic_array;

typedef struct doomsday_c_dynamic_array_header_t {
    int64_t _sizeof_elem;
    int64_t _array_size;
    int64_t _array_max_size;
    void *(*_allocator)(size_t);
    void (*_deallocator)(void *);
}doom_dynamic_array_header;

typedef struct doomsday_c_dynamic_array_t {
    int64_t * _sizeof_elem;
    int64_t * _array_size;
    int64_t * _array_max_size;
    void *(**_allocator)(size_t);
    void (**_deallocator)(void *);
    byte * _array;
} doom_dynamic_array_struct;

/* basic functionality */
int doom_dynamic_array_get_struct( doom_dynamic_array_struct * da_struct, doom_dynamic_array da );
void * doom_dynamic_array_base_ptr( doom_dynamic_array da );
int doom_dynamic_array_init( doom_dynamic_array * da , int64_t const sizeof_elem );
int doom_dynamic_array_init_size( doom_dynamic_array * da, int64_t const sizeof_elem, int64_t const max_size );
int doom_dynamic_array_init_copy( doom_dynamic_array * da, int64_t const sizeof_elem, void const * restrict from, int64_t const from_size );
void doom_dynamic_array_deinit( doom_dynamic_array da );
void doom_dynamic_array_deinit_ptr( doom_dynamic_array * da_ptr );
doom_dynamic_array doom_dynamic_array_new( int64_t const sizeof_elem );
doom_dynamic_array doom_dynamic_array_new_size( int64_t const sizeof_elem, int64_t const size );
doom_dynamic_array doom_dynamic_array_new_copy( int64_t const sizeof_elem, void const * restrict from, int64_t const from_size );
int64_t doom_dynamic_array_get_elem_size( void const * restrict da );
int64_t doom_dynamic_array_get_size( void const * restrict da );
int64_t doom_dynamic_array_get_max_size( void const * restrict da );
int doom_dynamic_array_expand( doom_dynamic_array * da );
int doom_dynamic_array_resize( doom_dynamic_array * da, int64_t const size );
int doom_dynamic_array_fit( doom_dynamic_array * da );
int doom_dynamic_array_reserve( doom_dynamic_array * da, int64_t const size );
/* Stack and Queue operations */
doom_dynamic_array doom_dynamic_array_back( void const * restrict da );
int doom_dynamic_array_push_back( doom_dynamic_array * da, void const * restrict value );
int doom_dynamic_array_pop_back( doom_dynamic_array * da );
doom_dynamic_array doom_dynamic_array_front( void const * restrict da );
int doom_dynamic_array_push_front( doom_dynamic_array * da, void const * restrict value );
int doom_dynamic_array_pop_front( doom_dynamic_array * da );

/*****************************************************************************/
/* DOOMSDAY C DYNAMIC ARRAY MACROS                                           */
/*****************************************************************************/

#ifdef DOOMSDAY_C_ENABLE_COMPILER_MAGIC
#ifndef __has_attribute
#   define __has_attribute(x) 0
#endif
#if __has_attribute(cleanup)
#   define DOOM_DA_DEFER_DEINIT __attribute__((cleanup(doom_dynamic_array_deinit_ptr)))
#else
#   warning "COMPILER MISSING CLEANUP FEATURE! Memory leacks are possible!"
#   define DOOM_DA_DEFER_DEINIT 
#endif
#endif /* ifdef DOOMSDAY_C_ENABLE_COMPILER_MAGIC */

#define DOOM_DYNAMIC_ARRAY_FOREACH(elem, array) \
    elem = array[0]; \
    for(uint64_t array_index=0; \
        array_index<doom_dynamic_array_get_size(array); \
        array_index++, elem = array[array_index] \
        )

/*****************************************************************************/
/* DOOMSDAY C DYNAMIC ARRAY STRIP PREFIX                                     */
/*****************************************************************************/
#ifdef DOOMSDAY_C_STRIP_PREFIX

#define dynamic_array_get_struct doom_dynamic_array_get_struct
#define dynamic_array_get_base_ptr doom_dynamic_array_base_ptr
#define dynamic_array_init doom_dynamic_array_init
#define dynamic_array_init_s doom_dynamic_array_init_size
#define dynamic_array_init_c doom_dynamic_array_init_copy
#define dynamic_array_deinit doom_dynamic_array_deinit
#define dynamic_array_new doom_dynamic_array_new
#define dynamic_array_new_copy doom_dynamic_array_new_copy
#define dynamic_array_new_size doom_dynamic_array_new_size
#define dynamic_array_get_elem_size doom_dynamic_array_get_elem_size
#define dynamic_array_get_size doom_dynamic_array_get_size
#define dynamic_array_get_max_size doom_dynamic_array_get_max_size
#define dynamic_array_expand doom_dynamic_array_expand
#define dynamic_array_resize doom_dynamic_array_resize
#define dynamic_array_fit doom_dynamic_array_fit
#define dynamic_array_reserve doom_dynamic_array_reserve

#endif /* DOOMSDAY_C_STRIP_PREFIX */


/*****************************************************************************/
/* DOOMSDAY C DYNAMIC ARRAY IMPLEMENTATION                                   */
/*****************************************************************************/
#ifdef DOOMSDAY_C_IMPLEMENTATION

int doom_dynamic_array_get_struct( doom_dynamic_array_struct * da_struct, doom_dynamic_array da ) {
    if (da_struct == NULL || da == NULL) {
        return DOOM_ARGUMENT_IS_NULL;
    }
    doom_dynamic_array_header * da_header = NULL;
    da_header = (doom_dynamic_array_header *)doom_dynamic_array_base_ptr(da);
    if (da_header == NULL) {
        return DOOM_INVALID_ARGUMENT; 
    }
    da_struct->_array = da;
    da_struct->_sizeof_elem = &da_header->_sizeof_elem;
    da_struct->_array_size = &da_header->_array_size;
    da_struct->_array_max_size = &da_header->_array_max_size;
    da_struct->_allocator = &da_header->_allocator;
    da_struct->_deallocator = &da_header->_deallocator;

    return DOOM_SUCCESS;
}

void * doom_dynamic_array_base_ptr( doom_dynamic_array da ) {
    if (da == NULL) {
        return NULL;
    }
    return (void *)((doom_dynamic_array_header *)da - 1);
}

int doom_dynamic_array_init( doom_dynamic_array * da, int64_t sizeof_elem) {
    if (sizeof_elem <= 0) {
        return DOOM_INVALID_ARGUMENT;
    }
    doom_dynamic_array_header * da_header;
    da_header = (doom_dynamic_array_header *)DOOMSDAY_C_DEFAULT_ALLOCATOR(
            sizeof(doom_dynamic_array_header) + 
            sizeof_elem * DOOM_ARRAY_INIT_DEFAULT_SIZE
            );
    if (da_header == NULL) {
        return DOOM_OUT_OF_MEMORY;
    } 

    da_header->_sizeof_elem = sizeof_elem;
    da_header->_array_size = 0;
    da_header->_array_max_size = DOOM_ARRAY_INIT_DEFAULT_SIZE;
    da_header->_allocator = DOOMSDAY_C_DEFAULT_ALLOCATOR;
    da_header->_deallocator = DOOMSDAY_C_DEFAULT_DEALLOCATOR;
    *da = (doom_dynamic_array)(da_header + 1);  

    return DOOM_SUCCESS;
}

int doom_dynamic_array_init_size( doom_dynamic_array * da, int64_t sizeof_elem, int64_t max_size ) {
    if (sizeof_elem <= 0 || max_size <= 0) {
        return DOOM_INVALID_ARGUMENT;
    }
    doom_dynamic_array_header * da_header;
    da_header = (doom_dynamic_array_header *)DOOMSDAY_C_DEFAULT_ALLOCATOR(
            sizeof(doom_dynamic_array_header) + 
            sizeof_elem * max_size
            );
    if (da_header == NULL) {
        return DOOM_OUT_OF_MEMORY;
    } 

    da_header->_sizeof_elem = sizeof_elem;
    da_header->_array_size = 0;
    da_header->_array_max_size = max_size;
    da_header->_allocator = DOOMSDAY_C_DEFAULT_ALLOCATOR;
    da_header->_deallocator = DOOMSDAY_C_DEFAULT_DEALLOCATOR;
    *da = (doom_dynamic_array)(da_header + 1);  

    return DOOM_SUCCESS;
}

// int doom_dynamic_array_init_copy( doom_dynamic_array * da, int64_t sizeof_elem, void const * restrict from, int64_t from_size ) {
//     if (sizeof_elem <= 0 || from_size <= 0 || from == NULL) {
//         return -2;
//     }
//     doom_dynamic_array alloc_ptr = NULL;
//     doom_dynamic_array_struct da_struct;
//     alloc_ptr = DOOM_DYNAMIC_ARRAY_ALLOCATOR(sizeof_elem*from_size*2 + sizeof(int64_t)*3);
//     if (alloc_ptr == NULL) {
//         return -1;
//     }
//     da_struct._array_max_size = alloc_ptr;
//     da_struct._array_size = ((int64_t *) alloc_ptr)+1;
//     da_struct._sizeof_elem = ((int64_t *) alloc_ptr)+2;
//     da_struct._array = (byte *)(((int64_t *) alloc_ptr)+3);
//
//     *da_struct._sizeof_elem = sizeof_elem;
//     *da_struct._array_size = from_size;
//     *da_struct._array_max_size = from_size*2;
//     memcpy(da_struct._array, from, from_size*sizeof_elem);
//     *da = da_struct._array;
//     return 0;
// }

void doom_dynamic_array_deinit( doom_dynamic_array da ) {
    if (da == NULL) {
        return;
    }
    doom_dynamic_array_header * da_header = NULL;
    da_header = doom_dynamic_array_base_ptr(da);
    if (da_header == NULL) {
        return;
    }

    da_header->_deallocator((void *)da_header);
}

void doom_dynamic_array_deinit_ptr( doom_dynamic_array * da_ptr ) {
    if (da_ptr == NULL) {
        return;
    }
    doom_dynamic_array_deinit(*da_ptr);
}

// doom_dynamic_array doom_dynamic_array_new( int64_t const sizeof_elem ) {
//     byte * aux_da = NULL;
//     if(doom_dynamic_array_init((doom_dynamic_array *)&aux_da, sizeof_elem) != 0) {
//         return NULL;
//     }
//     return aux_da;
// }
//
// doom_dynamic_array doom_dynamic_array_new_size( int64_t const sizeof_elem, int64_t const size ) {
//     byte * aux_da = NULL;
//     if(doom_dynamic_array_init_size((doom_dynamic_array *)&aux_da, sizeof_elem, size) != 0) {
//         return NULL;
//     }
//     return aux_da;
// }
//
// doom_dynamic_array doom_dynamic_array_new_copy( int64_t const sizeof_elem, void const * restrict from, int64_t const from_size ) {
//     byte * aux_da = NULL;
//     if(doom_dynamic_array_init_copy((doom_dynamic_array *)&aux_da, sizeof_elem, from, from_size) != 0) {
//         return NULL;
//     }
//     return aux_da;
// }
//
// int64_t doom_dynamic_array_get_elem_size( void const * restrict da ) {
//     if (da == NULL) {
//         return -1;
//     }
//     return *(((int64_t *)da)-1);
// }
//
// int64_t doom_dynamic_array_get_size( void const * restrict da ) {
//     if (da == NULL) {
//         return -1;
//     }
//     return *(((int64_t *)da)-2);
// }
//
// int64_t doom_dynamic_array_get_max_size( void const * restrict da ) {
//     if (da == NULL) {
//         return -1;
//     }
//     return *(((int64_t *)da)-3);
// }
//
// int doom_dynamic_array_expand( doom_dynamic_array * da ) {
//     if (da == NULL) {
//         return -1;
//     }
//     if (*da == NULL) {
//         return -1;
//     }
//     doom_dynamic_array_struct da_struct;
//     if ((doom_dynamic_array_get_struct(&da_struct, *da)) != 0) {
//         return -1;
//     }
//     doom_dynamic_array da_alloc = doom_dynamic_array_new_size(*da_struct._sizeof_elem, *da_struct._array_max_size*2);
//     if (da_alloc == NULL) {
//         return -2;
//     }
//     doom_dynamic_array_struct da_alloc_struct;
//     if ((doom_dynamic_array_get_struct(&da_alloc_struct, da_alloc)) != 0) {
//         return -2;
//     }
//     memcpy(da_alloc_struct._array, da_struct._array, *da_struct._array_size * *da_struct._sizeof_elem);
//     *da_alloc_struct._array_size = *da_struct._array_size;
//     doom_dynamic_array_deinit(*da);
//     *da = da_alloc;
//
//     return 0;
// }
//
// int doom_dynamic_array_resize( doom_dynamic_array * da, int64_t const size ) {
//     if (da == NULL) {
//         return -1;
//     }
//     if (*da == NULL) {
//         return -1;
//     }
//     if (size <= 0) {
//         return -2;
//     }
//
//     doom_dynamic_array_struct da_struct;
//     if ((doom_dynamic_array_get_struct(&da_struct, *da)) != 0) {
//         return -1;
//     }
//
//     while (*da_struct._array_max_size < size) {
//         if ((doom_dynamic_array_expand(da)) != 0) {
//             return -3;
//         }
//         if ((doom_dynamic_array_get_struct(&da_struct, *da)) != 0) {
//             return -3;
//         }
//     }
//     *da_struct._array_size = size;
//
//     return 0;
// }
//
// int doom_dynamic_array_fit( doom_dynamic_array * da ) {
//     if (da == NULL) {
//         return -1;
//     }
//
//     doom_dynamic_array_struct da_struct;
//     if ((doom_dynamic_array_get_struct(&da_struct, *da)) != 0) {
//         return -1;
//     }
//
//     if (*da_struct._array_max_size > *da_struct._array_size) {
//         doom_dynamic_array da_alloc = doom_dynamic_array_new_size(*da_struct._sizeof_elem, *da_struct._array_size);
//         if (da_alloc == NULL) {
//             return -2;
//         }
//         doom_dynamic_array_struct da_alloc_struct;
//         if ((doom_dynamic_array_get_struct(&da_alloc_struct, da_alloc)) != 0) {
//             return -2;
//         }
//         memcpy(da_alloc_struct._array, da_struct._array, *da_struct._array_size * *da_struct._sizeof_elem);
//         *da_alloc_struct._array_size = *da_struct._array_size;
//         doom_dynamic_array_deinit(*da);
//         *da = da_alloc;
//     }
//     return 0;
// }
//
// int doom_dynamic_array_reserve( doom_dynamic_array * da, int64_t const size ) {
//     if (da == NULL) {
//         return -1;
//     }
//     if (*da == NULL) {
//         return -1;
//     }
//     if (size <= 0) {
//         return -2;
//     }
//
//     doom_dynamic_array_struct da_struct;
//     if ((doom_dynamic_array_get_struct(&da_struct, *da)) != 0) {
//         return -1;
//     }
//
//     if (*da_struct._array_max_size < size) {
//         doom_dynamic_array da_alloc = doom_dynamic_array_new_size(*da_struct._sizeof_elem, size);
//         if (da_alloc == NULL) {
//             return -2;
//         }
//         doom_dynamic_array_struct da_alloc_struct;
//         if ((doom_dynamic_array_get_struct(&da_alloc_struct, da_alloc)) != 0) {
//             return -2;
//         }
//         memcpy(da_alloc_struct._array, da_struct._array, *da_struct._array_size * *da_struct._sizeof_elem);
//         *da_alloc_struct._array_size = *da_struct._array_size;
//         doom_dynamic_array_deinit(*da);
//         *da = da_alloc;
//     }
//
//     return 0;
// }
//
// /* Stack and Queue operations */
//
// doom_dynamic_array doom_dynamic_array_back( void const * restrict da ) {
//     if (da == NULL) {
//         return NULL;
//     }
//
//     doom_dynamic_array_struct da_struct;
//     if (doom_dynamic_array_get_struct(&da_struct,(doom_dynamic_array)da) != 0) {
//         return NULL;
//     }
//
//     if (*da_struct._array_size == 0) {
//         return NULL;
//     }
//
//     return &(da_struct._array[(*da_struct._array_size-1) * (*da_struct._sizeof_elem)]);
// }
//
// int doom_dynamic_array_push_back( doom_dynamic_array * da, void const * restrict value ) {
//     if (da == NULL || value == NULL) {
//         return -1;
//     }
//     if (*da == NULL) {
//         return -1;
//     }
//
//     doom_dynamic_array_struct da_struct;
//     if (doom_dynamic_array_get_struct(&da_struct, *da) != 0) {
//         return -1;
//     }
//
//     if (*da_struct._array_size == 0) {
//         return -2;
//     }
//
//     if (*da_struct._array_size+1 > *da_struct._array_max_size) {
//         if (doom_dynamic_array_expand(da) != 0) {
//             return -3;
//         }
//         if (doom_dynamic_array_get_struct(&da_struct, *da) != 0) {
//             return -1;
//         }
//     }
//
//     byte * pos = &(da_struct._array[(*da_struct._array_size) * (*da_struct._sizeof_elem)]);
//     memcpy(pos, value, *da_struct._sizeof_elem);
//     *da_struct._array_size += 1;
//
//     return 0;
// }
//
// int doom_dynamic_array_pop_back( doom_dynamic_array * da ) {
//     if (da == NULL) {
//         return -1;
//     }
//
//     doom_dynamic_array_struct da_struct;
//     if (doom_dynamic_array_get_struct(&da_struct,*da) != 0) {
//         return -1;
//     }
//
//     if (*da_struct._array_size == 0) {
//         return -2;
//     }
//
//     *da_struct._array_size-=1;
//
//     return 0;
// }
//
// doom_dynamic_array doom_dynamic_array_front( void const * restrict da ) {
//     if (da == NULL) {
//         return NULL;
//     }
//
//     doom_dynamic_array_struct da_struct;
//     if (doom_dynamic_array_get_struct(&da_struct,(doom_dynamic_array)da) != 0) {
//         return NULL;
//     }
//
//     if (*da_struct._array_size == 0) {
//         return NULL;
//     }
//
//     return da_struct._array;
// }
//
// int doom_dynamic_array_push_front( doom_dynamic_array * da, void const * restrict value ) {
//     if (da == NULL || value == NULL) {
//         return -1;
//     }
//     if (*da == NULL) {
//         return -1;
//     }
//
//     doom_dynamic_array_struct da_struct;
//     if (doom_dynamic_array_get_struct(&da_struct, *da) != 0) {
//         return -1;
//     }
//
//     if (*da_struct._array_size == 0) {
//         return -2;
//     }
//
//     if (*da_struct._array_size+1 > *da_struct._array_max_size) {
//         if (doom_dynamic_array_expand(da) != 0) {
//             return -3;
//         }
//         if (doom_dynamic_array_get_struct(&da_struct, *da) != 0) {
//             return -1;
//         }
//     }
//
//     memmove(da_struct._array+(*da_struct._sizeof_elem), da_struct._array,
//             (*da_struct._sizeof_elem) * (*da_struct._array_size));
//     memcpy(da_struct._array, value, *da_struct._sizeof_elem);
//     *da_struct._array_size += 1;
//
//     return 0;
// }
//
// int doom_dynamic_array_pop_front( doom_dynamic_array * da ) {
//     if (da == NULL) {
//         return -1;
//     }
//
//     doom_dynamic_array_struct da_struct;
//     if (doom_dynamic_array_get_struct(&da_struct,*da) != 0) {
//         return -1;
//     }
//
//     if (*da_struct._array_size == 0) {
//         return -2;
//     }
//
//     memmove(da_struct._array, da_struct._array+(*da_struct._sizeof_elem),
//             (*da_struct._sizeof_elem) * (*da_struct._array_size));
//     *da_struct._array_size-=1;
//
//     return 0;
// }

#endif /* DOOMSDAY_C_IMPLEMENTATION */


#endif /* DOOMSDAY_C */
