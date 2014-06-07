#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <string>
#include <unordered_map>

class Config
{
public:
    void reload() {}

    static const std::string &value(const std::string &key) {
        return valueFromMap(self()->_values, key);
    }
    static const std::string &defaultValue(const std::string &key) {
        return valueFromMap(_default_values, key);
    }

    static Config *self() { return _self; }
    static void init();

private:
    typedef std::unordered_map<std::string, std::string> Map;

    static const std::string &valueFromMap(const Map &map, const std::string &key);

    Map _values = _default_values;

    static const Map _default_values;
    static Config *_self;
};

#endif
