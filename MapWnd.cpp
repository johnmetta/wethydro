// MapWnd.cpp : implementation file
//

#include "stdafx.h"
#pragma hdrstop
#include "wet_h.h"
#include "wet_hView.h"
#include "wet_hdoc.h"
#include "mainfrm.h"
#include "MapWnd.h"

#include "MapListView.h"
#include "ellPropDlg1.h"
#include "MapStatsDlg.h"
#include "decisionmanager.h"
#include <scatterWnd.h>
#include "WizUserInteraction.h"
#include "ResultPlotWnd.h" 
#include "MessagesWnd.h"


#include <msgwnd.h>
#include <map.h>
#include <maplayer.h>
#include "Wet_hydro.h"

#include <afxcview.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MapWnd

extern MapWnd           *gpMapWnd;
extern CMainFrame       *gpMain;
extern DecisionManager  *gpDecisionManager;
extern CWet_hDoc      *gpDoc;
extern CWet_hView     *gpView;
extern WizUserInteraction *gpWizUserInteraction;
extern ResultPlotWnd *gpResultPlotWnd;
extern WET_Hydro *gpHydroModel;
extern MessagesWnd *gpMessagesWnd;

void MapProc( Map *pMap, NOTIFY_TYPE type, int a0, long a1, long extra );

typedef bool (*WH_NOTIFYPROC)( WET_Hydro*, WH_NOTIFY_TYPE, float, float, long, long );



bool UpdateMapProc( WET_Hydro*pHydro, WH_NOTIFY_TYPE type, float, float, long a1, long extra );

