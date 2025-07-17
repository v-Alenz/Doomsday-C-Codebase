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
#include <stdlib.h>
#include <string.h>
/* Doomsday C String */
#define DOOMSDAY_C_STRING_IMPLEMENTATION
#define DOOMSDAY_C_STRING_STRIP_PREFIX
#include "doomsday_c_string.h"
/* Doomsday C Dynamic Array */
#define DOOMSDAY_C_DYNAMIC_ARRAY_STRIP_PREFIX
#define DOOMSDAY_C_DYNAMIC_ARRAY_IMPLEMENTATION
#include "doomsday_c_dynamic_array.h"


#define DOOM_ASSERT(prop) do{assert(prop); asserts_count++;}while(0)
#define DOOM_TEST_CASE(name) do{printf(name); test_cases_count++;}while(0)


int main( void ) {

    uint64_t test_cases_count = 0;
    uint64_t asserts_count = 0;

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

    DOOM_TEST_CASE(" - doom_string_get_struct\n");
    DOOM_ASSERT(doom_string_get_struct(NULL, NULL) == -1);
    DOOM_ASSERT(doom_string_get_struct(&test_string_struct, test_string_null) == -1);

    DOOM_TEST_CASE(" - doom_string_base_pointer\n");
    DOOM_ASSERT(doom_string_base_pointer(test_string_null) == NULL);

    DOOM_TEST_CASE(" - doom_string_init\n");
    DOOM_ASSERT(doom_string_init(&test_string) == 0);
    DOOM_ASSERT(doom_string_get_struct(&test_string_struct, test_string) == 0);
    DOOM_ASSERT(*test_string_struct._size == 0);
    DOOM_ASSERT(test_string_struct._char_array[0] == 0);
    doom_string_deinit(test_string);

    DOOM_TEST_CASE(" - doom_string_init_size\n");
    DOOM_ASSERT(doom_string_init_size(&test_string, 20) == 0);
    DOOM_ASSERT(doom_string_get_struct(&test_string_struct, test_string) == 0);
    DOOM_ASSERT(*test_string_struct._size == 20);
    DOOM_ASSERT(test_string_struct._char_array[0] == 0);
    DOOM_ASSERT(test_string_struct._char_array[20] == 0);
    doom_string_deinit(test_string);

    DOOM_TEST_CASE(" - doom_string_init_copy\n");
    DOOM_ASSERT(doom_string_init_copy(&test_string, NULL) == -2);
    DOOM_ASSERT(doom_string_init_copy(&test_string, "Test String Content") == 0);
    DOOM_ASSERT(doom_string_get_struct(&test_string_struct, test_string) == 0);
    DOOM_ASSERT(*test_string_struct._size == 19);
    DOOM_ASSERT(test_string_struct._char_array[0] == 'T');
    DOOM_ASSERT(strcmp(test_string, "Test String Content") == 0);
    doom_string_deinit(test_string);

    DOOM_TEST_CASE(" - doom_string_deinit\n");

    DOOM_TEST_CASE(" - doom_string_factory\n");
    DOOM_ASSERT((test_string = doom_string_factory()) != NULL);
    DOOM_ASSERT(doom_string_get_struct(&test_string_struct, test_string) == 0);
    DOOM_ASSERT(*test_string_struct._size == 0);
    DOOM_ASSERT(test_string_struct._char_array[0] == 0);
    doom_string_deinit(test_string);

    DOOM_TEST_CASE(" - doom_string_factory_size\n");
    DOOM_ASSERT((test_string = doom_string_factory_size(20)) != NULL);
    DOOM_ASSERT(doom_string_get_struct(&test_string_struct, test_string) == 0);
    DOOM_ASSERT(*test_string_struct._size == 20);
    DOOM_ASSERT(test_string_struct._char_array[0] == 0);
    DOOM_ASSERT(test_string_struct._char_array[20] == 0);
    doom_string_deinit(test_string);

    DOOM_TEST_CASE(" - doom_string_factory_copy\n");
    DOOM_ASSERT((test_string = doom_string_factory_copy(NULL)) == NULL);
    DOOM_ASSERT((test_string = doom_string_factory_copy("Test String Content")) != NULL);
    DOOM_ASSERT(doom_string_get_struct(&test_string_struct, test_string) == 0);
    DOOM_ASSERT(*test_string_struct._size == 19);
    DOOM_ASSERT(test_string_struct._char_array[0] == 'T');
    DOOM_ASSERT(strcmp(test_string, "Test String Content") == 0);
    doom_string_deinit(test_string);

    DOOM_TEST_CASE(" - doom_string_resize\n");
    DOOM_ASSERT((doom_string_resize(NULL, 10)) == -1);
    DOOM_ASSERT((doom_string_resize(&test_string_null, 10)) == -2);
    DOOM_ASSERT((test_string = doom_string_factory_size(20)) != NULL);
    DOOM_ASSERT(doom_string_get_struct(&test_string_struct, test_string) == 0);
    DOOM_ASSERT(*test_string_struct._size == 20);
    DOOM_ASSERT(doom_string_resize(&test_string, 10) == 0);
    DOOM_ASSERT(doom_string_get_struct(&test_string_struct, test_string) == 0);
    DOOM_ASSERT(*test_string_struct._size == 10);
    doom_string_deinit(test_string);

    DOOM_TEST_CASE(" - doom_string_safe_resize\n");
    DOOM_ASSERT((doom_string_safe_resize(NULL, 10)) == -1);
    DOOM_ASSERT((doom_string_safe_resize(&test_string_null, 10)) == -2);
    DOOM_ASSERT((test_string = doom_string_factory_size(20)) != NULL);
    DOOM_ASSERT(doom_string_get_struct(&test_string_struct, test_string) == 0);
    DOOM_ASSERT(*test_string_struct._size == 20);
    DOOM_ASSERT(doom_string_safe_resize(&test_string, 10) == 0);
    DOOM_ASSERT(doom_string_get_struct(&test_string_struct, test_string) == 0);
    DOOM_ASSERT(*test_string_struct._size == 10);
    doom_string_deinit(test_string);
    DOOM_ASSERT((test_string = doom_string_factory_copy("Test Resize")) != NULL);
    DOOM_ASSERT((doom_string_safe_resize(&test_string, 2)) == -3);
    doom_string_deinit(test_string);

    DOOM_TEST_CASE(" - doom_string_fit\n");
    DOOM_ASSERT((doom_string_fit(NULL)) == -1);
    DOOM_ASSERT((doom_string_fit(&test_string_null)) == -2);
    DOOM_ASSERT((test_string = doom_string_factory_size(20)) != NULL);
    DOOM_ASSERT(doom_string_get_struct(&test_string_struct, test_string) == 0);
    DOOM_ASSERT(*test_string_struct._size == 20);
    DOOM_ASSERT(doom_string_fit(&test_string) == 0);
    DOOM_ASSERT(doom_string_get_struct(&test_string_struct, test_string) == 0);
    DOOM_ASSERT(*test_string_struct._size == 0);
    doom_string_deinit(test_string);
    DOOM_ASSERT((test_string = doom_string_factory_size(20)) != NULL);
    DOOM_ASSERT(doom_string_get_struct(&test_string_struct, test_string) == 0);
    DOOM_ASSERT(*test_string_struct._size == 20);
    test_string[0] =  'T';
    test_string[1] =  'e';
    test_string[2] =  's';
    test_string[3] =  't';
    DOOM_ASSERT(doom_string_fit(&test_string) == 0);
    DOOM_ASSERT(doom_string_get_struct(&test_string_struct, test_string) == 0);
    DOOM_ASSERT(*test_string_struct._size == 4);
    doom_string_deinit(test_string);

    DOOM_TEST_CASE(" - doom_string_get_max_size\n");
    DOOM_ASSERT((test_string = doom_string_factory()) != NULL);
    DOOM_ASSERT((doom_string_get_max_size(test_string)) == 0);
    doom_string_deinit(test_string);
    DOOM_ASSERT((test_string = doom_string_factory_size(20)) != NULL);
    DOOM_ASSERT((doom_string_get_max_size(test_string)) == 20);
    doom_string_deinit(test_string);
    DOOM_ASSERT((test_string = doom_string_factory_copy("Test String Content")) != NULL);
    DOOM_ASSERT((doom_string_get_max_size(test_string)) == 19);
    doom_string_deinit(test_string);

    DOOM_TEST_CASE(" - doom_string_stpcpy\n");
    DOOM_ASSERT((doom_string_stpcpy( NULL, NULL)) ==  NULL);
    DOOM_ASSERT((doom_string_stpcpy( NULL, "Test")) ==  NULL);
    DOOM_ASSERT((doom_string_stpcpy(&test_string_null, "Test")) ==  NULL);
    DOOM_ASSERT((test_string = doom_string_factory()) != NULL);
    DOOM_ASSERT((doom_string_stpcpy(&test_string, "Test String")) != NULL);
    DOOM_ASSERT(strcmp(test_string, "Test String") == 0);
    DOOM_ASSERT(doom_string_get_max_size(test_string) == 11);
    doom_string_deinit(test_string);

    DOOM_TEST_CASE(" - doom_string_strcat\n");
    DOOM_ASSERT((doom_string_stpcpy( NULL, NULL)) ==  NULL);
    DOOM_ASSERT((doom_string_stpcpy( NULL, "Test")) ==  NULL);
    DOOM_ASSERT((doom_string_stpcpy(&test_string_null, "Test")) ==  NULL);
    DOOM_ASSERT((test_string = doom_string_factory_copy("Test String")) != NULL);
    DOOM_ASSERT((doom_string_strcat(&test_string, " Concatenation")) != NULL);
    DOOM_ASSERT(strcmp(test_string, "Test String Concatenation") == 0);
    DOOM_ASSERT(doom_string_get_max_size(test_string) == 25);
    doom_string_deinit(test_string);

    DOOM_TEST_CASE(" - doom_string_strchr\n");

    DOOM_TEST_CASE(" - doom_string_strcmp\n");

    DOOM_TEST_CASE(" - doom_string_strcoll\n");

    DOOM_TEST_CASE(" - doom_string_strcoll_l\n");

    DOOM_TEST_CASE(" - doom_string_strcpy\n");

    DOOM_TEST_CASE(" - doom_string_strcspn\n");

    DOOM_TEST_CASE(" - doom_string_strdup\n");

    DOOM_TEST_CASE(" - doom_string_strlen\n");

    DOOM_TEST_CASE(" - doom_string_strpbrk\n");

    DOOM_TEST_CASE(" - doom_string_strspn\n");

    DOOM_TEST_CASE(" - doom_string_strstr\n");

    DOOM_TEST_CASE(" - doom_string_strtok\n");

    DOOM_TEST_CASE(" - doom_string_strtok_r\n");


    /* TESTS - Doomsday C Dynamic Array */
    int32_t * int_dynamic_array;
    void ** int_dynamic_array_ptr = (void **)(&int_dynamic_array);
    doom_string * doom_string_dynamic_array;
    void ** doom_string_dynamic_array_ptr = (void **)(&doom_string_dynamic_array);
    doom_dynamic_array_struct dynamic_array_struct;
    printf("\nDoomsday C Dynamic Array:\n");

    DOOM_TEST_CASE(" - doom_dynamic_array_get_struct\n");
    DOOM_ASSERT(doom_dynamic_array_get_struct(NULL, NULL) == -1);

    DOOM_TEST_CASE(" - doom_dynamic_array_get_base_ptr\n");
    DOOM_ASSERT(doom_dynamic_array_get_base_ptr(NULL) == NULL);

    DOOM_TEST_CASE(" - doom_dynamic_array_init\n");
    DOOM_ASSERT(doom_dynamic_array_init(int_dynamic_array_ptr, sizeof(int32_t)) == 0);
    DOOM_ASSERT(doom_dynamic_array_init(doom_string_dynamic_array_ptr, sizeof(doom_string)) == 0);
    DOOM_ASSERT(doom_dynamic_array_get_struct(&dynamic_array_struct, *int_dynamic_array_ptr) == 0);
    DOOM_ASSERT(*dynamic_array_struct._array_size == 0);
    DOOM_ASSERT(*dynamic_array_struct._array_max_size == ARRAY_DEFAULT_SIZE);
    DOOM_ASSERT(*dynamic_array_struct._sizeof_elem == sizeof(int32_t));
    DOOM_ASSERT(doom_dynamic_array_get_struct(&dynamic_array_struct, *doom_string_dynamic_array_ptr) == 0);
    DOOM_ASSERT(*dynamic_array_struct._array_size == 0);
    DOOM_ASSERT(*dynamic_array_struct._array_max_size == ARRAY_DEFAULT_SIZE);
    DOOM_ASSERT(*dynamic_array_struct._sizeof_elem == sizeof(doom_string));
    doom_dynamic_array_deinit(*int_dynamic_array_ptr);
    doom_dynamic_array_deinit(*doom_string_dynamic_array_ptr);

    DOOM_TEST_CASE(" - doom_dynamic_array_init_size\n");
    DOOM_ASSERT(doom_dynamic_array_init_size(int_dynamic_array_ptr, sizeof(int32_t), 10) == 0);
    DOOM_ASSERT(doom_dynamic_array_init_size(doom_string_dynamic_array_ptr, sizeof(doom_string), 20) == 0);
    DOOM_ASSERT(doom_dynamic_array_get_struct(&dynamic_array_struct, *int_dynamic_array_ptr) == 0);
    DOOM_ASSERT(*dynamic_array_struct._array_size == 10);
    DOOM_ASSERT(*dynamic_array_struct._array_max_size == 10);
    DOOM_ASSERT(*dynamic_array_struct._sizeof_elem == sizeof(int32_t));
    DOOM_ASSERT(doom_dynamic_array_get_struct(&dynamic_array_struct, *doom_string_dynamic_array_ptr) == 0);
    DOOM_ASSERT(*dynamic_array_struct._array_size == 20);
    DOOM_ASSERT(*dynamic_array_struct._array_max_size == 20);
    DOOM_ASSERT(*dynamic_array_struct._sizeof_elem == sizeof(doom_string));
    doom_dynamic_array_deinit(*int_dynamic_array_ptr);
    doom_dynamic_array_deinit(*doom_string_dynamic_array_ptr);

    DOOM_TEST_CASE(" - doom_dynamic_array_init_copy\n");
    int32_t from[5] = {1,2,3,4,5};
    DOOM_ASSERT(doom_dynamic_array_init_copy(int_dynamic_array_ptr, sizeof(int32_t), from, 5) == 0);
    DOOM_ASSERT(doom_dynamic_array_get_struct(&dynamic_array_struct, *int_dynamic_array_ptr) == 0);
    DOOM_ASSERT(*dynamic_array_struct._array_size == 5);
    DOOM_ASSERT(*dynamic_array_struct._array_max_size == 10);
    DOOM_ASSERT(*dynamic_array_struct._sizeof_elem == sizeof(int32_t));
    for (int i=0; i<5; i++) {
        DOOM_ASSERT(int_dynamic_array[i] == i+1);
    }
    doom_dynamic_array_deinit(*int_dynamic_array_ptr);

    DOOM_TEST_CASE(" - doom_dynamic_array_factory\n");
    DOOM_ASSERT((int_dynamic_array = (int32_t *) doom_dynamic_array_factory(sizeof(int32_t))) != NULL);
    DOOM_ASSERT((doom_string_dynamic_array = (doom_string *) doom_dynamic_array_factory(sizeof(doom_string))) != NULL);
    DOOM_ASSERT(doom_dynamic_array_get_struct(&dynamic_array_struct, *int_dynamic_array_ptr) == 0);
    DOOM_ASSERT(*dynamic_array_struct._array_size == 0);
    DOOM_ASSERT(*dynamic_array_struct._array_max_size == ARRAY_DEFAULT_SIZE);
    DOOM_ASSERT(*dynamic_array_struct._sizeof_elem == sizeof(int32_t));
    DOOM_ASSERT(doom_dynamic_array_get_struct(&dynamic_array_struct, *doom_string_dynamic_array_ptr) == 0);
    DOOM_ASSERT(*dynamic_array_struct._array_size == 0);
    DOOM_ASSERT(*dynamic_array_struct._array_max_size == ARRAY_DEFAULT_SIZE);
    DOOM_ASSERT(*dynamic_array_struct._sizeof_elem == sizeof(doom_string));
    doom_dynamic_array_deinit(*int_dynamic_array_ptr);
    doom_dynamic_array_deinit(*doom_string_dynamic_array_ptr);

    DOOM_TEST_CASE(" - doom_dynamic_array_factory_size\n");
    DOOM_ASSERT((int_dynamic_array = (int32_t *) doom_dynamic_array_factory_size(sizeof(int32_t), 10)) != NULL);
    DOOM_ASSERT((doom_string_dynamic_array = (doom_string *) doom_dynamic_array_factory_size(sizeof(doom_string), 20)) != NULL);
    DOOM_ASSERT(doom_dynamic_array_get_struct(&dynamic_array_struct, *int_dynamic_array_ptr) == 0);
    DOOM_ASSERT(*dynamic_array_struct._array_size == 10);
    DOOM_ASSERT(*dynamic_array_struct._array_max_size == 10);
    DOOM_ASSERT(*dynamic_array_struct._sizeof_elem == sizeof(int32_t));
    DOOM_ASSERT(doom_dynamic_array_get_struct(&dynamic_array_struct, *doom_string_dynamic_array_ptr) == 0);
    DOOM_ASSERT(*dynamic_array_struct._array_size == 20);
    DOOM_ASSERT(*dynamic_array_struct._array_max_size == 20);
    DOOM_ASSERT(*dynamic_array_struct._sizeof_elem == sizeof(doom_string));
    doom_dynamic_array_deinit(*int_dynamic_array_ptr);
    doom_dynamic_array_deinit(*doom_string_dynamic_array_ptr);

    DOOM_TEST_CASE(" - doom_dynamic_array_factory_copy\n");
    DOOM_ASSERT((int_dynamic_array = (int32_t *) doom_dynamic_array_factory_copy(sizeof(int32_t), from, 5)) != NULL);
    DOOM_ASSERT(doom_dynamic_array_get_struct(&dynamic_array_struct, *int_dynamic_array_ptr) == 0);
    DOOM_ASSERT(*dynamic_array_struct._array_size == 5);
    DOOM_ASSERT(*dynamic_array_struct._array_max_size == 10);
    DOOM_ASSERT(*dynamic_array_struct._sizeof_elem == sizeof(int32_t));
    for (int i=0; i<5; i++) {
        DOOM_ASSERT(int_dynamic_array[i] == i+1);
    }
    doom_dynamic_array_deinit(*int_dynamic_array_ptr);

    DOOM_TEST_CASE(" - doom_dynamic_array_deinit\n");

    DOOM_TEST_CASE(" - doom_dynamic_array_get_elem_size\n");
    DOOM_ASSERT((int_dynamic_array = (int32_t *) doom_dynamic_array_factory_copy(sizeof(int32_t), from, 5)) != NULL);
    DOOM_ASSERT(doom_dynamic_array_get_elem_size(*int_dynamic_array_ptr) == sizeof(int32_t));

    DOOM_TEST_CASE(" - doom_dynamic_array_get_size\n");
    DOOM_ASSERT(doom_dynamic_array_get_size(*int_dynamic_array_ptr) == 5);

    DOOM_TEST_CASE(" - doom_dynamic_array_get_max_size\n");
    DOOM_ASSERT(doom_dynamic_array_get_max_size(*int_dynamic_array_ptr) == 10);

    DOOM_TEST_CASE(" - doom_dynamic_array_expand\n");
    DOOM_ASSERT(doom_dynamic_array_expand(int_dynamic_array_ptr) == 0);
    DOOM_ASSERT(doom_dynamic_array_get_struct(&dynamic_array_struct, *int_dynamic_array_ptr) == 0);
    DOOM_ASSERT(*dynamic_array_struct._array_size == 5);
    DOOM_ASSERT(*dynamic_array_struct._array_max_size == 20);
    DOOM_ASSERT(*dynamic_array_struct._sizeof_elem == sizeof(int32_t));
    for (int i=0; i<5; i++) {
        DOOM_ASSERT(int_dynamic_array[i] == i+1);
    }

    DOOM_TEST_CASE(" - doom_dynamic_array_resize\n");
    DOOM_ASSERT(doom_dynamic_array_resize(int_dynamic_array_ptr, 40) == 0);
    DOOM_ASSERT(doom_dynamic_array_get_struct(&dynamic_array_struct, *int_dynamic_array_ptr) == 0);
    DOOM_ASSERT(*dynamic_array_struct._array_size == 40);
    DOOM_ASSERT(*dynamic_array_struct._array_max_size == 40);
    DOOM_ASSERT(*dynamic_array_struct._sizeof_elem == sizeof(int32_t));
    for (int i=0; i<5; i++) {
        DOOM_ASSERT(int_dynamic_array[i] == i+1);
    }

    DOOM_TEST_CASE(" - doom_dynamic_array_fit\n");

    DOOM_TEST_CASE(" - doom_dynamic_array_reserve\n");
    DOOM_ASSERT(doom_dynamic_array_reserve(int_dynamic_array_ptr, 120) == 0);
    DOOM_ASSERT(doom_dynamic_array_get_struct(&dynamic_array_struct, *int_dynamic_array_ptr) == 0);
    DOOM_ASSERT(*dynamic_array_struct._array_size == 40);
    DOOM_ASSERT(*dynamic_array_struct._array_max_size == 120);
    DOOM_ASSERT(*dynamic_array_struct._sizeof_elem == sizeof(int32_t));
    for (int i=0; i<5; i++) {
        DOOM_ASSERT(int_dynamic_array[i] == i+1);
    }
    doom_dynamic_array_deinit(*int_dynamic_array_ptr);

    /* End of Testing */
    printf("__________________________________________________\n");  
    printf("Tests Succeeded!\n");
    printf("Tested %lu assertions in %lu test cases!\n", asserts_count, test_cases_count);
    fflush(stdout);

    exit(EXIT_SUCCESS);
}
