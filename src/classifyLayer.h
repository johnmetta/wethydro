#if !defined(AFX_ClassifyLayerDlg_H__4C7D1614_12A4_4697_83BE_CB5C00EFF82B__INCLUDED_)
#define AFX_ClassifyLayerDlg_H__4C7D1614_12A4_4697_83BE_CB5C00EFF82B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClassifyLayerDlg.h : header file
#include "resource.h"
#include "MapWnd.h"
//
extern MapWnd *gpMapWnd;
class MapLayer;
class MapWnd;

#include <afxcoll.h>
/////////////////////////////////////////////////////////////////////////////
// ClassifyLayerDlg dialog

class ClassifyLayerDlg : public CDialog
{
// Construction
public:
	ClassifyLayerDlg(MapLayer*,CWnd* pParent = gpMapWnd);   // standard constructor
	MapLayer *m_pLayer;
   CStringArray m_rangeStringArray;
// Dialog Data
	//{{AFX_DATA(ClassifyLayerDlg)
	enum { IDD = IDD_ClassifyLayerDlg };
	CSpinButtonCtrl	m_binCountSpin;
	CEdit	m_binCount;
	CComboBox	m_colorRamp;
	CComboBox	m_dataField;
	CStatic	m_fieldType;
	CButton	m_generateBins;
	CButton	m_uniqueValues;
	CListCtrl	m_binList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ClassifyLayerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void UpdateInterface( void );
   void LoadBins( void );

	// Generated message map functions
	//{{AFX_MSG(ClassifyLayerDlg)
	afx_msg void OnGeneratebins();
	afx_msg void OnSelchangeDatafield();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeColorramp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ClassifyLayerDlg_H__4C7D1614_12A4_4697_83BE_CB5C00EFF82B__INCLUDED_)
