
/******************************************************************************/
/* DoomsDay C                                                                 */
/* Copyright (C) 2025 - v_Alenz                                               */
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

/* #define DOOMSDAY_C_STRING_IMPLEMENTATION */
/* #define DOOMSDAY_C_STRING_STRIP_PREFIX */

/*****************************************************************************/
/* DOOMSDAY C STRING                                                         */
/*****************************************************************************/
#ifndef DOOMSDAY_C_STRING_H
#define  DOOMSDAY_C_STRING_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef ALLOCATOR
#define ALLOCATOR(x) malloc(x)
#endif /* ALLOCATOR */
#ifndef DEALLOCATOR
#define DEALLOCATOR(x) free(x)
#endif /* DEALLOCATOR */

typedef struct doomsday_c_string_t {
    char * _char_array;
    int64_t * _size;
} doom_string;


int doom_string_to_struct( doom_string * doom_str, char * string);
void * doom_string_base_pointer( char * string );
int doom_string_init( char ** string );
int doom_string_init_s ( char ** string, size_t const size );
int doom_string_init_c ( char ** string, char * const c_string );
void doom_string_deinit( char * string );
char * doom_string_get( void );
char * doom_string_get_s( size_t const size );
char * doom_string_get_c ( char * const c_string );
int doom_string_resize( char ** string, size_t const size );
int doom_string_safe_resize( char ** string, size_t const size );
int doom_string_fit( char ** string );
int64_t doom_string_get_max_size( char * const string );


/*****************************************************************************/
/* DOOMSDAY C STRING STRIP PREFIX                                            */
/*****************************************************************************/
#ifdef DOOMSDAY_C_STRING_STRIP_PREFIX
#ifndef DOOMSDAY_C_STRING_STRIP_BARRIER
#define DOOMSDAY_C_STRING_STRIP_BARRIER

#define string_to_struct doom_string_to_struct
#define string_base_pointer doom_string_base_pointer
#define string_init doom_string_init
#define string_init_s doom_string_init_s
#define string_init_c doom_string_init_c
#define string_deinit doom_string_deinit
#define string_get doom_string_get
#define string_get_s doom_string_get_s
#define string_get_c doom_string_get_c
#define string_resize doom_string_resize
#define string_safe_resize doom_string_safe_resize
#define string_fit doom_string_fit
#define string_get_max_size doom_string_get_max_size

#endif /* DOOMSDAY_C_STRING_STRIP_BARRIER */
#endif /* DOOMSDAY_C_STRING_STRIP_PREFIX */

#endif /* DOOMSDAY_C_STRING_H */


/*****************************************************************************/
/* DOOMSDAY C STRING IMPLEMENTATIN                                           */
/*****************************************************************************/
#ifdef DOOMSDAY_C_STRING_IMPLEMENTATION

int doom_string_to_struct(doom_string *doom_str, char *string) {
    if (doom_str == NULL || string == NULL) {
        return -1;
    } 
    doom_str->_char_array = string;
    doom_str->_size = ((int64_t *)string)-1;
    return 0;
}

void * doom_string_base_pointer( char * string ) {
    if (string == NULL) {
        return NULL;
    }
    return ((int64_t *) string)-1;
}

int doom_string_init( char ** string ) {
    void * alloc_ptr = NULL;
    doom_string doom_str;
    memset(&doom_str, 0, sizeof(doom_str));
    alloc_ptr = ALLOCATOR(sizeof(char) + sizeof(int64_t));
    if (alloc_ptr == NULL) {
        return -1;
    }
    doom_str._size = (int64_t*)alloc_ptr;
    doom_str._char_array = (char*)(doom_str._size+1);

    *doom_str._size = 0;
    *doom_str._char_array = 0;
    
    *string = doom_str._char_array;
    return 0;
}

int doom_string_init_s ( char ** string, size_t const size ) {
    void * alloc_ptr = NULL;
    doom_string doom_str;
    memset(&doom_str, 0, sizeof(doom_str));
    alloc_ptr = ALLOCATOR((sizeof(char)*size+1) + sizeof(int64_t));
    if (alloc_ptr == NULL) {
        return -1;
    }
    doom_str._size = (int64_t*)alloc_ptr;
    doom_str._char_array = (char*)(doom_str._size+1);
    
    *doom_str._size = size;
    memset(doom_str._char_array, 0, *doom_str._size+1);

    *string = doom_str._char_array;
    return 0;
}

int doom_string_init_c ( char ** string, char * const c_string ) {
    void * alloc_ptr = NULL;
    doom_string doom_str;
    size_t c_string_size = strlen(c_string);
    memset(&doom_str, 0, sizeof(doom_str));
    alloc_ptr = ALLOCATOR((sizeof(char)*c_string_size+1) + sizeof(int64_t));
    if (alloc_ptr == NULL) {
        return -1;
    }
    doom_str._size = (int64_t*)alloc_ptr;
    doom_str._char_array = (char*)(doom_str._size+1);
    
    *doom_str._size = c_string_size;
    memset(doom_str._char_array, 0, *doom_str._size+1);
    memcpy(doom_str._char_array, c_string, *doom_str._size);

    *string = doom_str._char_array;
    return 0;
}

void doom_string_deinit( char * string ) {
    if (string == NULL) {
        return;
    }
    void * aux_ptr = doom_string_base_pointer(string);
    if (aux_ptr == NULL) {
        return;
    }
    DEALLOCATOR(aux_ptr);
    aux_ptr = NULL;
}

char * doom_string_get( void ) {
    char * aux_str = NULL; 
    doom_string_init(&aux_str);
    
    return aux_str;
}

char * doom_string_get_s( size_t const size ) {
    char * aux_str = NULL; 
    doom_string_init_s(&aux_str, size);
    
    return aux_str;
}

char * doom_string_get_c ( char * const c_string ) {
    char * aux_str = NULL; 
    doom_string_init_c(&aux_str, c_string);
    
    return aux_str;
}

int doom_string_resize( char ** string, size_t const size ) {
    if (string == NULL) {
        return -1;
    } 
    char * aux_str = doom_string_get_s(size);
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
    if (size <= strlen(*string)) {
        return -2;
    }
    doom_string_resize(string, size);
    return 0;
}

int doom_string_fit( char ** string ) {
    if (string == NULL) {
        return -1;
    }
    doom_string_resize(string, strlen(*string));

    return 0;
}

int64_t doom_string_get_max_size( char * const string ) {
    if (string == NULL) {
        return -1;
    }
    return *(((int64_t *)string)-1);
}



#endif /* DOOMSDAY_C_STRING_IMPLEMENTATION */

