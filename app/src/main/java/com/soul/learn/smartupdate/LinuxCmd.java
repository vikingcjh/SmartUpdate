/**
 * chenjianhua
 * 2016-1-21
 * TODO
 */
package com.soul.learn.smartupdate;

import java.io.File;

/**
 * chenjianhua 2016-1-21 下午2:27:31 todo: TODO
 * 
 * @version
 */
public class LinuxCmd {

	public static boolean execCommand(String command) {
		Process process = null;
		try {
			process = Runtime.getRuntime().exec(command);
			process.waitFor();
		} catch (Exception e) {
			return false;
		}
		return true;
	}
	
	//param1 文件全路径  param2 文件夹路径
	public static boolean copyFile(String frompath, String topath) {
		File file = new File(frompath);
		if (!file.exists()) {
			return false;
		}
		String filename = file.getName();
		String destfile = topath+File.separator+filename;
		String cmd = "cp"+" " + frompath +" "+destfile;
		return execCommand(cmd);
	}
}
