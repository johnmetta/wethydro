#pragma once
#include "afxwin.h"


// ppModelDefVertical dialog

class ppModelDefVertical : public CPropertyPage
{
	DECLARE_DYNAMIC(ppModelDefVertical)

public:
	ppModelDefVertical();
	virtual ~ppModelDefVertical();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_VERTICAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
   afx_msg void OnBnClickedRadio4();
   afx_msg void OnBnClickedRadio13();
   BOOL m_simulateInterception;
   int m_radioET;
   int m_radioUnsat;
   int m_radioSat;
   BOOL m_checkET;
   BOOL m_checkUnsat;
   virtual BOOL OnInitDialog();
   CButton m_cCheckUnsat;
   CButton m_cCheckET;
   CButton m_cRadioET;
   CButton m_cRatioUnsat;
   void ChangeVisibility(void);
   afx_msg void OnBnClickedRadiohargreaves();
   afx_msg void OnBnClickedChecket();
   afx_msg void OnBnClickedCheckunsat();
   int m_cRadioPenman;
   int m_cRadioPM;
   int m_cRadioMeas;
   afx_msg void OnBnClickedRadiomeas();
   afx_msg void OnBnClickedRadioseimc();
   afx_msg void OnBnClickedRadiostopmodel();
   CButton m_radioPorosity;
   afx_msg void OnEnChangeEditshapeparam();
 
   afx_msg void OnBnClickedCheckinterception();
   BOOL m_checkSnow;
   BOOL m_radioPorosityInt;
   BOOL m_checkGridMet;
};
