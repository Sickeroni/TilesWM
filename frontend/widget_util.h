#ifndef __WIDGET_UTIL_H__
#define __WIDGET_UTIL_H__

#include "rect.h"
#include "widget_backend.h"

namespace WidgetUtil
{
    enum Edge {
        EDGE_NONE = 0,
        EDGE_LEFT = 1,
        EDGE_RIGHT = 2,
        EDGE_TOP = 4,
        EDGE_BOTTOM = 8,
        EDGE_TOP_LEFT = EDGE_TOP | EDGE_LEFT,
        EDGE_TOP_RIGHT = EDGE_TOP | EDGE_RIGHT,
        EDGE_BOTTOM_LEFT = EDGE_BOTTOM | EDGE_LEFT,
        EDGE_BOTTOM_RIGHT = EDGE_BOTTOM | EDGE_RIGHT
    };

//     Edge getNearestEdge(const Rect &rect, int x, int y);

    WidgetBackend::CursorType resizeCursorForEdge(unsigned int edge);

}

#endif
