#include "llulu/lu_str.h"
#include <llulu/lu_buffer.h>
#include <llulu/lu_sync.h>
#include <stdbool.h>
#include <string.h>

lu_err
lu_arr_init(lu_array *self, void *buf, size_t elem_cap, size_t elem_sz)
{
    if (!buf || !elem_cap || !elem_sz) {
        return LU_ERR_BADARG;
    }

    self->count = 0;
    self->elem_sz = elem_sz;
    self->elem_cap = elem_cap;
    self->data = buf;
    return LU_ERR_SUCCESS;
}

void
lu_arr_put(lu_array *self, void *data, size_t elem_count)
{
    lu_err_assert(self && data && elem_count);
    lu_err_assert((self->elem_cap - self->count) > elem_count);
    memcpy(&self->data[self->count * self->elem_sz], data, elem_count * self->elem_sz);
    self->count += elem_count;
}

void
lu_arr_pull(lu_array *self, void *buf, size_t elem_count)
{
    lu_err_assert(self && buf && elem_count);
    lu_err_assert(self->count >= elem_count);
    self->count -= elem_count;
    memcpy(buf, &self->data[self->count * self->elem_sz], elem_count * self->elem_sz);
}

void *
lu_arr_push(lu_array *self)
{
    lu_err_assert(self);
    lu_err_assert(self->elem_cap > self->count);
    return &self->data[self->elem_sz * self->count++];
}

void *
lu_arr_pop(lu_array *self)
{
    lu_err_assert(self);
    lu_err_assert(self->count);
    return &self->data[--self->count * self->elem_sz];
}

void *
lu_arr_at(lu_array *self, size_t index)
{
    lu_err_assert(self);
    lu_err_assert(index < self->count);
    return &self->data[index * self->elem_sz];
}

static inline size_t
lu_ring_read_avail(lu_ring *self)
{
    return self->write_idx - self->read_idx;
}

static inline size_t
lu_ring_write_avail(lu_ring *self)
{
    return self->elem_cap - lu_ring_read_avail(self);
}

static size_t
lu_ring_read_claim(lu_ring *self, size_t elem_count,
                   void **data1, size_t *count1,
                   void **data2, size_t *count2)
{
    size_t avail = lu_ring_read_avail(self);
    elem_count = elem_count > avail ? avail : elem_count;
    size_t idx = self->read_idx & self->idx_mask;
    if ((idx + elem_count) > self->elem_cap) {
        *data1 = &self->data[idx * self->elem_sz];
        *count1 = self->elem_cap - idx;
        *data2 = &self->data[0];
        *count2 = elem_count - *count1;
    } else {
        *data1 = &self->data[idx * self->elem_sz];
        *count1 = elem_count;
        *data2 = NULL;
        *count2 = 0;
    }

    if (avail) {
        lu_sync_rbarrier();
    }

    return elem_count;
}

static size_t
lu_ring_write_claim(lu_ring *self, size_t elem_count,
                    void **data1, size_t *count1,
                    void **data2, size_t *count2)
{
    size_t avail = lu_ring_write_avail(self);
    elem_count = elem_count > avail ? avail : elem_count;
    size_t idx = self->write_idx & self->idx_mask;
    if ((idx + elem_count) > self->elem_cap) {
        *data1 = &self->data[idx * self->elem_sz];
        *count1 = self->elem_cap - idx;
        *data2 = &self->data[0];
        *count2 = elem_count - *count1;
    } else {
        *data1 = &self->data[idx * self->elem_sz];
        *count1 = elem_count;
        *data2 = NULL;
        *count2 = 0;
    }

    if (avail) {
        lu_sync_barrier();
    }

    return elem_count;
}

lu_err
lu_ring_init(lu_ring *self, void *buf, size_t elem_count, size_t elem_sz)
{
    if (!elem_count || !elem_sz) {
        return LU_ERR_BADARG;
    }

    if (((elem_count - 1) * elem_count) != 0) {
        return LU_ERR_BADARG;
    }

    self->data = buf;
    self->elem_sz = elem_sz;
    self->elem_cap = elem_count;
    self->read_idx = 0;
    self->write_idx = 0;
    self->idx_mask = elem_count - 1;

    return LU_ERR_SUCCESS;
}

/* 
 * Copy elem_count items to out_buf. Moves the read pointer.
 * out_buf must have at least elem_count * elem_sz bytes.
 * Return elements read (could be less than elem_count).
 */
