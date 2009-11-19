// PPUserInteractionSediment.cpp : implementation file
//

#include "stdafx.h"
#pragma hdrstop
#include "wet_h.h"
#include "PPUserInteractionSediment.h"
#include "WET_Hydro.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern WET_Hydro *gpHydroModel ;

/////////////////////////////////////////////////////////////////////////////
// PPUserInteractionSediment property page

IMPLEMENT_DYNCREATE(PPUserInteractionSediment, CPropertyPage)

PPUserInteractionSediment::PPUserInteractionSediment() : CPropertyPage(PPUserInteractionSediment::IDD)
{
	//{{AFX_DATA_INIT(PPUserInteractionSediment)
	m_checkUplandSediment = gpHydroModel->m_checkUplandSediment;
	m_reduceSedPercent = gpHydroModel->m_reduceSedPercent;
	m_applySedOptions = gpHydroModel->m_checkApplySedOptions ;
	m_bufferWidth = gpHydroModel->m_bufferWidth;
	//}}AFX_DATA_INIT
}

PPUserInteractionSediment::~PPUserInteractionSediment()
{
}

void PPUserInteractionSediment::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PPUserInteractionSediment)
	DDX_Check(pDX, IDC_CHECKUPLANDSEDIMENT, m_checkUplandSediment);
	DDX_Text(pDX, IDC_REDUCESEDIMENTPER, m_reduceSedPercent);
	DDX_Check(pDX, IDC_APPLYSEDOPTIONS, m_applySedOptions);
	DDX_Text(pDX, IDC_EDITBUFFERWIDTH, m_bufferWidth);
	DDV_MinMaxFloat(pDX, m_bufferWidth, 1.f, 500.f);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(PPUserInteractionSediment, CPropertyPage)
	//{{AFX_MSG_MAP(PPUserInteractionSediment)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PPUserInteractionSediment message handlers
