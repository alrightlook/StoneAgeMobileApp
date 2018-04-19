package com.jerrystudio.stoneage;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.lang.Boolean;
import java.lang.Override;
import java.lang.Thread;
import java.net.URL;
import java.net.URLConnection;
import java.net.HttpURLConnection;

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

import java.util.zip.ZipInputStream;
import android.view.WindowManager;
import java.io.File;
import java.util.zip.ZipEntry;
import java.lang.String;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.BufferedInputStream;
import java.util.Enumeration;
import java.util.zip.ZipEntry;
import java.util.zip.ZipException;
import java.util.zip.ZipFile;
import java.io.OutputStream;
import com.aspsine.multithreaddownload.DownloadConfiguration;
import com.aspsine.multithreaddownload.DownloadManager;
import com.aspsine.multithreaddownload.util.FileUtils;
import com.aspsine.multithreaddownload.CallBack;
import com.aspsine.multithreaddownload.DownloadRequest;
import com.aspsine.multithreaddownload.DownloadException;

public class DownloadPage extends Activity {
    final String resUrl = "http://jerrysa-1256047002.cos.ap-shanghai.myqcloud.com/jerrysa.zip";
    final String realBinURL = "http://jerrysa-1256047002.cos.ap-shanghai.myqcloud.com/real_137.bin";
    final String realBinFolder = "/jerrysa/data/";
    ProgressBar pb;
    TextView tv;
    int   fileSize;
    int   downLoadFileSize;
    String fileEx,fileNa,filename;
    String unzipFilePath;
    int TIME_OUT = 3000;
    int THREAD_NUM = 5;


    long lastTime;
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        pb=(ProgressBar)findViewById(R.id.down_pb);
        tv=(TextView)findViewById(R.id.tv);

        //getWindow().setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON, WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON); setContentView(R.layout.main);

        /*new Thread() {
            public void run() {
                startToDownload();
            }
        }.start();*/

        String sdcardPath = Environment.getExternalStorageDirectory().getAbsolutePath();
        String resPath = sdcardPath + "/jerrysa/";

        File resPathFolder = new File(resPath);

