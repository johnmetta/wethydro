// WizUserInteraction.cpp : implementation file
//

#include "stdafx.h"
#pragma hdrstop
#include "wet_h.h"
#include "WizUserInteraction.h"
#include "modelinteraction.h"
#include "interfaceinteraction.h"
#include "PPUserInteractionStucture.h"
#include "PPUserInteractionSimControl.h"
#include "PPUserInteractionTracer.h"
#include "PPUserInteractionSediment.h"
#include "PPUserInteractionET.h"
#include "PPLanduseChange.h"
#include "PPUserInteractionFlowMods.h"
#include "ppUserInteractionPesticide.h"

//#include "ModelEvaluation.h"

#include "wet_hydro.h"
#include "MapWnd.h"
#include "etParamHarDlg.h"
#include "etParamPMDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern WizUserInteraction *gpWizUserInteraction;
extern WET_Hydro *gpHydroModel;
extern MapWnd *gpMapWnd;
extern etParamHarDlg *gpEtParamHarDlg;
extern etParamPMDlg *gpEtParamPMDlg;

/////////////////////////////////////////////////////////////////////////////
// WizUserInteraction

IMPLEMENT_DYNAMIC(WizUserInteraction, CPropertySheet)

WizUserInteraction::WizUserInteraction(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
  
}

WizUserInteraction::WizUserInteraction(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	gpWizUserInteraction = this;

   AddPage( &m_interfaceInteractionPage );
	AddPage( &m_ppUserInteractionSimControl );
   

   // ET model Parameters

   m_latitude = gpHydroModel->m_latitude;
	m_ac = gpHydroModel->m_ac;
	m_airDensity = gpHydroModel->m_airDensity;
	m_bc = gpHydroModel->m_bc;
	m_bioGeoAbsorp = gpHydroModel->m_bioGeoAbsorp;
	m_extraTerrRadiation = gpHydroModel->m_extraTerrRadiation;
	m_clearFraction = gpHydroModel->m_clearFraction;
	m_cloudFraction = gpHydroModel->m_cloudFraction;
	m_gradientSatVapor = gpHydroModel->m_gradientSatVapor;
	m_heatCond = gpHydroModel->m_heatCond;
	m_heightOfHumid = gpHydroModel->m_heightOfHumid;
	m_heightOfWind = gpHydroModel->m_heightOfWind;
	m_shortWave = gpHydroModel->m_shortWave;
	m_psychrometric = gpHydroModel->m_psychrometric;
	m_relativeHumid = gpHydroModel->m_relativeHumid;
	m_satVapPres = gpHydroModel->m_satVapPres;
	m_stephanBoltz = gpHydroModel->m_stephanBoltz;
	m_specificHeat = gpHydroModel->m_specificHeat;
	m_storedE = gpHydroModel->m_storedE;
	m_windSpeed = gpHydroModel->m_windSpeed;

}

WizUserInteraction::~WizUserInteraction()
{
}


BEGIN_MESSAGE_MAP(WizUserInteraction, CPropertySheet)
	//{{AFX_MSG_MAP(WizUserInteraction)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// WizUserInteraction message handlers


void WizUserInteraction::OnIDOK()
	{

	}

void WizUserInteraction::OnDestroy() 
{
	CPropertySheet::OnDestroy();
	
	OnIDOK();	
}

