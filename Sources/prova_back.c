#include <stdint.h>
#include <stdio.h>
#define DOOMSDAY_C_DYNAMIC_ARRAY_STRIP_PREFIX
#define DOOMSDAY_C_DYNAMIC_ARRAY_IMPLEMENTATION
#include "doomsday_c_dynamic_array.h"


int main(void) {
    
    int64_t * da_int = NULL;
    doom_dynamic_array_struct da_int_struct;

    doom_dynamic_array_init((void**)&da_int, sizeof(int64_t));

    doom_dynamic_array_get_struct(&da_int_struct, da_int);
    setbuf(stdout, NULL);
    printf("{ _array='");
    for (int64_t i=0; i<*da_int_struct._array_size; i++) {
        printf("%li,", da_int[i]);
    }
    printf("', _sizeof_elem=%li, _size=%li, _max_size=%li }\n", 
           *da_int_struct._sizeof_elem, *da_int_struct._array_size, *da_int_struct._array_max_size);

    doom_dynamic_array_deinit(da_int);

    doom_dynamic_array_init_s((void**)&da_int, sizeof(int64_t), 100);

    doom_dynamic_array_get_struct(&da_int_struct, da_int);
    setbuf(stdout, NULL);
    printf("{ _array='");
    for (int64_t i=0; i<*da_int_struct._array_size; i++) {
        printf("%li,", da_int[i]);
    }
    printf("', _sizeof_elem=%li, _size=%li, _max_size=%li }\n", 
           *da_int_struct._sizeof_elem, *da_int_struct._array_size, *da_int_struct._array_max_size);

    doom_dynamic_array_deinit(da_int);

    int64_t pippo[] = {1, 2, 3, 4};
    doom_dynamic_array_init_c((void**)&da_int, sizeof(int64_t), pippo, 4);

    doom_dynamic_array_get_struct(&da_int_struct, da_int);
    setbuf(stdout, NULL);
    printf("{ _array='");
    for (int64_t i=0; i<*da_int_struct._array_size; i++) {
        printf("%li,", da_int[i]);
    }
    printf("', _sizeof_elem=%li, _size=%li, _max_size=%li }\n", 
           *da_int_struct._sizeof_elem, *da_int_struct._array_size, *da_int_struct._array_max_size);

    doom_dynamic_array_deinit(da_int);


    da_int = doom_dynamic_array_factory(sizeof(int64_t));

    doom_dynamic_array_get_struct(&da_int_struct, da_int);
    setbuf(stdout, NULL);
    printf("{ _array='");
    for (int64_t i=0; i<*da_int_struct._array_size; i++) {
        printf("%li,", da_int[i]);
    }
    printf("', _sizeof_elem=%li, _size=%li, _max_size=%li }\n", 
           *da_int_struct._sizeof_elem, *da_int_struct._array_size, *da_int_struct._array_max_size);

    doom_dynamic_array_deinit(da_int);

    da_int = doom_dynamic_array_factory_size(sizeof(int64_t), 10);

    doom_dynamic_array_get_struct(&da_int_struct, da_int);
    setbuf(stdout, NULL);
    printf("{ _array='");
    for (int64_t i=0; i<*da_int_struct._array_size; i++) {
        printf("%li,", da_int[i]);
    }
    printf("', _sizeof_elem=%li, _size=%li, _max_size=%li }\n", 
           *da_int_struct._sizeof_elem, *da_int_struct._array_size, *da_int_struct._array_max_size);

    doom_dynamic_array_deinit(da_int);

    int64_t paperino[] = {1, -2, 3, -4, 5, -6, 7, -8, 9, -10, 11, -12};
    da_int = doom_dynamic_array_factory_copy(sizeof(int64_t), paperino, 12);

    setbuf(stdout, NULL);
    printf("{ _array='");
    for (int64_t i=0; i<doom_dynamic_array_get_size(da_int); i++) {
        printf("%li,", da_int[i]);
    }
    printf("', _sizeof_elem=%li, _size=%li, _max_size=%li }\n", 
           doom_dynamic_array_get_elem_size(da_int),
           doom_dynamic_array_get_size(da_int), 
           doom_dynamic_array_get_max_size(da_int)
          );

    doom_dynamic_array_expand((void **)&da_int);
    printf("{ _array='");
    for (int64_t i=0; i<doom_dynamic_array_get_size(da_int); i++) {
        printf("%li,", da_int[i]);
    }
    printf("', _sizeof_elem=%li, _size=%li, _max_size=%li }\n", 
           doom_dynamic_array_get_elem_size(da_int),
           doom_dynamic_array_get_size(da_int), 
           doom_dynamic_array_get_max_size(da_int)
          );

    doom_dynamic_array_deinit(da_int);

    da_int = doom_dynamic_array_factory_size(sizeof(int64_t), 3);

    doom_dynamic_array_get_struct(&da_int_struct, da_int);
    setbuf(stdout, NULL);
    printf("{ _array='");
    for (int64_t i=0; i<*da_int_struct._array_size; i++) {
        printf("%li,", da_int[i]);
    }
    printf("', _sizeof_elem=%li, _size=%li, _max_size=%li }\n", 
           *da_int_struct._sizeof_elem, *da_int_struct._array_size, *da_int_struct._array_max_size);


    doom_dynamic_array_resize((void **)&da_int, 3);
    doom_dynamic_array_get_struct(&da_int_struct, da_int);
    setbuf(stdout, NULL);
    printf("{ _array='");
    for (int64_t i=0; i<*da_int_struct._array_size; i++) {
        printf("%li,", da_int[i]);
    }
    printf("', _sizeof_elem=%li, _size=%li, _max_size=%li }\n", 
           *da_int_struct._sizeof_elem, *da_int_struct._array_size, *da_int_struct._array_max_size);

    doom_dynamic_array_resize((void **)&da_int, 5);
    doom_dynamic_array_get_struct(&da_int_struct, da_int);
    setbuf(stdout, NULL);
    printf("{ _array='");
    for (int64_t i=0; i<*da_int_struct._array_size; i++) {
        printf("%li,", da_int[i]);
    }
    printf("', _sizeof_elem=%li, _size=%li, _max_size=%li }\n", 
           *da_int_struct._sizeof_elem, *da_int_struct._array_size, *da_int_struct._array_max_size);

    doom_dynamic_array_resize((void **)&da_int, 56);
    doom_dynamic_array_get_struct(&da_int_struct, da_int);
    setbuf(stdout, NULL);
    printf("{ _array='");
    for (int64_t i=0; i<*da_int_struct._array_size; i++) {
        printf("%li,", da_int[i]);
    }
    printf("', _sizeof_elem=%li, _size=%li, _max_size=%li }\n", 
           *da_int_struct._sizeof_elem, *da_int_struct._array_size, *da_int_struct._array_max_size);

    doom_dynamic_array_resize((void **)&da_int, 2);
    doom_dynamic_array_get_struct(&da_int_struct, da_int);
    setbuf(stdout, NULL);
    printf("{ _array='");
    for (int64_t i=0; i<*da_int_struct._array_size; i++) {
        printf("%li,", da_int[i]);
    }
    printf("', _sizeof_elem=%li, _size=%li, _max_size=%li }\n", 
           *da_int_struct._sizeof_elem, *da_int_struct._array_size, *da_int_struct._array_max_size);

    doom_dynamic_array_deinit(da_int);

    da_int = doom_dynamic_array_factory_size(sizeof(int64_t), 8);
    doom_dynamic_array_get_struct(&da_int_struct, da_int);
    setbuf(stdout, NULL);
    printf("{ _array='");
    for (int64_t i=0; i<*da_int_struct._array_size; i++) {
        printf("%li,", da_int[i]);
    }
    printf("', _sizeof_elem=%li, _size=%li, _max_size=%li }\n", 
           *da_int_struct._sizeof_elem, *da_int_struct._array_size, *da_int_struct._array_max_size);

    doom_dynamic_array_reserve((void **)&da_int, 34);
    doom_dynamic_array_get_struct(&da_int_struct, da_int);
    setbuf(stdout, NULL);
    printf("{ _array='");
    for (int64_t i=0; i<*da_int_struct._array_size; i++) {
        printf("%li,", da_int[i]);
    }
    printf("', _sizeof_elem=%li, _size=%li, _max_size=%li }\n", 
           *da_int_struct._sizeof_elem, *da_int_struct._array_size, *da_int_struct._array_max_size);

    doom_dynamic_array_reserve((void **)&da_int, 15);
    doom_dynamic_array_get_struct(&da_int_struct, da_int);
    setbuf(stdout, NULL);
    printf("{ _array='");
    for (int64_t i=0; i<*da_int_struct._array_size; i++) {
        printf("%li,", da_int[i]);
    }
    printf("', _sizeof_elem=%li, _size=%li, _max_size=%li }\n", 
           *da_int_struct._sizeof_elem, *da_int_struct._array_size, *da_int_struct._array_max_size);

    doom_dynamic_array_deinit(da_int);
}
