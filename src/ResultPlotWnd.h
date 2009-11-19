#if !defined(AFX_ResultPlotWnd_H__A76913A7_C847_11D3_95C5_00A076B0010A__INCLUDED_)
#define AFX_ResultPlotWnd_H__A76913A7_C847_11D3_95C5_00A076B0010A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ResultPlotWnd.h : header file
//
#include "mapwnd.h"
#include <idataobj.h>
#include <fdataobj.h>
#include <ScatterWnd.h>
#include "TreeView.h"
#include "ResultPlotWnd.h"

class TreeView;
class ScatterWnd;

#include <afxtempl.h>

struct RESULT_INFO
   {
   ScatterWnd *pClimateScatter ;
	ScatterWnd *pSwcScatter;
	ScatterWnd *pRunoffScatter;
	ScatterWnd *pInstreamScatter;

   char   name[5];
   int	 runNumber;

   RESULT_INFO( void ) : pClimateScatter( NULL ), pSwcScatter( NULL ), pRunoffScatter( NULL ), pInstreamScatter( NULL ), name(  ), runNumber( 0 ) { }
   };

class ResultInfoArray : public CArray< RESULT_INFO*, RESULT_INFO* >
   {
   public:
      // destructor cleans up any allocated structures
     ~ResultInfoArray( void ) { for ( int i=0; i < GetSize(); i++ ) delete GetAt( i ); }      
   };
class ResultPlotWndArray : public CArray< ScatterWnd*, ScatterWnd* >
   {
   };

/////////////////////////////////////////////////////////////////////////////
// ResultPlotWnd window

class ResultPlotWnd : public CWnd
{
// Construction
public:
	ResultPlotWnd();

// Attributes
public:
   TreeView *m_pResultTree;
	CWnd *m_pPlotWnd;
   CSplitterWnd m_splitterWnd;
	ResultInfoArray m_resultInfoArray;
   ResultPlotWndArray m_ResultPlotWndArray;

// Operations
public:
   void RemoveAll();    // clear out all maps, etc.
//   int *AddDataObject();    // add a new map for storage in results
	ScatterWnd *AllocateScatter( DataObj *pScatterData, LPCTSTR title);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ResultPlotWnd)

	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~ResultPlotWnd();

	// Generated message map functions
		//{{AFX_MSG(ResultPlotWnd)
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnSize(UINT nType, int cx, int cy);

		//}}AFX_MSG

protected:
	//{{AFX_MSG(ResultPlotWnd)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ResultPlotWnd_H__A76913A7_C847_11D3_95C5_00A076B0010A__INCLUDED_)
