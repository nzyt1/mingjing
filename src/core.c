/*
 * =====================================================================================
 *
 *       Filename:  fileUtil.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2017年02月19日 23时33分38秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ni Zhiyang (), nizyt1@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>

#include "debug.h"
#include "data.h"
#include "core.h"

int generate(unsigned char *app, unsigned char *username, unsigned char *b64_pwd, int pwd_len)
{
	unsigned char *password = (unsigned char *)malloc((pwd_len + 1) * sizeof(unsigned char));
	memset(password, '\0', pwd_len+1);

	pwd_gene(app, username, password, pwd_len);
	
	int len_cip = (strlen(password)/AES_ARR_LEN+1)*AES_ARR_LEN;
	unsigned char *ciphertext = (unsigned char *)malloc((len_cip + 1)*sizeof(unsigned char ));
	memset(ciphertext, '\0', len_cip + 1);
	
	unsigned char *key = "0123456789ABCDEF";

	encrypt(password, ciphertext, key);
	PDEBUG("CIPHER: %s\n", ciphertext);
	Base64Encode(ciphertext, &b64_pwd);
	PDEBUG("BASE64: %s\n", b64_pwd);
	free(password);
	free(ciphertext);

}

int insert(unsigned char *app, unsigned char *username, unsigned char *password)
{
	unsigned char *buffer;
	int length = strlen(app)+strlen(username)+strlen(password) + 4;
	buffer = (unsigned char *)malloc(length*sizeof(unsigned char));
	memset(buffer, '\0', length);
	strcat(buffer, app);
	strcat(buffer, ":");
	strcat(buffer, username);
	strcat(buffer, ":");
	strcat(buffer, password);
	strcat(buffer, "\n");

	PDEBUG("buffer: %s\n", buffer);

	if(strlen(buffer) < 1)
	{
		PDEBUG("BUFFER_EMPTY\n");
		return BUFFER_EMPTY;
	}

	unsigned char *home = getenv("HOME");
	PDEBUG("HOME: %s\n", home);

	int pwd_path_len = strlen(home) + strlen(PWDDIR) + strlen(PWDFILE) + 3;
	unsigned char *pwdpath = (unsigned char *)malloc((strlen(home) + strlen(PWDDIR) + strlen(PWDFILE) + 2)*sizeof(unsigned char));

	memset(pwdpath, '\0', pwd_path_len);
	strcat(pwdpath, home);
	strcat(pwdpath, "/");
	strcat(pwdpath, PWDDIR);

	PDEBUG("Path: %s\n", pwdpath);

	if(access(pwdpath, F_OK) < 0)
	{
		PDEBUG("DIR not exsit\n");
		mkdir(pwdpath, DIR_MASK);
	}else{
		PDEBUG("DIR exsit\n");
	}

	strcat(pwdpath, "/");
	strcat(pwdpath, PWDFILE);

	PDEBUG("PWDFile: %s\n", PWDFILE);

	int fd;
	if( (fd = open(pwdpath, O_RDWR | O_APPEND | O_CREAT , FILE_MASK)) == -1)
	{
		PDEBUG("FILE_NOT_EXSIT: %s %d\n", PWDFILE, fd);
		return FILE_NOT_EXSIT;
	}
	
	write(fd, buffer, strlen(buffer));
	
	close(fd);

	return INSERT_SUCCESS;
}

int search(unsigned char *app, unsigned char *username)
{
	if(true)
		return USER_EXSIT;
	else
		return USER_NOT_EXSIT;
}

int update(unsigned char *app, unsigned char *username, unsigned char *password)
{

}

int delete(unsigned char *app, unsigned char *username)
{

}
