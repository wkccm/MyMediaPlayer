#ifndef HFRAME_H
#define HFRAME_H

#include "hbuf.h"
#include <deque>
#include <mutex>

class HFrame
{
public:
    // 缓存
    HBuf buf;
    // 宽
    int w;
    // 高
    int h;
    // bits per pixel
    int bpp;
    int type;
    uint64_t ts;
    int64_t useridx;
    void* userdata;

    HFrame()
    {
        w = h = bpp = type =0;
        ts = 0;
        useridx = -1;
        userdata = NULL;
    }

    bool isNull()
    {
        return w == 0 || h == 0 || buf.isNull();
    }

    void copy(const HFrame& rhs)
    {
        w = rhs.w;
        h = rhs.h;
        bpp = rhs.bpp;
        type = rhs.type;
        ts = rhs.ts;
        useridx = rhs.useridx;
        userdata = rhs.userdata;
        buf.copy(rhs.buf.base, rhs.buf.len);
    }
};

typedef struct frame_info_s
{
    int w;
    int h;
    int type;
    int bpp;
}FrameInfo;

// 记录帧缓冲区的进出数量
typedef struct frame_stats_s
{
    int push_cnt;
    int pop_cnt;
    int push_ok_cnt;
    int pop_ok_cnt;

    frame_stats_s()
    {
        push_cnt = pop_cnt = push_ok_cnt = pop_ok_cnt = 0;
    }
}FrameStats;

// 缓存帧数量
#define DEFAULT_FRAME_CACHENUM 10

class HFrameBuf : public HRingBuf
{
public:
    // 缓存满时策略
    // 挤出、丢弃
    enum CacheFullPolicy
    {
        SQUEEZE,
        DISCARD,
    }policy;

    // 默认压缩
    HFrameBuf():HRingBuf()
    {
        cache_num = DEFAULT_FRAME_CACHENUM;
        policy = SQUEEZE;
    }
    // 设置参数
    void setCache(int num)
    {
        cache_num = num;
    }

    void setPolicy(CacheFullPolicy policy)
    {
        this->policy = policy;
    }

    int push(HFrame *pFrame);
    int pop(HFrame* pFrame);
    void clear();

    // 缓冲区大小
    int                 cache_num;
    // 缓冲区出入数据
    FrameStats          frame_stats;
    // 帧信息
    FrameInfo           frame_info;
    // 帧队列
    std::deque<HFrame>  frames;
    // 线程，可以设置锁
    std::mutex          mutex;
};

#endif // HFRAME_H
