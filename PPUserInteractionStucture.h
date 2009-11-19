#include "afxwin.h"
#if !defined(AFX_PPUSERINTERACTIONSTUCTURE_H__4DC678FC_5835_4BF5_858C_79524E2661EF__INCLUDED_)
#define AFX_PPUSERINTERACTIONSTUCTURE_H__4DC678FC_5835_4BF5_858C_79524E2661EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PPUserInteractionStucture.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// PPUserInteractionStucture dialog

class PPUserInteractionStucture : public CPropertyPage
{
	DECLARE_DYNCREATE(PPUserInteractionStucture)

// Construction
public:
	PPUserInteractionStucture();
	~PPUserInteractionStucture();

// Dialog Data
	//{{AFX_DATA(PPUserInteractionStucture)
	enum { IDD = IDD_PROPPAGE_MODELSTRUCTURE };
	CButton	m_checkSideOfStream;
	CButton	m_buttonRecalc;
	CEdit	m_numbandscontrol;
	int		m_radiostructure;
	float		m_widthofband;
	int		m_numbands;
	int		m_radioCentroid;
	BOOL	m_routeChannelWaterMethod;
	BOOL	m_checkRouteChannelSolutes;
   BOOL m_checkUnsaturated;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(PPUserInteractionStucture)
	public:
	virtual BOOL OnApply();
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType = adjustBorder);
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(PPUserInteractionStucture)
	afx_msg void OnRadiowatershed();
	afx_msg void OnRadiodistibute();
	afx_msg void OnButtoncalcstructure();
	afx_msg void OnChangeDistband();
	afx_msg void OnChangeNumbands();
	afx_msg void OnRadiobuffer();
	afx_msg void OnRadiocell();
	afx_msg void OnRadiocentroid();
	afx_msg void OnButtonrecalc();
	afx_msg void OnChecksideofstream();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
   afx_msg void OnBnClickedCheckriparian();
   // Run with an Unsaturated Zone Simulation?
 
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PPUSERINTERACTIONSTUCTURE_H__4DC678FC_5835_4BF5_858C_79524E2661EF__INCLUDED_)
