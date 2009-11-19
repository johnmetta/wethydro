#pragma once
#include "afxwin.h"


// ppUserInteractionPesticide dialog

class ppUserInteractionPesticide : public CPropertyPage
{
	DECLARE_DYNAMIC(ppUserInteractionPesticide)

public:
	ppUserInteractionPesticide();
	virtual ~ppUserInteractionPesticide();

// Dialog Data
	enum { IDD = IDD_PPUSERINTERACTIONPESTICIDE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
   afx_msg void OnBnClickedCheck1();
   BOOL m_checkPesticide;
   afx_msg void OnBnClickedButton1();
   afx_msg void OnEnChangeEdit1();
   afx_msg void OnEnChangeEditpestappdata();
   // This is the name of the xls file that contains application timing/rate information
   CString m_appDatabaseName;
   int ReadAppData( LPCSTR fileName , CString worksheet);
   int ReadCoefData( LPCSTR fileName, CString worksheet);
   void UpdateModelState();
   int m_numberPesticides;
   CUIntArray m_pesticideOffsets;

   CCheckListBox m_chemicalList;
   afx_msg void OnLbnSelchangeListchemical();
   afx_msg void OnBnClickedEditparam();
   afx_msg void OnBnClickedEditapp();


};
