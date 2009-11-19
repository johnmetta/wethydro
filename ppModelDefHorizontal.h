#pragma once
#include "afxwin.h"


// ppModelDefHorizontal dialog

class ppModelDefHorizontal : public CPropertyPage
{
	DECLARE_DYNAMIC(ppModelDefHorizontal)

public:
	ppModelDefHorizontal();
	virtual ~ppModelDefHorizontal();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_HORIZONTAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
   afx_msg void OnBnClickedRadio5();
   afx_msg void OnBnClickedRadio13();
   afx_msg void OnBnClickedRadio7();
   afx_msg void OnBnClickedRadio14();
   afx_msg void OnBnClickedRadio15();
   afx_msg void OnBnClickedRadio9();
   // //Allows the user to select data on which to base model parameterization
   int m_radioModelParameters;
   // a group of radio buttons to allow for selection of model scale and distribution
   int m_radioModelStructure;
   float m_bandWidth;
   float m_numberBands;
   BOOL m_independentSides;
   CButton m_cRadioCentroid;
   CButton m_cRadioStructure;
   CEdit m_cEditNumBands;
   CStatic m_cStatNumBands;
   void ChangeVisibility(void);
   virtual BOOL OnInitDialog();
   afx_msg void OnBnClickedRadio16();
   CButton m_cCheckSideOfStream;
};
