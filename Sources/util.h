#pragma once

#include <assert.h>
#define SDL_ASSERT_SUCCESS(v, x) \
{ \
    int retval = x; \
    assert(v==retval); \
}
