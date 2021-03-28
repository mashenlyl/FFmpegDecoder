//
// Created by jetson on 2021/3/2.
//

#include "accompany_decoder_controller.h"
#define LOG_TAG "liyaoliacDecoderController"

AccompanyDecoderController::AccompanyDecoderController() {
    macDecoder = NULL;
    mpcmFile = NULL;
}

AccompanyDecoderController::~AccompanyDecoderController() {

}

int AccompanyDecoderController::Init(const char *acPath, const char *pcmFilePath) {
    LOGI("AccompanyDecoderController::Init");
    macDecoder = new AccompanyDecoder();
    int ret = macDecoder->init(acPath, pcmFilePath);
    return ret;
}

void AccompanyDecoderController::Decoder() {
    LOGI("AccompanyDecoderController::Decoder");
}


void AccompanyDecoderController::Destroy() {
    LOGI("AccompanyDecoderController::Destroy");
}








