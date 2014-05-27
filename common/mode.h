#ifndef __MODE_H__
#define __MODE_H__

class Workspace;
class ShortcutSet;

class Mode
{
public:
    Mode();
    virtual ~Mode();

    virtual void activate(Workspace *workspace) = 0;
    virtual void initShortcuts() = 0;

    ShortcutSet *shortcuts() { return _shortcuts; }

private:
    ShortcutSet *_shortcuts;
};

#endif
