#ifndef HVIDEOTITLEBAR_H
#define HVIDEOTITLEBAR_H

#include <QFrame>
#include <QLabel>
#include <QPushButton>

class HVideoTitleBar : public QFrame
{
    Q_OBJECT
public:
    explicit HVideoTitleBar(QWidget *parent = nullptr);

protected:
    void initUI();
    void initConnect();

public:
    // 播放器标题
    QLabel *labTitle;
    // 播放器关闭按钮
    QPushButton *btnClose;
};

#endif // HVIDEOTITLEBAR_H
