#include "hopenmediadlg.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QFileDialog>
#include <QComboBox>
#include "confile.h"
#include <QDialogButtonBox>
#include "qtfunctions.h"
#include <QMessageBox>


// 本地文件
FileTab::FileTab(QWidget* parent) : QWidget(parent)
{
    // 垂直布局
    QVBoxLayout* vbox = new QVBoxLayout;
    // 补空白均分
    vbox->addStretch();
    // 添加标签
    vbox->addWidget(new QLabel(tr("FileL")));
    // 水平布局
    QHBoxLayout* hbox = new QHBoxLayout;
    // 行文本
    edit = new QLineEdit;
    // 从配置文件中读取上一次打开的文件名
    std::string str = g_confile->GetValue("last_file_source", "media");
    // 如果非空则直接设置
    if(!str.empty())
    {
        // 转换格式
        edit->setText(QString::fromUtf8(str.c_str()));
    }
    // 行文本插入水平布局
    hbox->addWidget(edit);
    // 按钮：浏览
    btnBrowse = new QPushButton("...");
    // 设置按钮函数，点击时打开文件选择窗口，设置文件类型和名称
    connect(btnBrowse, &QPushButton::clicked, this, [=](){
        QString file = QFileDialog::getOpenFileName(this, tr("Open Media File"), "", "Video Files(*.3gp *.amv *.asf *.avi *.flv *.m2v *.m4v *.mkv *.mp2 *.mp4 *.mpg *.swf *.ts *.rmvb *.wmv)\n""All Files(*)");
        if(!file.isEmpty())
        {
            edit->setText(file);
        }
    });
    // 水平布局中插入按钮
    hbox->addWidget(btnBrowse);
    // 垂直布局中插入水平布局
    vbox->addLayout(hbox);
    // 垂直布局补空
    vbox->addStretch();
    // 显示垂直布局
    setLayout(vbox);
}

// 网络文件
NetworkTab::NetworkTab(QWidget* parent):QWidget(parent)
{
    QVBoxLayout* vbox = new QVBoxLayout;

    vbox->addStretch();
    vbox->addWidget(new QLabel(tr("URL:")));

    edit = new QLineEdit;
    std::string str = g_confile->GetValue("last_network_source", "media");
    if(!str.empty())
    {
        edit->setText(QString::fromUtf8(str.c_str()));
    }

    vbox->addWidget(edit);
    vbox->addStretch();

    setLayout(vbox);
}

HOpenMediaDlg::HOpenMediaDlg(QWidget* parent):QDialog(parent)
{
    initUI();
    initConnect();
}

// 打开对话窗口
void HOpenMediaDlg::initUI()
{
    // 设置标题
    setWindowTitle(tr("Open media"));
    // 设置大小 
    setFixedSize(600, 300);
    // 按钮：打开和取消
    QDialogButtonBox* btns = new QDialogButtonBox(QDialogButtonBox::Open | QDialogButtonBox::Cancel);
    connect(btns, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(btns, &QDialogButtonBox::rejected, this, &QDialog::reject);

    // 标签组件
    tab = new QTabWidget;
    // 添加标签
    tab->addTab(new FileTab, QIcon(":/image/file.png"), tr("File"));
    tab->addTab(new NetworkTab, QIcon(":/image/network.png"), tr("Network"));
    // 设置打开窗口时的默认标签
    tab->setCurrentIndex(g_confile->Get<int>("last_tab", "media", DEFAULT_MEDIA_TYPE));

    QVBoxLayout* vbox = VBoxLayout();
    vbox->addWidget(tab);
    vbox->addWidget(btns);
    setLayout(vbox);
}

void HOpenMediaDlg::initConnect()
{

}

// 接受
void HOpenMediaDlg::accept()
{
    // 根据当前标签判断
    switch(tab->currentIndex())
    {
    case MEDIA_TYPE_FILE:
    {
        // 强转窗口
        FileTab* filetab = qobject_cast<FileTab*>(tab->currentWidget());
        if (filetab) {
            // 设置媒体类型
            media.type = MEDIA_TYPE_FILE;
            // 转格式
            media.src = filetab->edit->text().toUtf8().data();
            // 修改配置文件
            g_confile->SetValue("last_file_source", media.src.c_str(), "media");
            // 配置文件保存
            g_confile->Save();
        }
        break;
    }
    case MEDIA_TYPE_NETWORK:
    {
        NetworkTab* nettab = qobject_cast<NetworkTab*>(tab->currentWidget());
        if (nettab) {
            media.type = MEDIA_TYPE_NETWORK;
            media.src = nettab->edit->text().toUtf8().data();
            g_confile->SetValue("last_network_source", media.src.c_str(), "media");
            g_confile->Save();
        }
        break;
    }
    default:
        break;
    }
    // 如果没有文件类型，或无文件源，则弹出窗口提醒
    if(media.type == MEDIA_TYPE_NONE || (media.src.empty() && media.index < 0))
    {
        QMessageBox::information(this, tr("Info"), tr("Invalid media source!"));
        return;
    }
    // 记录当前停留的标签页
    g_confile->Set<int>("last_tab", tab->currentIndex(), "media");
    g_confile->Save();
    // 关闭窗口，为accepted槽设定返回值
    QDialog::accept();
}
