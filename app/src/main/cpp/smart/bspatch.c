/*-
 * Copyright 2003-2005 Colin Percival
 * All rights reserved
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted providing that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */



#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <err.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>    // android
#include <android/log.h>	// To be able to print to logcat

#include "bspatch.h"
#include "bzlib.h"


static off_t offtin(u_char *buf)
{
	off_t y;

	y=buf[7]&0x7F;
	y=y*256;y+=buf[6];
	y=y*256;y+=buf[5];
	y=y*256;y+=buf[4];
	y=y*256;y+=buf[3];
	y=y*256;y+=buf[2];
	y=y*256;y+=buf[1];
	y=y*256;y+=buf[0];

	if(buf[7]&0x80) y=-y;

	return y;
}

int bspatch(char **argv)
{

	FILE * f, * cpf, * dpf, * epf;
	BZFILE * cpfbz2, * dpfbz2, * epfbz2;
	int cbz2err, dbz2err, ebz2err;
	int fd;
	ssize_t oldsize,newsize;
	ssize_t bzctrllen,bzdatalen;
	u_char header[32],buf[8];
	u_char *old, *new;
	off_t oldpos,newpos;
	off_t ctrl[3];
	off_t lenread;
	off_t i;

	// !!! CHANGE !!!
	int 	argc;
	int		loopVar;

	argc = 4;

	__android_log_print(ANDROID_LOG_INFO, "bspatch.c", "%s",argv[0]);
	__android_log_print(ANDROID_LOG_INFO, "bspatch.c", "%s",argv[1]);
	/*__android_log_print(ANDROID_LOG_INFO, "bspatch.c", argv[0]);s
	__android_log_print(ANDROID_LOG_INFO, "bspatch.c", argv[1]);
	__android_log_print(ANDROID_LOG_INFO, "bspatch.c", argv[2]);
	__android_log_print(ANDROID_LOG_INFO, "bspatch.c", argv[3]);*/

	// !!! END CHANGE !!!

	if(argc!=4) errx(1,"usage: %s oldfile newfile patchfile\n",argv[0]);

	// !!! CHANGE !!!

	__android_log_print(ANDROID_LOG_INFO, "bspatch.c", "Point 0");

	/* Open patch file */
	f = fopen(argv[3], "r");
	if (f == NULL)
	{
		__android_log_print(ANDROID_LOG_INFO, "bspatch.c ERROR",
				"%s",argv[3]);
		return (-1);
	}
	else
	{
		__android_log_print(ANDROID_LOG_INFO, "bspatch.c",
				"Patch is opened");
	}

	// !!! END CHANGE !!!

	/* Open patch file */
	//if ((f = fopen(argv[3], "r")) == NULL)
		//err(1, "fopen(%s)", argv[3]);

	/*
	File format:
		0	8	"BSDIFF40"
		8	8	X
		16	8	Y
		24	8	sizeof(newfile)
		32	X	bzip2(control block)
		32+X	Y	bzip2(diff block)
		32+X+Y	???	bzip2(extra block)
	with control block a set of triples (x,y,z) meaning "add x bytes
	from oldfile to x bytes from the diff block; copy y bytes from the
	extra block; seek forwards in oldfile by z bytes".
	*/

	// !!! CHANGE !!!

	__android_log_print(ANDROID_LOG_INFO, "bspatch.c", "Point 1");

	// !!! END CHANGE !!!

	/* Read header */
	if (fread(header, 1, 32, f) < 32) {
		if (feof(f))
			errx(1, "Corrupt patch\n");
		err(1, "fread(%s)", argv[3]);
	}

	// !!! CHANGE !!!

	__android_log_print(ANDROID_LOG_INFO, "bspatch.c", "Point 2");

	// !!! END CHANGE !!!

	/* Check for appropriate magic */
	if (memcmp(header, "BSDIFF40", 8) != 0)
		errx(1, "Corrupt patch\n");

	// !!! CHANGE !!!

	__android_log_print(ANDROID_LOG_INFO, "bspatch.c", "Point 3");

	// !!! END CHANGE !!!

	/* Read lengths from header */
	bzctrllen=offtin(header+8);
	bzdatalen=offtin(header+16);
	newsize=offtin(header+24);
	if((bzctrllen<0) || (bzdatalen<0) || (newsize<0))
		errx(1,"Corrupt patch\n");

	// !!! CHANGE !!!

	__android_log_print(ANDROID_LOG_INFO, "bspatch.c", "Point 4");

	// !!! END CHANGE !!!

	/* Close patch file and re-open it via libbzip2 at the right places */
	if (fclose(f))
		err(1, "fclose(%s)", argv[3]);
	if ((cpf = fopen(argv[3], "r")) == NULL)
		err(1, "fopen(%s)", argv[3]);
	if (fseeko(cpf, 32, SEEK_SET))
		err(1, "fseeko(%s, %lld)", argv[3],
		    (long long)32);
	if ((cpfbz2 = BZ2_bzReadOpen(&cbz2err, cpf, 0, 0, NULL, 0)) == NULL)
		errx(1, "BZ2_bzReadOpen, bz2err = %d", cbz2err);
	if ((dpf = fopen(argv[3], "r")) == NULL)
		err(1, "fopen(%s)", argv[3]);
	if (fseeko(dpf, 32 + bzctrllen, SEEK_SET))
		err(1, "fseeko(%s, %lld)", argv[3],
		    (long long)(32 + bzctrllen));
	if ((dpfbz2 = BZ2_bzReadOpen(&dbz2err, dpf, 0, 0, NULL, 0)) == NULL)
		errx(1, "BZ2_bzReadOpen, bz2err = %d", dbz2err);
	if ((epf = fopen(argv[3], "r")) == NULL)
		err(1, "fopen(%s)", argv[3]);
	if (fseeko(epf, 32 + bzctrllen + bzdatalen, SEEK_SET))
		err(1, "fseeko(%s, %lld)", argv[3],
		    (long long)(32 + bzctrllen + bzdatalen));
	if ((epfbz2 = BZ2_bzReadOpen(&ebz2err, epf, 0, 0, NULL, 0)) == NULL)
		errx(1, "BZ2_bzReadOpen, bz2err = %d", ebz2err);

	// !!! CHANGE !!!

	__android_log_print(ANDROID_LOG_INFO, "bspatch.c", "Point 5");

	// !!! END CHANGE !!!

	if(((fd=open(argv[1],O_RDONLY,0))<0) ||
		((oldsize=lseek(fd,0,SEEK_END))==-1) ||
		((old=malloc(oldsize+1))==NULL) ||
		(lseek(fd,0,SEEK_SET)!=0) ||
		(read(fd,old,oldsize)!=oldsize) ||
		(close(fd)==-1)) err(1,"%s",argv[1]);
	if((new=malloc(newsize+1))==NULL) err(1,NULL);

	// !!! CHANGE !!!

	__android_log_print(ANDROID_LOG_INFO, "bspatch.c", "Point 6");

	// !!! END CHANGE !!!

	oldpos=0;newpos=0;
	while(newpos<newsize) {
		/* Read control data */
		for(i=0;i<=2;i++) {
			lenread = BZ2_bzRead(&cbz2err, cpfbz2, buf, 8);
			if ((lenread < 8) || ((cbz2err != BZ_OK) &&
			    (cbz2err != BZ_STREAM_END)))
				errx(1, "Corrupt patch\n");
			ctrl[i]=offtin(buf);
		};

		/* Sanity-check */
		if(newpos+ctrl[0]>newsize)
			errx(1,"Corrupt patch\n");

		/* Read diff string */
		lenread = BZ2_bzRead(&dbz2err, dpfbz2, new + newpos, ctrl[0]);
		if ((lenread < ctrl[0]) ||
		    ((dbz2err != BZ_OK) && (dbz2err != BZ_STREAM_END)))
			errx(1, "Corrupt patch\n");

		/* Add old data to diff string */
		for(i=0;i<ctrl[0];i++)
			if((oldpos+i>=0) && (oldpos+i<oldsize))
				new[newpos+i]+=old[oldpos+i];

		/* Adjust pointers */
		newpos+=ctrl[0];
		oldpos+=ctrl[0];

		/* Sanity-check */
		if(newpos+ctrl[1]>newsize)
			errx(1,"Corrupt patch\n");

		/* Read extra string */
		lenread = BZ2_bzRead(&ebz2err, epfbz2, new + newpos, ctrl[1]);
		if ((lenread < ctrl[1]) ||
		    ((ebz2err != BZ_OK) && (ebz2err != BZ_STREAM_END)))
			errx(1, "Corrupt patch\n");

		/* Adjust pointers */
		newpos+=ctrl[1];
		oldpos+=ctrl[2];
	};

	// !!! CHANGE !!!

	__android_log_print(ANDROID_LOG_INFO, "bspatch.c", "Point 7");

	// !!! END CHANGE !!!

	/* Clean up the bzip2 reads */
	BZ2_bzReadClose(&cbz2err, cpfbz2);
	BZ2_bzReadClose(&dbz2err, dpfbz2);
	BZ2_bzReadClose(&ebz2err, epfbz2);
	if (fclose(cpf) || fclose(dpf) || fclose(epf))
		err(1, "fclose(%s)", argv[3]);

	// !!! CHANGE !!!

	__android_log_print(ANDROID_LOG_INFO, "bspatch.c", "Point 8");

	// !!! END CHANGE !!!

	/* Write the new file */
	if(((fd=open(argv[2],O_CREAT|O_TRUNC|O_WRONLY,0666))<0) ||
		(write(fd,new,newsize)!=newsize) || (close(fd)==-1))
		err(1,"%s",argv[2]);

	// !!! CHANGE !!!

	__android_log_print(ANDROID_LOG_INFO, "bspatch.c", "Point 9");

	// !!! END CHANGE !!!

	free(new);
	free(old);

	// !!! CHANGE !!!

	__android_log_print(ANDROID_LOG_INFO, "bspatch.c", "Point 10");



	// !!! END CHANGE !!!

	__android_log_print(ANDROID_LOG_INFO, "bspatch.c", "理论上此处就成功了");


	return 0;
}

