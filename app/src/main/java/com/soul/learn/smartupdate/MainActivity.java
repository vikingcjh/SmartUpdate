package com.soul.learn.smartupdate;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;
import java.io.File;
import java.io.IOException;
import java.util.List;


import android.net.Uri;
import android.os.AsyncTask;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.R.integer;
import android.app.ProgressDialog;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.text.TextUtils;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

public class MainActivity extends Activity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    private String rootPath;

    private Button btn;
    private EditText mTxtOld;
    private EditText mTxtPatcher;
    private EditText mTxtNew;

    private long systime1;
    private long systime2;

    Handler mHandler = new Handler(){
        /* (non-Javadoc)
         * @see android.os.Handler#handleMessage(android.os.Message)
         */
        @Override
        public void handleMessage(Message msg) {
            // TODO Auto-generated method stub
            switch (msg.what) {
                case 123:
                    Toast.makeText(MainActivity.this, "复制完成", Toast.LENGTH_SHORT).show();
                    break;

                default:
                    break;
            }
        }
    };

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        try {
            super.onActivityResult(requestCode, resultCode, data);
            switch (requestCode) {
                case 0:
                    mTxtOld.setText(data.getData().getEncodedPath());
                    break;

                case 1:
                    mTxtPatcher.setText(data.getData().getEncodedPath());
                    break;

            }
        } catch (Exception ex) {

        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        rootPath = Environment.getExternalStorageDirectory().getAbsolutePath();
        setContentView(R.layout.activity_main);
        btn = (Button) this.findViewById(R.id.button1);
        mTxtOld = (EditText) this.findViewById(R.id.text_old);
        mTxtOld.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                Intent intent = new Intent(Intent.ACTION_GET_CONTENT);
                intent.setType("*/*");
                intent.addCategory(Intent.CATEGORY_OPENABLE);
                startActivityForResult(intent, 0);

            }
        });
        mTxtNew = (EditText) this.findViewById(R.id.text_new);

        mTxtPatcher = (EditText) this.findViewById(R.id.text_patcher);
        mTxtPatcher.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                Intent intent = new Intent(Intent.ACTION_GET_CONTENT);
                intent.setType("*/*");
                intent.addCategory(Intent.CATEGORY_OPENABLE);
                startActivityForResult(intent, 1);

            }
        });
        btn.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {

            	/*new Thread(new Runnable() {

        			@Override
        			public void run() {
        				// TODO Auto-generated method stub
        				List<ApplicationInfo> apps = InstallAPKManager.getInstance().getThirdAppInstalledAppInfo(MainActivity.this);
        		        for (int i = 0; i < apps.size(); i++) {
        					String filepath = apps.get(i).sourceDir;

//        					String ulr1 = File.separator+"data"+ File.separator+"app";
            		        String filedir = rootPath + File.separator+"amyapp2";
            		        try {
            		        	//com.netease.ldxy.huawei com.duowan.mobile
            		        	if (filepath.contains("com.netease.ldxy.huawei")) {
            		        		systime1 = System.currentTimeMillis();
//            		        		FileTools.copySinglefile(filepath, filedir);

            		        		LinuxCmd.copyFile(filepath, filedir);

            		        		systime2 = System.currentTimeMillis();
                                    Log.i("NIODATA","合成时间ms = "+(systime2-systime1));
								}
//            					FileTools.copySinglefile(filepath, ulr2);
            				} catch (Exception e) {
            					// TODO Auto-generated catch block
            					e.printStackTrace();
            				}
        				}

        		        mHandler.sendEmptyMessage(123);
        			}
        		}).start();*/

                if (TextUtils.isEmpty(mTxtOld.getText().toString())) {
                    Toast.makeText(MainActivity.this, "请选择旧版本文件", Toast.LENGTH_SHORT).show();
                    return;
                }

                if (TextUtils.isEmpty(mTxtNew.getText().toString())) {
                    Toast.makeText(MainActivity.this, "请输入合并后新版本文件名", Toast.LENGTH_SHORT).show();
                    return;
                }
                if (TextUtils.isEmpty(mTxtPatcher.getText().toString())) {
                    Toast.makeText(MainActivity.this, "请选择补丁文件", Toast.LENGTH_SHORT).show();
                    return;
                }

                AsyncTask task = new AsyncTask() {

                    private ProgressDialog progressDialog;

                    @Override
                    protected void onPostExecute(Object result) {
                        progressDialog.dismiss();
                        Toast.makeText(MainActivity.this, "打包完成，安装。。。。", Toast.LENGTH_SHORT).show();
                        Intent i = new Intent(Intent.ACTION_VIEW);
                        String filePath = rootPath + File.separator+"aaa" + File.separator+ mTxtNew.getText().toString() + ".apk";
                        i.setDataAndType(Uri.parse("file://" + filePath), "application/vnd.android.package-archive");
                        startActivity(i);

                    }

                    @Override
                    protected void onPreExecute() {
                        super.onPreExecute();
                        progressDialog = ProgressDialog.show(MainActivity.this, "正在生成APK...", "请稍等...", true, false);
                        progressDialog.show();
                    }

                    @Override
                    protected Object doInBackground(Object... arg0) {
                        String newApk = rootPath + File.separator+"aaa" + File.separator+ mTxtNew.getText().toString() + ".apk";
                        File file = new File(newApk);
                        if (file.exists())
                            file.delete();

                        systime1 = System.currentTimeMillis();
                        SmartPatcher.patcher(mTxtOld.getText().toString(), newApk, mTxtPatcher.getText().toString());
//                        patcher(mTxtOld.getText().toString(), newApk, mTxtPatcher.getText().toString());
                        systime2 = System.currentTimeMillis();
                        Log.i("NIODATA","合成时间ms = "+(systime2-systime1));
                        return null;
                    }
                };
                task.execute();
            }
        });

        /*new Thread(new Runnable() {

			@Override
			public void run() {
				// TODO Auto-generated method stub
				String ulr1 = rootPath + File.separator+"data"+ File.separator+"app";
		        String ulr2 = rootPath + File.separator+"amyapp";
		        try {
					FileTools.copy(ulr1, ulr2);
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
		        mHandler.sendEmptyMessage(123);
			}
		}).start();*/

        /*String path=getApplicationContext().getPackageResourcePath();
        path = getApplicationContext().getFilesDir().getAbsolutePath();
        List<ApplicationInfo> apps = InstallAPKManager.getInstance().getThirdAppInstalledAppInfo(this);
        for (int i = 0; i < apps.size(); i++) {
			String filepath = apps.get(i).sourceDir;
		}*/
        /*int version = InstallAPKManager.getInstance().getVersionCode(this, "com.netease.ldxy.huawei");
        mTxtOld.setText(""+version);
        String versionname = InstallAPKManager.getInstance().getVersionName(this, "com.netease.ldxy.huawei");
        mTxtNew.setText(""+versionname);*/
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

}
