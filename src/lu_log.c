#include <llulu/lu_log.h>
#include <llulu/lu_error.h>

#include <stdarg.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

static void *log_ostream;

lu_err
lu_log_open(const char *log_filename)
{
    if (log_ostream && log_ostream != stdout) {
        lu_log_warn("Can not open a new log stream since another one is being used. If the desired behavior is to replace the current one with %s, close it with lu_log_close before attempting to open another.", log_filename);
        return LU_ERR_STREAM_OPEN;
    }

    log_ostream = fopen(log_filename, "w");
    if (!log_ostream) {
        log_ostream = stdout;
        lu_log_warn("fopen file %s failed, using stdout instead.", log_filename);
        return LU_ERR_FALLBACK;
    }

    assert(log_ostream);
    return LU_ERR_SUCCESS;
}

lu_err
lu_log_close(void)
{
    int fclose_ret = 0;
    if (log_ostream && log_ostream != stdout) {
        fclose_ret = fclose(log_ostream);
    }
    log_ostream = NULL;
    return fclose_ret == 0 ? LU_ERR_SUCCESS : LU_ERR_STREAM;
}

lu_err
lu_log_ex(const char *tag, const char *file, int line, ...)
{
    int err = LU_ERR_SUCCESS;
    if (!log_ostream) {
        log_ostream = stdout;
        lu_log_verbose("Log stream is NULL; falling back to stdout.");
        err = LU_ERR_FALLBACK;
    }

    va_list args;
    va_start(args, line);
    const char *fmt = va_arg(args, const char*);
    fprintf(log_ostream, "[%s] %s(%d): ", tag, file, line);
    vfprintf(log_ostream, fmt, args);
    fputc('\n', log_ostream);
    va_end(args);
    return err;
}
