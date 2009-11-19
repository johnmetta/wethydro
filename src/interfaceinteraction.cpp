// interfaceinteraction.cpp : implementation file
//

#include "stdafx.h"
#pragma hdrstop
#include "wet_h.h"
#include "interfaceinteraction.h"
#include "wet_hdoc.h"
#include "Wet_Hydro.h"
#include "MapWnd.h"
#include ".\interfaceinteraction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern WET_Hydro *gpHydroModel;
extern CWet_hDoc *gpDoc;
extern MapWnd *gpMapWnd;

/////////////////////////////////////////////////////////////////////////////
// interfaceinteraction property page

IMPLEMENT_DYNCREATE(interfaceinteraction, CPropertyPage)

interfaceinteraction::interfaceinteraction() : CPropertyPage(interfaceinteraction::IDD)

{
	//{{AFX_DATA_INIT(interfaceinteraction)
	m_updatestream = TRUE;
	m_maxq = gpMapWnd->m_maxBinValue;
	m_minq = gpMapWnd->m_minBinValue;
	m_numbin = 10;
	m_widthline = gpMapWnd->m_lineWidth;
	m_updateupland = TRUE;
	m_collectionint = gpHydroModel->m_collectionInterval;
	m_upnumbin = 10;
	m_upmax = gpMapWnd->m_maxBinValueUpland;
	m_upmin = gpMapWnd->m_minBinValueUpland;
	m_radioq = 0;
	m_radioswc = 0;
   m_updateClassificationExtent = gpHydroModel->m_checkMassBalance;
	m_saveEmfOutput = gpHydroModel->m_saveEmfOutput;
	m_spinUp = gpHydroModel->m_spinUp;
   m_updatestream = gpHydroModel->m_updateStream;
   m_updateupland = gpHydroModel->m_updateUpland;
	
	m_radioswc = gpHydroModel->m_cellDataToCollect;
	//}}AFX_DATA_INIT
}

interfaceinteraction::~interfaceinteraction()
{
}

void interfaceinteraction::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(interfaceinteraction)
	DDX_Check(pDX, IDC_CHECKUPDATESTREAM, m_updatestream);
	DDX_Text(pDX, IDC_MAXQ, m_maxq);
	DDX_Text(pDX, IDC_MINQ, m_minq);
	DDX_Text(pDX, IDC_NUMBIN, m_numbin);
	DDX_Text(pDX, IDC_WIDTHLINE, m_widthline);
	DDX_Check(pDX, IDC_CHECKUPDATEUPLAND, m_updateupland);
	DDX_Text(pDX, IDC_COLLECTIONINT, m_collectionint);
	DDX_Text(pDX, IDC_UPNUMBIN, m_upnumbin);
	DDX_Text(pDX, IDC_UPMAX, m_upmax);
	DDX_Text(pDX, IDC_UPMIN, m_upmin);
	DDX_Radio(pDX, IDC_RADIOQ, m_radioq);
	DDX_Radio(pDX, IDC_RADIOSWC, m_radioswc);
	DDX_Check(pDX, IDC_CHECKUPDATECLASSEXTENT, m_updateClassificationExtent);
	DDX_Check(pDX, IDC_SAVEEMFOUT, m_saveEmfOutput);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_COLLECTIONINT2, m_spinUp);
	DDV_MinMaxInt(pDX, m_spinUp, 0, 500);
}


BEGIN_MESSAGE_MAP(interfaceinteraction, CPropertyPage)
	//{{AFX_MSG_MAP(interfaceinteraction)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
   ON_BN_CLICKED(IDC_CHECKUPDATECLASSEXTENT, OnBnClickedCheckupdateclassextent)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// interfaceinteraction message handlers

void interfaceinteraction::OnBnClickedCheckupdateclassextent()
{
   // TODO: Add your control notification handler code here
}
