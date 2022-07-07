#ifndef HFFPLAYER_H
#define HFFPLAYER_H

#include "HVideoPlayer.h"
#include "hthread.h"

#include "ffmpeg_util.h"

class HFFPlayer:public HVideoPlayer, public HThread
{
public:
    HFFPlayer();
    ~HFFPlayer();

    // 基类的纯虚函数要重写
    virtual int start() {
        quit = 0;
        return HThread::start();
    }
    virtual int stop() {
        quit = 1;
        return HThread::stop();
    }
    virtual int pause() {return HThread::pause();}
    virtual int resume() {return HThread::resume();}

    virtual int seek(int64_t ms);

    virtual void helper()
    {
        open();
    }
    
private:
    virtual bool doPrepare();
    virtual void doTask();
    virtual bool doFinish();

    int open();
    int close();

public:
    int64_t block_starttime;
    int64_t block_timeout;
    int     quit;

private:
    static std::atomic_flag s_ffmpeg_init;

    AVDictionary*       fmt_opts;
    AVDictionary*       codec_opts;

    AVFormatContext*    fmt_ctx;
    AVCodecContext*     codec_ctx;

    AVPacket* packet;
    AVFrame* frame;

    int video_stream_index;
    int audio_stream_index;
    int subtitle_stream_index;

    int video_time_base_num;
    int video_time_base_den;

    AVPixelFormat   src_pix_fmt;
    AVPixelFormat   dst_pix_fmt;
    SwsContext*     sws_ctx;
    uint8_t*        data[4];
    int             linesize[4];
    HFrame          hframe;
};

#endif // HFFPLAYER_H
