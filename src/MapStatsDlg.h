#if !defined(AFX_MAPSTATSDLG_H__FE01B544_7555_474D_AB08_C0B740EECAA0__INCLUDED_)
#define AFX_MAPSTATSDLG_H__FE01B544_7555_474D_AB08_C0B740EECAA0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MapStatsDlg.h : header file
//

class MapLayer;


/////////////////////////////////////////////////////////////////////////////
// MapStatsDlg dialog

class MapStatsDlg : public CDialog
{
// Construction
public:
	MapStatsDlg( MapLayer *, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(MapStatsDlg)
	enum { IDD = 10 };
	long	m_restoredArea;
	long	m_totalArea;
	long	m_restoredCells;
	long	m_totalCells;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MapStatsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(MapStatsDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAPSTATSDLG_H__FE01B544_7555_474D_AB08_C0B740EECAA0__INCLUDED_)
