#include "haudio.h"

#include "util/sdl_util.h"


static Uint8 *audio_chunk;
static Uint32 audio_len;
static Uint8 *audio_pos;

void read_audio_data(void *udata, Uint8 *stream, int len) {

    SDL_memset(stream, 0, len);
    if (audio_len == 0)
        return;
    len = (len > audio_len ? audio_len : len);

    SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);
    audio_pos += len;
    audio_len -= len;
}

HAudio::HAudio()
{
    pCodecParameters = NULL;
    pCodecCtx = NULL;
    pFormatCtx = NULL;
    pCodec = NULL;
    frame = NULL;
    quit = 0;
    flag = 0;
}

int HAudio::open()
{
    std::string file = media.src;

    if (avformat_open_input(&pFormatCtx, file.c_str(), NULL, NULL) != 0) {
        
        return -1; // Couldn't open file
    }

    audio_stream_index = av_find_best_stream(pFormatCtx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);

    if (audio_stream_index == -1) {
        
        return -1;// Didn't find a video stream
    }

    AVStream* audio_stream = pFormatCtx->streams[audio_stream_index];
    audio_time_base_num = audio_stream->time_base.num;
    audio_time_base_den = audio_stream->time_base.den;
    start_time = 0;

    if (audio_time_base_num && audio_time_base_den) {
        if (audio_stream->duration > 0) {
            duration = audio_stream->duration / (double)audio_time_base_den * audio_time_base_num * 1000;
        }
        if (audio_stream->start_time > 0) {
            start_time = audio_stream->start_time / (double)audio_time_base_den * audio_time_base_num * 1000;
        }
    }

    // 音频流参数
    pCodecParameters = pFormatCtx->streams[audio_stream_index]->codecpar;

    // 获取解码器
    pCodec = avcodec_find_decoder(pCodecParameters->codec_id);
    if (pCodec == NULL) {

        return -1; // Codec not found
    }

    // Copy context
    pCodecCtx = avcodec_alloc_context3(pCodec);
    if (avcodec_parameters_to_context(pCodecCtx, pCodecParameters) != 0) {

        return -1;// Error copying codec context
    }

    // Open codec
    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {

        return -1; // Could not open codec
    }

    
    return 0;
}


int HAudio::close()
{
    if(pFormatCtx)
    {
        avformat_close_input(&pFormatCtx);
        pFormatCtx = NULL;
    }
    if(pCodecCtx)
    {
        avcodec_close(pCodecCtx);
        avcodec_free_context(&pCodecCtx);
        pCodecCtx = NULL;
    }
    // if(pCodecParameters)
    // {
    //     avcodec_parameters_free(&pCodecParameters);
    //     pCodecParameters = NULL;
    // }
    if(pCodec)
    {
        pCodec = NULL;
    }
    if(packet)
    {
        av_packet_unref(packet);
        av_packet_free(&packet);
        packet = NULL;
    }
    if(frame)
    {
        av_frame_unref(frame);
        av_frame_free(&frame);
        frame = NULL;
    }
    if(au_convert_ctx)
    {
        swr_free(&au_convert_ctx);
    }


    return 0;
}

bool HAudio::doPrepare()
{
    int ret = open();
    return ret == 0;
}

void HAudio::doTask()
{
    packet = (AVPacket *) av_malloc(sizeof(AVPacket));
    av_init_packet(packet);
    frame = av_frame_alloc();

    uint64_t out_channel_layout = AV_CH_LAYOUT_STEREO;//输出声道
    int out_nb_samples = 1024;
    enum AVSampleFormat out_sample_fmt = AV_SAMPLE_FMT_S16;//输出格式S16
    int out_sample_rate = 44100;
    int out_channels = av_get_channel_layout_nb_channels(out_channel_layout);

    out_buffer_size = av_samples_get_buffer_size(NULL, out_channels, out_nb_samples, out_sample_fmt, 1);
    out_buffer = (uint8_t *) av_malloc(MAX_AUDIO_FRAME_SIZE * 2);

    //Init
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
        return;
    }

    SDL_AudioSpec spec;
    spec.freq = out_sample_rate;
    spec.format = AUDIO_S16SYS;
    spec.channels = out_channels;
    spec.silence = 0;
    spec.samples = out_nb_samples;
    spec.callback = read_audio_data;
    spec.userdata = pCodecCtx;

    if (SDL_OpenAudio(&spec, NULL) < 0) {
        printf("can't open audio.\n");
        return;
    }
    in_channel_layout = av_get_default_channel_layout(pCodecCtx->channels);

    au_convert_ctx = swr_alloc();
    au_convert_ctx = swr_alloc_set_opts(au_convert_ctx, out_channel_layout, out_sample_fmt, out_sample_rate,in_channel_layout, pCodecCtx->sample_fmt, pCodecCtx->sample_rate, 0, NULL);
    
    swr_init(au_convert_ctx);
    SDL_PauseAudio(0);
    while (!quit && av_read_frame(pFormatCtx, packet) >= 0) {
        if (packet->stream_index == audio_stream_index) {
            avcodec_send_packet(pCodecCtx, packet);
            while (!quit && avcodec_receive_frame(pCodecCtx, frame) == 0) {
                swr_convert(au_convert_ctx, &out_buffer, MAX_AUDIO_FRAME_SIZE, (const uint8_t **) frame->data,frame->nb_samples); // 转换音频
            }

            audio_chunk = (Uint8 *) out_buffer;
            audio_len = out_buffer_size;
            audio_pos = audio_chunk;
            while (audio_len > 0) {
                if(flag == 1)
                {
                    SDL_PauseAudio(1);
                }
                else
                {
                    SDL_PauseAudio(0);
                }
                SDL_Delay(1);//延迟播放
            }
        }
        av_packet_unref(packet);
    }
    SDL_Quit();
}

bool HAudio::doFinish()
{
    int ret = close();
    return ret == 0;
}
int HAudio::seek(int64_t ms)
{
    if(pFormatCtx)
    {
        return av_seek_frame(pFormatCtx, audio_stream_index, (start_time + ms) / 1000 / (double)audio_time_base_num * audio_time_base_den, AVSEEK_FLAG_BACKWARD);
    }
}
