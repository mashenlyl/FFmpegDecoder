LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_CFLAGS += -D__STDC_CONSTANT_MACROS
LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/3rdparty/ffmpeg/include

LOCAL_SRC_FILES = \
./decoder/jni/Mp3Decoder.cpp

LOCAL_STATIC_LIBRARIES := libffmpeg_decoder

LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog
LOCAL_LDLIBS += -lz
LOCAL_LDLIBS += -landroid

LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/prebuilt/x86/libavfilter.a
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/prebuilt/x86/libavformat.a
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/prebuilt/x86/libavcodec.a
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/prebuilt/x86/libavutil.a
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/prebuilt/x86/libswscale.a
LOCAL_LDFLAGS += $(LOCAL_PATH)/3rdparty/prebuilt/x86/libswresample.a

LOCAL_MODULE := libsongstudio
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/3rdparty/ffmpeg/include

LOCAL_CFLAGS := -DHAVE_CONFIG_H -ffast-math

LOCAL_SRC_FILES += \
./decoder/libffmpeg_decoder/accompany_decoder_controller.cpp \
 ./decoder/libffmpeg_decoder/accompany_decoder.cpp

LOCAL_MODULE := libffmpeg_decoder
include $(BUILD_STATIC_LIBRARY)