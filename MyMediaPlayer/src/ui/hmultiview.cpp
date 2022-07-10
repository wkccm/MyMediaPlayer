#include "hmultiview.h"
#include "hlog.h"
#include "qtstyles.h"
#include <QMouseEvent>
#include "htime.h"
#include "qtfunctions.h"
#include <QMessageBox>

#define SEPARATOR_LINE_WIDTH 1

HMultiView::HMultiView(QWidget *parent) : QWidget(parent)
{
    initUI();
    initConnect();
    // 初始时设置为false
    bStretch = false;
}

HMultiView::~HMultiView()
{
    hlogd("~HMultiView");
}

// 初始化界面
void HMultiView::initUI()
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    for(int i=0;i<MV_STYLE_MAXNUM;++i)
    {
        HVideoWidget* player = new HVideoWidget(this);
        player->playerid = i+1;
        views.push_back(player);
    }
    // 读配置文件中的行数、列数
    int row = g_confile->Get<int>("mv_row", "ui", MV_STYLE_ROW);
    int col = g_confile->Get<int>("mv_col", "ui", MV_STYLE_COL);

    setLayout(row, col);

    labDrag = new QLabel(this);
    labDrag->setFixedSize(DRAG_WIDTH, DRAG_HEIGHT);
    labDrag->hide();
    labDrag->setStyleSheet(DRAG_QSS);

    labRect = new QLabel(this);
    labRect->hide();
    labRect->setStyleSheet(RECT_QSS);
}

void HMultiView::initConnect()
{

}

// 设置当前状态
void HMultiView::setLayout(int row, int col)
{
    // 先保存当前状态
    saveLayout();
    // 初始化表
    table.init(row, col);
    // 更新界面
    updateUI();
    // 向配置文件中写入行数、列数
    g_confile->Set<int>("mv_row", row, "ui");
    g_confile->Set<int>("mv_col", col, "ui");
}

// 交换两个单元
void HMultiView::exchangeCells(HVideoWidget* player1, HVideoWidget* player2)
{
    QRect rcTmp = player1->geometry();
    int idTmp = player1->playerid;
    player1->setGeometry(player2->geometry());
    player1->playerid = player2->playerid;
    player2->setGeometry(rcTmp);
    player2->playerid = idTmp;
}

// 按id查找播放器
HVideoWidget* HMultiView::getPlayerByID(int playerid)
{
    for(int i=0;i<views.size();++i)
    {
        HVideoWidget* player = (HVideoWidget*)views[i];
        if(player->playerid == playerid)
        {
            return player;
        }
    }
    return NULL;
}

// 按鼠标点位查找播放器
HVideoWidget* HMultiView::getPlayerByPos(QPoint pt)
{
    for(int i=0;i<views.size();++i)
    {
        QWidget* wdg = views[i];
        if(wdg->isVisible() && wdg->geometry().contains(pt))
        {
            return (HVideoWidget*)wdg;
        }
    }
    return NULL;
}

// 抓取一个空闲播放器
HVideoWidget* HMultiView::getIdlePlayer()
{
    for(int i=0;i<views.size();++i)
    {
        HVideoWidget* player = (HVideoWidget*)views[i];
        if(player->isVisible() && player->status == HVideoWidget::STOP)
        {
            return player;
        }
    }
    return NULL;
}

// 更新界面
void HMultiView::updateUI()
{
    // 获取表格行数、列数
    int row = table.row;
    int col = table.col;
    if(row == 0 || col == 0)return;
    // 计算每个单元的宽和高
    int cell_w = width()/col;
    int cell_h = height()/row;
    int margin_x = (width()-cell_w*col)/2;
    int margin_y = (height()-cell_h*row)/2;
    int x=margin_x;
    int y=margin_y;

    // 先将所有窗口隐藏
    for(int i=0;i<views.size();++i)
    {
        views[i]->hide();
    }

    int cnt = 0;
    HTableCell cell;
    for(int r=0;r<row;++r)
    {
        for(int c=0;c<col;++c)
        {
            int id = r*col+c+1;
            if(table.getTableCell(id, cell))
            {
                QWidget* wdg = getPlayerByID(id);
                if(wdg)
                {
                    // 选取指定的窗口，画出并显示
                    wdg->setGeometry(x, y, cell_w*cell.colspan()-SEPARATOR_LINE_WIDTH, cell_h*cell.rowspan()-SEPARATOR_LINE_WIDTH);
                    wdg->show();
                    ++cnt;
                }
            }
            x+=cell_w;
        }
        x=margin_x;
        y+=cell_h;
    }
    // 当只有一个播放器可用时，伸展状态设为true
    bStretch = (cnt == 1);
}

