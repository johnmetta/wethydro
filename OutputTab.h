#if !defined(AFX_OUTPUTTAB_H__365532F5_B605_11D1_821E_444553540000__INCLUDED_)
#define AFX_OUTPUTTAB_H__365532F5_B605_11D1_821E_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// OutputTab.h : header file
//

#include "MapWnd.h"
#include "resultPlotWnd.h"
#include "ResultsWnd.h"
#include "MessagesWnd.h"


/////////////////////////////////////////////////////////////////////////////
// OutputTab window

class OutputTab : public CTabCtrl
{
// Construction
public:
	OutputTab();

// Attributes
public:

   MapWnd				m_mapTab;
   ResultPlotWnd		m_resultsPlotTab;
   ResultsWnd			m_resultsTab;
	MessagesWnd			m_messagesTab;
//   MsgWnd     m_msgTab;


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(OutputTab)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~OutputTab();

	// Generated message map functions
protected:
	CFont m_font;
	//{{AFX_MSG(OutputTab)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OUTPUTTAB_H__365532F5_B605_11D1_821E_444553540000__INCLUDED_)
