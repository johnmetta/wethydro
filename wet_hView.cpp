// wet_hView.cpp : implementation of the CWet_hView class
//

#include "stdafx.h"
#pragma hdrstop
#include "wet_h.h"
#include "math.h"

#include "wet_hDoc.h"
#include "wet_hView.h"
#include "wet_hydro.h"
#include "MapWnd.h"
#include <map.h>
#include <fdataObj.h>
#include <DataTable.h>
#include "ModelDefinition.h"
#include "ModelEvaluation.h"

#include "WizUserInteraction.h"
#include "ReportWriter.h"
#include "MapListView.h"
//#include "ClassifyLayerDlg.h"
#include "MCParamDlg.h"
#include <FDATAOBJ.h>

#include "Extensions.h"
#include "Restoration.h"

#include "mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <randgen\randunif.hpp>
#include <randgen\randln.hpp>
#include <randgen\randnorm.hpp>
#include ".\wet_hview.h"
extern CMainFrame       *gpMain;
extern CWet_hView         *gpView;
extern MapLayer           *gpCellLayer;
extern WET_Hydro          *gpHydroModel;
extern MapWnd             *gpMapWnd;
extern WizUserInteraction *gpWizUserInteraction;
extern ReportWriter       *gpReportWriter;
extern MapListView        *gpMapListView;
extern MCParamDlg         *gpMCParamDlg;
extern ModelDefinition    *gpModelDefinition;
extern ModelEvaluation    *gpModelEvaluation;
extern Extensions         *gpExtensions;
extern Restoration        *gpRestoration;

bool UpdateMapProc( WET_Hydro*pHydro, WH_NOTIFY_TYPE type, float, float, long a1, long extra );
/////////////////////////////////////////////////////////////////////////////
// CWet_hView

IMPLEMENT_DYNCREATE(CWet_hView, CView)

BEGIN_MESSAGE_MAP(CWet_hView, CView)
	//{{AFX_MSG_MAP(CWet_hView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_ZOOMIN, OnZoomin)
	ON_COMMAND(ID_ZOOMOUT, OnZoomout)
	ON_COMMAND(ID_SELECT, OnSelect)
	ON_COMMAND(ID_QUERYDISTANCE, OnQuerydistance)
	ON_COMMAND(ID_ZOOMFULL, OnZoomfull)
	ON_COMMAND(ID_DBTABLE, OnDbtable)
	ON_COMMAND(ID_PAN, OnPan)
	ON_COMMAND(ID_ANALYSIS_RUNMODEL, OnAnalysisRunmodel)
	ON_COMMAND(ID_MODELANALYSIS_USERINTERACTION, OnModelanalysisUserinteraction)
	ON_COMMAND(ID_MODELANALYSIS_GENERATEREPORT, OnModelanalysisGeneratereport)
	ON_COMMAND(ID_STATISTICS, OnStatistics)

	ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
	ON_COMMAND(ID_APP_STOP, OnAppStop)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
   ON_COMMAND(ID_MODELANALYSIS_DEFINEHYDROLOGYMODEL, OnModelanalysisDefinehydrologymodel)
   ON_COMMAND(ID_MODELANALYSIS_MODELEVALUATION, OnModelanalysisModelevaluation)
   ON_COMMAND(ID_MODELANALYSIS_PESTICIDEMODEL, OnModelanalysisPesticidemodel)
   ON_COMMAND(ID_MODELANALYSIS_RESTORATIONSENSITIVITY, OnModelanalysisRestorationsensitivity)
   ON_COMMAND(ID_TOPOGRAPHICCALCULATION_MEANWATERSHEDELEVATION, OnTopographiccalculationMeanwatershedelevation)
   ON_COMMAND(ID_TOPOGRAPHICCALCULATION_SUBBASINFPDISTRIBUTION, OnTopographiccalculationSubbasinfpdistribution)
   ON_COMMAND(ID_TOPOGRAPHICCALCULATION_SUBBASINDISTANCETODSOUTLETDISTRIBUTION, OnTopographiccalculationSubbasindistancetodsoutletdistribution)
   ON_COMMAND(ID_TOPOGRAPHICCALCULATION_DISTANCEFROMPOLYTOSTREAM, OnTopographiccalculationDistancefrompolytostream)
   ON_COMMAND(ID_TOPOGRAPHICCALCULATION_CALCULATEUPSTREAMAREA, OnTopographiccalculationCalculateupstreamarea)
   ON_COMMAND(ID_ADDITIONALSTUFF_LOADUARADAR, OnAdditionalstuffLoaduaradar)
   ON_COMMAND(ID_TOPOGRAPHICCALCULATION_SIDEOFSTREAM, OnTopographiccalculationSideofstream)
   ON_COMMAND(ID_MODELANALYSIS_SAVEMODELSTATE, OnModelanalysisSavemodelstate)
   ON_COMMAND(ID_TOPOGRAPHICCALCULATION_DISTTOSTREAM, OnTopographiccalculationDisttostream)
   ON_COMMAND(ID_Menu, OnMenu)
   ON_COMMAND(ID_MODELANALYSIS_RUNHILLSLOPEMODEL, OnModelanalysisRunhillslopemodel)
   ON_COMMAND(ID_TOPOGRAPHICCALCULATION_WILLAMETTEQMOVIE, OnTopographiccalculationWillametteqmovie)
   ON_COMMAND(ID_TOPOGRAPHICCALCULATION_DISGRADIENTANDQUOTIENTFORGRID, OnTopographiccalculationDisgradientandquotientforgrid)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWet_hView construction/destruction

CWet_hView::CWet_hView()
: m_pDBTable( NULL ),
  m_pMonteCarloDBTable( NULL ),
  m_mode( MM_NORMAL ),
  m_randUnif1(),
  m_randUnif2(),
  m_randUnif3(),
  m_randUnif4(),
  m_randUnif5(),
  m_randUnif6(),
  m_pRandLN1(NULL),
  m_pRandLN2(NULL),
  m_pRandLN3(NULL),
  m_pRandN1(NULL),
  m_pRandN2(NULL),
  m_pRandN3(NULL),
  m_checkIS(TRUE), 
  m_checkKSat(TRUE), 
  m_checkM(TRUE), 
  m_checkN(TRUE), 
  m_editNumIter(100), 
  m_editRandomSeed(2), 
  m_isDist(0), 
  m_isMax(0), 
  m_isMin(0), 
  m_kSatDist(1),  
  m_kSatMax(10.0f), 
  m_kSatMin(0.000001f), 
  m_mDist(1), 
  m_mMax(50.0f), 
  m_mMin(0.01f), 
  m_nDist(1), 
  m_nMax(1.0f), 
  m_nMin(0.01f), 
  m_checkUpslope(TRUE), 
  m_upslopeDist(1.0f), 
  m_upslopeMax(5), 
  m_upslopeMin(1)   
{ 
	// TODO: add construction code here
   gpView = this;  // initially
   m_pRandN1 = new RandNormal(0,0,0);
   m_pRandN2 = new RandNormal(0,0,0);
   m_pRandN3 = new RandNormal(0,0,0);
  
   m_pRandLN1 = new RandLogNormal(0,0,0);
   m_pRandLN2 = new RandLogNormal(0,0,0);
   m_pRandLN3 = new RandLogNormal(0,0,0);
   
   }

