#if !defined(AFX_UNIFORMPARAM_H__FB729346_AADC_4E9E_963A_74AD86E10E26__INCLUDED_)
#define AFX_UNIFORMPARAM_H__FB729346_AADC_4E9E_963A_74AD86E10E26__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UniformParam.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// UniformParam dialog

class UniformParam : public CDialog
{
// Construction
public:
	UniformParam(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(UniformParam)
	enum { IDD = IDD_DIALOGUNIFPARAM };
	float	m_max;
	float	m_min;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(UniformParam)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(UniformParam)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UNIFORMPARAM_H__FB729346_AADC_4E9E_963A_74AD86E10E26__INCLUDED_)
