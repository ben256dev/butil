#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "bassert.h"
#include "berrno.h"

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

#ifdef __cplusplus
}
#endif