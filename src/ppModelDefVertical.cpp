// ppModelDefVertical.cpp : implementation file
//

#include "stdafx.h"
#include "wet_h.h"
#include "ppModelDefVertical.h"
#include "WET_Hydro.h"
#include ".\ppmodeldefvertical.h"
extern WET_Hydro *gpHydroModel;


// ppModelDefVertical dialog

IMPLEMENT_DYNAMIC(ppModelDefVertical, CPropertyPage)
ppModelDefVertical::ppModelDefVertical()
	: CPropertyPage(ppModelDefVertical::IDD)
   , m_simulateInterception(gpHydroModel->m_checkInterception)
   , m_radioET(gpHydroModel->m_etMethod)
   , m_radioUnsat(gpHydroModel->m_unsatMethod)
   , m_radioSat(gpHydroModel->m_satMethod)
   , m_checkET(gpHydroModel->m_checkEtModel)
   , m_checkUnsat(gpHydroModel->m_checkUnsaturated)
   , m_cRadioPenman(0)
   , m_cRadioPM(0)
   , m_cRadioMeas(0)
 //  , m_radioPorosity(gpHydroModel->m_radioPorosity)
   , m_checkSnow(gpHydroModel->m_checkSnow)
   , m_radioPorosityInt(gpHydroModel->m_radioPorosity)
   , m_checkGridMet(gpHydroModel->m_checkGridMet)
{
}

ppModelDefVertical::~ppModelDefVertical()
{
}

void ppModelDefVertical::DoDataExchange(CDataExchange* pDX)
{
   CPropertyPage::DoDataExchange(pDX);
   DDX_Check(pDX, IDC_CHECKINTERCEPTION, m_simulateInterception);
   DDX_Check(pDX, IDC_CHECKET, m_checkET);
   DDX_Check(pDX, IDC_CHECKUNSAT, m_checkUnsat);
   DDX_Radio(pDX, IDC_RADIOHARGREAVES, m_radioET);
   DDX_Radio(pDX, IDC_RADIOVANGEN, m_radioUnsat);
   DDX_Radio(pDX, IDC_RADIOSTOPMODEL, m_radioSat);
   DDX_Control(pDX, IDC_CHECKUNSAT, m_cCheckUnsat);
   DDX_Control(pDX, IDC_CHECKET, m_cCheckET);
   DDX_Control(pDX, IDC_RADIOHARGREAVES, m_cRadioET);
   DDX_Control(pDX, IDC_RADIOVANGEN, m_cRatioUnsat);
   DDX_Control(pDX, IDC_RADIOPOROSITYCON, m_radioPorosity);
   DDX_Check(pDX, IDC_CHECKSNOW, m_checkSnow);
   DDX_Radio(pDX, IDC_RADIOPOROSITYCON, m_radioPorosityInt);
   DDX_Check(pDX, IDC_CHECKGRIDMET, m_checkGridMet);
}


BEGIN_MESSAGE_MAP(ppModelDefVertical, CPropertyPage)
   ON_BN_CLICKED(IDC_RADIO4, OnBnClickedRadio4)
//   ON_BN_CLICKED(IDC_RADIO13, OnBnClickedRadio13)
ON_BN_CLICKED(IDC_RADIOHARGREAVES, OnBnClickedRadiohargreaves)
ON_BN_CLICKED(IDC_CHECKET, OnBnClickedChecket)
ON_BN_CLICKED(IDC_CHECKUNSAT, OnBnClickedCheckunsat)
ON_BN_CLICKED(IDC_RADIOMEAS, OnBnClickedRadiomeas)
ON_BN_CLICKED(IDC_RADIOSEIMC, OnBnClickedRadioseimc)
ON_BN_CLICKED(IDC_RADIOSTOPMODEL, OnBnClickedRadiostopmodel)
//ON_EN_CHANGE(IDC_EDITSHAPEPARAM, OnEnChangeEditshapeparam)
ON_BN_CLICKED(IDC_CHECKINTERCEPTION, OnBnClickedCheckinterception)

END_MESSAGE_MAP()


// ppModelDefVertical message handlers

void ppModelDefVertical::OnBnClickedRadio4()
{
   // TODO: Add your control notification handler code here
}



BOOL ppModelDefVertical::OnInitDialog()
 {

   CPropertyPage::OnInitDialog();
    ChangeVisibility();
    m_radioPorosity.SetDlgItemInt(IDC_RADIOPOROSITYCON,gpHydroModel->m_radioPorosity);
    UpdateData(false);
   return TRUE;  // return TRUE unless you set the focus to a control
   // EXCEPTION: OCX Property Pages should return FALSE
   }

void ppModelDefVertical::ChangeVisibility(void)
   {
   if (m_cCheckET.GetCheck()==1)
      {
      m_cRadioET.EnableWindow(TRUE);
      GetDlgItem(IDC_RADIOPEN)->EnableWindow(TRUE);
      GetDlgItem(IDC_RADIOPENMON)->EnableWindow(TRUE);
      GetDlgItem(IDC_RADIOMEAS)->EnableWindow(TRUE);
      }
   else
   {
      m_cRadioET.EnableWindow(FALSE);
      GetDlgItem(IDC_RADIOPEN)->EnableWindow(FALSE);
      GetDlgItem(IDC_RADIOPENMON)->EnableWindow(FALSE);
      GetDlgItem(IDC_RADIOMEAS)->EnableWindow(FALSE);
   }

   if (m_cCheckUnsat.GetCheck()==1)
      {
      m_cRatioUnsat.EnableWindow(TRUE);
      GetDlgItem(IDC_RADIOSEIMC)->EnableWindow(TRUE);
      }
   else
      {
      m_cRatioUnsat.EnableWindow(FALSE);
      GetDlgItem(IDC_RADIOSEIMC)->EnableWindow(FALSE);
      }


   }

void ppModelDefVertical::OnBnClickedRadiohargreaves()
   {
   UpdateData(TRUE);
   ChangeVisibility();
   }

void ppModelDefVertical::OnBnClickedChecket()
   {
   UpdateData(TRUE);
   ChangeVisibility();
   }

void ppModelDefVertical::OnBnClickedCheckunsat()
   {
   UpdateData(TRUE);
   ChangeVisibility();
   }

   void ppModelDefVertical::OnBnClickedRadiomeas()
   {
      // TODO: Add your control notification handler code here
   }

   void ppModelDefVertical::OnBnClickedRadioseimc()
   {
      // TODO: Add your control notification handler code here
   }

   void ppModelDefVertical::OnBnClickedRadiostopmodel()
   {
      // TODO: Add your control notification handler code here
   }

   void ppModelDefVertical::OnEnChangeEditshapeparam()
   {
      // TODO:  If this is a RICHEDIT control, the control will not
      // send this notification unless you override the CPropertyPage::OnInitDialog()
      // function and call CRichEditCtrl().SetEventMask()
      // with the ENM_CHANGE flag ORed into the mask.

      // TODO:  Add your control notification handler code here
   }

   void ppModelDefVertical::OnBnClickedCheckinterception()
   {
      // TODO: Add your control notification handler code here
   }


