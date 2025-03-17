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
#define DOOMSDAY_C_DYNAMIC_ARRAY_STRIP_PREFIX
#define DOOMSDAY_C_DYNAMIC_ARRAY_IMPLEMENTATION

#ifndef DOOM_C_DYNAMIC_ARRAY_H
#define DOOM_C_DYNAMIC_ARRAY_H


#include <stdint.h>
#include <stdlib.h>
#include <string.h>


#ifndef ALLOCATOR
#define ALLOCATOR(x) malloc(x)
#endif /* ALLOCATOR */
#ifndef DEALLOCATOR
#define DEALLOCATOR(x) free(x)
#endif /* DEALLOCATOR */

#define ARRAY_DEFAULT_SIZE 1

typedef uint8_t byte;
typedef struct doomsday_c_dynamic_array_t {
    byte * _array;
    int64_t * _sizeof_elem;
    int64_t * _array_size;
    int64_t * _array_max_size;
} doom_dynamic_array_struct;

/* basic functionality */ 
int doom_dynamic_array_get_struct( doom_dynamic_array_struct * da_struct, void * da );
void * doom_dynamic_array_get_base_ptr( void * da );
int doom_dynamic_array_init( void ** da , int64_t const sizeof_elem );
int doom_dynamic_array_init_s( void ** da, int64_t const sizeof_elem, int64_t const size );
int doom_dynamic_array_init_c( void ** da, int64_t const sizeof_elem, void const * restrict from, int64_t const from_size );
void * doom_dynamic_array_get( int64_t const sizeof_elem );
void * doom_dynamic_array_get_s( int64_t const sizeof_elem, int64_t const size );
void * doom_dynamic_array_get_c( int64_t const sizeof_elem, void const * restrict from, int64_t const from_size );
void doom_dynamic_array_deinit( void * da );
int64_t doom_dynamic_array_get_elem_size( void const * restrict da );
int64_t doom_dynamic_array_get_size( void const * restrict da );
int64_t doom_dynamic_array_get_max_size( void const * restrict da );


#ifdef DOOMSDAY_C_DYNAMIC_ARRAY_STRIP_PREFIX
#ifndef DOOMSDAY_C_DYNAMIC_ARRAY_STRIP_BARRIER
#define DOOMSDAY_C_DYNAMIC_ARRAY_STRIP_BARRIER

#define dynamic_array_get_struct doom_dynamic_array_get_struct
#define dynamic_arrat_get_base_ptr doom_dynamic_array_get_base_ptr
#define dynamic_array_init doom_dynamic_array_init
#define dynamic_array_init_s doom_dynamic_array_init_s
#define dynamic_array_init_c doom_dynamic_array_init_c
#define dynamic_array_deinit doom_dynamic_array_deinit
#define dynamic_array_get doom_dynamic_array_get
#define dynamic_array_get_c doom_dynamic_array_get_c
#define dynamic_array_get_s doom_dynamic_array_get_s
#define dynamic_array_get_elem_size doom_dynamic_array_get_elem_size
#define dynamic_array_get_size doom_dynamic_array_get_size
#define dynamic_array_get_max_size doom_dynamic_array_get_max_size

#endif /* DOOMSDAY_C_DYNAMIC_ARRAY_STRIP_BARRIER */
#endif /* DOOMSDAY_C_DYNAMIC_ARRAY_STRIP_PREFIX */
#ifdef DOOMSDAY_C_DYNAMIC_ARRAY_IMPLEMENTATION
#ifndef DOOMSDAY_C_DYNAMIC_ARRAY_IMPLEMENTATION_BARRIER
#define DOOMSDAY_C_DYNAMIC_ARRAY_IMPLEMENTATION_BARRIER

int doom_dynamic_array_get_struct( doom_dynamic_array_struct * da_struct, void * da ) { 
    if (da_struct == NULL || da == NULL) {
        return -1;
    }
    da_struct->_array = da;
    da_struct->_sizeof_elem = ((int64_t *)da)-1;
    da_struct->_array_size = ((int64_t *)da)-2;
    da_struct->_array_max_size = ((int64_t *)da)-3;
    return 0;
}

void * doom_dynamic_array_get_base_ptr( void * da ) {
    if (da == NULL) {
        return NULL;
    }
    return ((int64_t *)da)-3; 
}