bool UpdateMapProc( WET_Hydro*pHydro, WH_NOTIFY_TYPE type, float time, float timeStep, long a1, long extra )
   {
   if ( type == WH_STARTRUN )
		{
		if (pHydro->m_updateStream==TRUE)
			{

			int col = pHydro->GetFieldColFromInterfaceValue(0);
			pHydro->m_pStreamLayer->SetActiveField( col ); // flow
			int activeField = pHydro->m_pStreamLayer->GetActiveField();
			pHydro->m_pStreamLayer->UseVarWidth(gpMapWnd->m_lineWidth);
			pHydro->m_pStreamLayer->SetBinColorFlag( BCF_BLUE );
            pHydro->m_pStreamLayer->SetBins( gpMapWnd->m_minBinValue, gpMapWnd->m_maxBinValue, gpMapWnd->m_numBin );
			pHydro->m_pStreamLayer->ClassifyData();	
		//	gpMapWnd->RefreshList();
            
			gpMain->SetStatusMsg( "starting run" );
			}

		if (pHydro->m_updateUpland==TRUE)
			{
			int col = pHydro->GetFieldColFromInterfaceValue(1);
			pHydro->m_pCellLayer->SetActiveField( col ); // flow
			int activeField = pHydro->m_pStreamLayer->GetActiveField();
			pHydro->m_pCellLayer->SetBinColorFlag( BCF_GRAY );
         pHydro->m_pCellLayer->SetBins(pHydro->m_pDynamicMapInfo->minVal, pHydro->m_pDynamicMapInfo->maxVal,15);
			
			pHydro->m_pCellLayer->ClassifyData();	
			
			if ( gpMapWnd != NULL )
			   gpMapWnd->RefreshList();
            
			gpMain->SetStatusMsg( "starting run" );
			}
		}
   
   if ( type == WH_ENDRUN )
		{
      gpMain->SetStatusMsg( "done" );
      gpMessagesWnd->SummarizeModel();
		}
   if ( type == WH_ALLOCATEOBJECTS )
		{
      CString msg;
	
      msg.Format("Allocating Model Elements for reach %i of %i", (int)time, (int)timeStep);
      gpMain->SetStatusMsg( msg);
      
		}
   if ( type == WH_BUILDTOPOLOGY )
		{
      CString msg;
      msg.Format("Connecting Model Elements for reach %i of %i - element %i of %i", (int)time, (int)timeStep,  (int)a1, (int)extra);
      gpMain->SetStatusMsg( msg);      
		}
   if ( type == WH_UPDATERADARRAIN)
      {
       CString msg;
       struct tm *newtime;
       int time2=(int)time;
       __int64 ltime = time2;
     // _time64( &ltime );

       newtime = _gmtime64( &ltime );
       int hour = newtime->tm_hour;
       int min=newtime->tm_min;
       int sec=newtime->tm_sec;
       int day=newtime->tm_mday;
       int mon=newtime->tm_mon+1;
       int year=newtime->tm_year+1900;
		//COleDateTime t( time );
	//	CString s = t.Format("%m/%d/%y:%H:%M" );
      if(pHydro->m_saveEmfOutput)
         pHydro->WriteFrame();

      msg.Format( "Radar Rainfall...Completed %i/%i/%i: %i:%i:%i " , mon,day,year,hour,min,sec);
      gpMain->SetStatusMsg( msg );     
      //pHydro->m_pMap->GetLayer("RADAR")->ClassifyData();
        // pHydro->m_pMap->UpdateMapExtents();
	   //   gpMapWnd->RefreshList();
         
	  //    gpMapWnd->m_pMap->RedrawWindow();     
 	      CDC *dc = gpMapWnd->m_pMap->GetDC();
	      gpMapWnd->m_pMap->DrawMap(*dc);
	      gpMapWnd->m_pMap->ReleaseDC(dc);     
      
      
      }
   
   if ( type == WH_UPDATE)
      {
      ///////////////////////////////////////////////////////////////
      // notify interface
      CString msg;
	  COleDateTime t( time );
	  CString s = t.Format("%m/%d/%y:%H:%M" );
	  double timeStepHours = timeStep * 24;
	  int timeInt = int(timeStepHours);
	  double timeStepMinutes = ( timeStepHours - timeInt ) * 60;
      msg.Format( "Completed %s    Current Time Step is %6.8f days - or %i hours %6.2f minutes  Run number %i" , s, timeStep , timeInt, timeStepMinutes, gpHydroModel->m_runNumber );
      gpMain->SetStatusMsg( msg );
      if (pHydro->m_updateStream==TRUE || pHydro->m_updateUpland==TRUE)
         {
		   if (pHydro->m_updateUpland)
			   {
			   int fieldCol = pHydro->GetFieldColFromInterfaceValue(1);//this just gets EXTRA_1
		      if (pHydro->m_dataToMap=="Soil Water Content")
			      gpHydroModel->CopyStateFromReachTreeToMapLayer(1,0); // this just copies to EXTRA_1
		      else if (pHydro->m_dataToMap=="ET")
			      gpHydroModel->CopyStateFromReachTreeToMapLayer(1,1);
		      else if (pHydro->m_dataToMap=="Air Temp")
			      gpHydroModel->CopyStateFromReachTreeToMapLayer(1,2);
		      else if (pHydro->m_dataToMap=="Unsat SWC")
			      gpHydroModel->CopyStateFromReachTreeToMapLayer(1,3);
		      else if (pHydro->m_dataToMap=="Saturated Depth")
			      gpHydroModel->CopyStateFromReachTreeToMapLayer(1,4);
		      else if (pHydro->m_dataToMap=="Saturation Deficit")
			      gpHydroModel->CopyStateFromReachTreeToMapLayer(1,5);
		      else if (pHydro->m_dataToMap=="Unsaturated Depth")
			      gpHydroModel->CopyStateFromReachTreeToMapLayer(1,6);
		      else if (pHydro->m_dataToMap=="Tracer Concentration")
			      gpHydroModel->CopyStateFromReachTreeToMapLayer(1,7);
		      else if (pHydro->m_dataToMap=="Sediment Output")
			      gpHydroModel->CopyStateFromReachTreeToMapLayer(1,8);
		      else if (pHydro->m_dataToMap=="Foliar Runoff Rate")
			      gpHydroModel->CopyStateFromReachTreeToMapLayer(1,9);
		      else if (pHydro->m_dataToMap=="Surf Pest")
			      gpHydroModel->CopyStateFromReachTreeToMapLayer(1,10);
		      else if (pHydro->m_dataToMap=="Unsat Pest")
			      gpHydroModel->CopyStateFromReachTreeToMapLayer(1,11);
		      else if (pHydro->m_dataToMap=="Sat Pest")
			      gpHydroModel->CopyStateFromReachTreeToMapLayer(1,12);
		      else if (pHydro->m_dataToMap=="Sat Pest Rate")
			      gpHydroModel->CopyStateFromReachTreeToMapLayer(1,13);
		      else if (pHydro->m_dataToMap=="Upland Identifier")
			      gpHydroModel->CopyStateFromReachTreeToMapLayer(1,14);
		      else if (pHydro->m_dataToMap=="Upslope UPSLOPE_INFO index")
			      gpHydroModel->CopyStateFromReachTreeToMapLayer(1,15);
		      else if (pHydro->m_dataToMap=="Upslope Area")
			      gpHydroModel->CopyStateFromReachTreeToMapLayer(1,16);
            else        //  if the data is not dynamic, we don't want to copy back to the maplayer (it's already there)
               fieldCol = gpHydroModel->m_pCellLayer->GetFieldCol(pHydro->m_pDynamicMapInfo->fieldname);

			   if (gpMapWnd->m_updateClassificationExtent || pHydro->m_pDynamicMapInfo->maxVal == 0.0f)
				   {
               float minValCell, maxValCell;
               pHydro->m_pCellLayer->GetDataMinMax( fieldCol, &minValCell, &maxValCell );

               pHydro->m_pCellLayer->SetBins( minValCell, maxValCell, gpMapWnd->m_numBinUpland );		
		   
				   }
            else 
               pHydro->m_pCellLayer->SetBins(pHydro->m_pDynamicMapInfo->minVal, pHydro->m_pDynamicMapInfo->maxVal,10);              

			   }
		   if (pHydro->m_updateStream==TRUE)
			   {
			   int fieldCol = pHydro->GetFieldColFromInterfaceValue(0);
			   pHydro->CopyStateFromReachTreeToMapLayer(0,0);

			   if (gpMapWnd->m_updateClassificationExtent)
				   {
               float minValStream, maxValStream;
               pHydro->m_pStreamLayer->GetDataMinMax( fieldCol, &minValStream, &maxValStream );
               pHydro->m_pStreamLayer->SetBins( minValStream, maxValStream, gpMapWnd->m_numBin );	
				   }
			   }  
          pHydro->m_pCellLayer->ClassifyData(); 
	      pHydro->m_pStreamLayer->ClassifyData();

		  if ( gpMapWnd != NULL )
     	      gpMapWnd->RefreshList();
         
	      gpMapWnd->m_pMapList->RedrawWindow();
	      CDC *dc = gpMapWnd->m_pMap->GetDC();
	      gpMapWnd->m_pMap->DrawMap(*dc);
	      gpMapWnd->m_pMap->ReleaseDC(dc);
         }

         gpMapWnd->m_pScatterPrecip->UpdateCurrentPos( time );
		 if (gpHydroModel->m_checkPesticide)
			{
			gpMapWnd->m_pScatterDischarge->UpdateLine((time-gpHydroModel->m_startTime)-timeStep,gpHydroModel->m_pPestInstreamData->IGet(time-timeStep,1),  time-gpHydroModel->m_startTime, gpHydroModel->m_pPestInstreamData->IGet(time,1) );
			gpMapWnd->m_pScatterPrecip->PlotVerticalLine(gpHydroModel->m_startTime+gpHydroModel->m_spinUp);	 
			}
		 else if (gpHydroModel->m_cellDataToCollect == 0)
			gpMapWnd->m_pScatterDischarge->UpdateLine((time-gpHydroModel->m_startTime)-timeStep,gpHydroModel->m_pInstreamData->IGet(time-timeStep,1),  time-gpHydroModel->m_startTime, gpHydroModel->m_pInstreamData->IGet(time,1) );


	   else if ( gpHydroModel->m_cellDataToCollect == 3)
         {
         for (int k=0;  k <gpHydroModel->m_pSatDepthData->GetColCount();k++)
            gpMapWnd->m_pScatterDischarge->UpdateLine((time-gpHydroModel->m_startTime)-timeStep,gpHydroModel->m_pSatDepthData->IGet(time-timeStep,k),  time-gpHydroModel->m_startTime, gpHydroModel->m_pSatDepthData->IGet(time,k) );
         }
			

      gpMapWnd->m_pMap->m_name = s;
      gpMapWnd->m_pMap->ShowTitle(true);

	  if (pHydro->m_checkPesticide )
		{
		CString msg1 ;
		msg1.Format( "2.5 kg/ha Diuron Applied to Surface %i days from Start",pHydro->m_spinUp );
		CString msg2 = "Aqueous Concentration at 75 cm below application ";
		CString msg3 = "GRAY = LOWER [ ]";
		CString msg4 = "BLACK = HIGHER [ ]";




		CDC *pDc = gpMapWnd->m_pMap->GetDC();
		RECT rect;
		gpMapWnd->m_pMap->GetClientRect( &rect ); 
		pDc->SetTextColor(RGB(255,0,0));
        CFont   labelFont;
		labelFont.CreatePointFont(150,"Arial");
	    CGdiObject *pOldFont = pDc->SelectObject( &labelFont );
		
		pDc->TextOut( rect.right/2.0, rect.top+10, msg1 );
		pDc->TextOut( rect.right/2.0, rect.top+40, msg2 );

		CFont   labelFontBig;
		labelFontBig.CreatePointFont(200,"Arial");
	    CGdiObject *pOldFont1 = pDc->SelectObject( &labelFontBig );

		pDc->SetTextColor(RGB(0,0,0));
        pDc->TextOut( rect.right/1.8, rect.top+130, msg4 );
		pDc->SetTextColor(RGB(150,150,150,0,0));
        pDc->TextOut( rect.right/1.8, rect.top+160, msg3 );
		pDc->SetTextColor(RGB(0,0,0));


//////////////////////////////////////

//Draw verical line to separate map from the plots...

   int r2 = pDc->SetROP2( R2_COPYPEN );
   CPen pen( PS_SOLID, 10, RGB( 0, 0, 255 ) );
   CPen *pOldPen = pDc->SelectObject( &pen );

   float windowWidth = -rect.left + rect.right;
   float windowHeight = -rect.top + rect.bottom;

   pDc->MoveTo( (int) windowWidth/2.1, rect.bottom );
   pDc->LineTo( (int) windowWidth/2.1,rect.top );
   pDc->SetROP2( r2 );
   pDc->SelectObject( pOldPen );



////////////////////////////////////




		gpMapWnd->m_pMap->ReleaseDC(pDc);

		if (pHydro->m_pesticideApp)
			{
			CString msg = "! Surface Diuron Application Now  ";
			CDC *pDc = gpMapWnd->m_pMap->GetDC();
			RECT rect;
			gpMapWnd->m_pMap->GetClientRect( &rect ); 
            CFont   labelFont;
		    labelFont.CreatePointFont(200,"Arial");
	        CGdiObject *pOldFont = pDc->SelectObject( &labelFont );
			pDc->SetTextColor(RGB(255,0,0));
			pDc->TextOut( rect.left+60, rect.bottom-30, msg );
			pDc->SetTextColor(RGB(255,0,0));

			pDc->TextOut( rect.left+60, rect.bottom-30, msg );
			gpMapWnd->m_pMap->ReleaseDC(pDc);
			}
		else
			{
			CString msg = "! Surface Diuron Application Now  !";
			CDC *pDc = gpMapWnd->m_pMap->GetDC();
			RECT rect;
			gpMapWnd->m_pMap->GetClientRect( &rect ); 
            CFont   labelFont;
		    labelFont.CreatePointFont(200,"Arial");
	        CGdiObject *pOldFont = pDc->SelectObject( &labelFont );
			pDc->SetTextColor(RGB(255,255,255));
			pDc->TextOut( rect.left+60, rect.bottom-30, msg );
			pDc->SetTextColor(RGB(255,255,255));
			gpMapWnd->m_pMap->ReleaseDC(pDc);
			}		
		}	


		


	  // Output to video(s)
      // Lots of help from codeguru.earthweb.com
      if(pHydro->m_saveEmfOutput)
         {
         pHydro->WriteFrame();
         }
	  
      }

   if ( type == WH_UPDATE_MONTE_CARLO)
      {
      ///////////////////////////////////////////////////////////////
      // notify interface
      CString msg;
      COleDateTime t( time );
	  CString s = t.Format("%m/%d/%y:%H:%M" );
	  double timeStepHours = timeStep * 24;
	  int timeInt = int(timeStepHours);
	  double timeStepMinutes = ( timeStepHours - timeInt ) * 60;
	  msg.Format( "MONTE CARLO SIMULATION: Run number %i",  gpHydroModel->m_runNumber );
      gpMain->SetStatusMsg( msg );

      }

   // yield control to other processes
   MSG  msg;
   while( PeekMessage( &msg, NULL, NULL, NULL , PM_REMOVE ) )
        {
        TranslateMessage(&msg); 
        DispatchMessage(& msg); 
        } 

   return true;   
   }


