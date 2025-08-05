#include <llulu/lu_error.h>
#include <llulu/lu_time.h>
#include <llulu/lu_hooks.h>
#include <llulu/lu_defs.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

static FILE *err_ostream;

void
lu_err_close_file(void *task_data, void *hook_data)
{
    (void)task_data;
    (void)hook_data;

    if (err_ostream) {
        int ret = fclose(err_ostream);
        lu_err_assert(!ret);
        err_ostream = NULL;
    }
    lu_err_ensures(!err_ostream);
}

void
lu_err_open_file(void)
{
    lu_err_expects(!err_ostream);
    static lu_hook_task close_file_task = {.task_data = NULL, .next = NULL, .run = lu_err_close_file};
    char buf[16];
    lu_time_fmt_date(buf);
    char filename[32];
    sprintf(filename, "lu_errors_%s.txt", buf);
    err_ostream = fopen(filename, "a");
    lu_hook_attach(LU_HOOK_POST_SHUTDOWN, LU_PRIORITY_INTERRUPT, &close_file_task);
    lu_err_ensures(err_ostream);
}

void
lu_err_to_file_ex(int err_code, const char *file, const char *func_name, int line, ...)
{
    if (!err_ostream) {
        lu_err_open_file();
    }

    lu_err_expects(err_ostream);
    char time[16];
    lu_time_fmt_time(time);
    va_list args;
    va_start(args, line);
    const char *fmt = va_arg(args, const char*);
    fprintf(err_ostream, "%s %s(%d) [Error %d in %s]: ", time, file, line, err_code, func_name);
    vfprintf(err_ostream, fmt, args);
    fputc('\n', err_ostream);
    va_end(args);
    lu_hook_notify(LU_HOOK_ERROR, err_ostream);
    lu_err_ensures(sizeof(time) >= 16);
}

void
lu_err_abort(void)
{
    lu_hook_notify(LU_HOOK_ABORT, NULL);
    LU_ABORT_BREAKPOINT();
    abort();
}
