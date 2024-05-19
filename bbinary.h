#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "bint.h"
#include "berror.h"

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

#ifdef __cplusplus
}
#endif