package com.itzxx.facetrackingdemo;

import android.view.Surface;

public class OpencvNativeJni {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    /**
     * 初始化 追踪器
     *
     * @param model
     */
    public native void init(String model);

    /**
     * 设置画布
     * ANativeWindow
     *
     * @param surface
     */
    public native void setSurface(Surface surface);

    /**
     * 处理摄像头数据
     *
     * @param data
     * @param w
     * @param h
     * @param cameraId
     */
    public native void postData(byte[] data, int w, int h, int cameraId);

    /**
     * 释放
     */
    public native void release();
}
