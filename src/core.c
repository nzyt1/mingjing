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
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>

#include "debug.h"
#include "core.h"

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