        if (!resPathFolder.exists())
        {
            DownloadWithDM();
        }
        else
        {
            Intent intent = new Intent(DownloadPage.this, JerryStoneAge.class);
            startActivity(intent);
            this.finish();
        }

    }

    private void DownloadWithDM()
    {
        DownloadTools dt = new DownloadTools(getApplicationContext(), new DownloadTools.OnProgressListener() {
            public void onProgress(float fraction)
            {
                pb.setProgress((int)(fraction * 100));
                tv.setText("下载进度："+ (int) (fraction * 100) + "%");
            }

            public void onCompleted()
            {
                final String sdcardPath = Environment.getExternalStorageDirectory().getAbsolutePath();
                tv.setText("正在解压缩资料。。。");
                UnZip zipInstance = new UnZip(sdcardPath + "/jerrysa.zip", sdcardPath + "/");
                zipInstance.execute();
            }
        });
        dt.downloadAPK(resUrl,"/", "jerrysa.zip");
    }

    private void startToDownload()
    {
        DownloadConfiguration configuration = new DownloadConfiguration();
        configuration.setMaxThreadNum(5);
        configuration.setThreadNum(5);
        DownloadManager.getInstance().init(getApplicationContext(), configuration);


        String sdcardPath = Environment.getExternalStorageDirectory().getAbsolutePath();
        String resPath = sdcardPath + "/jerrysa/";

        File resPathFolder = new File(resPath);

        if (!resPathFolder.exists())
        {
            final String zipFilePath = sdcardPath + "/jerrysa.zip";
            File zipFile = new File(zipFilePath);
            if (zipFile.exists())
            {
                zipFile.delete();
            }
            if (zipFile.exists()) {
                UnZip zipInstance = new UnZip(sdcardPath + "/jerrysa.zip", sdcardPath + "/");
                zipInstance.execute();
            }
            else
            {



                final DownloadRequest request = new DownloadRequest.Builder()
                        .setUri(resUrl)
                        .setName("jerrysa.zip")
                        .setFolder(new File(sdcardPath))
                        .build();

                DownloadManager.getInstance().download(request, resUrl, new CallBack() {
                    @Override
                    public void onStarted() {
                        lastTime = System.currentTimeMillis();
                    }

                    @Override
                    public void onConnecting() {

                    }

                    @Override
                    public void onConnected(long total, boolean isRangeSupport) {

                    }

                    @Override
                    public void onProgress(long finished, long total, int progress) {
                        long dt = System.currentTimeMillis() - lastTime;
                        float v = 0;
                        if (dt != 0) {
                            v = finished / dt;
                        }
                        final int theProgress = progress;
                        final float theV = v;
                        final float theTotal  = total;

                        DownloadPage.this.runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                pb.setProgress(theProgress);
                                tv.setText(theProgress + "%" + "大小:" + theTotal + "速度:" + theV + "KB/S");
                            }
                        });

                    }

                    @Override
                    public void onCompleted() {
                        final String sdcardPath = Environment.getExternalStorageDirectory().getAbsolutePath();
                        tv.setText("正在解压缩资料。。。");
                        UnZip zipInstance = new UnZip(sdcardPath + "/jerrysa.zip", sdcardPath + "/");
                        zipInstance.execute();

                    }

                    @Override
                    public void onDownloadPaused() {

                    }

                    @Override
                    public void onDownloadCanceled() {

                    }

                    @Override
                    public void onFailed(DownloadException e) {

                    }
                });
            }

        }
        else
        {
            Intent intent = new Intent(DownloadPage.this, JerryStoneAge.class);
            startActivity(intent);
            this.finish();
        }
    }


    public static void delete(File file) {
        if (file.isFile()) {
                        file.delete();
                        return;
                    }

                if(file.isDirectory()){
                        File[] childFiles = file.listFiles();
                        if (childFiles == null || childFiles.length == 0) {
                                file.delete();
                                return;
                            }

                        for (int i = 0; i < childFiles.length; i++) {
                                delete(childFiles[i]);
                            }
                        file.delete();
                }
    }


    void deleteRecursive(File fileOrDirectory) {
        if (fileOrDirectory.isDirectory())
            for (File child : fileOrDirectory.listFiles())
                deleteRecursive(child);

        fileOrDirectory.delete();
    }

    void DownloadRealBin()
    {
        Log.d("Jerry", "-----------------------Start Download RealBin--------------------------");
        final DownloadRequest realBinRequest = new DownloadRequest.Builder()
                .setUri(realBinURL)
                .setName("real_137.bin")
                .setFolder(new File(realBinFolder))
                .build();

        DownloadManager.getInstance().download(realBinRequest, realBinURL, new CallBack() {
            @Override
            public void onStarted() {
                lastTime = System.currentTimeMillis();
                pb.setMax(100);
            }

            @Override
            public void onConnecting() {

            }

            @Override
            public void onConnected(long total, boolean isRangeSupport) {

            }

            @Override
            public void onProgress(long finished, long total, int progress) {
                long dt = System.currentTimeMillis() - lastTime;
                float v = 0;
                if (dt != 0) {
                    v = finished / dt;
                }
                final int theProgress = progress;
                final float theV = v;
                DownloadPage.this.runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        pb.setProgress(theProgress);
                        tv.setText("正在下载realbin资料包：" + theProgress + "%" + "速度:" + theV + "KB/S");
                    }
                });


            }

            @Override
            public void onCompleted() {

                Intent intent = new Intent(DownloadPage.this, JerryStoneAge.class);
                startActivity(intent);
                DownloadPage.this.finish();
            }

            @Override
            public void onDownloadPaused() {

            }

            @Override
            public void onDownloadCanceled() {

            }

            @Override
            public void onFailed(DownloadException e) {
                Toast.makeText(getApplicationContext(), "下载错误" + e.toString(), 5000).show();
            }
        });

    }

    private class UnZip extends AsyncTask<Void, Integer, Integer> {

        private String _zipFile;
        private String _location;
        private int per = 0;

        public UnZip(String zipFile, String location) {
            _zipFile = zipFile;
            _location = location;
            _dirChecker("");
        }

        public void streamCopy(InputStream in, OutputStream out) throws IOException {
            byte[] buffer = new byte[32 * 1024]; // play with sizes..
            int readCount;
            while ((readCount = in.read(buffer)) != -1) {
                out.write(buffer, 0, readCount);
            }
        }

        protected Integer doInBackground(Void... params) {
            try  {
                ZipFile zip = new ZipFile(_zipFile);
                pb.setMax(zip.size());
                FileInputStream fin = new FileInputStream(_zipFile);
                ZipInputStream zin = new ZipInputStream(fin);
                ZipEntry ze = null;
                while ((ze = zin.getNextEntry()) != null) {

                    Log.v("Decompress", "Unzipping " + ze.getName());
                    //tv.setText("解压文件："+ ze.getName());
                    if(ze.isDirectory()) {
                        _dirChecker(ze.getName());
                    } else {
                        // Here I am doing the update of my progress bar
                        Log.v("Decompress", "more " + ze.getName());

                        per++;
                        DownloadPage.this.runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                publishProgress(per);
                            }
                        });


                        FileOutputStream fout = new FileOutputStream(_location + ze.getName());

                        streamCopy(zin, fout);

                        zin.closeEntry();
                        fout.close();
                    }
                }
                zin.close();
            } catch(Exception e) {
                Log.e("Decompress", "unzip", e);
            }
            return null;
        }

        protected void onProgressUpdate(Integer... progress) {
            pb.setProgress(per); //Since it's an inner class, Bar should be able to be called directly
        }

        protected void onPostExecute(Integer result) {
            Log.i("UnZip", "Completed. Total size: " + result);
            File f = new File(Environment.getExternalStorageDirectory().getAbsolutePath() + "/jerrysa.zip");
            f.delete();
            pb.setMax(100);
            DownloadTools dt = new DownloadTools(getApplicationContext(), new DownloadTools.OnProgressListener() {
                public void onProgress(float fraction)
                {
                    pb.setProgress((int)(fraction * 100));
                    tv.setText("下载进度："+ (int) (fraction * 100) + "%");
                }

                public void onCompleted()
                {
                    Intent intent = new Intent(DownloadPage.this, JerryStoneAge.class);
                    startActivity(intent);
                    DownloadPage.this.finish();
                }
            });
            File realbinFile = new File("/sdcard/jerrysa/data/real_137.bin");
            if (!realbinFile.exists())
            {
                dt.downloadAPK(realBinURL,realBinFolder, "real_137.bin");
            }
        }

        private void _dirChecker(String dir) {
            File f = new File(_location + dir);
            if(!f.isDirectory()) {
                f.mkdirs();
            }
        }

    }

    @Override
    public void onBackPressed() {
    }

}
