#ifndef __X11_GLOBAL_H__
#define __X11_GLOBAL_H__

#include <X11/Xlib.h>


namespace X11Global
{
    typedef unsigned int ModMask;

    struct KeySequence
    {
        KeySequence(KeySym key_sym, ModMask mod_mask) :
            key_sym(key_sym), mod_mask(mod_mask) {}

        bool equals(const KeySequence &other) const {
            return other.key_sym == key_sym && other.mod_mask == mod_mask;
        }

        KeySym key_sym;
        ModMask mod_mask;
    };

    Display *dpy();
}



#endif // __X11_GLOBAL_H__
