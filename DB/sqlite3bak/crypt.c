#include "crypt.h"
#include "memory.h"

/***********
�ؼ����ܺ���
***********/
// int My_Encrypt_Func(unsigned char * pData, unsigned int data_len, const char * key, unsigned int len_of_key)
int My_Encrypt_Func(unsigned char * pData, unsigned int data_len, unsigned char * key, unsigned int len_of_key)
{
	unsigned int i;
	unsigned char val;

	for (i = 0; i < data_len; i++)
	{
		val = ~(*pData);
		*pData = val;
		pData++;
	}

	return 0;
}

/***********
�ؼ����ܺ���
***********/
// int My_DeEncrypt_Func(unsigned char * pData, unsigned int data_len, const char * key, unsigned int len_of_key)
int My_DeEncrypt_Func(unsigned char * pData, unsigned int data_len, unsigned char * key, unsigned int len_of_key)
{
	unsigned int i;
	unsigned char val;

	for (i = 0; i < data_len; i++)
	{
		val = ~(*pData);
		*pData = val;
		pData++;
	}

	return 0;
}
