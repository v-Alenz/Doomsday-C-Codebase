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

#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#define DOOMSDAY_C_STRING_IMPLEMENTATION
#define DOOMSDAY_C_STRING_STRIP_PREFIX
#include "doomsday_c_string.h"


int main( void ) {
    
    /* disabling stdout buffer to not trigger memory allocaion on printf */
    setbuf(stdout, NULL);
    printf("+------------------------------------------------+\n");
    printf("| Doomsday C Testing                             |\n");
    printf("+------------------------------------------------+\n");

    /* TESTS - Doomsday C String */
    doom_string test_string = NULL;
    doom_string test_string_null = NULL;
    doom_string_struct test_string_struct;
    printf("Doomsday C String:\n");

    printf(" - doom_string_get_struct\n");
    assert(doom_string_get_struct(NULL, NULL) == -1);
    assert(doom_string_get_struct(&test_string_struct, test_string_null) == -1);

    printf(" - doom_string_base_pointer\n");
    assert(doom_string_base_pointer(test_string_null) == NULL);

    printf(" - doom_string_init\n"); 
    assert(doom_string_init(&test_string) == 0);
    assert(doom_string_get_struct(&test_string_struct, test_string) == 0);
    assert(*test_string_struct._size == 0);
    assert(test_string_struct._char_array[0] == 0);
    doom_string_deinit(test_string);

    printf(" - doom_string_init_s\n");
    assert(doom_string_init_s(&test_string, 20) == 0);
    assert(doom_string_get_struct(&test_string_struct, test_string) == 0);
    assert(*test_string_struct._size == 20);
    assert(test_string_struct._char_array[0] == 0);
    assert(test_string_struct._char_array[20] == 0);
    doom_string_deinit(test_string);

    printf(" - doom_string_init_c\n");
    assert(doom_string_init_c(&test_string, NULL) == -2);
    assert(doom_string_init_c(&test_string, "Test String Content") == 0);
    assert(doom_string_get_struct(&test_string_struct, test_string) == 0);
    assert(*test_string_struct._size == 19);
    assert(test_string_struct._char_array[0] == 'T');
    assert(strcmp(test_string, "Test String Content") == 0);
    doom_string_deinit(test_string);

    printf(" - doom_string_deinit\n");

    printf(" - doom_string_get\n"); 
    assert((test_string = doom_string_get()) != NULL);
    assert(doom_string_get_struct(&test_string_struct, test_string) == 0);
    assert(*test_string_struct._size == 0);
    assert(test_string_struct._char_array[0] == 0);
    doom_string_deinit(test_string);

    printf(" - doom_string_get_s\n");
    assert((test_string = doom_string_get_s(20)) != NULL);
    assert(doom_string_get_struct(&test_string_struct, test_string) == 0);
    assert(*test_string_struct._size == 20);
    assert(test_string_struct._char_array[0] == 0);
    assert(test_string_struct._char_array[20] == 0);
    doom_string_deinit(test_string);

    printf(" - doom_string_get_c\n");
    assert((test_string = doom_string_get_c(NULL)) == NULL);
    assert((test_string = doom_string_get_c("Test String Content")) != NULL);
    assert(doom_string_get_struct(&test_string_struct, test_string) == 0);
    assert(*test_string_struct._size == 19);
    assert(test_string_struct._char_array[0] == 'T');
    assert(strcmp(test_string, "Test String Content") == 0);
    doom_string_deinit(test_string);

    printf(" - doom_string_resize\n");
    assert((doom_string_resize(NULL, 10)) == -1);
    assert((doom_string_resize(&test_string_null, 10)) == -2);
    assert((test_string = doom_string_get_s(20)) != NULL);
    assert(doom_string_get_struct(&test_string_struct, test_string) == 0);
    assert(*test_string_struct._size == 20);
    assert(doom_string_resize(&test_string, 10) == 0);
    assert(doom_string_get_struct(&test_string_struct, test_string) == 0);
    assert(*test_string_struct._size == 10);
    doom_string_deinit(test_string);

    printf(" - doom_string_safe_resize\n");
    assert((doom_string_safe_resize(NULL, 10)) == -1);
    assert((doom_string_safe_resize(&test_string_null, 10)) == -2);
    assert((test_string = doom_string_get_s(20)) != NULL);
    assert(doom_string_get_struct(&test_string_struct, test_string) == 0);
    assert(*test_string_struct._size == 20);
    assert(doom_string_safe_resize(&test_string, 10) == 0);
    assert(doom_string_get_struct(&test_string_struct, test_string) == 0);
    assert(*test_string_struct._size == 10);
    doom_string_deinit(test_string);
    assert((test_string = doom_string_get_c("Test Resize")) != NULL);
    assert((doom_string_safe_resize(&test_string, 2)) == -3);
    doom_string_deinit(test_string);

    printf(" - doom_string_fit\n");
    assert((doom_string_fit(NULL)) == -1);
    assert((doom_string_fit(&test_string_null)) == -2);
    assert((test_string = doom_string_get_s(20)) != NULL);
    assert(doom_string_get_struct(&test_string_struct, test_string) == 0);
    assert(*test_string_struct._size == 20);
    assert(doom_string_fit(&test_string) == 0);
    assert(doom_string_get_struct(&test_string_struct, test_string) == 0);
    assert(*test_string_struct._size == 0);
    doom_string_deinit(test_string);
    assert((test_string = doom_string_get_s(20)) != NULL);
    assert(doom_string_get_struct(&test_string_struct, test_string) == 0);
    assert(*test_string_struct._size == 20);
    test_string[0] =  'T';
    test_string[1] =  'e';
    test_string[2] =  's';
    test_string[3] =  't';
    assert(doom_string_fit(&test_string) == 0);
    assert(doom_string_get_struct(&test_string_struct, test_string) == 0);
    assert(*test_string_struct._size == 4);
    doom_string_deinit(test_string);

    printf(" - doom_string_get_max_size\n"); 
    assert((test_string = doom_string_get()) != NULL);
    assert((doom_string_get_max_size(test_string)) == 0);
    doom_string_deinit(test_string);
    assert((test_string = doom_string_get_s(20)) != NULL);
    assert((doom_string_get_max_size(test_string)) == 20);
    doom_string_deinit(test_string);
    assert((test_string = doom_string_get_c("Test String Content")) != NULL);
    assert((doom_string_get_max_size(test_string)) == 19);
    doom_string_deinit(test_string);

    printf(" - doom_string_stpcpy\n");
    assert((doom_string_stpcpy( NULL, NULL)) ==  NULL);
    assert((doom_string_stpcpy( NULL, "Test")) ==  NULL);
    assert((doom_string_stpcpy(&test_string_null, "Test")) ==  NULL);
    assert((test_string = doom_string_get()) != NULL);
    assert((doom_string_stpcpy(&test_string, "Test String")) != NULL);
    assert(strcmp(test_string, "Test String") == 0);
    assert(doom_string_get_max_size(test_string) == 11);
    doom_string_deinit(test_string);

    printf(" - doom_string_strcat\n");
    assert((doom_string_stpcpy( NULL, NULL)) ==  NULL);
    assert((doom_string_stpcpy( NULL, "Test")) ==  NULL);
    assert((doom_string_stpcpy(&test_string_null, "Test")) ==  NULL);
    assert((test_string = doom_string_get_c("Test String")) != NULL);
    assert((doom_string_strcat(&test_string, " Concatenation")) != NULL);
    assert(strcmp(test_string, "Test String Concatenation") == 0);
    assert(doom_string_get_max_size(test_string) == 25);
    doom_string_deinit(test_string);

    printf(" - doom_string_strchr\n");

    printf(" - doom_string_strcmp\n");

    printf(" - doom_string_strcoll\n");

    printf(" - doom_string_strcoll_l\n");

    printf(" - doom_string_strcpy\n");

    printf(" - doom_string_strcspn\n");

    printf(" - doom_string_strdup\n");

    printf(" - doom_string_strlen\n");

    printf(" - doom_string_strpbrk\n");

    printf(" - doom_string_strspn\n");

    printf(" - doom_string_strstr\n");

    printf(" - doom_string_strtok\n");

    printf(" - doom_string_strtok_r\n");

    printf("__________________________________________________\n");
    printf("Tests Succeeded!\n");


    exit(EXIT_SUCCESS);
}
