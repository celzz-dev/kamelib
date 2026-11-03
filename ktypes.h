#pragma once

#include <stdbool.h>

#if defined(__x86_64__) || defined(_M_X64) || defined(__aarch64__)
    typedef unsigned long long usize;
    typedef long long ssize;
#else
    typedef unsigned int usize;
    typedef int ssize;
#endif


