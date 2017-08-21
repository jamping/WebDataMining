
#include "stdafx.h"
#include "GradientProgressCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGradientProgressCtrl

CGradientProgressCtrl::CGradientProgressCtrl()
{
	// Defaults assigned by CProgressCtrl()
	m_nLower = 0;
	m_nUpper = 100;
	m_nCurrentPosition = 0;
	m_nStep = 10;	
	
	// Initial colors
	m_clrStart	= COLORREF(RGB(128, 0,128));
	m_clrEnd =	 COLORREF(RGB(0,128,0));
	//�ı佥��ɫ����������ɫ
	m_clrBkGround = ::GetSysColor(COLOR_3DFACE);
	m_clrText = COLORREF(RGB(0, 255, 255));

	// Initial show percent
	m_bShowPercent = TRUE;
}

CGradientProgressCtrl::~CGradientProgressCtrl()
{
}


BEGIN_MESSAGE_MAP(CGradientProgressCtrl, CProgressCtrl)
	//{{AFX_MSG_MAP(CGradientProgressCtrl)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGradientProgressCtrl message handlers

/////////////////////////////////////////////////////////////////////////////
/* 
	OnPaint

	The main drawing routine.  Consists of two parts
	(1) Call the DrawGradient routine to draw the visible part of the progress gradient
	(2) If needed, show the percentage text

 */
/////////////////////////////////////////////////////////////////////////////
void CGradientProgressCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here
	CRect rectClient;
	GetClientRect(&rectClient);
	// If the current positionis  invalid then we should fade into the  background
	if (m_nCurrentPosition <= m_nLower || m_nCurrentPosition > m_nUpper)
	{
		CRect rect;
		GetClientRect(rect);
		CBrush brush;
		//�ػ�ʱ����ɫ��������Ϊ�Զ�����ɫ
		brush.CreateSolidBrush(m_clrBkGround);//::GetSysColor(COLOR_3DFACE));
		dc.FillRect(&rect, &brush);
		VERIFY(brush.DeleteObject());
		return;
	}
	
	// The actions to take depend on whether or not we are a vertical control
	DWORD dwStyles = GetStyle();
	BOOL bVertical = (BOOL)(dwStyles & PBS_VERTICAL);
	
	
	// Figure out what part should be visible so we can stop the gradient when needed
	float maxWidth;
	if (bVertical)
		maxWidth = ((float)m_nCurrentPosition/(float)m_nUpper * (float)rectClient.bottom);		
	else
		maxWidth = ((float)m_nCurrentPosition/(float)m_nUpper * (float)rectClient.right);
	
	
	// Draw the gradient
		DrawGradient(&dc, rectClient, (int)maxWidth, bVertical);

	// Show percent indicator if needed
	if (m_bShowPercent)
	{
		CString strPercent;
		float fp = 100.0f; 
		fp *= (float)(m_nCurrentPosition-m_nLower); 
		fp /= (float)(m_nUpper-m_nLower); 
		strPercent.Format(_T("%3.0f %%"), fp);
		
		dc.SetTextColor(m_clrText);
		dc.SetBkMode(TRANSPARENT);
		dc.DrawText(strPercent, &rectClient, DT_VCENTER |  DT_CENTER | DT_SINGLELINE);
	}

	// Do not call CProgressCtrl::OnPaint() for painting messages
}


/////////////////////////////////////////////////////////////////////////////
/*
	SetRange

	Overridden base class member to remember where the indicator thinks 
	it is and the boundary range of the control.

	Params
		nLower		lower bound
		nUpper		uppoer bound

*/
/////////////////////////////////////////////////////////////////////////////
void CGradientProgressCtrl:: SetRange(int nLower, int nUpper)
{
	m_nLower = nLower;
	m_nUpper = nUpper;
	m_nCurrentPosition = nLower;
	CProgressCtrl::SetRange(nLower, nUpper);
}

/////////////////////////////////////////////////////////////////////////////
/*

	SetRange32

	Overridden base class member to remember where the indicator thinks 
	it is and the boundary range of the control.

	Params
		nLower		lower bound
		nUpper		uppoer bound

*/
/////////////////////////////////////////////////////////////////////////////
void CGradientProgressCtrl:: SetRange32( int nLower, int nUpper )
{
	if ( nLower == nUpper ) return;
	m_nLower = nLower;
	m_nUpper = nUpper;
	m_nCurrentPosition = nLower;
	CProgressCtrl::SetRange(nLower, nUpper);
}


