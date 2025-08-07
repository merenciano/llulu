#ifndef __LLULU_BUFFER_H__
#define __LLULU_BUFFER_H__

#include "lu_error.h"
#include "lu_str.h"
#include <stddef.h>

#define LU_ARR_ALLOCA(ARR, TYPE, COUNT) \
TYPE ARR##_##TYPE##_buf[(COUNT)] = {0}; \
lu_arr_init(&(ARR), ARR##_##TYPE##_buf, (COUNT), sizeof(TYPE))

typedef struct lu_array {
    size_t count;
    size_t elem_sz;
    size_t elem_cap;
    char *data;
} lu_array;

lu_err lu_arr_init(lu_array *self, void *buf, size_t elem_cap, size_t elem_sz);
void lu_arr_put(lu_array *self, void *data, size_t elem_count);
void lu_arr_take(lu_array *self, void *buf, size_t elem_count);
void *lu_arr_push(lu_array *self);
void *lu_arr_pop(lu_array *self);
void *lu_arr_at(lu_array *self, size_t index);
static inline void lu_arr_clear(lu_array *self) { self->count = 0; }
static inline size_t lu_arr_count(lu_array *self) { return self->count; }
static inline size_t lu_arr_cap(lu_array *self) { return self->elem_cap; }
static inline bool lu_arr_is_empty(lu_array *self) { return !self->count; }
static inline bool lu_arr_is_full(lu_array *self) { return self->elem_cap == self->count; }

typedef struct lu_ring {
    size_t elem_sz;
    size_t elem_cap; /* power of 2 required */
    volatile size_t read_idx;
    volatile size_t write_idx;
    size_t idx_mask;
    char *data;
} lu_ring;

static inline void lu_ring_clear(lu_ring *self) { self->read_idx = self->write_idx = 0;}
lu_err lu_ring_init(lu_ring *self, void *buf, size_t elem_count, size_t elem_sz); /* elem_count must be pow2 */
size_t lu_ring_read(lu_ring *self, void *out_buf, size_t elem_count);
size_t lu_ring_write(lu_ring *self, void *in_buf, size_t elem_count);

/* TODO: Another implementation for free_list (bitfields?) or mark
 * the free nodes in some way that it is possible to detect
 * handle use after free.
 */
typedef int lu_pool_hnd;
typedef struct lu_pool {
    size_t elem_sz; /* At least sizeof(lu_pool_hnd) bytes */
    size_t elem_cap;
    lu_pool_hnd free_list; /* each free_idx has the next free_idx as value */
    lu_pool_hnd head;
    char *data;
} lu_pool;

lu_err lu_pool_init(lu_pool *self, void *buf, size_t elem_cap, size_t elem_sz);
lu_pool_hnd lu_pool_put(lu_pool *self, void *in);
void *lu_pool_take(lu_pool *self, void *out, lu_pool_hnd handle);
void *lu_pool_at(lu_pool *self, lu_pool_hnd handle);

typedef struct lu_map_entry {
    lu_sstr key;
    size_t value;
} lu_map_entry;

typedef struct lu_map {
    size_t elem_cap;
    lu_map_entry *data;
} lu_map;

lu_err lu_map_init(lu_map *self, void *buf, size_t elem_cap);
lu_err lu_map_put(lu_map *self, lu_sstr *key, size_t value);
lu_err lu_map_take(lu_map *self, lu_sstr *key, size_t *out_value);
size_t *lu_map_at(lu_map *self, lu_sstr *key);
void lu_map_clear(lu_map *self);

#endif /* __LLULU_BUFFER_H__ */
