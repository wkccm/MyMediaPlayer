#ifndef HFFPLAYER_H
#define HFFPLAYER_H

// 本头文件为ffmpeg接口
#include "HVideoPlayer.h"
#include "hthread.h"

#include "ffmpeg_util.h"

// 继承播放器、线程
class HFFPlayer:public HVideoPlayer, public HThread
{
public:
    HFFPlayer();
    ~HFFPlayer();

    // 基类的纯虚函数要重写
    virtual int start() {
        quit = 0;
        // 启动线程
        return HThread::start();
    }
    virtual int stop() {
        quit = 1;
        return HThread::stop();
    }
    // 暂停
    virtual int pause() {return HThread::pause();}
    // 恢复
    virtual int resume() {return HThread::resume();}
    // 寻找指定位置
    virtual int seek(int64_t ms);
    // 刷新参数
    virtual void helper()
    {
        // 读一遍媒体参数
        open();
    }
    
private:
    // 准备工作
    virtual bool doPrepare();
    // 执行
    virtual void doTask();
    // 执行结束
    virtual bool doFinish();
    // 打开媒体
    int open();
    // 关闭媒体
    int close();

public:
    // 阻塞开始时间
    int64_t block_starttime;
    // 阻塞超时
    int64_t block_timeout;
    // 退出标志位
    int     quit;

private:
    // 原子
    static std::atomic_flag s_ffmpeg_init;
    // 字典
    AVDictionary*       fmt_opts;
    AVDictionary*       codec_opts;
    // 输入输出流
    AVFormatContext*    fmt_ctx;
    // 编码
    AVCodecContext*     codec_ctx;
    // 数据包
    AVPacket* packet;
    // 视频音频数据
    AVFrame* frame;
    // 视频流索引
    int video_stream_index;
    // 音频流索引
    int audio_stream_index;
    // 字幕流索引
    int subtitle_stream_index;
    // 视频流时间单位分母
    int video_time_base_num;
    // 视频流时间单位分子
    int video_time_base_den;
    // 源像素格式
    AVPixelFormat   src_pix_fmt;
    // 目标像素格式
    AVPixelFormat   dst_pix_fmt;
    SwsContext*     sws_ctx;
    uint8_t*        data[4];
    int             linesize[4];
    HFrame          hframe;
};

#endif // HFFPLAYER_H
