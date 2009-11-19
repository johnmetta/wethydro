// SaveResults.cpp : implementation file
//

#include "stdafx.h"
#pragma hdrstop
#include "wet_h.h"
#include "SaveResults.h"
#include "ResultPlotWnd.h"
#include "WET_Hydro.h"

extern ResultPlotWnd *gpResultPlotWnd;
extern WET_Hydro *gpHydroModel;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SaveResults dialog


SaveResults::SaveResults(CWnd* pParent /*=NULL*/)
	: CDialog(SaveResults::IDD, pParent)
{
	//{{AFX_DATA_INIT(SaveResults)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void SaveResults::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SaveResults)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(SaveResults, CDialog)
	//{{AFX_MSG_MAP(SaveResults)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SaveResults message handlers




void SaveResults::OnOK() 

{
	CDialog OnOK;
	gpHydroModel->SaveData();
}
