//================================================================================
// CLASS   : CExtract
// FUNCTION: ��ȡ��ҳ���ݣ�ȥ��html��ǩ��
// AUTHOR  : 2006-05-21 Created by ocean
// REFER   : �����ļ�����*.htm/html��
// NOTE    : Extract.h , interface for the Extract class.
// Modified by ocean in 2006.11.11
//================================================================================
#pragma once
/********************************************************************************/
//�û������ļ� 0:��1:��
#define STR_USEFUL "<TITLE>,<TR>,<TD>,<P ,<P>,</P>,<BR>,<DIV>,<IMG>"
#define STR_A       0          //�Ƿ�ɾ��<A></A>����֮������ݣ�a<A>b</A>c��Ϊac��

#define STR_SPACE   0          //�Ƿ�ɾ�����пո�
#define STR_REPLACE_VERTICAL 1 //�Ƿ�|�滻�ɿո�
#define STR_TI_CAPITAL   1     //�Ƿ�Ҫ��Сд(title)��Ϊ��д(TITLE)
#define STR_TR_CAPITAL   1     //�Ƿ�Ҫ��Сд(tr)��Ϊ��д(TR)
#define STR_TD_CAPITAL   1     //�Ƿ�Ҫ��Сд(td)��Ϊ��д(TD)
#define STR_P_CAPITAL    1     //�Ƿ�Ҫ��Сд(p)��Ϊ��д(P)
#define STR_BR_CAPITAL   1     //�Ƿ�Ҫ��Сд(br)��Ϊ��д(BR)
#define STR_DI_CAPITAL   1     //�Ƿ�Ҫ��Сд(div)��Ϊ��д(DIV)
#define STR_A_CAPITAL    1     //�Ƿ�Ҫ��Сд(a)��Ϊ��д(A)
#define STR_XML_CAPITAL  1     //�Ƿ�Ҫ��Сд(xml)��Ϊ��д(XML)  ע��һ��ΪСд
/********************************************************************************/
#include "string"
#include "vector"
//using namespace std;

class CExtract  
{
public:
	CExtract();	
	virtual ~CExtract();

	void GetPureText(std::string& strResult,bool bSaveImg=true,bool bSaveEmptyLine=false);
	void GetPureHtml(std::string& strResult);
private:
	void Process(std::string filename);
	void Result(std::string strResult,bool bSaveEmptyLine=false);
	void Output(std::string& strResult);
	void ConvCapital(std::string& strLine);
	void PreProcess(std::string& strLine);
	//CountSymbol����������λ��i,j���symbol������
	int  CountSymbol(std::string strResult,int i,int j,std::string symbol);
	void Delete(std::string& strResult,std::string separator);
	void Delete2(std::string& strResult,std::string separator,int nPos);
	void KeepListLabel(std::string& strResult,int nPosOld,bool bSaveImg=true);
	void Replace(std::string& strLine,std::string strOld,std::string strNew);
	void Replace(std::wstring& strLine,std::wstring strOld,std::wstring strNew); //������ַ�
	void DelSideSpace(std::string& strLine);
	void DelBegAnd(std::string& strResult,int nPos);   //ȥ����&��ͷ��;�������ַ���
	void DelUselessLink(std::string& strResult);       //ɾ����������
private:
	std::vector<std::string> vecResult; //��Ž����һ�������д�һ�С�
	std::string strUseful;	
};