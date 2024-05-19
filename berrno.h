#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "bint.h"

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

#ifdef __cplusplus
}
#endif