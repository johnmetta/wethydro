// wet_h.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#pragma hdrstop
#include "wet_h.h"
#include "map.h"
#include "maplayer.h"
#include "MainFrm.h"
#include "wet_hDoc.h"
#include "wet_hView.h"
#include "MapWnd.h"
#include "ResultsWnd.h"
#include "ResultPlotWnd.h"
#include "decisionmanager.h"
#include "WizUserInteraction.h"
#include "TreeView.h"
#include "ReportWriter.h"
#include "etParamHarDlg.h"
#include "etParamPMDlg.h"
#include "maplistview.h"
#include "wet_hydro.h"
#include "optimizeDlg.h"
#include "MCParamDlg.h"
#include "ModelDefinition.h"
#include "ModelEvaluation.h"
#include "Extensions.h"
#include  "Restoration.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <randgen\randunif.hpp>


class MsgWnd;

/////////////////////////////////////////////////////////////////////////////
// CWet_hApp

BEGIN_MESSAGE_MAP(CWet_hApp, CWinApp)
	//{{AFX_MSG_MAP(CWet_hApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWet_hApp construction

CWet_hApp::CWet_hApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CWet_hApp object

CWet_hApp theApp;

CMainFrame   *gpMain = NULL;
CWet_hDoc  *gpDoc  = NULL;
CWet_hView *gpView   = NULL;
MapWnd       *gpMapWnd = NULL;
MapLayer     *gpCellLayer = NULL;
MapLayer     *gpStreamLayer = NULL;
ResultsWnd   *gpResultsWnd = NULL;
WizUserInteraction *gpWizUserInteraction = NULL;
ModelDefinition    *gpModelDefinition = NULL;
ModelEvaluation    *gpModelEvaluation = NULL;
ppParameterValues  *gpParameterValues = NULL;
Extensions  *gpExtensions = NULL;
Restoration *gpRestoration = NULL;

ReportWriter  *gpReportWriter = NULL;

WET_Hydro  *gpHydroModel   = NULL;

ReachTree  *gpReachTree = NULL;
ResultPlotWnd *gpResultPlotWnd = NULL;
optimizeDlg *gpOptimize = NULL;
OptimizeDlg3DPlot *gpOptimize3DPlot = NULL;
MessagesWnd   *gpMessagesWnd = NULL;
etParamHarDlg *gpEtHargreaves = NULL;
etParamPMDlg *gpEtPenmanMontieth = NULL;
MapListView *gpMapListView = NULL;

MCParamDlg *gpMCParamDlg = NULL;


/////////////////////////////////////////////////////////////////////////////
// CWet_hApp initialization

BOOL CWet_hApp::InitInstance()
{
	//afxMemDF |= checkAlwaysMemDF;

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CWet_hDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CWet_hView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CWet_hApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();

   FILE *fp = fopen( "randtest.csv", "wt" );
   RandUniform rn( NULL );

   for ( int i=0; i < 20000; i++ )
   {
      double v = double(rand()) / RAND_MAX; //rn.RandValue();
      //double v1 = rn.RandValue( 100, 200 );
      //double v2 = rn.RandValue( 100, 200 );
      //double v3 = rn.RandValue( 100, 200 );
      fprintf( fp, "%lf \n", v ); //, %lf, %lf, %lf,\n", v, v1, v2, v3 );
   }
   fclose( fp );

}

/////////////////////////////////////////////////////////////////////////////
// CWet_hApp message handlers


void CWet_hApp::OnFileOpen() 
{

	
   CFileDialog dlg( 1, "ini", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
      "INI files|*.ini|All files|*.*||");

   if ( dlg.DoModal() == IDOK )
      OpenDocumentFile( dlg.GetPathName() ); 
	
}
