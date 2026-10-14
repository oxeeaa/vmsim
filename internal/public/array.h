//
//
//      array.h
//      Alan Ksenzov 2026-10-08
//
//

#pragma once

#include <stddef.h>


typedef struct {
    size_t  capacity,
            size,
            elem_size;
} array_t;

#define ARRAY_HEAD(a) ((array_t*)(a) - 1)
#define ARRAY_SIZE(a) (ARRAY_HEAD(a)->size)
#define ARRAY_CAPACITY(a) (ARRAY_HEAD(a)->capacity)
#define ARRAY_ELEM_SIZE(a) (ARRAY_HEAD()->elem_size)


// ALlocates new array.
void*   array_new(size_t capacity, size_t elem_size);

// Deletes array a.
void    array_delete(void* a);

// Appends value pval to array a.
// Returns pointer to reallocated memory if reallocation took place.
void*   array_append(void* restrict a, const void* restrict pval);

// Pops an item pout from array a.
// Might shrink the array (i have not decided yet) so it returns a pointer to the array.
void*   array_pop(void* restrict a, void** restrict pout);


#ifdef ARRAY_IMPLEMENTATION

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"


void* array_new(size_t capacity, size_t elem_size) {
    if (capacity == 0 || elem_size == 0)
        return NULL;

    
    size_t total_size = sizeof(array_t) + capacity * elem_size;
    array_t* a = (array_t*)malloc(total_size);
    if (!a)
        return NULL;

    a->elem_size = elem_size;
    a->capacity = capacity;
    a->size = 0;

    
    return (void*)(a + 1);
}

void array_delete(void *a) {
    if (!a) return;

    free(ARRAY_HEAD(a));
}


void* array_append(void* restrict a, const void* restrict pval) {
    if (!a) return NULL;

    array_t* head = ARRAY_HEAD(a);

    if (head->size >= head->capacity) {
        head->capacity *= 2;
        head = realloc(head, sizeof(array_t) + head->capacity * head->elem_size);
        if (!head)
            return NULL;
    }
    
    char* buffer = (char*)(head + 1);
    char* dst = buffer + head->size * head->elem_size;
    memcpy(dst, pval, head->elem_size);

    ++head->size;
    
    return (void*)(head + 1);
}

void* array_pop(void* restrict a, void** restrict pout) {
    if (!a) return NULL;

    array_t* head = ARRAY_HEAD(a);

    // Since array might have garbage data in it
    if (head->size == 0) {
        *pout = NULL;
        return a;
    }

    char* buffer = a;
    char* src = buffer + head->size * head->elem_size;
    memcpy(*pout, src, head->elem_size);
    
    head->size--;

    return a;
}

#endif // ARRAY_IMPLEMENTATION
