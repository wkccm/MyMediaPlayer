#include "hframe.h"
#include "hlog.h"

// 防止缓冲区信息异常，三种操作加锁

int HFrameBuf::push(HFrame* pFrame)
{
    // 帧无效，返回
    if(pFrame->isNull())
    {
        return -10;
    }
    // 试图向缓冲区内添加一帧
    ++frame_stats.push_cnt;

    // 锁，出生存区自动解锁
    std::lock_guard<std::mutex> locker(mutex);

    // 缓存区满时，写日志
    if(frames.size() >= (size_t)cache_num)
    {
        hlogd("frame cache full!");
        // 如果策略是丢弃，则直接返回
        if(policy == HFrameBuf::DISCARD)
        {
            return -20;
        }
        // 策略是挤出，缓冲队列第一个需要被弹出
        HFrame& frame = frames.front();
        frames.pop_front();
        free(frame.buf.len);
        if(frame.userdata)
        {
            hlogd("free userdata");
            // 默认命名空间，否则free冲突
            ::free(frame.userdata);
            frame.userdata = NULL;
        }
    }

    // 此时缓冲区一定还有空间
    int ret = 0;
    if(isNull())
    {
        resize(pFrame->buf.len * cache_num);
        ret = 1;
        frame_info.w = pFrame->w;
        frame_info.h = pFrame->h;
        frame_info.type = pFrame->type;
        frame_info.bpp = pFrame->bpp;
    }

    // 本质上是实现实参的深拷贝
    // 结束后确认添加
    HFrame frame;
    frame.buf.base = alloc(pFrame->buf.len);
    frame.buf.len = pFrame->buf.len;
    frame.copy(*pFrame);
    frames.push_back(frame);
    ++frame_stats.push_ok_cnt;

    return ret;
}

// 从缓冲区拿出一帧
int HFrameBuf::pop(HFrame* pFrame)
{
    ++frame_stats.pop_cnt;

    std::lock_guard<std::mutex> locker(mutex);

    if(isNull())
    {
        return -10;
    }

    if(frames.size() == 0)
    {
        hlogd("frame cache empty!");
        return -20;
    }

    HFrame& frame = frames.front();
    frames.pop_front();
    free(frame.buf.len);

    if(frame.isNull())
    {
        return -30;
    }

    pFrame->copy(frame);
    ++frame_stats.pop_ok_cnt;

    return 0;
}

// 清除缓冲区
void HFrameBuf::clear()
{
    std::lock_guard<std::mutex> locker(mutex);
    frames.clear();
    HRingBuf::clear();
}
