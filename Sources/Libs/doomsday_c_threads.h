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

/*****************************************************************************/
/* DOOMSDAY C THREAD                                                         */
/*****************************************************************************/
#ifndef DOOMSDAY_C_THREAD_H
#define  DOOMSDAY_C_THREAD_H

#define DOOMSDAY_C_THREAD_IMPLEMENTATION
#define DOOMSDAY_C_THREAD_STRIP_PREFIX


#include <stdlib.h>
#include <pthread.h>


#ifndef DOOM_THREAD_ALLOCATOR
    #ifdef DOOM_ALLOCATOR
        #define DOOM_THREAD_ALLOCATOR(x) DOOM_ALLOCATOR(x)
    #else
        #define DOOM_THREAD_ALLOCATOR(x) malloc(x)
    #endif /* DOOM_ALLOCATOR */
#endif /* DOOM_THREAD_ALLOCATOR */

#ifndef DOOM_THREAD_DEALLOCATOR
    #ifdef DOOM_DEALLOCATOR
        #define DOOM_THREAD_DEALLOCATOR(x) DOOM_DEALLOCATOR(x)
    #else
        #define DOOM_THREAD_DEALLOCATOR(x) free(x)
    #endif /* DOOM_DEALLOCATOR */
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


typedef pthread_t doom_thread;

int doom_thread_create( doom_thread * restrict newthread, 
        void *(*start_routine)(void *), void *restrict arg );

int doom_thread_create_detached( doom_thread * restrict newthread, 
        void *(*start_routine)(void *), void *restrict arg );

doom_thread * doom_thread_new( void *(*start_routine)(void *), 
        void *restrict arg );

doom_thread * doom_thread_new_detached( void *(*start_routine)(void *), 
        void *restrict arg );

void * doom_thread_join( doom_thread * thread );

int doom_thread_detach( doom_thread * thread );

void doom_thread_deinit( void * result );

void doom_thread_result_deinit( void * result );


/*****************************************************************************/
/* DOOMSDAY C THREAD STRIP PREFIX                                            */
/*****************************************************************************/
#ifdef DOOMSDAY_C_THREAD_STRIP_PREFIX
#ifndef DOOMSDAY_C_THREAD_STRIP_BARRIER
#define DOOMSDAY_C_THREAD_STRIP_BARRIER

#define thread_create doom_thread_create
#define thread_create_detached doom_thread_create_detached
#define thread_new doom_thread_new
#define thread_new_detached doom_thread_new_detached
#define thread_join doom_thread_join
#define thread_detach doom_thread_detach
#define thread_deinit  doom_thread_deinit
#define thread_result_deinit  doom_thread_result_deinit

#endif /* DOOMSDAY_C_THREAD_STRIP_BARRIER */
#endif /* DOOMSDAY_C_THREAD_STRIP_PREFIX */


/*****************************************************************************/
/* DOOMSDAY C THREAD IMPLEMENTATIN                                           */
/*****************************************************************************/
#ifdef DOOMSDAY_C_THREAD_IMPLEMENTATION
#ifndef DOOMSDAY_C_THREAD_IMPLEMENTATION_BARRIER
#define DOOMSDAY_C_THREAD_IMPLEMENTATION_BARRIER


int doom_thread_create( doom_thread * restrict newthread, 
        void *(*start_routine)(void *), void *restrict arg ) {
    int result = 0;
    result = pthread_create(newthread, NULL, start_routine, arg);
    return result;
}

int doom_thread_create_detached( doom_thread * restrict newthread, 
        void *(*start_routine)(void *), void *restrict arg ) {
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

doom_thread * doom_thread_new_detached( void *(*start_routine)(void *), 
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


#endif /* DOOMSDAY_C_THREAD_IMPLEMENTATION_BARRIER */
#endif /* DOOMSDAY_C_THREAD_IMPLEMENTATION */

#endif /* DOOMSDAY_C_THREAD_H */
