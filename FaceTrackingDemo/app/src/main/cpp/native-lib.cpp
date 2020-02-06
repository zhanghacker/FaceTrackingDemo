#include <jni.h>
#include <string>
#include <android/native_window_jni.h>
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/core.hpp>
#include <DetectionBasedTracker_jni.h>

ANativeWindow *window = 0;
DetectionBasedTracker *tracker = 0;

extern "C"
JNIEXPORT void JNICALL
Java_com_itzxx_facetrackingdemo_OpencvNativeJni_init(JNIEnv *env, jobject thiz, jstring surface) {
    // TODO: implement init()
    const char *model = env->GetStringUTFChars(surface, 0);
    if (tracker) {
        tracker->stop();
        delete tracker;
        tracker = 0;
    }
    //智能指针
    Ptr<CascadeClassifier> classifier = makePtr<CascadeClassifier>(model);
    //创建一个跟踪适配器
    Ptr<CascadeDetectorAdapter> mainDetector = makePtr<CascadeDetectorAdapter>(classifier);
    Ptr<CascadeClassifier> classifier1 = makePtr<CascadeClassifier>(model);
    //创建一个跟踪适配器
    Ptr<CascadeDetectorAdapter> trackingDetector = makePtr<CascadeDetectorAdapter>(classifier1);
    //拿去用的跟踪器
    DetectionBasedTracker::Parameters DetectorParams;
    tracker = new DetectionBasedTracker(mainDetector, trackingDetector, DetectorParams);
    //开启跟踪器
    tracker->run();
    env->ReleaseStringUTFChars(surface, model);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_itzxx_facetrackingdemo_OpencvNativeJni_setSurface(JNIEnv *env, jobject thiz,
                                                        jobject surface) {
    // TODO: implement setSurface()
    if (window) {
        ANativeWindow_release(window);
        window = 0;
    }
    //#编译链接NDK/platforms/android-X/usr/lib/libandroid.so
    //target_link_libraries(XXX android )
    window = ANativeWindow_fromSurface(env, surface);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_itzxx_facetrackingdemo_OpencvNativeJni_postData(JNIEnv *env, jobject thiz, jbyteArray datas,
                                                      jint w, jint h, jint camera_id) {
    // TODO: implement postData()
    // nv21的数据
    jbyte *data = env->GetByteArrayElements(datas, NULL);
    //mat  data-》Mat
    //1、高 2、宽
    Mat src(h * 3 / 2, w, CV_8UC1, data);
    //颜色格式的转换 nv21->RGBA
    //将 nv21的yuv数据转成了rgba
    cvtColor(src, src, COLOR_YUV2RGBA_NV21);
    // 正在写的过程 退出了，导致文件丢失数据
    //imwrite("/sdcard/src.jpg",src);
    if (camera_id == 1) {
        //前置摄像头，需要逆时针旋转90度
        rotate(src, src, ROTATE_90_COUNTERCLOCKWISE);
        //水平翻转 镜像
        flip(src, src, 1);
    } else {
        //顺时针旋转90度
        rotate(src, src, ROTATE_90_CLOCKWISE);
    }
    Mat gray;
    //灰色
    cvtColor(src, gray, COLOR_RGBA2GRAY);
    //增强对比度 (直方图均衡)
    equalizeHist(gray, gray);
    std::vector<Rect> faces;
    //定位人脸 N个
    tracker->process(gray);
    tracker->getObjects(faces);
    for (Rect face : faces) {
        //画矩形
        //分别指定 bgra
        rectangle(src, face, Scalar(255, 0, 0));
    }
    //显示
    if (window) {
        //设置windows的属性
        // 因为旋转了 所以宽、高需要交换
        //这里使用 cols 和rows 代表 宽、高 就不用关心上面是否旋转了
        ANativeWindow_setBuffersGeometry(window, src.cols, src.rows, WINDOW_FORMAT_RGBA_8888);
        ANativeWindow_Buffer buffer;
        do {
            //lock失败 直接brek出去
            if (ANativeWindow_lock(window, &buffer, 0)) {
                ANativeWindow_release(window);
                window = 0;
                break;
            }
            //src.data ： rgba的数据
            //把src.data 拷贝到 buffer.bits 里去
            // 一行一行的拷贝
            //memcpy(buffer.bits, src.data, buffer.stride * buffer.height * 4);
            uint8_t *dst_data = static_cast<uint8_t *>(buffer.bits);
            //stride : 一行多少个数据
            //（RGBA） * 4
            int dst_linesize = buffer.stride * 4;

            //一行一行拷贝，src.data是图片的RGBA数据，要拷贝到dst_data中，也就是window的缓冲区里
            for (int i = 0; i < buffer.height; ++i) {
                memcpy(dst_data + i * dst_linesize, src.data + i * src.cols * 4, dst_linesize);
            }
            //提交刷新
            ANativeWindow_unlockAndPost(window);
        } while (0);
    }
    //释放Mat
    //内部采用的 引用计数
    src.release();
    gray.release();
    env->ReleaseByteArrayElements(datas, data, 0);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_itzxx_facetrackingdemo_OpencvNativeJni_release(JNIEnv *env, jobject thiz) {
    // TODO: implement release()
    if (tracker) {
        tracker->stop();
        delete tracker;
        tracker = 0;
    }
}