MapWnd::MapWnd()
: m_pMap( NULL ),
  m_pMapList( NULL ),
  m_currentPoly( -1 ),
  m_showAlternative( true ),
  m_isResult( false ),
  m_lineWidth(30),
  m_minBinValue(0.0f),
  m_maxBinValue(1.20f), /*this is 0.05 for maimai*/
  m_numBin(15),
  m_minBinValueUpland(0.3f),
  m_maxBinValueUpland(0.50f),
  m_numBinUpland(10),
  m_updateClassificationExtent(false)
   {
   }


MapWnd::MapWnd( MapWnd &mapWnd )
:  m_pMap( mapWnd.m_pMap ),
   m_pMapList( NULL ),
   m_currentPoly( -1 ),
   m_showAlternative( true ),
   m_isResult( mapWnd.m_isResult )
   {
   }


MapWnd::~MapWnd()
{
if ( m_pMap != NULL )
   delete m_pMap;




}


BEGIN_MESSAGE_MAP(MapWnd, CWnd)
	//{{AFX_MSG_MAP(MapWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_SHOWRESULTS, OnShowresults)
	ON_UPDATE_COMMAND_UI(ID_SHOWRESULTS, OnUpdateShowresults)
	ON_WM_ACTIVATE()
	ON_COMMAND(ID_CELLPROPERTIES, OnCellproperties)
	ON_COMMAND(ID_CLEARALTERNATIVES, OnClearAlternatives)
	ON_COMMAND(ID_SELECTWATERSHED, OnSelectwatershed)
	ON_COMMAND(ID_RECLASSIFYDATA, OnReclassifydata)
	ON_WM_MOUSEACTIVATE()
	//}}AFX_MSG_MAP
   ON_COMMAND_RANGE(40000, 42000, OnSelectField)
   ON_COMMAND(45000, OnMapStatistics)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// MapWnd message handlers


int MapWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
   {
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
		
   CCreateContext context;
   context.m_pNewViewClass = RUNTIME_CLASS( CWet_hView );
   context.m_pCurrentDoc = NULL; //GetDocument();
   context.m_pNewDocTemplate = NULL;
   context.m_pLastView = NULL;

   // create the background splitter window
   m_splitterWnd.CreateStatic( this,1, 2, WS_CHILD | WS_VISIBLE, 101 );

	CRect rect;
	GetClientRect(&rect);
	CSize sz(100, rect.Height() );

   // create the left and right panes
   m_splitterWnd.CreateView( 0, 0, RUNTIME_CLASS( MapListView ), sz, &context );

   sz.cx = rect.Width()- 100;
   sz.cy = rect.Height();
   m_splitterWnd.CreateView( 0, 1, RUNTIME_CLASS( Map ), sz, &context );

   
   //new - Kellie 110904!
//   sz.cx = rect.Width()- 100;
//   sz.cy = rect.Height()/2;
//   m_splitterWnd.CreateView( 1, 1, RUNTIME_CLASS( Map ), sz, &context );


   // store pointers to the split windows
   m_pMapList = (MapListView*) m_splitterWnd.GetPane( 0, 0 );
   m_pMap = (Map*) m_splitterWnd.GetPane( 0, 1 );

 //  m_pMap2 = (Map*) m_splitterWnd.GetPane(1,1);

   m_pMap->InstallNotifyHandler( MapProc, (long) this );
   m_pMapList->m_pMap = m_pMap;

   // all done
   m_pMap->ShowWindow( SW_SHOW );
         
	return 0;
   }


void MapWnd::OnSize(UINT nType, int cx, int cy) 
   {
	CWnd::OnSize(nType, cx, cy);

   if ( IsWindow( m_splitterWnd.m_hWnd ) )
      m_splitterWnd.MoveWindow( -2, -2, cx+2, cy+2, TRUE );
   }


void MapWnd::OnEditCopy() 
   {
   if ( m_pMap != NULL )
      m_pMap->CopyMapToClipboard( 0 );
   }



