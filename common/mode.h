#ifndef __MODE_H__
#define __MODE_H__

#include <string>

class WindowManager;
class Workspace;

class Mode
{
public:
    Mode(std::string name) : _name(name) {}
    virtual ~Mode() {}

    virtual WindowManager *createWindowManager(Workspace *workspace) = 0;

    const std::string &name() { return _name; }

private:
    std::string _name;
};

#endif
