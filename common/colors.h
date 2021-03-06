 #ifndef __COLORS_H__
#define __COLORS_H__

namespace Colors
{

#if 0
enum {
    CONTAINER = 0x000000,
    CONTAINER_BORDER = 0X666666,
    CONTAINER_FOCUS = 0xFFBB00,

    CLIENT_CONTAINER = CONTAINER,
    CLIENT_CONTAINER_BORDER = 0X666666,
    CLIENT_CONTAINER_FOCUS = 0xFF5555,

    CLIENT = 0x0,
    CLIENT_BORDER = 0x0,
    CLIENT_FOCUS = 0xCF7A7A,

    TAB = 0x2E2E2E,
    TAB_TEXT = 0x9E9E9E,
    TAB_FOCUSED = 0x116029,
    TAB_FOCUSED_TEXT = 0xFFFFFF,
    TAB_CURRENT = 0x000000,
    TAB_CURRENT_TEXT = 0xC2C2C2,
    TAB_ACTIVE = TAB_CURRENT,
    TAB_ACTIVE_TEXT = TAB_CURRENT_TEXT
};
#endif

#if 1
enum {
    CONTAINER = 0x000000,
    CONTAINER_BORDER = 0X666666,
    CONTAINER_FOCUS = 0xFFBB00,

    CLIENT_CONTAINER = CONTAINER,
    CLIENT_CONTAINER_BORDER = 0X666666,
    CLIENT_CONTAINER_FOCUS = 0xFFBB00,

    CLIENT = 0x0,
    CLIENT_BORDER = 0X666666,
    CLIENT_FOCUS = 0xFFBB00,

    TAB = 0x26282B,
    TAB_TEXT = 0x9E9E9E,
    TAB_BORDER = 0x464A4F,
    TAB_FOCUSED = 0x2A3763,
    TAB_FOCUSED_TEXT = 0xFFFFFF,
    TAB_FOCUSED_BORDER = 0x5D6FA8,
    TAB_CURRENT = 0x000000,
    TAB_CURRENT_TEXT = 0xC2C2C2,
};
#endif


}

#endif // __COLORS_H__
