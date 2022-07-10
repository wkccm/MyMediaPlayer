#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QWidget>

#include "lsidewidget.h"
#include "rsidewidget.h"
#include "hmultiview.h"


// 依赖三个组件
class CentralWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CentralWidget(QWidget *parent = nullptr);
    ~CentralWidget();

signals:

public slots:

protected:
    void initUI();
    void initConnect();

public:
    // 三个组件
    LsideWidget* lside;
    HMultiView* mv;
    RsideWidget* rside;
};

#endif // CENTRALWIDGET_H
