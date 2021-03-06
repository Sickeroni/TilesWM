#ifndef __COMMON_H__
#define __COMMON_H__

#include "debug_stream.h"

#include <cstdint>
#include <iostream>
#include <cassert>
#include <cstdlib>
#include <cstddef>
#include <stdexcept>
#include <type_traits>
#include <limits>

using std::cout;
using std::cerr;
using std::endl;
using std::uint32_t;

enum {
    INVALID_INDEX = size_t(0) - 1
};

namespace IntegerUtil
{
    inline int max(int a, int b) {
        return std::max(a, b);
    }
    inline int min(int a, int b) {
        return std::min(a, b);
    }
    inline void limit(int minval, int &value, int maxval) {
        value = max(minval, value);
        value = min(value, maxval);
    }
    inline void limitMin(int &value, int minval) {
        value = max(minval, value);
    }
    inline void limitMax(int &value, int maxval) {
        value = min(maxval, value);
    }
}

#define STATIC_ASSERT(condition) static_assert(condition, #condition)

template <typename To, typename From>
To make_unsigned(From value) {
    STATIC_ASSERT(std::is_integral<From>::value);
    STATIC_ASSERT(std::is_integral<To>::value);
    STATIC_ASSERT(std::is_signed<From>::value);
    STATIC_ASSERT(std::is_unsigned<To>::value);
    STATIC_ASSERT(sizeof(To) == sizeof(From));

    if (value >= 0)
        return static_cast<To>(value);
    else
        throw std::out_of_range(std::string(__PRETTY_FUNCTION__) + ", arg = " + std::to_string(value));
}

template <typename To, typename From>
To make_signed(From value) {
    STATIC_ASSERT(std::is_integral<From>::value);
    STATIC_ASSERT(std::is_integral<To>::value);
    STATIC_ASSERT(std::is_unsigned<From>::value);
    STATIC_ASSERT(std::is_signed<To>::value);
    STATIC_ASSERT(sizeof(To) == sizeof(From));

    if (value <= static_cast<From>(std::numeric_limits<To>::max()))
        return static_cast<To>(value);
    else
        throw std::out_of_range(std::string(__PRETTY_FUNCTION__) + ", arg = " + std::to_string(value));
}

template <typename To, typename From>
To int_cast(From value) {
    STATIC_ASSERT(std::is_integral<From>::value);
    STATIC_ASSERT(std::is_integral<To>::value);
    STATIC_ASSERT(std::is_signed<To>::value == std::is_signed<From>::value);
    STATIC_ASSERT(sizeof(To) < sizeof(From));

    if (value >= std::numeric_limits<To>::min() && value <= std::numeric_limits<To>::max())
        return static_cast<To>(value);
    else
        throw std::out_of_range(std::string(__PRETTY_FUNCTION__) + ", arg = " + std::to_string(value));
}

// #define UNIMPLEMENTED assert(0);
#define UNIMPLEMENTED cerr<<"UNIMPLEMENTED: "<<__PRETTY_FUNCTION__<<endl;

#endif // __COMMON_H__