int doom_dynamic_array_init( void ** da, int64_t sizeof_elem) {
    if (sizeof_elem <= 0) {
        return -2;
    }
    void * alloc_ptr = NULL;     
    doom_dynamic_array_struct da_struct;
    alloc_ptr = ALLOCATOR(sizeof_elem*ARRAY_DEFAULT_SIZE + sizeof(int64_t)*3);
    if (alloc_ptr == NULL) {
        return -1;
    }
    da_struct._array_max_size = alloc_ptr;
    da_struct._array_size = ((int64_t *) alloc_ptr)+1;
    da_struct._sizeof_elem = ((int64_t *) alloc_ptr)+2;
    da_struct._array = (byte *)(((int64_t *) alloc_ptr)+3);

    *da_struct._sizeof_elem = sizeof_elem;
    *da_struct._array_size = 0;
    *da_struct._array_max_size = ARRAY_DEFAULT_SIZE;
    *da = da_struct._array;
    return 0;
}

int doom_dynamic_array_init_s( void ** da, int64_t sizeof_elem, int64_t size ) {
    if (sizeof_elem <= 0 || size <= 0) {
        return -2;
    }
    void * alloc_ptr = NULL;     
    doom_dynamic_array_struct da_struct;
    alloc_ptr = ALLOCATOR(sizeof_elem*size + sizeof(int64_t)*3);
    if (alloc_ptr == NULL) {
        return -1;
    }
    da_struct._array_max_size = alloc_ptr;
    da_struct._array_size = ((int64_t *) alloc_ptr)+1;
    da_struct._sizeof_elem = ((int64_t *) alloc_ptr)+2;
    da_struct._array = (byte *)(((int64_t *) alloc_ptr)+3);

    *da_struct._sizeof_elem = sizeof_elem;
    *da_struct._array_size = 0;
    *da_struct._array_max_size = size;
    *da = da_struct._array;
    return 0;
}

int doom_dynamic_array_init_c( void ** da, int64_t sizeof_elem, void const * restrict from, int64_t from_size ) {
    if (sizeof_elem <= 0 || from_size <= 0 || from == NULL) {
        return -2;
    }
    void * alloc_ptr = NULL;     
    doom_dynamic_array_struct da_struct;
    alloc_ptr = ALLOCATOR(sizeof_elem*from_size + sizeof(int64_t)*3);
    if (alloc_ptr == NULL) {
        return -1;
    }
    da_struct._array_max_size = alloc_ptr;
    da_struct._array_size = ((int64_t *) alloc_ptr)+1;
    da_struct._sizeof_elem = ((int64_t *) alloc_ptr)+2;
    da_struct._array = (byte *)(((int64_t *) alloc_ptr)+3);

    *da_struct._sizeof_elem = sizeof_elem;
    *da_struct._array_size = from_size;
    *da_struct._array_max_size = from_size*2;
    memcpy(da_struct._array, from, from_size*sizeof_elem);
    *da = da_struct._array;
    return 0;
}

void * doom_dynamic_array_get( int64_t const sizeof_elem ) {
    byte * aux_da = NULL;
    if(doom_dynamic_array_init((void **)&aux_da, sizeof_elem) != 0) {
        return NULL;
    }
    return aux_da;
}

void * doom_dynamic_array_get_s( int64_t const sizeof_elem, int64_t const size ) {
    byte * aux_da = NULL;
    if(doom_dynamic_array_init_s((void **)&aux_da, sizeof_elem, size) != 0) {
        return NULL;
    }
    return aux_da;
}

void * doom_dynamic_array_get_c( int64_t const sizeof_elem, void const * restrict from, int64_t const from_size ) {
    byte * aux_da = NULL;
    if(doom_dynamic_array_init_c((void **)&aux_da, sizeof_elem, from, from_size) != 0) {
        return NULL;
    }
    return aux_da;
}

void doom_dynamic_array_deinit( void * da ) {
    if (da == NULL) {
        return;
    } 
    void * base_ptr = doom_dynamic_array_get_base_ptr(da);
    if (base_ptr == NULL) {
        return;
    }
    DEALLOCATOR(base_ptr);
}

int64_t doom_dynamic_array_get_elem_size( void const * restrict da ) {
    if (da == NULL) {
        return -1;
    }
    return *(((int64_t *)da)-1); 
}

int64_t doom_dynamic_array_get_size( void const * restrict da ) {
    if (da == NULL) {
        return -1;
    }
    return *(((int64_t *)da)-2); 
}

int64_t doom_dynamic_array_get_max_size( void const * restrict da ) {
    if (da == NULL) {
        return -1;
    }
    return *(((int64_t *)da)-3); 
}
 
#endif /* DOOMSDAY_C_DYNAMIC_ARRAY_IMPLEMENTATION_BARRIER */
#endif /* DOOMSDAY_C_DYNAMIC_ARRAY_IMPLEMENTATION */
#endif /* DOOM_C_DYNAMIC_ARRAY_H */
