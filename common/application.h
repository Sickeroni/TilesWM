#ifndef __APPLICATION_H__
#define __APPLICATION_H__

class Workspace;
class ContainerContainer;
class ClientContainer;
class Monitor;

class Application
{
public:
    ~Application();

    virtual Monitor *activeMonitor() = 0;
    virtual Workspace *createWorkspace() = 0;
    virtual ContainerContainer *createContainerContainer() = 0;
    virtual ClientContainer *createClientContainer() = 0;
    virtual void setActiveMonitor(Monitor *monitor) = 0;

    static Application *self() { return _self; }

protected:
    Application();

    static Application *_self;
};

#endif // __APPLICATION_H__
