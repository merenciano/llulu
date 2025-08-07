#ifndef LLULU_HMAP_H
#define LLULU_HMAP_H

#include "lu_str.h"
#include <stddef.h>
#include <stdbool.h>

typedef struct lu_hmap_entry {
    lu_sstr key;
    size_t value;
} lu_hmap_entry;

typedef struct lu_hmap {
    lu_hmap_entry *data;
    size_t used;
    size_t capacity;
} lu_hmap;

static inline bool
lu_hmap_empty_or_equal(lu_hmap *hm, int index, lu_sstr *k)
{
    const lu_sstr *key = &hm->data[index].key;
    return lu_sstr_is_empty(key) || lu_sstr_equals(k, key);
}

static inline int
lu_hmap_find_index(lu_hmap *hm, lu_sstr *k)
{
    if (!hm) {
        return -1;
    }

    const int first_index = lu_sstr_hash(k) % hm->capacity;
    int index = first_index;
    while (!lu_hmap_empty_or_equal(hm, index, k)) {
        index = (index + 1) % hm->capacity;
        if (index == first_index) {
            return -1;
        }
    }

    return index;
}

static inline void
lu_hmap_insert_nocheck(lu_hmap *hm, int index, lu_sstr *k, size_t v)
{
    hm->data[index].key = *k;
    hm->data[index].value = v;
}

static inline int
lu_hmap_set(lu_hmap *hm, lu_sstr *k, size_t value)
{
    int index = lu_hmap_find_index(hm, k);
    if (index != -1) {
        lu_hmap_insert_nocheck(hm, index, k, value);
    }

    return index;
}

#endif /* LLULU_HMAP_H */

