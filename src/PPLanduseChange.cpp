// PPLanduseChange.cpp : implementation file
//

#include "stdafx.h"
#pragma hdrstop
#include "wet_h.h"
#include "PPLanduseChange.h"
#include "WET_Hydro.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern WET_Hydro *gpHydroModel;

/////////////////////////////////////////////////////////////////////////////
// PPLanduseChange property page

IMPLEMENT_DYNCREATE(PPLanduseChange, CPropertyPage)

PPLanduseChange::PPLanduseChange() : CPropertyPage(PPLanduseChange::IDD)
{
	//{{AFX_DATA_INIT(PPLanduseChange)
	m_checkModelWetlands = gpHydroModel->m_checkSimulateWetlands;
	m_checkModelTileDrains = gpHydroModel->m_checkModelTileDrains;;
	m_tileOuflowCoef = gpHydroModel->m_tileOutflowCoef;
	m_tileThreshold = gpHydroModel->m_tileThreshold;
	//}}AFX_DATA_INIT
}

PPLanduseChange::~PPLanduseChange()
{
}

void PPLanduseChange::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PPLanduseChange)
	DDX_Check(pDX, IDC_CHECKMODELWETLANDS, m_checkModelWetlands);
	DDX_Check(pDX, IDC_CHECKMODELTILEDRAINS, m_checkModelTileDrains);
	DDX_Text(pDX, IDC_EDITTILEOUTFLOW, m_tileOuflowCoef);
	DDX_Text(pDX, IDC_EDITTILETHRESH, m_tileThreshold);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(PPLanduseChange, CPropertyPage)
	//{{AFX_MSG_MAP(PPLanduseChange)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PPLanduseChange message handlers