CWet_hView::~CWet_hView()
   {
   if ( m_pDBTable != NULL )
      delete m_pDBTable;

   if ( m_pRandLN1 != NULL) 
      delete m_pRandLN1;
   if ( m_pRandLN2 != NULL )
      delete m_pRandLN2;
   if ( m_pRandLN3 != NULL )
      delete m_pRandLN3;

   if ( m_pRandN1 != NULL )
      delete m_pRandN1;
   if ( m_pRandN2 != NULL )
      delete m_pRandN2;
   if ( m_pRandN3 != NULL )
      delete m_pRandN3;

   }

BOOL CWet_hView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CWet_hView drawing

void CWet_hView::OnDraw(CDC* pDC)
{
	CWet_hDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CWet_hView printing

BOOL CWet_hView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CWet_hView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CWet_hView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CWet_hView diagnostics

#ifdef _DEBUG
void CWet_hView::AssertValid() const
{
	CView::AssertValid();
}

void CWet_hView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CWet_hDoc* CWet_hView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWet_hDoc)));
	return (CWet_hDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWet_hView message handlers
int CWet_hView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

   RECT rect;
   GetClientRect( &rect );

	BOOL ok = m_outputTab.Create( WS_CHILD | WS_VISIBLE, rect, this, 100 );
   ASSERT( ok );
	return 0;
}

void CWet_hView::OnSize(UINT nType, int cx, int cy) 
   {
	CView::OnSize(nType, cx, cy);
	
   //size the control
   m_outputTab.MoveWindow( 0, 0, cx, cy, TRUE );
	
   }


BOOL CWet_hView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
   {
   //-- handle the toolbar control beiing pushed --//
   NMHDR *pNotify = (NMHDR*) lParam;

   switch( pNotify->idFrom )
      {
      case 100:   // m_tab control
         {
         if ( pNotify->code == TCN_SELCHANGE )
            {
            // hide everything

            m_outputTab.m_mapTab.ShowWindow( SW_HIDE );
            m_outputTab.m_resultsPlotTab.ShowWindow( SW_HIDE );
            m_outputTab.m_resultsTab.ShowWindow( SW_HIDE );
            m_outputTab.m_messagesTab.ShowWindow( SW_HIDE );

            int index = TabCtrl_GetCurSel( pNotify->hwndFrom );

            switch ( index )
               {
               case 0:  m_outputTab.m_mapTab.ShowWindow( SW_SHOW );  break;
               case 1:  m_outputTab.m_resultsPlotTab.ShowWindow( SW_SHOW );  break;
               case 2:  m_outputTab.m_resultsTab.ShowWindow( SW_SHOW );  break;
               case 3:  m_outputTab.m_messagesTab.ShowWindow( SW_SHOW );  break;


               }
            }  // end of:  code == TCN_SELCHANGE
         }
      break;
      }

	return CView::OnNotify(wParam, lParam, pResult);
   }

void CWet_hView::OnEditCopy() 
   {
   if ( m_outputTab.m_mapTab.IsWindowVisible() )
      m_outputTab.m_mapTab.OnEditCopy();
   }

void CWet_hView::OnZoomin() 
   {
   if ( m_outputTab.m_mapTab.m_pMap != NULL )
      m_outputTab.m_mapTab.m_pMap->StartZoomIn();

   }

void CWet_hView::OnZoomout() 
   {
   if ( m_outputTab.m_mapTab.m_pMap != NULL )
      m_outputTab.m_mapTab.m_pMap->StartZoomOut();

   }


void CWet_hView::OnZoomfull() 
   {
	if ( m_outputTab.m_mapTab.m_pMap != NULL )
      m_outputTab.m_mapTab.m_pMap->ZoomFull();
   if ( IsWindow( gpMapWnd->m_hWnd ) && gpMapWnd->m_pMap != NULL )
      gpMapWnd->m_pMap->ZoomFull();
   }


void CWet_hView::OnSelect() 
   {
   if ( m_outputTab.m_mapTab.m_pMap != NULL )
      m_outputTab.m_mapTab.m_pMap->StartSelect();
   }

void CWet_hView::OnQuerydistance() 
   {
	if ( m_outputTab.m_mapTab.m_pMap != NULL )
      m_outputTab.m_mapTab.m_pMap->StartQueryDistance();   
   }

void CWet_hView::OnDbtable() 
   {
   if ( gpCellLayer == NULL )
      return;

   if ( m_pDBTable == NULL )
      m_pDBTable = new DataTable( gpCellLayer->m_pData );

   if ( IsWindow( m_pDBTable->m_hWnd ) == FALSE )
      {
      RECT rect;
      rect.left = 50;
      rect.top = 100;
      rect.right = 1400;
      rect.bottom = 1400;
      m_pDBTable->Create( gpCellLayer->m_database, rect, m_outputTab.m_mapTab.m_pMap, 200 ) ;
      }	
   }

void CWet_hView::OnPan() 
   {
   if ( m_outputTab.m_mapTab.m_pMap != NULL )
      m_outputTab.m_mapTab.m_pMap->StartPan();
   	
   }
