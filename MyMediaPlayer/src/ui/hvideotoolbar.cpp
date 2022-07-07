#include "hvideotoolbar.h"
#include "qtstyles.h"
#include "qtfunctions.h"

HVideoToolBar::HVideoToolBar(QWidget *parent) : QFrame(parent)
{
    initUI();
    initConnect();
}

void HVideoToolBar::initUI()
{
    setFixedHeight(VIDEO_TOOLBAR_HEIGHT);

    QSize sz(VIDEO_TOOLBAR_ICON_WIDTH, VIDEO_TOOLBAR_ICON_HEIGHT);
    btnStart = pushButton(QPixmap(":/image/start.png"), tr("start"));
    btnPause = pushButton(QPixmap(":/image/pause.png"), tr("pause"));
    btnPrev = pushButton(QPixmap(":/image/prev.png"), tr("prev"));
    btnStop = pushButton(QPixmap(":/image/stop.png"), tr("stop"));
    btnStop->setAutoDefault(true);
    btnNext = pushButton(QPixmap(":/image/next.png"), tr("next"));

    sldProgress = new QSlider;
    sldProgress->setOrientation(Qt::Horizontal);
    lblDuration = new QLabel("00:00:00");

    QHBoxLayout *hbox = HBoxLayout();
    hbox->setSpacing(5);
    hbox->addWidget(btnStart, 0, Qt::AlignLeft);
    hbox->addWidget(btnPause, 0, Qt::AlignLeft);
    btnPause->hide();

    hbox->addSpacing(5);
    hbox->addWidget(btnPrev, 0, Qt::AlignLeft);
    btnPrev->hide();
    hbox->addWidget(btnStop, 0, Qt::AlignLeft);
    btnStop->hide();
    hbox->addWidget(btnNext, 0, Qt::AlignLeft);
    btnNext->hide();

    hbox->addSpacing(5);
    hbox->addWidget(sldProgress);
    sldProgress->hide();
    hbox->addWidget(lblDuration);
    lblDuration->hide();

    setLayout(hbox);
}

void HVideoToolBar::initConnect()
{
    connectButtons(btnStart, btnPause);
    connect(btnStart, SIGNAL(clicked(bool)), this, SIGNAL(sigStart()));
    connect(btnPause, SIGNAL(clicked(bool)), this, SIGNAL(sigPause()));
    connect(btnStop, SIGNAL(clicked(bool)), this, SIGNAL(sigStop()));

    connect(btnStop, SIGNAL(clicked(bool)), btnStart, SLOT(show()));
    connect(btnStop, SIGNAL(clicked(bool)), btnPause, SLOT(hide()));
}
