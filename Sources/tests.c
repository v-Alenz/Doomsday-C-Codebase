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
#define DOOMSDAY_C_DYNAMIC_ARRAY_IMPLEMENTATION
#define DOOMSDAY_C_DYNAMIC_ARRAY_STRIP_PREFIX
#include "doomsday_c_dynamic_array.h"
/* Doomsday C List */
#define DOOMSDAY_C_LIST_IMPLEMENTATION
#define DOOMSDAY_C_LIST_STRIP_PREFIX
#include "doomsday_c_list.h"


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

    DOOM_TEST_CASE(" - doom_string_new\n");
    DOOM_ASSERT((test_string = doom_string_new()) != NULL);
    DOOM_ASSERT(doom_string_get_struct(&test_string_struct, test_string) == 0);
    DOOM_ASSERT(*test_string_struct._size == 0);
    DOOM_ASSERT(test_string_struct._char_array[0] == 0);
    doom_string_deinit(test_string);

    DOOM_TEST_CASE(" - doom_string_new_size\n");
    DOOM_ASSERT((test_string = doom_string_new_size(20)) != NULL);
    DOOM_ASSERT(doom_string_get_struct(&test_string_struct, test_string) == 0);
    DOOM_ASSERT(*test_string_struct._size == 20);
    DOOM_ASSERT(test_string_struct._char_array[0] == 0);
    DOOM_ASSERT(test_string_struct._char_array[20] == 0);
    doom_string_deinit(test_string);

    DOOM_TEST_CASE(" - doom_string_new_copy\n");
    DOOM_ASSERT((test_string = doom_string_new_copy(NULL)) == NULL);
    DOOM_ASSERT((test_string = doom_string_new_copy("Test String Content")) != NULL);
    DOOM_ASSERT(doom_string_get_struct(&test_string_struct, test_string) == 0);
    DOOM_ASSERT(*test_string_struct._size == 19);
    DOOM_ASSERT(test_string_struct._char_array[0] == 'T');
    DOOM_ASSERT(strcmp(test_string, "Test String Content") == 0);
    doom_string_deinit(test_string);

    DOOM_TEST_CASE(" - doom_string_resize\n");
    DOOM_ASSERT((doom_string_resize(NULL, 10)) == -1);
    DOOM_ASSERT((doom_string_resize(&test_string_null, 10)) == -2);
    DOOM_ASSERT((test_string = doom_string_new_size(20)) != NULL);
    DOOM_ASSERT(doom_string_get_struct(&test_string_struct, test_string) == 0);
    DOOM_ASSERT(*test_string_struct._size == 20);
    DOOM_ASSERT(doom_string_resize(&test_string, 10) == 0);
    DOOM_ASSERT(doom_string_get_struct(&test_string_struct, test_string) == 0);
    DOOM_ASSERT(*test_string_struct._size == 10);
    doom_string_deinit(test_string);

    DOOM_TEST_CASE(" - doom_string_safe_resize\n");
    DOOM_ASSERT((doom_string_safe_resize(NULL, 10)) == -1);
    DOOM_ASSERT((doom_string_safe_resize(&test_string_null, 10)) == -2);
    DOOM_ASSERT((test_string = doom_string_new_size(20)) != NULL);
    DOOM_ASSERT(doom_string_get_struct(&test_string_struct, test_string) == 0);
    DOOM_ASSERT(*test_string_struct._size == 20);
    DOOM_ASSERT(doom_string_safe_resize(&test_string, 10) == 0);
    DOOM_ASSERT(doom_string_get_struct(&test_string_struct, test_string) == 0);
    DOOM_ASSERT(*test_string_struct._size == 10);
    doom_string_deinit(test_string);
    DOOM_ASSERT((test_string = doom_string_new_copy("Test Resize")) != NULL);
    DOOM_ASSERT((doom_string_safe_resize(&test_string, 2)) == -3);
    doom_string_deinit(test_string);

    DOOM_TEST_CASE(" - doom_string_fit\n");
    DOOM_ASSERT((doom_string_fit(NULL)) == -1);
    DOOM_ASSERT((doom_string_fit(&test_string_null)) == -2);
    DOOM_ASSERT((test_string = doom_string_new_size(20)) != NULL);
    DOOM_ASSERT(doom_string_get_struct(&test_string_struct, test_string) == 0);
    DOOM_ASSERT(*test_string_struct._size == 20);
    DOOM_ASSERT(doom_string_fit(&test_string) == 0);
    DOOM_ASSERT(doom_string_get_struct(&test_string_struct, test_string) == 0);
    DOOM_ASSERT(*test_string_struct._size == 0);
    doom_string_deinit(test_string);
    DOOM_ASSERT((test_string = doom_string_new_size(20)) != NULL);
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
    DOOM_ASSERT((test_string = doom_string_new()) != NULL);
    DOOM_ASSERT((doom_string_get_max_size(test_string)) == 0);
    doom_string_deinit(test_string);
    DOOM_ASSERT((test_string = doom_string_new_size(20)) != NULL);
    DOOM_ASSERT((doom_string_get_max_size(test_string)) == 20);
    doom_string_deinit(test_string);
    DOOM_ASSERT((test_string = doom_string_new_copy("Test String Content")) != NULL);
    DOOM_ASSERT((doom_string_get_max_size(test_string)) == 19);
    doom_string_deinit(test_string);

    DOOM_TEST_CASE(" - doom_string_stpcpy\n");
    DOOM_ASSERT((doom_string_stpcpy( NULL, NULL)) ==  NULL);
    DOOM_ASSERT((doom_string_stpcpy( NULL, "Test")) ==  NULL);
    DOOM_ASSERT((doom_string_stpcpy(&test_string_null, "Test")) ==  NULL);
    DOOM_ASSERT((test_string = doom_string_new()) != NULL);
    DOOM_ASSERT((doom_string_stpcpy(&test_string, "Test String")) != NULL);
    DOOM_ASSERT(strcmp(test_string, "Test String") == 0);
    DOOM_ASSERT(doom_string_get_max_size(test_string) == 11);
    doom_string_deinit(test_string);

    DOOM_TEST_CASE(" - doom_string_strcat\n");
    DOOM_ASSERT((doom_string_stpcpy( NULL, NULL)) ==  NULL);
    DOOM_ASSERT((doom_string_stpcpy( NULL, "Test")) ==  NULL);
    DOOM_ASSERT((doom_string_stpcpy(&test_string_null, "Test")) ==  NULL);
    DOOM_ASSERT((test_string = doom_string_new_copy("Test String")) != NULL);
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
    int32_t int_aux;
    int32_t * int_ptr;
    int32_t * int_dynamic_array;
    void ** int_dynamic_array_ptr = (void **)(&int_dynamic_array);
    doom_string * doom_string_dynamic_array;
    void ** doom_string_dynamic_array_ptr = (void **)(&doom_string_dynamic_array);
    doom_dynamic_array_struct dynamic_array_struct;
    printf("Doomsday C Dynamic Array:\n");

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

    DOOM_TEST_CASE(" - doom_dynamic_array_new\n");
    DOOM_ASSERT((int_dynamic_array = (int32_t *) doom_dynamic_array_new(sizeof(int32_t))) != NULL);
    DOOM_ASSERT((doom_string_dynamic_array = (doom_string *) doom_dynamic_array_new(sizeof(doom_string))) != NULL);
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

    DOOM_TEST_CASE(" - doom_dynamic_array_new_size\n");
    DOOM_ASSERT((int_dynamic_array = (int32_t *) doom_dynamic_array_new_size(sizeof(int32_t), 10)) != NULL);
    DOOM_ASSERT((doom_string_dynamic_array = (doom_string *) doom_dynamic_array_new_size(sizeof(doom_string), 20)) != NULL);
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

    DOOM_TEST_CASE(" - doom_dynamic_array_new_copy\n");
    DOOM_ASSERT((int_dynamic_array = (int32_t *) doom_dynamic_array_new_copy(sizeof(int32_t), from, 5)) != NULL);
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
    DOOM_ASSERT((int_dynamic_array = (int32_t *) doom_dynamic_array_new_copy(sizeof(int32_t), from, 5)) != NULL);
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
    DOOM_ASSERT(doom_dynamic_array_resize(int_dynamic_array_ptr, 10) == 0);
    DOOM_ASSERT(doom_dynamic_array_get_struct(&dynamic_array_struct, *int_dynamic_array_ptr) == 0);
    DOOM_ASSERT(*dynamic_array_struct._array_size == 10);
    DOOM_ASSERT(*dynamic_array_struct._array_max_size == 40);
    DOOM_ASSERT(*dynamic_array_struct._sizeof_elem == sizeof(int32_t));
    for (int i=0; i<5; i++) {
        DOOM_ASSERT(int_dynamic_array[i] == i+1);
    }
    DOOM_ASSERT(doom_dynamic_array_fit(int_dynamic_array_ptr) == 0);
    DOOM_ASSERT(doom_dynamic_array_get_struct(&dynamic_array_struct, *int_dynamic_array_ptr) == 0);
    DOOM_ASSERT(*dynamic_array_struct._array_size == 10);
    DOOM_ASSERT(*dynamic_array_struct._array_max_size == 10);
    DOOM_ASSERT(*dynamic_array_struct._sizeof_elem == sizeof(int32_t));
    for (int i=0; i<5; i++) {
        DOOM_ASSERT(int_dynamic_array[i] == i+1);
    }

    DOOM_TEST_CASE(" - doom_dynamic_array_reserve\n");
    DOOM_ASSERT(doom_dynamic_array_reserve(int_dynamic_array_ptr, 120) == 0);
    DOOM_ASSERT(doom_dynamic_array_get_struct(&dynamic_array_struct, *int_dynamic_array_ptr) == 0);
    DOOM_ASSERT(*dynamic_array_struct._array_size == 10);
    DOOM_ASSERT(*dynamic_array_struct._array_max_size == 120);
    DOOM_ASSERT(*dynamic_array_struct._sizeof_elem == sizeof(int32_t));
    for (int i=0; i<5; i++) {
        DOOM_ASSERT(int_dynamic_array[i] == i+1);
    }
    doom_dynamic_array_deinit(*int_dynamic_array_ptr);

    DOOM_TEST_CASE(" - doom_dynamic_array_back\n");
    *int_dynamic_array_ptr = NULL;
    DOOM_ASSERT(doom_dynamic_array_back(*int_dynamic_array_ptr) == NULL);
    DOOM_ASSERT((int_dynamic_array = (int32_t *) doom_dynamic_array_new(sizeof(int32_t))) != NULL);
    DOOM_ASSERT(doom_dynamic_array_back(*int_dynamic_array_ptr) == NULL);
    doom_dynamic_array_deinit(*int_dynamic_array_ptr);
    DOOM_ASSERT((int_dynamic_array = (int32_t *) doom_dynamic_array_new_copy(sizeof(int32_t), from, 5)) != NULL);
    DOOM_ASSERT((int_ptr = (int32_t *) doom_dynamic_array_back((void *) int_dynamic_array)) != NULL);
    DOOM_ASSERT(*int_ptr == 5);
    doom_dynamic_array_deinit(*int_dynamic_array_ptr);

    DOOM_TEST_CASE(" - doom_dynamic_array_push_back\n");
    DOOM_ASSERT((int_dynamic_array = (int32_t *) doom_dynamic_array_new_copy(sizeof(int32_t), from, 5)) != NULL);
    DOOM_ASSERT(doom_dynamic_array_get_struct(&dynamic_array_struct, *int_dynamic_array_ptr) == 0);
    DOOM_ASSERT(*dynamic_array_struct._array_size == 5);
    DOOM_ASSERT(*dynamic_array_struct._array_max_size == 10);
    DOOM_ASSERT(*dynamic_array_struct._sizeof_elem == sizeof(int32_t));
    for (int i=0; i<5; i++) {
        DOOM_ASSERT(int_dynamic_array[i] == i+1);
    }
    int_aux = 6;
    DOOM_ASSERT(doom_dynamic_array_push_back(int_dynamic_array_ptr, &int_aux) == 0);
    DOOM_ASSERT(doom_dynamic_array_get_struct(&dynamic_array_struct, *int_dynamic_array_ptr) == 0);
    DOOM_ASSERT(*dynamic_array_struct._array_size == 6);
    DOOM_ASSERT(*dynamic_array_struct._array_max_size == 10);
    DOOM_ASSERT(*dynamic_array_struct._sizeof_elem == sizeof(int32_t));
    for (int i=0; i<6; i++) {
        DOOM_ASSERT(int_dynamic_array[i] == i+1);
    }
    DOOM_ASSERT(doom_dynamic_array_fit(int_dynamic_array_ptr) == 0);
    int_aux = 7;
    DOOM_ASSERT(doom_dynamic_array_push_back(int_dynamic_array_ptr, &int_aux) == 0);
    DOOM_ASSERT(doom_dynamic_array_get_struct(&dynamic_array_struct, *int_dynamic_array_ptr) == 0);
    DOOM_ASSERT(*dynamic_array_struct._array_size == 7);
    DOOM_ASSERT(*dynamic_array_struct._array_max_size == 12);
    DOOM_ASSERT(*dynamic_array_struct._sizeof_elem == sizeof(int32_t));
    for (int i=0; i<7; i++) {
        DOOM_ASSERT(int_dynamic_array[i] == i+1);
    }

    DOOM_TEST_CASE(" - doom_dynamic_array_pop_back\n");
    DOOM_ASSERT(doom_dynamic_array_pop_back(int_dynamic_array_ptr) == 0);
    DOOM_ASSERT(doom_dynamic_array_get_struct(&dynamic_array_struct, *int_dynamic_array_ptr) == 0);
    DOOM_ASSERT(*dynamic_array_struct._array_size == 6);
    DOOM_ASSERT(*dynamic_array_struct._array_max_size == 12);
    DOOM_ASSERT(*dynamic_array_struct._sizeof_elem == sizeof(int32_t));
    for (int i=0; i<6; i++) {
        DOOM_ASSERT(int_dynamic_array[i] == i+1);
    }
    DOOM_ASSERT(doom_dynamic_array_pop_back(int_dynamic_array_ptr) == 0);
    DOOM_ASSERT(doom_dynamic_array_fit(int_dynamic_array_ptr) == 0);
    DOOM_ASSERT(doom_dynamic_array_get_struct(&dynamic_array_struct, *int_dynamic_array_ptr) == 0);
    DOOM_ASSERT(*dynamic_array_struct._array_size == 5);
    DOOM_ASSERT(*dynamic_array_struct._array_max_size == 5);
    DOOM_ASSERT(*dynamic_array_struct._sizeof_elem == sizeof(int32_t));
    for (int i=0; i<5; i++) {
        DOOM_ASSERT(int_dynamic_array[i] == i+1);
    }
    doom_dynamic_array_deinit(*int_dynamic_array_ptr);

    DOOM_TEST_CASE(" - doom_dynamic_array_front\n");
    *int_dynamic_array_ptr = NULL;
    DOOM_ASSERT(doom_dynamic_array_front(*int_dynamic_array_ptr) == NULL);
    DOOM_ASSERT((int_dynamic_array = (int32_t *) doom_dynamic_array_new(sizeof(int32_t))) != NULL);
    DOOM_ASSERT(doom_dynamic_array_front(*int_dynamic_array_ptr) == NULL);
    doom_dynamic_array_deinit(*int_dynamic_array_ptr);
    DOOM_ASSERT((int_dynamic_array = (int32_t *) doom_dynamic_array_new_copy(sizeof(int32_t), from, 5)) != NULL);
    DOOM_ASSERT((int_ptr = (int32_t *) doom_dynamic_array_front((void *) int_dynamic_array)) != NULL);
    DOOM_ASSERT(*int_ptr == 1);

    DOOM_TEST_CASE(" - doom_dynamic_array_push_front\n");
    int_aux = 0;
    DOOM_ASSERT(doom_dynamic_array_push_front(int_dynamic_array_ptr, &int_aux) == 0);
    DOOM_ASSERT(doom_dynamic_array_get_struct(&dynamic_array_struct, *int_dynamic_array_ptr) == 0);
    DOOM_ASSERT(*dynamic_array_struct._array_size == 6);
    DOOM_ASSERT(*dynamic_array_struct._array_max_size == 10);
    DOOM_ASSERT(*dynamic_array_struct._sizeof_elem == sizeof(int32_t));
    for (int i=0; i<6; i++) {
        DOOM_ASSERT(int_dynamic_array[i] == i);
    }
    DOOM_ASSERT(doom_dynamic_array_fit(int_dynamic_array_ptr) == 0);
    int_aux = -1;
    DOOM_ASSERT(doom_dynamic_array_push_front(int_dynamic_array_ptr, &int_aux) == 0);
    DOOM_ASSERT(doom_dynamic_array_get_struct(&dynamic_array_struct, *int_dynamic_array_ptr) == 0);
    DOOM_ASSERT(*dynamic_array_struct._array_size == 7);
    DOOM_ASSERT(*dynamic_array_struct._array_max_size == 12);
    DOOM_ASSERT(*dynamic_array_struct._sizeof_elem == sizeof(int32_t));
    for (int i=0; i<7; i++) {
        DOOM_ASSERT(int_dynamic_array[i] == i-1);
    }

    DOOM_TEST_CASE(" - doom_dynamic_array_pop_front\n");
    DOOM_ASSERT(doom_dynamic_array_pop_front(int_dynamic_array_ptr) == 0);
    DOOM_ASSERT(doom_dynamic_array_get_struct(&dynamic_array_struct, *int_dynamic_array_ptr) == 0);
    DOOM_ASSERT(*dynamic_array_struct._array_size == 6);
    DOOM_ASSERT(*dynamic_array_struct._array_max_size == 12);
    DOOM_ASSERT(*dynamic_array_struct._sizeof_elem == sizeof(int32_t));
    for (int i=0; i<6; i++) {
        DOOM_ASSERT(int_dynamic_array[i] == i);
    }
    DOOM_ASSERT(doom_dynamic_array_pop_front(int_dynamic_array_ptr) == 0);
    DOOM_ASSERT(doom_dynamic_array_fit(int_dynamic_array_ptr) == 0);
    DOOM_ASSERT(doom_dynamic_array_get_struct(&dynamic_array_struct, *int_dynamic_array_ptr) == 0);
    DOOM_ASSERT(*dynamic_array_struct._array_size == 5);
    DOOM_ASSERT(*dynamic_array_struct._array_max_size == 5);
    DOOM_ASSERT(*dynamic_array_struct._sizeof_elem == sizeof(int32_t));
    for (int i=0; i<5; i++) {
        DOOM_ASSERT(int_dynamic_array[i] == i+1);
    }
    doom_dynamic_array_deinit(*int_dynamic_array_ptr);

    /* Doom List Test */
    printf("Doomsday C List:\n");
    doom_list_node node;
    memset(&node, 0, sizeof(doom_list_node));
    doom_list_node * node_ptr;
    node_ptr = NULL;
    doom_list list;
    memset(&list, 0, sizeof(doom_list));
    doom_list * list_ptr;
    list_ptr = NULL;

    DOOM_TEST_CASE(" - doom_list_node_new\n");
    DOOM_ASSERT((node_ptr = doom_list_node_new()) != NULL);
    DOOM_ASSERT(node_ptr->payload == NULL);
    DOOM_ASSERT(node_ptr->next == NULL);
    DOOM_ASSERT(node_ptr->prev == NULL);
    doom_list_node_deinit(node_ptr);

    DOOM_TEST_CASE(" - doom_list_node_init\n");
    node_ptr = NULL;
    DOOM_ASSERT(doom_list_node_init(node_ptr, sizeof(double)) == -1);
    DOOM_ASSERT((node_ptr = doom_list_node_new()) != NULL);
    DOOM_ASSERT(doom_list_node_init(node_ptr, sizeof(double)) == sizeof(double));
    *((double *)node_ptr->payload) = 10.10;
    DOOM_ASSERT((*(double *) node_ptr->payload) == 10.10);

    DOOM_TEST_CASE(" - doom_list_node_deinit\n");
    doom_list_node_deinit(node_ptr);


    DOOM_TEST_CASE(" - doom_list_node_assign\n");
    node_ptr = NULL;
    int32_t aux_int = 6969;
    DOOM_ASSERT(doom_list_node_assign(node_ptr, &aux_int, sizeof(int32_t)) == -1);
    DOOM_ASSERT((node_ptr = doom_list_node_new()) != NULL);
    DOOM_ASSERT(doom_list_node_init(node_ptr, sizeof(int32_t)) == sizeof(int32_t));
    DOOM_ASSERT(doom_list_node_assign(node_ptr, NULL, sizeof(int32_t)) == -1);
    DOOM_ASSERT(doom_list_node_assign(node_ptr, (void *)&aux_int, sizeof(int32_t)) == 0);
    DOOM_ASSERT((*((int32_t *)node_ptr->payload)) == aux_int);
    doom_list_node_deinit(node_ptr);

    DOOM_TEST_CASE(" - doom_list_node_get\n");
    node_ptr = NULL;
    float aux_float = 69.69f;
    DOOM_ASSERT(doom_list_node_get(node_ptr) == NULL);
    DOOM_ASSERT((node_ptr = doom_list_node_new()) != NULL);
    DOOM_ASSERT(doom_list_node_get(node_ptr) == NULL);
    DOOM_ASSERT(doom_list_node_init(node_ptr, sizeof(float)) == sizeof(float));
    DOOM_ASSERT(doom_list_node_assign(node_ptr, (void *)&aux_float, sizeof(float)) == 0);
    DOOM_ASSERT(*((float *)doom_list_node_get(node_ptr)) == aux_float);
    doom_list_node_deinit(node_ptr);

    DOOM_TEST_CASE(" - doom_list_node_insert_after\n");

    DOOM_TEST_CASE(" - doom_list_node_insert_before\n");

    DOOM_TEST_CASE(" - doom_list_new\n");
    DOOM_ASSERT((list_ptr = doom_list_new(sizeof(double))) != NULL);
    DOOM_ASSERT(list_ptr->_size == 0);
    DOOM_ASSERT(list_ptr->_data_size == sizeof(double));
    DOOM_ASSERT(list_ptr->first == NULL);
    DOOM_ASSERT(list_ptr->last == NULL);
    doom_list_deinit(list_ptr);

    DOOM_TEST_CASE(" - doom_list_init\n");
    list_ptr = NULL;
    DOOM_ASSERT((doom_list_init(&list_ptr, sizeof(byte))) == 0);
    DOOM_ASSERT(list_ptr->_size == 0);
    DOOM_ASSERT(list_ptr->_data_size == sizeof(byte));
    DOOM_ASSERT(list_ptr->first == NULL);
    DOOM_ASSERT(list_ptr->last == NULL);
    doom_list_deinit(list_ptr);

    DOOM_TEST_CASE(" - doom_list_push_front\n");
    list_ptr = NULL;
    DOOM_ASSERT((doom_list_push_front(NULL, NULL)) == -1);
    DOOM_ASSERT((doom_list_push_front(list_ptr, (void *)&aux_int)) == -1);
    DOOM_ASSERT((list_ptr = doom_list_new(sizeof(int32_t))) != NULL);
    DOOM_ASSERT((doom_list_push_front(list_ptr, NULL)) == -1);
    DOOM_ASSERT((doom_list_push_front(list_ptr, &aux_int)) == 0);
    DOOM_ASSERT(list_ptr->_size == 1);
    DOOM_ASSERT(list_ptr->_data_size == sizeof(int32_t));
    DOOM_ASSERT(list_ptr->first != NULL);
    DOOM_ASSERT(list_ptr->last != NULL);
    DOOM_ASSERT(list_ptr->first->prev == NULL);
    DOOM_ASSERT(list_ptr->first->next == NULL);
    DOOM_ASSERT((*(int32_t *)list_ptr->first->payload) == aux_int);
    aux_int = 9696;
    DOOM_ASSERT((doom_list_push_front(list_ptr, &aux_int)) == 0);
    DOOM_ASSERT(list_ptr->_size == 2);
    DOOM_ASSERT(list_ptr->_data_size == sizeof(int32_t));
    DOOM_ASSERT(list_ptr->first != NULL);
    DOOM_ASSERT(list_ptr->last != NULL);
    DOOM_ASSERT(list_ptr->first->prev == NULL);
    DOOM_ASSERT(list_ptr->first->next != NULL);
    DOOM_ASSERT(list_ptr->first->next->prev != NULL);
    DOOM_ASSERT(list_ptr->first->next->next == NULL);
    DOOM_ASSERT((*(int32_t *)list_ptr->first->payload) == aux_int);

    DOOM_TEST_CASE(" - doom_list_pop_front\n");
    DOOM_ASSERT(doom_list_pop_front(NULL) == -1);
    DOOM_ASSERT(doom_list_pop_front(list_ptr) == 0);
    DOOM_ASSERT(list_ptr->first == list_ptr->last);
    DOOM_ASSERT(list_ptr->_size == 1);
    DOOM_ASSERT(doom_list_pop_front(list_ptr) == 0);
    DOOM_ASSERT(list_ptr->first == NULL);
    DOOM_ASSERT(list_ptr->last == NULL);
    DOOM_ASSERT(list_ptr->_size == 0);
    DOOM_ASSERT(doom_list_pop_front(list_ptr) == -1);
    doom_list_deinit(list_ptr);

    DOOM_TEST_CASE(" - doom_list_push_back\n");
    list_ptr = NULL;
    DOOM_ASSERT((doom_list_push_back(NULL, NULL)) == -1);
    DOOM_ASSERT((doom_list_push_back(list_ptr, (void *)&aux_int)) == -1);
    DOOM_ASSERT((list_ptr = doom_list_new(sizeof(int32_t))) != NULL);
    DOOM_ASSERT((doom_list_push_back(list_ptr, NULL)) == -1);
    DOOM_ASSERT((doom_list_push_back(list_ptr, &aux_int)) == 0);
    DOOM_ASSERT(list_ptr->_size == 1);
    DOOM_ASSERT(list_ptr->_data_size == sizeof(int32_t));
    DOOM_ASSERT(list_ptr->first != NULL);
    DOOM_ASSERT(list_ptr->last != NULL);
    DOOM_ASSERT(list_ptr->last->prev == NULL);
    DOOM_ASSERT(list_ptr->last->next == NULL);
    DOOM_ASSERT((*(int32_t *)list_ptr->last->payload) == aux_int);
    aux_int = 6969;
    DOOM_ASSERT((doom_list_push_back(list_ptr, &aux_int)) == 0);
    DOOM_ASSERT(list_ptr->_size == 2);
    DOOM_ASSERT(list_ptr->_data_size == sizeof(int32_t));
    DOOM_ASSERT(list_ptr->first != NULL);
    DOOM_ASSERT(list_ptr->last != NULL);
    DOOM_ASSERT(list_ptr->first->prev == NULL);
    DOOM_ASSERT(list_ptr->first->next != NULL);
    DOOM_ASSERT(list_ptr->first->next->prev != NULL);
    DOOM_ASSERT(list_ptr->first->next->next == NULL);
    DOOM_ASSERT((*(int32_t *)list_ptr->last->payload) == aux_int);

    DOOM_TEST_CASE(" - doom_list_pop_back\n");
    DOOM_ASSERT(doom_list_pop_back(NULL) == -1);
    DOOM_ASSERT(doom_list_pop_back(list_ptr) == 0);
    DOOM_ASSERT(list_ptr->first == list_ptr->last);
    DOOM_ASSERT(list_ptr->_size == 1);
    DOOM_ASSERT(doom_list_pop_back(list_ptr) == 0);
    DOOM_ASSERT(list_ptr->first == NULL);
    DOOM_ASSERT(list_ptr->last == NULL);
    DOOM_ASSERT(list_ptr->_size == 0);
    DOOM_ASSERT(doom_list_pop_back(list_ptr) == -1);
    doom_list_deinit(list_ptr);

    DOOM_TEST_CASE(" - doom_list_front\n");
    DOOM_ASSERT((list_ptr = doom_list_new(sizeof(float))) != NULL);
    DOOM_ASSERT((doom_list_push_front(list_ptr, &aux_float)) == 0);
    DOOM_ASSERT(list_ptr->_size == 1);
    DOOM_ASSERT(list_ptr->_data_size == sizeof(int32_t));
    DOOM_ASSERT(list_ptr->first != NULL);
    DOOM_ASSERT(list_ptr->last != NULL);
    DOOM_ASSERT(list_ptr->first->prev == NULL);
    DOOM_ASSERT(list_ptr->first->next == NULL);
    DOOM_ASSERT((*(float *)list_ptr->first->payload) == aux_float);
    DOOM_ASSERT(*(float *)doom_list_front(list_ptr) == aux_float);

    DOOM_TEST_CASE(" - doom_list_back\n");
    DOOM_ASSERT(*(float *)doom_list_back(list_ptr) == aux_float);
    doom_list_deinit(list_ptr);

    DOOM_TEST_CASE(" - doom_list_at\n");
    list_ptr = NULL;
    DOOM_ASSERT((list_ptr = doom_list_new(sizeof(uint32_t))) != NULL);
    for (int32_t i=0; i<10; i++) {
        DOOM_ASSERT(doom_list_push_back(list_ptr, &i) == 0);
    }
    DOOM_ASSERT((int32_t*)doom_list_at(NULL, 0) == NULL);
    DOOM_ASSERT((int32_t*)doom_list_at(list_ptr, -1) == NULL);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 0) == 0);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 1) == 1);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 2) == 2);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 3) == 3);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 4) == 4);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 5) == 5);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 6) == 6);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 7) == 7);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 8) == 8);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 9) == 9);
    DOOM_ASSERT((int32_t*)doom_list_at(list_ptr, 10) == NULL);

    DOOM_TEST_CASE(" - doom_list_insert_at\n");
    aux_int = 96;
    DOOM_ASSERT(doom_list_insert_at(NULL, &aux_int, 0) == -1);
    DOOM_ASSERT(doom_list_insert_at(list_ptr, NULL, 0) == -1);
    DOOM_ASSERT(doom_list_insert_at(list_ptr, &aux_int, -1) == -1);
    DOOM_ASSERT(doom_list_insert_at(list_ptr, &aux_int, 100) == -1);
    DOOM_ASSERT(doom_list_insert_at(list_ptr, &aux_int, 0) == 0);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 0) == 96);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 1) == 0);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 2) == 1);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 3) == 2);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 4) == 3);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 5) == 4);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 6) == 5);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 7) == 6);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 8) == 7);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 9) == 8);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 10) == 9);
    aux_int = 196;
    DOOM_ASSERT(doom_list_insert_at(list_ptr, &aux_int, 11) == 0);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 0) == 96);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 1) == 0);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 2) == 1);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 3) == 2);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 4) == 3);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 5) == 4);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 6) == 5);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 7) == 6);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 8) == 7);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 9) == 8);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 10) == 9);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 11) == 196);
    aux_int = 2196;
    DOOM_ASSERT(doom_list_insert_at(list_ptr, &aux_int, 5) == 0);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 0) == 96);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 1) == 0);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 2) == 1);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 3) == 2);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 4) == 3);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 5) == 2196);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 6) == 4);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 7) == 5);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 8) == 6);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 9) == 7);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 10) == 8);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 11) == 9);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 12) == 196);

    DOOM_TEST_CASE(" - doom_list_remove_at\n");
    DOOM_ASSERT(doom_list_remove_at(NULL, 4) == -1);
    DOOM_ASSERT(doom_list_remove_at(list_ptr, -1) == -1);
    DOOM_ASSERT(doom_list_remove_at(list_ptr, list_ptr->_size+10) == -1);
    DOOM_ASSERT(doom_list_remove_at(list_ptr, 0) == 0);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 0) == 0);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 1) == 1);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 2) == 2);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 3) == 3);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 4) == 2196);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 5) == 4);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 6) == 5);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 7) == 6);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 8) == 7);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 9) == 8);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 10) == 9);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 11) == 196);
    DOOM_ASSERT(doom_list_remove_at(list_ptr, 11) == 0);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 0) == 0);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 1) == 1);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 2) == 2);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 3) == 3);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 4) == 2196);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 5) == 4);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 6) == 5);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 7) == 6);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 8) == 7);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 9) == 8);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 10) == 9);
    DOOM_ASSERT(doom_list_remove_at(list_ptr, 4) == 0);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 0) == 0);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 1) == 1);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 2) == 2);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 3) == 3);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 4) == 4);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 5) == 5);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 6) == 6);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 7) == 7);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 8) == 8);
    DOOM_ASSERT(*(int32_t*)doom_list_at(list_ptr, 9) == 9);

    DOOM_TEST_CASE(" - doom_list_deinit\n");
    doom_list_deinit(list_ptr);

    /* End of Testing */
    printf("__________________________________________________\n");
    printf("Tests Succeeded!\n");
    printf("Tested %lu assertions in %lu test cases!\n", asserts_count, test_cases_count);
    fflush(stdout);

    exit(EXIT_SUCCESS);
}
