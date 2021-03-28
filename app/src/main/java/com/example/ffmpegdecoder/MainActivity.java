package com.example.ffmpegdecoder;

import androidx.appcompat.app.AppCompatActivity;
import com.example.ffmpegdecoder.decoder.Mp3Decoder;

import android.os.Bundle;
import android.util.Log;

public class MainActivity extends AppCompatActivity {

    private static String TAG = "liyaoliMainActivity";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    @Override
    protected void onResume() {
        super.onResume();
        Mp3Decoder decoder = new Mp3Decoder();
        Log.e(TAG, getExternalCacheDir().getPath());
        int ret = decoder.init("/storage/emulated/0/Android/data/com.example.ffmpegdecoder/cache/123.mp3", "/storage/emulated/0/Android/data/com.example.ffmpegdecoder/cache/ff.pcm");
        if (ret >= 0) {
            decoder.decode();
            decoder.destroy();
        } else {
            Log.e(TAG, "Decoder init failed...");
        }
    }
}