void MapProc( Map *pMap, NOTIFY_TYPE type, int a0, long a1, long extra )
   {
   gpMapWnd = (MapWnd*) extra;

   switch( type )
      {
		case NT_UPDATEMAP:
			{

			
			}

      case NT_LOADVECTOR:
         if ( a0 > 0 )
            {
            if ( a0%100 == 0 )
               {
               char msg[ 32 ];
               lstrcpy( msg, "Loading vector: " );
               itoa( a0, msg+lstrlen( msg ), 10 );
               gpMain->SetStatusMsg( msg );
               }
            }
         else
            {
            char msg[ 32 ];
            lstrcpy( msg, "Loaded " );
            itoa( -a0, msg+lstrlen( msg ), 10 );
            lstrcat( msg, " vectors" );
            gpMain->SetStatusMsg( msg );
            }            
         break;

      case NT_LOADRECORD:
         if ( a0 > 0 )
            {
            if ( a0%100 == 0 )
               {
               char msg[ 32 ];
               lstrcpy( msg, "Loading record: " );
               itoa( a0, msg+lstrlen( msg ), 10 );
               gpMain->SetStatusMsg( msg );
               }
            }
         else
            {
            char msg[ 32 ];
            lstrcpy( msg, "Loaded " );
            itoa( -a0, msg+lstrlen( msg ), 10 );
            lstrcat( msg, " records" );
            gpMain->SetStatusMsg( msg );
            }            
         break;

      case NT_LOADGRIDROW:
         if ( a0 > 0 )  // a0 is the row number
            {
            if ( a0%100 == 0 )
               {
               char msg[ 32 ];
               lstrcpy( msg, "Loading row: " );
               itoa( a0, msg+lstrlen( msg ), 10 );
               gpMain->SetStatusMsg( msg );
               }
            }
         else
            {
            char msg[ 32 ];
            lstrcpy( msg, "Loaded " );
            itoa( -a0, msg+lstrlen( msg ), 10 );
            lstrcat( msg, " rows" );
            gpMain->SetStatusMsg( msg );
            }            
         break;

      case NT_LBUTTONDBLCLK:
         {
         MapLayer *pLayer = pMap->GetActiveLayer();

         if ( pLayer == NULL )
            {
            MessageBeep( 0 );
            gpMain->SetStatusMsg( "No Active Layer set..." );
            }
         else
            {
            Poly *pPoly = pLayer->FindPolygon( a0, a1 );

            if ( pPoly == NULL )
               {
               MessageBeep( 0 );
               gpMain->SetStatusMsg( "No polygon selected..." );            
               break;
               }

            CString msg;
            msg.Format( "%s - PolyID: %d   ", (LPCSTR) gpMapWnd->m_pMap->m_name, pPoly->m_id );

            int col = pLayer->GetActiveField();
            if ( col < 0 )
               msg += "(No active field)";
            else
               {
               CString value;
               pLayer->GetDataAtPoint( a0, a1, col, value );
               msg += pLayer->GetFieldLabel( col );
               msg += ": ";

               int colAltern  = pLayer->GetFieldCol( "Altern" );
               int colAltern2 = pLayer->GetFieldCol( "Altern_2" );
               int colAltern3 = pLayer->GetFieldCol( "Altern_3" );

               if ( col == colAltern || col == colAltern2 || col == colAltern3 )
                  {               
                  int id = atoi( value );
                  if ( id >= 0 )
                     {
                     Alternative *pAlt = gpDecisionManager->GetAlternativeFromID( id );
                     msg += pAlt->m_name;
                     msg += " (";
                     msg += value;
                     msg += ")";
                     }
                  }
               else
                  {
                  msg += value;
                  }
               }

            int row1 = 0;
            int col1 = 0;

            pLayer = pMap->GetLayer(1);
         POINT pt;
         pt.x = a0;
         pt.y = a1;
         pMap->DPtoLP( pt );
         float x, y;
         pMap->LPtoVP( pt, x, y );

            pLayer->GetGridCellFromCoord(x,y,row1, col1);
            gpHydroModel->ApplyTracerToGridCell(row1,col1);

            gpMain->SetStatusMsg( msg );
            }

         }
         break;

 			//  pick streams or cells to sample
			//  pick streams or cells to sample
      case NT_LBUTTONDOWN:
        {
        MapLayer *pLayer = pMap->GetActiveLayer();
        if ( pLayer == NULL )
            {
            MessageBeep( 0 );
            gpMain->SetStatusMsg( "No Active Layer set..." );
            break;
            }

			MapLayer *pCellLayer = gpMapWnd->m_pMap->GetLayer( 0 );
			MapLayer *pStreamLayer = gpMapWnd->m_pMap->GetLayer( "Streams" );

			DataObj *pCellData = pCellLayer->m_pData;
			ASSERT( pCellData != NULL );

			DataObj *pStreamData = pStreamLayer->m_pData;
			ASSERT( pStreamData != NULL );

			int colWatershedID = pCellData->GetCol("hydro_id"); 
			int colStreamID = pStreamData->GetCol("hydro_id");
			
			ASSERT( colWatershedID >= 0 );       // Did it work?

         ASSERT( gpWizUserInteraction != NULL );

         if ( gpMapWnd->m_pMap->GetLayer( 0 ) != NULL 
           && gpMapWnd->m_pMap->GetLayer( 1 ) != NULL
           && IsWindow( gpWizUserInteraction->m_ppUserInteractionSimControl ) )
            {
            int cellIndex;
            int row;
            int col;
            Poly *pCell = pCellLayer->FindPolygon(a0, a1, &cellIndex );
            if (gpHydroModel->m_modelStructureMethod==WH_DISTRIBUTE_FPGRID || gpHydroModel->m_modelStructureMethod==WH_DISTRIBUTE_WTHEIGHT)
               {
				   if ( gpWizUserInteraction->m_ppUserInteractionSimControl.IsDlgButtonChecked( IDC_SAMPLEREACHES))
					   {
                  int watershedID;
                  bool ok = pCellLayer->GetData( cellIndex, colWatershedID, watershedID );
                  ASSERT( ok );

                  // find corresponding reach 

                  int reachIndex = pStreamLayer->FindIndex( colStreamID, watershedID );
						ASSERT( reachIndex >= 0 );
                  gpWizUserInteraction->m_ppUserInteractionSimControl.AddReach( reachIndex );
						}
               else
                  {
                  REAL x; REAL y;
                  POINT pt;
                  pt.x = a0;
                  pt.y = a1;
                  gpHydroModel->m_pMap->DPtoVP(pt, x, y);              // convert to logical
                  gpHydroModel->m_pDEMLayer->GetGridCellFromCoord(x, y, row, col);
   				   if (gpWizUserInteraction->m_ppUserInteractionSimControl.IsDlgButtonChecked( IDC_SAMPLECELLS))
					      gpWizUserInteraction->m_ppUserInteractionSimControl.AddGridCell(col,row);
                  }
               }


//				Poly *pCell = pCellLayer->FindPolygon( a0, a1 );
//				cellIndex = pCell->m_id - 1;
            if ( pCell != NULL && gpHydroModel->m_modelStructureMethod !=WH_DISTRIBUTE_FPGRID && gpHydroModel->m_modelStructureMethod !=WH_DISTRIBUTE_WTHEIGHT) 
               {
               // sampling cells?
               // if ( gpView->m_runDlg.IsDlgButtonChecked( IDC_SAMPLECELLS ) )
               //    gpView->m_runDlg.AddCell( cellIndex );

   				if (gpWizUserInteraction->m_ppUserInteractionSimControl.IsDlgButtonChecked( IDC_SAMPLECELLS))
					   gpWizUserInteraction->m_ppUserInteractionSimControl.AddCell(cellIndex);

				   else if ( gpWizUserInteraction->m_ppUserInteractionSimControl.IsDlgButtonChecked( IDC_SAMPLEREACHES))
					   {
                  int watershedID;
                  bool ok = pCellLayer->GetData( cellIndex, colWatershedID, watershedID );
                  ASSERT( ok );

                  // find corresponding reach 

                  int reachIndex = pStreamLayer->FindIndex( colStreamID, watershedID );
						ASSERT( reachIndex >= 0 );
                  gpWizUserInteraction->m_ppUserInteractionSimControl.AddReach( reachIndex );
						}


					}

            }  // end of: is ( gpHydroModel->m_pRestoreMatrix != NULL .... )
         }  // end of:  if ( gpView->m_runDlg.m_hWnd != 0 ... )
 //     }  // endof switch:
		  break;

      case NT_RBUTTONDBLCLK:
         {
         MapLayer *pLayer = pMap->GetActiveLayer();

         if ( pLayer == NULL )
            {
            MessageBeep( 0 );
            gpMain->SetStatusMsg( "No Active Layer set..." );
            break;
            }
         int row=0;
         int col=0;
         pLayer->GetGridCellFromCoord(a0,a1,row, col);
         gpHydroModel->ApplyTracerToGridCell(row,col);

         }
      
      
      case NT_RBUTTONDOWN:   // pop up an context menu.
         {

         MapLayer *pLayer = pMap->GetActiveLayer();

         if ( pLayer == NULL )
            {
            MessageBeep( 0 );
            gpMain->SetStatusMsg( "No Active Layer set..." );
            break;
            }

         int index;
         Poly *pPoly = pLayer->FindPolygon( a0, a1, &index );
         
         gpMapWnd->m_currentPoly = index;

         CMenu mainMenu;
         CMenu *pPopup = NULL;

         CMenu uplandInfoMenu;
 //        if ( gpDoc->m_currentRun > 0 )
            uplandInfoMenu.CreatePopupMenu();

         //  if NOT over polygons, make a list of available fields
         if ( pPoly == NULL )
            {
            //MessageBeep( 0 );
            //gpMain->SetStatusMsg( "No polygon selected..." );

            mainMenu.CreatePopupMenu();
            pPopup = &mainMenu;

            CString activeLabel = pLayer->GetFieldLabel();

            // load up any field information registrered with the documents field info
            for ( int i=0; i < gpDoc->GetFieldInfoCount(); i++ )
               {
               WET_FIELD_INFO *pInfo = gpDoc->GetFieldInfo( i );
               UINT flag = MF_STRING;
               if ( activeLabel.CompareNoCase( pInfo->fieldname ) == 0 )
                  flag |= MF_UNCHECKED;

               if ( pLayer->GetFieldCol( pInfo->fieldname ) >= 0 )
                  {
                  // only use level 0 in the main menu
                  if ( pInfo->level == 0 )						
                     mainMenu.AppendMenu( flag, 40000+i,  (LPCTSTR) pInfo->label );

                  else if ( pInfo->level == 1 ) // anything greater than level 0
                     uplandInfoMenu.AppendMenu(flag, 40000+i, (LPCTSTR) pInfo->label );
						
                  }
               }

//            if ( gpDoc->m_currentRun > 0 )
//               {
               mainMenu.AppendMenu( MF_SEPARATOR ); 
               mainMenu.AppendMenu( MF_POPUP | MF_STRING, (UINT) uplandInfoMenu.m_hMenu, "UPLAND_INFO_STRUCTURE" );
               mainMenu.AppendMenu(MF_POPUP | MF_STRING, 883231, "Add Tracer" );
               uplandInfoMenu.Detach();
 //              mainMenu.AppendMenu( MF_STRING, 45000, "Map Statistics" );
//               }

            }
         else  // over a polygon, load menu from menu resource
            {
            // have polygon, pop up context menu
            mainMenu.LoadMenu( IDR_MAPCONTEXTMENU );   // load it from a menu resource
            pPopup = mainMenu.GetSubMenu( 0 );
            }

         ASSERT_VALID( pPopup );

         // convert points to screen coords
         CPoint point( a0, a1 );
         pMap->ClientToScreen( &point );

         // display and track the menu
         pPopup->TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, gpMapWnd, NULL );
         }
         break;

      case NT_LBUTTONUP:
         {
         //switch( gpView->m_mode )
            //{
            // for any of the zoom modes, if we're in a result window, zoom all results
            //case MM_ZOOMIN:
            //case MM_ZOOMOUT:
            //case MM_PAN:
            //   if ( IsWindow( gpMapWnd->m_hWnd ) )
            //      pMap->StartPan();
            //   break;
            //}

  //       gpView->m_mode = MM_NORMAL;
         }
         break;
      
      case NT_MOUSEMOVE:
         {
         POINT pt;
         pt.x = a0;
         pt.y = a1;
         pMap->DPtoLP( pt );
         float x, y;
         pMap->LPtoVP( pt, x, y );

         char msg[ 64 ];
         sprintf( msg, "(%f, %f)", x, y );
 //        gpMain->SetStatusMsg( msg );
         }
         break;

      case NT_EXTENTSCHANGED:
         {
         float vxMin, vxMax, vyMin, vyMax;
         pMap->GetViewExtents( vxMin, vxMax, vyMin, vyMax );
         CString msg1;
         msg1.Format( "View: (%g,%g)-(%g,%g)   ", vxMin, vyMin, vxMax, vyMax );

         pMap->GetMapExtents( vxMin, vxMax, vyMin, vyMax );
         CString msg2;
         msg2.Format( "Map: (%g,%g)-(%g,%g)", vxMin, vyMin, vxMax, vyMax );
         msg1 += msg2;
         gpMain->SetStatusMsg( msg1 );
         }
         break;

      case NT_CALCDIST:
         {
         CString msg1;
         msg1.Format( "Getting Distance for number %i of %i", a0, a1  );
         gpMain->SetStatusMsg( msg1 );
         }
         break;

      case NT_CLASSIFYING:
//         gpMain->SetStatusMsg( "Classifying data..." );
         break;
      }  // endof switch:
   }


