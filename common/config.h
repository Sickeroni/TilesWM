#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <string>
#include <unordered_map>

class Config
{
public:
    static const std::string &value(const std::string &key);
    static void setValue(const std::string &key, const std::string &value);

    static Config *self() { return _self; }
    static void init();

private:
    typedef std::unordered_map<std::string, std::string> Map;
    Map _values;

    static Config *_self;
};

#endif
