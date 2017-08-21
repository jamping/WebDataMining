/**
 * @version 1.0
 * @author jjp
 *      这个类将所有的正文文本从一篇HTML文档中提取出来，并且分成若干段落。
 *      @note 段落 是指一篇HTML文档中，语义相对集中的一段文字。
 *      每一个提取出来的段落将会获得一个权重。这个权重是该段落对HTML文档主题的贡献程度
 */

#pragma once

class CJTagWindow;

class CJTextExtractor
{
public:
	CJTextExtractor(void);
	~CJTextExtractor(void);

	 /**
     * 这个标记最大允许的锚文本密度，该值目前为.5
     */
//    const static double MAX_ANCHOR_DEN = 0.5;

    CString Extract(const CString& strBuf);
	CString GetHtmlText();
private:
	void Cleanup(TidyDoc doc,TidyNode node);
	void ExtractWindows(TidyDoc doc,TidyNode node);
	 /**
     * 该HTML文档中有多少正文文字？
     */
     int m_nTotalTextLen ;
    /**
     * 该HTML文档中有多少超链接文字？
     */
     int m_nTotalAnchorTextLen ;
    /**
     * 该HTML文档中有多少infoNodes?
     */
     int m_nTotalNumInfoNodes;
    /**
     * 标记列表
     */
     CList<CJTagWindow*,CJTagWindow*> m_windowsList;
	 // the max score tag
	 CJTagWindow*  m_pMaxTagWindow;
    /**
     * w3cHTML文档模型
     */
     TidyDoc m_doc;	
};

