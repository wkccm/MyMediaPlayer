#ifndef HVIDEOTOOLBAR_H
#define HVIDEOTOOLBAR_H

#include <QFrame>
#include <QPushButton>
#include <QSlider>
#include <QLabel>

class HVideoToolBar : public QFrame
{
    Q_OBJECT
public:
    explicit HVideoToolBar(QWidget *parent = nullptr);

signals:
    // 开始信号
    void sigStart();
    // 暂停信号
    void sigPause();
    // 停止信号
    void sigStop();

public slots:

protected:
    void initUI();
    void initConnect();

public:
    // 开始按钮
    QPushButton *btnStart;
    // 暂停按钮
    QPushButton *btnPause;
    // 停止按钮
    QPushButton *btnStop;
    // 播放滑动条
    QSlider     *sldProgress;
    // 视频时长
    QLabel      *lblDuration;

};

#endif // HVIDEOTOOLBAR_H
