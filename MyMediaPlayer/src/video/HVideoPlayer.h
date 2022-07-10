#ifndef HVIDEOPLAYER_H
#define HVIDEOPLAYER_H

#include "confile.h"
#include "hframe.h"
#include "hmedia.h"

// 默认帧数
// 默认缓存帧数
#define DEFAULT_FPS         25
#define DEFAULT_FRAME_CACHE 5

// 软件编码、硬件QSV编码、硬件CUVID编码
enum
{
    SOFTWARE_DECODE         =1,
    HARDWARE_DECODE_QSV     =2,
    HARDWARE_DECODE_CUVID   =3,
};
// 默认解码模式
#define DEFAULT_DECODE_MODE HARDWARE_DECODE_CUVID

// 播放器事件
// 打开失败、打开成功、文件结束、关闭、错误
enum hplayer_event_e
{
    HPLAYER_OPEN_FAILED,
    HPLAYER_OPENED,
    HPLAYER_EOF,
    HPLAYER_CLOSED,
    HPLAYER_ERROR,
};

// 定义播放器事件句柄，接受播放器事件和用户数据作为参数
typedef int (*hplayer_event_cb)(hplayer_event_e e, void* userdata);

class HVideoPlayer
{
public:
    // 初始化播放器时设置缓存帧数
    HVideoPlayer()
    {
        set_frame_cache(g_confile->Get<int>("frame_cache", "video", DEFAULT_FRAME_CACHE));
    }

    virtual ~HVideoPlayer(){}

    // 纯虚函数
    virtual int start() = 0;
    virtual int stop() = 0;
    virtual int pause() = 0;
    virtual int resume() = 0;

    virtual void helper() = 0;

    virtual int seek(int64_t ms)
    {
        return 0;
    }

    // 设置各参数
    void set_media(HMedia& media)
    {
        this->media = media;
    }
    void set_decode_mode(int mode)
    {
        decode_mode = mode;
    }
    void set_frame_cache(int cache)
    {
        frame_buf.setCache(cache);
    }
    // 获得各参数
    FrameStats get_frame_stats()
    {
        return frame_buf.frame_stats;
    }
    FrameInfo get_frame_info()
    {
        return frame_buf.frame_info;
    }
    // 清缓存
    void clear_frame_cache()
    {
        frame_buf.clear();
    }
    // 缓冲区进一帧
    int push_frame(HFrame* pFrame)
    {
        return frame_buf.push(pFrame);
    }
    // 缓冲区出一帧
    int pop_frame(HFrame* pFrame)
    {
        return frame_buf.pop(pFrame);
    }
    // 设置回调函数
    void set_event_callback(hplayer_event_cb cb, void* userdata)
    {
        event_cb = cb;
        event_cb_userdata = userdata;
    }
    // 触发回调
    void event_callback(hplayer_event_e e)
    {
        if(event_cb)
        {
            event_cb(e, event_cb_userdata);
        }
    }

public:
    // 媒体文件
    HMedia      media;
    // 帧率
    int         fps;
    // 解码模式
    int         decode_mode;
    // 实际解码模式
    int         real_decode_mode;
    // 宽
    int32_t     width;
    // 高
    int32_t     height;
    // 持续时长
    int64_t     duration;
    // 开始时间
    int64_t     start_time;
    // 文件结束标志
    int         eof;
    // 文件错误标志
    int         error;
protected:
    // 缓冲区
    HFrameBuf           frame_buf;
    // 回调函数
    hplayer_event_cb    event_cb;
    // 用户数据
    void*               event_cb_userdata;

};

#endif // HVIDEOPLAYER_H
