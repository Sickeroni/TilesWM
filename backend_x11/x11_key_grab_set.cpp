#include "x11_key_grab_set.h"

#include "x11_application.h"
#include "string_util.h"
#include "common.h"

int X11KeyGrabSet::addGrab(std::string key_sequence_str)
{
    std::vector<std::string> tokens;
    StringUtil::tokenize(key_sequence_str, '+', tokens);

    X11Global::KeySequence key_sequence(NoSymbol, 0);

    for (size_t i = 0; i < tokens.size(); i++) {
        std::string &s = tokens[i];

        if (s.compare("CTRL") == 0)
            key_sequence.mod_mask |= ControlMask;
        else if (s.compare("MOD1") == 0)
            key_sequence.mod_mask |= Mod1Mask;
        else if (s.compare("SHIFT") == 0)
            key_sequence.mod_mask |= ShiftMask;
        else {
            key_sequence.key_sym = XStringToKeysym(s.c_str());
            if (key_sequence.key_sym == NoSymbol) {
                debug<<"invalid key symbol"<<s<<"in key sequence"<<key_sequence_str;
                return INVALID_INDEX;
            }
            break;
        }
    }

    if (key_sequence.key_sym == NoSymbol) {
        debug<<"missing key symbol in key sequence"<<key_sequence_str;
        return INVALID_INDEX;
    }
    else if (X11Application::self()->addKeyGrab(key_sequence)) {
        _grabs.push_back(key_sequence);
        return _grabs.size() - 1;
    } else
        return INVALID_INDEX;

}

void X11KeyGrabSet::clear()
{
    for (const X11Global::KeySequence &grab : _grabs)
        X11Application::self()->releaseKeyGrab(grab);
    _grabs.clear();
}

int X11KeyGrabSet::find(const X11Global::KeySequence &key_sequence) const
{
    for (size_t i = 0; i < _grabs.size(); i++) {
        if (_grabs[i].equals(key_sequence))
            return i;
    }
    return INVALID_INDEX;
}