void CWet_hView::OnAnalysisRunmodel() 
	{
   if (gpHydroModel==NULL)
      {
      CString msg="Load *.ini file first";
      ErrorMsg(msg);
      return;
      }

   if (!gpHydroModel->m_checkMonteCarlo==WH_MONTE_CARLO )
      {
      //  copy parameters from the dialog into the hydrologic model..
    //  gpModelEvaluation->SampleParameters();
      gpModelEvaluation->CopyParamsToHydrologicModel(gpHydroModel->m_runNumber);
      gpHydroModel->UpdateParameterVector();

      // set up updateable scatterwnd with precip data  (allocated in call, must delete here)
      FDataObj precipData = FDataObj( 2, 0 );
      precipData.SetLabel( 0, "" );
      precipData.SetLabel( 1, "Precip" );
      FDataObj dischargeData = FDataObj(2, 0 );
      dischargeData.SetLabel( 0, "Time (Days)" );
      dischargeData.SetLabel( 1, "Meas" );
      dischargeData.SetLabel( 2, "Model" );
     
      float *data = new float[ 2 ];
      //int num = gpHydroModel->m_pMeasuredDischargeData->GetColCount();
      float *data1 = new float[ 3 ];

      for ( double time=gpHydroModel->m_startTime; time <= gpHydroModel->m_stopTime; time += 1.0f/24.0f )
         {
			 data[ 0 ] = (float)time;
         data[ 1 ] = gpHydroModel->m_pClimateData->IGet( (float)time, 1 );
         precipData.AppendRow( data );
		   data[0]=(float)time;
		   data[1]=0.0f;
		   precipData.AppendRow( data );
		   data[0]=(float)time+1.0f/24.0f;
		   data[1]=0.0f;
		   precipData.AppendRow( data );

         data1[0]=(float)time-gpHydroModel->m_startTime;
        // for (int kk=1;kk<gpHydroModel->m_pMeasuredDischargeData->GetColCount()-1;kk++);
            data1[1]=gpHydroModel->m_pMeasuredDischargeData->IGet((float)time,1);
         data1[2]=0.0f;
         dischargeData.AppendRow( data1 );
         }
      delete [] data;
      delete [] data1;

	  
      WH_NOTIFYPROC calcWatershed = gpHydroModel->InstallNotifyHandler( UpdateMapProc );
      if (gpHydroModel->m_runNumber==0)
		   gpHydroModel->CalcWatershedInfo();

 //     if (gpHydroModel->m_checkGridMet)
 //        gpHydroModel->LoadDistributedAirTemp();
      gpHydroModel->InstallNotifyHandler( calcWatershed );
//	   gpHydroModel->CalculateTotalArea();
      gpMapWnd->m_pScatterPrecip = gpMapWnd->AllocateScatter( &precipData, 0 );
      if (gpHydroModel->m_checkPesticide)
	      gpMapWnd->m_pScatterPrecip->PlotVerticalLine(gpHydroModel->m_startTime+gpHydroModel->m_spinUp);
      gpMapWnd->m_pScatterDischarge = gpMapWnd->AllocateScatter( &dischargeData, 1 );
      if (gpHydroModel->m_checkPesticide)
         gpMapWnd->m_pScatterDischarge->PlotVerticalLine(gpHydroModel->m_startTime+gpHydroModel->m_spinUp);
      gpHydroModel->UpdateParameterVector();

  //   if (gpHydroModel->m_checkDistributeInitCond) //if this is not the 1st run, we need to restore initial conditions.
   //     gpHydroModel->RestoreState(2);

      WH_NOTIFYPROC p = gpHydroModel->InstallNotifyHandler( UpdateMapProc );
	   gpHydroModel->Run(); 
   //      if (gpHydroModel->m_runNumber==1) //if we just finished the first run, save the current state as an initial condition
  //          gpHydroModel->SaveState(2);
      gpHydroModel->InstallNotifyHandler( p );

      delete gpMapWnd->m_pScatterPrecip;
      delete gpMapWnd->m_pScatterDischarge;
	  gpHydroModel->PlotOutput();
      gpHydroModel->ResetVariableValues();

      }

   else // This is a Monte Carlo Simulation...
      {
      
      gpHydroModel->m_pMonteCarloResults = new FDataObj( 20, 0 );  
      ASSERT( gpHydroModel->m_pMonteCarloResults != NULL );
      gpHydroModel->m_pMonteCarloQTimeSeries = new FDataObj(2,0); // 1 for time and 1 for measured, which we will store
      gpHydroModel->m_pMonteCarloTracerTimeSeries = new FDataObj(1,0);
      // add labels   

      gpHydroModel->m_pMonteCarloResults->SetLabel( 0, "RunNumber" );
      gpHydroModel->m_pMonteCarloResults->SetLabel( 1, "rmse1" );
      gpHydroModel->m_pMonteCarloResults->SetLabel( 2, "ns1" );
      gpHydroModel->m_pMonteCarloResults->SetLabel( 3, "1stMoment" );
      gpHydroModel->m_pMonteCarloResults->SetLabel( 4, "r2" );
      gpHydroModel->m_pMonteCarloResults->SetLabel( 5, "PercentNewWater" );
	  gpHydroModel->m_pMonteCarloResults->SetLabel( 6, "MRT" );
      gpHydroModel->m_pMonteCarloResults->SetLabel( 7, "meanUpMRT" );
      gpHydroModel->m_pMonteCarloResults->SetLabel( 8, "minUpMRT" );
	  gpHydroModel->m_pMonteCarloResults->SetLabel( 9, "maxUpMRT" );



      gpHydroModel->m_pMonteCarloResults->SetLabel( 10, "m" );
      gpHydroModel->m_pMonteCarloResults->SetLabel( 11, "initSaturation" );
      gpHydroModel->m_pMonteCarloResults->SetLabel( 12, "kSat" );
      gpHydroModel->m_pMonteCarloResults->SetLabel( 13, "SoilDepth" );
      gpHydroModel->m_pMonteCarloResults->SetLabel( 14, "phi" );
      gpHydroModel->m_pMonteCarloResults->SetLabel( 15, "PoreSizeDist" );
      gpHydroModel->m_pMonteCarloResults->SetLabel( 16, "PLE" );
      gpHydroModel->m_pMonteCarloResults->SetLabel( 17,"ResidualTheta" );
      gpHydroModel->m_pMonteCarloResults->SetLabel( 18,"thetaTrace" );
      gpHydroModel->m_pMonteCarloResults->SetLabel( 19,"kDepth" );

	 
  //    gpHydroModel->m_reachDataArray.RemoveAll();
      // Here was assume that the measured values represent the last reach in the tree - and that the root node has
      //  only a left hand upstream pointer.  This won't work if the tree ends in a junction, but if that were the case
      //  the data probably were not measured there anyway and more work is likely necessary on the GIS side...
  //    gpHydroModel->m_reachDataArray.Add(gpHydroModel->m_reachTree.m_pRoot->m_pLeft->m_reachInfo.index);
   //   gpModelEvaluation->SampleParameters();
      gpModelEvaluation->CopyParamsToHydrologicModel(0);//allocate the model parameters
      gpHydroModel->UpdateParameterVector();
      gpHydroModel->CalcWatershedInfo();
      gpHydroModel->CalculateTotalArea();
      
      for (int i=0; i<gpHydroModel->m_numIterations; i++)
         {
         WH_NOTIFYPROC p = gpHydroModel->InstallNotifyHandler( UpdateMapProc );

	      gpHydroModel->Run( ); 

         gpHydroModel->InstallNotifyHandler( p );
		   if (fmod((double)i,100.0) < 0.01f)
            {
			   gpHydroModel->m_pMonteCarloResults->WriteAscii("c:\\temp\\monte.csv");
            gpHydroModel->m_pMonteCarloQTimeSeries->WriteAscii("c:\\temp\\monteQ.csv");
            gpHydroModel->m_pMonteCarloTracerTimeSeries->WriteAscii("c:\\temp\\monteT.csv");
            }

         gpModelEvaluation->SampleParameters();
         gpModelEvaluation->CopyParamsToHydrologicModel(1);//replace model parameters (in the model parameter database)
         gpHydroModel->UpdateParameterVector();//copy params from model database into state variables
         gpHydroModel->ResetVariableValues();
         }
      gpHydroModel->m_pMonteCarloResults->WriteAscii("c:\\temp\\monte.csv");
      gpHydroModel->m_pMonteCarloQTimeSeries->WriteAscii("c:\\temp\\monteQ.csv");
      gpHydroModel->m_pMonteCarloTracerTimeSeries->WriteAscii("c:\\temp\\monteT.csv");

      }

	}
	
//}

void CWet_hView::OnModelanalysisUserinteraction() 
	{
   if (gpHydroModel==NULL)
      {
      CString msg="Load *.ini file first";
      ErrorMsg(msg);
      return;
      }

   if (gpWizUserInteraction->m_hWnd != NULL)
		gpWizUserInteraction->ShowWindow( SW_SHOW );	
	else
		{
		gpWizUserInteraction = new WizUserInteraction( "Model Interaction" );
		gpWizUserInteraction->Create( gpMapWnd->m_pMap, WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME | WS_MAXIMIZEBOX );
		gpWizUserInteraction->ShowWindow( SW_SHOW );	
		}
	}