BOOL MapWnd::OnNotify(WPARAM ctrlID, LPARAM lParam, LRESULT* pResult) 
   {
	return CWnd::OnNotify(ctrlID, lParam, pResult);
   }



void MapWnd::OnShowresults() 
   {
   CPropertySheet dlg( "Site Results", this, 0 );


   }


void MapWnd::OnUpdateShowresults(CCmdUI* pCmdUI) 
   {
   pCmdUI->Enable();
   }


void MapWnd::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
   {
	CWnd::OnActivate(nState, pWndOther, bMinimized);
	
   if ( nState != WA_INACTIVE )
   	gpMapWnd = this; 
   }



void MapWnd::OnCellproperties() 
   {
   MapLayer *pLayer = gpMapWnd->m_pMap->GetLayer( 0 );

   CellPropDlg1 dlg( pLayer, m_currentPoly, this );
   dlg.DoModal();
   }


void MapWnd::OnMapStatistics() 
   {
   MapLayer *pLayer = gpMapWnd->m_pMap->GetLayer( 0 );

   MapStatsDlg dlg( pLayer );
   dlg.DoModal();
   }



void MapWnd::OnClearAlternatives() 
   {
   MapLayer *pLayer = gpMapWnd->m_pMap->GetLayer( 0 );

   int colAltern = pLayer->GetFieldCol( "ALTERN" );
   ASSERT( colAltern >= 0 );

   int colAltern2 = pLayer->GetFieldCol( "ALTERN_2" );
   ASSERT( colAltern2 >= 0 );

   int colAltern3 = pLayer->GetFieldCol( "ALTERN_3" );
   ASSERT( colAltern3 >= 0 );

   int colScore = pLayer->GetFieldCol( "SCORE" );
   ASSERT( colScore >= 0 );

   float noDataValue = pLayer->GetNoDataValue();
   pLayer->SetColData( colAltern, noDataValue, false );
   pLayer->SetColData( colAltern2, noDataValue, false );
   pLayer->SetColData( colAltern3, noDataValue, false );
   pLayer->SetColData( colScore, 0.0f, true );

   m_pMap->RedrawWindow();
   }


