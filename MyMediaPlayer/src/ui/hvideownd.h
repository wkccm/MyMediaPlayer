#ifndef HVIDEOWND_H
#define HVIDEOWND_H

#include <QWidget>

#include "hframe.h"

class HVideoWnd
{
public:
    HVideoWnd(QWidget *parent = nullptr);
    virtual ~HVideoWnd(){}

    // 纯虚函数，设置几何体
    virtual void setGeometry(const QRect& rc) = 0;
    // 纯虚函数，更新窗口
    virtual void update() = 0;

protected:
    // 计算帧率
    void calcFPS();

public:
    // 记录最后一帧
    HFrame      last_frame;
    // 帧率
    int         fps;
    // 是否显示时间
    bool        draw_time;
    // 是否显示帧率
    bool        draw_fps;
    // 是否显示分辨率
    bool        draw_resolution;

protected:
    // 辅助计算帧率,上一次打点时间
    uint64_t    tick;
    // 辅助计算帧率,帧数统计
    int         framecnt;
};

#endif // HVIDEOWND_H
