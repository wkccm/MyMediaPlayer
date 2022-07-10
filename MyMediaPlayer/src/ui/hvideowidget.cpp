#include "hvideowidget.h"
#include "QCustomEvent.h"
#include "hlog.h"
#include <QApplication>
#include "qtfunctions.h"
#include "hopenmediadlg.h"
#include "qtstyles.h"
#include <QMessageBox>
#include "HVideoPlayer.h"
#include "HVideoPlayerFactory.h"
#include "htime.h"

// 设置重试默认参数
#define DEFAULT_RETRY_INTERVAL  10000
#define DEFAULT_RETRY_MAXCNT    6

// 回调函数
static int hplayer_event_callback(hplayer_event_e e, void* userdata)
{
    // 将用户数据转换成视频组件
    HVideoWidget* wdg = (HVideoWidget*)userdata;
    // 初始化事件
    int custom_event_type = QCustomEvent::User;
    // 已打开、打开失败、播放结束、播放出错
    switch(e)
    {
    case HPLAYER_OPENED:
        custom_event_type = QCustomEvent::OpenMediaSucceed;
        break;
    case HPLAYER_OPEN_FAILED:
        custom_event_type = QCustomEvent::OpenMediaFailed;
        break;
    case HPLAYER_EOF:
        custom_event_type = QCustomEvent::PlayerEOF;
        break;
    case HPLAYER_ERROR:
        custom_event_type = QCustomEvent::PlayerError;
        break;
    default:
        return 0;
    }
    hlogi("postEvent %d", custom_event_type);
    // 向事件队列中添加事件
    QApplication::postEvent(wdg, new QEvent((QEvent::Type)custom_event_type));
    return 0;
}

// 返回渲染类型
static renderer_type_e renderer_type_enum(const char* str)
{
    if(stricmp(str, "sdl") == 0 || stricmp(str, "sdl2"))
    {
        return RENDERER_TYPE_SDL2;
    }

    return DEFAULT_RENDER_TYPE;
}

// 初始化
HVideoWidget::HVideoWidget(QWidget* parent) :QFrame(parent)
{
    // 视频id
    playerid = 0;
    // 当前播放状态
    status = STOP;
    // 播放器对象
    pImpl_player = NULL;
    // 从配置文件中读取fps
    fps = g_confile->Get<int>("fps", "video");
    // 调整显示比例
    std::string str = g_confile->GetValue("aspect_ratio", "video");
    const char* c_str = str.c_str();
    aspect_ratio.type = ASPECT_FULL;
    if(str.empty() || strcmp(c_str, "100%"))
    {
        aspect_ratio.type = ASPECT_FULL;
    }
    else if(stricmp(c_str, "w:h") == 0)
    {
        aspect_ratio.type = ASPECT_ORIGINAL_RATIO;
    }
    else if(stricmp(c_str, "wxh") == 0 || stricmp(c_str, "w*h") == 0)
    {
        aspect_ratio.type = ASPECT_ORIGINAL_SIZE;
    }
    else if(strchr(c_str, '%'))
    {
        int percent = 0;
        sscanf(c_str, "%d%%", &percent);
        if(percent)
        {
            aspect_ratio.type = ASPECT_PERCENT;
            aspect_ratio.w = percent;
            aspect_ratio.h = percent;
        }
    }
    else if(strchr(c_str, ':'))
    {
        int w =0;
        int h =0;
        sscanf(c_str, "%d:%d", &w, &h);
        if(w && h)
        {
            aspect_ratio.type = ASPECT_CUSTOM_RATIO;
            aspect_ratio.w = w;
            aspect_ratio.h = h;
        }
    }
    else if(strchr(c_str, 'x'))
    {
        int w = 0;
        int h = 0;
        sscanf(c_str, "%dx%d", &w, &h);
        if(w && h)
        {
            aspect_ratio.type = ASPECT_CUSTOM_SIZE;
            aspect_ratio.w = w;
            aspect_ratio.h = h;
        }
    }
    else if(strchr(c_str, 'X'))
    {
        int w = 0;
        int h = 0;
        sscanf(c_str, "%dX%d", &w, &h);
        if(w && h)
        {
            aspect_ratio.type = ASPECT_CUSTOM_SIZE;
            aspect_ratio.w = w;
            aspect_ratio.h = h;
        }
    }
    else if(strchr(c_str, '*'))
    {
        int w = 0;
        int h = 0;
        sscanf(c_str, "%d*%d", &w, &h);
        if(w && h)
        {
            aspect_ratio.type = ASPECT_CUSTOM_SIZE;
            aspect_ratio.w = w;
            aspect_ratio.h = h;
        }
    }
    else
    {
        aspect_ratio.type = ASPECT_FULL;
    }
    hlogd("aspect_ratio type=%d w=%d h=%d", aspect_ratio.type, aspect_ratio.w, aspect_ratio.h);
    // 设置渲染类型
    str = g_confile->GetValue("renderer", "video");
    if(str.empty())
    {
        renderer_type = RENDERER_TYPE_SDL2;
    }
    else
    {
        renderer_type = renderer_type_enum(str.c_str());
    }
    // 设置重试规则
    retry_interval = g_confile->Get<int>("retry_interval", "video", DEFAULT_RETRY_INTERVAL);
    retry_maxcnt = g_confile->Get<int>("retry_maxcnt", "video", DEFAULT_RETRY_MAXCNT);
    last_retry_time = 0;
    retry_cnt = 0;

    initUI();
    initConnect();
}

