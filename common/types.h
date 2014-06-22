#ifndef __COMMON_TYPES_H__
#define __COMMON_TYPES_H__

typedef unsigned int uint32;

#define ASSERT_SIZE(type, size) static_assert(sizeof(type) == size, "sizeof(" #type ") == " #size)

namespace TypeCheck
{
    ASSERT_SIZE(uint32, 4);
}

#undef ASSERT_SIZE

#endif
