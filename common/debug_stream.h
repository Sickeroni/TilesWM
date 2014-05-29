#ifndef __DEBUG_STREAM_H__
#define __DEBUG_STREAM_H__

#include "types.h"

#include <iostream>


class DebugStream
{
public:
    DebugStream() : num_items(0) {}
    DebugStream(const char *context) : num_items(0) {
        std::cerr << context << ": ";
    }
    DebugStream(int line, const char *context) : num_items(0) {
        std::cerr << context << " - " << line << ": ";
    }
    ~DebugStream() {
        std::cerr << std::endl;
    }

    template <typename t>
    DebugStream &operator<<(t param) {
        if (num_items)
            std::cerr << ' ';
        std::cerr << param;
        num_items++;
        return *this;
    }

private:
    int num_items;
};


// #define debug (DebugStream(__LINE__, __PRETTY_FUNCTION__))
#define debug (DebugStream(__PRETTY_FUNCTION__))
#define printvar(var) { debug << #var << "=" << var; }


#endif // __DEBUG_STREAM_H__