void CWet_hView::OnModelanalysisGeneratereport() 
   {
   ReportWriter r( "template.htm" );
   r.GenerateReport();

   m_outputTab.m_messagesTab.Navigate( r.GetReportPath() );
  


	CWaitCursor c;
   }

void CWet_hView::OnStatistics() 
{
   if (gpHydroModel==NULL)
      {
      CString msg="Load *.ini file first";
      ErrorMsg(msg);
      return;
      }

   if ( gpHydroModel->m_pMonteCarloResults == NULL )
      return;

   if (m_pMonteCarloDBTable != NULL)
      delete m_pMonteCarloDBTable;

   DataTable *m_pMonteCarloDBTable = new DataTable( gpHydroModel->m_pMonteCarloResults );


   RECT rect;
	CRect rect1;
	GetClientRect(&rect1);
//	CSize sz(0, rect.Height() );
   rect.left = 0;
   rect.top = 0;
   rect.right = rect1.right-20;
   rect.bottom = 500;
   m_pMonteCarloDBTable->Create( "Monte Carlo Results", rect, m_outputTab.m_resultsPlotTab.m_pPlotWnd, 200 ) ;	
}

float CWet_hView::GenerateRandom(float minValue, float maxValue)
   {
    
  // srand((unsigned)time(NULL));
  // float value = ((double)rand()/(double)RAND_MAX)*maxValue;
     
//   float val = (float) m_randUniform.RandValue( minValue, maxValue);   
  return -1;
   }



void CWet_hView::OnFileClose() 
{
   
   CString msg ;
	msg.Format("Save Model Before Closing");
   int retCode = AfxGetMainWnd()->MessageBox( msg, "Save?", MB_YESNOCANCEL );
   if( retCode == IDYES )
      {
      CFileDialog dlg( TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
            "ModelFiles|*.wet|All files|*.*||");

      if ( dlg.DoModal() == IDOK )
         {
         CString filename( dlg.GetPathName() );
		   gpHydroModel->SaveParameters(filename);	
		   }
      }
   if (retCode != IDCANCEL)
      {
      if (gpHydroModel != NULL)
         delete gpHydroModel;
      gpHydroModel   = NULL;
      /*
      Map *pMap = gpMapWnd->m_pMap;
	   int mapCount = pMap->GetLayerCount();

      for (int i=0; i<mapCount;i++)      
         pMap->RemoveLayer(i,false);
      gpMapWnd->RefreshList();
      gpMapWnd->m_pMap->m_name = " ";
      pMap->RedrawWindow();
      */
      }
   

   }

void CWet_hView::OnAppStop() 
   {
   if (gpHydroModel==NULL)
      {
      CString msg="Load *.ini file first";
      ErrorMsg(msg);
      return;
      }
   if (gpHydroModel != NULL)
      {
      gpHydroModel->m_stopTimeHolder = gpHydroModel->m_stopTime;
      gpHydroModel->m_stopTime = gpHydroModel->m_startTime;	
      gpHydroModel->m_numIterations = 1;
      }
   }


void CWet_hView::GenerateRandomValues()
   {/*
   for (int i=0;i<6;i++) //
      {
      RANDOM_NUMBER *pRandom = gpView->m_randomNumberArray[i];
      int x=i+1;
      if (pRandom->distNum==0)
         {
         if (i==0) // m
            a[x] = (float)m_randUnif1.RandValue(pRandom->minValue,pRandom->maxValue); 
         if (i==1) // is
            a[x] = (float)m_randUnif2.RandValue(pRandom->minValue,pRandom->maxValue); 
         if (i==2) // n
            a[x] = (float)m_randUnif3.RandValue(pRandom->minValue,pRandom->maxValue); 
         if (i==3)  // ks
            a[x] = (float)m_randUnif4.RandValue(pRandom->minValue,pRandom->maxValue); 
         if (i==4) //kdepth
            a[x] = (float)m_randUnif5.RandValue(pRandom->minValue,pRandom->maxValue); 
         if (i==5) // 'porosity'
            a[x] = (float)m_randUnif6.RandValue(pRandom->minValue,pRandom->maxValue); 


         }
      if (pRandom->distNum==1)
         {
         m_pRandN1->SetMean( pRandom->mean );
         m_pRandN1->SetStdDev( pRandom->std );
         a[x] = (float)m_pRandN1->RandValue();
         }
      if (pRandom->distNum==2)
         {

         a[x] = (float)m_pRandLN1->RandValue();
         }


      }
*/
   }


void CWet_hView::OnModelanalysisDefinehydrologymodel()
   {
   if (gpModelDefinition->m_hWnd != NULL)
		gpModelDefinition->ShowWindow( SW_SHOW );	
	else
		{
		gpModelDefinition = new ModelDefinition( "Model Definition" );
		gpModelDefinition->Create( gpMapWnd->m_pMap, WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME | WS_MAXIMIZEBOX );
		gpModelDefinition->ShowWindow( SW_SHOW );	
		}
   }
void CWet_hView::OnModelanalysisModelevaluation()
   {
   if (gpModelEvaluation->m_hWnd != NULL)
		gpModelEvaluation->ShowWindow( SW_SHOW );	
	else
		{
		gpModelEvaluation = new ModelEvaluation( "Model Evaluation" );
		gpModelEvaluation->Create( gpMapWnd->m_pMap, WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME | WS_MAXIMIZEBOX );
		gpModelEvaluation->ShowWindow( SW_SHOW );	
		}
   }
void CWet_hView::OnModelanalysisPesticidemodel()
   {
   if (gpExtensions->m_hWnd != NULL)
		gpExtensions->ShowWindow( SW_SHOW );	
	else
		{
		gpExtensions = new Extensions( "Model Extensions" );
		gpExtensions->Create( gpMapWnd->m_pMap, WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME | WS_MAXIMIZEBOX );
		gpExtensions->ShowWindow( SW_SHOW );	
		}
   }
void CWet_hView::OnModelanalysisRestorationsensitivity()
   {
   if (gpRestoration->m_hWnd != NULL)
		gpRestoration->ShowWindow( SW_SHOW );	
	else
		{
		gpRestoration = new Restoration( "Model Restoration" );
		gpRestoration->Create( gpMapWnd->m_pMap, WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME | WS_MAXIMIZEBOX );
		gpRestoration->ShowWindow( SW_SHOW );	
		}
   }
void CWet_hView::GetFPDistGrid()
   {
   MapLayer *pFlowDir = gpMapWnd->m_pMap->GetLayer( "FLOWDIR" );
   MapLayer *pStreamGrid = gpMapWnd->m_pMap->GetLayer( "STRGRID" );
   MapLayer *pDEM = gpMapWnd->m_pMap->GetLayer( "DEM" );
   MapLayer *pFPDistGrid = gpMapWnd->m_pMap->GetLayer( "FLOWDIRX" );
   
 //  pFPDistGrid->m_name = "FP DistGrad";
   MapLayer *pWatershed = gpMapWnd->m_pMap->GetLayer( "CATCH" );


   float flowPathDistance=0.0f;
   int flowPathCount=0;

   int rows = pDEM->GetRowCount();
   int cols = pDEM->GetColCount();
   for ( int row=0; row < rows; row++ )
      {
      for ( int col=0; col < cols; col++ )
         {
         float distance = pFlowDir->GetDownFlowPathDistance( row, col, pStreamGrid );
         pFPDistGrid->m_pData->Set(col, row, (distance));
         }  // end of:  for ( col < cols )
      }  // end of:  for ( row < rows )
      pFPDistGrid->SaveGridFile("c:\\research\\maimai\\nato\\fpDist.grd");
   }

