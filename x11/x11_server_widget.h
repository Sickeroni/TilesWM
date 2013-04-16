class X11EventHandler
{
    virtual void HandleEvent(XEvent *event);
};


class X11ServerWidget : public X11Widget
{
    GC _gc;
    X11EventHandler *_eventHandler;
};

