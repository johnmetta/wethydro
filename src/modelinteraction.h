#if !defined(AFX_MODELINTERACTION_H__18F6BEA9_53AE_4A2D_A9C1_F04128123CA0__INCLUDED_)
#define AFX_MODELINTERACTION_H__18F6BEA9_53AE_4A2D_A9C1_F04128123CA0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// modelinteraction.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// modelinteraction dialog

class modelinteraction : public CPropertyPage
{
	DECLARE_DYNCREATE(modelinteraction)

// Construction
public:
	modelinteraction();
	~modelinteraction();

// Dialog Data
	//{{AFX_DATA(modelinteraction)
	enum { IDD = IDD_PROPPAGE_MODELINTERACTION };
	CListBox	m_soilList;
	float		m_deltax;
	float	m_is;
	float	m_ksat;
	float	m_m;
	float	m_n;
	float	m_soildepth;
	float	m_wdratio;
	BOOL	m_ksAsParam;
	BOOL	m_sdAsParam;
	float	m_ltPrecip;
	BOOL	m_checkPrism;
	BOOL	m_checkUseSoilList;
	float	m_phi;
	int	m_checkMonteCarlo;
	int		m_numIterations;
	BOOL	m_checkThreeBasins;
	float	m_kDepth;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(modelinteraction)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(modelinteraction)
	afx_msg void OnButtonmcadditional();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODELINTERACTION_H__18F6BEA9_53AE_4A2D_A9C1_F04128123CA0__INCLUDED_)