int bspatch2(char **argv)
{

	FILE * f, * cpf, * dpf, * epf;
	BZFILE * cpfbz2, * dpfbz2, * epfbz2;
	int cbz2err, dbz2err, ebz2err;
	int fd;
	ssize_t oldsize,newsize;
	ssize_t bzctrllen,bzdatalen;
	u_char header[32],buf[8];
	u_char *old, *new;
	off_t oldpos,newpos;
	off_t ctrl[3];
	off_t lenread;
	off_t i;

	// !!! CHANGE !!!
	int 	argc;
	int		loopVar;

	argc = 4;

	__android_log_print(ANDROID_LOG_INFO, "bspatch.c", "%s",argv[0]);
	__android_log_print(ANDROID_LOG_INFO, "bspatch.c", "%s",argv[1]);

	// !!! END CHANGE !!!

	if(argc!=4) errx(1,"usage: %s oldfile newfile patchfile\n",argv[0]);

	// !!! CHANGE !!!

	__android_log_print(ANDROID_LOG_INFO, "bspatch.c", "Point 0");

	/* Open patch file */
	f = fopen(argv[3], "r");
	if (f == NULL)
	{
		__android_log_print(ANDROID_LOG_INFO, "bspatch.c ERROR",
				"%s",argv[3]);
		return (-1);
	}
	else
	{
		__android_log_print(ANDROID_LOG_INFO, "bspatch.c",
				"Patch is opened");
	}

	// !!! END CHANGE !!!

	/* Open patch file */
	//if ((f = fopen(argv[3], "r")) == NULL)
		//err(1, "fopen(%s)", argv[3]);

	/*
	File format:
		0	8	"BSDIFF40"
		8	8	X
		16	8	Y
		24	8	sizeof(newfile)
		32	X	bzip2(control block)
		32+X	Y	bzip2(diff block)
		32+X+Y	???	bzip2(extra block)
	with control block a set of triples (x,y,z) meaning "add x bytes
	from oldfile to x bytes from the diff block; copy y bytes from the
	extra block; seek forwards in oldfile by z bytes".
	*/

	/* Read header */
	if (fread(header, 1, 32, f) < 32) {
		if (feof(f))
			errx(1, "Corrupt patch\n");
		err(1, "fread(%s)", argv[3]);
	}

	/* Check for appropriate magic */
	if (memcmp(header, "BSDIFF40", 8) != 0)
		errx(1, "Corrupt patch\n");

	/* Read lengths from header */
	bzctrllen=offtin(header+8);
	bzdatalen=offtin(header+16);
	newsize=offtin(header+24);
	if((bzctrllen<0) || (bzdatalen<0) || (newsize<0))
		errx(1,"Corrupt patch\n");

	/* Close patch file and re-open it via libbzip2 at the right places */
	if (fclose(f))
		err(1, "fclose(%s)", argv[3]);
	if ((cpf = fopen(argv[3], "r")) == NULL)
		err(1, "fopen(%s)", argv[3]);
	if (fseeko(cpf, 32, SEEK_SET))
		err(1, "fseeko(%s, %lld)", argv[3],
		    (long long)32);
	if ((cpfbz2 = BZ2_bzReadOpen(&cbz2err, cpf, 0, 0, NULL, 0)) == NULL)
		errx(1, "BZ2_bzReadOpen, bz2err = %d", cbz2err);
	if ((dpf = fopen(argv[3], "r")) == NULL)
		err(1, "fopen(%s)", argv[3]);
	if (fseeko(dpf, 32 + bzctrllen, SEEK_SET))
		err(1, "fseeko(%s, %lld)", argv[3],
		    (long long)(32 + bzctrllen));
	if ((dpfbz2 = BZ2_bzReadOpen(&dbz2err, dpf, 0, 0, NULL, 0)) == NULL)
		errx(1, "BZ2_bzReadOpen, bz2err = %d", dbz2err);
	if ((epf = fopen(argv[3], "r")) == NULL)
		err(1, "fopen(%s)", argv[3]);
	if (fseeko(epf, 32 + bzctrllen + bzdatalen, SEEK_SET))
		err(1, "fseeko(%s, %lld)", argv[3],
		    (long long)(32 + bzctrllen + bzdatalen));
	if ((epfbz2 = BZ2_bzReadOpen(&ebz2err, epf, 0, 0, NULL, 0)) == NULL)
		errx(1, "BZ2_bzReadOpen, bz2err = %d", ebz2err);

	if(((fd=open(argv[1],O_RDONLY,0))<0) ||
		((oldsize=lseek(fd,0,SEEK_END))==-1) ||
		((old=malloc(oldsize+1))==NULL) ||
		(lseek(fd,0,SEEK_SET)!=0) ||
		(read(fd,old,oldsize)!=oldsize) ||
		(close(fd)==-1)) err(1,"%s",argv[1]);
//	if((new=malloc(newsize+1))==NULL) err(1,NULL);

	__android_log_print(ANDROID_LOG_INFO, "bspatch.c","Malloc succeed");

	oldpos=0;newpos=0;
	/* Write the new file */
	if((fd=open(argv[2],O_CREAT|O_TRUNC|O_WRONLY,0666))<0)
		err(1,"%s",argv[2]);
	while(newpos<newsize) {
		/* Read control data */
		for(i=0;i<=2;i++) {
			lenread = BZ2_bzRead(&cbz2err, cpfbz2, buf, 8);
			if ((lenread < 8) || ((cbz2err != BZ_OK) &&
			    (cbz2err != BZ_STREAM_END)))
				errx(1, "Corrupt patch\n");
			ctrl[i]=offtin(buf);
		};

		__android_log_print(ANDROID_LOG_INFO, "bspatch.c", "ctrl[0]=%d",ctrl[0]);
		__android_log_print(ANDROID_LOG_INFO, "bspatch.c", "ctrl[1]=%d",ctrl[1]);
//		__android_log_print(ANDROID_LOG_INFO, "bspatch.c", "ctrl[2]=%d",ctrl[2]);
		if (ctrl[0]>ctrl[1]) {
			if((new=malloc(ctrl[0]+1))==NULL) err(1,NULL);
		}else{
			if((new=malloc(ctrl[1]+1))==NULL) err(1,NULL);
		}

		/* Sanity-check */
		if(newpos+ctrl[0]>newsize)
			errx(1,"Corrupt patch\n");

		/* Read diff string */
//		off_t blocksize = ctrl[0];
//		if((new=malloc(ctrl[0]+1))==NULL) err(1,NULL);
		lenread = 0;
//		__android_log_print(ANDROID_LOG_INFO, "bspatch.c", "beforeparse");
		while(lenread<ctrl[0]){
			off_t pagesize = BZ2_bzRead(&dbz2err, dpfbz2, new, ctrl[0]);
			if ((pagesize < ctrl[0]) || ((dbz2err != BZ_OK) && (dbz2err != BZ_STREAM_END)))
				errx(1, "Corrupt patch\n");
//			__android_log_print(ANDROID_LOG_INFO, "bspatch.c", "pagesize=%d",pagesize);

			/* Add old data to diff string */
			for(i=0;i<pagesize;i++)
			{
				if((oldpos+i>=0) && (oldpos+i<oldsize))
					new[i]+=old[oldpos+i];
			}
			if (ctrl[0]>0) {
				if (lseek(fd,newpos,SEEK_SET)!=newpos) {
					errx(1, "Corrupt patch\n");
				}
				if((write(fd,new,pagesize)!=pagesize))
					err(1,"%s",argv[2]);
			}

			lenread += pagesize;

			/* Adjust pointers */
			newpos+=pagesize;
			oldpos+=pagesize;
		}

//		/* Adjust pointers */
//		newpos+=ctrl[0];
//		oldpos+=ctrl[0];

		/* Sanity-check */
		if(newpos+ctrl[1]>newsize)
			errx(1,"Corrupt patch\n");

		/* Read extra string */
		lenread = BZ2_bzRead(&ebz2err, epfbz2, new, ctrl[1]);
		if ((lenread < ctrl[1]) ||
		    ((ebz2err != BZ_OK) && (ebz2err != BZ_STREAM_END)))
			errx(1, "Corrupt patch\n");
		if (ctrl[1]>0) {
			if (lseek(fd,newpos,SEEK_SET)!=newpos) {
				errx(1, "Corrupt patch\n");
			}
			if((write(fd,new,ctrl[1])!=ctrl[1]))
				err(1,"%s",argv[2]);
		}

		/* Adjust pointers */
		newpos+=ctrl[1];
		oldpos+=ctrl[2];

		free(new);
	};

	/* Clean up the bzip2 reads */
	BZ2_bzReadClose(&cbz2err, cpfbz2);
	BZ2_bzReadClose(&dbz2err, dpfbz2);
	BZ2_bzReadClose(&ebz2err, epfbz2);
	if (fclose(cpf) || fclose(dpf) || fclose(epf))
		err(1, "fclose(%s)", argv[3]);

	/* Write the new file */
//	if(((fd=open(argv[2],O_CREAT|O_TRUNC|O_WRONLY,0666))<0) ||
//		(write(fd,new,newsize)!=newsize) || (close(fd)==-1))
//		err(1,"%s",argv[2]);

	if (lseek(fd,0,SEEK_SET)!=0) {
		errx(1, "Corrupt patch\n");
	}
	if(close(fd)==-1)
		err(1,"%s",argv[2]);
//	if (new != NULL) {
//		free(new);
//	}

	free(old);

	__android_log_print(ANDROID_LOG_INFO, "bspatch.c", "Point 10");

	return 0;
}

