//
// Created by jetson on 2021/3/2.
//

#include "accompany_decoder.h"

#define MAX_AUDIO_FRAME_SIZE 192000
#define LOG_TAG "liyaoliAcDecoder"

AccompanyDecoder::AccompanyDecoder() {
    macFilePath = NULL;
}

AccompanyDecoder::~AccompanyDecoder() {
    if (NULL != macFilePath) {
        delete [] macFilePath;
        macFilePath = NULL;
    }
}

int AccompanyDecoder::getMusicMeta(const char *fileString, int *metaData) {
    int ret = 0;
    LOGI("getMusicMeta init success");
    if (ret < 0) {
        return -1;
    }
    int sampleRate = mavCodecContext->sample_rate;
    LOGI("sampleRate is %d", sampleRate);
    int bitRate = mavCodecContext->bit_rate;
    LOGI("bitRate is %d", bitRate);
    destroy();
    metaData[0] = sampleRate;
    metaData[1] = bitRate;
    return 0;
}

int AccompanyDecoder::init(const char *fileString, const char* pcmPath) {
    LOGI("enter AccompanyDecoder::init");
    maudioBuffer = NULL;
    mposition = -1.0f;
    maudioBufferCursor = 0;
    maudioBufferSize = 0;
    mswrContext = NULL;
    mswrBuffer = NULL;
    mswrBufferSize = 0;
    uint8_t *out_buffer;
    int index = 0;
    AVSampleFormat out_sample_fmt=AV_SAMPLE_FMT_S16;

    FILE * pcmFile = fopen(pcmPath, "wb+");
    // 注册解码器
    avcodec_register_all();
    av_register_all();
    mavFormatContext = avformat_alloc_context();
    LOGI("open ac file %s...", fileString);

    // 打开文件，并解析文件，然后填充avformat
    int result = avformat_open_input(&mavFormatContext, fileString, NULL, NULL);
    if (result != 0) {
        LOGI("can't open file %s result %s", fileString, av_err2str(result));
        return -1;
    } else {
        LOGI("open file %s success and result is %s", fileString, av_err2str(result));
    }

    // 检查文件中的流信息
    result = avformat_find_stream_info(mavFormatContext, NULL);
    if (result < 0) {
        LOGI("fail avformat avformat_find_stream_info %s result %s", fileString, av_err2str(result));
        return -1;
    } else {
        LOGI("avformat_find_stream_info success result is %s", fileString, av_err2str(result));
    }
    mstream_index = av_find_best_stream(mavFormatContext, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
    LOGI("stream_index is %d", mstream_index);
    if (mstream_index == -1) {
        LOGI("no audio stream");
        return -1;
    }
    AVStream* audioStream = mavFormatContext->streams[mstream_index];
    //if (audioStream->time_base.den && audioStream->time_base.num)
    mavCodecContext = audioStream->codec;
    LOGI("avCodecContext->codec_id is %d AV_CODEC_ID_AAC is %d", mavCodecContext->codec_id, AV_CODEC_ID_AAC);
    AVCodec* avCodec = avcodec_find_decoder(mavCodecContext->codec_id);
    if (avCodec == NULL) {
        LOGI("Unsupported codec");
        return -1;
    }
    result = avcodec_open2(mavCodecContext, avCodec, NULL);
    if (result < 0) {
        LOGI("avcodec_open2 fail avformat_find_stream_info result is %s", av_err2str(result));
        return -1;
    } else {
        LOGI("avcodec_open2 sucess avformat_find_stream_info result is %s", av_err2str(result));
    }
    if (!audioCodecIsSupported()) {
        LOGI("because of audio Codec Is Not Supported so we will init swresampler...");
        mswrContext = swr_alloc();
        mswrContext = swr_alloc_set_opts(mswrContext, av_get_default_channel_layout(OUT_PUT_CHANNELS), AV_SAMPLE_FMT_S16, 44100,
                av_get_default_channel_layout(mavCodecContext->channels), mavCodecContext->sample_fmt, mavCodecContext->sample_rate, 0, NULL);
        if (!mswrContext || swr_init(mswrContext)) {
            if (mswrContext)
                swr_free(&mswrContext);
            avcodec_close(mavCodecContext);
            LOGI("init resampler failed...");
            return -1;
        }
    }
    LOGI("liyaoli channels is %d sampleRate is %d", mavCodecContext->channels, mavCodecContext->sample_rate);

    av_init_packet(&mpacket);
    mpAudioFrame = av_frame_alloc();

    int ret = 1;
    av_init_packet(&mpacket);
    int gotframe = 0;
    out_buffer=(uint8_t *)av_malloc(MAX_AUDIO_FRAME_SIZE*2);

    while (av_read_frame(mavFormatContext, &mpacket) >= 0) {
            if (mpacket.stream_index == mstream_index) {
                int len = avcodec_decode_audio4(mavCodecContext, mpAudioFrame, &gotframe, &mpacket);
                if (len < 0) {
                    LOGI("decode audio error, skip packet");
                    return -1;
                }
                if (gotframe) {
                int linesize = 0;
                    int out_buffer_size=mpAudioFrame->nb_samples *
                            OUT_PUT_CHANNELS *
                            av_get_bytes_per_sample(out_sample_fmt);
                    //LOGI("nb_samples = %d, layout = %d, bytes = %d ", mpAudioFrame->nb_samples, OUT_PUT_CHANNELS, av_get_bytes_per_sample(out_sample_fmt));
                    //LOGI("liyaoli out_buffer_size = %d", out_buffer_size);
                    int numFrames = 0;
                    // 重新采样
                    if (mswrContext) {
                        numFrames = swr_convert(mswrContext, &out_buffer,
                                                mpAudioFrame->nb_samples,
                                (const uint8_t **)mpAudioFrame->data,
                                mpAudioFrame->nb_samples);
                        if (numFrames < 0) {
                            LOGI("fail resample audio");
                            ret = -1;
                            break;
                        }
                        LOGI("index:%5d\t pts:%lld\t packet size:%d out_buffer_size: %d\n",index,mpacket.pts,mpacket.size, out_buffer_size);
                        //Write PCM
                        fwrite(out_buffer, 1, out_buffer_size, pcmFile);
                        index++;
                        av_packet_unref(&mpacket);
                    }
                }
            }
    }
    av_free(&mpacket);
    LOGI("liyaoli end");
    av_free(mpAudioFrame);
    swr_free(&mswrContext);
    fclose(pcmFile);
    av_free(out_buffer);
    avcodec_close(mavCodecContext);
    avformat_close_input(&mavFormatContext);

    return 0;
}

bool AccompanyDecoder::audioCodecIsSupported() {
    if (mavCodecContext->sample_fmt == AV_SAMPLE_FMT_S16) {
        return true;
    }
    return false;
}

AudioPacket* AccompanyDecoder::decodePacket() {
    short * samples = new short[mpacketBufferSize];
    int stereoSampleSize = readSamples(samples, mpacketBufferSize);
    AudioPacket* samplePacket = new AudioPacket();
    if (stereoSampleSize > 0) {
    	//构造成一个packet
    	samplePacket->mbuffer = samples;
    	samplePacket->msize = stereoSampleSize;
    } else {
    	samplePacket->msize = -1;
    }
    return samplePacket;
}

int AccompanyDecoder::readSamples(short *samples, int size) {
    int sampleSize = size;
    while (size > 0) {
        if (maudioBufferCursor < maudioBufferSize) {
            int audioBufferDataSize = maudioBufferSize - maudioBufferCursor;
            int copySize = MIN(size, audioBufferDataSize);
            memcpy(samples + (sampleSize - size), maudioBuffer + maudioBufferCursor, copySize * 2);
            size -= copySize;
            maudioBufferCursor += copySize;
        } else {
            if (readFrame() < 0) {
                break;
            }
        }
        int fillSize = sampleSize - size;
        if (fillSize == 0) {
            return -1;
        }
        return fillSize;
    }
}

int AccompanyDecoder::readFrame() {
    int ret = 1;
    av_init_packet(&mpacket);
    int gotframe = 0;
    int readFrameCode = -1;
    while (true) {
        readFrameCode = av_read_frame(mavFormatContext, &mpacket);
        if (readFrameCode >= 0) {
            if (mpacket.stream_index == mstream_index) {
                int len = avcodec_decode_audio4(mavCodecContext, mpAudioFrame,
                        &gotframe, &mpacket);
                if (len < 0) {
                    LOGI("decode audio error, skip packet");
                }
                if (gotframe) {
                    int numChannels = OUT_PUT_CHANNELS;
                    int numFrames = 0;
                    void* audioData;
                    // 重新采样
                    if (mswrContext) {
                        const int ratio = 2;
                        const int bufSize = av_samples_get_buffer_size(NULL,
                                numChannels, mpAudioFrame->nb_samples * ratio,
                                AV_SAMPLE_FMT_S16, 1);
                        if (!mswrBuffer || mswrBufferSize < bufSize) {
                            mswrBufferSize = bufSize;
                            mswrBuffer = realloc(mswrBuffer, mswrBufferSize);
                        }
                        byte *outbuf[2] = { (byte*)mswrBuffer, NULL};
                        numFrames = swr_convert(mswrContext, outbuf,
                                mpAudioFrame->nb_samples * ratio,
                                (const uint8_t **)mpAudioFrame->data,
                                mpAudioFrame->nb_samples);
                        if (numFrames < 0) {
                            LOGI("fail resample audio");
                            ret = -1;
                            break;
                        }
                        audioData = mswrBuffer;
                    } else {
                        if (mavCodecContext->sample_fmt != AV_SAMPLE_FMT_S16) {
                            LOGI("bucheck, audio format is invalid");
                            ret = -1;
                            break;
                        }
                        audioData = mpAudioFrame->data[0];
                        numFrames = mpAudioFrame->nb_samples;
                    }
                    maudioBufferSize = numFrames * numChannels;
                    maudioBuffer = (short*) audioData;
                    maudioBufferCursor = 0;
                    break;
                }
            }
        } else {
            ret = -1;
            break;
        }
    }
    av_free(&mpacket);
    return ret;
}


void AccompanyDecoder::destroy() {
    if (NULL != mswrBuffer) {
        free(mswrBuffer);
        mswrBuffer = NULL;
        mswrBufferSize = 0;
    }
    if (NULL != mswrContext) {
        swr_free(&mswrContext);
        mswrContext = NULL;
    }
    /*if (NULL != mpAudioFrame) {
        av_frame_free(&mpAudioFrame);
        mpAudioFrame = NULL;
    }*/
    if (NULL != mavCodecContext) {
        avcodec_close(mavCodecContext);
        mavCodecContext = NULL;
    }
    if (NULL != mavFormatContext) {
        LOGI("leave LiveReceiver::destory");
        avformat_close_input(&mavFormatContext);
        mavFormatContext = NULL;
    }
}

