/////////////////////////////////////////////////////////////////////////////
/*
	SetPos

	Overridden base class member to retain where the current progress indicator
	is located.

	Params
		nPos		Current position in range

*/
/////////////////////////////////////////////////////////////////////////////
int CGradientProgressCtrl:: SetPos(int nPos)
{
	m_nCurrentPosition = nPos;
	return (CProgressCtrl::SetPos(nPos));
}

/////////////////////////////////////////////////////////////////////////////
/*
	SetStep

	Overridden base class member to retain the step interval used when 
	filling the progress control

	Params
		nStep		step interval for filling progress control

*/
/////////////////////////////////////////////////////////////////////////////
int CGradientProgressCtrl:: SetStep(int nStep)
{
	m_nStep = nStep;
	return (CProgressCtrl::SetStep(nStep));
}

/////////////////////////////////////////////////////////////////////////////
/*
	StepIt

	Overridden base class member to increment the control according to the
	current position and the step interval

	Params
		nStep		step interval for filling progress control

*/
/////////////////////////////////////////////////////////////////////////////
int CGradientProgressCtrl:: StepIt(void)
{
	m_nCurrentPosition += m_nStep;
	return (CProgressCtrl::StepIt());
}


/////////////////////////////////////////////////////////////////////////////
/*
	DrawGradient

	Called from OnPaint, it does most of the work of filling in the client 
	rectangle with the appropriate colors.  The normal routine would fill
	the entire client rectangle, but we truncate the drawing to reflect
	the current position in the progress control

	Params
		pDC			pointer to CPaintDC for rendering
		rectClient	client rectangle where we should draw
		nMaxWidth	where we should stop drawing the gradient
*/
/////////////////////////////////////////////////////////////////////////////
void CGradientProgressCtrl::DrawGradient(CPaintDC *pDC, const RECT &rectClient, const int &nMaxWidth, const BOOL &bVertical)
{
	RECT rectFill;			   // Rectangle for filling band
	float fStep;              // How wide is each band?
	CBrush brush;			// Brush to fill in the bar	

	
	CMyMemDC memDC(pDC);

	// First find out the largest color distance between the start and end colors.  This distance
	// will determine how many steps we use to carve up the client region and the size of each
	// gradient rect.
	int r, g, b;					// First distance, then starting value
	float rStep, gStep, bStep;		// Step size for each color
		
	BOOL  bSameColor = FALSE;		// Handle case if start color == end color

	// Get the color differences
	r = (GetRValue(m_clrEnd) - GetRValue(m_clrStart));
	g = (GetGValue(m_clrEnd) - GetGValue(m_clrStart));
	b =  (GetBValue(m_clrEnd) - GetBValue(m_clrStart));

	// Check to see if colors are same
	if((r == 0) && (g == 0) && (b == 0))
	{
		bSameColor = TRUE;
		//Added the three lines below to fix the drawing 
		//problem which used to occur when both the start 
		//and end colors are same.
		r = GetRValue(m_clrStart);
		g = GetGValue(m_clrStart);
		b = GetBValue(m_clrStart);
	}

	int nSteps;
	//Select max. possible value for nSteps if the colors are equal
	if(bSameColor && m_clrStart == 0)
		nSteps = 255;
	else 	// Make the number of steps equal to the greatest distance
		nSteps = max(abs(r), max(abs(g), abs(b)));	
	
	// Determine how large each band should be in order to cover the
	// client with nSteps bands (one for every color intensity level)
	if (bVertical)
		fStep = (float)rectClient.bottom / (float)nSteps;	
	else
		fStep = (float)rectClient.right / (float)nSteps;

	// Calculate the step size for each color
	rStep = r/(float)nSteps;
	gStep = g/(float)nSteps;
	bStep = b/(float)nSteps;

	// Reset the colors to the starting position
	r = GetRValue(m_clrStart);
	g = GetGValue(m_clrStart);
	b = GetBValue(m_clrStart);
	
	// Start filling bands
	for (int iOnBand = 0; iOnBand < nSteps; iOnBand++) 
	{
		// Fill the vertical control
		if (bVertical)
		{
			::SetRect(&rectFill,
						0,							// Upper left X
						(int)(iOnBand * fStep),		// Upper left Y
						rectClient.right+1,		// Lower right X
						(int)((iOnBand+1) * fStep));// Lower right Y
		
			// CDC::FillSolidRect is faster, but it does not handle 8-bit color depth
			if ( brush.CreateSolidBrush(RGB(r+rStep*iOnBand, g + gStep*iOnBand, b + bStep *iOnBand)) )
			{
				memDC.FillRect(&rectFill,&brush);
				VERIFY(brush.DeleteObject());
			}


			// If we are past the maximum for the current position we need to get out of the loop.
			// Before we leave, we repaint the remainder of the client area with the background color.
			if (rectFill.bottom > nMaxWidth)
			{
				::SetRect(&rectFill, 0, rectFill.bottom, rectClient.right, rectClient.bottom);
				if ( brush.CreateSolidBrush(m_clrBkGround) )
				{
					memDC.FillRect(&rectFill, &brush);
					VERIFY(brush.DeleteObject());
				}
				return;
			}
		}

		else // Fill the horizontal control
		{
			::SetRect(&rectFill,
						(int)(iOnBand * fStep),     // Upper left X
						 0,							// Upper left Y
						(int)((iOnBand+1) * fStep), // Lower right X
						rectClient.bottom+1);		// Lower right Y
		
			// CDC::FillSolidRect is faster, but it does not handle 8-bit color depth
			if ( brush.CreateSolidBrush(RGB(r+rStep*iOnBand, g + gStep*iOnBand, b + bStep *iOnBand)) )
			{
				memDC.FillRect(&rectFill,&brush);
				VERIFY(brush.DeleteObject());
			}

			// If we are past the maximum for the current position we need to get out of the loop.
			// Before we leave, we repaint the remainder of the client area with the background color.
			if (rectFill.right > nMaxWidth)
			{
				::SetRect(&rectFill, rectFill.right, 0, rectClient.right, rectClient.bottom);
				if ( brush.CreateSolidBrush(m_clrBkGround) )
				{
					memDC.FillRect(&rectFill, &brush);
					VERIFY(brush.DeleteObject());
				}
				return;
			}
		}

	}
}



