#include "hvideotoolbar.h"
#include "qtstyles.h"
#include "qtfunctions.h"

HVideoToolBar::HVideoToolBar(QWidget *parent) : QFrame(parent)
{
    initUI();
    initConnect();
}

// 初始化界面
void HVideoToolBar::initUI()
{
    // 设定高度
    setFixedHeight(VIDEO_TOOLBAR_HEIGHT);
    // 设置按钮图标大小，可备用
    QSize sz(VIDEO_TOOLBAR_ICON_WIDTH, VIDEO_TOOLBAR_ICON_HEIGHT);
    // 绑定各按钮
    btnStart = pushButton(QPixmap(":/image/start.png"), tr("start"), sz);
    btnPause = pushButton(QPixmap(":/image/pause.png"), tr("pause"), sz);
    btnStop = pushButton(QPixmap(":/image/stop.png"), tr("stop"), sz);
    btnStop->setAutoDefault(true);

    sldProgress = new QSlider;
    sldProgress->setOrientation(Qt::Horizontal);
    lblDuration = new QLabel("00:00:00");

    QHBoxLayout *hbox = HBoxLayout();
    hbox->setSpacing(5);
    hbox->addWidget(btnStart, 0, Qt::AlignLeft);
    hbox->addWidget(btnPause, 0, Qt::AlignLeft);
    btnPause->hide();

    hbox->addSpacing(5);
    hbox->addWidget(btnStop, 50, Qt::AlignLeft);
    btnStop->hide();

    hbox->addSpacing(5);
    hbox->addWidget(sldProgress);
    sldProgress->hide();
    hbox->addWidget(lblDuration);
    lblDuration->hide();

    setLayout(hbox);
}

void HVideoToolBar::initConnect()
{
    // 开始和暂停互斥
    connectButtons(btnStart, btnPause);
    // 绑定信号
    connect(btnStart, SIGNAL(clicked(bool)), this, SIGNAL(sigStart()));
    connect(btnPause, SIGNAL(clicked(bool)), this, SIGNAL(sigPause()));
    connect(btnStop, SIGNAL(clicked(bool)), this, SIGNAL(sigStop()));
    // 停止时，显示开始按钮，隐藏关闭按钮
    connect(btnStop, SIGNAL(clicked(bool)), btnStart, SLOT(show()));
    connect(btnStop, SIGNAL(clicked(bool)), btnPause, SLOT(hide()));
}
