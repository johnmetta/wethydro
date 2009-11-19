// ppModelDefChannel.cpp : implementation file
//

#include "stdafx.h"
#include "wet_h.h"
#include "ppModelDefChannel.h"
#include "WET_Hydro.h"
#include ".\ppmodeldefchannel.h"
extern WET_Hydro *gpHydroModel;


// ppModelDefChannel dialog

IMPLEMENT_DYNAMIC(ppModelDefChannel, CPropertyPage)
ppModelDefChannel::ppModelDefChannel()
	: CPropertyPage(ppModelDefChannel::IDD)
   , m_radioInstreamRoutingMethod(gpHydroModel->m_routeChannelWaterMethod)
   , m_radioInstreamSolutes(gpHydroModel->m_routeChannelSolutesMethod)
{
}

ppModelDefChannel::~ppModelDefChannel()
{
}

void ppModelDefChannel::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
   DDX_Radio(pDX, IDC_RADIO1, m_radioInstreamRoutingMethod);
   DDX_Radio(pDX, IDC_RADIO3, m_radioInstreamSolutes);
}


BEGIN_MESSAGE_MAP(ppModelDefChannel, CPropertyPage)
 
END_MESSAGE_MAP()


// ppModelDefChannel message handlers

void ppModelDefChannel::OnBnClickedRadiosum()
{
   // TODO: Add your control notification handler code here
}