void MapWnd::OnSelectField( UINT nID )
   {

   WET_FIELD_INFO *pFieldInfo = gpDoc->GetFieldInfo( nID - 40000 );
   if (gpHydroModel->m_pDynamicMapInfo !=NULL )
      {
      gpHydroModel->m_dataToMap = pFieldInfo->label;
      gpHydroModel->m_pDynamicMapInfo->minVal = pFieldInfo->minVal;
      gpHydroModel->m_pDynamicMapInfo->maxVal = pFieldInfo->maxVal;
      gpHydroModel->m_pDynamicMapInfo->fieldname = pFieldInfo->fieldname;
      }
   if ( pFieldInfo->level == 0 )	
	if ( pFieldInfo == NULL )
		return;
	MapLayer *pLayer = gpMapWnd->m_pMap->GetLayer( 0 );
	if ( pLayer == NULL )
		return;
	int activeField = pLayer->GetFieldCol( pFieldInfo->fieldname );
	pLayer->SetActiveField( activeField );
	if (pFieldInfo->level==0)	
		{

		// classify field      
		pLayer->ClearBins();

		// figure out which field it is
		int colAltern  = pLayer->GetFieldCol( "Altern" );
		int colAltern2 = pLayer->GetFieldCol( "Altern_2" );
		int colAltern3 = pLayer->GetFieldCol( "Altern_3" );

		if ( activeField == colAltern || activeField == colAltern2 || activeField == colAltern3 )
			{
			CStringArray values;
			pLayer->GetUniqueValues( colAltern, values );    // get unique restoration ID's

			for ( int i=0; i < values.GetSize(); i++ )
				{
				// convert restoration alternative identifier to an int
				int altID = atoi( values[ i ] );

				if ( altID >= 0 )
					{
					Alternative *pAlt = gpDecisionManager->GetAlternativeFromID( altID );

					COLORREF color = pAlt->m_color;
					pLayer->AddBin( color, pAlt->m_name, BT_NUMERIC, altID-0.1f, altID+0.1f );
					}
				}

		//	pLayer->AddNoDataBin();
			}

		else
			{
			// is it a lulc class?
			int lulcLevel = 0;
			if ( activeField == pLayer->GetFieldCol( "LULC_A" ) )
				lulcLevel = 1;
			else if ( activeField == pLayer->GetFieldCol( "LULC_B" ) )
				lulcLevel = 2;
			else if ( activeField == pLayer->GetFieldCol( "LULC_C" ) )
				lulcLevel = 3;

			if ( lulcLevel > 0 )    // is this a lulc field
				{
				LulcNode *pNode = gpDoc->m_lulcTree.GetRootNode();     // get the root

				while ( ( pNode = gpDoc->m_lulcTree.GetNextNode() ) != NULL )
					{
					if ( pNode->GetNodeLevel() == lulcLevel )
						{
						CString label;
						label.Format( "%s (%i)", pNode->m_name, pNode->m_id );

						pLayer->AddBin( pNode->m_color, label, BT_NUMERIC, pNode->m_id-0.1f, pNode->m_id+0.1f );
						}
					}
	//			pLayer->AddNoDataBin();
				}  // end of: ( lulc > 0 )

			else  // not an altern col, not an lulc col
				{
				// its not the altern col, is a column we have field info for?
				LPCSTR label = pLayer->GetFieldLabel();  // get label for active col

				WET_FIELD_INFO *pInfo = gpDoc->FindFieldInfo( label );

				if ( pInfo != NULL )    // found a field, now use it
					{
					pLayer->SetBinColorFlag( (BCFLAG) pInfo->displayFlags );

					switch( pInfo->type )
						{
						case TYPE_INT:
						case TYPE_BOOL:
							pLayer->SetUniqueBins( activeField );
							break;

						case TYPE_FLOAT:
						case TYPE_DOUBLE:
							{
							float minVal, maxVal;
							pLayer->GetDataMinMax( activeField, &minVal, &maxVal );
							pLayer->SetBins( minVal, maxVal, 20 );
							break;
							}
						}  // end of:  switch( pInfo->type )

					}   // end of: if ( pInfo != NULL )
				else
					{  // pInfo == NULL, check any remaining cases
					}
				}  // end of:  else ( not a lulc col )
			}  // end of:  else ( not a altern col )


		// any data to display?
		if ( pLayer->GetBinCount() > 0 )
			{
			//pLayer->ShowLegend();
			pLayer->ShowBinCount();
			pLayer->ClassifyData(); 
			}

		} //end of if level==0;

	else if (pFieldInfo->level==1) //This data must be grabbed from UPSLOPE_INFO, not from MapLayer
		{

		if (pFieldInfo->label=="Soil Water Content")
			gpHydroModel->CopyStateFromReachTreeToMapLayer(1,0);
		if (pFieldInfo->label=="ET")
			gpHydroModel->CopyStateFromReachTreeToMapLayer(1,1);
		if (pFieldInfo->label=="Air Temp")
			gpHydroModel->CopyStateFromReachTreeToMapLayer(1,2);
		if (pFieldInfo->label=="Unsat SWC")
			gpHydroModel->CopyStateFromReachTreeToMapLayer(1,3);
		if (pFieldInfo->label=="Saturated Depth")
			gpHydroModel->CopyStateFromReachTreeToMapLayer(1,4);
		if (pFieldInfo->label=="Saturation Deficit")
			gpHydroModel->CopyStateFromReachTreeToMapLayer(1,5);
		if (pFieldInfo->label=="Unsaturated Depth")
			gpHydroModel->CopyStateFromReachTreeToMapLayer(1,6);
		if (pFieldInfo->label=="Tracer Concentration")
			gpHydroModel->CopyStateFromReachTreeToMapLayer(1,7);
		if (pFieldInfo->label=="Sediment Output")
			gpHydroModel->CopyStateFromReachTreeToMapLayer(1,8);
		if (pFieldInfo->label=="Foliar Runoff Rate")
			gpHydroModel->CopyStateFromReachTreeToMapLayer(1,9);
		if (pFieldInfo->label=="Surf Pest")
			gpHydroModel->CopyStateFromReachTreeToMapLayer(1,10);
		if (pFieldInfo->label=="Unsat Pest")
			gpHydroModel->CopyStateFromReachTreeToMapLayer(1,11);
		if (pFieldInfo->label=="Sat Pest")
			gpHydroModel->CopyStateFromReachTreeToMapLayer(1,12);
		if (pFieldInfo->label=="Sat Pest Rate")
			gpHydroModel->CopyStateFromReachTreeToMapLayer(1,13);
		if (pFieldInfo->label=="Upland Identifier")
			gpHydroModel->CopyStateFromReachTreeToMapLayer(1,14);
		if (pFieldInfo->label=="Upslope UPSLOPE_INFO index")
			gpHydroModel->CopyStateFromReachTreeToMapLayer(1,15);
		if (pFieldInfo->label=="Upslope Area")
			gpHydroModel->CopyStateFromReachTreeToMapLayer(1,16);

      switch( pFieldInfo->type )
			{
			case TYPE_INT:
			case TYPE_BOOL:
				pLayer->SetUniqueBins( activeField );
				break;
			case TYPE_FLOAT:
			case TYPE_DOUBLE:
				{
				float minVal, maxVal;
				pLayer->GetDataMinMax( activeField, &minVal, &maxVal );
				pLayer->SetBins( minVal, maxVal, 20 );
				break;
				}
			}
		pLayer->SetBinColorFlag( (BCFLAG) pFieldInfo->displayFlags );
		pLayer->ShowBinCount();
		pLayer->ClassifyData(); 
		}

   RefreshList();

   m_pMap->RedrawWindow();
   }


