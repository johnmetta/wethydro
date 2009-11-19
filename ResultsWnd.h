#if !defined(AFX_RESULTSWND_H__A76913A7_C847_11D3_95C5_00A076B0010A__INCLUDED_)
#define AFX_RESULTSWND_H__A76913A7_C847_11D3_95C5_00A076B0010A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ResultsWnd.h : header file
//

#include "mapwnd.h"
#include "TreeView.h"
#include <afxtempl.h>
//#include <dataobj.h>

class TreeView;

class MapWndArray : public CArray< MapWnd*, MapWnd* >
   {
   };


/////////////////////////////////////////////////////////////////////////////
// ResultsWnd window

class ResultsWnd : public CWnd
{
// Construction
public:
	ResultsWnd();

// Attributes
public:
   MapWndArray m_mapWndArray;
   int m_resultCount;
   CListView *m_pDataListTree;
	CWnd *m_pDataViewWnd;
   CSplitterWnd m_splitWnd;


// Operations
public:
   void RemoveAll();    // clear out all maps, etc.

   MapWnd *AddMap();    // add a new map for storage in results
	void SetTreeData();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ResultsWnd)
	public:
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~ResultsWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(ResultsWnd)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESULTSWND_H__A76913A7_C847_11D3_95C5_00A076B0010A__INCLUDED_)
