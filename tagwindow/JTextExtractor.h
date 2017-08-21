/**
 * @version 1.0
 * @author jjp
 *      ����ཫ���е������ı���һƪHTML�ĵ�����ȡ���������ҷֳ����ɶ��䡣
 *      @note ���� ��ָһƪHTML�ĵ��У�������Լ��е�һ�����֡�
 *      ÿһ����ȡ�����Ķ��佫����һ��Ȩ�ء����Ȩ���Ǹö����HTML�ĵ�����Ĺ��׳̶�
 */

#pragma once

class CJTagWindow;

class CJTextExtractor
{
public:
	CJTextExtractor(void);
	~CJTextExtractor(void);

	 /**
     * ��������������ê�ı��ܶȣ���ֵĿǰΪ.5
     */
//    const static double MAX_ANCHOR_DEN = 0.5;

    CString Extract(const CString& strBuf);
	CString GetHtmlText();
private:
	void Cleanup(TidyDoc doc,TidyNode node);
	void ExtractWindows(TidyDoc doc,TidyNode node);
	 /**
     * ��HTML�ĵ����ж����������֣�
     */
     int m_nTotalTextLen ;
    /**
     * ��HTML�ĵ����ж��ٳ��������֣�
     */
     int m_nTotalAnchorTextLen ;
    /**
     * ��HTML�ĵ����ж���infoNodes?
     */
     int m_nTotalNumInfoNodes;
    /**
     * ����б�
     */
     CList<CJTagWindow*,CJTagWindow*> m_windowsList;
	 // the max score tag
	 CJTagWindow*  m_pMaxTagWindow;
    /**
     * w3cHTML�ĵ�ģ��
     */
     TidyDoc m_doc;	
};

