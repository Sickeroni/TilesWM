#ifndef __KEY_GRAB_SET_H__
#define __KEY_GRAB_SET_H__

#include <string>

class KeyGrabSet
{
public:
    // return: the index of the added grab in the set - may be INVALID_INDEX if grabbing is not possible
    // sequence: an opaque backend-specific representation of the key sequence
    virtual int addGrab(std::string sequence) = 0;
    // clear the set and releases the key grabs
    virtual void clear() = 0;
};

#endif
