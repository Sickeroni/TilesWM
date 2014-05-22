#include "monitor.h"

#include "application.h"
#include "workspace.h"
#include "rect.h"

Monitor::Monitor() :
    _w(0),
    _h(0),
    _workspace(Application::self()->createWorkspace())
{
    _workspace->setMonitor(this);
}

void Monitor::setSize(int w, int h)
{
    _w = w;
    _h = h;
    layoutContents();
}

void Monitor::layoutContents()
{
    _workspace->setRect(Rect(0, 0, _w, _h));
}
