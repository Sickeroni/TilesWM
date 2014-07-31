#ifndef __CONTAINER_CONTAINER_H__
#define __CONTAINER_CONTAINER_H__

#include "container.h"
#include "common.h"

#include <vector>

class ClientContainer;
class Canvas;
class ContainerContainerLayout;

class ContainerContainer final : public Container
{
public:
    enum MinimizeMode {
        MINIMIZE_NONE,
        MINIMIZE_ALL,
        MINIMIZE_INACTIVE
    };

    ContainerContainer();
    virtual ~ContainerContainer();

    virtual void draw(Canvas *canvas) override;

    virtual Index numElements() const override { return static_cast<Index>(_children.size()); }
    virtual ClientContainer *activeClientContainer() override;
    virtual bool isEmpty() const override { return _children.empty(); }
    virtual void redrawAll() override;
    virtual ContainerLayout *getLayout() override;
    virtual bool isMinimized() const {
        return _minimize_mode == MINIMIZE_ALL;
    }
    virtual void setOrientation(Orientation orientation) override;
    virtual void setHasFocus(bool has_focus) override;

    Container *child(Index index) { return _children[index]; }
    Index activeChildIndex() { return _active_child_index; }
    void setActiveChild(Index index);
    Index indexOfChild(const Container *child);
    // ret: index of added child
    Index addChild(Container *child);
    // inserts child at insert_pos
    void insertChild(Container *child, Index insert_pos);
    // de-parents and returns the child at index
    Container *replaceChild(Index index, Container *new_child);
    Container *removeChild(Index index);

//     void setDirty(bool set);

    void handleSizeHintsChanged(Container *child);

    Container *activeChild() {
        return activeChildIndex() >= 0 ? child(activeChildIndex()) : 0;
    }

    void setMinimizeMode(MinimizeMode mode);
    MinimizeMode minimizeMode() { return _minimize_mode; }

private:
    void updateDirtyStatus();
    void clear();
    void applyMinimizeMode(Container *child);

//     bool _dirty; // is this container unused or are there unused child containers ?

    ContainerContainerLayout *_layout = 0;
    MinimizeMode _minimize_mode = MINIMIZE_NONE;
    std::vector<Container*> _children;
    Index _active_child_index = INVALID_INDEX;
};


#endif // __CONTAINER_CONTAINER_H__
