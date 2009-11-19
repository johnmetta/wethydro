#if !defined(AFX_LOGNORMPARAM_H__9FF64439_950F_4B20_92D8_9894DE3E0675__INCLUDED_)
#define AFX_LOGNORMPARAM_H__9FF64439_950F_4B20_92D8_9894DE3E0675__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogNormParam.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// LogNormParam dialog

class LogNormParam : public CDialog
{
// Construction
public:
	LogNormParam(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(LogNormParam)
	enum { IDD = IDD_LOGNORMPARAM };
	float	m_mean;
	float	m_std;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(LogNormParam)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(LogNormParam)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGNORMPARAM_H__9FF64439_950F_4B20_92D8_9894DE3E0675__INCLUDED_)
