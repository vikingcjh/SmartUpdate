package com.soul.learn.smartupdate;

import java.io.UnsupportedEncodingException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

/**
 * MD5加密工具类
 */
public class MD5Utils {
	public static String MD5(String source) {
		String resultHash = null;
		MessageDigest md5;
		try {
			md5 = MessageDigest.getInstance("MD5");
			byte[] result = new byte[md5.getDigestLength()];
			md5.reset();
			md5.update(source.getBytes("UTF-8"));
			result = md5.digest();
			StringBuffer buf = new StringBuffer(result.length * 2);

			for (int i = 0; i < result.length; i++) {
				int intVal = result[i] & 0xff;
				if (intVal < 0x10) {
					buf.append("0");
				}
				buf.append(Integer.toHexString(intVal));
			}

			resultHash = buf.toString();
		} catch (NoSuchAlgorithmException e) {
			e.printStackTrace();
		} catch (UnsupportedEncodingException e) {
			e.printStackTrace();
		}

		

		return resultHash;

	}
}
