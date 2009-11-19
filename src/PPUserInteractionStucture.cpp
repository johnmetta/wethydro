// PPUserInteractionStucture.cpp : implementation file
//

#include "stdafx.h"
#pragma hdrstop
#include "wet_h.h"
#include "PPUserInteractionStucture.h"
#include "WET_hydro.h"

extern WET_Hydro *gpHydroModel;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PPUserInteractionStucture property page

IMPLEMENT_DYNCREATE(PPUserInteractionStucture, CPropertyPage)

PPUserInteractionStucture::PPUserInteractionStucture() : CPropertyPage(PPUserInteractionStucture::IDD)
{
	//{{AFX_DATA_INIT(PPUserInteractionStucture)
	m_radiostructure = gpHydroModel->m_modelStructureMethod;
	m_widthofband = gpHydroModel->m_widthBand;
	m_numbands = gpHydroModel->m_numBands;
	m_radioCentroid = 1;
	m_routeChannelWaterMethod = gpHydroModel->m_routeChannelWaterMethod;
//	m_checkRouteChannelSolutes = gpHydroModel->m_routeChannelSolutes;
	m_checkUnsaturated = gpHydroModel->m_checkUnsaturated;
   //}}AFX_DATA_INIT
}

PPUserInteractionStucture::~PPUserInteractionStucture()
{
}

void PPUserInteractionStucture::DoDataExchange(CDataExchange* pDX)
{
   CPropertyPage::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(PPUserInteractionStucture)
   DDX_Control(pDX, IDC_CHECKSIDEOFSTREAM, m_checkSideOfStream);
   DDX_Control(pDX, IDC_BUTTONRECALC, m_buttonRecalc);
   DDX_Control(pDX, IDC_NUMBANDS, m_numbandscontrol);
   DDX_Radio(pDX, IDC_RADIOWATERSHED, m_radiostructure);
   DDX_Text(pDX, IDC_DISTBAND, m_widthofband);
   DDX_Text(pDX, IDC_NUMBANDS, m_numbands);
   DDX_Radio(pDX, IDC_RADIOCENTROID, m_radioCentroid);
   DDX_Check(pDX, IDC_CHECKROUTECHANNEL, m_routeChannelWaterMethod);
   DDX_Check(pDX, IDC_CHECKROUTECHANSOLUTES, m_checkRouteChannelSolutes);
   DDX_Check(pDX, IDC_CHECKUNSATURATED, m_checkUnsaturated);
   //}}AFX_DATA_MAP
   
}


BEGIN_MESSAGE_MAP(PPUserInteractionStucture, CPropertyPage)
	//{{AFX_MSG_MAP(PPUserInteractionStucture)
	ON_BN_CLICKED(IDC_RADIOWATERSHED, OnRadiowatershed)
	ON_BN_CLICKED(IDC_RADIODISTIBUTE, OnRadiodistibute)
	ON_EN_CHANGE(IDC_DISTBAND, OnChangeDistband)
	ON_EN_CHANGE(IDC_NUMBANDS, OnChangeNumbands)
	ON_BN_CLICKED(IDC_RADIOBUFFER, OnRadiobuffer)
	ON_BN_CLICKED(IDC_RADIOCELL, OnRadiocell)
	ON_BN_CLICKED(IDC_RADIOCENTROID, OnRadiocentroid)
	ON_BN_CLICKED(IDC_BUTTONRECALC, OnButtonrecalc)
	ON_BN_CLICKED(IDC_CHECKSIDEOFSTREAM, OnChecksideofstream)
	//}}AFX_MSG_MAP
   ON_BN_CLICKED(IDC_CHECKRIPARIAN, OnBnClickedCheckriparian)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PPUserInteractionStucture message handlers

void PPUserInteractionStucture::OnRadiowatershed() 
	{
   if (m_radiostructure==0)
      {
      m_checkSideOfStream.SetCheck(0);

      }

	CPropertyPage::SetModified( TRUE) ;

	}

void PPUserInteractionStucture::OnRadiodistibute() 
{

	CPropertyPage::SetModified( TRUE) ;
	
}



void PPUserInteractionStucture::OnButtoncalcstructure() 
	{
	CPropertyPage::SetModified( TRUE) ;

	}

void PPUserInteractionStucture::OnChangeDistband() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
		CPropertyPage::SetModified( TRUE) ;
	
}

void PPUserInteractionStucture::OnChangeNumbands() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here		
	CPropertyPage::SetModified( TRUE) ;

	
}

void PPUserInteractionStucture::OnRadiobuffer() 
{
	// TODO: Add your control notification handler code here
		CPropertyPage::SetModified( TRUE) ;
	
}

void PPUserInteractionStucture::OnRadiocell() 
{
	// TODO: Add your control notification handler code here
		CPropertyPage::SetModified( TRUE) ;	
}

void PPUserInteractionStucture::OnRadiocentroid() 
{
	// TODO: Add your control notification handler code here
		CPropertyPage::SetModified( TRUE) ;	
}

BOOL PPUserInteractionStucture::OnApply() 
	{
//	gpHydroModel->CalcWatershedInfo(gpHydroModel->m_is);
	return CPropertyPage::OnApply();
	}

void PPUserInteractionStucture::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CPropertyPage::CalcWindowRect(lpClientRect, nAdjustType);
}

BOOL PPUserInteractionStucture::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}



void PPUserInteractionStucture::OnButtonrecalc() 
{
//	gpHydroModel->CalcWatershedInfo(gpHydroModel->m_is);
	
}

void PPUserInteractionStucture::OnChecksideofstream() 
{

}

void PPUserInteractionStucture::OnBnClickedCheckriparian()
{
   // TODO: Add your control notification handler code here
}
