#ifndef __X11_GLOBAL_H__
#define __X11_GLOBAL_H__

#include "abstract_key_sequence.h"

#include <X11/Xlib.h>
#include <cassert>

namespace X11Global
{
    typedef unsigned int ModMask;

    struct KeySequence final : public AbstractKeySequence
    {
        KeySequence(KeySym key_sym, ModMask mod_mask) :
            key_sym(key_sym), mod_mask(mod_mask) {}

        bool equals(const KeySequence &other) const {
            return other.key_sym == key_sym && other.mod_mask == mod_mask;
        }

        virtual bool equals(const AbstractKeySequence *other) const override {
            const KeySequence *s = dynamic_cast<const KeySequence*>(other);
            assert(s);
            return s ? equals(*s) : false;
        }

        KeySym key_sym;
        ModMask mod_mask;
    };

    Display *dpy();
}



#endif // __X11_GLOBAL_H__
