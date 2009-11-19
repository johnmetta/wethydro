#if !defined(AFX_MAPWND_H__27CF1BB8_178F_11D3_95A0_00A076B0010A__INCLUDED_)
#define AFX_MAPWND_H__27CF1BB8_178F_11D3_95A0_00A076B0010A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MapWnd.h : header file
//

class Map;
class MapLayer;
class Poly;
class ScatterWnd;
class DataObj;



#include "MapListView.h"

/////////////////////////////////////////////////////////////////////////////
// MapWnd window

class MapWnd : public CWnd
{
// Construction
public:
	MapWnd();
   MapWnd( MapWnd & );  // copy constructor

// Attributes
public:
   CSplitterWnd m_splitterWnd;
 
   Map         *m_pMap;         // pointers to the two contained windows
   Map         *m_pMap2;
   MapListView *m_pMapList;

   int   m_currentPoly;   // set after clicking...index of the clicked polygon
   bool  m_showAlternative;
   bool  m_isResult;
	int   m_lineWidth;
	float m_minBinValue;
	float m_maxBinValue;
	int	m_numBin;
   float m_minBinValueUpland;
   float m_maxBinValueUpland;
   int   m_numBinUpland;
	BOOL  m_updateClassificationExtent;
   CString m_currentField;
// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MapWnd)
	protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~MapWnd();

   void RefreshList( void ) { if ( m_pMapList != NULL ) m_pMapList->Refresh(); }

	ScatterWnd *AllocateScatter( DataObj *pScatterData, int value );
   ScatterWnd *m_pScatterPrecip;
   ScatterWnd *m_pScatterDischarge;


	// Generated message map functions
public:
	//{{AFX_MSG(MapWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnEditCopy();
	afx_msg void OnShowresults();
	afx_msg void OnUpdateShowresults(CCmdUI* pCmdUI);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnCellproperties();
	afx_msg void OnClearAlternatives();
	afx_msg void OnSelectwatershed();
	afx_msg void OnReclassifydata();
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
   afx_msg void OnSelectField(UINT nID);
   afx_msg void OnMapStatistics();

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAPWND_H__27CF1BB8_178F_11D3_95A0_00A076B0010A__INCLUDED_)
