//
// Created by jetson on 2021/3/2.
//

#ifndef FFMPEGDECODER_ACCOMPANY_DECODER_H
#define FFMPEGDECODER_ACCOMPANY_DECODER_H

#include "../../common/CommonTools.h"

extern "C" {
    #include "../../3rdparty/ffmpeg/include/libavcodec/avcodec.h"
    #include "../../3rdparty/ffmpeg/include/libavcodec/packet.h"
    #include "../../3rdparty/ffmpeg/include/libavformat/avformat.h"
    #include "../../3rdparty/ffmpeg/include/libavutil/avutil.h"
#include "../../3rdparty/ffmpeg/include/libavutil/mem.h"
    #include "../../3rdparty/ffmpeg/include/libavutil/samplefmt.h"
    #include "../../3rdparty/ffmpeg/include/libavutil/common.h"
    #include "../../3rdparty/ffmpeg/include/libavutil/channel_layout.h"
    #include "../../3rdparty/ffmpeg/include/libavutil/opt.h"
    #include "../../3rdparty/ffmpeg/include/libavutil/frame.h"
    #include "../../3rdparty/ffmpeg/include/libavutil/imgutils.h"
    #include "../../3rdparty/ffmpeg/include/libavutil/mathematics.h"
    #include "../../3rdparty/ffmpeg/include/libswscale/swscale.h"
    #include "../../3rdparty/ffmpeg/include/libswresample/swresample.h"
};

typedef struct AudioPacket {
    short * mbuffer;
    int msize;

    float mextra_param1;
    float mextra_param2;

    AudioPacket() {
        mbuffer = NULL;
        msize = 0;
        mextra_param1 = 0;
        mextra_param2 = 0;
    }

    ~AudioPacket() {
        if (NULL != mbuffer) {
            delete [] mbuffer;
            mbuffer = NULL;
        }
    }
} AudioPacket;

#define OUT_PUT_CHANNELS 2

class AccompanyDecoder {
private:

    AVFormatContext* mavFormatContext;
    AVCodecContext* mavCodecContext;
    int mstream_index;
    AVFrame* mpAudioFrame;
    AVPacket mpacket;

    char* macFilePath;

    int mpacketBufferSize;

    short *maudioBuffer;
    float mposition;
    int maudioBufferCursor;
    int maudioBufferSize;

    SwrContext *mswrContext;
    void *mswrBuffer;
    int mswrBufferSize;

    int readSamples(short* samples, int size);
    int readFrame();
    bool audioCodecIsSupported();

public:
    AccompanyDecoder();
    virtual ~AccompanyDecoder();

    virtual int getMusicMeta(const char* fileString, int * metaData);
    virtual int init(const char* mp3Path, const char * pcmPath);
    virtual AudioPacket* decodePacket();
    virtual void destroy();
};



#endif //FFMPEGDECODER_ACCOMPANY_DECODER_H
