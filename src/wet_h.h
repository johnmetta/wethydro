// wet_h.h : main header file for the WET_H application
//

#if !defined(AFX_WET_H_H__03FA995D_B764_4570_9C30_CC0F7CE385C9__INCLUDED_)
#define AFX_WET_H_H__03FA995D_B764_4570_9C30_CC0F7CE385C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CWet_hApp:
// See wet_h.cpp for the implementation of this class
//

class CWet_hApp : public CWinApp
{
public:
	CWet_hApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWet_hApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CWet_hApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileOpen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WET_H_H__03FA995D_B764_4570_9C30_CC0F7CE385C9__INCLUDED_)
