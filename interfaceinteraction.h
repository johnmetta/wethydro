#if !defined(AFX_INTERFACEINTERACTION_H__D8E59CCF_A84A_4988_870C_3F1A1B29E609__INCLUDED_)
#define AFX_INTERFACEINTERACTION_H__D8E59CCF_A84A_4988_870C_3F1A1B29E609__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// interfaceinteraction.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// interfaceinteraction dialog

class interfaceinteraction : public CPropertyPage
{
	DECLARE_DYNCREATE(interfaceinteraction)

// Construction
public:
	interfaceinteraction();
	~interfaceinteraction();

// Dialog Data
	//{{AFX_DATA(interfaceinteraction)
	enum { IDD = IDD_PROPPAGE_INTERFACEINTERACTION };
	BOOL	m_updatestream;
	float	m_maxq;
	float	m_minq;
	int	m_numbin;
	int	m_widthline;
	BOOL	m_updateupland;
	float	m_collectionint;
	int	m_upnumbin;
	float	m_upmax;
	float	m_upmin;
	int	m_radioq;
	int	m_radioswc;
	BOOL	m_updateClassificationExtent;
	BOOL	m_saveEmfOutput;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(interfaceinteraction)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(interfaceinteraction)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
   afx_msg void OnBnClickedCheckupdateclassextent();
   // //number of days to ignore prior to calculating efficiency statistics
   int m_spinUp;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INTERFACEINTERACTION_H__D8E59CCF_A84A_4988_870C_3F1A1B29E609__INCLUDED_)