int bspatch3(char **argv)
{

	FILE * f, * cpf, * dpf, * epf;
	BZFILE * cpfbz2, * dpfbz2, * epfbz2;
	int cbz2err, dbz2err, ebz2err;
	int oldfd,newfd;
	ssize_t oldsize,newsize;
	ssize_t bzctrllen,bzdatalen;
	u_char header[32],buf[8];
	u_char *old, *new;
	off_t oldpos,newpos;
	off_t ctrl[3];
	off_t lenread;
	off_t i;

	off_t new_malloc_size;
	off_t old_malloc_size;

	// !!! CHANGE !!!
	int 	argc;
	int		loopVar;

	argc = 4;

//	__android_log_print(ANDROID_LOG_INFO, "bspatch.c", "%s",argv[0]);
//	__android_log_print(ANDROID_LOG_INFO, "bspatch.c", "%s",argv[1]);

	// !!! END CHANGE !!!

	if(argc!=4)
		return -1;

	/* Open patch file */
	f = fopen(argv[3], "r");
	if (f == NULL)
	{
		return (-1);
	}

	/*
	File format:
		0	8	"BSDIFF40"
		8	8	X
		16	8	Y
		24	8	sizeof(newfile)
		32	X	bzip2(control block)
		32+X	Y	bzip2(diff block)
		32+X+Y	???	bzip2(extra block)
	with control block a set of triples (x,y,z) meaning "add x bytes
	from oldfile to x bytes from the diff block; copy y bytes from the
	extra block; seek forwards in oldfile by z bytes".
	*/

	/* Read header */
	if (fread(header, 1, 32, f) < 32) {
		if (feof(f)){
			fclose(f);
			return -1;
		}
		fclose(f);
		return -1;
	}

	/* Check for appropriate magic */
	if (memcmp(header, "BSDIFF40", 8) != 0)
		return (-1);

	/* Read lengths from header */
	bzctrllen=offtin(header+8);
	bzdatalen=offtin(header+16);
	newsize=offtin(header+24);
	if((bzctrllen<0) || (bzdatalen<0) || (newsize<0))
		return (-1);

	/* Close patch file and re-open it via libbzip2 at the right places */
	if (fclose(f))
		return (-1);
	if ((cpf = fopen(argv[3], "r")) == NULL)
		return (-1);
	if (fseeko(cpf, 32, SEEK_SET))
		return (-1);
	if ((cpfbz2 = BZ2_bzReadOpen(&cbz2err, cpf, 0, 0, NULL, 0)) == NULL)
		return (-1);
	if ((dpf = fopen(argv[3], "r")) == NULL)
		return (-1);
	if (fseeko(dpf, 32 + bzctrllen, SEEK_SET))
		return (-1);
	if ((dpfbz2 = BZ2_bzReadOpen(&dbz2err, dpf, 0, 0, NULL, 0)) == NULL)
		return (-1);
	if ((epf = fopen(argv[3], "r")) == NULL)
		return (-1);
	if (fseeko(epf, 32 + bzctrllen + bzdatalen, SEEK_SET))
		return (-1);
	if ((epfbz2 = BZ2_bzReadOpen(&ebz2err, epf, 0, 0, NULL, 0)) == NULL)
		return (-1);

	if(((oldfd=open(argv[1],O_RDONLY,0))<0) ||
		((oldsize=lseek(oldfd,0,SEEK_END))==-1))
		return (-1);

	oldpos=0;newpos=0;
	new_malloc_size = 0;
	old_malloc_size = 0;
	/* Write the new file */
	if((newfd=open(argv[2],O_CREAT|O_TRUNC|O_WRONLY,0666))<0)
		return (-1);
	while(newpos<newsize) {
		/* Read control data */
		for(i=0;i<=2;i++) {
			lenread = BZ2_bzRead(&cbz2err, cpfbz2, buf, 8);
			if ((lenread < 8) || ((cbz2err != BZ_OK) &&
			    (cbz2err != BZ_STREAM_END)))
				return (-1);
			ctrl[i]=offtin(buf);
		};

//		__android_log_print(ANDROID_LOG_INFO, "bspatch.c", "ctrl[0]=%d",ctrl[0]);
//		__android_log_print(ANDROID_LOG_INFO, "bspatch.c", "ctrl[1]=%d",ctrl[1]);
//		__android_log_print(ANDROID_LOG_INFO, "bspatch.c", "ctrl[2]=%d",ctrl[2]);
		if (ctrl[0]>ctrl[1]) {
			if (ctrl[0]+1>new_malloc_size && (new_malloc_size!=0)) {
				if((new=realloc(new,ctrl[0]+1))==NULL)
					return (-1);
				new_malloc_size = ctrl[0]+1;
			}else if (new_malloc_size==0) {
				if((new=malloc(ctrl[0]+1))==NULL)
					return (-1);
				new_malloc_size = ctrl[0]+1;
			}
		}else{
			if (ctrl[1]+1>new_malloc_size && (new_malloc_size!=0)) {
				if((new=realloc(new,ctrl[1]+1))==NULL)
					return (-1);
				new_malloc_size = ctrl[1]+1;
			}else if (new_malloc_size==0) {
				if((new=malloc(ctrl[1]+1))==NULL)
					return (-1);
				new_malloc_size = ctrl[1]+1;
			}
		}

		if (ctrl[0]+1>old_malloc_size && (old_malloc_size!=0)) {
			if((old=realloc(old,ctrl[0]+1))==NULL)
				return (-1);
			old_malloc_size = ctrl[0]+1;
		}else if(old_malloc_size==0){
			if((old=malloc(ctrl[0]+1))==NULL)
				return (-1);
			old_malloc_size = ctrl[0]+1;
		}
//		if((old=malloc(ctrl[0]+1))==NULL) return (-1);

		if (lseek(oldfd,oldpos,SEEK_SET)==-1) {
			return (-1);
		}
		if((read(oldfd,old,ctrl[0])!=ctrl[0]))
			return (-1);


		/* Sanity-check */
		if(newpos+ctrl[0]>newsize)
			return (-1);

		/* Read diff string */
		lenread = 0;
//		__android_log_print(ANDROID_LOG_INFO, "bspatch.c", "beforeparse");
		while(lenread<ctrl[0]){
			off_t pagesize = BZ2_bzRead(&dbz2err, dpfbz2, new, ctrl[0]);
//			if ((pagesize < ctrl[0]) || ((dbz2err != BZ_OK) && (dbz2err != BZ_STREAM_END)))
//				errx(1, "Corrupt patch\n");
//			__android_log_print(ANDROID_LOG_INFO, "bspatch.c", "pagesize=%d",pagesize);

			/* Add old data to diff string */
			for(i=0;i<pagesize;i++)
			{
				if((oldpos+i>=0) && (oldpos+i<oldsize))
					new[i]+=old[i];
			}
			if (ctrl[0]>0) {
				if (lseek(newfd,newpos,SEEK_SET)!=newpos) {
					return (-1);
				}
				if((write(newfd,new,pagesize)!=pagesize))
					return (-1);
			}

			lenread += pagesize;

			/* Adjust pointers */
			newpos+=pagesize;
			oldpos+=pagesize;
		}

//		/* Adjust pointers */
//		newpos+=ctrl[0];
//		oldpos+=ctrl[0];

		/* Sanity-check */
		if(newpos+ctrl[1]>newsize)
			return (-1);

		/* Read extra string */
		lenread = BZ2_bzRead(&ebz2err, epfbz2, new, ctrl[1]);
		if ((lenread < ctrl[1]) ||
		    ((ebz2err != BZ_OK) && (ebz2err != BZ_STREAM_END)))
			return (-1);
		if (ctrl[1]>0) {
			if (lseek(newfd,newpos,SEEK_SET)!=newpos) {
				return (-1);
			}
			if((write(newfd,new,ctrl[1])!=ctrl[1]))
				return (-1);
		}

		/* Adjust pointers */
		newpos+=ctrl[1];
		oldpos+=ctrl[2];

//		free(new);
//		free(old);
	};

	__android_log_print(ANDROID_LOG_INFO, "NIODATA", "old_malloc_size=%d",old_malloc_size);
	__android_log_print(ANDROID_LOG_INFO, "NIODATA", "new_malloc_size=%d",new_malloc_size);

	free(new);
	free(old);

	/* Clean up the bzip2 reads */
	BZ2_bzReadClose(&cbz2err, cpfbz2);
	BZ2_bzReadClose(&dbz2err, dpfbz2);
	BZ2_bzReadClose(&ebz2err, epfbz2);
	if (fclose(cpf) || fclose(dpf) || fclose(epf))
		return (-1);

	if (lseek(newfd,0,SEEK_SET)!=0) {
		return (-1);
	}
	if(close(newfd)==-1)
		return (-1);

	if (lseek(oldfd,0,SEEK_SET)!=0) {
		return (-1);
	}
	if(close(oldfd)==-1)
		return (-1);

//	free(new);
//	free(old);

//	__android_log_print(ANDROID_LOG_INFO, "bspatch.c", "Point 10");

	return 0;
}
