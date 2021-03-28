//
// Created by jetson on 2021/3/2.
//

#ifndef FFMPEGDECODER_COMMONTOOLS_H
#define FFMPEGDECODER_COMMONTOOLS_H

#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <sys/time.h>
#include <math.h>
#include <stdlib.h>

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
typedef unsigned char byte;

#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#endif //FFMPEGDECODER_COMMONTOOLS_H
