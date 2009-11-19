#if !defined(AFX_PPUSERINTERACTIONET_H__17CF9D39_DF47_41E5_AAF0_6FD9E75B2678__INCLUDED_)
#define AFX_PPUSERINTERACTIONET_H__17CF9D39_DF47_41E5_AAF0_6FD9E75B2678__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PPUserInteractionET.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// PPUserInteractionET dialog

class PPUserInteractionET : public CPropertyPage
{
	DECLARE_DYNCREATE(PPUserInteractionET)

// Construction
public:
	PPUserInteractionET();
	~PPUserInteractionET();

// Dialog Data
	//{{AFX_DATA(PPUserInteractionET)
	enum { IDD = IDD_PROPPAGE_ET };
	CButton	m_buttonViewParam;
	int		m_etMethod;
	BOOL	m_checkSimEt;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(PPUserInteractionET)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(PPUserInteractionET)
	afx_msg void OnButtonviewparam();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PPUSERINTERACTIONET_H__17CF9D39_DF47_41E5_AAF0_6FD9E75B2678__INCLUDED_)
