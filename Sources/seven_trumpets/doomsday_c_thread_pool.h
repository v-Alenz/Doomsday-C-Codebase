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

#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#define DOOMSDAY_C_THREAD_POOL_IMPLEMENTATION
#define DOOMSDAY_C_THREAD_POOL_STRIP_PREFIX

/*****************************************************************************/
/* DOOMSDAY C THREAD_POOL                                                    */
/*****************************************************************************/
#ifndef DOOMSDAY_C_THREAD_POOL_H
#define  DOOMSDAY_C_THREAD_POOL_H


#include <stdint.h>


#define DOOMSDAY_C_THREADS_IMPLEMENTATION
#include "../seven_seals/doomsday_c_threads.h"
#define DOOMSDAY_C_DYNAMIC_ARRAY_IMPLEMENTATION
#include "../seven_seals/doomsday_c_dynamic_array.h"
#define DOOMSDAY_C_LIST_IMPLEMENTATION
#include "../seven_seals/doomsday_c_list.h"


#ifndef DOOM_THREAD_POOL_ALLOCATOR
    #ifdef DOOM_ALLOCATOR
        #define DOOM_THREAD_POOL_ALLOCATOR(x) DOOM_ALLOCATOR(x)
    #else
        #define DOOM_THREAD_POOL_ALLOCATOR(x) malloc(x)
    #endif /* DOOM_ALLOCATOR */
#endif /* DOOM_THREAD_POOL_ALLOCATOR */

#ifndef DOOM_THREAD_POOL_DEALLOCATOR
    #ifdef DOOM_DEALLOCATOR
        #define DOOM_THREAD_POOL_DEALLOCATOR(x) DOOM_DEALLOCATOR(x)
    #else
        #define DOOM_THREAD_POOL_DEALLOCATOR(x) free(x)
    #endif /* DOOM_DEALLOCATOR */
#endif /* DOOM_THREAD_POOL_DEALLOCATOR */


typedef struct doom_thread_pool_task_t {
    void * (* function_ptr)( void *);
    void * function_args;
} doom_thread_pool_task;

typedef struct doom_thread_pool_t {
    int64_t threads_number;
    doom_thread * threads;
    doom_list * tasks_queue;
    pthread_mutex_t tasks_queue_mutex;
    void * (* specialization_function_ptr)( void *);
} doom_thread_pool;


int doom_thread_pool_init( 
        doom_thread_pool * thread_pool, 
        int64_t threads_count,
        void * (* specialization_function)( void *)
        );

void doom_thread_pool_deinit( doom_thread_pool * thread_pool );

int doom_thread_pool_submit( 
        doom_thread_pool * pool,
        void * (* function)( void *),
        void * args
        );


/*****************************************************************************/
/* DOOMSDAY C THREAD_POOL STRIP PREFIX                                       */
/*****************************************************************************/
#ifdef DOOMSDAY_C_THREAD_POOL_STRIP_PREFIX
#ifndef DOOMSDAY_C_THREAD_POOL_STRIP_BARRIER
#define DOOMSDAY_C_THREAD_POOL_STRIP_BARRIER


#endif /* DOOMSDAY_C_THREAD_POOL_STRIP_BARRIER */
#endif /* DOOMSDAY_C_THREAD_POOL_STRIP_PREFIX */


/*****************************************************************************/
/* DOOMSDAY C THREAD POOL IMPLEMENTATIN                                      */
/*****************************************************************************/
#ifdef DOOMSDAY_C_THREAD_POOL_IMPLEMENTATION
#ifndef DOOMSDAY_C_THREAD_POOL_IMPLEMENTATION_BARRIER
#define DOOMSDAY_C_THREAD_POOL_IMPLEMENTATION_BARRIER