HVideoWidget::~HVideoWidget()
{
    hlogd("~HVideoWidget");
    close();
}

void HVideoWidget::initUI()
{
    // 设置窗口获取焦点类型
    setFocusPolicy(Qt::ClickFocus);
    // 根据渲染类型创建窗口
    videownd = HVideoWndFactory::create(renderer_type, this);
    // 新建窗口标题
    titlebar = new HVideoTitleBar(this);
    // 新建窗口工具栏
    toolbar = new HVideoToolBar(this);
    // 新建媒体选择按钮
    btnMedia = pushButton(QPixmap(":/image/media_bk.png"), tr("Open media"));
    // 垂直显示
    QVBoxLayout* vbox = VBoxLayout();
    vbox->addWidget(titlebar, 0, Qt::AlignTop);
    vbox->addWidget(btnMedia, 0, Qt::AlignCenter);
    vbox->addWidget(toolbar, 0, Qt::AlignBottom);
    setLayout(vbox);
    // 组件显示
    titlebar->show();
    toolbar->show();
}

void HVideoWidget::initConnect()
{
    // 媒体选择按钮为打开浏览窗口
    connect(btnMedia, &QPushButton::clicked, [this]{
        HOpenMediaDlg dlg(this);
        if(dlg.exec() == QDialog::Accepted)
        {
            open(dlg.media);
        }
    });
    // 关联按钮
    connect( titlebar->btnClose, SIGNAL(clicked(bool)), this, SLOT(close()));
    connect( toolbar, SIGNAL(sigStart()), this, SLOT(start()));
    connect( toolbar, SIGNAL(sigPause()), this, SLOT(pause()));
    connect( toolbar, SIGNAL(sigStop()),  this, SLOT(stop()));
    // 拖拽滚动条，更新当前进度
    connect( toolbar->sldProgress, &QSlider::sliderReleased, [this]() {
        if (pImpl_player)
        {
            pImpl_player->seek(toolbar->sldProgress->value()*1000);
        }
    });
    // 时间
    timer = new QTimer(this);
    // 精确时间
    timer->setTimerType(Qt::PreciseTimer);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimerUpdate()));
}

void HVideoWidget::updateUI()
{
    // 设置标题为当前id+文件标题
    titlebar->labTitle->setText(QString::asprintf("%02d ", playerid) + title);
    // 初始时设置开始播放按钮可见
    toolbar->btnStart->setVisible(status != PLAY);
    // 初始时设置暂停播放按钮不可见
    toolbar->btnPause->setVisible(status == PLAY);
    // 只有当状态为停止时显示媒体文件选择按钮
    btnMedia->setVisible(status == STOP);
    // 停止状态时不显示进度条
    if(status == STOP)
    {
        toolbar->sldProgress->hide();
        toolbar->lblDuration->hide();
    }
    // 播放时显示进度条
    else if(status == PLAY)
    {
        toolbar->sldProgress->show();
        toolbar->lblDuration->show();
    }
}

// 设置长宽比
void HVideoWidget::resizeEvent(QResizeEvent* e)
{
    setAspectRatio(aspect_ratio);
}

// 
void HVideoWidget::enterEvent(QEvent* e)
{
    updateUI();
    titlebar->show();
    toolbar->show();
}

void HVideoWidget::leaveEvent(QEvent* e)
{
//    titlebar->hide();
//    toolbar->hide();
}

// 记录点击位置
void HVideoWidget::mousePressEvent(QMouseEvent* e)
{
    ptMousePress = e->pos();
#if WITH_MV_STYLE
    e->ignore();
#endif
}

void HVideoWidget::mouseReleaseEvent(QMouseEvent* e)
{
#if WITH_MV_STYLE
    e->ignore();
#endif
}

void HVideoWidget::mouseMoveEvent(QMouseEvent* e)
{
#if WITH_MV_STYLE
    e->ignore();
#endif
}

