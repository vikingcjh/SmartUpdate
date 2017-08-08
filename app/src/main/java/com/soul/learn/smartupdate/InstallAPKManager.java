package com.soul.learn.smartupdate;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;

import android.content.ActivityNotFoundException;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.net.Uri;
import android.text.TextUtils;



public class InstallAPKManager {

//	private static final String ACTION_LETV_STORE_QUIET_INSTALL_FAILED = "com.letv.caros.appstore.installAPKFailure";
	private static InstallAPKManager downloadInstallingManager = new InstallAPKManager();
	private Map<String, InstallingApp> letvAppInstallingMap;
	private String reason;


	public boolean isInstalling(String packageName) {
		boolean result = false;
		Set<Entry<String, InstallingApp>> installingEntrySet = letvAppInstallingMap.entrySet();
		Iterator<Entry<String, InstallingApp>> installingIterator = installingEntrySet.iterator();
		Entry<String, InstallingApp> next;
		while (installingIterator.hasNext()) {
			next = installingIterator.next();
			if (packageName.equals(next.getKey())) {
				result = true;
				break;
			}
		}
		return result;

	}

	public List<String> getInstallingPackageList() {
		List<String> resultList = new ArrayList<String>();
		Set<Entry<String, InstallingApp>> installingEntrySet = letvAppInstallingMap.entrySet();
		Iterator<Entry<String, InstallingApp>> installingIterator = installingEntrySet.iterator();
		Entry<String, InstallingApp> next;
		while (installingIterator.hasNext()) {
			next = installingIterator.next();
			resultList.add(next.getKey());

		}
		return resultList;
	}

	private InstallAPKManager() {
		letvAppInstallingMap = new HashMap<String, InstallingApp>();
	}

	public int getInstallingSize() {
		int result = 0;
		if (letvAppInstallingMap != null) {
			result = letvAppInstallingMap.size();
		}
		return result;
	}

	public static InstallAPKManager getInstance() {
		return downloadInstallingManager;
	}

	public InstallingApp getInstalingApp(String packageName) {
		InstallingApp installingApp = null;
		if (packageName != null) {
			installingApp = letvAppInstallingMap.get(packageName);
		}
		return installingApp;
	}

	/**
	 * 获取应用版本
	 * 
	 * @param context
	 *            上下文引用
	 * @return 应用版本
	 * @throws NameNotFoundException
	 *             未设置应用版本
	 */
	public static int getVersionCode(Context context) {

		int versionCode = 0;
		try {
			PackageInfo packageInfo = context.getPackageManager().getPackageInfo(context.getPackageName(), 0);
			if (null != packageInfo) {
				versionCode = packageInfo.versionCode;
			}

		} catch (NameNotFoundException e) {
			e.printStackTrace();
		}

		return versionCode;
	}

	public static int getVersionCode(Context context, String packageName) {

		int versionCode = 0;
		try {
			PackageInfo packageInfo = context.getPackageManager().getPackageInfo(packageName, 0);
			if (null != packageInfo) {
				versionCode = packageInfo.versionCode;
			}

		} catch (NameNotFoundException e) {
			e.printStackTrace();
		}

		return versionCode;
	}
	
	public static String getVersionName(Context context, String packageName) {

		String versionName = null;
		try {
			PackageInfo packageInfo = context.getPackageManager().getPackageInfo(packageName, 0);
			if (null != packageInfo) {
				versionName = packageInfo.versionName;
			}

		} catch (NameNotFoundException e) {
			e.printStackTrace();
		}

		return versionName;
	}

	/**
	 * 获取应用版本名
	 * 
	 * @param context
	 * @return
	 */
	public static String getVersionName(Context context) {
		String versionName = "";
		try {
			PackageInfo packageInfo = context.getPackageManager().getPackageInfo(context.getPackageName(), 0);
			if (null != packageInfo) {
				versionName = packageInfo.versionName;
			}

		} catch (NameNotFoundException e) {
			e.printStackTrace();
		}
		return versionName;
	}

