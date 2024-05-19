#pragma once

#include <assert.h>

#ifndef NDEBUG
#define DEBUG_ONLY(...)
#else
#define DEBUG_ONLY(...) do { __VA_ARGS__ } while (0)
#endif