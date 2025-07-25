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

#define DOOMSDAY_C_STRING_IMPLEMENTATION
#define DOOMSDAY_C_STRING_STRIP_PREFIX

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
} doom_string_struct;

typedef char * doom_string ;

/* basic functionality */
int     doom_string_get_struct( doom_string_struct * doom_str, char * string);
void *  doom_string_base_pointer( char * string );
int     doom_string_init( char ** string );
int     doom_string_init_size ( char ** string, size_t const size );
int     doom_string_init_copy ( char ** string, char const * restrict c_string );
void    doom_string_deinit( char * string );
char *  doom_string_factory( void );
char *  doom_string_factory_size( size_t const size );
char *  doom_string_factory_copy ( char const * restrict c_string );
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
#ifdef DOOMSDAY_C_STRING_STRIP_PREFIX
#ifndef DOOMSDAY_C_STRING_STRIP_BARRIER
#define DOOMSDAY_C_STRING_STRIP_BARRIER

#define string_to_struct doom_string_get_struct
#define string_base_pointer doom_string_base_pointer
#define string_init doom_string_init
#define string_init_s doom_string_init_size
#define string_init_c doom_string_init_copy
#define string_deinit doom_string_deinit
#define string_get doom_string_factory
#define string_get_s doom_string_factory_size
#define string_get_c doom_string_factory_copy
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

#endif /* DOOMSDAY_C_STRING_STRIP_BARRIER */
#endif /* DOOMSDAY_C_STRING_STRIP_PREFIX */



/*****************************************************************************/
/* DOOMSDAY C STRING IMPLEMENTATIN                                           */
/*****************************************************************************/
#ifdef DOOMSDAY_C_STRING_IMPLEMENTATION
#ifndef DOOMSDAY_C_STRING_IMPLEMENTATION_BARRIER
#define DOOMSDAY_C_STRING_IMPLEMENTATION_BARRIER

int doom_string_get_struct(doom_string_struct *doom_str, char *string) {
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
    doom_string_struct doom_str;
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

int doom_string_init_size ( char ** string, size_t const size ) {
    void * alloc_ptr = NULL;
    doom_string_struct doom_str;
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

int doom_string_init_copy ( char ** string, char const * restrict c_string ) {
    if (c_string == NULL) {
        return -2;
    }
    void * alloc_ptr = NULL;
    doom_string_struct doom_str;
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
}

char * doom_string_factory( void ) {
    char * aux_str = NULL; 
    if((doom_string_init(&aux_str)) != 0) {
        return NULL;
    }
    
    return aux_str;
}

char * doom_string_factory_size( size_t const size ) {
    char * aux_str = NULL; 
    if((doom_string_init_size(&aux_str, size)) != 0) {
        return NULL;
    }
    
    return aux_str;
}

char * doom_string_factory_copy ( char const * restrict c_string ) {
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
    char * aux_str = doom_string_factory_size(size);
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

    return *(((int64_t *)string)-1);
}

char * doom_string_stpcpy( char ** dst, char const * restrict src ) {
    if (dst == NULL || src == NULL || *dst == NULL) {
        return NULL;
    } 
    if (doom_string_get_max_size(*dst) < doom_string_strlen(src)) {
        doom_string_resize(dst, doom_string_strlen(src));
    }

    return stpcpy(*dst, src);
}

char * doom_string_strcat( char ** dst, char const * restrict src ) {
    if (dst == NULL || src == NULL || *dst == NULL) {
        return NULL;
    }
    if (doom_string_get_max_size(*dst) < 
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
    if (doom_string_get_max_size(*dst) < doom_string_strlen(src)) {
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
    return doom_string_factory_copy(s);
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

    return strpbrk(s, accept);
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

    return strstr(haystack, needle);
}

char * doom_string_strtok( char * restrict str, char const * restrict delim) {
    return strtok(str, delim);
}

char * doom_string_strtok_r( char * restrict str, char const * restrict delim, 
                                                  char ** restrict saveptr) {
    return strtok_r(str, delim, saveptr);
}

#endif /* DOOMSDAY_C_STRING_IMPLEMENTATION_BARRIER */
#endif /* DOOMSDAY_C_STRING_IMPLEMENTATION */

#endif /* DOOMSDAY_C_STRING_H */
