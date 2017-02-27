/*
 * =====================================================================================
 *
 *       Filename:  fileUtil.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2017年02月20日 03时05分07秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ni Zhiyang (), nizyt1@163.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef FILE_UTIL_H
#define FILE_UTIL_H

#define FILE_NOT_EXSIT 0
#define BUFFER_EMPTY 0

#define INSERT_SUCCESS 1
#define INSERT_FAIL 0

#define PWDDIR ".mingjing"
#define DIR_MASK S_IRWXU
#define PWDFILE "mjpasswd"
#define FILE_MASK S_IRUSR | S_IWUSR

#define true 1
#define USER_EXSIT 1
#define USER_NOT_EXSIT 0

int insert(unsigned char *app, unsigned char *username, unsigned char *password);
#endif
