#include <llulu/lu_time.h>
#include <llulu/lu_log.h>
#include <llulu/lu_error.h>

#include <stdarg.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define LU_LOG_RSTCOLOR      "\e[0m"
#define LU_LOG_BLACK         "\e[30m"
#define LU_LOG_GRAY          "\e[1;30m"
#define LU_LOG_RED           "\e[31m"
#define LU_LOG_RED_BRIGHT    "\e[1:51:97:101m"
#define LU_LOG_GREEN         "\e[32m"
#define LU_LOG_YELLOW        "\e[33m"
#define LU_LOG_BLUE          "\e[94m"

struct {
    void *ostream;
    uint32_t flags;
    char custom_tag[28];
} static log_ctx;

inline void
lu_log_setopt(int flags, bool enable)
{
    if (enable) {
        log_ctx.flags |= flags;
    } else {
        log_ctx.flags &= ~flags;
    }
}

static inline const char *
log_get_loglvl_color(int tag)
{
    switch(tag) {
        case LU_LOG_PANIC:   return LU_LOG_RED_BRIGHT;
        case LU_LOG_ERROR:   return LU_LOG_RED;
        case LU_LOG_WARNING: return LU_LOG_YELLOW;
        case LU_LOG_LOG:     return LU_LOG_RSTCOLOR;
        case LU_LOG_VERBOSE: return LU_LOG_GRAY;
        case LU_LOG_CUSTOM: return LU_LOG_BLUE;
        default: assert(false); return LU_LOG_RSTCOLOR;
    }
}

static inline const char *
log_get_loglvl_name(int tag)
{
    switch(tag) {
        case LU_LOG_PANIC:   return "LU_PANIC";
        case LU_LOG_ERROR:   return "LU_ERROR";
        case LU_LOG_WARNING: return "LU_WARN";
        case LU_LOG_LOG:     return "LU_LOG";
        case LU_LOG_VERBOSE: return "LU_VERBOSE";
        case LU_LOG_CUSTOM:  return log_ctx.custom_tag;
        default: assert(false); return "";
    }
}

void lu_log_set_custom_tag(const char *tag)
{
    strncpy(log_ctx.custom_tag, tag, sizeof(log_ctx.custom_tag) - 1);
}

lu_err
lu_log_open(const char *log_filename)
{
    if (log_ctx.ostream && log_ctx.ostream != stdout) {
        lu_log_warn("Can not open a new log stream since another one is being used. If the desired behavior is to replace the current one with %s, close it with lu_log_close before attempting to open another.", log_filename);
        return LU_ERR_STREAM_OPEN;
    }

    log_ctx.ostream = fopen(log_filename, "w");
    if (!log_ctx.ostream) {
        log_ctx.ostream = stdout;
        lu_log_warn("fopen file %s failed, using stdout instead.", log_filename);
        return LU_ERR_FALLBACK;
    }

    assert(log_ctx.ostream);
    return LU_ERR_SUCCESS;
}

lu_err
lu_log_close(void)
{
    int fclose_ret = 0;
    if (log_ctx.ostream && log_ctx.ostream != stdout) {
        fclose_ret = fclose(log_ctx.ostream);
    }
    log_ctx.ostream = NULL;
    return fclose_ret == 0 ? LU_ERR_SUCCESS : LU_ERR_STREAM;
}

lu_err
lu_log_ex(int loglvl, const char *func, const char *file, int line, ...)
{
    char date_buf[16] = {0};
    char time_buf[17] = {0};
    char source_buf[1024] = {0};
    int err = LU_ERR_SUCCESS;
    if (!log_ctx.ostream) {
        log_ctx.ostream = stdout;
        lu_log_verbose("Log stream is NULL; falling back to stdout.");
        err = LU_ERR_FALLBACK;
    }

    bool color = log_ctx.flags & LU_LOG_COLOR;
    bool date = log_ctx.flags & LU_LOG_DATE;
    int time = log_ctx.flags & (LU_LOG_TIME | LU_LOG_SECONDS);
    int source = log_ctx.flags & (LU_LOG_FUNC | LU_LOG_FILE);

    if (time & LU_LOG_TIME) {
        time_buf[0] = ' ';
        lu_time_fmt_time(time_buf + 1);
        if (time == LU_LOG_TIME) { 
            /* LU_LOG_SECONDS IS DISABLED */
            char *it = &time_buf[4]; /* Avoiding the H:M separator so next is seconds */
            while (*it) {
                if (*it == ':') {
                    *it = '\0';
                } else {
                    it++;
                }
            }
        }
    }

    if (source) {
        if (source & LU_LOG_FUNC) {
            char *it = source_buf;
            *it++ = ' ';
            uint64_t len = strlen(func);
            memcpy(it, func, len);
            it += len;
            if (source & LU_LOG_FILE) {
                *it++ = '@';
                len = strlen(file);
                memcpy(it, file, len);
                it += len;
            }
            *it++ = '(';
            memset(it, 0, 8);
            sprintf(it, "%d", line);
            for (;*it;++it);
            *it++ = ')';
            *it++ = ':';
            *it++ = ' ';
        }
    }

    va_list args;
    va_start(args, line);
    const char *fmt = va_arg(args, const char*);
    fprintf(log_ctx.ostream, "%s[%s]%s %s%s%s",
        color ? log_get_loglvl_color(loglvl) : "",
        log_get_loglvl_name(loglvl),
        color ? LU_LOG_RSTCOLOR : "",
        date ? lu_time_fmt_date(date_buf) : "",
        time_buf,
        source_buf);
    vfprintf(log_ctx.ostream, fmt, args);
    fputc('\n', log_ctx.ostream);
    va_end(args);
    return err;
}
