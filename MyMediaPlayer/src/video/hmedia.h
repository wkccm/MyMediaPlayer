#ifndef HMEDIA_H
#define HMEDIA_H

#include <string>
#include "avdef.h"

typedef struct media_s
{
    media_type_e    type;
    std::string     src;
    std::string     desrc;
    int             index;

    media_s()
    {
        type = MEDIA_TYPE_NONE;
        index = -1;
    }
}HMedia;

#endif // HMEDIA_H
