#ifndef __MONITOR_H__
#define __MONITOR_H__

class Workspace;

class Monitor
{
public:
    Monitor();

    Workspace *workspace() { return _workspace; }
    bool isActive() { return true; }
    void layoutContents();
    void setSize(int w, int h); //HACK

private:
    int _w, _h;
    Workspace *_workspace;
};

#endif // __MONITOR_H__
