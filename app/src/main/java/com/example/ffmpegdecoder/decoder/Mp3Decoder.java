package com.example.ffmpegdecoder.decoder;

public class Mp3Decoder {
    static {
        System.loadLibrary("songstudio");
    }

    public native int init(String mp3FilePath, String pcmFilePath);

    public native void decode();

    public native void destroy();
}