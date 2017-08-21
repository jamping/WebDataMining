/**
 * 一个HTML标记窗在此文本抽取系统中的抽象。
 * 可以用它来获取此标记本身的一些特征参数，例如超文本密度等等。
 * @author jjp
 */

#pragma once
class CJTagWindow
{
public:
	CJTagWindow(TidyDoc doc,TidyNode node);
	~CJTagWindow(void);

	double Weight(int nTotalT,int nTotalA,int nTotalNumInfoNodes);
	CString GetInnerText(BOOL bViewMode);
	CString GetOuterText();
	int GetNumInfoNode() { return m_nNumInfoNodes; }
	TidyNode GetNode() { return m_node; }

	static CString GetInnerText(TidyDoc doc,TidyNode node,BOOL bViewMode);
	static CString GetAnchorText(TidyDoc doc,TidyNode node);
	static int GetNumInfoNode(TidyNode node);
private:
	TidyDoc    m_doc;
	TidyNode   m_node;
	CString    m_strText;
	CString    m_strAnchorText;
	int        m_nNumInfoNodes;

	static double fn(double x);
	double AnchorDensity();
	double Share(int nTotalA,int nTotalT);
	CString GetAnchorText();	
};

