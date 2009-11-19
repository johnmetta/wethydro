#if !defined(AFX_PPUSERINTERACTIONSEDIMENT_H__2D7A9301_65B2_4FD0_B386_E4CAAEB29246__INCLUDED_)
#define AFX_PPUSERINTERACTIONSEDIMENT_H__2D7A9301_65B2_4FD0_B386_E4CAAEB29246__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PPUserInteractionSediment.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// PPUserInteractionSediment dialog

class PPUserInteractionSediment : public CPropertyPage
{
	DECLARE_DYNCREATE(PPUserInteractionSediment)

// Construction
public:
	PPUserInteractionSediment();
	~PPUserInteractionSediment();

// Dialog Data
	//{{AFX_DATA(PPUserInteractionSediment)
	enum { IDD = IDD_PROPPAGE_SEDIMENT };
	BOOL	m_checkUplandSediment;
	float	m_reduceSedPercent;
	BOOL	m_applySedOptions;
	float	m_bufferWidth;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(PPUserInteractionSediment)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(PPUserInteractionSediment)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PPUSERINTERACTIONSEDIMENT_H__2D7A9301_65B2_4FD0_B386_E4CAAEB29246__INCLUDED_)
