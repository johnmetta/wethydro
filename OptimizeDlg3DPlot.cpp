// OptimizeDlg3DPlot.cpp : implementation file
//

#include "stdafx.h"
#include "wet_h.h"
#include "OptimizeDlg3DPlot.h"
#include "optimizeDlg.h"

#include "DerivedDouble1.h"
#include "Axis1.h"
#include "AxisCollection1.h"
#include "ChartArea1.h"
#include "Chart3DData.h"
#include "ChartGroup1.h"
#include "ChartGroupCollection1.h"
//#include "SymbolStyle.h"
//#include "ChartStyle.h"
//#include "ChartStyleCollection.h"
#include "Label1.h"
#include "ChartLabel1.h"
#include "ChartLabelCollection1.h"
#include "LabelCollection1.h"
#include "Border1.h"
#include "Interior1.h"
#include "LineStyle1.h"
#include "legend1.h"

#include "Font.h"
#include "title1.h"
#include "3dconst.h"

#include "Elevation.h"
#include "bar1.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern optimizeDlg *gpOptimize;


/////////////////////////////////////////////////////////////////////////////
// OptimizeDlg3DPlot dialog


OptimizeDlg3DPlot::OptimizeDlg3DPlot(CWnd* pParent /*=NULL*/)
	: CDialog(OptimizeDlg3DPlot::IDD, pParent)
{
	//{{AFX_DATA_INIT(OptimizeDlg3DPlot)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void OptimizeDlg3DPlot::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(OptimizeDlg3DPlot)
	DDX_Control(pDX, IDC_3DCHART, m_3dchart);
	UpdatePlot();
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(OptimizeDlg3DPlot, CDialog)
	//{{AFX_MSG_MAP(OptimizeDlg3DPlot)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// OptimizeDlg3DPlot message handlers
void OptimizeDlg3DPlot::UpdatePlot()
	{
	


	}