// 根据事件调用函数
void HVideoWidget::customEvent(QEvent* e)
{
    switch(e->type())
    {
    case QCustomEvent::OpenMediaSucceed:
        onOpenSucceed();
        break;
    case QCustomEvent::OpenMediaFailed:
        onOpenFailed();
        break;
    case QCustomEvent::PlayerEOF:
        onPlayerEOF();
        break;
    case QCustomEvent::PlayerError:
        onPlayerError();
        break;
    default:
        break;
    }
}

// 打开媒体
void HVideoWidget::open(HMedia& media)
{
    // 设置媒体
    this->media = media;
    // 调用start
    start();
}

// 关闭媒体
void HVideoWidget::close()
{
    // 先停止
    stop();
    // 置空媒体类型
    this->media.type = MEDIA_TYPE_NONE;
    // 置空媒体标题
    title = "";
    // 更新界面
    updateUI();
}

// 开始播放媒体
void HVideoWidget::start()
{
    // 若当前媒体类型为空
    if (media.type == MEDIA_TYPE_NONE)
    {
        // 提示没有媒体文件
        QMessageBox::information(this, tr("Info"), tr("Please first set media source, then start."));
        // 更新界面
        updateUI();
        return;
    }
    // 如果播放器为空
    if (!pImpl_player)
    {
        // 创建一个指定类型的新播放器
        pImpl_player = HVideoPlayerFactory::create(media.type);
        // 设置媒体文件
        pImpl_player->set_media(media);
        // 设置回调函数
        pImpl_player->set_event_callback(hplayer_event_callback, this);
        // 设置标题
        title = media.src.c_str();
        // 在更新界面之前刷新参数
        pImpl_player->helper();
        // 更新界面
        updateUI();
        // 新建一个线程并开始
        int ret = pImpl_player->start();
        if (ret != 0)
        {
            onOpenFailed();
        }
        else
        {
            onOpenSucceed();
        }
        updateUI();
    }
    // 如果是暂停状态，则继续
    else
    {
        if (status == PAUSE)
        {
            resume();
        }
    }
}

// 停止
void HVideoWidget::stop() {
    // 定时器停止
    timer->stop();
    // 如果播放器存在
    if (pImpl_player) {
        // 停止播放器
        pImpl_player->stop();
        // 释放空间
        SAFE_DELETE(pImpl_player);
    }
    // 清除上一帧
    videownd->last_frame.buf.cleanup();
    // 更新绘图
    videownd->update();
    // 切换状态
    status = STOP;
    // 上次重试时间归零
    last_retry_time = 0;
    // 重试次数归零
    retry_cnt = 0;
    // 更新界面
    updateUI();
}

// 暂停
void HVideoWidget::pause() {
    // 若有播放器则暂停
    if (pImpl_player) {
        pImpl_player->pause();
    }
    // 定时器停止
    timer->stop();
    // 切换状态
    status = PAUSE;
    // 更新界面
    updateUI();
}

// 继续
void HVideoWidget::resume() {
    // 如果当前时暂停状态且有播放器
    if (status == PAUSE && pImpl_player) {
        // 播放器继续
        pImpl_player->resume();
        // 定时器开始
        timer->start(1000 / (fps ? fps : pImpl_player->fps));
        // 切换状态
        status = PLAY;
        // 更新界面
        updateUI();
    }
}

// 重启
void HVideoWidget::restart() {
    hlogi("restart...");
    if (pImpl_player) {
        pImpl_player->stop();
        pImpl_player->start();
    }
    else {
        start();
    }
}

// 重试
void HVideoWidget::retry() {
    // 当重试次数未用尽时
    if (retry_maxcnt < 0 || retry_cnt < retry_maxcnt) {
        // 重试次数加1
        ++retry_cnt;
        // 获取当前时间
        int64_t cur_time = gettimeofday_ms();
        // 距上一次尝试的时间间隔
        int64_t timespan = cur_time - last_retry_time;
        // 若超时则再次重试
        if (timespan >= retry_interval) {
            last_retry_time = cur_time;
            restart();
        }
        // 若没有超时
        else {
            // 更新上次尝试时间
            last_retry_time += retry_interval;
            // 如果播放器可用，则停止
            if (pImpl_player) {
                pImpl_player->stop();
            }
            // 计算等待时间
            int retry_after = retry_interval - timespan;
            hlogi("retry after %dms", retry_after);
            // 等待时间后重启
            QTimer::singleShot(retry_after, this, SLOT(restart()));
        }
    }
    // 重试次数用尽时直接停止
    else {
        stop();
    }
}

