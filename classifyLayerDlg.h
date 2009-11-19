#if !defined(AFX_CLASSIFYLAYERDLG_H__549185FE_6E96_4270_AF03_DF3AFCE1F80C__INCLUDED_)
#define AFX_CLASSIFYLAYERDLG_H__549185FE_6E96_4270_AF03_DF3AFCE1F80C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClassifyLayerDlg.h : header file
//
class MapLayer;

#include <afxcoll.h>

/////////////////////////////////////////////////////////////////////////////
// classifyLayerDlg dialog

class ClassifyLayerDlg : public CDialog
{
// Construction
public:
	ClassifyLayerDlg(MapLayer*,CWnd* pParent = NULL);   // standard constructor

	MapLayer *m_pLayer;
   CStringArray m_rangeStringArray;

// Dialog Data
	//{{AFX_DATA(classifyLayerDlg)
	enum { IDD = IDD_ClassifyLayerDlg };
	CButton	m_uniqueValues;
	CSpinButtonCtrl	m_binCountSpin;
	CButton	m_generateBins;
	CStatic	m_fieldType;
	CComboBox	m_dataField;
	CComboBox	m_colorRamp;
	CListCtrl	m_binList;
	CEdit	m_binCount;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(classifyLayerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void UpdateInterface( void );
   void LoadBins( void );

	// Generated message map functions
	//{{AFX_MSG(classifyLayerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnGeneratebins();
	virtual void OnOK();
	afx_msg void OnSelchangeDatafield();
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLASSIFYLAYERDLG_H__549185FE_6E96_4270_AF03_DF3AFCE1F80C__INCLUDED_)
