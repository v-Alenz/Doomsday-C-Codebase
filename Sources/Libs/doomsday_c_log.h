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
#define DOOMSDAY_C_ENABLE_DEFAULT_LOGGER

/*****************************************************************************/
/* DOOMSDAY C DYNAMIC ARRAY                                                  */
/*****************************************************************************/
#ifndef DOOM_C_LOG_H
#define DOOM_C_LOG_H


#include <stdint.h>
#include <stdbool.h>


#ifndef DOOM_LOG_ALLOCATOR
    #ifdef DOOM_ALLOCATOR
        #define DOOM_LOG_ALLOCATOR(x) DOOM_ALLOCATOR(x)
    #else
        #define DOOM_LOG_ALLOCATOR(x) malloc(x)
    #endif /* DOOM_ALLOCATOR */
#endif /* DOOM_LOG_ALLOCATOR */

#ifndef DOOM_LOG_DEALLOCATOR
    #ifdef DOOM_DEALLOCATOR
        #define DOOM_LOG_DEALLOCATOR(x) DOOM_DEALLOCATOR(x)
    #else
        #define DOOM_LOG_DEALLOCATOR(x) free(x)
    #endif /* DOOM_DEALLOCATOR */
#endif /* DOOM_LOG_DEALLOCATOR */


typedef enum doom_log_level_t {
    DOOM_LOG_DEBUG,
    DOOM_LOG_INFO,
    DOOM_LOG_ERROR,
    DOOM_LOG_CRITICAL
} doom_log_level;

typedef enum doom_log_output_t {
    DOOM_LOG_STDOUT,
    // DOOM_LOG_FILE,
    // DOOM_LOG_SOCKET,
} doom_log_output;

typedef enum doom_log_format_t {
    DOOM_LOG_PLAIN,
    DOOM_LOG_JSON,
} doom_log_format;


typedef struct doom_logger_t {
    /* Logger config */
    doom_log_level log_level;
    doom_log_output log_output;
    doom_log_format log_format;
    /* Output config */
} doom_logger;


void doom_log_init(doom_logger * logger);


#ifdef DOOMSDAY_C_ENABLE_DEFAULT_LOGGER
doom_logger default_logger  = {
    DOOM_LOG_INFO, 
    DOOM_LOG_STDOUT,
    DOOM_LOG_PLAIN,
};
#endif /* DOOMSDAY_C_ENABLE_DEFAULT_LOGGER */


/*****************************************************************************/
/* DOOMSDAY C DYNAMIC ARRAY STRIP PREFI                                     */
/*****************************************************************************/
#ifdef DOOMSDAY_C_LOG_STRIP_PREFIX
#ifndef DOOMSDAY_C_LOG_STRIP_BARRIER
#define DOOMSDAY_C_LOG_STRIP_BARRIER
#endif /* DOOMSDAY_C_LOG_STRIP_BARRIER */
#endif /* DOOMSDAY_C_LOG_STRIP_PREFIX */


/*****************************************************************************/
/* DOOMSDAY C DYNAMIC ARRAY IMPLEMENTATION                                   */
/*****************************************************************************/
#ifdef DOOMSDAY_C_LOG_IMPLEMENTATION
#ifndef DOOMSDAY_C_LOG_IMPLEMENTATION_BARRIER
#define DOOMSDAY_C_LOG_IMPLEMENTATION_BARRIER


#endif /* DOOMSDAY_C_LOG_IMPLEMENTATION_BARRIER */
#endif /* DOOMSDAY_C_LOG_IMPLEMENTATION */

#endif /* DOOM_C_LOG_H */

