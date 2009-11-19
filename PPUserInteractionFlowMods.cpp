// PPUserInteractionFlowMods.cpp : implementation file
//

#include "stdafx.h"
#include "wet_h.h"
#include "PPUserInteractionFlowMods.h"
#include "WET_Hydro.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern WET_Hydro *gpHydroModel;
/////////////////////////////////////////////////////////////////////////////
// PPUserInteractionFlowMods property page

IMPLEMENT_DYNCREATE(PPUserInteractionFlowMods, CPropertyPage)

PPUserInteractionFlowMods::PPUserInteractionFlowMods() : CPropertyPage(PPUserInteractionFlowMods::IDD)
{
	//{{AFX_DATA_INIT(PPUserInteractionFlowMods)
   CTime t(1999, 7, 01, 22, 15, 0 );
   CTime t2(1999, 9, 15, 22, 15, 0 );
   m_checkIncrFlow = gpHydroModel->m_checkIncrFlow;
	m_incrFlowBegin = t;
	m_incrFlowEnd = t2;
	m_incrFlowPer = gpHydroModel->m_incrFlowPercent; // 1cfs converted to m3/s
	//}}AFX_DATA_INIT
}

PPUserInteractionFlowMods::~PPUserInteractionFlowMods()
{
}

void PPUserInteractionFlowMods::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PPUserInteractionFlowMods)
	DDX_Check(pDX, IDC_CHECKINCRFLOW, m_checkIncrFlow);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKERBEGIN, m_incrFlowBegin);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKEREND, m_incrFlowEnd);
	DDX_Text(pDX, IDC_EDITPERINCREASE, m_incrFlowPer);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(PPUserInteractionFlowMods, CPropertyPage)
	//{{AFX_MSG_MAP(PPUserInteractionFlowMods)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PPUserInteractionFlowMods message handlers
