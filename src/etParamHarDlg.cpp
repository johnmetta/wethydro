// etParamHarDlg.cpp : implementation file
//

#include "stdafx.h"
#pragma hdrstop
#include "wet_h.h"
#include "etParamHarDlg.h"

#include "wizuserinteraction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern WizUserInteraction *gpWizUserInteraction;

/////////////////////////////////////////////////////////////////////////////
// etParamHarDlg dialog


etParamHarDlg::etParamHarDlg(CWnd* pParent /*=NULL*/)
	: CDialog(etParamHarDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(etParamHarDlg)
	m_latitude = gpWizUserInteraction->m_latitude;
	//}}AFX_DATA_INIT
}


void etParamHarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(etParamHarDlg)
	DDX_Text(pDX, IDC_EDIT1, m_latitude);

	//}}AFX_DATA_MAP

}


BEGIN_MESSAGE_MAP(etParamHarDlg, CDialog)
	//{{AFX_MSG_MAP(etParamHarDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// etParamHarDlg message handlers

void etParamHarDlg::OnOK() 
{

	// TODO: Add extra validation here
 
	
	CDialog::OnOK();
  gpWizUserInteraction->m_latitude = m_latitude;

}
