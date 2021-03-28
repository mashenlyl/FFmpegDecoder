#ifndef ACCOMPANY_DECODER_CONTROLLER_H
#define ACCOMPANY_DECODER_CONTROLLER_H

#include <unistd.h>
#include "accompany_decoder.h"

#define CHANNEL_PER_FRAME 2
#define BITS_PER_CHANNEL 16
#define BITS_PER_BYTE 8

class AccompanyDecoderController {
protected:
    FILE* mpcmFile;
    AccompanyDecoder* macDecoder;

    int macSampleRate;
    int macPacketBufferSize;
public:
    AccompanyDecoderController();
    ~AccompanyDecoderController();

    int Init(const char* acPath, const char* pcmFilePath);
    void Decoder();
    void Destroy();

};





#endif