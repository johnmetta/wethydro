#if !defined(AFX_PPUSERINTERACTIONTRACER_H__F6B401D9_36AE_488D_95E3_4331382AB24C__INCLUDED_)
#define AFX_PPUSERINTERACTIONTRACER_H__F6B401D9_36AE_488D_95E3_4331382AB24C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PPUserInteractionTracer.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// PPUserInteractionTracer dialog

class PPUserInteractionTracer : public CPropertyPage
{
	DECLARE_DYNCREATE(PPUserInteractionTracer)

// Construction
public:
	PPUserInteractionTracer();
	~PPUserInteractionTracer();

// Dialog Data
	//{{AFX_DATA(PPUserInteractionTracer)
	enum { IDD = IDD_PROPPAGE_TRACERINTERACTION };
	float	m_editTracerMgL;
	BOOL	m_checkTracerModel;
	int		m_radioTracerInput;
	float	m_initTracerConc;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(PPUserInteractionTracer)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(PPUserInteractionTracer)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PPUSERINTERACTIONTRACER_H__F6B401D9_36AE_488D_95E3_4331382AB24C__INCLUDED_)
