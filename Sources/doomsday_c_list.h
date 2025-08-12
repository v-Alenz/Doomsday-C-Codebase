/******************************************************************************/
/* DoomsDay C                                                                 */
/* Copyright (C) 2025 - v-Alenz                                               */
/*                                                                            */
/* This program is DEALLOCATOR software: you can redistribute it and/or modify       */
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

#define DOOMSDAY_C_LIST_IMPLEMENTATION
#define DOOMSDAY_C_LIST_STRIP_PREFIX

/******************************************************************************/
/*  DOOMSDAY C LIST                                                           */
/******************************************************************************/
#ifndef DOOMSDAY_C_LIST_H
#define DOOMSDAY_C_LIST_H

#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>

#ifndef ALLOCATOR
#define ALLOCATOR(x) malloc(x)
#endif /* ALLOCATOR */
#ifndef DEALLOCATOR
#define DEALLOCATOR(x) free(x)
#endif /* DEALLOCATOR */

/* doom list node */
typedef struct doomsday_c_list_node_t {
    void * payload;
    struct doomsday_c_list_node_t * next;
    struct doomsday_c_list_node_t * prev;
} doom_list_node;

/* doom list sentry */
typedef struct doomsday_c_list_sentry_t {
    uint64_t _data_size;
    uint64_t _size;
    struct doomsday_c_list_node_t * first;
    struct doomsday_c_list_node_t * last;
} doom_list_sentry;

/* doom list */
typedef doom_list_sentry doom_list;


/* list nodes functionality */
doom_list_node * doom_list_node_new( void );
int    doom_list_node_init( doom_list_node * node, uint64_t size);
void   doom_list_node_deinit( doom_list_node * node);
int    doom_list_node_assign( doom_list_node * dest, void * payload, uint64_t size );
void * doom_list_node_get( doom_list_node * node );
int    doom_list_node_insert_after( doom_list_node * ref, doom_list_node * node );
int    doom_list_node_insert_before( doom_list_node * ref, doom_list_node * node );
/* list (sentry) functional */
doom_list * doom_list_new( uint64_t data_size );
int    doom_list_init( doom_list ** list, uint64_t data_size );
int    doom_list_push_front( doom_list * list, void * value );
int    doom_list_push_back( doom_list * list, void * value );
int    doom_list_pop_front( doom_list * list );
int    doom_list_pop_back( doom_list * list );
void * doom_list_front( doom_list * list );
void * doom_list_back( doom_list * list );
void   doom_list_deinit( doom_list * list );


/*****************************************************************************/
/* DOOMSDAY C LIST STRIP PREFIX                                              */
/*****************************************************************************/
#ifdef DOOMSDAY_C_LIST_STRIP_PREFIX
#ifndef DOOMSDAY_C_LIST_STRIP_BARRIER
#define DOOMSDAY_C_LIST_STRIP_BARRIER

#define list_node_new doom_list_node_new
#define list_node_init doom_list_node_init
#define list_node_deinit doom_list_node_deinit
#define list_node_assign doom_list_node_assign
#define list_node_get doom_list_node_get
#define list_node_insert_after doom_list_node_insert_after
#define list_node_insert_before doom_list_node_insert_before
#define list_new doom_list_new
#define list_init doom_list_init
#define list_push_front doom_list_push_front
#define list_push_back doom_list_push_back
#define list_pop_front doom_list_pop_front
#define list_pop_back doom_list_pop_back
#define list_front doom_list_front
#define list_back doom_list_back
#define list_deinit doom_list_deinit

#endif /* DOOMSDAY_C_LIST_STRIP_PREFIX */
#endif /* DOOMSDAY_C_LIST_STRIP_BARRIER */


/******************************************************************************/
/*  DOOMSDAY C LIST NODE - IMPLEMENTATION                                     */
/******************************************************************************/
#ifdef DOOMSDAY_C_LIST_IMPLEMENTATION
#ifndef DOOMSDAY_C_LIST_IMPLEMENTATION_BARRIER
#define DOOMSDAY_C_LIST_IMPLEMENTATION_BARRIER

doom_list_node * doom_list_node_new( void ) {
    doom_list_node * node = (doom_list_node *) ALLOCATOR(sizeof(doom_list_node));
    if (node == NULL) {
        return node;
    }
    node->payload = NULL;
    node->next = NULL;
    node->prev = NULL;
    return node;
}

int doom_list_node_init( doom_list_node * node, uint64_t size) {
    if (node == NULL) {
        return -1;
    }
    node->payload = ALLOCATOR(size);
    if (node->payload == NULL) {
        return -1;
    }
    return size;
}

void doom_list_node_deinit( doom_list_node * node ) {
    if (node == NULL) {
        return;
    }
    if (node->payload != NULL) {
        DEALLOCATOR(node->payload);
    }
    DEALLOCATOR(node);
}

