#ifndef HVIDEOPLAYERFACTORY_H
#define HVIDEOPLAYERFACTORY_H

#include "hmedia.h"
#include "HVideoPlayer.h"
#include "hffplayer.h"
#include "haudio.h"

// 根据媒体类型创建播放器
class HVideoPlayerFactory
{
public:
    static HVideoPlayer* create(media_type_e type)
    {
        switch(type)
        {
        case MEDIA_TYPE_FILE:
        case MEDIA_TYPE_NETWORK:
            return new HFFPlayer;
        default:
            return NULL;
        }
    }

    static HVideoPlayer* create_audio(media_type_e type)
    {
        switch(type)
        {
        case MEDIA_TYPE_FILE:
        case MEDIA_TYPE_NETWORK:
            return new HAudio;
        default:
            return NULL;
        }
    }
};

#endif // HVIDEOPLAYERFACTORY_H
