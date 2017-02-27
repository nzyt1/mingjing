/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  the main part of the programe
 *
 *        Version:  1.0
 *        Created:  2017年02月1日 19时02分39秒
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"
#include "core.h"
#include "debug.h"

const static char *programe_name = "mingjing";

static int usage(char **argv)
{
	printf("Usage: %s [OPTION] -f APPNAME [USERNAME]\t find the strong password\n"
		   "   or: %s [OPTION] -g APPNAME USERNAME  \t generate the strong password\n"
		   "\t-l, --list \t list all the app and password \n",
			argv[0], argv[0]);
	return EXIT_FAILURE;
}

#define MAX_COMMAND_FLAGS 1

int main(int argc, char *argv[])
{
	int start_argv = 1;
	size_t i;
	unsigned char *appname = NULL;
	unsigned char *username = NULL;
	const char *prefix=NULL;

	PFUNC();
	
	for(i = 0; i < MAX_COMMAND_FLAGS; i++)
	{
		if(start_argv < argc && argv[start_argv][0] == '-')
		{
			if(argv[start_argv][1] == 'f')
			{
				PDEBUG("Enter find function\n");
				if(start_argv + 1 < argc)
					appname = argv[start_argv + 1];
				if(start_argv + 2 < argc)
					username = argv[start_argv + 2];
				PDEBUG("APPNAME: %s\t", appname);
				PDEBUG("USERNAME: %s\n", username);
			}else if(argv[start_argv][1] == 'g'){
				PDEBUG("Enter generate function\n");
				if(start_argv + 1 < argc)
					appname = argv[start_argv + 1];
				if(start_argv + 2 < argc)
					username = argv[start_argv + 2];
				else
					return usage(argv);

				int pwd_len =25;

				unsigned char *password = (unsigned char *)malloc((pwd_len + 1) * sizeof(unsigned char));
				memset(password, '\0', pwd_len+1);

				pwd_gene(appname, username, password, pwd_len);

				int len_cip = (strlen(password)/AES_ARR_LEN+1)*AES_ARR_LEN;
				unsigned char *ciphertext = (unsigned char *)malloc((len_cip+1)*sizeof(unsigned char));
				memset(ciphertext, '\0', len_cip+1);

				unsigned char *key = "0123456789ABCDEF";

				encrypt(password, ciphertext, key);
				PDEBUG("CIPHER: %s\n", ciphertext);
				char *base64code;
				Base64Encode(ciphertext, &base64code);
				PDEBUG("BASE64: %s\n",base64code);

				insert(appname, username, base64code);

				char *tmp;
				Base64Decode(base64code, &tmp);
				PDEBUG("BASE DECODE: %s\n", tmp);

				decrypt(tmp, password, key);
				PDEBUG("CLEAR:  %s\n", password);

				free(password);
			}
		}else{
			break;
		}
	}
	if(start_argv >= argc)
		return usage(argv);

	return 0;
}

