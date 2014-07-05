#include "string_util.h"

#include <sstream>

using namespace std;

namespace StringUtil
{


void tokenize(string str, char separator, vector<string> &tokens, bool include_empty_tokens)
{
    stringstream ss(str);
    string s;

    while(getline(ss, s, separator)) {
        if (!s.empty() || include_empty_tokens)
            tokens.push_back(s);
    }
}


} // namespace StringUtil
