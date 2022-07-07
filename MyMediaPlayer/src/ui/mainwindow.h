#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "singleton.h"
#include "centralwidget.h"


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
    void openMediaDlg(int index);

public:
    QAction *actMenubar;
    QAction *actFullscreen;
    QAction *actMvFullscreen;

    QVector<QToolBar*> toolbars;
    CentralWidget *center;
};

#define g_mainwd MainWindow::instance()
#endif // MAINWINDOW_H
