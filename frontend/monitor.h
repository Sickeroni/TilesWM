#ifndef __MONITOR_H__
#define __MONITOR_H__

#include <string>

class Workspace;
class StatusBar;

class Monitor
{
public:
    Monitor();
    ~Monitor();

    Workspace *workspace() { return _workspace; }
    bool isActive() { return true; }
    void layoutContents();
    void setSize(int w, int h); //HACK

    void setStatusText(const std::string &text);
    void setHasFocus(bool has_focus);

private:
    int _w = 0;
    int _h = 0;
    Workspace *_workspace = 0;
    StatusBar *_status_bar = 0;
    bool _has_focus = false;
};

#endif // __MONITOR_H__
