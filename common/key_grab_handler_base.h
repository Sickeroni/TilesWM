#ifndef __KEY_GRAB_HANDLER_BASE__
#define __KEY_GRAB_HANDLER_BASE__

class KeyGrabSet;

class KeyGrabHandlerBase
{
public:
    virtual const KeyGrabSet *grabs() = 0;
    virtual void handleKeyGrabPressed(int grab_id) = 0;
};

#endif
