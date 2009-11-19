// PPUserInteractionET.cpp : implementation file
//

#include "stdafx.h"
#pragma hdrstop
#include "wet_h.h"
#include "PPUserInteractionET.h"
#include "etParamHarDlg.h"
#include "etParamPMDlg.h"
#include "WET_Hydro.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern WET_Hydro *gpHydroModel;
/////////////////////////////////////////////////////////////////////////////
// PPUserInteractionET property page

IMPLEMENT_DYNCREATE(PPUserInteractionET, CPropertyPage)

PPUserInteractionET::PPUserInteractionET() : CPropertyPage(PPUserInteractionET::IDD)
{
	//{{AFX_DATA_INIT(PPUserInteractionET)
	m_etMethod = gpHydroModel->m_etMethod;
	m_checkSimEt = gpHydroModel->m_checkEtModel;
	//}}AFX_DATA_INIT
}

PPUserInteractionET::~PPUserInteractionET()
{
}

void PPUserInteractionET::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PPUserInteractionET)
	DDX_Control(pDX, IDC_BUTTONVIEWPARAM, m_buttonViewParam);
	DDX_Radio(pDX, IDC_RADIOHAR, m_etMethod);
	DDX_Check(pDX, IDC_CHECKSIMET, m_checkSimEt);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(PPUserInteractionET, CPropertyPage)
	//{{AFX_MSG_MAP(PPUserInteractionET)
	ON_BN_CLICKED(IDC_BUTTONVIEWPARAM, OnButtonviewparam)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PPUserInteractionET message handlers

void PPUserInteractionET::OnButtonviewparam() 
{
   UpdateData(TRUE);
   

   if (m_etMethod==0)
      {
      etParamHarDlg dlg = new etParamHarDlg;
      dlg.DoModal();	
      }
   if (m_etMethod==1)
      {
      etParamPMDlg dlg = new etParamPMDlg;
      dlg.DoModal();
      }
   }


BOOL PPUserInteractionET::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
   UpdateData();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