// 更新尺寸
void HMultiView::resizeEvent(QResizeEvent* e)
{
    updateUI();
}

// 鼠标点击事件
void HMultiView::mousePressEvent(QMouseEvent* e)
{
    // 取点
    ptMousePress = e->pos();
    // 取点击时间
    tsMousePress = gettick_ms();
}

// 鼠标释放事件
void HMultiView::mouseReleaseEvent(QMouseEvent* e)
{
    // 若为交换事件
    if(action == EXCHANGE)
    {
        // 取点击发生时的窗口
        HVideoWidget* player1 = getPlayerByPos(ptMousePress);
        // 取释放发生时的窗口
        HVideoWidget* player2 = getPlayerByPos(e->pos());
        // 当窗口有效且不同时交换
        if(player1 && player2 && player1 != player2)
        {
            exchangeCells(player1, player2);
        }
    }
    // 点击完成后，拖拽过程中的临时窗口设为不可见
    labRect->QWidget::setVisible(false);
    labDrag->QWidget::setVisible(false);
    setCursor(Qt::ArrowCursor);
}

// 鼠标移动事件
void HMultiView::mouseMoveEvent(QMouseEvent* e)
{
    // 如果按下鼠标时的位置没有播放器
    HVideoWidget* player = getPlayerByPos(ptMousePress);
    if(player == NULL)
    {
        return;
    }
    // 只有鼠标左键点击时生效
    if(e->buttons() == Qt::LeftButton)
    {
        // 若拖拽窗口不可见
        if(!labDrag->isVisible())
        {
            // 按下鼠标0.3s内不显示拖拽窗口
            if(gettick_ms()-tsMousePress < 300)return;
            // 设为交换模式
            action = EXCHANGE;
            // 改变鼠标样式
            setCursor(Qt::OpenHandCursor);
            // 在鼠标位置设置一个播放器的缩小版
            labDrag->setPixmap(player->grab().scaled(labDrag->size()));
            // 让临时矩形可见
            labDrag->setVisible(true);
        }
        else
        {
            // 设置鼠标在临时矩形的下边中点
            labDrag->move(e->pos()-QPoint(labDrag->width()/2, labDrag->height()));
        }
    }
}

// 鼠标双击事件
void HMultiView::mouseDoubleClickEvent(QMouseEvent* e)
{
    // 捕获播放器
    HVideoWidget* player = getPlayerByPos(e->pos());
    // 如果播放器有效
    if(player)
    {
        // 将选中的播放器全屏
        stretch(player);
    }
}

// 播放器独占
void HMultiView::stretch(QWidget* wdg)
{
    // 如果已经独占，则返回
    if(table.row == 1 && table.col == 1)return;
    // 若播放器已经独占
    if(bStretch)
    {
        // 返回独占前的状态
        restoreLayout();
        // 独占标志设为false
        bStretch = false;
    }
    else
    {
        // 独占前记录状态
        saveLayout();
        // 将所有播放器隐藏
        for(int i=0;i<views.size();++i)
        {
            views[i]->hide();
        }
        // 将选中的播放器设为上一级的矩形
        wdg->setGeometry(rect());
        // 显示
        wdg->show();
        // 将独占标志设为true
        bStretch = true;
    }
}

// 保存当前状态
void HMultiView::saveLayout()
{
    prev_table = table;
}

// 恢复状态
void HMultiView::restoreLayout()
{
    // 将上一次状态设为默认表
    HTable tmp = table;
    table = prev_table;
    prev_table = tmp;
    // 更新界面
    updateUI();
}

// 播放
void HMultiView::play(HMedia& media)
{
    // 选中一个空闲播放器
    HVideoWidget* player = getIdlePlayer();
    // 当没有空闲播放器时弹窗提醒
    if(player == NULL)
    {
        QMessageBox::information(this, tr("Info"), tr("No spare player, please stop one and try again!"));
    }
    // 打开空闲播放器，播放指定媒体文件 
    else
    {
        player->open(media);
    }
}
