#if !defined(AFX_VDATATABLE_H__7D8BB2EC_A943_44AA_9CA7_D363AB86950B__INCLUDED_)
#define AFX_VDATATABLE_H__7D8BB2EC_A943_44AA_9CA7_D363AB86950B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VDataTable.h : header file
//


#include <grid\gridctrl.h>

class VDataObj;


/////////////////////////////////////////////////////////////////////////////
// VDataTable window

class VDataTable : public CWnd
{
// Construction
public:
	VDataTable( VDataObj*);

// Attributes
public:
   VDataObj *m_pData;
   CGridCtrl m_grid;

// Operations
public:
   void LoadTable();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(VDataTable)
	public:
   virtual BOOL Create( LPCTSTR lpszWindowName, const RECT& rect, CWnd* pParentWnd, UINT nID );
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~VDataTable();

	// Generated message map functions
protected:
	//{{AFX_MSG(VDataTable)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VDATATABLE_H__7D8BB2EC_A943_44AA_9CA7_D363AB86950B__INCLUDED_)
