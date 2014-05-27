#ifndef __STRING_UTIL_H__
#define __STRING_UTIL_H__

#include <string>
#include <vector>

namespace StringUtil
{
    void tokenize(std::string str, char separator, std::vector<std::string> &tokens);
}

#endif
