#ifndef __CONTAINER_H__
#define __CONTAINER_H__

#include <list>

class Client;

class Container
{
public:
    enum Orientation {
        HORIZONTAL,
        VERTICAL
    };

    static void startup(int screen_width, int screen_height);
    static void shutdown();
    static Container *root() {
        return _root;
    }

    void addClient(Client *c);
    void removeClient(Client *c);
    void layout();

    int x() { return _x; }
    int y() { return _y; }
    int width() { return _w; }
    int height() { return _h; }

private:
    Container(Container *parent, int x, int y, int w, int h);
    void local_to_global(int &x, int &y);
    void moveClientRight();
    Container *east();
    Container *south();

    static Container *_root;

    Container *_parent;
    std::list<Client*> _clients;
    Orientation _orientation;
    int _x, _y, _w, _h;
};

#endif // __CONTAINER_H__
