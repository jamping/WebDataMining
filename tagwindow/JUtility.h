/**
 * 分析中需要调用的一些静态方法，工具集
 * @author jjp
 */
#pragma once

class CJUtility
{	 
public:
	static BOOL IsTableNode(TidyNode e);
	static BOOL IsLargeNode(TidyNode e);
	static BOOL IsInfoNode(TidyNode e);
	static BOOL IsHeading(TidyNode e);
	static BOOL IsInvalidElement(TidyNode e);
	static BOOL IsLinkNode(TidyNode e);
	static BOOL IsImgNode(TidyNode e);
	static BOOL IsNeedWarp(TidyNode e);
	static BOOL IsNeedSpace(TidyNode e);
	static BOOL IsAppearanceTag(TidyNode e);
	static BOOL IsContainsInput(TidyNode e);
	static BOOL IsContainsNoise(const CString& strText);
	static int NumLinkNode(TidyNode e);
	static int GetElementsByTagName(TidyNode e,const CString& strTagName,CList<TidyNode,TidyNode>& list,BOOL bSelf = TRUE);
	static CString ReplaceAll(const CString& strText,const CString& strRegex,const CString& strReplacement);
	static void FilterInvalidElements(CString& strText);
	static CString GetNodeText(TidyDoc doc,TidyNode node,BOOL bOnlyContent = TRUE);
	static TidyNode RemoveChild(TidyNode child);
	static TidyNode DiscardChild(TidyDoc doc,TidyNode child);
};