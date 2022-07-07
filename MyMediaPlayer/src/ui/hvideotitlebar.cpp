#include "hvideotitlebar.h"
#include "qtstyles.h"
#include "qtfunctions.h"

HVideoTitleBar::HVideoTitleBar(QWidget *parent) : QFrame(parent)
{
    initUI();
    initConnect();
}

void HVideoTitleBar::initUI()
{
    setFixedHeight(VIDEO_TITLEBAR_HEIGHT);
    labTitle = new QLabel;
    btnClose = pushButton(QPixmap(":/image/close.png"), tr("close"));

    QHBoxLayout *hbox = HBoxLayout();
    hbox->addWidget(labTitle);
    hbox->addStretch();
    hbox->addWidget(btnClose);
    setLayout(hbox);
}

void HVideoTitleBar::initConnect()
{

}
