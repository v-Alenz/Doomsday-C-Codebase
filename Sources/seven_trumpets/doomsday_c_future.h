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
/* DOOMSDAY C FUTURE                                                         */
/*****************************************************************************/
#ifndef DOOMSDAY_C_FUTURE_H
#define  DOOMSDAY_C_FUTURE_H


#ifndef DOOM_FUTURE_ALLOCATOR
    #ifdef DOOM_ALLOCATOR
        #define DOOM_FUTURE_ALLOCATOR(x) DOOM_ALLOCATOR(x)
    #else
        #define DOOM_FUTURE_ALLOCATOR(x) malloc(x)
    #endif /* DOOM_ALLOCATOR */
#endif /* DOOM_FUTURE_ALLOCATOR */

#ifndef DOOM_FUTURE_DEALLOCATOR
    #ifdef DOOM_DEALLOCATOR
        #define DOOM_FUTURE_DEALLOCATOR(x) DOOM_DEALLOCATOR(x)
    #else
        #define DOOM_FUTURE_DEALLOCATOR(x) free(x)
    #endif /* DOOM_DEALLOCATOR */
#endif /* DOOM_FUTURE_DEALLOCATOR */


/*****************************************************************************/
/* DOOMSDAY C FUTURE STRIP PREFIX                                            */
/*****************************************************************************/
#ifdef DOOMSDAY_C_FUTURE_STRIP_PREFIX
#ifndef DOOMSDAY_C_FUTURE_STRIP_BARRIER
#define DOOMSDAY_C_FUTURE_STRIP_BARRIER


#endif /* DOOMSDAY_C_FUTURE_STRIP_BARRIER */
#endif /* DOOMSDAY_C_FUTURE_STRIP_PREFIX */


/*****************************************************************************/
/* DOOMSDAY C FUTURE IMPLEMENTATIN                                           */
/*****************************************************************************/
#ifdef DOOMSDAY_C_FUTURE_IMPLEMENTATION
#ifndef DOOMSDAY_C_FUTURE_IMPLEMENTATION_BARRIER
#define DOOMSDAY_C_FUTURE_IMPLEMENTATION_BARRIER


#endif /* DOOMSDAY_C_FUTURE_IMPLEMENTATION_BARRIER */
#endif /* DOOMSDAY_C_FUTURE_IMPLEMENTATION */

#endif /* DOOMSDAY_C_FUTURE_H */
