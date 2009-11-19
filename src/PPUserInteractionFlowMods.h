#if !defined(AFX_PPUSERINTERACTIONFLOWMODS_H__DFBAA474_38B6_401A_8B95_6E3ADF9BF5DD__INCLUDED_)
#define AFX_PPUSERINTERACTIONFLOWMODS_H__DFBAA474_38B6_401A_8B95_6E3ADF9BF5DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PPUserInteractionFlowMods.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// PPUserInteractionFlowMods dialog

class PPUserInteractionFlowMods : public CPropertyPage
{
	DECLARE_DYNCREATE(PPUserInteractionFlowMods)

// Construction
public:
	PPUserInteractionFlowMods();
	~PPUserInteractionFlowMods();

// Dialog Data
	//{{AFX_DATA(PPUserInteractionFlowMods)
	enum { IDD = IDD_PROPPAGE_FLOWMOD };
	BOOL	m_checkIncrFlow;
	CTime	m_incrFlowBegin;
	CTime	m_incrFlowEnd;
	float	m_incrFlowPer;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(PPUserInteractionFlowMods)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(PPUserInteractionFlowMods)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PPUSERINTERACTIONFLOWMODS_H__DFBAA474_38B6_401A_8B95_6E3ADF9BF5DD__INCLUDED_)
