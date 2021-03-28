#include "com_example_ffmpegdecoder_decoder_Mp3Decoder.h"
#include "../libffmpeg_decoder/accompany_decoder_controller.h"

#define LOG_TAG "liyaoliMp3Decoder"

AccompanyDecoderController* decoderController;

JNIEXPORT jint JNICALL Java_com_example_ffmpegdecoder_decoder_Mp3Decoder_init
  (JNIEnv * env, jobject obj, jstring mp3PathParam, jstring pcmPathParam) {
    const char * pcmPath = env->GetStringUTFChars(pcmPathParam, NULL);
    const char * mp3Path = env->GetStringUTFChars(mp3PathParam, NULL);
    decoderController = new AccompanyDecoderController();
    jint ret = decoderController->Init(mp3Path, pcmPath);
    env->ReleaseStringUTFChars(mp3PathParam, mp3Path);
    env->ReleaseStringUTFChars(pcmPathParam, pcmPath);
    return ret;
}

/*
 * Class:     com_example_ffmpegdecoder_decoder_Mp3Decoder
 * Method:    decode
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_example_ffmpegdecoder_decoder_Mp3Decoder_decode
  (JNIEnv *, jobject) {
    if (decoderController) {
        decoderController->Decoder();
    }
}

/*
 * Class:     com_example_ffmpegdecoder_decoder_Mp3Decoder
 * Method:    destroy
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_example_ffmpegdecoder_decoder_Mp3Decoder_destroy
(JNIEnv *, jobject) {
    if (decoderController) {
        decoderController->Destroy();
        delete decoderController;
        decoderController = NULL;
    }
}





