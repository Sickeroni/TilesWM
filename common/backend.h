#ifndef __BACKEND_H__
#define __BACKEND_H__

#include <string>

class KeyGrabSet;
class WidgetBackend;
class Icon;
struct AbstractKeySequence;

class Backend
{
public:
    virtual void requestQuit() = 0;
    virtual WidgetBackend *createWidgetBackend() = 0;
    virtual Icon *loadImage(std::string filename) = 0;

    // parse a backend-specific representation af a key sequence; the returned object is owned by caller
    virtual AbstractKeySequence *parseKeySequence(std::string sequence) = 0;
    virtual bool addKeyGrab(const AbstractKeySequence *key_sequence) = 0;
    virtual void releaseKeyGrab(const AbstractKeySequence *key_sequence) = 0;

    //HACK
    virtual void getMonitorSize(int &w, int &h) = 0;
};

#endif