void CWet_hView::OnTopographiccalculationMeanwatershedelevation()
   {
   MapLayer *pFlowDir = gpMapWnd->m_pMap->GetLayer( "FLOWDIR" );
   MapLayer *pStreamGrid = gpMapWnd->m_pMap->GetLayer( "STRGRID" );
   MapLayer *pDEM = gpMapWnd->m_pMap->GetLayer( "DEM" );
   MapLayer *pFPDistGrid = gpMapWnd->m_pMap->CloneLayer( *pDEM );
   pFPDistGrid->m_name = "FP DistGrad";
   MapLayer *pWatershed = gpMapWnd->m_pMap->GetLayer( "Cells" );
   MapLayer *pStreamV = gpMapWnd->m_pMap->GetLayer("Streams");
   
   int method = 2;
  // the third parameter is the method to use.  a 1 simply returns the average fpdist for each polygon
   // a two returns the entire fp distribution
   int watershedCount   = pWatershed->GetRecordCount();
   int reachCount = pStreamV->GetRecordCount();
   gpHydroModel->m_fpDistributionArray.SetSize(reachCount);
   gpHydroModel->m_fpDistributionArrayStream.SetSize(reachCount);
   for (int i=0; i < reachCount; i++ )
		{
      Poly *pStreamPoly = pStreamV->m_polyArray[i];
      int streamID;
      gpHydroModel->m_pStreamLayer->GetData(i,pStreamV->m_pData->GetCol("HYDRO_ID"), streamID);
      for ( int j=0; j < watershedCount; j++ )//this should be for each reach
         {
		   Poly *pThisPoly = pWatershed->m_polyArray[j];
         int watershedID;
         gpHydroModel->m_pCellLayer->GetData(j,pWatershed->m_pData->GetCol("HYDRO_ID"), watershedID);
         if (watershedID==streamID) // this polygon is in the current reach
            {
		      Poly *pThisPoly = pWatershed->m_polyArray[j];
            float flowPathDistance=0.0f;
            int flowPathCount=0;
            
            float flowPathGradient = 0.001f;
            int rows = pDEM->GetRowCount();
            int cols = pDEM->GetColCount();
            for ( int row=0; row < rows; row++ )
               {
               for ( int col=0; col < cols; col++ )
                  {
                  float x = 0;
                  float y = 0;
                  pDEM->GetGridCellCenter(row, col, x, y);
                  
                  Vertex point;
                  point.x = x;
                  point.y = y;       ///the stream grid is smaller than the flowdir grid
                  if (pThisPoly->IsPointInPoly(point))
                     {
                     int flowPathCellCount=0;
                     float distance = pFlowDir->GetDownFlowPathDistance( row, col, pStreamGrid );
                     float gradient = pFlowDir->GetDownFlowPathGradient(row,col,pStreamGrid,pDEM,flowPathCellCount);
                     gradient=gradient/flowPathCellCount;        
                     float dist_grad = distance/gradient;
                     if (distance==pFlowDir->GetNoDataValue())
                        {
                        dist_grad = pFlowDir->GetNoDataValue();
                        gradient = pFlowDir->GetNoDataValue();
                        }
                     pFPDistGrid->m_pData->Set(col, row, (distance/gradient)); 
                     if (method==1)// we just want the average fpdistance
                        {
                        flowPathGradient +=gradient;
                        flowPathDistance +=distance;
                        flowPathCount++;
                        }
                     if (method==2)// we want to maintain the entire distribution
                        {
                        int dist = (int)(distance/gradient);
                        gpHydroModel->m_fpDistributionArray[i].Add(dist);
                        }
                     }
                  }  // end of:  for ( col < cols )
               }  // end of:  for ( row < rows )
            } // end of :  if watershed==streamID
         }// end of j (watershedCount)
   //  we have found all the flowpaths in the jth reach, but want to now find the median value of that distribution
      float median=-1;
      if (method==2)
         {
         int  temp=-1;
         for (int bottom=gpHydroModel->m_fpDistributionArray[i].GetCount()-1; bottom>0;bottom--)
            {
            for (int position=0;position<bottom;position++)
               {
               int val = gpHydroModel->m_fpDistributionArray[i][position+1];
               int val2 = gpHydroModel->m_fpDistributionArray[i][position];
               if (val < val2)
                  {
                  int temp2 = gpHydroModel->m_fpDistributionArray[i][position];
                  gpHydroModel->m_fpDistributionArray[i][position] = gpHydroModel->m_fpDistributionArray[i][position+1];
                  gpHydroModel->m_fpDistributionArray[i][position+1] = temp2;
                  }
               }
            }
         if (gpHydroModel->m_fpDistributionArray[i].GetCount()<=1)//there were no watersheds for this reach
            {
            median = 0.0f;
            }
         else
            {
            int midValue = gpHydroModel->m_fpDistributionArray[i].GetCount()/2;
            median = (float)gpHydroModel->m_fpDistributionArray[i][midValue];
            }
         }//end of method
   //  float rt = (0.00881f*averageFPDistGrad) + 0.00314f;
 		gpHydroModel->m_pMap->Notify( NT_CALCDIST, i, reachCount );   
      pStreamV->SetData(i,pStreamV->m_pData->GetCol("EXTRA_1"), median);
      
      int cellID = -1;
      pStreamV->GetData(i,pStreamV->m_pData->GetCol("HYDRO_ID"), cellID);
      for (int k=0;k<pStreamV->GetRecordCount();k++)
         {
	      ReachNode *pNode = gpHydroModel->m_reachTree.m_nodeArray[k];
	      REACH_INFO_HYDRO *pHydro1 = (REACH_INFO_HYDRO*) pNode->m_reachInfo.pData; 
         if (pNode->m_reachInfo.index == i)
            pHydro1->cumSedimentOutflow = median;
         }
      }// end of this REACHs


      //Now are finished finding the median fpdist/gradient for each subwatershed and need to accumulate them downslope

      ReachNode *pNode = gpHydroModel->m_reachTree.FindLeftLeaf();  // find leftmost leaf of entire tree (no params defaults to m_pRoot )
	   REACH_INFO_HYDRO *pHydro = gpHydroModel->GetReachInfoHydroFromReachID( pNode->m_reachInfo.reachID );
      gpHydroModel->m_fpDistributionArrayStream[pNode->m_reachInfo.index].Add((int)pHydro->cumSedimentOutflow);
 	   int count=0;
      while ( pNode->m_pDown->m_reachInfo.index != ROOT_NODE )
         {
         ReachNode *pDown = pNode->m_pDown;
         if (( pDown->m_pLeft == pNode ) && ( pDown->m_pRight != NULL ))
            {
            pNode = gpHydroModel->m_reachTree.FindLeftLeaf( pDown->m_pRight );
			   REACH_INFO_HYDRO *pHydro = gpHydroModel->GetReachInfoHydroFromReachID( pNode->m_reachInfo.reachID );
            gpHydroModel->m_fpDistributionArrayStream[pNode->m_reachInfo.index].Add((int)pHydro->cumSedimentOutflow);
            count++;
            }

         else // We have all upstream orders, so can move downstream.
            {
		      pNode = pDown;
			   int reachID = pNode->m_reachInfo.reachID;
			   if ( pNode->m_reachInfo.index == PHANTOM_NODE)  //phantom node - we need our right's right and left pointers!
				//if (  pNode->m_reachInfo.reachID==-1)
            {
				   pNode = pNode->m_pDown;    // just move downstream one node
				   REACH_INFO_HYDRO *pHydro = gpHydroModel->GetReachInfoHydroFromReachID( pNode->m_reachInfo.reachID );
               gpHydroModel->m_fpDistributionArrayStream[pNode->m_reachInfo.index].Add((int)pHydro->cumSedimentOutflow);
               count++;
               int size = gpHydroModel->m_fpDistributionArray[pNode->m_pRight->m_pRight->m_reachInfo.index].GetSize();
               
               for (int i=0; i<gpHydroModel->m_fpDistributionArrayStream[pNode->m_pRight->m_pRight->m_reachInfo.index].GetSize();i++) 
                  gpHydroModel->m_fpDistributionArrayStream[pNode->m_reachInfo.index].Add((int)gpHydroModel->m_fpDistributionArrayStream[pNode->m_pRight->m_pRight->m_reachInfo.index][i]);
               
               for (int i=0; i<gpHydroModel->m_fpDistributionArrayStream[pNode->m_pRight->m_pLeft->m_reachInfo.index].GetSize();i++) 
                   gpHydroModel->m_fpDistributionArrayStream[pNode->m_reachInfo.index].Add((int)gpHydroModel->m_fpDistributionArrayStream[pNode->m_pRight->m_pLeft->m_reachInfo.index][i]);
               
               for (int i=0; i<gpHydroModel->m_fpDistributionArrayStream[pNode->m_pLeft->m_reachInfo.index].GetSize();i++) 
                   gpHydroModel->m_fpDistributionArrayStream[pNode->m_reachInfo.index].Add((int)gpHydroModel->m_fpDistributionArrayStream[pNode->m_pLeft->m_reachInfo.index][i]);

               }
			   else //Not a phantom so just add up the left, right and current area.
				   {
				   REACH_INFO_HYDRO *pHydro = gpHydroModel->GetReachInfoHydroFromReachID( pNode->m_reachInfo.reachID );
				   if ( pNode->m_pRight != NULL )
                  {
                  gpHydroModel->m_fpDistributionArrayStream[pNode->m_reachInfo.index].Add((int)pHydro->cumSedimentOutflow);
                  count++;
                  for (int i=0; i<gpHydroModel->m_fpDistributionArrayStream[pNode->m_pRight->m_reachInfo.index].GetSize();i++) 
                     gpHydroModel->m_fpDistributionArrayStream[pNode->m_reachInfo.index].Add((int)gpHydroModel->m_fpDistributionArrayStream[pNode->m_pRight->m_reachInfo.index][i]);
                  
                  for (int i=0; i<gpHydroModel->m_fpDistributionArrayStream[pNode->m_pLeft->m_reachInfo.index].GetSize();i++) 
                     gpHydroModel->m_fpDistributionArrayStream[pNode->m_reachInfo.index].Add((int)gpHydroModel->m_fpDistributionArrayStream[pNode->m_pLeft->m_reachInfo.index][i]);                                 
                  }
               else
                  {
                  gpHydroModel->m_fpDistributionArrayStream[pNode->m_reachInfo.index].Add((int)pHydro->cumSedimentOutflow); 
                  count++;
                  for (int i=0; i<gpHydroModel->m_fpDistributionArrayStream[pNode->m_pLeft->m_reachInfo.index].GetSize();i++) 
                     gpHydroModel->m_fpDistributionArrayStream[pNode->m_reachInfo.index].Add((int)gpHydroModel->m_fpDistributionArrayStream[pNode->m_pLeft->m_reachInfo.index][i]);
                  }
               }
         }//end of : else
      }//end of While (search for the root node...)

//Now go through the m_fpDistributionArray and again find the median value.

 for (int j=0;j<pStreamV->GetRecordCount();j++)
    {  
   float median=-1.0f;
   float rt=0.0f;
   if (method==2)
      {
      int  temp=-1;
      int count = gpHydroModel->m_fpDistributionArrayStream[j].GetCount();
      for (int bottom=gpHydroModel->m_fpDistributionArrayStream[j].GetCount()-1; bottom>0;bottom--)
         {
         for (int position=0;position<bottom;position++)
            {
            int val = gpHydroModel->m_fpDistributionArrayStream[j][position+1];
            int val2 = gpHydroModel->m_fpDistributionArrayStream[j][position];
            if (val < val2)
               {
               int temp2 = gpHydroModel->m_fpDistributionArrayStream[j][position];
               gpHydroModel->m_fpDistributionArrayStream[j][position] = gpHydroModel->m_fpDistributionArrayStream[j][position+1];
               gpHydroModel->m_fpDistributionArrayStream[j][position+1] = temp2;
               }
            }
         }
     // if (gpHydroModel->m_fpDistributionArrayStream[j].GetCount()<=1)//there were no watersheds for this reach
       //  median = 0;
     // else
       //  {
         
         int midValue = gpHydroModel->m_fpDistributionArrayStream[j].GetCount();
         if (midValue<0)
            median=0.0f;
         else
            median = (float)gpHydroModel->m_fpDistributionArrayStream[j][(int)(midValue/2)];

         rt = (0.002347f*median) + 0.186f;
       //  }
      }//end of j - now on to the next stream reach in the array  

      // Then add the median value to somewhere that can be mapped...
    gpHydroModel->m_pMap->Notify( NT_CALCDIST, j, reachCount );   
    pStreamV->SetData(j,pStreamV->m_pData->GetCol("EXTRA_1"), median);
    int streamID=-1;
    pStreamV->GetData(j,pStreamV->m_pData->GetCol("HYDRO_ID"), streamID);

      for ( int jj=0; jj < watershedCount; jj++ )//this should be for each reach
         {
		   Poly *pThisPoly = pWatershed->m_polyArray[jj];
         int watershedID;
         gpHydroModel->m_pCellLayer->GetData(jj,pWatershed->m_pData->GetCol("HYDRO_ID"), watershedID);
         if (watershedID==streamID) // this polygon is in the current reach
            pWatershed->SetData(jj,pWatershed->m_pData->GetCol("EXTRA_1"), rt);
            
         }


    }   
    float minVal=0.0f;
    float maxVal=0.0f;
    pWatershed->GetDataMinMax( pWatershed->m_pData->GetCol("EXTRA_1"), &minVal, &maxVal );
    pWatershed->SetBins(minVal,maxVal,10);
    pWatershed->ClassifyData(pWatershed->m_pData->GetCol("EXTRA_1"));
    if ( gpMapWnd != NULL )
       gpMapWnd->RefreshList();  
	gpMapWnd->m_pMapList->RedrawWindow();
	CDC *dc = gpMapWnd->m_pMap->GetDC();
	gpMapWnd->m_pMap->DrawMap(*dc);
	gpMapWnd->m_pMap->ReleaseDC(dc);
    
    pWatershed->SaveShapeFile("c:\\temp\\test.shp",false);

   }//end of method...

   void CWet_hView::OnTopographiccalculationSubbasinfpdistribution()
   {// to accomplish this, we simply want to maintain all of the flowpath lengths for each of the basins.
      // TODO: Add your command handler code here
   }

   void CWet_hView::OnTopographiccalculationSubbasindistancetodsoutletdistribution()
   {//  here we need to calculate the distance from the top of the stream channel to the end of the channel (in each basin)
      // TODO: Add your command handler code here
   }

