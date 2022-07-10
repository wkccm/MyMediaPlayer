#ifndef HMEDIA_H
#define HMEDIA_H

#include <string>
#include "avdef.h"

typedef struct media_s
{
    // 媒体文件类型
    media_type_e    type;
    // 文件路径
    std::string     src;
    int             index;

    media_s()
    {
        type = MEDIA_TYPE_NONE;
        index = -1;
    }
}HMedia;

#endif // HMEDIA_H
