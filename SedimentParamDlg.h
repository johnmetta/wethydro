#if !defined(AFX_SEDIMENTPARAMDLG_H__A628E573_9A2A_4A37_8939_3E7A065C5D69__INCLUDED_)
#define AFX_SEDIMENTPARAMDLG_H__A628E573_9A2A_4A37_8939_3E7A065C5D69__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SedimentParamDlg.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// SedimentParamDlg dialog

class SedimentParamDlg : public CDialog
{
// Construction
public:
	SedimentParamDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(SedimentParamDlg)
	enum { IDD = IDD_SEDIMENTPARAM };
	float	m_c1;
	float	m_c10;
	float	m_p1;
	float	m_p2;
	float	m_p3;
	float	m_p4;
	float	m_p5;
	float	m_p6;
	float	m_p7;
	float	m_p8;
	float	m_p9;
	float	m_c2;
	float	m_p10;
	float	m_c3;
	float	m_c4;
	float	m_c5;
	float	m_c6;
	float	m_c7;
	float	m_c8;
	float	m_c9;
	CString	m_landuse1;
	CString	m_landuse10;
	CString	m_landuse2;
	CString	m_landuse3;
	CString	m_landuse4;
	CString	m_landuse5;
	CString	m_landuse6;
	CString	m_landuse7;
	CString	m_landuse8;
	CString	m_landuse9;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SedimentParamDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(SedimentParamDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEDIMENTPARAMDLG_H__A628E573_9A2A_4A37_8939_3E7A065C5D69__INCLUDED_)
