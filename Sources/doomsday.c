#ifndef DOOMSDAY_C
#define DOOMSDAY_C


#include <stdint.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*****************************************************************************/
/* DOOMSDAY C DEFINITIONS                                                    */
/*****************************************************************************/

#define DOOMSDAY_C_IMPLEMENTATION
#define DOOMSDAY_C_STRIP_PREFIX

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
    DOOM_SUCCESS,
    DOOM_OUT_OF_MEMORY,
    DOOM_ARGUMENT_IS_NULL
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
    if (arena == NULL || memory == NULL || size == 0) {
        return DOOM_ARGUMENT_IS_NULL; 
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
/* DOOMSDAY C STRING                                                         */
/*****************************************************************************/

#define DOOM_STRING_HEADER_FIELDS \

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
int     doom_string_get_struct( doom_string_struct * string_struct, char * string);
void *  doom_string_base_pointer( char * string );
int     doom_string_init( char ** string );
int     doom_string_init_size ( char ** string, size_t const size );
int     doom_string_init_copy ( char ** string, char const * restrict c_string );
int     doom_string_init_alloc( char ** string, void *(*custom_allocator)(size_t), void (*custom_deallocator)(void *));
int     doom_string_init_size_alloc ( char ** string, size_t const size, void *(*custom_allocator)(size_t), void (*custom_deallocator)(void *));
int     doom_string_init_copy_alloc ( char ** string, char const * restrict c_string, void *(*custom_allocator)(size_t), void (*custom_deallocator)(void *));
void    doom_string_deinit( char * string );
char *  doom_string_new( void );
char *  doom_string_new_size( size_t const size );
char *  doom_string_new_copy ( char const * restrict c_string );
int     doom_string_resize( char ** string, size_t const size );
int     doom_string_safe_resize( char ** string, size_t const size );
int     doom_string_fit( char ** string );
int64_t doom_string_get_max_size( char const * string );
/* string.h funcitons weappers */
char *  doom_string_stpcpy( char ** dst, char const * restrict src );
char *  doom_string_strcat( char ** dst, char const * restrict src );
char *  doom_string_strchr( char * dst, int c );
int     doom_string_strcmp( char const * s1, char const * s2 );
int     doom_string_strcoll( char const * s1, char const * s2 );
int     doom_string_strcoll_l( char const * s1, char const * s2,
                               locale_t locale );
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
#define string_new_size doom_string_new_size
#define string_new_copy doom_string_new_copy
#define string_resize doom_string_resize
#define string_safe_resize doom_string_safe_resize
#define string_fit doom_string_fit
#define string_get_max_size doom_string_get_max_size
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

int doom_string_get_struct(doom_string_struct * string_struct, char * string) {
    if (string_struct == NULL || string == NULL) {
        return DOOM_ARGUMENT_IS_NULL;
    }
    doom_string_header * aux_header = ((doom_string_header *)string) - 1;
    string_struct->_size = &aux_header->_size;
    string_struct->_allocator = &aux_header->_allocator;
    string_struct->_deallocator = &aux_header->_deallocator;
    string_struct->_char_array = string;
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

char * doom_string_new( void ) {
    char * aux_str = NULL;
    if((doom_string_init(&aux_str)) != 0) {
        return NULL;
    }

    return aux_str;
}

 char * doom_string_new_size( size_t const size ) {
     char * aux_str = NULL;
     if((doom_string_init_size(&aux_str, size)) != 0) {
         return NULL;
     }

     return aux_str;
 }

 char * doom_string_new_copy ( char const * restrict c_string ) {
     char * aux_str = NULL;
     if((doom_string_init_copy(&aux_str, c_string)) != 0) {
         return NULL;
     }

     return aux_str;
 }

int doom_string_resize( char ** string, size_t const size ) {
    if (string == NULL) {
        return -1;
    }
    if (*string == NULL) {
        return -2;
    }
    char * aux_str = doom_string_new_size(size);
    int64_t aux_len = doom_string_get_max_size(*string);
    memcpy(aux_str, *string, (aux_len < size) ? aux_len : size);
    doom_string_deinit(*string);

    *string = aux_str;
    return 0;
}

int doom_string_safe_resize( char ** string, size_t const size ) {
    if (string == NULL) {
        return -1;
    }
    if (*string == NULL) {
        return -2;
    }
    if (size <= strlen(*string)) {
        return -3;
    }

    doom_string_resize(string, size);
    return 0;
}

int doom_string_fit( char ** string ) {
    if (string == NULL) {
        return -1;
    }
    if (*string == NULL) {
        return -2;
    }

    return doom_string_resize(string, strlen(*string));
}

int64_t doom_string_get_max_size( char const * string ) {
    if (string == NULL) {
        return -1;
    }

    // TODO: Reimplementare!
    return DOOM_SUCCESS;
}

// char * doom_string_stpcpy( char ** dst, char const * restrict src ) {
//     if (dst == NULL || src == NULL || *dst == NULL) {
//         return NULL;
//     }
//     if (doom_string_get_max_size(*dst) < doom_string_strlen(src)) {
//         doom_string_resize(dst, doom_string_strlen(src));
//     }
//
//     return stpcpy(*dst, src);
// }
//
// char * doom_string_strcat( char ** dst, char const * restrict src ) {
//     if (dst == NULL || src == NULL || *dst == NULL) {
//         return NULL;
//     }
//     if (doom_string_get_max_size(*dst) <
//         doom_string_strlen(*dst) + doom_string_strlen(src)) {
//         doom_string_resize(
//                 dst,
//                 doom_string_strlen(*dst)+doom_string_strlen(src)
//                 );
//     }
//
//     return strcat(*dst, src);
// }
//
// char * doom_string_strchr( char * dst, int c ) {
//     if (dst == NULL) {
//         return NULL;
//     }
//
//     return strchr(dst, c);
// }
//
// int doom_string_strcmp( char const * s1, char const * s2 ) {
//     if (s1 == NULL || s2 == NULL) {
//         return -2;
//     }
//     int aux = strcmp(s1, s2);
//
//     return (aux<0) ? -1 : (aux>0) ? 1 : 0;
// }
//
// int doom_string_strcoll( char const * s1, char const * s2 ) {
//     if (s1 == NULL || s2 == NULL) {
//         return -2;
//     }
//     int aux = strcoll(s1, s2);
//
//     return (aux<0) ? -1 : (aux>0) ? 1 : 0;
// }
//
// int doom_string_strcoll_l( char const * s1, char const * s2,
//                            locale_t locale ) {
//     if (s1 == NULL || s2 == NULL) {
//         return -2;
//     }
//     int aux = strcoll_l(s1, s2, locale);
//
//     return (aux<0) ? -1 : (aux>0) ? 1 : 0;
// }
//
// char * doom_string_strcpy( char ** dst, char const * restrict src ) {
//     if (dst == NULL || src == NULL || *dst == NULL) {
//         return NULL;
//     }
//     if (doom_string_get_max_size(*dst) < doom_string_strlen(src)) {
//         doom_string_resize(dst, doom_string_strlen(src));
//     }
//
//     return strcpy(*dst, src);
// }
//
// size_t doom_string_strcspn( char const * s, char const * reject ) {
//     if (s == NULL || reject == NULL) {
//         return 0;
//     }
//
//     return strcspn(s, reject);
// }
//
// char * doom_string_strdup( char const * s ) {
//     return doom_string_new_copy(s);
// }
//
// size_t doom_string_strlen( char const * s ) {
//     if (s == NULL) {
//         return 0;
//     }
//
//     return strlen(s);
// }
//
// char * doom_string_strpbrk( char const * s, char const * accept ) {
//     if (s == NULL || accept == NULL) {
//         return 0;
//     }
//
//     return strpbrk(s, accept);
// }
//
// size_t doom_string_strspn( char const * s, char const * accept ) {
//     if (s == NULL || accept == NULL) {
//         return 0;
//     }
//
//     return strspn(s, accept);
// }
//
// char * doom_string_strstr( char const * haystack, char const * needle ) {
//     if (haystack == NULL || needle == NULL) {
//         return NULL;
//     }
//     if (doom_string_strlen(needle) > doom_string_strlen(haystack)) {
//         return NULL;
//     }
//
//     return strstr(haystack, needle);
// }
//
// char * doom_string_strtok( char * restrict str, char const * restrict delim) {
//     return strtok(str, delim);
// }
//
// char * doom_string_strtok_r( char * restrict str, char const * restrict delim,
//                                                   char ** restrict saveptr) {
//     return strtok_r(str, delim, saveptr);
// }

#endif /* DOOMSDAY_C_IMPLEMENTATION */

#endif /* DOOMSDAY_C */