int doom_list_node_assign( doom_list_node * dest, void * payload, uint64_t size) {
    if (dest == NULL) {
        return -1;
    }
    if (dest->payload == NULL) {
        return -1;
    }
    if (payload == NULL) {
        return -1;
    }
    memcpy(dest->payload, payload, size);
    return 0;
}

void * doom_list_node_get( doom_list_node * node ) {
    if (node == NULL) {
        return NULL;
    }
    return node->payload;
}

int doom_list_node_insert_after( doom_list_node * ref, doom_list_node * node ) {
    doom_list_node * aux;
    aux = NULL;

    if (ref == NULL || node == NULL) {
        return -1;
    }
    if (ref->next != NULL) {
        aux = ref->next;
    }
    ref->next = node;
    node->prev = ref;
    if (aux != NULL) {
        node->next = aux;
        aux->prev = node;
    }
    return 0;
}

int doom_list_node_insert_before( doom_list_node * ref, doom_list_node * node ) {
    doom_list_node * aux;
    aux = NULL;

    if (ref == NULL || node == NULL) {
        return -1;
    }
    if (ref->prev != NULL) {
        aux = ref->next;
    }
    ref->prev = node;
    node->next = ref;
    if (aux != NULL) {
        node->prev = aux;
        aux->next = node;
    }
    return 0;
}

doom_list * doom_list_new( uint64_t data_size ) {
    doom_list * list = (doom_list *) ALLOCATOR(sizeof(doom_list));
    if (list == NULL) {
        return NULL;
    }
    list->_size = 0;
    list->_data_size = data_size;
    list->first = NULL;
    list->last = NULL;
    return list;
}

int doom_list_init( doom_list ** list, uint64_t data_size ) {
    doom_list * list_aux;
    list_aux = (doom_list *) ALLOCATOR(sizeof(doom_list));
    if (list == NULL) {
        return -1;
    }
    list_aux->_size = 0;
    list_aux->_data_size = data_size;
    list_aux->first = NULL;
    list_aux->last = NULL;
    *list = list_aux;
    return 0;
}

int doom_list_push_front( doom_list * list, void * value ) {
    if (list == NULL) {
        return -1;
    }
    if (value == NULL) {
        return -1;
    }
    doom_list_node * new_node = doom_list_node_new();
    if (new_node == NULL) {
        return -1;
    }
    if ((doom_list_node_init(new_node, list->_data_size)) == -1) {
        return -1;
    }
    if ((doom_list_node_assign(new_node, value, list->_data_size)) == -1) {
        return -1;
    }
    if (list->_size == 0) {
        list->first = new_node;
        list->last = new_node;
        list->_size++;
        return 0;
    }
    list->first->prev = new_node;
    new_node->next = list->first;
    list->first = new_node;
    list->_size++;
    return 0;
}

int doom_list_push_back( doom_list * list, void * value ) {
    if (list == NULL) {
        return -1;
    }
    if (value == NULL) {
        return -1;
    }
    doom_list_node * new_node = doom_list_node_new();
    if (new_node == NULL) {
        return -1;
    }
    if ((doom_list_node_init(new_node, list->_data_size)) == -1) {
        return -1;
    }
    if ((doom_list_node_assign(new_node, value, list->_data_size)) == -1) {
        return -1;
    }
    if (list->_size == 0) {
        list->first = new_node;
        list->last = new_node;
        list->_size++;
        return 0;
    }
    list->last->next = new_node;
    new_node->prev = list->last;
    list->last = new_node;
    list->_size++;
    return 0;
}

int doom_list_pop_front( doom_list * list ) {
    doom_list_node * aux;
    if (list == NULL) {
        return -1;
    }
    if (list->_size == 0) {
        return -1;
    }
    aux = list->first;
    list->first = list->first->next;
    doom_list_node_deinit(aux);
    list->_size--;
    if (list->_size == 0) {
        list->last = NULL;
    }
    return 0;
}

int doom_list_pop_back( doom_list * list ) {
    doom_list_node * aux;
    if (list == NULL) {
        return -1;
    }
    if (list->_size == 0) {
        return -1;
    }
    aux = list->last;
    list->last = list->last->prev;
    doom_list_node_deinit(aux);
    list->_size--;
    if (list->_size == 0) {
        list->first = NULL;
    }
    return 0;
}

void * doom_list_front( doom_list * list ) {
    if (list == NULL) {
        return NULL;
    }
    if (list->_size == 0) {
        return NULL;
    }
    return list->first->payload;
}

void * doom_list_back( doom_list * list ) {
    if (list == NULL) {
        return NULL;
    }
    if (list->_size == 0) {
        return NULL;
    }
    return list->last->payload;
}

void doom_list_deinit( doom_list * list ) {
    if (list == NULL) {
        return;
    }
    while ((doom_list_pop_front(list)) == 0) {}
    DEALLOCATOR(list);
}


#endif /* DOOMSDAY_C_LIST_IMPLEMENTATION */
#endif /* DOOMSDAY_C_LIST_IMPLEMENTATION_BARRIER */

#endif /* DOOMSDAY_C_LIST_H */
