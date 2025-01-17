#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

#ifndef NO_BUTIL_TYPES
typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef unsigned long long u64;
typedef signed char        i8;
typedef signed short       i16;
typedef signed int         i32;
typedef signed long long   i64;
typedef float              f32;
typedef double             f64;
typedef long double        f128;
#endif

#define lie(fmt, ...) \
    do { \
        fflush(stdout); \
        fprintf(stderr, fmt "\n", ##__VA_ARGS__); \
        exit(EXIT_SUCCESS); \
    } while (0)

#define cry(fmt, ...) \
    do { \
        fflush(stdout); \
        fprintf(stderr, "%s:%d \x1b[1;31merror: %s(): \x1b[0m" fmt "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
    } while (0)

#define die(fmt, ...) \
    do { \
        fflush(stdout); \
        fprintf(stderr, "%s:%d \x1b[1;31merror: %s(): \x1b[0m" fmt "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
        exit(EXIT_FAILURE); \
    } while (0)

#define pdie(fmt, ...) \
    do { \
        fflush(stdout); \
        fprintf(stderr, "%s:%d \x1b[1;31merror: %s(): " fmt ": \x1b[0m%s\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__, strerror(errno)); \
        exit(EXIT_FAILURE); \
    } while (0)

#ifdef PLIBSYS_HEADER_PLIBSYS_H
#define plibdie(fmt, ...) \
    do { \
        fflush(stdout); \
        fprintf(stderr, fmt "\n", ##__VA_ARGS__); \
        p_libsys_shutdown(); \
        exit(EXIT_FAILURE); \
    } while (0)
#endif

void* xmalloc(size_t size);
__attribute__((nonnull(1))) void* xrealloc(void* ptr, size_t size);
void* xcalloc(size_t num, size_t size);
FILE* xfopen(const char* file_path, const char* mode);
char* xmfopen(const char* file_path);

#ifdef _WIN32 //WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define wdie(fmt, ...) \
    do { \
        fflush(stdout); \
        char err_msg[256]; \
        FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, \
                       NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), \
                       err_msg, sizeof(err_msg), NULL); \
        fprintf(stderr, "%s:%d \x1b[1;31merror: %s(): " fmt ": \x1b[0m%s\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__, err_msg); \
        exit(EXIT_FAILURE); \
    } while (0)
#endif
