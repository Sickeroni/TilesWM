#include "common.h"

int main()
{
    unsigned int unsigned_small = 5;
    unsigned int unsigned_big = std::numeric_limits<unsigned int>::max();

    int foo = make_signed<int>(unsigned_small);
    assert(foo == 5);

    try {
        foo = make_signed<int>(unsigned_big);
        assert(false); // should not be reached
    }
    catch (std::out_of_range &e) {}

    return 0;
}
