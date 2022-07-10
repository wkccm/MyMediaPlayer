#include "sdl2wnd.h"
#include "avdef.h"
#include "hlog.h"

std::atomic_flag SDL2Wnd::s_sdl_init = ATOMIC_FLAG_INIT;

// 颜色编码类型
static SDL_PixelFormatEnum SDL_pix_fmt(int type)
{
    switch(type)
    {
    case PIX_FMT_IYUV:
        return SDL_PIXELFORMAT_IYUV;
    case PIX_FMT_YV12:
        return SDL_PIXELFORMAT_YV12;
    case PIX_FMT_NV12:
        return SDL_PIXELFORMAT_NV12;
    case PIX_FMT_NV21:
        return SDL_PIXELFORMAT_NV21;
    case PIX_FMT_YUY2:
        return SDL_PIXELFORMAT_YUY2;
    case PIX_FMT_YVYU:
        return SDL_PIXELFORMAT_YVYU;
    case PIX_FMT_UYVY:
        return SDL_PIXELFORMAT_UYVY;
    case PIX_FMT_RGB:
        return SDL_PIXELFORMAT_RGB24;
    case PIX_FMT_BGR:
        return SDL_PIXELFORMAT_BGR24;
    case PIX_FMT_RGBA:
        return SDL_PIXELFORMAT_RGBA8888;
    case PIX_FMT_BGRA:
        return SDL_PIXELFORMAT_RGBA8888;
    case PIX_FMT_ARGB:
        return SDL_PIXELFORMAT_ARGB8888;
    case PIX_FMT_ABGR:
        return SDL_PIXELFORMAT_ABGR8888;
    }
    return SDL_PIXELFORMAT_UNKNOWN;
}


SDL2Wnd::SDL2Wnd(QWidget* parent) : HVideoWnd(parent), QWidget(parent)
{
    // 如果可用，则创建事件
    if(!s_sdl_init.test_and_set())
    {
        SDL_Init(SDL_INIT_VIDEO);
    }
    // 设置窗口类型
    setAttribute(Qt::WA_NativeWindow);
    // 在本地窗口上创建SDL窗体
    sdlWindow = SDL_CreateWindowFrom((void*)winId());
    // 为一个窗体创建渲染器，硬件加速，与刷新率同步
    sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    // 若创建失败则输出错误，且不设置参数情况下再试一次
    if(!sdlRenderer)
    {
        hlogw("Failed to initialize a hardware accelerated renderer: %s", SDL_GetError());
        sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, 0);
    }
    // 创建成功
    if(sdlRenderer)
    {
        // 分配空间
        memset(&renderer_info, 0, sizeof(SDL_RendererInfo));
        // 若获取渲染器信息成功
        if(SDL_GetRendererInfo(sdlRenderer, &renderer_info) == 0)
        {
            hlogd("Initialized %s renderer.", renderer_info.name);
        }
    }
    // 若出现错误，则输出错误
    if(!sdlWindow || !sdlRenderer || !renderer_info.num_texture_formats)
    {
        hloge("Failed to create window or renderer: %s", SDL_GetError());
        exit(-1);
    }
    // 设置缩放时的算法：线性插值，解决锯齿问题
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    sdlTexture = NULL;
    tex_w = tex_h = 0;
}

// 销毁纹理、渲染器、窗口
SDL2Wnd::~SDL2Wnd()
{
    if(sdlTexture)
    {
        SDL_DestroyTexture(sdlTexture);
        sdlTexture = NULL;
    }
    if(sdlRenderer)
    {
        SDL_DestroyRenderer(sdlRenderer);
        sdlRenderer = NULL;
    }
    if(sdlWindow)
    {
        SDL_DestroyWindow(sdlWindow);
        sdlWindow = NULL;
    }
}

// 更新时绘图
void SDL2Wnd::update()
{
    paintEvent(NULL);
}

// 绘图事件
void SDL2Wnd::paintEvent(QPaintEvent* e)
{
    // 首先计算帧率，添加一帧
    calcFPS();
    // 设置背景颜色：黑色
    SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, 255);
    // 用背景颜色清除版面
    SDL_RenderClear(sdlRenderer);
    // 如果上一帧非空
    if(!last_frame.isNull())
    {
        // 如果有改动
        if(sdlTexture == NULL || tex_w != last_frame.w || tex_h != last_frame.h || tex_pix_fmt != last_frame.type)
        {
            // 销毁已有纹理
            if(sdlTexture)
            {
                SDL_DestroyTexture(sdlTexture);
                sdlTexture = NULL;
            }
            // 根据帧类型确定颜色编码
            sdl_pix_fmt = SDL_pix_fmt(last_frame.type);
            // 创建新纹理
            sdlTexture = SDL_CreateTexture(sdlRenderer, sdl_pix_fmt, SDL_TEXTUREACCESS_STREAMING, last_frame.w, last_frame.h);
            if(sdlTexture == NULL)
            {
                hloge("SDL_CreateTexture");
                return;
            }
            void* pixels;
            int pitch;
            // 加锁，将更新的像素绑定，pitch代表一行的字节数
            if(SDL_LockTexture(sdlTexture, NULL, &pixels, &pitch) < 0)
            {
                return;
            }
            // 分配空间
            memset(pixels, 0, pitch * last_frame.h);
            // 解锁
            SDL_UnlockTexture(sdlTexture);
            // 继承参数
            tex_w = last_frame.w;
            tex_h = last_frame.h;
            tex_pix_fmt = last_frame.type;
            tex_bpp = pix_fmt_bpp(last_frame.type);
            tex_pitch = pitch;
            hlogi("SDL_Texture w=%d h=%d bpp=%d pitch=%d pix_fmt=%d", tex_w, tex_h, tex_bpp, tex_pitch, tex_pix_fmt);
        }
        // 更新纹理
        SDL_UpdateTexture(sdlTexture, NULL, last_frame.buf.base, tex_pitch);
        // 渲染全部
        SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, NULL);
    }
    // 渲染显示
    SDL_RenderPresent(sdlRenderer);
}

void SDL2Wnd::resizeEvent(QResizeEvent* e) {

}
