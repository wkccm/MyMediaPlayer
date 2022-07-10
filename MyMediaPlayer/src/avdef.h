#ifndef AVDEF_H
#define AVDEF_H

// 详见FFmpeg的libavutil/pixfmt.h
// plannar线性存储，先存Y，然后存U，最后存V
// packed交叉存储
typedef enum
{
    PIX_FMT_NONE = 0,

    PIX_FMT_GRAY,

    PIX_FMT_YUV_FIRST = 100,
    PIX_FMT_YUV_PLANNAR_FIRST = 200,
    PIX_FMT_IYUV,
    PIX_FMT_YV12,
    PIX_FMT_NV12,
    PIX_FMT_NV21,
    PIX_FMT_YUV_PLANNAR_LAST,

    PIX_FMT_YUV_PACKED_FIRST = 300,
    PIX_FMT_YUY2,
    PIX_FMT_YVYU,
    PIX_FMT_UYVY,
    PIX_FMT_PACKED_LAST,
    PIX_FMT_YUV_LASY,

    PIX_FMT_RGB_FIRST=400,
    PIX_FMT_RGB,
    PIX_FMT_BGR,
    PIX_FMT_RGBA,
    PIX_FMT_BGRA,
    PIX_FMT_ARGB,
    PIX_FMT_ABGR,
    PIX_FMT_RGB_LAST,
}pix_fmt_e;

// YUV格式过多，用函数判断
static inline bool pix_fmt_is_yuv(int type)
{
    return type > PIX_FMT_YUV_FIRST && type < PIX_FMT_YUV_PLANNAR_LAST;
}

// 其余格式switch判断
static inline int pix_fmt_bpp(int type)
{
    if(pix_fmt_is_yuv(type))
    {
        return 12;
    }
    switch(type)
    {
    case PIX_FMT_RGB:
    case PIX_FMT_BGR:
        return 24;
    case PIX_FMT_RGBA:
    case PIX_FMT_BGRA:
    case PIX_FMT_ARGB:
    case PIX_FMT_ABGR:
        return 32;
    case PIX_FMT_GRAY:
        return 8;
    }
    return 0;
}

// 媒体文件类型
typedef enum
{
    MEDIA_TYPE_FILE = 0,
    MEDIA_TYPE_NETWORK,
    MEDIA_TYPE_NB,
}media_type_e;

// 默认捕获
#define DEFAULT_MEDIA_TYPE  MEDIA_TYPE_FILE
#define MEDIA_TYPE_NONE     MEDIA_TYPE_NB

// av格式
typedef enum
{
    AVSTREAM_TYPE_VIDEO,
    AVSTREAM_TYPE_AUDIO,
    AVSTREAM_TYPE_SUBTITLE,
    AVSTREAM_TYPE_NB,
}avstream_type_e;

// 显示类型
typedef enum
{
    ASPECT_FULL,
    ASPECT_PERCENT,
    ASPECT_ORIGINAL_RATIO,
    ASPECT_ORIGINAL_SIZE,
    ASPECT_CUSTOM_RATIO,
    ASPECT_CUSTOM_SIZE,
}aspect_ratio_e;

#define DEFAULT_ASPECT_RATIO

// 显示类型及长宽
typedef struct aspect_ratio_s
{
    aspect_ratio_e type;
    int w,h;
}aspect_ratio_t;

#endif // AVDEF_H
