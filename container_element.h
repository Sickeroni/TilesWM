#ifndef __CONTAINER_ELEMENT_H__
#define __CONTAINER_ELEMENT_H__


class ContainerElement
{
    friend class ContainerElementList;

public:
    enum Type { CONTAINER, CLIENT };

    ContainerElement *prev() { return _prev; }
    ContainerElement *next() { return _next; }

    Type type() { return _type; }

protected:
    ContainerElement(Type type) : _type(type) {}

private:
    Type _type;
    ContainerElement *_prev, *_next;
};


#endif // __CONTAINER_ELEMENT_H__