	/**
	 * 判断应用是否已经安装
	 * 
	 * @param packageName
	 * @return
	 */
	public static boolean isAppInstalled(Context context, String packageName) {
		List<ApplicationInfo> list = getAllInstalledAppInfo(context);
		if (list == null || list.size() == 0 || TextUtils.isEmpty(packageName)) {
			return false;
		}
		boolean isExists = false;
		for (ApplicationInfo info : list) {
			if (info.packageName.equals(packageName)) {
				isExists = true;
				break;
			}
		}
		return isExists;

	}

	/**
	 * 获取全部应用列表
	 * 
	 * **/
	public static List<ApplicationInfo> getAllInstalledAppInfo(Context context) {
		PackageManager pm = context.getPackageManager();
		List<ApplicationInfo> packs = pm.getInstalledApplications(0);
		return packs;
	}
	/**
	 * 获取系统应用列表
	 * 
	 * **/
	public static List<ApplicationInfo> getSystemInstalledAppInfo(Context context) {
		PackageManager pm = context.getPackageManager();
		List<ApplicationInfo> packs = pm.getInstalledApplications(PackageManager.GET_UNINSTALLED_PACKAGES);
		List<ApplicationInfo> appInfos = new ArrayList<ApplicationInfo>();
		appInfos.clear();
        for (ApplicationInfo app : packs) {
            if ((app.flags & ApplicationInfo.FLAG_SYSTEM) != 0) {
                appInfos.add(app);
            }
        }
        return appInfos;
	}
	
	/**
	 * 获取三方应用列表
	 * 
	 * **/
	public static List<ApplicationInfo> getThirdAppInstalledAppInfo(Context context) {
		PackageManager pm = context.getPackageManager();
		List<ApplicationInfo> packs = pm.getInstalledApplications(PackageManager.GET_UNINSTALLED_PACKAGES);
		List<ApplicationInfo> appInfos = new ArrayList<ApplicationInfo>();
		appInfos.clear();
        for (ApplicationInfo app : packs) {
        	if ((app.flags & ApplicationInfo.FLAG_SYSTEM) <= 0) {
                appInfos.add(app);
            } 
            //本来是系统程序，被用户手动更新后，该系统程序也成为第三方应用程序了
            else if ((app.flags & ApplicationInfo.FLAG_UPDATED_SYSTEM_APP) != 0){
                appInfos.add(app);
            }
        }
        return appInfos;
	}
	
	/**
	 * 安装在SDCard的应用程序
	 * 
	 * **/
	public static List<ApplicationInfo> getSdcardInstalledAppInfo(Context context) {
		PackageManager pm = context.getPackageManager();
		List<ApplicationInfo> packs = pm.getInstalledApplications(PackageManager.GET_UNINSTALLED_PACKAGES);
		List<ApplicationInfo> appInfos = new ArrayList<ApplicationInfo>();
		appInfos.clear();
		for (ApplicationInfo app : packs) {
            if ((app.flags & ApplicationInfo.FLAG_EXTERNAL_STORAGE) != 0) {
                appInfos.add(app);
            }
        }
        return appInfos;
	}


	// 是否有静默安装
	public static boolean hasQuietInstallReceiverInit(Context context) {
		boolean isHaveQuietInstall = false;
		PackageInfo packageInfo = null;
		try {
			packageInfo = context.getPackageManager().getPackageInfo("com.android.packageinstaller", PackageManager.GET_RECEIVERS);
		} catch (NameNotFoundException e) {
			e.printStackTrace();
		}
		if (null != packageInfo) {
			ActivityInfo[] receivers = packageInfo.receivers;
			if (receivers != null) {
				for (ActivityInfo ai : receivers) {
					if (ai.name.equalsIgnoreCase("com.android.packageinstaller.InstallQuietReceiver")) {
						isHaveQuietInstall = true;
						break;
					}
				}
			}
		}

		return isHaveQuietInstall;
	}

