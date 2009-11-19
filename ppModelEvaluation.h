#pragma once
#include "afxwin.h"


// ppModelEvaluation dialog

class ppModelEvaluation : public CPropertyPage
{
	DECLARE_DYNAMIC(ppModelEvaluation)

public:
	ppModelEvaluation();
	virtual ~ppModelEvaluation();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_EVALUATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
   virtual BOOL OnInitDialog();
   afx_msg void OnBnClickedCheck18();
   int m_numIterations;
   // 0 is true, meaning run a single time.  1 is false, meaning run multiple times in a monte carlo sense
   int m_singleModelRun;
   bool m_checkStructure;
   bool m_checkRates;
   bool m_checkM;
   bool m_checkKSat;
   bool m_checkInitSat;
   bool m_checkPhi;
   bool m_checkFieldCapac;
   bool m_checkSoilDepth;
   bool m_checkKDepth;
   bool m_checkVanGenuctN;
   bool m_checkK1;
   bool m_checkK2;
   bool m_checkWD;
   bool m_checkN;
   CButton m_checkCStruc;
   CButton m_checkCEq;
   CButton m_checkCM;
   CButton m_checkCKsat;
   CButton m_checkCInit;
   CButton m_checkCPhi;
   CButton m_checkCFC;
   CButton m_checkCSD;
   CButton m_checkCKD;
   CButton m_checkCVan;
   CButton m_checkCK1;
   CButton m_checkCK2;
   CButton m_checkCWD;
   CButton m_checkCN;
   afx_msg void OnBnClickedRadiomonte();
   afx_msg void OnBnSetfocusRadiomonte();
   afx_msg void OnBnHotItemChangeRadiomonte(NMHDR *pNMHDR, LRESULT *pResult);
   afx_msg void OnBnClickedRadio3();
   CEdit m_CEditNumIter;
   CStatic m_CNumIterText;
void ChangeVisibility(BOOL val);
// the wilting point of the soils
CButton m_checkWP;
afx_msg void OnBnClickedCheck2();

CButton m_checkPLE2;
afx_msg void OnBnClickedCheck1();
};
