#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "singleton.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    SINGLETON_DECL(MainWindow)

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    enum window_state_enum
    {
        NORMAL = 0,
        MINIMIZED,
        MAXIMIZED,
        FULLSCREEN,
    }window_state;

protected:
    void initUI();
    void initConnect();
    void initMenu();

    virtual void keyPressEvent(QKeyEvent* e);
    virtual void changeEvent(QEvent* e);

signals:

public slots:
    void about();
    void fullscreen();
    void onMVStyleSelected(int id);
    void mv_fullscreen();
    void openMediaDialog(int index);

public:
    QAction *actMenuBar;
    QAction *actFullscreen;
    QAction *actMvFullscreen;

    QVector<QToolBar*> toolBars;
    CentralWidget *center;
};
#endif // MAINWINDOW_H
