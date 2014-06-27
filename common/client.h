#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "rect.h"

#include <map>
#include <string>

class ClientContainer;
class Icon;
class Canvas;
class Workspace;

class Client
{
public:
    virtual ~Client();

    virtual Icon *icon() = 0;
    virtual const Rect &rect() = 0;
    virtual void setRect(const Rect &rect) = 0;
    virtual int maxTextHeight() = 0;
    virtual void requestClose() = 0;
    virtual void raise() = 0;
    virtual void setContainer(ClientContainer *container) = 0;

    ClientContainer *container() { return _container; }
    Workspace *workspace() { return 0; }
    bool hasDecoration() { return !container(); }
    bool isMapped() { return _is_mapped; }
    bool hasFocus() { return _has_focus; }
    const std::string &name() { return _name; }
    const std::string &className() { return _class; }
    const std::string &iconName() { return _icon_name; }
    const std::string &title() { return _title; }
    int minWidth() { return _min_width; }
    int minHeight() { return _min_height; }
    int maxWidth() { return _max_width; }
    int maxHeight() { return _max_height; }


protected:
    Client(bool is_mapped);

    bool _is_mapped;
    bool _has_focus;
    std::string _name;
    std::string _class;
    std::string _icon_name;
    std::string _title;
    int _min_width, _min_height;
    int _max_width, _max_height;
    ClientContainer *_container = 0;
};

#endif // __CLIENT_H__
