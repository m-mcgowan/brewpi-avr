#pragma once

typedef int64_t msec_t;

#if defined(__WIN32__)

#include <windows.h>

inline msec_t time_ms(void)
{
    return GetTickCount();
}

#else

#include <sys/time.h>

msec_t time_ms(void)
{
    struct timeval tv;
    gettimeofday(&tv, 0);
    return (msec_t)tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

#endif

msec_t millisSinceStartup();