#ifndef __LLULU_BUFFER_H__
#define __LLULU_BUFFER_H__

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct lu_data {
    const void *data;
    size_t size; /* Bytes */
} lu_data;

typedef struct lu_buffer {
    char *data;
    size_t size;
} lu_buffer;

typedef struct lu_array {
    lu_buffer buf;
    size_t count;
    size_t elem_sz;
} lu_array;

typedef struct lu_ring {
    lu_buffer buf;
    size_t write;
    size_t read;
} lu_ring;

static inline void *lu_array_push(lu_array *self)
{
    if ((self->count + 1) * self->elem_sz > self->buf.size) {
        char *new_data = (char*)malloc(self->buf.size * 2);
        if (new_data) {
            memcpy(new_data, self->buf.data, self->buf.size);
            free(self->buf.data);
            self->buf.data = new_data;
            self->buf.size *= 2;
        } else {
            printf("lu_arr_push failed to realloc buffer to %llu bytes.\n", self->buf.size * 2);
            return NULL;
        }
    }

    return &self->buf.data[self->count++ * self->elem_sz];
}

static inline void *lu_array_pop(lu_array *self)
{
    return &self->buf.data[--self->count * self->elem_sz];
}

#endif /* __LLULU_BUFFER_H__ */
