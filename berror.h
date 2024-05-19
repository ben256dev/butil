#pragma once
#pragma warning(push)
#pragma warning(disable:4996)

#ifdef __cplusplus
extern "C" {
#endif

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bassert.h"
#include "berrno.h"
#include "bstring.h"

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