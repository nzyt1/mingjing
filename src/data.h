/*
 * Copyright 2016 - 2030 The MingJing Project Authors. All Rights Reserved.
 *
 * Licensed under the MingJing license (the "License"). You ma not use
 * this file except in compliance with the License. You can obtain a copy
 * in the file LICENSE in the source distribution or at 
 * https://github.com/nzyt1/mingjing/blob/master/LICENSE
 */

#ifndef CORE_H
#define CORE_H

#define BOX_NUM 3
#define WBOX_LEN 52
#define NBOX_LEN 10
#define SBOX_LEN 32

const static unsigned char wBox[] = 
{
	0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
	0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E,
	0x4F, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 
	0x56, 0x57, 0x58, 0x59, 0x5A,
	0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
	0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E,
	0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 
	0x76, 0x77, 0x78, 0x79, 0x7A

};	// A~Z, a~z, 52

const static unsigned char nBox[] =
{
	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39
};	// 0~9, 10 

const static unsigned char sBox[] = 
{
	0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29,
	0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x3A, 0x3B, 0x3C,
	0x3D, 0x3E, 0x3F, 0x40, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
	0x60, 0x7B, 0x7C, 0x7D, 0x7E
};	// 32

//function

int seed_gene(unsigned char *app, unsigned char *username, unsigned char *seed);
int random_num_gene(unsigned char *seed, unsigned char *random_num, int random_len);
int box_num_gene(int box_num[BOX_NUM], unsigned char random[BOX_NUM], int pwd_len);
int pword_gene(unsigned char *seed, unsigned char *password, int box_num[BOX_NUM], int pwd_len);
int adj_order(unsigned char *pwd);
int pwd_gene(unsigned char *app, unsigned char *username, unsigned char *password, int pwd_len);
int hash_gene(unsigned char *app, unsigned char *username, unsigned char *hashcode);

// AES parameter
#define AES_DATA_LEN 128
#define AES_KEY_LEN 128
#define AES_ARR_LEN (AES_DATA_LEN/8)

int encrypt(unsigned char *in, unsigned char *out, unsigned char *key);
int decrypt(unsigned char *in, unsigned char *out, unsigned char *key);

char *base64encode(unsigned char *in);
char *base64decode(unsigned char *in);
int Base64Encode(const char* message, char** buffer); 
int Base64Decode(char* b64message, char** buffer); 

#endif
