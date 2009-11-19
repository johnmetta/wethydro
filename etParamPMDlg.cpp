// etParamPMDlg.cpp : implementation file
//

#include "stdafx.h"
#pragma hdrstop
#include "wet_h.h"
#include "etParamPMDlg.h"
#include "wizuserinteraction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern WizUserInteraction *gpWizUserInteraction;

/////////////////////////////////////////////////////////////////////////////
// etParamPMDlg dialog


etParamPMDlg::etParamPMDlg(CWnd* pParent /*=NULL*/)
	: CDialog(etParamPMDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(etParamPMDlg)
	m_ac = gpWizUserInteraction->m_ac;
	m_airDensity = gpWizUserInteraction->m_airDensity;
	m_bc = gpWizUserInteraction->m_bc;
	m_bioGeoAbsorp = gpWizUserInteraction->m_bioGeoAbsorp;
	m_extraTerrRadiation = gpWizUserInteraction->m_extraTerrRadiation;
	m_clearFraction = gpWizUserInteraction->m_clearFraction;
	m_cloudFraction = gpWizUserInteraction->m_cloudFraction;
	m_gradientSatVapor = gpWizUserInteraction->m_gradientSatVapor;
	m_heatCond = gpWizUserInteraction->m_heatCond;
	m_heightOfHumid = gpWizUserInteraction->m_heightOfHumid;
	m_heightOfWind = gpWizUserInteraction->m_heightOfWind;
	m_shortWave = gpWizUserInteraction->m_shortWave;
	m_psychrometric = gpWizUserInteraction->m_psychrometric;
	m_relativeHumid = gpWizUserInteraction->m_relativeHumid;
	m_satVapPres = gpWizUserInteraction->m_satVapPres;
	m_stephanBoltz = gpWizUserInteraction->m_stephanBoltz;
	m_specificHeat = gpWizUserInteraction->m_specificHeat;
	m_storedE = gpWizUserInteraction->m_storedE;
	m_windSpeed = gpWizUserInteraction->m_windSpeed;
	//}}AFX_DATA_INIT
}


void etParamPMDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(etParamPMDlg)
	DDX_Text(pDX, IDC_AC, m_ac);
	DDX_Text(pDX, IDC_AIRDEN, m_airDensity);
	DDX_Text(pDX, IDC_BC, m_bc);
	DDX_Text(pDX, IDC_BIOGEOABS, m_bioGeoAbsorp);
	DDX_Text(pDX, IDC_ETR, m_extraTerrRadiation);
	DDX_Text(pDX, IDC_FRARACLEAR, m_clearFraction);
	DDX_Text(pDX, IDC_FRRADCLOUD, m_cloudFraction);
	DDX_Text(pDX, IDC_GRADSATVAPP, m_gradientSatVapor);
	DDX_Text(pDX, IDC_HEATCOND, m_heatCond);
	DDX_Text(pDX, IDC_HEIGHTHUM, m_heightOfHumid);
	DDX_Text(pDX, IDC_HEIGHTWIND, m_heightOfWind);
	DDX_Text(pDX, IDC_INCSHORT, m_shortWave);
	DDX_Text(pDX, IDC_LHE, m_latentHeatEvap);
	DDX_Text(pDX, IDC_PSYCH, m_psychrometric);
	DDX_Text(pDX, IDC_RH, m_relativeHumid);
	DDX_Text(pDX, IDC_SATVAPP, m_satVapPres);
	DDX_Text(pDX, IDC_SBC, m_stephanBoltz);
	DDX_Text(pDX, IDC_SPEHEAT, m_specificHeat);
	DDX_Text(pDX, IDC_STOREDE, m_storedE);
	DDX_Text(pDX, IDC_WINDSPEED, m_windSpeed);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(etParamPMDlg, CDialog)
	//{{AFX_MSG_MAP(etParamPMDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// etParamPMDlg message handlers

void etParamPMDlg::OnOK() 
{
	
	CDialog::OnOK();

	gpWizUserInteraction->m_ac = m_ac;
	gpWizUserInteraction->m_airDensity = m_airDensity;
	gpWizUserInteraction->m_bc = m_bc;
	gpWizUserInteraction->m_bioGeoAbsorp = m_bioGeoAbsorp;
	gpWizUserInteraction->m_extraTerrRadiation = m_extraTerrRadiation;
	gpWizUserInteraction->m_clearFraction = m_clearFraction;
	gpWizUserInteraction->m_cloudFraction = m_cloudFraction;
	gpWizUserInteraction->m_gradientSatVapor = m_gradientSatVapor;
	gpWizUserInteraction->m_heatCond = m_heatCond;
	gpWizUserInteraction->m_heightOfHumid = m_heightOfHumid;
	gpWizUserInteraction->m_heightOfWind = m_heightOfWind;
	gpWizUserInteraction->m_shortWave = m_shortWave;
	gpWizUserInteraction->m_psychrometric = m_psychrometric;
	gpWizUserInteraction->m_relativeHumid = m_relativeHumid;
	gpWizUserInteraction->m_satVapPres = m_satVapPres;
	gpWizUserInteraction->m_stephanBoltz = m_stephanBoltz;
	gpWizUserInteraction->m_specificHeat = m_specificHeat;
	gpWizUserInteraction->m_storedE = m_storedE;
	gpWizUserInteraction->m_windSpeed = m_windSpeed;
}
