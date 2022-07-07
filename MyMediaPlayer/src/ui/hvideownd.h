#ifndef HVIDEOWND_H
#define HVIDEOWND_H

#include <QWidget>

#include "hframe.h"

class HVideoWnd
{
public:
    HVideoWnd(QWidget *parent = nullptr);
    virtual ~HVideoWnd(){}

    virtual void setGeometry(const QRect& rc) = 0;
    virtual void update() = 0;

protected:
    void calcFPS();

public:
    HFrame      last_frame;
    int         fps;
    bool        draw_time;
    bool        draw_fps;
    bool        draw_resolution;

protected:
    uint64_t    tick;
    int         framecnt;
};

#endif // HVIDEOWND_H
