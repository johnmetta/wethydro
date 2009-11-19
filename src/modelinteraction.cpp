// modelinteraction.cpp : implementation file
//

#include "stdafx.h"
#pragma hdrstop
#include "wet_h.h"
#include "modelinteraction.h"
#include "wet_hydro.h"
#include "MCParamDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern WET_Hydro *gpHydroModel;

/////////////////////////////////////////////////////////////////////////////
// modelinteraction property page

IMPLEMENT_DYNCREATE(modelinteraction, CPropertyPage)

modelinteraction::modelinteraction() : CPropertyPage(modelinteraction::IDD)
{
	//{{AFX_DATA_INIT(modelinteraction)
	m_deltax = gpHydroModel->m_deltaX;

	m_ksAsParam = gpHydroModel->m_ksAsParam;
	m_sdAsParam = gpHydroModel->m_sdAsParam;
	m_ltPrecip = gpHydroModel->m_stationLtPrecip;
	m_checkPrism = gpHydroModel->m_checkPrism;
	m_checkUseSoilList = gpHydroModel->m_checkUseSoilList;

	m_checkMonteCarlo = gpHydroModel->m_checkMonteCarlo;
	m_numIterations = gpHydroModel->m_numIterations;;
	m_checkThreeBasins = gpHydroModel->m_checkThreeBasins;

	//}}AFX_DATA_INIT
}

modelinteraction::~modelinteraction()
{
}

void modelinteraction::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(modelinteraction)
	DDX_Text(pDX, IDC_DELTAX, m_deltax);
	DDX_Text(pDX, IDC_IS, m_is);
	DDX_Text(pDX, IDC_KSAT, m_ksat);
	DDX_Text(pDX, IDC_M, m_m);
	DDX_Text(pDX, IDC_N, m_n);
	DDX_Text(pDX, IDC_SOILDEPTH, m_soildepth);
	DDX_Text(pDX, IDC_WDRATIO, m_wdratio);
	DDX_Check(pDX, IDC_KSATASPARAM, m_ksAsParam);
	DDX_Check(pDX, IDC_SDASPARAM, m_sdAsParam);
	DDX_Text(pDX, IDC_LTPRECIP, m_ltPrecip);
	DDX_Check(pDX, IDC_CHECKPRISM, m_checkPrism);
	DDX_Check(pDX, IDC_CHECKUSESOILLIST, m_checkUseSoilList);
	DDX_Text(pDX, IDC_PHI, m_phi);
	DDX_Check(pDX, IDC_CHECKMONTE, m_checkMonteCarlo);
	DDX_Text(pDX, IDC_EDITNUMITERAT, m_numIterations);
	DDX_Check(pDX, IDC_CHECKTHREEGAGES, m_checkThreeBasins);
	DDX_Text(pDX, IDC_KDEPTH, m_kDepth);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(modelinteraction, CPropertyPage)
	//{{AFX_MSG_MAP(modelinteraction)
	ON_BN_CLICKED(IDC_BUTTONMCADDITIONAL, OnButtonmcadditional)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// modelinteraction message handlers

void modelinteraction::OnButtonmcadditional() 
{
      MCParamDlg dlg;
      dlg.DoModal();	
}
