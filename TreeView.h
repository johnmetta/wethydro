#if !defined(AFX_TREEVIEW_H__6D28D01A_0B25_423C_A61C_079DEE82702A__INCLUDED_)
#define AFX_TREEVIEW_H__6D28D01A_0B25_423C_A61C_079DEE82702A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TreeView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// TreeView view


#include "ResultPlotWnd.h"
#include "wet_hDoc.h"



class TreeView : public CTreeView
{
protected:
	TreeView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(TreeView)

// Attributes
public:
		CImageList	m_imageState;

// Operations
public:
	void refresh();
	void OnActivate();
	void OnShowWindow(BOOL bShow, UINT nStatus);
   void SaveFile(DataObj *currentObj, CString name);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TreeView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~TreeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(TreeView)
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRdblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TREEVIEW_H__6D28D01A_0B25_423C_A61C_079DEE82702A__INCLUDED_)
