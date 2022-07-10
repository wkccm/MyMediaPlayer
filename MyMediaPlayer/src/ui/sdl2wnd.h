#ifndef SDL2WND_H
#define SDL2WND_H

#include "hvideownd.h"
#include "sdl_util.h"

class SDL2Wnd : public HVideoWnd, QWidget
{
public:
    SDL2Wnd(QWidget* parent = nullptr);
    ~SDL2Wnd();

    // 设置窗口位置
    virtual void setGeometry(const QRect& rc)
    {
        QWidget::setGeometry(rc);
    }
    // 更新
    virtual void update();

protected:
    // 绘画事件
    virtual void paintEvent(QPaintEvent* e);
    // 重新
    virtual void resizeEvent(QResizeEvent* e);
    // 原子操作
    static std::atomic_flag     s_sdl_init;
    // sdl窗体
    SDL_Window*                 sdlWindow;
    // sdl渲染器
    SDL_Renderer*               sdlRenderer;
    // sdl纹理
    SDL_Texture*                sdlTexture;
    // sdl渲染器信息
    SDL_RendererInfo            renderer_info;

    int                         tex_w;
    int                         tex_h;
    int                         tex_pix_fmt;
    int                         tex_bpp;
    int                         tex_pitch;
    // 纹理访问方式
    SDL_PixelFormatEnum         sdl_pix_fmt;
};

#endif // SDL2WND_H
