#include "hvideownd.h"
#include "confile.h"
#include "htime.h"

// 初始化各参数
HVideoWnd::HVideoWnd(QWidget *parent)
{
    fps = 0;
    framecnt = 0;
    tick = 0;
    draw_time = g_confile->Get<bool>("draw_time", "ui", false);
    draw_fps = g_confile->Get<bool>("draw_fps", "ui", false);
    draw_resolution = g_confile->Get<bool>("draw_resolution", "ui", false);
}

// 计算帧率
void HVideoWnd::calcFPS()
{
    // 与上次打点时间相隔超过1s时
    if(gettick_ms() - tick > 1000)
    {
        // 为帧率赋值,并刷新计数
        fps = framecnt;
        framecnt = 0;
        tick = gettick_ms();
    }
    else
    {
        ++framecnt;
    }
}
