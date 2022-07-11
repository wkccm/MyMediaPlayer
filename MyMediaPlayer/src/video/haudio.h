#ifndef HAUDIO_H
#define HAUDIO_H

// 本头文件为ffmpeg接口
#include "HVideoPlayer.h"
#include "hthread.h"

#include "ffmpeg_util.h"

#define MAX_AUDIO_FRAME_SIZE 19200

class HAudio:public HVideoPlayer, public HThread
{
public:
    HAudio();
    ~HAudio(){};

    // 基类的纯虚函数要重写
    virtual int start() {
        flag = 0;
        quit = 0;
        // 启动线程
        return HThread::start();
    }

    virtual int stop() {
        quit = 1;
        return HThread::stop();
    }
    // 暂停
    virtual int pause() 
    {
        flag = 1;
        return HThread::pause();
    }
    // 恢复
    virtual int resume() 
    {
        flag = 0;
        return HThread::resume();
    }
    // 寻找指定位置
    virtual int seek(int64_t ms);
    // 刷新参数
    virtual void helper(){};

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


private:

    AVFormatContext*    pFormatCtx;
    // 编码
    AVCodecContext*     pCodecCtx ;
    AVCodecParameters*  pCodecParameters;
    AVCodec*            pCodec;
    // 数据包
    AVPacket* packet;
    // 视频音频数据
    AVFrame* frame;

    uint8_t* out_buffer;
    int64_t in_channel_layout;
    struct SwrContext *au_convert_ctx;

    // 音频流索引
    int audio_stream_index;

    int64_t start_time;
    int audio_time_base_num;
    int audio_time_base_den;
    int out_buffer_size;
    int quit;
    int flag;
};

#endif // HAUDIO_H
