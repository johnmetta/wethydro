// ppModelDefHorizontal.cpp : implementation file
//

#include "stdafx.h"
#include "wet_h.h"
#include "ppModelDefHorizontal.h"
#include "WET_Hydro.h"

extern WET_Hydro *gpHydroModel;

// ppModelDefHorizontal dialog

IMPLEMENT_DYNAMIC(ppModelDefHorizontal, CPropertyPage)
ppModelDefHorizontal::ppModelDefHorizontal()
	: CPropertyPage(ppModelDefHorizontal::IDD)
   , m_radioModelParameters(gpHydroModel->m_modelParamMethod)
   , m_radioModelStructure(gpHydroModel->m_modelStructureMethod)
   , m_bandWidth(gpHydroModel->m_widthBand)
   , m_numberBands((float)gpHydroModel->m_numBands)
   , m_independentSides(gpHydroModel->m_checkSideOfStream)
{
}

ppModelDefHorizontal::~ppModelDefHorizontal()
{
}

void ppModelDefHorizontal::DoDataExchange(CDataExchange* pDX)
{
   CPropertyPage::DoDataExchange(pDX);

   DDX_Text(pDX, IDC_EDITNUMBANDS, m_numberBands);
   DDX_Check(pDX, IDC_CHECKSIDEOFSTREAM, m_independentSides);
   DDX_Radio(pDX, IDC_RADIO9, m_radioModelParameters);
   DDX_Radio(pDX, IDC_RADIOSEMI, m_radioModelStructure);

  // DDX_Control(pDX, IDC_CHECK1, m_cRadioCentroid);
  
   DDX_Control(pDX, IDC_RADIOSEMI, m_cRadioStructure);
   DDX_Control(pDX, IDC_EDITNUMBANDS, m_cEditNumBands);
   DDX_Control(pDX, IDC_STATICNUMBANDS, m_cStatNumBands);
   DDX_Control(pDX, IDC_CHECKSIDEOFSTREAM, m_cCheckSideOfStream);
}


BEGIN_MESSAGE_MAP(ppModelDefHorizontal, CPropertyPage)
   ON_BN_CLICKED(IDC_RADIO13, OnBnClickedRadio13)
   ON_BN_CLICKED(IDC_RADIO14, OnBnClickedRadio14)
   ON_BN_CLICKED(IDC_RADIO15, OnBnClickedRadio15)
   ON_BN_CLICKED(IDC_RADIO9, OnBnClickedRadio9)
   ON_BN_CLICKED(IDC_RADIO16, OnBnClickedRadio16)
END_MESSAGE_MAP()


// ppModelDefHorizontal message handlers

void ppModelDefHorizontal::OnBnClickedRadio5()
{
   // TODO: Add your control notification handler code here
}

void ppModelDefHorizontal::OnBnClickedRadio13()
{
   UpdateData(TRUE);
   ChangeVisibility();

   // TODO: Add your control notification handler code here
}

void ppModelDefHorizontal::OnBnClickedRadio7()
{
   // TODO: Add your control notification handler code here
}

void ppModelDefHorizontal::OnBnClickedRadio14()
{   
   UpdateData(TRUE);
   ChangeVisibility();
   // TODO: Add your control notification handler code here
}

void ppModelDefHorizontal::OnBnClickedRadio15()
{
      UpdateData(TRUE);
   ChangeVisibility();// TODO: Add your control notification handler code here
}

void ppModelDefHorizontal::OnBnClickedRadio9()
{
   // TODO: Add your control notification handler code here
}

BOOL ppModelDefHorizontal::OnInitDialog()
{
   CPropertyPage::OnInitDialog();

   if (m_radioModelStructure==WH_DISTRIBUTE_WATERSHEDS )
      {
      m_cEditNumBands.EnableWindow(FALSE);
      m_cStatNumBands.EnableWindow(FALSE);
      m_cCheckSideOfStream.EnableWindow(FALSE);
      }
   if ( m_radioModelStructure==WH_DISTRIBUTE_HILLRIPARIAN)
      {
      m_cEditNumBands.EnableWindow(FALSE);
      m_cStatNumBands.EnableWindow(FALSE);

      }

   return TRUE;  // return TRUE unless you set the focus to a control
   // EXCEPTION: OCX Property Pages should return FALSE
}
void ppModelDefHorizontal::ChangeVisibility(void)
   {
   if (m_radioModelStructure==WH_DISTRIBUTE_CENTROIDDISTANCEBINS)
      {
      m_cEditNumBands.EnableWindow(TRUE);
      m_cStatNumBands.EnableWindow(TRUE);
      m_cCheckSideOfStream.EnableWindow(TRUE);
      }


   else if (m_radioModelStructure==WH_DISTRIBUTE_WATERSHEDS)
      {
      m_cCheckSideOfStream.EnableWindow(FALSE);
      m_cEditNumBands.EnableWindow(FALSE);
      m_cStatNumBands.EnableWindow(FALSE);
      }
   else
      {
      m_cCheckSideOfStream.EnableWindow(TRUE);
      m_cEditNumBands.EnableWindow(TRUE);
      m_cStatNumBands.EnableWindow(TRUE);
      }
   }

void ppModelDefHorizontal::OnBnClickedRadio16()
{
      UpdateData(TRUE);
   ChangeVisibility();// TODO: Add your control notification handler code here
}
