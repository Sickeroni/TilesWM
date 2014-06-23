#include "config.h"

#include "common.h"

Config *Config::_self = 0;

void Config::init()
{
    if (_self)
        abort();
    _self = new Config();
}

void Config::shutdown()
{
    delete _self;
    _self = 0;
    _default_values.clear();
}

const std::string &Config::valueFromMap(const Map &map, const std::string &key)
{
    static const std::string empty;

    Map::const_iterator it = map.find(key);
    if (it != map.end())
        return it->second;
    else
        return empty;
}
