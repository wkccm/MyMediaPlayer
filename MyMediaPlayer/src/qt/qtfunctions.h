#ifndef QTFUNCTIONS_H
#define QTFUNCTIONS_H

#include <QWidget>
#include <QtGui>
#include <QApplication>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>


#include "qtheaders.h"


typedef QWidget HWidget;

// 为一个图形界面程序设置字体大小
inline void setFont(int size)
{
    QFont font = qApp->font();
    font.setPointSize(size);
    qApp->setFont(font);
}

// 设置调色板
inline void setPalette(QColor color)
{
    qApp->setPalette(QPalette(color));
}

// 加载皮肤
inline void loadSkin(const char* skin)
{
    QFile file(QString(":/skin/") + QString(skin) + QString(".qss"));
    if(file.open(QFile::ReadOnly))
    {
        qApp->setStyleSheet(file.readAll());
        file.close();
    }
}

// 加载语言
// 注意生成qm文件
inline void loadLang(const char* lang)
{
    QTranslator *app_translator = new QTranslator(qApp);
    if(lang && *lang)
    {
        app_translator->load(QString(":/lang/app_") + QString(lang) + QString(".qm"));
    }
    else
    {
        app_translator->load(QLocale(), "app", "_", ":/lang");
    }

    qApp->installTranslator(app_translator);

    QTranslator *qt_translator = new QTranslator(qApp);
    if(lang && *lang)
    {
        qt_translator->load(QString(":/lang/qt_") + QString(lang) + QString(".qm"));
    }
    else
    {
        qt_translator->load(QLocale(), "qt", "_", ":/lang");
    }
    qApp->installTranslator(qt_translator);
}

// pixmap像素图
inline QPushButton* pushButton(QPixmap pixmap, QString tooltip = QString(), QSize sz = QSize(0, 0), QWidget* parent = NULL)
{
    QPushButton* btn = new QPushButton(parent);
    // 突出效果
    btn->setFlat(true);
    if(sz.isEmpty())
    {
        sz = pixmap.size();
    }
    btn->setFixedSize(sz);
    btn->setIconSize(sz);
    // 忽略原图片长宽比，纹理平滑转换
    btn->setIcon(pixmap.scaled(sz, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    // 按钮提醒框
    btn->setToolTip(tooltip);
    return btn;
}

inline QHBoxLayout* HBoxLayout()
{
    QHBoxLayout* hbox = new QHBoxLayout;
    hbox->setContentsMargins(10, 1, 10, 1);
    hbox->setSpacing(1);
    return hbox;
}

inline QVBoxLayout* VBoxLayout()
{
    QVBoxLayout* vbox = new QVBoxLayout;
    vbox->setContentsMargins(1, 1, 1, 1);
    vbox->setSpacing(1);
    return vbox;
}

// 设置前景背景
inline void setBgFg(QWidget* wdg, QColor bg, QColor fg = Qt::white)
{
    wdg->setAutoFillBackground(true);
    QPalette pal = wdg->palette();
    // Qt6中background和foreground被window和windowtext取代
    pal.setColor(QPalette::Window, bg);
    pal.setColor(QPalette::WindowText, fg);
    wdg->setPalette(pal);
}

// 按钮的互斥存在函数
inline void connectButtons(QPushButton* btn1, QPushButton* btn2)
{
    QObject::connect(btn1, SIGNAL(clicked(bool)), btn1, SLOT(hide()));
    QObject::connect(btn1, SIGNAL(clicked(bool)), btn2, SLOT(show()));

    QObject::connect(btn2, SIGNAL(clicked(bool)), btn2, SLOT(hide()));
    QObject::connect(btn2, SIGNAL(clicked(bool)), btn1, SLOT(show()));
}

// 设置应用在桌面上居中
inline void centerWidget(QWidget* wdg)
{
    int w = wdg->width();
    int h = wdg->height();
    if(w < DESKTOP_WIDTH && h < DESKTOP_HEIGHT)
    {
        wdg->setGeometry((DESKTOP_WIDTH-w)/2,(DESKTOP_HEIGHT-h)/2, w, h);
    }
}

// 画矩形
inline QRect adjustRect(QPoint pt1, QPoint pt2)
{
    int xMin = qMin(pt1.x(), pt2.x());
    int xMax = qMax(pt1.x(), pt2.x());
    int yMin = qMin(pt1.y(), pt2.y());
    int yMax = qMax(pt1.y(), pt2.y());
    return QRect(QPoint(xMin, yMin), QPoint(xMax, yMax));
}

// 切换可见性
inline void toggle(QWidget* wdg)
{
    wdg->setVisible(!wdg->isVisible());
}

#endif // QTFUNCTIONS_H
