#pragma once


// ppModelDefChannel dialog

class ppModelDefChannel : public CPropertyPage
{
	DECLARE_DYNAMIC(ppModelDefChannel)

public:
	ppModelDefChannel();
	virtual ~ppModelDefChannel();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_CHANNEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
   int m_radioInstreamRoutingMethod;
   int m_radioInstreamSolutes;
   afx_msg void OnBnClickedRadiosum();
};
