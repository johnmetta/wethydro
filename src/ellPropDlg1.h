#if !defined(AFX_ELLPROPDLG1_H__4E41F769_40FB_47D6_A436_C024E03A2379__INCLUDED_)
#define AFX_ELLPROPDLG1_H__4E41F769_40FB_47D6_A436_C024E03A2379__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ellPropDlg1.h : header file
//
class MapLayer;

#include "grid\gridctrl.h"

/////////////////////////////////////////////////////////////////////////////
// CellPropDlg1 dialog

class CellPropDlg1 : public CDialog
{
// Construction
public:
	CellPropDlg1(MapLayer *pLayer, int currentCell, CWnd* pParent = NULL);   // standard constructor
   CGridCtrl m_grid;

   int       m_currentCell;
   MapLayer *m_pLayer;
	void LoadStatistics( void );


// Dialog Data
	//{{AFX_DATA(CellPropDlg1)
	enum { IDD = IDD_CELLPROPERTIES1 };
	CButton	m_prevcell;
	CButton	m_nextcell;
	CButton	m_flashcell;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CellPropDlg1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
   void LoadCell( void );


	// Generated message map functions
	//{{AFX_MSG(CellPropDlg1)
	virtual BOOL OnInitDialog();
	afx_msg void OnFlashcell();
	afx_msg void OnNextcell();
	afx_msg void OnPrevcell();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ELLPROPDLG1_H__4E41F769_40FB_47D6_A436_C024E03A2379__INCLUDED_)
