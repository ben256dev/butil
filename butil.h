#pragma once

#define die(fmt, ...) \
    do { \
        fflush(stdout); \
        fprintf(stderr, "%s:%d \x1b[1;31merror: %s()\x1b[0m " fmt "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
        exit(-1); \
    } while (0)

#define pdie(fmt, ...) \
    do { \
        fflush(stdout); \
        fprintf(stderr, "%s:%d \x1b[1;31merror: %s(): " fmt ": \x1b[0m%s\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__, strerror(errno)); \
        exit(EXIT_FAILURE); \
    } while (0)

void* xmalloc(size_t size)
{
   void* ptr = malloc(size);
   if (ptr == NULL)
      pdie("malloc(%zu)", size);
   return ptr;
}
