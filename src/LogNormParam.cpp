// LogNormParam.cpp : implementation file
//

#include "stdafx.h"
#include "wet_h.h"
#include "LogNormParam.h"

#include "MCParamDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern MCParamDlg *gpMCParamDlg;
/////////////////////////////////////////////////////////////////////////////
// LogNormParam dialog


LogNormParam::LogNormParam(CWnd* pParent /*=NULL*/)
	: CDialog(LogNormParam::IDD, pParent)
{
	//{{AFX_DATA_INIT(LogNormParam)
	m_mean = 0.0f;
	m_std = 0.0f;
	//}}AFX_DATA_INIT
}


void LogNormParam::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(LogNormParam)
	DDX_Text(pDX, IDC_EDIT1, m_mean);
	DDX_Text(pDX, IDC_EDIT2, m_std);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(LogNormParam, CDialog)
	//{{AFX_MSG_MAP(LogNormParam)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// LogNormParam message handlers

void LogNormParam::OnOK() 
{
   UpdateData(TRUE);
   if (gpMCParamDlg->m_currentParam==1)
      {
      gpMCParamDlg->m_mMean  = m_mean;
      gpMCParamDlg->m_mStd = m_std;
      }
   if (gpMCParamDlg->m_currentParam==2)
      {
      gpMCParamDlg->m_isMean = m_mean;
      gpMCParamDlg->m_isStd = m_std;
      }
   if (gpMCParamDlg->m_currentParam==3)
      {
      gpMCParamDlg->m_nMean = m_mean;
      gpMCParamDlg->m_nStd = m_std;
      }
   if (gpMCParamDlg->m_currentParam==4)
      {
      gpMCParamDlg->m_ksMean = m_mean;
      gpMCParamDlg->m_ksStd = m_std;
      } 	
   if (gpMCParamDlg->m_currentParam==5)
      {
      gpMCParamDlg->m_kDepthMean = m_mean;
      gpMCParamDlg->m_kDepthStd = m_std;
      }
   if (gpMCParamDlg->m_currentParam==6)
      {
      gpMCParamDlg->m_phiMean = m_mean;
      gpMCParamDlg->m_phiStd = m_std;
      } 	   
	CDialog::OnOK();
}

BOOL LogNormParam::OnInitDialog() 
{

	
   if (gpMCParamDlg->m_currentParam==1)
      {
      m_mean = gpMCParamDlg->m_nMean ;
      m_std = gpMCParamDlg->m_nStd ;
      }
   if (gpMCParamDlg->m_currentParam==2)
      {
      m_mean = gpMCParamDlg->m_isMean ;
      m_std = gpMCParamDlg->m_isStd ;
      }
   if (gpMCParamDlg->m_currentParam==3)
      {
      m_mean = gpMCParamDlg->m_nMean ;
      m_std = gpMCParamDlg->m_nStd ;
      }
   if (gpMCParamDlg->m_currentParam==4)
      {
      m_mean = gpMCParamDlg->m_ksMean ;
      m_std = gpMCParamDlg->m_ksStd ;
      }  
   if (gpMCParamDlg->m_currentParam==5)
      {
      m_mean = gpMCParamDlg->m_kDepthMean ;
      m_std = gpMCParamDlg->m_kDepthStd ;
      }
   if (gpMCParamDlg->m_currentParam==6)
      {
      m_mean = gpMCParamDlg->m_phiMean ;
      m_std = gpMCParamDlg->m_phiStd ;
      }    
	CDialog::OnInitDialog();	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