// 成功打开
void HVideoWidget::onOpenSucceed() {
    // 定时器
    timer->start(1000 / (fps ? fps : pImpl_player->fps));
    // 切换状态
    status = PLAY;
    // 调整显示比例
    setAspectRatio(aspect_ratio);
    // 若持续时长存在
    if (pImpl_player->duration > 0) {
        // 转换为秒数
        int duration_sec = pImpl_player->duration / 1000;
        // 转换格式
        char szTime[16];
        duration_fmt(duration_sec, szTime);
        // 将转换后的格式赋给进度条文本
        toolbar->lblDuration->setText(szTime);
        // 设置进度条长度
        toolbar->sldProgress->setRange(0, duration_sec);
        // 显示
        toolbar->lblDuration->show();
        toolbar->sldProgress->show();
    }
    // 若重试次数不为0，则显示重试成功
    if (retry_cnt != 0) {
        hlogi("retry succeed: cnt=%d media.src=%s", retry_cnt, media.src.c_str());
    }
}

// 打开失败
void HVideoWidget::onOpenFailed() {
    // 重试次数为0时
    if (retry_cnt == 0) {
        // 提醒打开失败
        QMessageBox::critical(this, tr("ERROR"), tr("Could not open media: \n") +
                              media.src.c_str() +
                              QString::asprintf("\nerrcode=%d", pImpl_player->error));
        stop();
    }
    // 不为0时显示重试失败
    else {
        hlogw("retry failed: cnt=%d media.src=%s", retry_cnt, media.src.c_str());
        retry();
    }
}

// 播放至文件结束
void HVideoWidget::onPlayerEOF() {
    switch (media.type) {
    case MEDIA_TYPE_NETWORK:
        retry();
        break;
    case MEDIA_TYPE_FILE:
        // 如果配置文件中设置循环播放，则重新播放
        if (g_confile->Get<bool>("loop_playback", "video")) {
            restart();
        }
        else {
            stop();
        }
        break;
    default:
        stop();
        break;
    }
}

// 播放出错
void HVideoWidget::onPlayerError() {
    switch (media.type) {
    case MEDIA_TYPE_NETWORK:
        retry();
        break;
    default:
        stop();
        break;
    }
}

// 定时器更新
void HVideoWidget::onTimerUpdate() {
    if (pImpl_player == NULL)   return;
    // 帧
    if (pImpl_player->pop_frame(&videownd->last_frame) == 0) {
        // 若滚动条可见
        if (toolbar->sldProgress->isVisible()) {
            // 进度更新
            int progress = (videownd->last_frame.ts - pImpl_player->start_time) / 1000;
            // 滑动条没有被按下时
            if (toolbar->sldProgress->value() != progress &&
                !toolbar->sldProgress->isSliderDown()) {
                toolbar->sldProgress->setValue(progress);
            }
        }
        // 更新视频帧
        videownd->update();
    }
}

// 设置界面比例
void HVideoWidget::setAspectRatio(aspect_ratio_t ar) {
    aspect_ratio = ar;
    int border = 1;
    int scr_w = width() - border * 2;
    int scr_h = height() - border * 2;
    if (scr_w <= 0 || scr_h <= 0) return;
    int pic_w = 0;
    int pic_h = 0;
    if (pImpl_player && status != STOP) {
        pic_w = pImpl_player->width;
        pic_h = pImpl_player->height;
    }
    if (pic_w == 0) pic_w = scr_w;
    if (pic_h == 0) pic_h = scr_h;
    // calc videownd rect
    int dst_w=0, dst_h=0;
    switch (ar.type) {
    case ASPECT_FULL:
        dst_w = scr_w;
        dst_h = scr_h;
        break;
    case ASPECT_PERCENT:
        dst_w = pic_w * ar.w / 100;
        dst_h = pic_h * ar.h / 100;
        break;
    case ASPECT_ORIGINAL_SIZE:
        dst_w = pic_w;
        dst_h = pic_h;
        break;
    case ASPECT_CUSTOM_SIZE:
        dst_w = ar.w;
        dst_h = ar.h;
        break;
    case ASPECT_ORIGINAL_RATIO:
    case ASPECT_CUSTOM_RATIO:
    {
        double scr_ratio = (double)scr_w / (double)scr_h;
        double dst_ratio = 1.0;
        if (ar.type == ASPECT_CUSTOM_RATIO) {
            dst_ratio = (double)ar.w / (double)ar.h;
        }
        else {
            dst_ratio = (double)pic_w / (double)pic_h;
        }
        if (dst_ratio > scr_ratio) {
            dst_w = scr_w;
            dst_h = scr_w / dst_ratio;
        }
        else {
            dst_h = scr_h;
            dst_w = scr_h * dst_ratio;
        }
    }
        break;
    }
    dst_w = MIN(dst_w, scr_w);
    dst_h = MIN(dst_h, scr_h);
    // align 4
    dst_w = dst_w >> 2 << 2;
    dst_h = dst_h >> 2 << 2;

    int x = border + (scr_w - dst_w) / 2;
    int y = border + (scr_h - dst_h) / 2;
    videownd->setGeometry(QRect(x, y, dst_w, dst_h));
}