/////////////////////////////////////////////////////////////////////////////
/*
	OnEraseBkgnd

	Overridden CWnd function so that all drawing is done in the OnPaint call.
	We return TRUE so that CWnd doesn't try to erase our background.

	Params
		pDC			pointer to CDC for rendering
*/
/////////////////////////////////////////////////////////////////////////////
BOOL CGradientProgressCtrl::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
}


BOOL CGradientProgressCtrl::Create(CStatusBar *parent, UINT id, DWORD style)
{
	CRect r;
	
	setup(parent, id, r);
	
	return CProgressCtrl::Create(style | WS_CHILD, r, parent, id);
}

/****************************************************************************
*                            CStatusControl::setup
* Inputs:
*	CStatusBar * parent: Parent window (status bar)
*	UINT id: ID of pane
*	CRect & r: Place to put rectangle
* Result: BOOL
*       TRUE if successful
*	FALSE if the area is off-window
* Effect: 
*       Computes the rectangle for the pane, given the status bar and id
****************************************************************************/
BOOL CGradientProgressCtrl::setup(CStatusBar *parent, UINT id, CRect &r)
{
	int i = parent->CommandToIndex(id);
	
	parent->GetItemRect(i, &r);
	parent->SetPaneText(i, "");
	
	// If the pane was not visible, GetItemRect has returned a
	// (0, 0, 0, 0) rectangle. Attempting to create the control
	// using this rectangle creates it, possibly of zero size,
	// at the left of the status bar. We correct this by
	// forcing it to be off the visible right end of the status
	// bar. If the programmer properly handles the parent frame's
	// OnSize method, when the control becomes visible it will
	// move to the correct location.
	if(r.IsRectEmpty())
	{ /* offscreen */
		CRect r1;
		parent->GetWindowRect(&r1); // get parent width
		r.left = r1.right + 1;
		r.top =  r1.top;
		r.right = r1.right + 2;
		r.bottom = r1.bottom;
		return FALSE;
	} /* offscreen */
	
	return TRUE;
}

void CGradientProgressCtrl::Reposition()
{
	if ( m_hWnd == NULL)
		return;
	UINT id = ::GetWindowLong(m_hWnd, GWL_ID);
	CRect r;
	
	// Note that because the control ID is the same as the
	// pane ID, this actually works well enough that
	// no special variable is required to obtain the
	// pane index.
	CStatusBar * parent = (CStatusBar *)GetParent();
	int i = parent->CommandToIndex(id);
	parent->GetItemRect(i, &r);
	SetWindowPos(&wndTop, r.left, r.top, r.Width(), r.Height(), 0);
}
