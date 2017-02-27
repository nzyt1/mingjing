/*
 * =====================================================================================
 *
 *       Filename:  core.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2017年02月15日 18时38分18秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ni Zhiyang (), nizyt1@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <openssl/rand.h>
#include <openssl/md5.h>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>

#include "data.h"
#include "debug.h"

/*	generate seed of random */
int seed_gene(unsigned char *app, unsigned char *username, unsigned char *seed)
{
	PDEBUG("Enter seed_gene\n");

	strcat(seed, app);
	strcat(seed, username);
	return 0;
}

/* generate random number which length is random_len*/
int random_num_gene(unsigned char *seed, unsigned char *random_num, int random_len)
{
	PDEBUG("Enter random_num_gene\n");
	RAND_seed(seed, strlen(seed));
	//RAND_set_rand_method(RAND_get_rand_method());

	while(1)
	{
		if(RAND_status())
		{
			break;
		}else{
			RAND_poll();
		}
	}

	RAND_bytes(random_num, random_len);

	PDEBUG("Leaver random_num_gene\n");

	return 0;
}

/* generate number of three box */
int box_num_gene(int box_num[BOX_NUM], unsigned char random[BOX_NUM], int pwd_len)
{
	PDEBUG("Enter box_num_gene\n");
	box_num[1] = (int)random[1]%(pwd_len-2)+1;
	box_num[2] = (int)random[2]%2+1;
	box_num[0] = (int)(pwd_len - box_num[1] - box_num[2]);
	return 0;
}

/* generate strong password */
int pword_gene(unsigned char *seed, unsigned char *password, int box_num[BOX_NUM], int pwd_len)
{
	PDEBUG("Enter pword_gene\n");
	unsigned char *pwd = (unsigned char*)malloc((pwd_len+1) * sizeof(unsigned char));
	memset(pwd, '\0', pwd_len+1);
	unsigned char *random_num = (unsigned char*)malloc(pwd_len * sizeof(unsigned char));

	random_num_gene(seed, random_num, pwd_len);
	PDEBUG("random_num: %s\n", random_num);

	for(int i=0; i<box_num[0];i++)
		*(pwd+i) = wBox[(random_num[i] % WBOX_LEN)];
	for(int i=0; i<box_num[1];i++)
		*(pwd+box_num[0]+i) = nBox[(random_num[i+box_num[0]] % NBOX_LEN)];
	for(int i=0; i<box_num[2];i++)
		*(pwd+box_num[0]+box_num[1]+i) = sBox[(random_num[i+box_num[0]+box_num[1]] % SBOX_LEN)];
	
	memset(password, '\0', pwd_len+1);
	memcpy(password,pwd,pwd_len);

	free(random_num);
	free(pwd);

	return 0;

}

/*  adjust the order of the pwd */
int adj_order(unsigned char *pwd)
{
	PDEBUG("Enter adj_order\n");
	int length = strlen(pwd);
	int idx = 0;

	srand(time(NULL));
	
	while(length)
	{
		idx = rand()%length;
		if(idx == length - 1)
		{
			//do nothing
		}else{
			//swap the array
			unsigned char tmp = *(pwd + length - 1);
			*(pwd + length - 1) = *(pwd + idx);
			*(pwd + idx) = tmp;
		}
		length --;
	}	
}

int pwd_gene(unsigned char *app, unsigned char *username, unsigned char *password, int pwd_len)
{
	PDEBUG("Enter pwd_gene\n");

	int box_num[BOX_NUM];

	unsigned char *seed = (unsigned char*)malloc((strlen(app)+strlen(username)+1)*sizeof(unsigned char));
	memset(seed, '\0', strlen(app)+strlen(username)+1);
	unsigned char *random_num = (unsigned char*)malloc(BOX_NUM * sizeof(unsigned char));

	seed_gene(app, username, seed);

	random_num_gene(seed, random_num, BOX_NUM);
	PDEBUG("random_num: %s\n", random_num);

	box_num_gene(box_num, random_num, pwd_len);

	pword_gene(seed, password, box_num, pwd_len); 

	PDEBUG("BEFOR ADJ: %s\n", password);

	adj_order(password);

	PDEBUG("AFTER ADJ: %s\n", password);

	free(seed);
	free(random_num);

	return 0;
}

int encrypt(unsigned char *in, unsigned char *out, unsigned char *key)
{
	PDEBUG("Enter encrypt function\n");
	if(strlen(key) < 0)
		return 0;

	unsigned char cleartext[AES_ARR_LEN];
	unsigned char ciphertext[AES_ARR_LEN];
	int round;
	int tail = strlen(in) % AES_ARR_LEN;
	if( tail != 0)
		round = strlen(in)/AES_ARR_LEN + 1;
	else
		round = strlen(in)/AES_ARR_LEN;

	AES_KEY aes_key;
	AES_set_encrypt_key(key, AES_KEY_LEN, &aes_key);

	int idx = 0;
	while(idx < round)
	{
		memset(cleartext, '\0', AES_ARR_LEN);
		if((idx+1) == round)
			memcpy(cleartext, in+idx*AES_ARR_LEN, tail);
		else
			memcpy(cleartext, in+idx*AES_ARR_LEN, AES_ARR_LEN);
		PDEBUG("cleartext: %s\n", cleartext);
		AES_encrypt(cleartext, ciphertext, &aes_key);
		PDEBUG("ciphertext: %s\n", ciphertext);
		memcpy(out + idx * AES_ARR_LEN, ciphertext, AES_ARR_LEN);
		idx ++;
	}

	return 1;
}

int decrypt(unsigned char *in, unsigned char *out, unsigned char *key)
{
	PDEBUG("Enter encrypt function\n");
	if(strlen(key) < 0)
		return 0;

	unsigned char cleartext[AES_ARR_LEN];
	unsigned char ciphertext[AES_ARR_LEN];
	int round;
	round = strlen(in)/AES_ARR_LEN;

	AES_KEY aes_key;
	AES_set_decrypt_key(key, AES_KEY_LEN, &aes_key);

	int idx = 0;
	while(idx < round)
	{
		memset(ciphertext, '\0', AES_ARR_LEN);
		memcpy(ciphertext, in + idx * AES_ARR_LEN, AES_ARR_LEN);
		PDEBUG("ciphertext: %s\n", ciphertext);
		AES_decrypt(ciphertext, cleartext, &aes_key);
		PDEBUG("cleartext: %s\n", cleartext);
		memcpy(out + idx * AES_ARR_LEN, cleartext, AES_ARR_LEN);
		idx ++;
	}

	return 1;
}

int base64encode(unsigned char *in, unsigned char *out)
{
	BIO *bmem, *b64;
	BUF_MEM *bptr;

	b64 = BIO_new(BIO_f_base64());
	BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);

	bmem = BIO_new(BIO_s_mem());
	b64 = BIO_push(b64, bmem);

	BIO_write(b64, in, strlen(in));
	BIO_flush(b64);
	BIO_get_mem_ptr(b64, &bptr);

	char *buff = (char*)malloc(bptr->length + 1);
	buff[bptr->length] = 0;

	BIO_free_all(b64);

	return 1;
}

int base64decode(unsigned char *in, unsigned char *out)
{

}
