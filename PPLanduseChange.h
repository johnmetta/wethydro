#if !defined(AFX_PPLANDUSECHANGE_H__D8DAF6E4_98EA_4CE9_B641_DD6C624C1FA6__INCLUDED_)
#define AFX_PPLANDUSECHANGE_H__D8DAF6E4_98EA_4CE9_B641_DD6C624C1FA6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PPLanduseChange.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// PPLanduseChange dialog

class PPLanduseChange : public CPropertyPage
{
	DECLARE_DYNCREATE(PPLanduseChange)

// Construction
public:
	PPLanduseChange();
	~PPLanduseChange();

// Dialog Data
	//{{AFX_DATA(PPLanduseChange)
	enum { IDD = IDD_PROPPAGE_LANDUSECHANGE };
	BOOL	m_checkModelWetlands;
	BOOL	m_checkModelTileDrains;
	float	m_tileOuflowCoef;
	float	m_tileThreshold;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(PPLanduseChange)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(PPLanduseChange)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PPLANDUSECHANGE_H__D8DAF6E4_98EA_4CE9_B641_DD6C624C1FA6__INCLUDED_)
