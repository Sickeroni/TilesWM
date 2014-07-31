#ifndef __KEY_GRAB_SET_H__
#define __KEY_GRAB_SET_H__

#include <string>

class KeyGrabSet
{
public:
    virtual ~KeyGrabSet() {}

    // return: the id of the added grab, or INVALID_INDEX if grabbing is not possible
    // key_sequence: an opaque backend-specific representation of the key sequence
    virtual size_t addGrab(std::string key_sequence) = 0;
    // clear the set and releases the key grabs
    virtual void clear() = 0;
};

#endif
