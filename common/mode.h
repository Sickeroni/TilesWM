#ifndef __MODE_H__
#define __MODE_H__

class Workspace;
class ShortcutSet;

class Mode
{
public:
    virtual void activate(Workspace *workspace) = 0;
    virtual ShortcutSet *shortcuts() = 0;
};

#endif
