// Des.h: interface for the CDes class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#define DES_ENCRYPT 1
#define DES_DECRYPT 0

#define ROUNDS 16

typedef struct
{
    unsigned long half[2];
}des_keys[16];

class CDes  
{
public:
	CDes();
	virtual ~CDes();

    CString DesCrypteString(CString strPass,CString str,int nFlag);

	void EnCrypteFile(CString password,CString ming,CString mi);
	void DeCrypteFile(CString password,CString mi,CString ming);
private:
	int set_key(unsigned char * key);
	void SetKey(CString strPass);	
	void DesCrypte(unsigned char * data_in,unsigned char * data_out,int encrypt_flag);

	des_keys m_keys;
};