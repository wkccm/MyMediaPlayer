#include "centralwidget.h"
#include "hlog.h"
#include <QSplitter>
#include "qtstyles.h"
#include "qtfunctions.h"

CentralWidget::CentralWidget(QWidget* parent) : QWidget(parent)
{
    initUI();
    initConnect();
}

CentralWidget::~CentralWidget()
{
    hlogd("~CentralWidget");
}

void CentralWidget::initUI()
{
    lside = new LsideWidget;
    mv = new HMultiView;
    rside = new RsideWidget;

    // 分离部件水平方向布置左侧栏、多窗口播放器、右侧栏
    QSplitter *split = new QSplitter(Qt::Horizontal);
    split->addWidget(lside);
    split->addWidget(mv);
    split->addWidget(rside);

    // 设置尺寸
    lside->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    lside->setMinimumWidth(LSIDE_MIN_WIDTH);
    lside->setMaximumWidth(LSIDE_MAX_WIDTH);

    mv->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mv->setMinimumWidth(MV_MIN_WIDTH);
    mv->setMinimumHeight(MV_MIN_HEIGHT);

    rside->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    rside->setMinimumWidth(RSIDE_MIN_WIDTH);
    rside->setMaximumWidth(RSIDE_MAX_WIDTH);

    // 设置三个组件都可伸缩
    split->setStretchFactor(0, 1);
    split->setStretchFactor(1, 1);
    split->setStretchFactor(2, 1);

    // 水平布局设置
    QHBoxLayout *hbox = HBoxLayout();
    hbox->addWidget(split);
    setLayout(hbox);
}

void CentralWidget::initConnect()
{

}
