#ifndef HVIDEOTOOLBAR_H
#define HVIDEOTOOLBAR_H

#include <QFrame>
#include <QPushButton>
#include <QSlider>
#include <QLabel>

class HVideoToolBar : public QFrame
{
    Q_OBJECT
public:
    explicit HVideoToolBar(QWidget *parent = nullptr);

signals:
    void sigStart();
    void sigPause();
    void sigStop();

public slots:

protected:
    void initUI();
    void initConnect();

public:
    QPushButton *btnStart;
    QPushButton *btnPause;
    QPushButton *btnPrev;
    QPushButton *btnStop;
    QPushButton *btnNext;

    QSlider     *sldProgress;
    QLabel      *lblDuration;

};

#endif // HVIDEOTOOLBAR_H
