#include "widget_util.h"

#include <cassert>

namespace WidgetUtil
{
#if 0
    Edge getNearestEdge(const Rect &rect, int x, int y)
    {
        Edge ret = EDGE_NONE;

        int w = rect.w;
        int h = rect.h;

        int x1 = w / 3;
        int x2 = (w / 3) * 2;
        int y1 = h / 3;
        int y2 = (h / 3) * 2;

        if (y < y1) {
            if (x < x1)
                ret = EDGE_TOP_LEFT;
            else if (x < x2)
                ret = EDGE_TOP;
            else
                ret = EDGE_TOP_RIGHT;
        } else if (y < y2) {
            if (x < x1)
                ret = EDGE_LEFT;
            else if (x < x2)
                ret = EDGE_NONE;
            else
                ret = EDGE_RIGHT;
        } else {
            if (x < x1)
                ret = EDGE_BOTTOM_LEFT;
            else if (x < x2)
                ret = EDGE_BOTTOM;
            else
                ret = EDGE_BOTTOM_RIGHT;
        }

        return ret;
    }
#endif
    WidgetBackend::CursorType resizeCursorForEdge(unsigned int edge)
    {
        switch (edge) {
            case EDGE_NONE:
                return WidgetBackend::CURSOR_DEFAULT;
            case EDGE_LEFT:
                return WidgetBackend::CURSOR_RESIZE_LEFT;
            case EDGE_RIGHT:
                return WidgetBackend::CURSOR_RESIZE_RIGHT;
            case EDGE_TOP:  
                return WidgetBackend::CURSOR_RESIZE_TOP;
            case EDGE_BOTTOM:
                return WidgetBackend::CURSOR_RESIZE_BOTTOM;
            case EDGE_TOP_LEFT:
                return WidgetBackend::CURSOR_RESIZE_TOP_LEFT;
            case EDGE_TOP_RIGHT:
                return WidgetBackend::CURSOR_RESIZE_TOP_RIGHT;
            case EDGE_BOTTOM_LEFT:
                return WidgetBackend::CURSOR_RESIZE_BOTTOM_LEFT;
            case EDGE_BOTTOM_RIGHT:
                return WidgetBackend::CURSOR_RESIZE_BOTTOM_RIGHT;
            default:
                assert(0);
        }
    }
}
