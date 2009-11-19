// MCParamDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wet_h.h"
#include "MCParamDlg.h"
#include "UniformParam.h"
#include "LogNormParam.h"
#include "WET_hView.h"

extern MCParamDlg *gpMCParamDlg;
extern CWet_hView         *gpView;
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MCParamDlg dialog


MCParamDlg::MCParamDlg(CWnd* gpMapWnd /*=NULL*/)
	: CDialog(MCParamDlg::IDD, gpMapWnd),
  m_randUnif1(),
  m_randUnif2(),
  m_randUnif3(),
  m_randUnif4(),
  m_randUnif5(),
  m_randUnif6(),
  m_pRandN1(NULL),
  m_pRandN2(NULL),
  m_pRandN3(NULL),
  m_pRandN4(NULL),
  m_pRandN5(NULL),
  m_pRandN6(NULL),
  m_mMin(0.1f),
  m_mMax(10.0f),
  m_mMean(5.0f),
  m_mStd(3.0f),
  m_isMin(0.75f),
  m_isMax(1.0f),
  m_isMean(0.75f),
  m_isStd(0.1f),
  m_nMin(0.01f),
  m_nMax(0.2f),
  m_nMean(0.1f),
  m_nStd(0.1f),
  m_ksMin(0.001f),
  m_ksMax(3.0f),
  m_ksMean(1.0f),
  m_ksStd(0.10f),
  m_kDepthMin(0.000001f),
  m_kDepthMax(0.001f),
  m_kDepthMean(0.0001f),
  m_kDepthStd(0.00001f),
  m_phiMin(0.25f),
  m_phiMax(0.35f),
  m_phiMean(0.3f),
  m_phiStd(0.1f),
  m_currentParam(0)


{
	//{{AFX_DATA_INIT(MCParamDlg)
	m_checkIS = FALSE;
	m_checkKSat = FALSE;
	m_checkM = FALSE;
	m_checkN = FALSE;
	m_editNumIter = 0;
	m_editRandomSeed = 0.0f;
	m_checkUpslope = FALSE;
	m_checkKDepth = FALSE;
	m_checkPorosity = FALSE;
	//}}AFX_DATA_INIT
}


void MCParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(MCParamDlg)
	DDX_Control(pDX, IDC_COMBOPOROSITY, m_comboPhi);
	DDX_Control(pDX, IDC_COMBOKDEPTH, m_comboKDepth);
	DDX_Control(pDX, IDC_COMBOUP, m_comboUp);
	DDX_Control(pDX, IDC_COMBON, m_comboN);
	DDX_Control(pDX, IDC_COMBOM, m_comboM);
	DDX_Control(pDX, IDC_COMBOKSAT, m_comboKsat);
	DDX_Control(pDX, IDC_COMBOIS, m_comboIS);
	DDX_Check(pDX, IDC_CHECKIS, m_checkIS);
	DDX_Check(pDX, IDC_CHECKKSAT, m_checkKSat);
	DDX_Check(pDX, IDC_CHECKM, m_checkM);
	DDX_Check(pDX, IDC_CHECKN, m_checkN);
	DDX_Text(pDX, IDC_EDITNUMITERAT, m_editNumIter);
	DDX_Text(pDX, IDC_EDITRANDSEED, m_editRandomSeed);
	DDX_Check(pDX, IDC_CHECKUPSLOPE, m_checkUpslope);
	DDX_Check(pDX, IDC_CHECKKDEPTH, m_checkKDepth);
	DDX_Check(pDX, IDC_CHECKPOROSITY, m_checkPorosity);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(MCParamDlg, CDialog)
	//{{AFX_MSG_MAP(MCParamDlg)
	ON_BN_DOUBLECLICKED(IDC_CHECKIS, OnDoubleclickedCheckis)
	ON_CBN_DBLCLK(IDC_COMBOIS, OnDblclkCombois)
	ON_BN_CLICKED(IDC_BUTTONN, OnButtonn)
	ON_BN_CLICKED(IDC_BUTTONM, OnButtonm)
	ON_BN_CLICKED(IDC_BUTTONK, OnButtonk)
	ON_BN_CLICKED(IDC_BUTTONIS, OnButtonis)
	ON_BN_CLICKED(IDC_BUTTONU, OnButtonu)
	ON_BN_CLICKED(IDC_BUTTONKDEPTH, OnButtonkdepth)
	ON_BN_CLICKED(IDC_BUTTONPOROSITY, OnButtonporosity)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MCParamDlg message handlers

