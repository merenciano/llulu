#ifndef LLULU_LU_ERROR_H
#define LLULU_LU_ERROR_H

#include <stdbool.h>

typedef int lu_err;

enum {
    LU_ERR_SUCCESS = 0, /* This "error" denotes the absence of an error. */
    LU_ERR_ERROR = -1, /* Error not specified. */
    LU_ERR_FALLBACK = -100, /* Due to an error, the system has resorted to a fallback option to avoid failure. */
    LU_ERR_STREAM = -110, /* Generic stream error, usually an invalid stream identifier. */
    LU_ERR_STREAM_OPEN = -111, /* Could not open the stream, e.g. fopen failed. */
    LU_ERR_FILE = -112, /* File related errors. */
    LU_ERR_BADARG = -120, /* Invalid argument value, e.g. NULL pointer or size == 0. */
    LU_ERR_MEM = -130, /* Memory-related errors. */
    LU_ERR_MEM_BUF = -131, /* Invalid buffer, usually not big enough. */
};

void lu_err_to_file_ex(int err_code, const char *file, const char *func_name, int line, ...);
#define lu_err_to_file(ERR,...) lu_err_to_file_ex(ERR, DESCRIPTION, __FILE__, __func__, __LINE__, __VA_ARGS__)

void lu_err_abort(void);

static inline void
lu_err_assert(bool condition)
{
#ifndef LU_DISABLE_ASSERTS
    if (!condition) {
        lu_err_abort();
    }
#else
    (void)condition;
#endif
}

static inline void
lu_err_expects(bool condition)
{
#ifndef LU_DISABLE_CONTRACTS
    if (!condition) {
        lu_err_abort();
    }
#else
    (void)condition;
#endif
}

static inline void
lu_err_ensures(bool condition)
{
#ifndef LU_DISABLE_CONTRACTS
    if (!condition) {
        lu_err_abort();
    }
#else
    (void)condition;
#endif
}

#endif /* LLULU_LU_ERROR_H */
