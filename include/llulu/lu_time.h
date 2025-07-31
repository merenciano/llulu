#ifndef LLULU_TIME_H
#define LLULU_TIME_H

#include <stdint.h>

/** A specific point in time in nanoseconds. */
typedef int64_t lu_timestamp;

/**
 * @brief Gets the current timestamp in nanoseconds.
 * @note The actual value is undefined since it is useless by itself.
 * @return Current timestamp in nanoseconds.
 */
lu_timestamp lu_time_get(void);

/**
 * @brief Gets the duration (in nanoseconds) of the elapsed time between the
 * specified timestamp and the current time.
 * @param time_from Beginning of the time span.
 * @return Nanoseconds elapsed since the timestamp.
 */
static inline int64_t lu_time_elapsed(lu_timestamp time_from) {
    return lu_time_get() - time_from;
}

/** Time unit conversion helpers. */
static inline float lu_time_sec(lu_timestamp ns) { return ns / 1000000000.0f; }
static inline int64_t lu_time_ms(lu_timestamp ns) { return ns / 1000000; }

/**
 * @brief Returns the current time as string, formatted as Hour:Min:Sec.
 * @note The returned string is just the param buffer, for convenience.
 * @param buf Buffer with at least 16 bytes of space.
 * @return Current hours, minutes and seconds in formatted text.
 */
const char *lu_time_fmt_time(char buf[16]);

/**
 * @brief Returns the current date as string, formatted as Year-Month-Day.
 * @note The returned string is just the param buffer, for convenience.
 * @param buf Buffer with at least 16 bytes of space.
 * @return Current year, month and dat in formatted text.
 */
const char *lu_time_fmt_date(char buf[16]);

#endif /* LLULU_TIME_H */