void * __doom_thread_pool_start( void * args ) {

    doom_thread_pool * this_pool = (doom_thread_pool *) args;
    doom_thread_pool_task current_task;
    doom_thread_pool_task * current_task_ptr = &current_task;
    doom_thread_pool_task * aux = NULL;

    while (1) {
        /* Task Pooling */
        pthread_mutex_lock(&this_pool->tasks_queue_mutex);
        if (this_pool->tasks_queue->_size == 0) {
            pthread_mutex_unlock(&this_pool->tasks_queue_mutex);
            continue;
        }
        aux = (doom_thread_pool_task *)doom_list_front(this_pool->tasks_queue); 
        if (aux == NULL) {
            pthread_mutex_unlock(&this_pool->tasks_queue_mutex);
            continue;
        }
        memcpy(current_task_ptr, aux, sizeof(doom_thread_pool_task));
        doom_list_pop_front(this_pool->tasks_queue);
        pthread_mutex_unlock(&this_pool->tasks_queue_mutex);
        /* Task Execution */
        current_task.function_ptr(current_task.function_args);
    }

    return NULL;
}


int doom_thread_pool_init( 
        doom_thread_pool * thread_pool, 
        int64_t threads_count, 
        void * (* specialization_function)( void *)
        ) {
    if (thread_pool == NULL) {
        return -1;
    }
    /* thread_pool->threads_number */
    if (threads_count <= 0) {
        return -1;
    } 
    thread_pool->threads_number = threads_count;

    /* thread_pool->specialization_function_ptr */
    thread_pool->specialization_function_ptr = specialization_function;

    /* thread_pool->tasks_queue */
    thread_pool->tasks_queue = doom_list_new( 
            sizeof(doom_thread_pool_task)
            );

    /* thread_pool->tasks_queue_mutex */
    pthread_mutex_init(&thread_pool->tasks_queue_mutex, NULL);

    /* thread_pool->threads */
    thread_pool->threads = doom_dynamic_array_new_size(
            thread_pool->threads_number,
            sizeof(doom_thread)
            );
    for (int i=0; i<thread_pool->threads_number; i++) {
        doom_thread_create(
                &thread_pool->threads[i], 
                __doom_thread_pool_start, 
                (void *)thread_pool);
    }

    return 0;
}

void doom_thread_pool_deinit( doom_thread_pool * thread_pool ) {

    if (thread_pool == NULL) {
        return;
    }

    /* thread_pool->threads */
    /* TODO: Terminate threads and join them */
    doom_dynamic_array_deinit(thread_pool->threads);

    /* thread_pool->tasks_queue_mutex */
    pthread_mutex_destroy(&thread_pool->tasks_queue_mutex);

    /* thread_pool->tasks_queue */
    doom_list_deinit(thread_pool->tasks_queue);
}

int doom_thread_pool_submit( 
        doom_thread_pool * pool,
        void * (* function)( void *),
        void * args
        ) {

    if (pool == NULL) {
        return -1;
    }
    if (function == NULL && pool->specialization_function_ptr == NULL) {
        return -1;
    }
    
    /* Generating Task */
    doom_thread_pool_task * new_task = NULL;
    new_task = (doom_thread_pool_task *) DOOM_THREAD_POOL_ALLOCATOR(
            sizeof(doom_thread_pool_task));
    if (new_task == NULL) {
        return -1;
    }

    if (function == NULL) {
       new_task->function_ptr = pool->specialization_function_ptr; 
    } else {
        new_task->function_ptr = function;
    }
    new_task->function_args = args;

    /* Task Enqueuing */ 
    int result = 0;
    pthread_mutex_lock(&pool->tasks_queue_mutex);
    result = doom_list_push_back(pool->tasks_queue, (void *)new_task);
    pthread_mutex_unlock(&pool->tasks_queue_mutex);
    if (result != 0) {
        return -1;
    }
    DOOM_THREAD_POOL_DEALLOCATOR(new_task);

    return 0;
}


#endif /* DOOMSDAY_C_THREAD_POOL_IMPLEMENTATION_BARRIER */
#endif /* DOOMSDAY_C_THREAD_POOL_IMPLEMENTATION */

#endif /* DOOMSDAY_C_THREAD_POOL_H */
