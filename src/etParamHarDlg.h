#if !defined(AFX_ETPARAMHARDLG_H__0E0A0DE5_DF92_48A4_AC63_D5FB965F91F0__INCLUDED_)
#define AFX_ETPARAMHARDLG_H__0E0A0DE5_DF92_48A4_AC63_D5FB965F91F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// etParamHarDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// etParamHarDlg dialog

class etParamHarDlg : public CDialog
{
// Construction
public:
	etParamHarDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(etParamHarDlg)
	enum { IDD = IDD_ETPARAMHARDLG };
	float	m_latitude;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(etParamHarDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(etParamHarDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ETPARAMHARDLG_H__0E0A0DE5_DF92_48A4_AC63_D5FB965F91F0__INCLUDED_)
