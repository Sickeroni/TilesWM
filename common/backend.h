#ifndef __BACKEND_H__
#define __BACKEND_H__

#include <string>

class KeyGrabSet;
class WidgetBackend;
class Icon;

class Backend
{
public:
    virtual void requestQuit() = 0;
    virtual WidgetBackend *createWidgetBackend() = 0;
    virtual KeyGrabSet *createKeyGrabSet() = 0;
    virtual Icon *loadImage(std::string filename) = 0;

    //HACK
    virtual void getMonitorSize(int &w, int &h) = 0;
};

#endif
