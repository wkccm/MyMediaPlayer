#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QWidget>

#include "lsidewidget.h"
#include "rsidewidget.h"
#include "hmultiview.h"

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
    LsideWidget* lside;
    HMultiView* mv;
    RsideWidget* rside;
};

#endif // CENTRALWIDGET_H
