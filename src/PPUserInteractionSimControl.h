#if !defined(AFX_PPUSERINTERACTIONSIMCONTROL_H__010A4FCF_3CB5_4201_8B2F_E5CED163D020__INCLUDED_)
#define AFX_PPUSERINTERACTIONSIMCONTROL_H__010A4FCF_3CB5_4201_8B2F_E5CED163D020__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PPUserInteractionSimControl.h : header file
//
class ScatterWnd;
class DataObj;
/////////////////////////////////////////////////////////////////////////////
// PPUserInteractionSimControl dialog

class PPUserInteractionSimControl : public CPropertyPage
{
	DECLARE_DYNCREATE(PPUserInteractionSimControl)

// Construction
public:
	PPUserInteractionSimControl();
	~PPUserInteractionSimControl();
   void AddCell (int polyID);
   void AddGridCell( int col, int row);
   void AddReach(int polyID);
   BOOL Show();

// Dialog Data
	//{{AFX_DATA(PPUserInteractionSimControl)
	enum { IDD = IDD_PROPPAGE_SIMULATIONCONTROL };
	CListBox	m_reachList;
	CComboBox	m_comboStopTime;
	CComboBox	m_comboStartTime;
	CListBox	m_cellList;
	double	m_errorTolerance;
	double	m_initialTimeStep;
	double	m_maximumTimeStep;
	double	m_minimumTimeStep;
	CString	m_modelStartTime;
	CString	m_modelStopTime;
	BOOL	m_useVariableStep;
	//}}AFX_DATA

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(PPUserInteractionSimControl)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
	
	   void OnUpdateCmdUI( CFrameWnd *pTarget, BOOL disableIfNoHandler );
protected:
	// Generated message map functions
	//{{AFX_MSG(PPUserInteractionSimControl)
	afx_msg void OnRunmodel();
	afx_msg void OnSelectallreaches();
	afx_msg void OnDoubleclickedRemovereaches();
	afx_msg void OnSixReaches();

	afx_msg void OnOptimize();
	afx_msg void OnRunsynthetic();
	afx_msg void OnSamplecells();
	afx_msg void OnSamplereaches();
	afx_msg void OnRemovecells();
	afx_msg void OnRemovereaches();
	//}}AFX_MSG
public:
		afx_msg void OnIdok();
	DECLARE_MESSAGE_MAP()

   afx_msg void OnCbnSelchangeModelstarttime();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PPUSERINTERACTIONSIMCONTROL_H__010A4FCF_3CB5_4201_8B2F_E5CED163D020__INCLUDED_)