void CWet_hView::OnTopographiccalculationDistancefrompolytostream()
   {
		MapLayer *pToLayer = gpMapWnd->m_pMap->GetLayer("STRGRID");
		MapLayer *pFlowDirectionLayer = gpMapWnd->m_pMap->GetLayer("FLOWDIR");
		int colToSet = gpHydroModel->m_pCellLayer->GetFieldCol("CENTROID");
		gpHydroModel->m_pCellLayer->PopulateFlowPathDistanceFromCentroid(pFlowDirectionLayer,pToLayer,colToSet);
   }

void CWet_hView::OnTopographiccalculationCalculateupstreamarea()
   {
      /*gpHydroModel->CalcWatershedInfo();
      gpHydroModel->CalculateTotalArea();*/
    gpHydroModel->GetNeighbors(1, "c:\\temp\\hiOriginal.prn","c:\\temp\\hiMe.prn","c:\\temp\\hiNeighbors.prn","c:\\temp\\hiWeight.prn");      
   }

void CWet_hView::OnAdditionalstuffLoaduaradar()
   {
   WH_NOTIFYPROC p = gpHydroModel->InstallNotifyHandler( UpdateMapProc );   
   gpHydroModel->LoadDpaData("c:\\temp\\crap");
   gpHydroModel->InstallNotifyHandler( p );
   }

