// PPUserInteractionSimControl.cpp : implementation file
//
#include "stdafx.h"
#pragma hdrstop
#include <time.h>

#include "wet_h.h"
#include "PPUserInteractionSimControl.h"
#include "WizUserInteraction.h"
#include "WET_Hydro.h"
#include "MapWnd.h"
#include <Map.h>
#include "MainFrm.h"
#include <ScatterWnd.h>
#include "wet_hView.h"
#include ".\ppuserinteractionsimcontrol.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern WET_Hydro *gpHydroModel;
extern WizUserInteraction *gpWizUserInteraction;
extern MapWnd *gpMapWnd;
extern CWet_hView *gpView;
extern CMainFrame *gpMain;

bool UpdateMapProc( WET_Hydro*pHydro, WH_NOTIFY_TYPE type, float, float, long a1, long extra );

/////////////////////////////////////////////////////////////////////////////
// PPUserInteractionSimControl property page

IMPLEMENT_DYNCREATE(PPUserInteractionSimControl, CPropertyPage)

PPUserInteractionSimControl::PPUserInteractionSimControl() : CPropertyPage(PPUserInteractionSimControl::IDD)
{
	//{{AFX_DATA_INIT(PPUserInteractionSimControl)
	m_errorTolerance = gpHydroModel->m_rkvTolerance;
	m_initialTimeStep = gpHydroModel->m_timeStep;
	m_maximumTimeStep = gpHydroModel->m_rkvMaxTimeStep;
	m_minimumTimeStep = gpHydroModel->m_rkvMinTimeStep;
	m_modelStartTime = _T("");
	m_modelStopTime = _T("");
	m_useVariableStep = gpHydroModel->m_useVariableStep;
	//}}AFX_DATA_INIT
}

PPUserInteractionSimControl::~PPUserInteractionSimControl()
{
}

void PPUserInteractionSimControl::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PPUserInteractionSimControl)
	DDX_Control(pDX, IDC_REACHLIST, m_reachList);
	DDX_Control(pDX, IDC_MODELSTOPTIME, m_comboStopTime);
	DDX_Control(pDX, IDC_MODELSTARTTIME, m_comboStartTime);
	DDX_Control(pDX, IDC_CELLLIST, m_cellList);
	DDX_Text(pDX, IDC_ERRORTOLERANCE, m_errorTolerance);
	DDX_Text(pDX, IDC_INITIALTIMESTEP, m_initialTimeStep);
	DDX_Text(pDX, IDC_MAXIMUMTIMESTEP, m_maximumTimeStep);
	DDX_Text(pDX, IDC_MINIMUMTIMESTEP, m_minimumTimeStep);
	DDX_CBString(pDX, IDC_MODELSTARTTIME, m_modelStartTime);
	DDX_CBString(pDX, IDC_MODELSTOPTIME, m_modelStopTime);
	DDX_Check(pDX, IDC_VARIABLETIMESTEP, m_useVariableStep);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(PPUserInteractionSimControl, CPropertyPage)
	//{{AFX_MSG_MAP(PPUserInteractionSimControl)
	ON_BN_CLICKED(IDC_SAMPLECELLS, OnSamplecells)
	ON_BN_CLICKED(IDC_SAMPLEREACHES, OnSamplereaches)
	ON_BN_CLICKED(IDC_REMOVECELLS, OnRemovecells)
	ON_BN_CLICKED(IDC_REMOVEREACHES, OnRemovereaches)
	//}}AFX_MSG_MAP
   ON_CBN_SELCHANGE(IDC_MODELSTARTTIME, OnCbnSelchangeModelstarttime)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PPUserInteractionSimControl message handlers
/////////////////////////////////////////////////////////////////////////////

void PPUserInteractionSimControl::AddCell(int polyID)
   {
   CString str;
   str.Format( "%i", polyID );
   m_cellList.AddString( str );
   }
void PPUserInteractionSimControl::AddGridCell(int col, int row)
   {
   CString str;
   str.Format( "%i, %i", col, row );
   m_cellList.AddString( str );
   }

void PPUserInteractionSimControl::AddReach(int polyID)
   {
   // has the reach already been added?
   for ( int i=0; i < m_reachList.GetCount(); i++ )
      {
      char buffer[ 64 ];
      m_reachList.GetText( i, buffer );

      int reach;
      int fields = sscanf( buffer, "%i", &reach );
      ASSERT( fields == 1 );

      if ( reach == polyID )
         {
         MessageBeep( 0 );
         return;
         }
      }

   // not already there, so add it
   CString str;
   str.Format( "%i", polyID );
   m_reachList.AddString( str );
   }


void PPUserInteractionSimControl::OnRunmodel() 
   {

   }

void PPUserInteractionSimControl::OnIdok() 
{

//   ShowWindow( SW_HIDE );


   }

void PPUserInteractionSimControl::OnUpdateCmdUI( CFrameWnd *pTarget, BOOL disableIfNoHandler )
   {
   UpdateDialogControls( pTarget, FALSE );
   }

