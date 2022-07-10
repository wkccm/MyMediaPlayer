#ifndef HVIDEOWIDGET_H
#define HVIDEOWIDGET_H

#include <QFrame>
#include "hmedia.h"
#include "HVideoWndFactory.h"
#include "hvideotitlebar.h"
#include "hvideotoolbar.h"
#include "HVideoPlayer.h"

class HVideoWidget : public QFrame
{
    Q_OBJECT
public:
    // 播放器状态
    enum Status
    {
        STOP,
        PAUSE,
        PLAY,
    };

    explicit HVideoWidget(QWidget *parent = nullptr);
    ~HVideoWidget();

signals:

public slots:
    // 打开媒体
    void open(HMedia &media);
    // 关闭媒体
    void close();
    // 开始播放
    void start();
    // 停止播放
    void stop();
    // 暂停播放
    void pause();
    void resume();
    void restart();
    void retry();

    void onTimerUpdate();
    void onOpenSucceed();
    void onOpenFailed();
    void onPlayerEOF();
    void onPlayerError();

    void setAspectRatio(aspect_ratio_t aspect_ratio);

protected:
    void initUI();
    void initConnect();
    void updateUI();

    virtual void resizeEvent(QResizeEvent *e);
    virtual void enterEvent(QEvent *e);
    virtual void leaveEvent(QEvent *e);
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void customEvent(QEvent *e);

public:
    // 播放器id
    int     playerid;
    // 播放器状态
    int     status;
    // 标题
    QString title;
    // 帧率
    int     fps;
    // 显示比例
    aspect_ratio_t      aspect_ratio;
    // 渲染类型
    renderer_type_e     renderer_type;
    // 视频窗口
    HVideoWnd*           videownd;
    // 视频标题
    HVideoTitleBar*      titlebar;
    // 视频工具栏
    HVideoToolBar*       toolbar;
    // 媒体选择按钮
    QPushButton*         btnMedia;

private:
    // 记录鼠标点击位置
    QPoint          ptMousePress;
    // 记录时间
    QTimer*         timer;
    // 媒体文件
    HMedia          media;
    // 播放器
    HVideoPlayer*   pImpl_player;
    // 重试间隔
    int         retry_interval;
    // 重试最大次数
    int         retry_maxcnt;
    // 上一次重试时间
    int64_t     last_retry_time;
    // 重试次数
    int         retry_cnt;
};

#endif // HVIDEOWIDGET_H
