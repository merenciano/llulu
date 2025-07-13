#ifndef LLULU_LU_LOG_H
#define LLULU_LU_LOG_H

#include "lu_error.h"

#if defined(LU_LOG_DISABLED)
#define LU_INTERNAL_LOG(LL, ...) (void)0
#elif defined(LU_LOG_RESTRICTED)
#define lu_log_verbose(...) (void)0
#define lu_log(...) (void)0
#define lu_log_warn(...) (void)0
#elif !defined(LU_DEBUG)
#define lu_log_verbose(...) (void)0
#endif

#ifndef LU_INTERNAL_LOG
#define LU_INTERNAL_LOG(TAG,...) lu_log_ex(TAG, __FILE__, __LINE__, __VA_ARGS__)
#endif

#ifndef lu_log_verbose 
#define lu_log_verbose(...) LU_INTERNAL_LOG("LU_VERBOSE", __VA_ARGS__)
#endif

#ifndef lu_log 
#define lu_log(...) LU_INTERNAL_LOG("LU_LOG", __VA_ARGS__)
#endif

#ifndef lu_log_warn
#define lu_log_warn(...) LU_INTERNAL_LOG("LU_WARN", __VA_ARGS__)
#endif

#define lu_log_err(...) LU_INTERNAL_LOG("LU_ERR", __VA_ARGS__)
#define lu_log_panic(...) LU_INTERNAL_LOG("LU_PANIC", __VA_ARGS__)

/**
 * Extended log, usually called from log macros.
 * @param tag Log header for emitter identifier and log level.
 * @param file Filename, usually from __FILE__ macro.
 * @param line File line of the log call, usually from __LINE__ macro.
 * @param var_args Text format and values.
 */
lu_err lu_log_ex(const char *tag, const char *file, int line, ...);

#endif /* LLULU_LU_LOG_H */
