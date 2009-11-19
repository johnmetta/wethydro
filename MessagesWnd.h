#if !defined(AFX_MESSAGESWND_H__F5492421_7D7F_4A39_8A75_515E93AFD5DB__INCLUDED_)
#define AFX_MESSAGESWND_H__F5492421_7D7F_4A39_8A75_515E93AFD5DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MessagesWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// MessagesWnd window
#include <afxhtml.h>
class MessagesWnd : public CHtmlView
{
// Construction
public:
	MessagesWnd();


// Attributes
public:

// Operations
public:

	void SummarizeModel();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MessagesWnd)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~MessagesWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(MessagesWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
   afx_msg void OnSize(UINT nType, int cx, int cy);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MESSAGESWND_H__F5492421_7D7F_4A39_8A75_515E93AFD5DB__INCLUDED_)
