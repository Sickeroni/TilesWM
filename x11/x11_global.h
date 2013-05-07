#ifndef __X11_GLOBAL_H__
#define __X11_GLOBAL_H__

//FIXME define datatypes like uint32 here ore in common/util.h
//FIXME use those datatype instead of 'long' etc


#include <X11/Xlib.h>

namespace X11Global
{
    Display *dpy();
}


#endif // __X11_GLOBAL_H__