BOOL WizUserInteraction::OnCommand(WPARAM wParam, LPARAM lParam) 
{
   switch( LOWORD( wParam ) )
      {
      case IDOK:  // map properties
			{
			m_interfaceInteractionPage.UpdateData(TRUE);
		//	m_modelInteractionPage.UpdateData(TRUE);
		//	m_ppUserInteractionStructure.UpdateData(TRUE);
			m_ppUserInteractionSimControl.UpdateData(TRUE);	
 
			gpHydroModel->m_collectionInterval = m_interfaceInteractionPage.m_collectionint;
         gpHydroModel->m_checkMassBalance = m_interfaceInteractionPage.m_updateClassificationExtent;

			gpHydroModel->m_streamDataToCollect = m_interfaceInteractionPage.m_radioq;
			gpHydroModel->m_cellDataToCollect = m_interfaceInteractionPage.m_radioswc;
         gpMapWnd->m_maxBinValue=m_interfaceInteractionPage.m_maxq;
         gpMapWnd->m_minBinValue=m_interfaceInteractionPage.m_minq;
         gpMapWnd->m_numBin = m_interfaceInteractionPage.m_numbin;
         gpHydroModel->m_saveEmfOutput = m_interfaceInteractionPage.m_saveEmfOutput; // added by MG
		   gpHydroModel->m_spinUp = m_interfaceInteractionPage.m_spinUp;
         gpHydroModel->m_updateStream = m_interfaceInteractionPage.m_updatestream;
         gpHydroModel->m_updateUpland = m_interfaceInteractionPage.m_updateupland;
         gpHydroModel->m_cellDataToCollect=m_interfaceInteractionPage.m_radioswc;


   /*      gpHydroModel->m_checkCKsat=m_modelEvaluation.m_ppModelEvalModelEvaluation.m_checkKSat;
         gpHydroModel->m_checkCFC=m_modelEvaluation.m_ppModelEvalModelEvaluation.m_checkFieldCapac;
         gpHydroModel->m_checkCInit=m_modelEvaluation.m_ppModelEvalModelEvaluation.m_checkInitSat;
         gpHydroModel->m_checkCK1= m_modelEvaluation.m_ppModelEvalModelEvaluation.m_checkK1;
         gpHydroModel->m_checkCKD=m_modelEvaluation.m_ppModelEvalModelEvaluation.m_checkKDepth;
         gpHydroModel->m_checkCPhi=m_modelEvaluation.m_ppModelEvalModelEvaluation.m_checkPhi;
         gpHydroModel->m_checkCSD=m_modelEvaluation.m_ppModelEvalModelEvaluation.m_checkSoilDepth;
         gpHydroModel->m_checkCVan=m_modelEvaluation.m_ppModelEvalModelEvaluation.m_checkVanGenuctN;
 */

         
         if (m_interfaceInteractionPage.m_updatestream)
				gpHydroModel->m_updateStream=TRUE;
			else
				gpHydroModel->m_updateStream=FALSE;

			if (m_interfaceInteractionPage.m_updateupland)
				gpHydroModel->m_updateUpland=TRUE;
			else
				gpHydroModel->m_updateUpland=FALSE;


			gpHydroModel->m_cellDataArray.RemoveAll();
			for ( int i=0; i < m_ppUserInteractionSimControl.m_cellList.GetCount(); i++ )
				{
				char buffer[ 64 ];
				m_ppUserInteractionSimControl.m_cellList.GetText( i, buffer );
            if (gpHydroModel->m_modelStructureMethod==WH_DISTRIBUTE_FPGRID || gpHydroModel->m_modelStructureMethod==WH_DISTRIBUTE_WTHEIGHT)
               {
				   int cell;int cell2;
				   int fields = sscanf( buffer, "%i ,%i", &cell, &cell2 );
               
				   //ASSERT( fields == 1 );

				   gpHydroModel->m_cellDataArray.Add( cell );
               gpHydroModel->m_cellDataArray.Add( cell2 );


               }
            else
               {
				   int cell;
				   int fields = sscanf( buffer, "%i", &cell );
				   ASSERT( fields == 1 );
               
				   gpHydroModel->m_cellDataArray.Add( cell );
               }
				}
			gpHydroModel->m_reachDataArray.RemoveAll();
			for ( i=0; i < m_ppUserInteractionSimControl.m_reachList.GetCount(); i++ )
				{
				char buffer[ 64 ];
				m_ppUserInteractionSimControl.m_reachList.GetText( i, buffer );

				int reach;
				int fields = sscanf( buffer, "%i", &reach );
				ASSERT( fields == 1 );

				gpHydroModel->m_reachDataArray.Add( reach );
				}

			gpHydroModel->SetDeltaX( gpHydroModel->m_deltaX );

			gpHydroModel->SetTimeStep( m_ppUserInteractionSimControl.m_initialTimeStep );

			CString start ,stop;
			int indexStart = m_ppUserInteractionSimControl.m_comboStartTime.GetCurSel();
			int indexStop = m_ppUserInteractionSimControl.m_comboStopTime.GetCurSel();
			m_ppUserInteractionSimControl.m_comboStartTime.GetLBText(indexStart,start);
			m_ppUserInteractionSimControl.m_comboStopTime.GetLBText(indexStop,stop);
			int stopMonth = atoi(stop.Left(2));
			int stopDay   = atoi(stop.Mid(3,2));
			int stopYear  = atoi(stop.Right(4));
			COleDateTime stopTime( stopYear, stopMonth, stopDay, 0, 0, 0 );

			int startMonth=atoi(start.Left(2));
			int startDay  = atoi(start.Mid(3,2));
			int startYear = atoi(start.Right(4));
			COleDateTime startTime( startYear, startMonth, startDay, 0, 0, 0 );

			gpHydroModel->SetStopTime( stopTime.m_dt );
         gpHydroModel->m_stopTimeHolder = stopTime.m_dt;
         
			gpHydroModel->SetStartTime( startTime.m_dt );
			gpHydroModel->m_useVariableStep = m_ppUserInteractionSimControl.m_useVariableStep ? true : false;
			gpHydroModel->m_timeStep=m_ppUserInteractionSimControl.m_initialTimeStep;
			gpHydroModel->m_rkvMinTimeStep=m_ppUserInteractionSimControl.m_minimumTimeStep;
			gpHydroModel->m_rkvMaxTimeStep=m_ppUserInteractionSimControl.m_maximumTimeStep;
			gpHydroModel->m_rkvTolerance=m_ppUserInteractionSimControl.m_errorTolerance;


			m_interfaceInteractionPage.SetModified(false);
		//	m_modelInteractionPage.SetModified(false);
		//	m_ppUserInteractionStructure.SetModified(false);
			m_ppUserInteractionSimControl.SetModified(false);	
      //   m_ppUserInteractionFlowMod.SetModified(false);
      //   m_ppUserInteractionTracer.SetModified(false);
			
			}
			ShowWindow(SW_HIDE);

         break;

      case IDCANCEL:
			{
			ShowWindow(SW_HIDE);
			}
			break;
      case 12321:
			{
			m_interfaceInteractionPage.UpdateData(TRUE);
			//m_modelInteractionPage.UpdateData(TRUE);
			//m_ppUserInteractionStructure.UpdateData(TRUE);
			m_ppUserInteractionSimControl.UpdateData(TRUE);		


			m_interfaceInteractionPage.SetModified(FALSE);
		//	m_modelInteractionPage.SetModified(FALSE);
		//	m_ppUserInteractionStructure.SetModified(FALSE);
			m_ppUserInteractionSimControl.SetModified(FALSE);	

			}
			break;
	}
	
//	return CPropertySheet::OnCommand(wParam, lParam);
	return true;	
}

BOOL WizUserInteraction::OnInitDialog() 
{
	m_bModeless = FALSE;
	BOOL bResult = CPropertySheet::OnInitDialog();

   m_bModeless = TRUE;

	int numPages = GetPageCount();
	for (int i=0; i< numPages; i++)
		SetActivePage(i);
	
	return bResult;
}
