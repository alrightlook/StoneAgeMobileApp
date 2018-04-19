package com.jerrystudio.stoneage;

import android.app.DownloadManager.Request;
import android.app.DownloadManager;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.lang.Boolean;
import java.lang.Override;
import java.lang.Thread;
import java.net.URL;
import java.net.URLConnection;
import java.net.HttpURLConnection;


import android.app.DownloadManager;
import android.app.DownloadManager.Request;
import android.content.BroadcastReceiver;
import android.content.Intent;
import android.content.IntentFilter;
import android.database.ContentObserver;

import android.content.SharedPreferences;
import android.app.DownloadManager.Request;
import android.net.Uri;
import android.content.SharedPreferences.Editor;
import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;
import android.util.Log;
import android.content.Intent;
import android.os.Environment;
import 	android.os.AsyncTask;
import android.content.Context;
import android.content.BroadcastReceiver;
import 	android.app.DownloadManager.Query;
import android.content.IntentFilter;
import android.database.Cursor;


public class DownloadTools {
    //下载器
    private DownloadManager downloadManager;
    //上下文
    private Context mContext;
    private OnProgressListener onProgressListener;
    public static final int HANDLE_DOWNLOAD = 0x001;
    //下载的ID
    private long downloadId;
    ProgressBar mProgressBar;
    public  DownloadTools(Context context, OnProgressListener listener){
        this.mContext = context;
        this.onProgressListener = listener;
    }

    //下载apk
    public void downloadAPK(String url,String path, String name) {

        isCompleted = false;
        //创建下载任务
        Request request = new Request(Uri.parse(url));
        //移动网络情况下是否允许漫游
        request.setAllowedOverRoaming(false);

        //在通知栏中显示，默认就是显示的
        request.setNotificationVisibility(DownloadManager.Request.VISIBILITY_VISIBLE);
        request.setDescription("Apk Downloading");
        request.setVisibleInDownloadsUi(true);

        //设置下载的路径
        request.setDestinationInExternalPublicDir(path , name);

        //获取DownloadManager
        downloadManager = (DownloadManager) mContext.getSystemService(Context.DOWNLOAD_SERVICE);
        //将下载请求加入下载队列，加入下载队列后会给该任务返回一个long型的id，通过该id可以取消任务，重启任务、获取下载的文件等等
        downloadId = downloadManager.enqueue(request);

        //注册广播接收者，监听下载状态
        mContext.registerReceiver(receiver,
                new IntentFilter(DownloadManager.ACTION_DOWNLOAD_COMPLETE));

        Uri myDownloads = Uri.parse( "content://downloads/my_downloads" );
        mContext.getContentResolver().registerContentObserver(myDownloads, true, new DownloadObserver());


    }

    public Handler downLoadHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            if (onProgressListener != null && HANDLE_DOWNLOAD == msg.what) {
                //被除数可以为0，除数必须大于0
                if (msg.arg1 >= 0 && msg.arg2 > 0) {
                    onProgressListener.onProgress(msg.arg1 / (float) msg.arg2);
                }
            }
        }
    };
    private boolean isCompleted;
    class DownloadObserver extends ContentObserver {
        public DownloadObserver()
        {
            super(downLoadHandler);
        }

        @Override
        public void onChange(boolean selfChange, Uri uri) {
            if (!isCompleted) {
                int[] bytesAndStatus = getBytesAndStatus(downloadId);
                downLoadHandler.sendMessage(downLoadHandler.obtainMessage(HANDLE_DOWNLOAD, bytesAndStatus[0], bytesAndStatus[1], bytesAndStatus[2]));
            }
        }
    }

    public interface OnProgressListener {
        void onProgress(float fraction);
        void onCompleted();
    }

    private int[] getBytesAndStatus(long downloadId) {
        int[] bytesAndStatus = new int[]{
                -1, -1, 0
        };
        DownloadManager.Query query = new DownloadManager.Query().setFilterById(downloadId);
        Cursor cursor = null;
        try {
            cursor = downloadManager.query(query);
            if (cursor != null && cursor.moveToFirst()) {
                //已经下载文件大小
                bytesAndStatus[0] = cursor.getInt(cursor.getColumnIndexOrThrow(DownloadManager.COLUMN_BYTES_DOWNLOADED_SO_FAR));
                //下载文件的总大小
                bytesAndStatus[1] = cursor.getInt(cursor.getColumnIndexOrThrow(DownloadManager.COLUMN_TOTAL_SIZE_BYTES));
                //下载状态
                bytesAndStatus[2] = cursor.getInt(cursor.getColumnIndex(DownloadManager.COLUMN_STATUS));
            }
        } finally {
            if (cursor != null) {
                cursor.close();
            }
        }
        return bytesAndStatus;
    }



    //广播监听下载的各个状态
    private BroadcastReceiver receiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            checkStatus();
        }
    };


    //检查下载状态
    private void checkStatus() {
        Query query = new Query();
        //通过下载的id查找
        query.setFilterById(downloadId);
        Cursor c = downloadManager.query(query);
        if (c.moveToFirst()) {
            int status = c.getInt(c.getColumnIndex(DownloadManager.COLUMN_STATUS));
            switch (status) {
                //下载暂停
                case DownloadManager.STATUS_PAUSED:
                    break;
                //下载延迟
                case DownloadManager.STATUS_PENDING:
                    break;
                //正在下载
                case DownloadManager.STATUS_RUNNING:
                    Toast.makeText(mContext, "正在下载", 5000).show();
                    break;
                //下载完成
                case DownloadManager.STATUS_SUCCESSFUL:
                    isCompleted = true;
                    downLoadHandler.removeCallbacksAndMessages(null);
                    onProgressListener.onCompleted();
                    //下载完成安装APK
                    //installAPK();
                    break;
                //下载失败
                case DownloadManager.STATUS_FAILED:
                    Toast.makeText(mContext, "下载失败", Toast.LENGTH_SHORT).show();
                    break;
            }
        }
    }

    //下载到本地后执行安装
    private void installAPK() {
        //获取下载文件的Uri
        Uri downloadFileUri = downloadManager.getUriForDownloadedFile(downloadId);
        if (downloadFileUri != null) {
            Intent intent= new Intent(Intent.ACTION_VIEW);
            intent.setDataAndType(downloadFileUri, "application/vnd.android.package-archive");
            intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
            mContext.startActivity(intent);
        }
    }

}