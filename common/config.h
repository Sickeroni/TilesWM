#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <string>
#include <vector>
#include <unordered_map>

class Config
{
public:
    struct Entry {
        std::string key;
        std::string value;
    };

    class Section {
    public:
        const std::vector<Entry> &entries() const { return _entries; }
        void addEntry(const std::string &key, const std::string &value);

    private:
        std::vector<Entry> _entries;
    };

    ~Config();

    void setDefaults();
    void reload() {}
    const Section *section(const std::string &name);

    static Config *self() { return _self; }
    static void init();
    static void shutdown();

private:

    void clear();
    Section *createSection(const std::string &name);

    std::unordered_map<std::string, Section*> _sections;

    static Config *_self;
};

#endif