void PPUserInteractionSimControl::OnSelectallreaches() 
	{
	for ( int i=0; i < gpHydroModel->m_pStreamLayer->GetRecordCount(); i++ )
      gpWizUserInteraction->m_ppUserInteractionSimControl.AddReach( i );
	}

void PPUserInteractionSimControl::OnDoubleclickedRemovereaches() 
{
	m_reachList.AddString("0");
	
}

void PPUserInteractionSimControl::OnSixReaches() 
{
	// TODO: Add your control notification handler code here
	
}

void PPUserInteractionSimControl::OnRemovereaches() 
{
/*	m_reachList.AddString("0");
	m_reachList.AddString("972");
	m_reachList.AddString("807");
	m_reachList.AddString("475");
	m_reachList.AddString("1146");	
	m_reachList.AddString("277");*/

   m_reachList.DeleteString(m_reachList.GetCurSel());
}

void PPUserInteractionSimControl::OnOptimize() 
	{
	for (int i=0; i < m_reachList.GetCount(); i++ )
      {
      char buffer[ 64 ];
      m_reachList.GetText( i, buffer );

      int reach;
      int fields = sscanf( buffer, "%i", &reach );
      ASSERT( fields == 1 );

      gpHydroModel->m_reachDataArray.Add( reach );
      }

	gpHydroModel->SetTimeStep( m_initialTimeStep );
	gpHydroModel->SetStopTime( (float) atof(m_modelStopTime) );
   gpHydroModel->m_stopTimeHolder = gpHydroModel->m_stopTime;
	gpHydroModel->SetStartTime( (float) atof(m_modelStartTime) );
	gpHydroModel->m_useVariableStep = m_useVariableStep ? true : false;
//	if ( gpHydroModel->m_runNumber == 1 )
//		gpHydroModel->CalcWatershedInfo( gpHydroModel->m_is);	

	gpView->m_optimizeDlg.DoModal();	
	}

void PPUserInteractionSimControl::OnRunsynthetic() 
{
}


BOOL PPUserInteractionSimControl::OnSetActive() 
   {
   for (int i=0;i<m_cellList.GetCount();i++)
      m_cellList.DeleteString(i); 
   m_cellList.ResetContent();
   if (gpHydroModel->m_modelStructureMethod==WH_DISTRIBUTE_FPGRID || gpHydroModel->m_modelStructureMethod==WH_DISTRIBUTE_WTHEIGHT)
      {
      for ( int y=0;y<gpHydroModel->m_cellDataArray.GetSize();y++)
         {
         CString str;
         str.Format( "%i,%i", gpHydroModel->m_cellDataArray[y],gpHydroModel->m_cellDataArray[y+1] );
         y++;
         m_cellList.AddString( str );
         }
      }
   else
      {
      for ( int y=0;y<gpHydroModel->m_cellDataArray.GetSize();y++)
         {
         CString str;
         str.Format( "%i", gpHydroModel->m_cellDataArray[y] );
         m_cellList.AddString( str );
         }
      }

   for (i=0;i<m_reachList.GetCount();i++)
      m_reachList.DeleteString(i);
   m_reachList.ResetContent();
   for (int y=0;y<gpHydroModel->m_reachDataArray.GetSize();y++)
      {
      CString str;
      str.Format( "%i", gpHydroModel->m_reachDataArray[y] );
      m_reachList.AddString( str );
      }

	int colCount = gpHydroModel->m_pClimateData->GetRowCount();
	for(i=0; i < (int) gpHydroModel->m_pClimateData->GetRowCount();i++)
		{
		float datetime = gpHydroModel->m_pClimateData->Get(0,i);
		COleDateTime t( datetime );
      CString s = t.Format("%m/%d/%Y" );

		   m_comboStartTime.AddString(s);
		   m_comboStopTime.AddString(s);
        
      
		}
	
	COleDateTime t1( gpHydroModel->m_startTime );
	CString start = t1.Format( "%m/%d/%Y" );

	COleDateTime t2( gpHydroModel->m_stopTime );
	CString stop = t2.Format( "%m/%d/%Y" );
		
	m_comboStartTime.SetCurSel(m_comboStartTime.FindStringExact(0,start));
   m_comboStopTime.SetCurSel(m_comboStartTime.FindStringExact(0,stop)) ;

   CheckDlgButton( IDC_SAMPLEREACHES, 1 );
	
	return CPropertyPage::OnSetActive();
}

void PPUserInteractionSimControl::OnSamplecells() 
   {
   this->MessageBox( "Select the desired reaches, and then double click on the caption of the minimized window to go again" );
   GetParent()->ShowWindow( SW_MINIMIZE );	
   }

void PPUserInteractionSimControl::OnSamplereaches() 
   {
   this->MessageBox( "Select the desired reaches, and then double click on the caption of the minimized window to go again" );

   GetParent()->ShowWindow( SW_MINIMIZE );	
	
   }



void PPUserInteractionSimControl::OnRemovecells() 
{
   m_cellList.DeleteString(m_cellList.GetCurSel());
	
}

void PPUserInteractionSimControl::OnCbnSelchangeModelstarttime()
{
   // TODO: Add your control notification handler code here
}
