#if !defined(AFX_MCPARAMDLG_H__87F65579_3BE6_4CD3_ABA8_336C557EE1D1__INCLUDED_)
#define AFX_MCPARAMDLG_H__87F65579_3BE6_4CD3_ABA8_336C557EE1D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MCParamDlg.h : header file
//
#include "MapWnd.h"

#include <randgen\randunif.hpp>
#include <randgen\randLN.hpp>
#include <randgen\randnorm.hpp>
//
extern MapWnd *gpMapWnd;
/////////////////////////////////////////////////////////////////////////////
// MCParamDlg dialog





class MCParamDlg : public CDialog
{
// Construction
public:
	MCParamDlg(CWnd* pParent = gpMapWnd);   // standard constructor

// Dialog Data
	//{{AFX_DATA(MCParamDlg)
	enum { IDD = IDD_MCPARAM };
	CComboBox	m_comboPhi;
	CComboBox	m_comboKDepth;
	CComboBox	m_comboUp;
	CComboBox	m_comboN;
	CComboBox	m_comboM;
	CComboBox	m_comboKsat;
	CComboBox	m_comboIS;
	BOOL	m_checkIS;
	BOOL	m_checkKSat;
	BOOL	m_checkM;
	BOOL	m_checkN;
	int		m_editNumIter;
	float	m_editRandomSeed;
	BOOL	m_checkUpslope;
	BOOL	m_checkKDepth;
	BOOL	m_checkPorosity;
	//}}AFX_DATA

   void GenerateRandomValues( );
   RandUniform m_randUnif1;
   RandUniform m_randUnif2;
   RandUniform m_randUnif3;
   RandUniform m_randUnif4;
   RandUniform m_randUnif5;
   RandUniform m_randUnif6;

   RandNormal *m_pRandN1;
   RandNormal *m_pRandN2;
   RandNormal *m_pRandN3;
   RandNormal *m_pRandN4;
   RandNormal *m_pRandN5;
   RandNormal *m_pRandN6;

   float   m_mMin;
   float   m_mMax;
   float   m_mMean;
   float   m_mStd;
   float   m_isMin;
   float   m_isMax;
   float   m_isMean;
   float   m_isStd;
   float   m_nMin;
   float   m_nMax;
   float   m_nMean;
   float   m_nStd;
   float   m_ksMin;
   float   m_ksMax;
   float   m_ksMean;
   float   m_ksStd;
   int     m_currentParam;
   float   m_kDepthMin;
   float   m_kDepthMax;
   float   m_kDepthMean;
   float   m_kDepthStd;
   float   m_phiMin;
   float   m_phiMax;
   float   m_phiMean;
   float   m_phiStd;




// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MCParamDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(MCParamDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDoubleclickedCheckis();
	afx_msg void OnDblclkCombois();
	afx_msg void OnButtonn();
	afx_msg void OnButtonm();
	afx_msg void OnButtonk();
	afx_msg void OnButtonis();
	afx_msg void OnButtonu();
	virtual void OnOK();
	afx_msg void OnButtonkdepth();
	afx_msg void OnButtonporosity();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MCPARAMDLG_H__87F65579_3BE6_4CD3_ABA8_336C557EE1D1__INCLUDED_)
