//{{AFX_INCLUDES()
#include "chart3d.h"
//}}AFX_INCLUDES
#if !defined(AFX_OPTIMIZEDLG3DPLOT_H__F8F60822_0B63_4CB0_90B6_E278D379ABC8__INCLUDED_)
#define AFX_OPTIMIZEDLG3DPLOT_H__F8F60822_0B63_4CB0_90B6_E278D379ABC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
// OptimizeDlg3DPlot.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// OptimizeDlg3DPlot dialog

class OptimizeDlg3DPlot : public CDialog
{
// Construction
public:
	OptimizeDlg3DPlot(CWnd* pParent = NULL);   // standard constructor
	void UpdatePlot();
// Dialog Data
	//{{AFX_DATA(OptimizeDlg3DPlot)
	enum { IDD = IDD_3DCHART };
	CChart3D	m_3dchart;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(OptimizeDlg3DPlot)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(OptimizeDlg3DPlot)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIMIZEDLG3DPLOT_H__F8F60822_0B63_4CB0_90B6_E278D379ABC8__INCLUDED_)
