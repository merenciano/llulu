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
#define LU_INTERNAL_LOG(TAG,...) lu_log_ex(TAG, __func__, __FILE__, __LINE__, __VA_ARGS__)
#endif

#ifndef lu_log_verbose 
#define lu_log_verbose(...) LU_INTERNAL_LOG(LU_LOG_VERBOSE, __VA_ARGS__)
#endif

#ifndef lu_log 
#define lu_log(...) LU_INTERNAL_LOG(LU_LOG_LOG, __VA_ARGS__)
#endif

#ifndef lu_log_warn
#define lu_log_warn(...) LU_INTERNAL_LOG(LU_LOG_WARNING, __VA_ARGS__)
#endif

#define lu_log_err(...) LU_INTERNAL_LOG(LU_LOG_ERROR, __VA_ARGS__)
#define lu_log_panic(...) LU_INTERNAL_LOG(LU_LOG_PANIC, __VA_ARGS__)

enum lu_log_config_flags {
    LU_LOG_COLOR   = 0x0001,
    LU_LOG_DATE    = 0x0002,
    LU_LOG_TIME    = 0x0004, /* HH:MM */
    LU_LOG_SECONDS = 0x0008, /* HH:MM:SS */
    LU_LOG_FILE    = 0x0010,
    LU_LOG_FUNC    = 0x0020,
};

enum lu_log_levels {
    LU_LOG_PANIC = 0,
    LU_LOG_ERROR,
    LU_LOG_WARNING,
    LU_LOG_LOG,
    LU_LOG_VERBOSE,
    LU_LOG_CUSTOM, /* User-defined tag */
    LU_LOG_LEVELS
};

/**
 * @brief Logger configuration. Enable or disable the desired options.
 * @param flags One or more flags from the enum @see @enum lu_log_config_flags. 
 * @param enable Choose between enable or disable the options specified in the flags.
 */
void lu_log_setopt(int flags, bool enable);
void lu_log_set_custom_tag(const char *tag);

lu_err lu_log_open(const char *log_filename);
lu_err lu_log_close(void);

/**
 * Extended log, usually called from log macros.
 * @param log_level Use the enum values @see @enum lu_log_levels.
 * @param func Function name, usually from __func__ macro.
 * @param file Filename, usually from __FILE__ macro.
 * @param line File line of the log call, usually from __LINE__ macro.
 * @param var_args Text format and values.
 */
lu_err lu_log_ex(int log_level, const char *func, const char *file, int line, ...);

#endif /* LLULU_LU_LOG_H */
