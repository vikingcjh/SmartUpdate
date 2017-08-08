/**
 * chenjianhua
 * 2016-1-12
 * TODO
 */
package com.soul.learn.smartupdate;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

/**
 * chenjianhua 2016-1-12 下午3:48:00 todo: TODO
 * 
 * @version
 */
public class FileTools {
	// 源文件夹
//	static String url1 = "f:/photos";
	// 目标文件夹
//	static String url2 = "d:/tempPhotos";

	public static void copy(String urlSource,String urlDes) throws IOException {
		// 创建目标文件夹
		(new File(urlDes)).mkdirs();
		// 获取源文件夹当前下的文件或目录
		File[] file = (new File(urlSource)).listFiles();
		for (int i = 0; i < file.length; i++) {
			if (file[i].isFile()) {
				// 复制文件
				copyFile(file[i], new File(urlDes + file[i].getName()));
			}
			if (file[i].isDirectory()) {
				// 复制目录
				String sourceDir = urlSource + File.separator + file[i].getName();
				String targetDir = urlDes + File.separator + file[i].getName();
				copyDirectiory(sourceDir, targetDir);
			}
		}
	}
	
	public static void copySinglefile(String filepath,String urlDes) throws IOException {
		// 创建目标文件夹
		(new File(urlDes)).mkdirs();
		// 获取源文件夹当前下的文件或目录
		File file = new File(filepath);
		if (file==null || (!file.exists())) {
			return;
		}
		for (int i = 0; i < 1; i++) {
			if (file.isFile()) {
				// 复制文件
				copyFile(file, new File(urlDes + File.separator+ file.getName()));
			}
			if (file.isDirectory()) {
				// 复制目录
				String sourceDir = filepath + File.separator + file.getName();
				String targetDir = urlDes + File.separator + file.getName();
				copyDirectiory(sourceDir, targetDir);
			}
		}
	}

	// 复制文件
	public static void copyFile(File sourceFile, File targetFile)
			throws IOException {
		// 新建文件输入流并对它进行缓冲
		FileInputStream input = new FileInputStream(sourceFile);
		BufferedInputStream inBuff = new BufferedInputStream(input);

		// 新建文件输出流并对它进行缓冲
		FileOutputStream output = new FileOutputStream(targetFile);
		BufferedOutputStream outBuff = new BufferedOutputStream(output);

		// 缓冲数组
		byte[] b = new byte[1024 * 5];
		int len;
		while ((len = inBuff.read(b)) != -1) {
			outBuff.write(b, 0, len);
		}
		// 刷新此缓冲的输出流
		outBuff.flush();

		// 关闭流
		inBuff.close();
		outBuff.close();
		output.close();
		input.close();
	}

	// 复制文件夹
	public static void copyDirectiory(String sourceDir, String targetDir)
			throws IOException {
		// 新建目标目录
		(new File(targetDir)).mkdirs();
		// 获取源文件夹当前下的文件或目录
		File[] file = (new File(sourceDir)).listFiles();
		for (int i = 0; i < file.length; i++) {
			if (file[i].isFile()) {
				// 源文件
				File sourceFile = file[i];
				// 目标文件
				File targetFile = new File(
						new File(targetDir).getAbsolutePath() + File.separator
								+ file[i].getName());
				copyFile(sourceFile, targetFile);
			}
			if (file[i].isDirectory()) {
				// 准备复制的源文件夹
				String dir1 = sourceDir + "/" + file[i].getName();
				// 准备复制的目标文件夹
				String dir2 = targetDir + "/" + file[i].getName();
				copyDirectiory(dir1, dir2);
			}
		}
	}
}
