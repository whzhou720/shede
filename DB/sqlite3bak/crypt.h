#pragma once

#ifndef  DCG_SQLITE_CRYPT_FUNC_
#define  DCG_SQLITE_CRYPT_FUNC_

/***********
SQLITE ���ܹؼ�������
***********/

/***********
�ؼ����ܺ���
***********/
//int My_Encrypt_Func(unsigned char * pData, unsigned int data_len, const char * key, unsigned int len_of_key);
int My_Encrypt_Func(unsigned char * pData, unsigned int data_len, unsigned char * key, unsigned int len_of_key);

/***********
�ؼ����ܺ���
***********/
//int My_DeEncrypt_Func(unsigned char * pData, unsigned int data_len, const char * key, unsigned int len_of_key);
int My_DeEncrypt_Func(unsigned char * pData, unsigned int data_len,	unsigned char * key, unsigned int len_of_key);
#endif

