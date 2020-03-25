#pragma once

#ifndef  DCG_SQLITE_CRYPT_FUNC_
#define  DCG_SQLITE_CRYPT_FUNC_

/***********
SQLITE 加密关键函数库
***********/

/***********
关键加密函数
***********/
//int My_Encrypt_Func(unsigned char * pData, unsigned int data_len, const char * key, unsigned int len_of_key);
int My_Encrypt_Func(unsigned char * pData, unsigned int data_len, unsigned char * key, unsigned int len_of_key);

/***********
关键解密函数
***********/
//int My_DeEncrypt_Func(unsigned char * pData, unsigned int data_len, const char * key, unsigned int len_of_key);
int My_DeEncrypt_Func(unsigned char * pData, unsigned int data_len,	unsigned char * key, unsigned int len_of_key);
#endif

