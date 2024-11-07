#ifndef __LUC_BUFFER_H__
#define __LUC_BUFFER_H__

#include <stddef.h>
#include <stdint.h>

enum lu_buf_flags_e {
    LU_BUF_OWNED = 0x01, /* Owner of the memory pointed by head. Flag off: prevent free and resize operations. */
    LU_BUF_FIXED = 0x02, /* Prevent reallocations. Flag off: the buffer grows if the requested memory is greater than the available space. */
    LU_BUF_NULLTERM = 0x04, /* Null terminated data (usually text). */
    LU_BUF_READONLY = 0x08,
};

typedef struct {
    uint32_t flags;
    uint32_t element_size;
    size_t capacity;
    char *tail;
    char *head;
} lu_buf_hdr;

typedef void* lu_buf;

static inline void *lu_buf_at(lu_buf self, size_t idx)
{
    lu_buf_hdr *buf = self;
    return self->head
}

#endif /* __LUC_BUFFER_H__ */
