#ifndef __LLULU_BUFFER_H__
#define __LLULU_BUFFER_H__

#include <stddef.h>

typedef struct lu_buffer {
    char *buf;
    size_t size;
} lu_buffer;

#endif /* __LLULU_BUFFER_H__ */
