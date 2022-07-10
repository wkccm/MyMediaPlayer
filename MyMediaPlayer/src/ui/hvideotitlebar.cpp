#include "hvideotitlebar.h"
#include "qtstyles.h"
#include "qtfunctions.h"

HVideoTitleBar::HVideoTitleBar(QWidget *parent) : QFrame(parent)
{
    initUI();
    initConnect();
}

// 初始化界面
void HVideoTitleBar::initUI()
{
    // 设定高度
    setFixedHeight(VIDEO_TITLEBAR_HEIGHT);
    // 标题标签
    labTitle = new QLabel;
    // 按钮绑定
    btnClose = pushButton(QPixmap(":/image/close.png"), tr("close"));
    // 设置水平布局
    QHBoxLayout *hbox = HBoxLayout();
    // 添加标题
    hbox->addWidget(labTitle);
    // 补空
    hbox->addStretch();
    // 添加关闭按钮
    hbox->addWidget(btnClose);
    // 设置布局
    setLayout(hbox);
}

void HVideoTitleBar::initConnect()
{

}