	/**
	 * Send an Intent to open the download currently pointed to by the given
	 * cursor.
	 */
	public static void openUri(Context context, String stringUri, String mediaType) {
		Uri localUri = Uri.parse(stringUri);
		try {
			context.getContentResolver().openFileDescriptor(localUri, "r").close();
		} catch (FileNotFoundException exc) {
			return;
		} catch (IOException exc) {
			exc.printStackTrace();
		}

		Intent intent = new Intent(Intent.ACTION_VIEW);
		intent.setDataAndType(localUri, mediaType);
		intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_GRANT_READ_URI_PERMISSION);
		try {
			context.startActivity(intent);
		} catch (ActivityNotFoundException ex) {
			ex.printStackTrace();
		}
	}

	/**
	 * 静默安装
	 * 
	 * @param context
	 * @param path
	 */
	public static boolean installAPKFileQuiet(Context context, String path, String packageName) {

		// ToastUtil.showToast(context, "URI:"+path);
		boolean isStartSuccess = false;
		try {
			String command = "chmod " + "666" + " " + path;
			Runtime runtime = Runtime.getRuntime();
			runtime.exec(command);
		} catch (IOException e) {
			e.printStackTrace();
		}

		Intent intent = new Intent("android.intent.action.INSTALL_PACKAGE_QUIET");
		if (path != null) {
			final Uri uri = Uri.parse(path);
			intent.setData(uri);
			
//			intent.putExtra("fromLetv", true);
//			intent.putExtra("INSTALL_PACKAGE_NAME", packageName);
//			intent.putExtra("INSTALL_APP_NAME", packageName);
//			intent.putExtra("CLIENT_NAME", "LetvStore3.0"); // for example: TV版
//			context.sendBroadcast(intent);
//			isStartSuccess = true;
			
			intent.putExtra("INSTALL_APP_NAME", packageName);  // 即将安装的app名称
	        intent.putExtra("INSTALL_PACKAGE_NAME", packageName); //需要安装的app的包名
	        intent.putExtra("CLIENT_NAME", context.getPackageName());  //调用者app包名, 必须传, 否则拒绝安装.
//	        intent.setData(Uri.fromFile(new File("/storage/sdcard0/Test.apk")));
	        context.sendBroadcast(intent);
	        isStartSuccess = true;
		}
		return isStartSuccess;
	}

	public static void installAPKFile(Context context, String path) {
		Intent intent = new Intent();
		intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
		intent.setAction(Intent.ACTION_VIEW);
		intent.setDataAndType(Uri.parse("file://" + path), "application/vnd.android.package-archive");
		context.startActivity(intent);
	}

	public static void installAPKFileQuietAndOpen(Context context, String downloadPath, String packageName) {

		Intent intent = new Intent("android.intent.action.VIEW");
		intent.setDataAndType(Uri.fromFile(new File(downloadPath)), "application/vnd.android.package-archive");
		intent.putExtra("install&start&param", 11);
		intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
		context.startActivity(intent);
	}

	public static boolean isSystemApp(Context context, String packageName) {
		boolean result = false;
		try {
			PackageManager pm = context.getPackageManager();
			PackageInfo packInfo = pm.getPackageInfo(packageName, 0);
			// 不等于0,就是系统应用;
			if (packInfo != null && packInfo.applicationInfo != null && (packInfo.applicationInfo.flags & ApplicationInfo.FLAG_SYSTEM) != 0) {
				result = true;
			}
		} catch (NameNotFoundException e) {
			e.printStackTrace();
		}
		return result;
	}

	public void startApp(Context context, String packageName) {
		PackageManager packageManager = context.getPackageManager();
		Intent startAppIntent = packageManager.getLaunchIntentForPackage(packageName);
		if (startAppIntent != null) {
			try {
				Intent intent = packageManager.getLaunchIntentForPackage(packageName);
//				context.startService(new Intent(context, AppMonitoringService.class));
				context.startActivity(intent);
			} catch (Exception e) {
				e.printStackTrace();
			}
		}

	}

	public static class InstallingApp {
		private String packageName;
		private String name;
		private long currentTime;

		public String getPackageName() {
			return packageName;
		}

		public void setPackageName(String packageName) {
			this.packageName = packageName;
		}

		public String getName() {
			return name;
		}

		public void setName(String name) {
			this.name = name;
		}

		public Long getCurrentTime() {
			return currentTime;
		}

		public void setCurrentTime(long currentTime) {
			this.currentTime = currentTime;
		}
	}

	public void setReason(String reason) {
		this.reason = reason;
	}

	public String getReason() {
		return reason;
	}

}