void CWet_hView::OnTopographiccalculationSideofstream()
   {
      MapLayer *pSourceLayer = gpMapWnd->m_pMap->GetLayer( "Cells" );
		MapLayer *pToLayer = gpMapWnd->m_pMap->GetLayer("STRGRID");
		MapLayer *pFlowDirectionLayer = gpMapWnd->m_pMap->GetLayer("FLOWDIR");
		int colToSet = pSourceLayer->GetFieldCol("SIDE");
 
		int thisCount = pSourceLayer->GetRecordCount();
      int order = -1;
      
      for ( int i=0; i < thisCount; i++ )
			{
			Poly *pPoly = pSourceLayer->GetPolygon(i);
         float test = pPoly->GetArea();
			Vertex centroid = pPoly->GetCentroid();
      //   int colOrder = pSourceLayer->GetFieldCol("ORDER");
            
      //   pSourceLayer->GetData(i,colOrder,order);
         int side = -1;      
     //    if (order == 1)
         //   side = 0;
      //   else
         //   {
		   	int row = 0;
		   	int col = 0;
		   	pFlowDirectionLayer->GetGridCellFromCoord( centroid.x, centroid.y, row, col);
		   	side = pFlowDirectionLayer->GetSideOfStream(row, col, pToLayer);
         //   }

         pSourceLayer->SetData(i, colToSet, side);
         }
   }


   void CWet_hView::OnModelanalysisSavemodelstate()
   {
      // TODO: Add your command handler code here
      gpHydroModel->SaveState(2);
   }

   void CWet_hView::OnTopographiccalculationDisttostream()
   {
      GetFPDistGrid();
      // TODO: Add your command handler code here
   }

   void CWet_hView::OnMenu()
   {
      CFileDialog dlg( TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
            "Ascii Grid Files|*.asc|All files|*.*||");

      if ( dlg.DoModal() == IDOK )
         {
         CString filename( dlg.GetPathName() );
		 gpMapWnd->m_pMap->AddGridLayer(filename,DOT_FLOAT);
		 }



   }

   void CWet_hView::OnModelanalysisRunhillslopemodel()
   {
	   // assume we've loaded a DEM
	   // Need to allocate a WET_H object
   }

void CWet_hView::OnTopographiccalculationWillametteqmovie()
	{
	}
		/*
	//read Willamette Shapefile
    //read data with 365 rows and 9 cols.  1 column for each major subwatershed (minus the middle Willamette, for which we don't have data. 1 row for each day in year
    //for each day, classify map according to data
    //append to avi file
    //finish
    BOOL m_saveEmfOutput = TRUE;
    if (m_saveEmfOutput)
         { 
         numMovies = 1;
         movieFrameRate = 30;
         AVIGenerators = new CAVIGenerator*[numMovies];  // array of pointers to AVI generators
         auxMovieStuff = new AuxMovieStuff[numMovies];
         CDC* dc = gpView->GetWindowDC();
         // get the window's dimensions
         CRect rect;
         gpView->GetWindowRect(rect);
         for(int i = 0; i < numMovies; i++)
            {
            // get output filename from user
            CFileDialog movieSaveDlg(FALSE, "avi", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Movie Files (*.avi)|*.avi");
            CString movieFileName;
            if (movieSaveDlg.DoModal() == IDOK)
               {
               movieFileName = movieSaveDlg.GetPathName();
               }
            // need to handle canceling out of the file dialog box somehow

            AVIGenerators[i] = new CAVIGenerator(movieFileName, (CView*)gpView, movieFrameRate);

            AVIGenerators[i]->InitEngine();
            auxMovieStuff[i].bitmapDC.CreateCompatibleDC(dc);
            auxMovieStuff[i].bitmap.CreateCompatibleBitmap(dc, rect.Width(), rect.Height());
            auxMovieStuff[i].bitmapDC.SelectObject(&auxMovieStuff[i].bitmap);
            auxMovieStuff[i].bi.bmiHeader = *AVIGenerators[i]->GetBitmapHeader();
            auxMovieStuff[i].lpbi = &auxMovieStuff[i].bi;
            auxMovieStuff[i].bitAddress = new BYTE[3*rect.Width()*rect.Height()];
            }
         gpView->ReleaseDC(dc);
         }
	


    ASSERT ( gpMapWnd != NULL );
    ASSERT ( gpMapWnd->m_pMap != NULL );
	gpMapWnd->m_pMap->AddShapeLayer("c:\\research\\willamette\\will2.shp", true);
	MapLayer *pWill = gpMapWnd->m_pMap->GetLayer(0);
	FDataObj pAsciiData = FDataObj();
	pAsciiData.ReadAscii("c:\\research\\willamette\\willINDI.csv", ',');
	int numDays = pAsciiData.GetRowCount();
	int numWshed = pAsciiData.GetColCount()-1;
	float ti = -1.0f;
    
	gpMapWnd->m_pScatterPrecip = gpMapWnd->AllocateScatter( &pAsciiData, 0 );
	gpMapWnd->m_pScatterPrecip->RedrawWindow();

    for (int i=0;i<numDays;i++)           // for each day of the year (or row in the data)
		{
		int numSheds = pWill->m_pData->GetRowCount();
		float time = -1.0f;
		pAsciiData.Get(0,i,time);
        gpMapWnd->m_pScatterPrecip->UpdateCurrentPos( time );
		//gpMapWnd->m_pScatterPrecip->RedrawWindow();
		for (int j=0;j<numSheds;j++)  // go through each watershed (there are 12 of them)
			{
			float value = -1.0f;                              // and stick the data from pAsciiData into the datatable
			pAsciiData.Get(j+1,i,value);                         //dischargeValue
			pWill->SetData(j,pWill->GetFieldCol("EXTRA_2"),value); //add the value to the correct column in the Maplayer
			
			}
        CString msg;
		COleDateTime t( time );
		CString s = t.Format("%m/%d" );
		msg.Format( "Completed %s" , s );
        gpMain->SetStatusMsg( msg );

		pWill->SetBinColorFlag( BCF_BLUEGREEN );
		pWill->SetBins( -0.2, 0.4, 20 );
		pWill->SetActiveField( pWill->GetFieldCol( "EXTRA_2" ) );
	    pWill->ClassifyData();
        
		if ( gpMapWnd != NULL )
     	   gpMapWnd->RefreshList();

	    gpMapWnd->m_pMapList->RedrawWindow();
	    CDC *dc = gpMapWnd->m_pMap->GetDC();
	    gpMapWnd->m_pMap->DrawMap(*dc);
	    gpMapWnd->m_pMap->ReleaseDC(dc);
   
		if (m_saveEmfOutput)
			{
			CDC* dc = gpView->GetWindowDC();
			CRect rect;
			gpView->GetWindowRect(rect);

			for(int i = 0; i < numMovies; i++)
				{
				// copy from the application window to the new device context (and thus the bitmap)
				BOOL blitSuc = auxMovieStuff[i].bitmapDC.BitBlt(0, 0,
																rect.Width(),
																rect.Height(),
																dc, 0, 0, SRCCOPY);

				GetDIBits(auxMovieStuff[i].bitmapDC, HBITMAP(auxMovieStuff[i].bitmap),
							0, rect.Height(), auxMovieStuff[i].bitAddress,
							auxMovieStuff[i].lpbi, DIB_RGB_COLORS);

				AVIGenerators[i]->AddFrame((BYTE*)auxMovieStuff[i].bitAddress);
				}
			gpView->ReleaseDC(dc);
			}
		}
		

	if(m_saveEmfOutput)
		{
		for(int i = 0; i < numMovies; i++)
			{
			AVIGenerators[i]->ReleaseEngine();
			delete AVIGenerators[i];
			AVIGenerators[i] = 0;

			//ReleaseDC(NULL,auxMovieStuff[i].bitmapDC);
			delete [] auxMovieStuff[i].bitAddress;
			}
		delete [] AVIGenerators;
		delete [] auxMovieStuff;
		AVIGenerators = 0;
		}
	}
   */
