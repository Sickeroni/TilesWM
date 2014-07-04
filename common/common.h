#ifndef __COMMON_H__
#define __COMMON_H__

#include "debug_stream.h"

#include <cstdint>
#include <iostream>
#include <cassert>
#include <cstdlib>

using std::cout;
using std::cerr;
using std::endl;
using std::uint32_t;

enum {
    INVALID_INDEX = -1
};

// #define UNIMPLEMENTED assert(0);
#define UNIMPLEMENTED cerr<<"UNIMPLEMENTED: "<<__PRETTY_FUNCTION__<<endl;

#endif // __COMMON_H__