size_t
lu_ring_read(lu_ring *self, void *out_buf, size_t elem_count)
{
    void *data1;
    void *data2;
    size_t count1;
    size_t count2;

    size_t read_count = lu_ring_read_claim(self, elem_count, &data1, &count1, &data2, &count2);
    if (count2) {
        memcpy(out_buf, data1, count1 * self->elem_sz);
        out_buf = ((char*)out_buf) + count1 * self->elem_sz;
        memcpy(out_buf, data2, count2 * self->elem_sz);
    } else {
        memcpy(out_buf, data1, count1 * self->elem_sz);
    }

    lu_sync_barrier();
    self->read_idx += read_count;
    return read_count;
}

/*
 * Copy elem_count items from in_buf. Moves the write pointer.
 * in_buf must have at least elem_count * elem_sz bytes of data to write.
 * Return elements written (could be less than elem_count).
 */
size_t
lu_ring_write(lu_ring *self, void *in_buf, size_t elem_count)
{
    void *data1;
    void *data2;
    size_t count1;
    size_t count2;

    size_t write_count = lu_ring_write_claim(self, elem_count, &data1, &count1, &data2, &count2);
    if (count2) {
        memcpy(data1, in_buf, count1 * self->elem_sz);
        in_buf = ((char*)in_buf) + count1 * self->elem_sz;
        memcpy(data2, in_buf, count2 * self->elem_sz);
    } else {
        memcpy(data1, in_buf, count1 * self->elem_sz);
    }

    lu_sync_wbarrier();
    self->write_idx += write_count;
    return write_count;
}

lu_err
lu_pool_init(lu_pool *self, void *buf, size_t elem_cap, size_t elem_sz)
{
    lu_err_assert(self && buf && elem_cap);
    if (elem_sz < sizeof(lu_pool_hnd)) {
        return LU_ERR_BADARG;
    }

    self->elem_sz = elem_sz;
    self->elem_cap = elem_cap;
    self->free_list = -1;
    self->head = 0;
    self->data = buf;
    return LU_ERR_SUCCESS;
}

lu_pool_hnd
lu_pool_push(lu_pool *self, void *in)
{
    lu_pool_hnd ret = self->free_list;
    if (self->free_list >= 0) {
        self->free_list = *(lu_pool_hnd*)&self->data[self->free_list * self->elem_sz];
    } else if (self->head < self->elem_cap) {
        ret = self->head++;
    } else {
        return -1;
    }

    memcpy(&self->data[ret * self->elem_sz], in, self->elem_sz);
    return ret;
}

void *
lu_pool_pull(lu_pool *self, void *out, lu_pool_hnd handle)
{
    if (handle < self->head) {
        memcpy(out, &self->data[handle * self->elem_sz], self->elem_sz);
        *(lu_pool_hnd*)&self->data[handle * self->elem_sz] = self->free_list;
        self->free_list = handle;
        return out;
    }
    return NULL;
}

void *
lu_pool_at(lu_pool *self, lu_pool_hnd handle)
{
    return &self->data[handle * self->elem_sz];
}

inline void
lu_map_clear(lu_map *self)
{
    memset(self->data, 0, self->elem_cap * sizeof(*self->data));
}

lu_err
lu_map_init(lu_map *self, void *buf, size_t elem_cap)
{
    self->elem_cap = elem_cap;
    self->data = buf;
    return LU_ERR_SUCCESS;
}

static inline bool
lu_map_slot_match(lu_map *self, int idx, lu_sstr *k)
{
    const lu_sstr * const other = &self->data[idx].key;
    return lu_sstr_is_empty(other) || lu_sstr_equals(k, other);
}

static inline int
lu_map_slot_find(lu_map *self, lu_sstr *k)
{
    const int first = lu_sstr_hash(k) % self->elem_cap;
    int i = first;
    for (; !lu_map_slot_match(self, i, k); ++i) { /* linear probing */
        if ((i % self->elem_cap) == first) {
            return -1;
        }
    }

    return i;
}

lu_err
lu_map_put(lu_map *self, lu_sstr *key, size_t value)
{
    int idx = lu_map_slot_find(self, key);
    if (idx < 0) {
        return LU_ERR_CONTAINER_FULL;
    }
    self->data[idx].key = *key;
    self->data[idx].value = value;
    
    return LU_ERR_SUCCESS;
}

lu_err
lu_map_take(lu_map *self, lu_sstr *key, size_t *out_value)
{
    int idx = lu_map_slot_find(self, key);
    if (idx < 0 || lu_sstr_is_empty(&self->data[idx].key)) {
        return LU_ERR_NOT_FOUND;
    }

    lu_sstr_clear(&self->data[idx].key);
    memcpy(out_value, &self->data[idx].value, sizeof(self->data->value));
    return LU_ERR_SUCCESS;
}

size_t *
lu_map_at(lu_map *self, lu_sstr *key)
{
    int idx = lu_map_slot_find(self, key);
    if (idx < 0 || lu_sstr_is_empty(&self->data[idx].key)) {
        return NULL;
    }
    return &self->data[idx].value;
}
