/*
* bassert
* bint
* bbinary
* berrno
* bstring
* berror
*/
#pragma once

#ifdef __cplusplus
extern "C" {
#endif
	
#include <assert.h>
#include <vcruntime.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// @bassert
#pragma once

#ifndef NDEBUG
#define DEBUG_ONLY(...)
#else
#define DEBUG_ONLY(...) do { __VA_ARGS__ } while (0)
#endif

// @bint
#pragma warning(push)
#pragma warning(disable: _VCRUNTIME_DISABLED_WARNINGS)

typedef signed char        i8;
typedef short              i16;
typedef int                i32;
typedef long long          i64;
typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef unsigned long long u64;
typedef float              f32;
typedef double             f64;

#pragma warning(pop)

// @bbinary
int bbinary_put_string(char* dst, u8 val)
{
	DEBUG_ONLY({
	if (dst == NULL) { BERRNO = BEARGNULL | BERROR_SEVERITY_LOW;  return -1; }
		});

	for (int i = 0; i < 8; i++)
	{
		dst[i] = val << i & 0x80 ? '1' : '0';
	}
	dst[8] = '\0';

	return 0;
}

// @berrno
u16 BERRNO = 0;

typedef enum : u16 {
	BEFINE = 0,
	BEARGNULL = 1,
	BEMEMNULL = 2,
	BENODIFF = 3,
	BENOMEM = 4,
	BEEXPECTNULL = 5,
	BEBUFFNULL = 6,
	BERANGE = 7,
	BEPARTFAIL = 8,
} BERRNO_CODE;
const char* const BERRNO_NAMES[] = {
	"BEFINE",
	"BEARGNULL",
	"BEMEMNULL",
	"BENODIFF",
	"BENOMEM",
	"BEEXPECTNULL",
	"BEBUFFNULL",
	"BERANGE",
	"BEPARTFAIL",
};
const char* const BERRNO_NAMES_FULL[] = {
	"BEN_ERROR_FINE",
	"BEN_ERROR_ARGUMENT_NULL",
	"BEN_ERROR_MEMBER_NULL",
	"BEN_ERROR_NO_DIFFERENCE",
	"BEN_ERROR_NO_MEMORY",
	"BEN_ERROR_EXPECTED_NULL",
	"BEN_ERROR_BUFFER_NULL",
	"BEN_ERROR_RANGE_NULL",
	"BEN_ERROR_THIRD_PARTY_FAILURE",
};
const char* const BERRNO_MESSAGES[] = {
	"no error",
	"specified function argument was null",
	"specified function argument had an unexpected null member",
	"function exited early because one or more parameters implied that nothing else needed to be done",
	"could not allocate new memory because the program ran out of heap space",
	"function expected parameter to be null, but it wasn't",
	"the pointer for a stack allocated buffer turned out to be null",
	"specified an invalid range",
	"third party function returned a value indicating failure or the functionality otherwise failed"
};
//                            sizeof(BERRNO) * 8 - 3
#define BERRNO_SEVERITY_SHIFT 13
#define BERRNO_SEVERITY       BERRNO >> BERRNO_SEVERITY_SHIFT
#define BERRNO_CODE           BERRNO & 0x1FFF

typedef enum : u16 {
	BERROR_SEVERITY_UNKNOWN = 0 << BERRNO_SEVERITY_SHIFT,
	BERROR_SEVERITY_NONE    = 1 << BERRNO_SEVERITY_SHIFT,
	BERROR_SEVERITY_NOTIFY  = 2 << BERRNO_SEVERITY_SHIFT,
	BERROR_SEVERITY_LOW     = 3 << BERRNO_SEVERITY_SHIFT,
	BERROR_SEVERITY_MEDIUM  = 4 << BERRNO_SEVERITY_SHIFT,
	BERROR_SEVERITY_HIGH    = 5 << BERRNO_SEVERITY_SHIFT,
} BERROR_SEVERITY_LEVEL;
const char* const BERRNO_SEVERITY_MESSAGES[] = {
	"SEVERITY_UNKNOWN",
	"SEVERITY_NONE",
	"SEVERITY_NOTIFY",
	"SEVERITY_LOW",
	"SEVERITY_MEDIUM",
	"SEVERITY_HIGH",
};

// @bstring
typedef char* bstring;

#define BSTRING_SIZE_OFFSET -8
#define BSTRING_LENGTH_OFFSET -4

size_t bstring_size(bstring str)
{
	DEBUG_ONLY({
	if (str == NULL) { BERRNO = BEARGNULL | BERROR_SEVERITY_MEDIUM;  return NULL; }
		});

	return (size_t)(str + BSTRING_SIZE_OFFSET);
}
size_t* bstring_size_ptr(bstring str)
{
	DEBUG_ONLY({
	if (str == NULL) { BERRNO = BEARGNULL | BERROR_SEVERITY_MEDIUM;  return NULL; }
		});

	return (size_t*)(str + BSTRING_SIZE_OFFSET);
}
size_t bstring_length(bstring str)
{
	DEBUG_ONLY({
	if (str == NULL) { BERRNO = BEARGNULL | BERROR_SEVERITY_MEDIUM;  return NULL; }
		});

	return (size_t)(str + BSTRING_LENGTH_OFFSET);
}
size_t* bstring_length_ptr(bstring str)
{
	DEBUG_ONLY({
	if (str == NULL) { BERRNO = BEARGNULL | BERROR_SEVERITY_MEDIUM;  return NULL; }
		});

	return (size_t*)(str + BSTRING_LENGTH_OFFSET);
}
size_t bstring_max_length(bstring str)
{
	DEBUG_ONLY({
	if (str == NULL) { BERRNO = BEARGNULL | BERROR_SEVERITY_MEDIUM;  return NULL; }
		});

	return bstring_size(str) - sizeof(size_t) - sizeof(size_t);
}
bstring bstring_cat(bstring str, const char* buffer_ptr, size_t bytes)
{
	DEBUG_ONLY({
	if (str == NULL) { BERRNO = BEARGNULL | BERROR_SEVERITY_LOW;  return NULL; }
	if (buffer_ptr == NULL) { BERRNO = BEARGNULL | BERROR_SEVERITY_MEDIUM; return NULL; }
	if (bytes == 0) { BERRNO = BENODIFF | BERROR_SEVERITY_NOTIFY; return NULL; }
		});

	if (str == NULL)
	{
		const size_t alloc_size_bytes = sizeof(size_t) + sizeof(size_t) + bytes * 2;
		bstring new_str = (bstring)malloc(alloc_size_bytes);
		DEBUG_ONLY({ if (new_str == NULL) { BERRNO = BENOMEM | BERROR_SEVERITY_HIGH; return NULL; } });
		new_str += sizeof(size_t) + sizeof(size_t);
		*bstring_size_ptr(new_str) = alloc_size_bytes;
		*bstring_length_ptr(new_str) = bytes;
		memcpy(new_str, buffer_ptr, bytes);
		return new_str;
	}

	*bstring_length_ptr(str) += bytes;
	if (bstring_length(str) > bstring_max_length(str))
	{
		const size_t alloc_size_bytes = sizeof(size_t) + sizeof(size_t) + bstring_length(str);
		bstring new_str = (bstring)realloc(str, alloc_size_bytes);
		DEBUG_ONLY({ if (new_str == NULL) { BERRNO = BENOMEM | BERROR_SEVERITY_HIGH; return NULL; } });
		new_str += sizeof(size_t) + sizeof(size_t);
		*bstring_size_ptr(new_str) = alloc_size_bytes;
		*bstring_length_ptr(new_str) = bytes;
		return new_str;
	}

	memcpy(str + bstring_length(str) - bytes, buffer_ptr, bytes);
	return str;
}
void bstring_delete(bstring str)
{
	DEBUG_ONLY({ if (str == NULL) { BERRNO = BEARGNULL | BERROR_SEVERITY_LOW;  return NULL; } });

	free(str - sizeof(size_t) - sizeof(size_t));
}

// @berror
#pragma warning(push)
#pragma warning(disable:4996)

bstring berror_buffer;

#define BERROR_MESSAGE_SIZE 1024
char* berror_handle_rv(int rv, const char* message)
{
	static char berror_message_buffer[BERROR_MESSAGE_SIZE];
	if (message == NULL)
		return berror_message_buffer;

	if (rv < 0)
	{
		if (errno != 0)
		{
			int n = snprintf(berror_message_buffer, BERROR_MESSAGE_SIZE, "ERRNO:%s:%s\n",
				strerror(errno), message);
			bstring_cat(berror_buffer, berror_message_buffer, n > -1 ? n : BERROR_MESSAGE_SIZE);
			errno = 0;
		}
		if (BERRNO != BEFINE)
		{
			int n = snprintf(berror_message_buffer, BERROR_MESSAGE_SIZE, "BERRNO:%s:%s:%s:%s\n",
				BERRNO_SEVERITY_MESSAGES[BERRNO_SEVERITY], BERRNO_NAMES_FULL[BERRNO_CODE], BERRNO_MESSAGES[BERRNO_CODE], message);
			bstring_cat(berror_buffer, berror_message_buffer, n > -1 ? n : BERROR_MESSAGE_SIZE);
			BERRNO = 0;
		}
	}
	return NULL;
}
void berror_handle_rv_ptr(void* rv_ptr, const char* message)
{
	char* berror_message_ptr = berror_handle_rv(0, 0);
	assert(rv_ptr != NULL);
	assert(berror_message_ptr != NULL);

	if (rv_ptr == nullptr)
	{
		if (errno != 0)
		{
			int n = snprintf(berror_message_ptr, BERROR_MESSAGE_SIZE, "ERRNO:%s:%s\n",
				strerror(errno), message);
			bstring_cat(berror_buffer, berror_message_ptr, n > -1 ? n : BERROR_MESSAGE_SIZE);
			errno = 0;
		}
		if (BERRNO != BEFINE)
		{
			int n = snprintf(berror_message_ptr, BERROR_MESSAGE_SIZE, "BERRNO:%s:%s:%s:%s\n",
				BERRNO_SEVERITY_MESSAGES[BERRNO_SEVERITY], BERRNO_NAMES_FULL[BERRNO_CODE], BERRNO_MESSAGES[BERRNO_CODE], message);
			bstring_cat(berror_buffer, berror_message_ptr, n > -1 ? n : BERROR_MESSAGE_SIZE);
			BERRNO = 0;
		}
	}
}
void berror_init(void)
{
	int error = 0;
	DEBUG_ONLY({ if (berror_buffer != NULL) { BERRNO = BEEXPECTNULL | BERROR_SEVERITY_LOW; error = -1; } });
	#define PRIMARY_MESSAGE "-BERROR-\n"
	bstring_cat(berror_buffer, PRIMARY_MESSAGE, sizeof(PRIMARY_MESSAGE) - 1);
	berror_handle_rv(error, "berror_init possibly called twice");
}
void b_error_deinit(void)
{
	free(berror_buffer);
}
#pragma warning(pop)

#ifdef __cplusplus
}
#endif
