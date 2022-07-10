#ifndef HOPENMEDIADLG_H
#define HOPENMEDIADLG_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QDialog>
#include <QTabWidget>

#include "hmedia.h"

// 本地文件弹出窗口类
class FileTab : public QWidget
{
    Q_OBJECT
public:
    explicit FileTab(QWidget* parent=nullptr);

public:
    // 可编辑行文本
    QLineEdit* edit;
    // 浏览按钮
    QPushButton* btnBrowse;
};

// 网络文件选择窗口类
class NetworkTab:public QWidget
{
    Q_OBJECT
public:
    explicit NetworkTab(QWidget* parent = nullptr);

public:
    QLineEdit* edit;
};

// 对话窗口
class HOpenMediaDlg : public QDialog
{
    Q_OBJECT
public:
    explicit HOpenMediaDlg(QWidget* parent = nullptr);

signals:
public slots:
    virtual void accept();
protected:
    void initUI();
    void initConnect();

public:
    QTabWidget* tab;
    HMedia media;
};

#endif // HOPENMEDIADLG_H