BOOL MCParamDlg::OnInitDialog() 
{

	CDialog::OnInitDialog();	
   m_comboUp.DeleteString(0);
   m_comboUp.AddString( "Uniform" );
   m_comboUp.AddString( "Normal" );	
   m_comboUp.AddString( "LogNormal" );	
   m_comboUp.SetCurSel( 0 );
 
   m_comboN.DeleteString(0);
   m_comboN.AddString( "Uniform" );
   m_comboN.AddString( "Normal" );	
   m_comboN.AddString( "LogNormal" );
   m_comboN.SetCurSel( 0 );

   m_comboM.DeleteString(0);
   m_comboM.AddString( "Uniform" );
   m_comboM.AddString( "Normal" );	
   m_comboM.AddString( "LogNormal" );
   m_comboM.SetCurSel( 0 );

   m_comboKsat.DeleteString(0);
   m_comboKsat.AddString( "Uniform" );
   m_comboKsat.AddString( "Normal" );	
   m_comboKsat.AddString( "LogNormal" );
   m_comboKsat.SetCurSel( 0 );

   m_comboIS.DeleteString(0);
   m_comboIS.AddString( "Uniform" );
   m_comboIS.AddString( "Normal" );	
   m_comboIS.AddString( "LogNormal" );
   m_comboIS.SetCurSel( 0 );
   
   m_comboKDepth.DeleteString(0);
   m_comboKDepth.AddString( "Uniform" );
   m_comboKDepth.AddString( "Normal" );	
   m_comboKDepth.AddString( "LogNormal" );
   m_comboKDepth.SetCurSel( 0 );

   m_comboPhi.DeleteString(0);
   m_comboPhi.AddString( "Uniform" );
   m_comboPhi.AddString( "Normal" );	
   m_comboPhi.AddString( "LogNormal" );
   m_comboPhi.SetCurSel( 0 );

   gpMCParamDlg = this;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void MCParamDlg::OnDoubleclickedCheckis() 
   {


   }

void MCParamDlg::OnDblclkCombois() 
{

	
}
void MCParamDlg::OnButtonm() 
{
   int i = m_comboM.GetCurSel();
   CString name;
   m_comboIS.GetDlgItemText(i,name);
   m_currentParam = 1;
   if (i==0)
      {
      UniformParam dlg;
      dlg.DoModal();
      }

   if (i==1 || i==2)
      {
      LogNormParam dlg;
      dlg.DoModal();
      }	
}
void MCParamDlg::OnButtonis() 
{
   int i = m_comboIS.GetCurSel();
   CString name;
   m_comboIS.GetDlgItemText(i,name);
   m_currentParam = 2;
   if (i==0)
      {

      UniformParam dlg;
      dlg.DoModal();
      }

   if (i==1 || i==2)
      {
      LogNormParam dlg;
      dlg.DoModal();
      }	
}

void MCParamDlg::OnButtonn() 
{
   int i = m_comboN.GetCurSel();
   CString name;
   m_comboIS.GetDlgItemText(i,name);
   m_currentParam = 3;
   if (i==0)
      {
      UniformParam dlg;
      dlg.DoModal();
      }

   if (i==1 || i==2)
      {
      LogNormParam dlg;
      dlg.DoModal();
      }	
}



void MCParamDlg::OnButtonk() 
{
   int i = m_comboKsat.GetCurSel();
   CString name;
   m_comboIS.GetDlgItemText(i,name);
   m_currentParam = 4;
   if (i==0)
      {
      UniformParam dlg;
      dlg.DoModal();
      }

   if (i==1 || i==2)
      { 
      LogNormParam dlg;
      dlg.DoModal();
      }	
}


void MCParamDlg::OnButtonkdepth() 
{
   int i = m_comboKDepth.GetCurSel();
   CString name;
   m_comboKDepth.GetDlgItemText(i,name);
   m_currentParam = 5;
   if (i==0)
      {
      UniformParam dlg;
      dlg.DoModal();
      }

   if (i==1 || i==2)
      {
      LogNormParam dlg;
      dlg.DoModal();
      }	
	
}

void MCParamDlg::OnButtonporosity() 
{
   int i = m_comboPhi.GetCurSel();
   CString name;
   m_comboPhi.GetDlgItemText(i,name);
   m_currentParam = 6;
   if (i==0)
      {
      UniformParam dlg;
      dlg.DoModal();
      }

   if (i==1 || i==2)
      {
      LogNormParam dlg;
      dlg.DoModal();
      }		
}

void MCParamDlg::OnButtonu() 
{
   int i = m_comboUp.GetCurSel();

   m_currentParam = 7;

   if (i==0)
      {
      UniformParam dlg;
      dlg.DoModal();
      }

   if (i==1 || i==2)
      {
      LogNormParam dlg;
      dlg.DoModal();
      }	
}
//  Call only AFTER m_randomNumber has been allocated and values assigned

void MCParamDlg::GenerateRandomValues()
   {
/*   for (int i=1;i<7;i++)
      {
      RANDOM_NUMBER *pRandom = gpView->m_randomNumberArray[i];
      if (pRandom->distNum==0)
         a[i] = (float)m_randUnif1.RandValue(pRandom->minValue,pRandom->maxValue); 
      if (pRandom->distNum==1)
         {
         m_pRandN1->SetMean( pRandom->mean );
         m_pRandN1->SetStdDev( pRandom->std );
         a[i] = (float)m_pRandN1->RandValue();
         }
      }
*/
   }




void MCParamDlg::OnOK() 
{
//  When the user clicks ok, the dlg goes away and also populates the m_randomParameterArray.  This
//  includes adding min mean std max and the selected distribuution to each RANDOM_NUMBER
//  All this needs to be done, because WET_Hview, when the model is run, assumes that the structure
//  is populated.  

   for ( int i=0; i < gpView->m_randomNumberArray.GetSize(); i++ )
      delete gpView->m_randomNumberArray[ i ];

   gpView->m_randomNumberArray.RemoveAll(); 
//  we know there are only 6 parameters
   for (i=1;i<7;i++)
      {
      RANDOM_NUMBER *pRandom = new RANDOM_NUMBER;
      switch (i)  
         {
         case 1:
            {
            pRandom->minValue=m_mMin;
            pRandom->maxValue=m_mMax;
            pRandom->mean=m_mMean;
            pRandom->std=m_mStd;
            int distNum = m_comboM.GetCurSel();
            pRandom->distNum = distNum;
            gpView->m_randomNumberArray.Add( pRandom );
            break;
            }
         case 2:
            {
            pRandom->minValue=m_isMin;
            pRandom->maxValue=m_isMax;
            pRandom->mean=m_isMean;
            pRandom->std=m_isStd;
            int distNum = m_comboIS.GetCurSel();
            pRandom->distNum = distNum;
            gpView->m_randomNumberArray.Add( pRandom );
            break;
            }
         case 3:
            {
            pRandom->minValue=m_nMin;
            pRandom->maxValue=m_nMax;
            pRandom->mean=m_nMean;
            pRandom->std=m_nStd;
            int distNum = m_comboN.GetCurSel();
            pRandom->distNum = distNum;
            gpView->m_randomNumberArray.Add( pRandom );
            break;
            }
         case 4:
            {
            pRandom->minValue=m_ksMin;
            pRandom->maxValue=m_ksMax;
            pRandom->mean=m_ksMean;
            pRandom->std=m_ksStd;
            int distNum = m_comboKsat.GetCurSel();
            pRandom->distNum = distNum;
            gpView->m_randomNumberArray.Add( pRandom );
            break;
            }
         case 5:
            {
            pRandom->minValue=m_kDepthMin;
            pRandom->maxValue=m_kDepthMax;
            pRandom->mean=m_kDepthMean;
            pRandom->std=m_kDepthStd;
            int distNum = m_comboKDepth.GetCurSel();
            pRandom->distNum = distNum;
            gpView->m_randomNumberArray.Add( pRandom );
            break;
            }
         case 6:
            {
            pRandom->minValue=m_phiMin;
            pRandom->maxValue=m_phiMax;
            pRandom->mean=m_phiMean;
            pRandom->std=m_phiStd;
            int distNum = m_comboPhi.GetCurSel();
            pRandom->distNum = distNum;
            gpView->m_randomNumberArray.Add( pRandom );
            break;
            }
         }
      }

	
	CDialog::OnOK();
}


