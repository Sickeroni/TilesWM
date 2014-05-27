#include "config.h"

#include "common.h"

Config *Config::_self = 0;

void Config::init()
{
    if (_self)
        abort();
    _self = new Config();
}

const std::string &Config::value(const std::string &key)
{
    static const std::string empty;

    Map::iterator it = self()->_values.find(key);
    if (it != self()->_values.end())
        return it->second;
    else
        return empty;
}

void Config::setValue(const std::string &key, const std::string &value)
{
    self()->_values[key] = value;
}
