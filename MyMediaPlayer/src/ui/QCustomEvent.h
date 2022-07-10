#ifndef QCUSTOMEVENT_H
#define QCUSTOMEVENT_H

// 播放器状态
class QCustomEvent
{
public:
    enum Type
    {
        User = 10000,
        OpenMediaSucceed,
        OpenMediaFailed,
        PlayerEOF,
        PlayerError,
    };
};

#endif // QCUSTOMEVENT_H
