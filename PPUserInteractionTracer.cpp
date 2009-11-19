// PPUserInteractionTracer.cpp : implementation file
//

#include "stdafx.h"
#pragma hdrstop
#include "wet_h.h"
#include "PPUserInteractionTracer.h"
#include "wet_Hydro.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern WET_Hydro *gpHydroModel;
/////////////////////////////////////////////////////////////////////////////
// PPUserInteractionTracer property page

IMPLEMENT_DYNCREATE(PPUserInteractionTracer, CPropertyPage)

PPUserInteractionTracer::PPUserInteractionTracer() : CPropertyPage(PPUserInteractionTracer::IDD)
{
	//{{AFX_DATA_INIT(PPUserInteractionTracer)
	m_editTracerMgL = gpHydroModel->m_editTracerMgL;
	m_checkTracerModel = gpHydroModel->m_checkTracerModel;
	m_radioTracerInput = gpHydroModel->m_radioTracerInput;
	m_initTracerConc = gpHydroModel->m_initTracerConc;
	//}}AFX_DATA_INIT
}

PPUserInteractionTracer::~PPUserInteractionTracer()
{
}

void PPUserInteractionTracer::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PPUserInteractionTracer)
	DDX_Text(pDX, IDC_EDITCLMGL, m_editTracerMgL);
	DDX_Check(pDX, IDC_CHECKCL, m_checkTracerModel);
	DDX_Radio(pDX, IDC_RADIOCLTIME, m_radioTracerInput);
	DDX_Text(pDX, IDC_EDITINITSOILCONC, m_initTracerConc);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(PPUserInteractionTracer, CPropertyPage)
	//{{AFX_MSG_MAP(PPUserInteractionTracer)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PPUserInteractionTracer message handlers
