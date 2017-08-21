// OptionsWDMConfig.cpp : implementation file
//

#include "stdafx.h"
#include "webdatamining.h"
#include "OptionsWDMConfig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsWDMConfig property page

IMPLEMENT_DYNCREATE(COptionsWDMConfig, CMFCPropertyPage)

COptionsWDMConfig::COptionsWDMConfig() : CMFCPropertyPage(COptionsWDMConfig::IDD)
{
	//{{AFX_DATA_INIT(COptionsWDMConfig)
	m_nTopicSaveDays = 0;
	m_nPictureHeight = 0;
	m_nPictureWidth = 0;
	m_nTopicTopTime = 0;
	m_nTopicKeyWord = 0;
	m_strTopicModifyPath = _T("");
	m_strTopicViewPath = _T("");
	m_nTopicTopKeyWord = 0;
	m_nPicMaxSize = 0;
	//}}AFX_DATA_INIT
	m_nInterpolation = 0;
}

COptionsWDMConfig::~COptionsWDMConfig()
{
}

void COptionsWDMConfig::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsWDMConfig)
	DDX_Text(pDX, IDC_TOPIC_SAVE_DAYS, m_nTopicSaveDays);
	DDX_Text(pDX, IDC_PICTURE_HEIGHT, m_nPictureHeight);
	DDX_Text(pDX, IDC_PICTURE_WIDTH, m_nPictureWidth);
	DDX_Text(pDX, IDC_TOPIC_TOPTIME, m_nTopicTopTime);
	DDX_Text(pDX, IDC_TOPIC_KEYWORD, m_nTopicKeyWord);
	DDX_Text(pDX, IDC_TOPIC_MODIFY_PATH, m_strTopicModifyPath);
	DDX_Text(pDX, IDC_TOPIC_VIEW_PATH, m_strTopicViewPath);
	DDX_Text(pDX, IDC_TOPIC_TOPKEYWORD, m_nTopicTopKeyWord);
	DDX_Text(pDX, IDC_PICTURE_MAXSIZE, m_nPicMaxSize);
	//}}AFX_DATA_MAP
	DDX_CBIndex(pDX, IDC_INTERPOLATION, m_nInterpolation);
}


BEGIN_MESSAGE_MAP(COptionsWDMConfig, CMFCPropertyPage)
	//{{AFX_MSG_MAP(COptionsWDMConfig)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionsWDMConfig message handlers

BOOL COptionsWDMConfig::OnInitDialog() 
{
	CMFCPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

