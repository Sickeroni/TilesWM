#include "string_util.h"

#include <sstream>

using namespace std;

namespace StringUtil
{


void tokenize(string str, char separator, vector<string> &tokens)
{
    stringstream ss(str);
    string s;

    while(getline(ss, s, separator))
        tokens.push_back(s);
}


} // namespace StringUtil
