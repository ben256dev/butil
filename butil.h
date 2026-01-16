#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <stdint.h>

#define log(fmt, ...)                                                           \
    do {                                                                        \
        fprintf(stderr, "%s:%d \x1b[1;31merror: %s(): \x1b[0m" fmt "\n",        \
            __FILE__, __LINE__, __func__, ##__VA_ARGS__);                       \
    } while (0)

#define die(fmt, ...)                                                           \
    do {                                                                        \
        fflush(stdout);                                                         \
        log(fmt, ##__VA_ARGS__);                                                \
        exit(EXIT_FAILURE);                                                     \
    } while (0)

// Allocation macros that automatically check return values
static inline void *xmalloc(size_t n);
static inline void *xcalloc(size_t count, size_t size);
static inline void *xrealloc(void *ptr, size_t n);
static inline char *xstrdup(const char *s);

#ifdef BUTIL_IMPLEMENTATION
static inline void *xmalloc(size_t n)
{
    void *p = malloc(n);
    if (!p && n != 0)
        die("malloc(%zu) failed: %s", n, strerror(errno));
    return p;
}

static inline void *xcalloc(size_t count, size_t size)
{
    if (size != 0 && count > SIZE_MAX / size)
        die("calloc overflow: count=%zu size=%zu", count, size);

    void *p = calloc(count, size);
    if (!p && count != 0 && size != 0)
        die("calloc(%zu, %zu) failed: %s", count, size, strerror(errno));
    return p;
}

static inline void *xrealloc(void *ptr, size_t n)
{
    void *p = realloc(ptr, n);
    if (!p && n != 0)
        die("realloc(%p, %zu) failed: %s", ptr, n, strerror(errno));
    return p;
}

static inline char *xstrdup(const char *s)
{
    if (!s)
        die("strdup(NULL)");

    size_t n = strlen(s) + 1;
    char *p = xmalloc(n);
    memcpy(p, s, n);
    return p;
}
#endif

// Forbid Usage of std allocation functions
#ifndef BUTIL_ALLOW_STD_ALLOC
#ifdef malloc
#undef malloc
#endif
#ifdef calloc
#undef calloc
#endif
#ifdef realloc
#undef realloc
#endif
#ifdef strdup
#undef strdup
#endif

#define BUTIL_POISON(msg) \
    (0 * sizeof(struct { _Static_assert(0, msg); int _; }))

#define malloc(...)  BUTIL_POISON("Do not use malloc(); use xmalloc()")
#define calloc(...)  BUTIL_POISON("Do not use calloc(); use xcalloc()")
#define realloc(...) BUTIL_POISON("Do not use realloc(); use xrealloc()")
#define strdup(...)  BUTIL_POISON("Do not use strdup(); use xstrdup()")
#endif

/* Dynamic Array header idea from Dylan Falconer:
   [bytesbeneath](https://www.bytesbeneath.com/p/dynamic-arrays-in-c) */
typedef struct {
	void *(*alloc) (size_t bytes, void *context);
	void *(*free)  (size_t bytes, void *ptr, void *context);
	void *context;
} Allocator;

typedef struct {
	size_t    length;
	size_t    capacity;
	size_t    padding;
 	Allocator *a;
} Array_Header;

#define ARRAY_INITIAL_CAPACITY 16

#define array(T, a) array_init(sizeof(T), ARRAY_INITIAL_CAPACITY, a)
#define array_header(a) ((Array_Header *)(a) - 1)
#define array_length(a) (array_header(a)->length)
#define array_capacity(a) (array_header(a)->capacity)
#define array_remove_unordered(a, i) do { \
    Array_Header *h = array_header(a); \
    if (i == h->length - 1) { \
        h->length -= 1; \
    } else if (h->length > 1) { \
        void *ptr = &a[i]; \
        void *last = &a[h->length - 1]; \
        h->length -= 1; \
        memcpy(ptr, last, sizeof(*a)); \
    } \
} while (0);
#define array_remove_unordered(a, i) do { \
    Array_Header *h = array_header(a); \
    if (i == h->length - 1) { \
        h->length -= 1; \
    } else if (h->length > 1) { \
        void *ptr = &a[i]; \
        void *last = &a[h->length - 1]; \
        h->length -= 1; \
        memcpy(ptr, last, sizeof(*a)); \
    } \
} while (0);
#define array_pop_back(a) (array_header(a)->length -= 1)

#define array_append(a, v) ( \
	(a) = (array_ensure_capacity)(a, 1, sizeof(v)), \
	(a)[array_header(a)->length] = (v), \
	&(a)[array_header(a)->length++])

#define array_foreach(Type, el, array) for (float *it = array, el = *array; it < array + array_length(array) && (el = *it, 1); ++it)

#define array_remove(a, i) do { \
    Array_Header *h = array_header(a); \
    size_t _i = (i); \
    if (h->length == 0 || _i >= h->length) break; \
    if (_i != h->length - 1) { \
        memmove(&(a)[_i], &(a)[_i + 1], (h->length - _i - 1) * sizeof(*(a))); \
    } \
    h->length -= 1; \
} while (0)

void *da_alloc(size_t bytes, void *context);
void *da_free(size_t bytes, void *ptr, void *context);
void *array_init(size_t item_size, size_t capacity, Allocator *a);
void *array_ensure_capacity(void *a, size_t item_count, size_t item_size);

#ifdef BUTIL_IMPLEMENTATION
void *da_alloc(size_t bytes, void *context) {
	(void)context;
	return xmalloc(bytes);
}

void *da_free(size_t bytes, void *ptr, void *context) {
	(void)ptr; (void)context;
	free(ptr);
}

void *array_init(size_t item_size, size_t capacity, Allocator *a) {
	void *ptr       = 0;
	size_t size     = item_size * capacity + sizeof(Array_Header);
	Array_Header *h = a->alloc(size, a->context);

	if (h) {
		h->capacity = capacity;
		h->length   = 0;
		h->a        = a;
		ptr         = h + 1;
	}
}

void *array_ensure_capacity(void *a, size_t item_count, size_t item_size) {
	Array_Header *h = array_header(a);
	size_t desired_capacity = h->length + item_count;

	if (h->capacity < desired_capacity) {
		size_t new_capacity = h->capacity * 2;
		while (new_capacity < desired_capacity) {
			new_capacity *= 2;
		}

		size_t new_size = sizeof(Array_Header) + new_capacity * item_size;
		Array_Header *new_h = h->a->alloc(new_size, h->a->context);

		if (new_h) {
			size_t old_size = sizeof(*h) + h->length * item_size;
			memcpy(new_h, h, old_size);

			if (h->a->free) {
				h->a->free(old_size, h, h->a->context);
			}

			new_h->capacity = new_capacity;
			h = new_h + 1;
		} else {
			h = 0;
		}
	} else { h += 1; }

	return h;
}
#endif
