#include "config.h"

#include "common.h"

using std::string;
using std::unordered_map;
using std::pair;

void Config::Section::addEntry(const std::string &key, const std::string &value)
{
    _entries.push_back( { key, value } );
}

Config *Config::_self = 0;

Config::~Config()
{
    clear();
}

void Config::init()
{
    if (_self)
        abort();
    _self = new Config();

    _self->setDefaults();
}

void Config::shutdown()
{
    delete _self;
    _self = 0;
}

void Config::clear()
{
    for (pair<string, Section*> _mapping : _sections)
        delete _mapping.second;
    _sections.clear();
}

const Config::Section *Config::section(const std::string &name)
{
    std::unordered_map<std::string, Section*>::const_iterator it = _sections.find(name);
    if (it != _sections.end())
        return it->second;
    else
        return 0;

    return 0;
}

Config::Section *Config::createSection(const std::string &name)
{
    assert(!section(name));

    Section *section = new Section();
    _sections.insert(pair<string, Section*>(name, section));

    return section;
}
