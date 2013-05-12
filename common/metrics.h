#ifndef __METRICS_H__
#define __METRICS_H__

namespace Metrics
{

#if 0
// big frames
enum {
    CLIENT_CONTAINER_FRAME = 10,
    CLIENT_INNER_FRAME = 10
};
#endif

#if 1
// small frames
enum {
    CLIENT_CONTAINER_FRAME = 0, //FIXME -> CLIENT_CONTAINER_FRAME_MARGIN
    CLIENT_INNER_FRAME_MARGIN = 0,
    CLIENT_DECORATION_MARGIN = 10,
    CLIENT_TITLEBAR_INNER_MARGIN = 2
};
#endif

}

#endif // __METRICS_H__
