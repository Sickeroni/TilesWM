#ifndef __X11_KEY_GRAB_SET_H__
#define __X11_KEY_GRAB_SET_H__

#include "key_grab_set.h"
#include "x11_global.h"

#include <vector>

class X11KeyGrabSet final : public KeyGrabSet
{
public:
    virtual int addGrab(std::string sequence) override;
    virtual void clear() override;

    int find(const X11Global::KeySequence &sequence) const;

private:
    std::vector<X11Global::KeySequence> _grabs;
};

#endif
