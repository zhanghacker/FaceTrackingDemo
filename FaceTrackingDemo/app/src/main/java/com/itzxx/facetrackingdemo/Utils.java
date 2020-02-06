package com.itzxx.facetrackingdemo;

import android.content.Context;
import android.content.Intent;
import android.os.Environment;

import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;

/**
 * Desc:
 * <p>
 * Author: 张潇潇
 * Date: 2020-01-29
 * Copyright: Copyright (c) 2010-2020
 * Company: @咪咕动漫
 * Updater:
 * Update Time:
 * Update Comments:
 */
public class Utils {

    public static void copyAssets(Context context, String path) {
        File model = new File(path);
        File file = new File(context.getExternalFilesDir(null), model.getName());
        if (file.exists()) {
            return;
        }
        try {
            FileOutputStream fos = new FileOutputStream(file);
            InputStream is = context.getAssets().open(path);
            int len;
            byte[] b = new byte[2048];
            while ((len = is.read(b)) != -1) {
                fos.write(b, 0, len);
            }
            fos.close();
            is.close();
        } catch (Exception e) {
            e.printStackTrace();
        }

    }
}
