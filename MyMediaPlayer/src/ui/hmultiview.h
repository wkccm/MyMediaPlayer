#ifndef HMULTIVIEW_H
#define HMULTIVIEW_H

#include <QWidget>
#include "hvideowidget.h"
#include "htable.h"


#define MV_STYLE_MAXNUM     64

// 利用define设置enum
#define FOREACH_MV_STYLE(F) \
    F(MV_STYLE_1, 1, 1, "MV1", ":/image/style1.png")    \
    F(MV_STYLE_2, 1, 2, "MV2", ":/image/style2.png")    \
    F(MV_STYLE_4, 2, 2, "MV4", ":/image/style4.png")    \
    F(MV_STYLE_9, 3, 3, "MV9", ":/image/style9.png")    \
    F(MV_STYLE_16, 4, 4, "MV16", ":/image/style16.png")    \
    F(MV_STYLE_25, 5, 5, "MV25", ":/image/style25.png")    \
    F(MV_STYLE_36, 6, 6, "MV36", ":/image/style36.png")    \
    F(MV_STYLE_49, 7, 7, "MV49", ":/image/style49.png")    \
    F(MV_STYLE_64, 8, 8, "MV64", ":/image/style64.png")    \

enum MV_STYLE
{
#define ENUM_MV_STYLE(id, row, col, label, image) id,
    FOREACH_MV_STYLE(ENUM_MV_STYLE)
};


class HMultiView : public QWidget
{
    Q_OBJECT
public:
    // 伸展、交换、混合
    enum Action
    {
        STRETCH,
        EXCHANGE,
        MERGE,
    };

    explicit HMultiView(QWidget *parent = nullptr);
    ~HMultiView();

    // 根据id查找播放器
    HVideoWidget* getPlayerByID(int playerid);
    // 根据当前点位查找播放器
    HVideoWidget* getPlayerByPos(QPoint pt);
    // 获取一个空闲播放器
    HVideoWidget* getIdlePlayer();

signals:

public slots:
    // 设置某行数和列数的界面
    void setLayout(int row, int col);
    // 保存当前界面状态
    void saveLayout();
    // 恢复上一次界面状态
    void restoreLayout();
    // 交换单元
    void exchangeCells(HVideoWidget* player1, HVideoWidget* player2);
    // 播放器独占
    void stretch(QWidget* wdg);
    // 播放媒体
    void play(HMedia& media);

protected:
    // 初始化界面
    void initUI();
    // 初始化连接
    void initConnect();
    // 更新界面
    void updateUI();
    // 调整界面尺寸事件
    virtual void resizeEvent(QResizeEvent* e);
    // 鼠标点击事件
    virtual void mousePressEvent(QMouseEvent* e);
    // 鼠标释放事件
    virtual void mouseReleaseEvent(QMouseEvent* e);
    // 鼠标移动事件
    virtual void mouseMoveEvent(QMouseEvent* e);
    // 鼠标双击事件
    virtual void mouseDoubleClickEvent(QMouseEvent* e);

public:
    // 引用开源库表,记录当前界面状态
    HTable table;
    // 记录上一次界面状态
    HTable prev_table;
    // 多个窗口
    QVector<QWidget*> views;
    // 矩形工具
    QLabel* labRect;
    // 伸展工具
    QLabel* labDrag;
    // 记录鼠标位置
    QPoint ptMousePress;
    // 记录鼠标点击时间
    uint64_t tsMousePress;
    // 记录事件
    Action action;
    // 记录是否独占
    bool bStretch;
};

#endif // HMULTIVIEW_H
