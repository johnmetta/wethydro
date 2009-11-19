// ppModelEvaluation.cpp : implementation file
//

#include "stdafx.h"
#include "wet_h.h"
#include "ppModelEvaluation.h"
#include "WET_Hydro.h"
#include ".\ppmodelevaluation.h"

extern WET_Hydro *gpHydroModel;


// ppModelEvaluation dialog

IMPLEMENT_DYNAMIC(ppModelEvaluation, CPropertyPage)
ppModelEvaluation::ppModelEvaluation()
	: CPropertyPage(ppModelEvaluation::IDD)
   , m_numIterations(gpHydroModel->m_numIterations)
   , m_singleModelRun(gpHydroModel->m_checkMonteCarlo)
   , m_checkStructure(gpHydroModel->m_checkCStruc)
   , m_checkRates(false)
   , m_checkM(false)
   , m_checkKSat(false)
   , m_checkInitSat(false)
   , m_checkPhi(false)
   , m_checkFieldCapac(false)
   , m_checkSoilDepth(false)
   , m_checkKDepth(false)
   , m_checkVanGenuctN(false)
   , m_checkK1(false)
   , m_checkK2(false)
   , m_checkWD(false)
   , m_checkN(false)
   
  
{
}

ppModelEvaluation::~ppModelEvaluation()
{
}

void ppModelEvaluation::DoDataExchange(CDataExchange* pDX)
{
   CPropertyPage::DoDataExchange(pDX);

   DDX_Text(pDX, IDC_EDITNUMITER, m_numIterations);

   DDX_Radio(pDX, IDC_RADIOMONTE, m_singleModelRun);


   DDX_Control(pDX, IDC_CHECKSTUC, m_checkCStruc);
   DDX_Control(pDX, IDC_CHECKEQUATION, m_checkCEq);
   DDX_Control(pDX, IDC_CHECKPARAMM, m_checkCM);
   DDX_Control(pDX, IDC_CHECKPARAMKSAT, m_checkCKsat);
   DDX_Control(pDX, IDC_CHECKPARAMINIT, m_checkCInit);
   DDX_Control(pDX, IDC_CHECKPARAMPHI, m_checkCPhi);
   DDX_Control(pDX, IDC_CHECKPARAMFC, m_checkCFC);
   DDX_Control(pDX, IDC_CHECKPARAMZ, m_checkCSD);
   DDX_Control(pDX, IDC_CHECKPARAMKD, m_checkCKD);
   DDX_Control(pDX, IDC_CHECKPARAMVAN, m_checkCVan);
   DDX_Control(pDX, IDC_CHECKPARAMK1, m_checkCK1);
   DDX_Control(pDX, IDC_CHECKPARAMK2, m_checkCK2);
   DDX_Control(pDX, IDC_CHECKPARAMWD, m_checkCWD);
   DDX_Control(pDX, IDC_CHECKPARAMN, m_checkCN);
   DDX_Control(pDX, IDC_EDITNUMITER, m_CEditNumIter);
   DDX_Control(pDX, IDC_STATICNUMITER, m_CNumIterText);
   DDX_Control(pDX, IDC_CHECKPARAMWP, m_checkWP);
   DDX_Control(pDX, IDC_CHECKPLE2, m_checkPLE2);
}   


BEGIN_MESSAGE_MAP(ppModelEvaluation, CPropertyPage)
   ON_BN_CLICKED(IDC_RADIOMONTE, OnBnClickedRadiomonte)
   ON_BN_SETFOCUS(IDC_RADIOMONTE, OnBnSetfocusRadiomonte)
   ON_NOTIFY(BCN_HOTITEMCHANGE, IDC_RADIOMONTE, OnBnHotItemChangeRadiomonte)
   ON_BN_CLICKED(IDC_RADIO3, OnBnClickedRadio3)
   ON_BN_CLICKED(IDC_CHECK2, OnBnClickedCheck2)
   ON_BN_CLICKED(IDC_CHECK1, OnBnClickedCheck1)
END_MESSAGE_MAP()


// ppModelEvaluation message handlers

BOOL ppModelEvaluation::OnInitDialog()
{
   CPropertyPage::OnInitDialog();
   ChangeVisibility(FALSE);

   return TRUE;  // return TRUE unless you set the focus to a control
   // EXCEPTION: OCX Property Pages should return FALSE
}

void ppModelEvaluation::OnBnClickedCheck18()
{
   // TODO: Add your control notification handler code here
}


void ppModelEvaluation::OnBnClickedRadiomonte()
   {
   UpdateData(TRUE);
   if (m_singleModelRun==WH_SINGLE_SIMULATION)
      ChangeVisibility(FALSE);    
   else
      ChangeVisibility(TRUE);
      
   }

void ppModelEvaluation::OnBnSetfocusRadiomonte()
   {
   UpdateData(TRUE);
   if (m_singleModelRun==WH_SINGLE_SIMULATION)
      ChangeVisibility(FALSE);
   else
      ChangeVisibility(TRUE);
   }

void ppModelEvaluation::OnBnHotItemChangeRadiomonte(NMHDR *pNMHDR, LRESULT *pResult)
{
   // This feature requires Internet Explorer 6 or greater.
   // The symbol _WIN32_IE must be >= 0x0600.
   LPNMBCHOTITEM pHotItem = reinterpret_cast<LPNMBCHOTITEM>(pNMHDR);
   // TODO: Add your control notification handler code here
   *pResult = 0;
}

void ppModelEvaluation::OnBnClickedRadio3()
   {
   UpdateData(TRUE);
   if (m_singleModelRun==WH_SINGLE_SIMULATION)
      ChangeVisibility(FALSE);  
   else
      ChangeVisibility(TRUE);
   }

void ppModelEvaluation::ChangeVisibility(BOOL val)
   {
   m_checkCStruc.EnableWindow(val);
//   m_checkSoilDepth.EnableWindow(val);
   m_checkCEq.EnableWindow(val);
   m_checkCM.EnableWindow(val);
   m_checkCKsat.EnableWindow(val);
   m_checkCInit.EnableWindow(val);
   m_checkCPhi.EnableWindow(val);
   m_checkCFC.EnableWindow(val);
   m_checkCSD.EnableWindow(val);
   m_checkCVan.EnableWindow(val);
   m_checkCKD.EnableWindow(val);
   m_checkCK1.EnableWindow(val);
   m_checkCK2.EnableWindow(val);
   m_checkCWD.EnableWindow(val);
   m_checkWP.EnableWindow(val);
   m_checkCN.EnableWindow(val);
   m_checkPLE2.EnableWindow(val);
   m_CEditNumIter.EnableWindow(val);
   m_CNumIterText.EnableWindow(val);
   }

   void ppModelEvaluation::OnBnClickedCheck2()
   {
      // TODO: Add your control notification handler code here
   }

   void ppModelEvaluation::OnBnClickedCheck1()
   {
      // TODO: Add your control notification handler code here
   }