void MapWnd::OnSelectwatershed() 
   {
   MapLayer *pLayer = gpMapWnd->m_pMap->GetLayer( 0 );

   int colReachID = pLayer->GetFieldCol( "GRID_CODE" );
   ASSERT( colReachID >= 0 );

   int reachID;
   pLayer->GetData( m_currentPoly, colReachID, reachID );

   int selectedCount = gpDoc->SelectWatershed( pLayer, reachID, colReachID );

   CString msg;
   msg.Format( "Selected %i cell draining into reach %i", selectedCount, reachID );
   MessageBox( msg );

   m_pMap->RedrawWindow();	
   }


void MapWnd::OnReclassifydata() 
   {
   MapLayer *pLayer = gpMapWnd->m_pMap->GetLayer( 0 );

   pLayer->ClassifyData( -1 );

   m_pMap->RedrawWindow();	
   }

int MapWnd::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message) 
   {
	SetWindowPos( &wndTop, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE );
	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
   }

ScatterWnd *MapWnd::AllocateScatter( DataObj *pScatterData, int value)
	{
   ASSERT( pScatterData != NULL );
   ScatterWnd *pScatterPlot = new ScatterWnd;
   RECT rect;
   m_pMap->GetClientRect( &rect );

   if (value==0)
      {
      int width = 5*rect.right/2;
      int height = rect.bottom/2.5;
      //rect.left = rect.right - 500;
      rect.right = rect.left + 500;
      rect.bottom = height + 4;
      rect.top = 4;
      pScatterPlot->Create( NULL, "Precipitation (m/day)", WS_CHILD | WS_BORDER | WS_VISIBLE | WS_CAPTION | WS_THICKFRAME, rect, gpMapWnd->m_pMap, 50000 );
      pScatterPlot->SetLabelText(YAXIS,"Precip. Rate");
	  pScatterPlot->ShowAxisScale(XAXIS,false);
	  }
   if (value==1)
      {
      RECT rect;
      m_pMap->GetClientRect( &rect );
      int width = 5*rect.right/2;
      int height = (rect.bottom)/2.5;
      int widthWindow = rect.bottom -rect.top;
    //  rect.left = rect.right-500/* - width - 4*/;
      rect.right = rect.left +500/*+ width*/;
      rect.bottom = widthWindow-100;
	  rect.top = rect.bottom-height+10;
	  if (gpHydroModel->m_checkPesticide)
		{
		pScatterPlot->Create( NULL, "Instream Pesticide Concentrations", WS_CHILD | WS_BORDER | WS_VISIBLE | WS_CAPTION | WS_THICKFRAME, rect, gpMapWnd->m_pMap, 50000 );
		pScatterPlot->SetLabelText(YAXIS,"Diuron (ug/l)");
		pScatterPlot->SetExtents(YAXIS,0.0f,0.04f);
		 pScatterPlot->xAxis.showLabel=true;
		 pScatterPlot->ShowAxisScale(XAXIS,true);
		}
	  else if (gpHydroModel->m_cellDataToCollect == 0)
		{
		pScatterPlot->Create( NULL, "Stream Water Discharge", WS_CHILD | WS_BORDER | WS_VISIBLE | WS_CAPTION | WS_THICKFRAME, rect, gpMapWnd->m_pMap, 50000 );
		pScatterPlot->SetLabelText(YAXIS,"Q (m3/s)");
		 pScatterPlot->xAxis.showLabel=true;
		 pScatterPlot->ShowAxisScale(XAXIS,true);
		}

	  else if (gpHydroModel->m_cellDataToCollect == 3)
		{
		pScatterPlot->Create( NULL, "Well Levels (Distance from ", WS_CHILD | WS_BORDER | WS_VISIBLE | WS_CAPTION | WS_THICKFRAME, rect, gpMapWnd->m_pMap, 50000 );
		pScatterPlot->SetLabelText(YAXIS,"Well Level (m)");
		 pScatterPlot->xAxis.showLabel=true;
		 pScatterPlot->ShowAxisScale(XAXIS,true);
		}
   }
  // if (gpHydroModel->m_checkPesticide)
	//	{
		pScatterPlot->SetXCol( pScatterData, 0 );
		
		for ( int i=1; i < (int) pScatterData->GetColCount(); i++ )
			pScatterPlot->AppendYCol( pScatterData, i );
		pScatterPlot->SetAxisDigits(YAXIS, 3);
		pScatterPlot->SetLegendHeight(125);
		
		pScatterPlot->SetLabelHeight(150);
	    pScatterPlot->yAxis.showLabel=true;
	   
//		}
/*   else
		{
		pScatterPlot->SetXCol( pScatterData, 0 );
		
		for ( int i=1; i < (int) pScatterData->GetColCount(); i++ )
			pScatterPlot->AppendYCol( pScatterData, i );
		pScatterPlot->SetAxisDigits(YAXIS, 3);
		pScatterPlot->SetLegendHeight(125);
		pScatterPlot->ShowAxisScale(XAXIS,false);
		pScatterPlot->SetLabelHeight(150);
	    pScatterPlot->yAxis.showLabel=true;
	    pScatterPlot->xAxis.showLabel=false;
		}

		*/
    return pScatterPlot;
	}