void CWet_hView::OnTopographiccalculationDisgradientandquotientforgrid()
   {
   MapLayer *pFlowDir = gpMapWnd->m_pMap->GetLayer( "FLOWDIR" );
   MapLayer *pStreamGrid = gpMapWnd->m_pMap->GetLayer( "STRGRID" );
   MapLayer *pFPDistGrid = gpMapWnd->m_pMap->GetLayer( "FLOWDIRX" );
   MapLayer *pFPDistGrid1 = gpMapWnd->m_pMap->CloneLayer( *pFPDistGrid );
   MapLayer *pFPDistGrid2 = gpMapWnd->m_pMap->CloneLayer( *pFPDistGrid );
   MapLayer *pDEM = gpMapWnd->m_pMap->GetLayer("DEM");
   /*
   float flowPathDistance=0.0f;
   int flowPathCount=0;
   
   float flowPathGradient = 0.0001f;
   int rows = pFlowDir->GetRowCount();
   int cols = pFlowDir->GetColCount();
   int cellCount = rows*cols;
   int sofar=0;
   for ( int row=0; row < rows; row++ )
      {
      for ( int col=0; col < cols; col++ )
         {
         int flowPathCellCount=0;
         float distance = pFlowDir->GetDownFlowPathDistance( row, col, pStreamGrid );
         float gradient = pFlowDir->GetDownFlowPathGradient(row,col,pStreamGrid,pDEM,flowPathCellCount);
         gradient=gradient/flowPathCellCount;        
         float dist_grad = distance/gradient;
         if (distance==pFlowDir->GetNoDataValue())
            {
            dist_grad = pFlowDir->GetNoDataValue();
            gradient = pFlowDir->GetNoDataValue();
            }

         pFPDistGrid->m_pData->Set(col, row, gradient);  
         pFPDistGrid1->m_pData->Set(col, row, distance);  
         pFPDistGrid2->m_pData->Set(col, row, dist_grad);  
         sofar = sofar++;
         gpHydroModel->m_pMap->Notify( NT_CALCDIST, sofar, cellCount ); 
         }  // end of:  for ( col < cols )
      }  // end of:  for ( row < rows )
*/
   float flowPathDistance=0.0f;
   int flowPathCount=0;
    float flowPathGradient = 0.0001f;
   MapLayer *pWatershed = gpMapWnd->m_pMap->GetLayer( "Cells" );
   int polygonCount   = pWatershed->GetRecordCount();
   for ( int i=0; i < polygonCount; i++ )
		{    
		Poly *pThisPoly = pWatershed->m_polyArray[ i ];
      float flowPathDistance=0.0f;
      int flowPathCount=0;
      float flowPathGradient = 0.0001f;
      int rows = pDEM->GetRowCount();
      int cols = pDEM->GetColCount();
      for ( int row=0; row < rows; row++ )
         {
         for ( int col=0; col < cols; col++ )
            {
               float x = 0;
               float y = 0;
               pDEM->GetGridCellCenter(row, col, x, y);
              
               Vertex point;
               point.x = x;
               point.y = y;       
               if (pThisPoly->IsPointInPoly(point))
                  {
                  int flowPathCellCount=0;
                  float distance = pFlowDir->GetDownFlowPathDistance( row, col, pStreamGrid );
                  float gradient = pFlowDir->GetDownFlowPathGradient(row,col,pStreamGrid,pDEM,flowPathCellCount);
                  gradient=gradient/flowPathCellCount;        
                  float dist_grad = distance/gradient;
                  if (distance==pFlowDir->GetNoDataValue())
                     {
                     dist_grad = pFlowDir->GetNoDataValue();
                     gradient = pFlowDir->GetNoDataValue();
                     }
                  
                  pFPDistGrid->m_pData->Set(col, row, (distance/gradient));
                  flowPathGradient +=gradient;
                  flowPathDistance +=distance;

                  }
               }
            }  // end of:  for ( col < cols )
         }  // end of:  for ( row < rows )
      
      float averageFPDistance = flowPathDistance/flowPathCount;
      float averageFPGrad = flowPathGradient/flowPathCount;
      float averageFPDistGrad = averageFPDistance/averageFPGrad;
      //float rt = (0.00881f*averageFPDistGrad) + 0.00314f;
 		gpHydroModel->m_pMap->Notify( NT_CALCDIST, i, polygonCount );   
      pWatershed->SetData(i,pWatershed->m_pData->GetCol("EXTRA_1"), averageFPDistGrad);
















   pFPDistGrid2->SaveGridFile("c:\\temp\\Dist_Grad.asc");
   pFPDistGrid1->SaveGridFile("c:\\temp\\Dist.asc");
   pFPDistGrid->SaveGridFile("c:\\temp\\Grad.asc");
   }
