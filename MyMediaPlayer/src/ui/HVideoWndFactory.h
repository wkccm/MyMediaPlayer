#ifndef HVIDEOWNDFACTORY_H
#define HVIDEOWNDFACTORY_H

#include "hvideownd.h"
#include "sdl2wnd.h"

// 渲染类型
enum renderer_type_e
{
    RENDERER_TYPE_SDL2,
};

#define DEFAULT_RENDER_TYPE RENDERER_TYPE_SDL2

class HVideoWndFactory
{
public:
    static HVideoWnd* create(renderer_type_e type = RENDERER_TYPE_SDL2, QWidget* parent = NULL)
    {
        switch(type)
        {
        case RENDERER_TYPE_SDL2:
            return new SDL2Wnd(parent);
        default:
            return NULL;
        }
    }
};

#endif // HVIDEOWNDFACTORY_H
