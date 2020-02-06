package com.itzxx.facetrackingdemo;

import androidx.appcompat.app.AppCompatActivity;

import android.hardware.Camera;
import android.os.Bundle;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import java.io.File;

public class MainActivity extends AppCompatActivity implements SurfaceHolder.Callback, Camera.PreviewCallback {

    private CameraHelper cameraHelper;
    int cameraId = Camera.CameraInfo.CAMERA_FACING_FRONT;
    private OpencvNativeJni opencvJni;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        opencvJni = new OpencvNativeJni();
        SurfaceView surfaceView = findViewById(R.id.surfaceView);
        surfaceView.getHolder().addCallback(this);
        cameraHelper = new CameraHelper(cameraId);
        cameraHelper.setPreviewCallback(this);

        Utils.copyAssets(this, "lbpcascade_frontalface.xml");
    }

    @Override
    protected void onResume() {
        super.onResume();
        String path = new File(getExternalFilesDir(null), "lbpcascade_frontalface.xml").getAbsolutePath();
        //初始化跟踪器
        opencvJni.init(path);
        cameraHelper.startPreview();
    }

    @Override
    protected void onStop() {
        super.onStop();
        //释放跟踪器
        opencvJni.release();
        cameraHelper.stopPreview();
    }

    @Override
    public void surfaceCreated(SurfaceHolder surfaceHolder) {

    }

    @Override
    public void surfaceChanged(SurfaceHolder surfaceHolder, int i, int i1, int i2) {
        //设置surface 用于显示
        opencvJni.setSurface(surfaceHolder.getSurface());
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder surfaceHolder) {

    }

    @Override
    public void onPreviewFrame(byte[] bytes, Camera camera) {
        //bytes摄像头采集的数据是NV21格式数据
        //传输数据
        opencvJni.postData(bytes, CameraHelper.WIDTH, CameraHelper.HEIGHT, cameraId);
    }
}
