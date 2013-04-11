#ifndef __CONTAINER_ELEMENT_LIST__
#define __CONTAINER_ELEMENT_LIST__

class ContainerElement;

class ContainerElementList
{
public:
    void prepend(ContainerElement *element);
    void append(ContainerElement *element);
//     void insert(ContainerElement *element, ContainerElement *after);
    void replace(ContainerElement *old_element, ContainerElement *new_element);

private:
    ContainerElement *_first;
    ContainerElement *_last;
};


#endif // __CONTAINER_ELEMENT_LIST__
