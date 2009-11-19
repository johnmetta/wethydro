// UniformParam.cpp : implementation file
//

#include "stdafx.h"
#include "wet_h.h"
#include "UniformParam.h"
#include "MCParamDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern MCParamDlg *gpMCParamDlg;

/////////////////////////////////////////////////////////////////////////////
// UniformParam dialog


UniformParam::UniformParam(CWnd* pParent /*=NULL*/)
	: CDialog(UniformParam::IDD, pParent)
{
	//{{AFX_DATA_INIT(UniformParam)
	m_max = 0.0f;
	m_min = 0.0f;
	//}}AFX_DATA_INIT
}


void UniformParam::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(UniformParam)
	DDX_Text(pDX, IDC_NMAX, m_max);
	DDX_Text(pDX, IDC_NMIN, m_min);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(UniformParam, CDialog)
	//{{AFX_MSG_MAP(UniformParam)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// UniformParam message handlers

BOOL UniformParam::OnInitDialog() 
{

	
   if (gpMCParamDlg->m_currentParam==1)
      {
      m_min = gpMCParamDlg->m_mMin ;
      m_max = gpMCParamDlg->m_mMax ;
      }
   if (gpMCParamDlg->m_currentParam==2)
      {
      m_min = gpMCParamDlg->m_isMin ;
      m_max = gpMCParamDlg->m_isMax ;
      }
   if (gpMCParamDlg->m_currentParam==3)
      {
      m_min = gpMCParamDlg->m_nMin ;
      m_max = gpMCParamDlg->m_nMax ;
      }
   if (gpMCParamDlg->m_currentParam==4)
      {
      m_min = gpMCParamDlg->m_ksMin ;
      m_max = gpMCParamDlg->m_ksMax ;
      }   
     if (gpMCParamDlg->m_currentParam==5)
      {
      m_min = gpMCParamDlg->m_kDepthMin ;
      m_max = gpMCParamDlg->m_kDepthMax ;
      } 
     if (gpMCParamDlg->m_currentParam==6)
      {
      m_min = gpMCParamDlg->m_phiMin ;
      m_max = gpMCParamDlg->m_phiMax ;
      } 
	CDialog::OnInitDialog();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void UniformParam::OnOK() 
{
   UpdateData(TRUE);
   if (gpMCParamDlg->m_currentParam==1)
      {
      gpMCParamDlg->m_mMin  = m_min;
      gpMCParamDlg->m_mMax = m_max;
      }
   if (gpMCParamDlg->m_currentParam==2)
      {
      gpMCParamDlg->m_isMin = m_min;
      gpMCParamDlg->m_isMax = m_max;
      }
   if (gpMCParamDlg->m_currentParam==3)
      {
      gpMCParamDlg->m_nMin = m_min;
      gpMCParamDlg->m_nMax = m_max;
      }
   if (gpMCParamDlg->m_currentParam==4)
      {
      gpMCParamDlg->m_ksMin = m_min;
      gpMCParamDlg->m_ksMax = m_max;
      } 
   if (gpMCParamDlg->m_currentParam==5)
      {
      gpMCParamDlg->m_kDepthMin = m_min;
      gpMCParamDlg->m_kDepthMax = m_max;
      }
   if (gpMCParamDlg->m_currentParam==6)
      {
      gpMCParamDlg->m_phiMin = m_min;
      gpMCParamDlg->m_phiMax = m_max;
      } 
	
	CDialog::OnOK();
}
