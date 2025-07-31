#include <llulu/lu_time.h>
#include <time.h>

#if defined(_POSIX_TIMERS) && _POSIX_TIMERS > 0L
/* Best option if available. */
#if defined(_POSIX_MONOTONIC_CLOCK) && _POSIX_MONOTONIC_CLOCK > 0L
#define CLOCK_TYPE CLOCK_MONOTONIC
#else
#define CLOCK_TYPE CLOCK_REALTIME
#endif

lu_timestamp
lu_time_get(void)
{
    struct timespec time;
    clock_gettime(CLOCK_TYPE, &time);
    return time.tv_nsec + time.tv_sec * 1e9;
}

#elif defined(_WIN32)
/* Windows' monotonic clock equivalent. */
#include <windows.h>

lu_timestamp
lu_time_get(void)
{
    LARGE_INTEGER freq, ticks;
    if (!QueryPerformanceFrequency(&freq))
        return 0;

    if (!QueryPerformanceCounter(&ticks))
        return 0;

    int64_t sec = ticks.QuadPart / freq.QuadPart;
    int64_t nsec = (int) (((ticks.QuadPart % freq.QuadPart) * 1e9 + (freq.QuadPart >> 1)) / freq.QuadPart);
    return nsec + sec * 1e9;
}

#elif __STDC_VERSION__ >= 201112L
/* C11 std. Always CLOCK_RUNTIME but better than nothing. */
lu_timestamp
lu_time_get(void)
{
    struct timespec time;
    timespec_get(&time, TIME_UTC);
    return time.tv_nsec + time.tv_sec * 1e9;
}

#else
/* Backup, microseconds precision at best. */
lu_timestamp
lu_time_get(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_usec * 1000 + tv.tv_sec * 1e9;
}

#endif

const char *lu_time_fmt_time(char buf[16])
{
    time_t t = time(NULL);
    struct tm *time = localtime(&t);
    buf[strftime(buf, 16, "%H:%M:%S", time)] = '\0';
    return buf;
}

const char *lu_time_fmt_date(char buf[16])
{
    time_t t = time(NULL);
    struct tm *time = localtime(&t);
    buf[strftime(buf, 16, "%Y-%m-%d", time)] = '\0';
    return buf;
}
