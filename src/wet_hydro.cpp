// WET_Hydro.cpp: implementation of the WET_Hydro class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WET_Hydro.h"
#include <unitconv.h>
#include <math.h>
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#include <idataobj.h>
#include <fdataobj.h>
#include <map.h>
#include <maplayer.h>
#include "MapWnd.h"
#include "mainfrm.h"
#include "ResultPlotWnd.h"
#include "nrutil.h"
#include <DataStore.h>
#include "optimizeDlg.h"
#include "wet_hView.h"
#include ".\wet_hydro.h"

extern CMainFrame *gpMain;
extern MapWnd     *gpMapWnd;
extern ReachTree  *gpReachTree;
extern ResultPlotWnd *gpResultPlotWnd;
extern optimizeDlg *gpOptimize;

extern CWet_hView *gpView;    // pointer to the view to render to AVI

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

WET_Hydro::WET_Hydro()    // Calls the constructor, defined in the header
: m_pMap( NULL ),         // initializes m_pMap and m_pSlopeLayer to a null pointer
                          // these are protected members of the WET_Hydro class   
  m_pFlowDirLayer( NULL),
  m_pDEMLayer( NULL ),
  m_pWatershedLayer( NULL ),
  m_pCellLayer( NULL ),
  m_pStreamLayer( NULL ),
  m_pSurfaceWaterLayer( NULL ),
  m_pSubSurfaceWaterLayer( NULL ),
  m_pClimateData( NULL ),
  m_gridResolution(10.0f),
  m_pTracerData( NULL ),
  m_pTracerConcData( NULL ),
  m_pSwcData( NULL ),
  m_pRateData( NULL ),
  m_pPestVegData(NULL),
  m_pPestInstreamData(NULL),
  m_pPestSurfData(NULL),
  m_pPestUnsatData(NULL),
  m_pPestSatData(NULL),
  m_pPestMassBalanceData(NULL),
  m_pUnsatDepthData(NULL),
  m_pSatDepthData(NULL),
  m_pSatDeficitData(NULL),
  m_pTracerConcChannelData(NULL),
  m_pTracerMassChannelData(NULL),
  m_pModelClimateData( NULL ),
  m_pETData( NULL ),
  m_pInstreamSedimentData(NULL),
  m_pUplandSedimentData(NULL),
  m_pTotalSedimentOutflowData(NULL),
  m_pSedimentOutflowData(NULL),
  m_pInstreamData( NULL ),
  m_pMeasuredDischargeData( NULL ),
  m_pMeasuredEtData( NULL ),
  m_pMeasuredTracerInputData( NULL ),
  m_pGeneralData( NULL ),
  m_pMassBalanceData( NULL ),
  m_pUplandStorageData( NULL ),
  m_pSyntheticqData( NULL ),
  m_pReachStatistics( NULL ),
  m_pMonteCarloResults(NULL),
  m_pMonteCarloTracerTimeSeries(NULL),
  m_pMonteCarloQTimeSeries(NULL),
  m_pDataStore( NULL ),
  m_pSavedData( NULL ),
  m_pInitialConditions( NULL ),
  m_timeStep( 0.001f ),   // day;
  m_stopTime( 2.0f ),  // day
  m_startTime(0.0f ),
  m_timeOffset(0),
  m_curTimeStep(0.001f),
  m_maxReachDeltaX( 5000.0f ), // meters
  m_deltaX( 5000.0f),
  m_surfaceWaterThreshold( 50.0f ),  // mm
  m_beta( 1.0f ),
  m_ddExp( 1.67f ),                  // depth/discharge exponent (see Bedient and Huber, 1992,  pg276)
  m_collectDataCount(0),
  m_colArea(-1),
  m_colSlope( -1 ),
  m_colLengthSlope( -1 ),
  m_colAltern(-1),
  m_colZ( -1 ),
  m_colWatershedID( -1 ),
  m_colStreamID( -1 ),
  m_colAbbrev(-1),
  m_colLulc(-1),
  m_colSoilDepth(-1),
  m_colLulc_b(-1),
  m_colCentroid(-1),
  m_colBuffDist(-1),
  m_colSideOfStream(-1),
  m_colLtPrecip(-1),
  m_colOCarbon(-1),
  m_colKffact(-1),
  m_colAltLulc(-1),
  m_colAltArea(-1),
  m_colIsRiparian(-1),
  m_colGeoCode(-1),
  m_checkSideOfStream(FALSE),
  m_checkUnsaturated(FALSE),
  m_routeChannelWaterMethod(WH_NETWORK_EXPLICIT),
  m_checkApplySedOptions(FALSE),
  m_checkSimulateWetlands(FALSE),
  m_checkDistributeInitCond(FALSE),      /////////////////////////////////////
  m_saveEmfOutput(FALSE),
  m_useVariableStep( TRUE ),
  m_checkIncrFlow(FALSE),
  m_checkPesticide(FALSE),
  m_checkSnow(FALSE),
  m_checkGridMet(FALSE),
  m_radioPorosity(DRAIN_P_CONSTANT),
  m_drainShape(10.0f),
  m_incrFlowBegin(0.0f),
  m_incrFlowEnd(0.0f),
  m_incrFlowPercent(10.0f),
  m_rkvMaxTimeStep( 0.10f ), //0.02 is about 1 hour
  m_rkvMinTimeStep( 0.0001f ),
  m_rkvTolerance(   0.0000001f ),
  m_k1( NULL ),
  m_k2( NULL ),
  m_k3( NULL ),
  m_k4( NULL ),
  m_k5( NULL ),
  m_k6( NULL ),
  m_derivative( NULL ),
  m_svTemp( NULL ),
  m_stateVar( NULL ),
  m_stateVarStream(NULL),
  m_svCount(0),
  m_svCountStream(0),
  m_uplandUnitCount(0),
  m_simulationTime(0.0f),
  m_runNumber( 0 ),
  m_optNumber( 0 ),
  m_et(0.001f),
  m_stationLtPrecip(0.02f),
  m_collectionInterval(0.5f),
  m_updateStream(TRUE),
  m_updateUpland(FALSE),
  m_streamDataToCollect(0),
  m_cellDataToCollect(TRUE),
  m_maxCentroidDistance(1000.0f),
  m_numBands(2),
  m_ksAsParam(TRUE),
  m_sdAsParam(TRUE),
  m_checkPrism(FALSE),
  m_checkUseSoilList(TRUE), 
  m_checkMonteCarlo(WH_SINGLE_SIMULATION ),
  m_numIterations(100),
  m_editTracerMgL(0.0f),
  m_initTracerConc(0.0f),
  m_radioTracerInput(0),
  m_checkTracerModel(FALSE),
  m_checkEtModel(FALSE),
  m_checkInterception(TRUE),
  m_checkMassBalance(TRUE),
  m_routeChannelSolutesMethod(WH_NETWORK_SUM),
  m_extra(0),
  m_extra1(0),
  m_checkUplandSediment(FALSE),
  m_reduceSedPercent(20.0f),
  m_bufferWidth(50.0f),
  m_checkModelTileDrains(FALSE),
  m_checkTill(FALSE),
  m_checkSoilDepthMap(TRUE),
  m_tileOutflowCoef(2.0f),
  m_tileThreshold(30.0f),
  m_checkThreeBasins(FALSE),
  m_etMethod(ET_HARGREAVES),
  m_satMethod(SAT_TOPMODEL),
  m_unsatMethod(UNSAT_BROOKSCOREY),
  m_modelParamMethod(WH_PARAM_WATERSHEDS),
  m_modelStructureMethod(WH_DISTRIBUTE_WATERSHEDS),
  m_areaTotal(0.0f),
  m_tempPrimet(10.0f),
  m_latitude(45.0f),
  m_ac(0.34f),
  m_airDensity(1.2f),
  m_bc(-0.14f),
  m_bioGeoAbsorp(0.02f),
  m_extraTerrRadiation(118.104f),
  m_clearFraction(0.5f),
  m_cloudFraction(0.25f),
  m_gradientSatVapor(0.189f),
  m_heatCond(0.0f), 
  m_heightOfHumid(2.0f),
  m_heightOfWind(2.0f),
  m_shortWave(0.0f),
  m_psychrometric(0.5f),
  m_relativeHumid(0.5f),
  m_satVapPres(3.169f),
  m_stephanBoltz(0.0000004903f),
  m_specificHeat(1.103f),
  m_storedE(0.0f),
  m_windSpeed(1.0f),
  m_rtIntegral(0.0f),
  /*m_dataToMap("Sat Pest Rate")*/m_dataToMap("Saturated Depth"),
  m_pDynamicMapInfo(NULL),
  m_randUnif1(),
  m_randUnif2(),
  m_randUnif3(),
  m_numberPesticides(0),
  m_pesticideOffsets(),
  m_radar(FALSE),
  AVIGenerators(0)   // initialize to null pointer

   {                      
   
   }


WET_Hydro::~WET_Hydro()  //The destructor
   {
   if ( m_pClimateData != NULL )
      delete m_pClimateData;


   if ( m_pTracerData != NULL )
      delete m_pTracerData;

   if ( m_pSwcData != NULL )
      delete m_pSwcData;
   if ( m_pRateData != NULL )
      delete m_pRateData;

   if ( m_pPestVegData != NULL )
      delete m_pPestVegData;

   if ( m_pPestInstreamData != NULL )
      delete m_pPestInstreamData;

   if ( m_pPestSurfData != NULL )
      delete m_pPestSurfData;
   
   if ( m_pPestUnsatData != NULL )
      delete m_pPestUnsatData;
   
   if ( m_pPestSatData != NULL )
      delete m_pPestSatData;

   if ( m_pPestMassBalanceData != NULL )
      delete m_pPestMassBalanceData;

   if ( m_pUnsatDepthData != NULL )
      delete m_pUnsatDepthData;

   if ( m_pSatDepthData != NULL )
      delete m_pSatDepthData;

   if ( m_pSatDeficitData != NULL )
      delete m_pSatDeficitData;

   if ( m_pTracerConcChannelData != NULL )
      delete m_pTracerConcChannelData;
   if ( m_pTracerMassChannelData != NULL )
      delete m_pTracerMassChannelData;

   if ( m_pTracerConcData != NULL )
      delete m_pTracerConcData;

   if ( m_pModelClimateData != NULL )
      delete m_pModelClimateData;

   if ( m_pETData != NULL )
      delete m_pETData;

   if ( m_pInstreamData != NULL )
      delete m_pInstreamData;

   if ( m_pInstreamSedimentData != NULL )
      delete m_pInstreamSedimentData;

   if ( m_pUplandSedimentData != NULL )
      delete m_pUplandSedimentData;
   
   if ( m_pTotalSedimentOutflowData != NULL )
      delete m_pTotalSedimentOutflowData;

   if ( m_pSedimentOutflowData != NULL )
      delete m_pSedimentOutflowData;

   if ( m_pGeneralData != NULL )
      delete m_pGeneralData;

   if ( m_pMassBalanceData != NULL )
      delete m_pMassBalanceData;

   if ( m_pUplandStorageData != NULL )
      delete m_pUplandStorageData;

   if ( m_pMeasuredDischargeData != NULL )
      delete m_pMeasuredDischargeData;
	
	if ( m_pMeasuredEtData != NULL )
		delete m_pMeasuredEtData;

	if ( m_pMeasuredTracerInputData != NULL )
		delete m_pMeasuredTracerInputData;

	if ( m_pSyntheticqData != NULL )
		delete m_pSyntheticqData;

	if ( m_pReachStatistics != NULL )
		delete m_pReachStatistics;

   if ( m_pMonteCarloTracerTimeSeries != NULL )
		delete m_pMonteCarloTracerTimeSeries;
   if ( m_pMonteCarloQTimeSeries!= NULL )
		delete m_pMonteCarloQTimeSeries;
   if ( m_pMonteCarloResults != NULL )
		delete m_pMonteCarloResults;
   if (m_pDynamicMapInfo != NULL)
      delete m_pDynamicMapInfo;
   if (m_pMap != NULL)
	   delete m_pMap;

   if(AVIGenerators != 0)
      {
      for(int i = 0; i < numMovies; i++)
         {
         delete AVIGenerators[i];
         }
      delete [] AVIGenerators;
      }
  }

int WET_Hydro::GetFieldColFromInterfaceValue(int model)
	{
	int colToDisplay = -1;
   if (model == 0)
      colToDisplay = m_pStreamLayer->GetFieldCol("EXTRA_1") ;
   if (model == 1)
      colToDisplay = m_pCellLayer->GetFieldCol("EXTRA_1") ;
	return colToDisplay;
	}


/////////////////////////////////////////////////////////////////////////////////////////////
//
// Copies modeled state data into maplayers.  Useful when interacting with the interface.
// The boolean 'model' variable indicates whether stream or cells are being updated (1=stream, 0=cell)
// interfaceValue comes from the radio buttons in the interactionwizard. 
//
/////////////////////////////////////////////////////////////////////////////////////////////


int WET_Hydro::CopyStateFromReachTreeToMapLayer(int model, int stateVar)
	{					
      
	if (model == 0) 
	   {
      int colToSet = m_pStreamLayer->GetFieldCol("EXTRA_1") ;
		for ( int i=0; i < m_pStreamLayer->m_polyArray.GetSize(); i++ )
			{
			ReachNode *pNode = m_reachTree.m_nodeArray[i];
			REACH_INFO_HYDRO *pHydro = (REACH_INFO_HYDRO*) pNode->m_reachInfo.pData;
			int j = pNode->m_reachInfo.subnodeCount;
			if (stateVar == 0)
			//	m_pStreamLayer->SetData(i, colToSet ,pHydro->instreamPesticideArray[0]->pesticideConc*1E6f);
				
			m_pStreamLayer->SetData(i, colToSet ,pNode->m_reachInfo.qArray[ j-1 ]);
			else if (stateVar == 1)
				m_pStreamLayer->SetData(i, colToSet ,pNode->m_reachInfo.qArray[ j-1 ]);
			//	m_pStreamLayer->SetData(i, colToSet ,pHydro->instreamPesticideArray[0]->pesticideConc*1E6f);

			}
		}
	else if (m_modelStructureMethod != WH_DISTRIBUTE_FPGRID && m_modelStructureMethod != WH_DISTRIBUTE_WTHEIGHT)// for upland areas, case 0 means display volumetric swc
		{

      int colToSet = m_pCellLayer->GetFieldCol("EXTRA_1") ;
      ASSERT(colToSet != -1);
		for ( int i=0; i < m_pStreamLayer->m_polyArray.GetSize(); i++ )
			{
   		REACH_INFO_HYDRO *pHydroData = m_reachInfoHydroArray[ i ];
			for ( int j=0; j < pHydroData->upslopeInfoArray.GetSize(); j++)
				{
				UPSLOPE_INFO *pUpslope = pHydroData->upslopeInfoArray[ j ];
				for ( int k=0; k<pUpslope->cellsRepresentedArray.GetSize();k++)
               {
               if (stateVar==0)
                  m_pCellLayer->SetData(pUpslope->cellsRepresentedArray[k], colToSet , pUpslope->sdmm);
               else if (stateVar==1)
                  m_pCellLayer->SetData(pUpslope->cellsRepresentedArray[k], colToSet , pUpslope->currentET);  
               else if (stateVar==2)
                  m_pCellLayer->SetData(pUpslope->cellsRepresentedArray[k], colToSet , pUpslope->currentAirTemp);
               else if (stateVar==3)
                  m_pCellLayer->SetData(pUpslope->cellsRepresentedArray[k], colToSet , pUpslope->pondedVolume);
               else if (stateVar==4)
                  m_pCellLayer->SetData(pUpslope->cellsRepresentedArray[k], colToSet , pUpslope->saturatedDepth);
               else if (stateVar==5)
                  m_pCellLayer->SetData(pUpslope->cellsRepresentedArray[k], colToSet , pUpslope->sdmm);
               else if (stateVar==6)
                  m_pCellLayer->SetData(pUpslope->cellsRepresentedArray[k], colToSet , pUpslope->areaTotal/1000000);
               else if (stateVar==7)
                  m_pCellLayer->SetData(pUpslope->cellsRepresentedArray[k], colToSet , pUpslope->tracerConc);
               else if (stateVar==8)
                  m_pCellLayer->SetData(pUpslope->cellsRepresentedArray[k], colToSet , pUpslope->sedimentOutflowMass);
               else if (stateVar==9)
                  m_pCellLayer->SetData(pUpslope->cellsRepresentedArray[k], colToSet , pUpslope->pesticideArray[0]->pVeg->rateFoliarRunoff);
               else if (stateVar==10)
                  m_pCellLayer->SetData(pUpslope->cellsRepresentedArray[k], colToSet , pUpslope->pesticideArray[0]->pSurfWater->concentration);
               else if (stateVar==11)
                  m_pCellLayer->SetData(pUpslope->cellsRepresentedArray[k], colToSet , pUpslope->pesticideArray[0]->pUnsatWater->concentration);
               else if (stateVar==12)
                  m_pCellLayer->SetData(pUpslope->cellsRepresentedArray[k], colToSet , pUpslope->pesticideArray[0]->pSatWater->concentration);
               else if (stateVar==13)
                  m_pCellLayer->SetData(pUpslope->cellsRepresentedArray[k], colToSet , pUpslope->pesticideArray[0]->pSatWater->concentration);
               else if (stateVar==14)
                  m_pCellLayer->SetData(pUpslope->cellsRepresentedArray[k], colToSet , pUpslope->index);
               else if (stateVar==15)
                  m_pCellLayer->SetData(pUpslope->cellsRepresentedArray[k], colToSet , pUpslope->pUpslope->index);
               else if (stateVar==16)
                  m_pCellLayer->SetData(pUpslope->cellsRepresentedArray[k], colToSet , pUpslope->areaTotal);

               }
				}
         }
      }
   else 
      {	  
		int colToSet = m_pCellLayer->GetFieldCol("EXTRA_1") ;
		for ( int i=0; i < m_pStreamLayer->m_polyArray.GetSize(); i++ )
			{
   		REACH_INFO_HYDRO *pHydroData = m_reachInfoHydroArray[ i ];
         int size = pHydroData->upslopeInfoArray.GetSize();
			for ( int j=0; j < pHydroData->upslopeInfoArray.GetSize(); j++)
				{
				UPSLOPE_INFO *pUpslope = pHydroData->upslopeInfoArray[ j ];
			//	m_pFlowDirLayer->SetData(pUpslope->rowIndex,pUpslope->colIndex, pUpslope->pesticideArray[0]->pSatWater->concentration);
				m_pFlowDirLayer->SetData(pUpslope->rowIndex,pUpslope->colIndex,  pUpslope->saturatedDepth);

                }
            }
		m_pFlowDirLayer->SetBins(0.0f,40.0f,20);//for pesti concent

		m_pFlowDirLayer->SetBins(0.4f,1.6,20);
		m_pFlowDirLayer->SetBinColorFlag(BCF_GRAY);
      }
   
      
   return 1;
   }


int WET_Hydro::LoadClimateData( LPCSTR filename )
   {
   if ( m_pClimateData != NULL )  // is there already a climate data obj?
      delete m_pClimateData;

   m_pClimateData = new FDataObj;   // create a new one
   ASSERT( m_pClimateData != NULL );

   int records = (int) m_pClimateData->ReadAscii( filename, ',', TRUE );

   return records;
   }



int WET_Hydro::LoadMeasuredDischargeData( LPCSTR filename )
   {
   if ( m_pMeasuredDischargeData != NULL )  // is there already a climate data obj?
      delete m_pMeasuredDischargeData;

   m_pMeasuredDischargeData = new FDataObj;   // create a new one
   ASSERT( m_pMeasuredDischargeData != NULL );

   //int records = (int) m_pMeasuredDischargeData->ReadAscii( filename, ',', TRUE );
   int records = (int) m_pMeasuredDischargeData->ReadXls( filename );
	if ( records==0)
		{
		CFileDialog dlg( 1, "csv", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		"comma delimited|*.csv|All files|*.*||");

			if ( dlg.DoModal() == IDOK )
				//int records = (int) m_pMeasuredDischargeData->ReadAscii( dlg.GetPathName(), ',', TRUE );			
            int records = (int) m_pMeasuredDischargeData->ReadXls( filename );
		
      }	
   m_pMeasuredDischargeData->AppendCol("Modeled Q");

   return records;
   }
int WET_Hydro::LoadMeasuredEtData( LPCSTR filename )
   {
   if ( m_pMeasuredEtData != NULL )  // is there already a climate data obj?
      delete m_pMeasuredEtData;

   m_pMeasuredEtData = new FDataObj;   // create a new one
   ASSERT( m_pMeasuredEtData != NULL );

   int records = (int) m_pMeasuredEtData->ReadAscii( filename, ',', TRUE );

   return records;
   }
int WET_Hydro::LoadTracerInputData( LPCSTR filename )
   {
   if ( m_pMeasuredTracerInputData != NULL )  // is there already a climate data obj?
      delete m_pMeasuredTracerInputData;

   m_pMeasuredTracerInputData = new FDataObj;   // create a new one
   ASSERT( m_pMeasuredTracerInputData != NULL );

   int records = (int) m_pMeasuredTracerInputData->ReadAscii( filename, ',', TRUE );

   return records;
   }

int WET_Hydro::LoadParameters(LPCSTR filename)
	{
   FILE *fp = fopen( filename, "rt" );

   if ( fp == NULL )
      {
      CString msg;
      msg.Format( "Unable to open file %s", filename );
      ErrorMsg( msg );
      return -1;
      }

 //  while ( ! feof( fp ) )
   //   {
   //   if ( fgets( buffer, 511, fp ) == NULL )
   //      break;
      int count = fscanf(fp,"%le,%le,%le,%le,%le,%le/n",&m_timeStep,&m_stopTime,&m_startTime,&m_rkvMaxTimeStep,&m_rkvMinTimeStep,&m_rkvTolerance);
      count = fscanf(fp,"%f,%i,%f,%i/n",&m_latitude,&m_checkEtModel,&m_maxReachDeltaX,&m_checkSideOfStream);
      count = fscanf(fp,"%i,%i,%i,%i,%i,%i/n",&m_routeChannelWaterMethod,&m_checkApplySedOptions,&m_checkPesticide,&m_saveEmfOutput,&m_useVariableStep,&m_modelParamMethod);
      count = fscanf(fp,"%f,%f,%f,%f,%i,%i/n",&m_incrFlowPercent,&m_et,&m_stationLtPrecip,&m_collectionInterval,&m_updateStream,&m_updateUpland);
      count = fscanf(fp,"%f,%i,%i,%i,%i,%i/n",&m_maxCentroidDistance,&m_numBands,&m_ksAsParam,&m_sdAsParam,&m_modelStructureMethod,&m_checkUseSoilList);
      count = fscanf(fp,"%i,%i,%f,%f,%i,%i/n",&m_checkMonteCarlo,&m_numIterations,&m_editTracerMgL,&m_initTracerConc,&m_radioTracerInput,&m_checkTracerModel);
      count = fscanf(fp,"%i,%i,%i,%f,%f,%i/n",&m_routeChannelSolutesMethod,&m_checkUplandSediment,&m_checkUplandSediment,&m_reduceSedPercent,&m_bufferWidth,&m_checkModelTileDrains);
      count = fscanf(fp,"%f,%f,%i/n",&m_tileOutflowCoef,&m_tileThreshold,&m_etMethod);   

      int cellArraySize ;
      count = fscanf(fp,"%i/n",&cellArraySize);

      m_cellDataArray.SetSize(cellArraySize);
      
      for(int i=0; i<cellArraySize;i++)
         {
         int value=-1;
         count = fscanf(fp,"%i,",&value);
         m_cellDataArray[i] = value;
         }

      int reachArraySize ;
      count = fscanf(fp,"\n%i\n",&reachArraySize);
      m_reachDataArray.SetSize(reachArraySize);

      for(i=0; i<reachArraySize;i++)
         {
         int value=-1;
         count = fscanf(fp,"%i,",&value);
         m_reachDataArray[i]=value;
         }
   
   //   }

   fclose( fp );

   return 1;	
	}

int WET_Hydro::SaveParameters(LPCSTR filename)
	{
   FILE *fp = fopen( filename, "w" );

   if ( fp == NULL )
      {
      CString msg;
      msg.Format( "Unable to open file %s", filename );
      ErrorMsg( msg );
      return -1;
      }
      int count = fprintf(fp,"%le,%le,%le,%le,%le,%le\n",m_timeStep,m_stopTime,m_startTime,m_rkvMaxTimeStep,m_rkvMinTimeStep,m_rkvTolerance);

      count = fprintf(fp,"%f,%i,%f,%i\n",m_latitude,m_checkEtModel,m_maxReachDeltaX,m_checkSideOfStream);
      count = fprintf(fp,"%i,%i,%i,%i,%i,%i\n",m_routeChannelWaterMethod,m_checkApplySedOptions,m_checkPesticide,m_saveEmfOutput,m_useVariableStep,m_modelParamMethod);
      count = fprintf(fp,"%f,%f,%f,%f,%i,%i\n",m_incrFlowPercent,m_et,m_stationLtPrecip,m_collectionInterval,m_updateStream,m_updateUpland);
      count = fprintf(fp,"%f,%i,%i,%i,%i,%i\n",m_maxCentroidDistance,m_numBands,m_ksAsParam,m_sdAsParam,m_modelStructureMethod,m_checkUseSoilList);
      count = fprintf(fp,"%i,%i,%f,%f,%i,%i\n",m_checkMonteCarlo,m_numIterations,m_editTracerMgL,m_initTracerConc,m_radioTracerInput,m_checkTracerModel);
      count = fprintf(fp,"%i,%i,%i,%f,%f,%i\n",m_routeChannelSolutesMethod,m_checkUplandSediment,m_checkUplandSediment,m_reduceSedPercent,m_bufferWidth,m_checkModelTileDrains);
      count = fprintf(fp,"%f,%f,%i\n",m_tileOutflowCoef,m_tileThreshold,m_etMethod);

      int cellArraySize = m_cellDataArray.GetSize();
      count = fprintf(fp,"%i\n",cellArraySize);

      for(int i=0; i<cellArraySize;i++)
         count = fprintf(fp,"%i,",m_cellDataArray[i]);

      int reachArraySize = m_reachDataArray.GetSize();
      count = fprintf(fp,"\n%i\n",reachArraySize);

      for(i=0; i<m_reachDataArray.GetSize();i++)
         count = fprintf(fp,"%i,",m_reachDataArray[i]);

   fclose( fp );

   return 1;	
	}

//--------------------------------------------------------------------------------------
// WET_Hydro::LoadSoilData
//
//  
// This method reads a file that contains soil dependent parameters.  The most notable of these parameters is the
// c value that defines how Ks changes with changes in theta.
//
//.The soil data is a text file, each line of the file is converted to a structure (SOIL INFO) and this structure
//  is stored as an array (m_soilInfoArray).  
//
//--------------------------------------------------------------------------------------


int WET_Hydro::LoadSoilData( LPCSTR filename )
   {
   for ( int i=0; i < m_soilInfoArray.GetSize(); i++ )
      delete m_soilInfoArray[ i ];

   m_soilInfoArray.RemoveAll(); 

   FILE *fp = fopen( filename, "rt" );

   if ( fp == NULL )
      {
      CString msg;
      msg.Format( "Unable to open file %s", filename );
      ErrorMsg( msg );
      return -1;
      }

   char buffer[ 128 ];
   while ( ! feof( fp ) )
      {
      if ( fgets( buffer, 127, fp ) == NULL )
         break;

      SOIL_INFO *pSoil = new SOIL_INFO;
      m_soilInfoArray.Add( pSoil );

      int count = sscanf( buffer, "%[^,], %[^,], %f, %f, %f, %f, %f", &pSoil->texture, &pSoil->abbrev, &pSoil->c, &pSoil->phi, &pSoil->kSat, &pSoil->wiltPt, &pSoil->fieldCapac );

      if ( count != 7 )
         break;
      }

   fclose( fp );

   return m_soilInfoArray.GetSize();
   }


//--------------------------------------------------------------------------------------
// WET_Hydro::LoadSoilData
//
//  
// This method reads a file that contains soil dependent parameters.  The most notable of these parameters is the
// c value that defines how Ks changes with changes in theta.
//
//.The soil data is a text file, each line of the file is converted to a structure (SOIL INFO) and this structure
//  is stored as an array (m_soilInfoArray).  
//
//--------------------------------------------------------------------------------------


int WET_Hydro::LoadAuxillaryModelData( LPCSTR filename )
   {
   for ( int i=0; i < m_auxillaryModelInfoArray.GetSize(); i++ )
      delete m_auxillaryModelInfoArray[ i ];

   m_auxillaryModelInfoArray.RemoveAll(); 

   FILE *fp = fopen( filename, "rt" );

   if ( fp == NULL )
      {
      CString msg;
      msg.Format( "Unable to open file %s", filename );
      ErrorMsg( msg );
      return -1;
      }

   char buffer[ 128 ];
   while ( ! feof( fp ) )
      {
      if ( fgets( buffer, 127, fp ) == NULL )
         break;

      AUXILLARY_MODEL_INFO *pAuxillaryModel = new AUXILLARY_MODEL_INFO;
      m_auxillaryModelInfoArray.Add( pAuxillaryModel );

      int count = sscanf( buffer, "%[^,], %i, %f, %f", &pAuxillaryModel->name, &pAuxillaryModel->landuse, &pAuxillaryModel->c, &pAuxillaryModel->p );

      if ( count != 4 )
         break;
      }

   fclose( fp );

   return m_auxillaryModelInfoArray.GetSize();
   }

//*****************************************************************************************************************
// WET_Hydro::GetAuxillaryModelParameters( int cell)
//
// This method is called within the upslope model, and is passed the current cell (which is a j in cell model)
//
// The method compares records in the cell model to each record in the soil parameter file, and sets member variables
//  to the values in the soil parameter table.  This is essentially a 'Join'.

//  These are c and p values for the USLE
//
//*****************************************************************************************************************

AUXILLARY_MODEL_INFO *WET_Hydro::GetAuxillaryModelParameters( int cell )
   {
	DataObj *pData = m_pCellLayer->m_pData;

   ASSERT( pData != NULL );

   int lulc;
   bool ok = m_pCellLayer->GetData( cell, m_colLulc, lulc );
   for (int j=0; j < m_auxillaryModelInfoArray.GetSize(); j++ )
      {
	   AUXILLARY_MODEL_INFO *pAuxillaryModel = m_auxillaryModelInfoArray.GetAt( j );

      if (pAuxillaryModel->landuse == lulc)
         return pAuxillaryModel;
      }
   return NULL;
   }




//*****************************************************************************************************************
// WET_Hydro::GetSoilParameters( int cell)
//
// This method is called within the upslope model, and is passed the current cell (which is a j in cell model)
//
// The method compares records in the cell model to each record in the soil parameter file, and sets member variables
//  to the values in the soil parameter table.  This is essentially a 'Join'.
//
//*****************************************************************************************************************

SOIL_INFO *WET_Hydro::GetSoilParameters( int cell )
   {
	DataObj *pData = m_pCellLayer->m_pData;

   ASSERT( pData != NULL );
   
   CString Cellabbrev;
   if (m_ksAsParam)
      Cellabbrev = "SICL";
   else
   {

	CString Cellabbrev = pData->GetAsString( m_colAbbrev, cell) ; //for each polygon, get the abbreviation for the soil texture.
   Cellabbrev.TrimRight();
   }


   for (int j=0; j < m_soilInfoArray.GetSize(); j++ )
      {
	   SOIL_INFO *pSoil = m_soilInfoArray.GetAt( j );

		if ( strcmp( pSoil->abbrev, Cellabbrev ) == 0 )
         return pSoil;
      }
   return NULL;
   }

void WET_Hydro::SaveData( )
	{
   
   DataObj** row = new DataObj*[26];
   row[0] = m_pModelClimateData;
   row[1] = m_pInstreamData;
   row[2] = m_pETData;
   row[3] = m_pSwcData;
   row[4] = m_pMassBalanceData;
   row[5] = m_pTracerData;
   row[6] = m_pUplandStorageData;
   row[7] = m_pGeneralData;
   row[8] = m_pTracerConcData;
   row[9] = m_pTracerConcChannelData;
   row[10] = m_pInstreamSedimentData ;
   row[11] = m_pUplandSedimentData;
   row[12] = m_pTotalSedimentOutflowData;
   row[13] = m_pSedimentOutflowData;
   row[14] = m_pMonteCarloResults;
   row[15] = m_pUnsatDepthData;
   row[16] = m_pSatDepthData;
   row[17] = m_pSatDeficitData;
   row[18] = m_pPestVegData;
   row[19] = m_pPestSurfData;
   row[20] = m_pPestUnsatData;
   row[21] = m_pPestSatData;
   row[22] = m_pRateData;
   row[23] = m_pPestInstreamData;
   row[24] = m_pPestMassBalanceData;
   row[25] = m_pTracerMassChannelData;

   m_resultDataArray.Add( row );	
	}

void WET_Hydro::CalcInitialConditions( double endCalib,  bool save )
	{
//   InitRun();         // do any necessary initialization, set up state variable matrices	
   double time = m_startTime;
   int step = 0;
   int count = 0;
	endCalib += time;
   while ( time <= endCalib )
      {
      double curTimeStep = m_timeStep;
      RunUpslopeModel( time );
      double nextTimeStep = m_timeStep;
      m_timeStep = curTimeStep;
      int nodeCount = RunInstreamModel( time );
      int x = GetReachCount();
		m_time = time;
      step++;
      time += curTimeStep;
      m_timeStep = nextTimeStep;		
			
		}
	m_time = time;
	m_runNumber += 1;

	if (save)
		{
		int type = 2;
		SaveState(type);
		}

	}

///////////////////////////////////////////////////////////////////////////////////////////////////
//  The Run Method is called only when NOT optimizing.  
//
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////


void WET_Hydro::Run(  )
   {
   if (m_checkMonteCarlo==WH_SINGLE_SIMULATION)
      Notify( WH_STARTRUN, 0.0f, 0.0f, m_runNumber, m_runNumber );
   clock_t start = clock();
   InitRun();         // do any necessary initialization, set up state variable matrices


   double time = m_startTime;
   m_time = (float)time;
   int count = 0;
   while ( time <= m_stopTime )
      {
      // casts added by MG for VS 2003
      double remainder = fmod( (double)(time+m_timeStep/2), (double)(m_collectionInterval) ) ;
      if ( remainder < m_timeStep  )
         {
         if  (m_checkMonteCarlo==WH_SINGLE_SIMULATION)
            CollectDataSingleRun(  );
            //CollectData(m_time);
         else
            CollectMonteCarloData(time);
         count++;
	    if (m_checkMonteCarlo==WH_SINGLE_SIMULATION)
				Notify( WH_UPDATE, (float)time, (float)m_timeStep, m_runNumber, m_runNumber );
         else
				Notify( WH_UPDATE_MONTE_CARLO, (float)time, (float)m_timeStep, m_runNumber, m_runNumber );

			}

      double curTimeStep = m_timeStep;
      m_curTimeStep = curTimeStep;

     if (m_radar)
         { //go through all model boxes, collect rainfall from the rainfall grid...
            //  do this here because rain will not change within each integration step.

         }

      RunUpslopeModel( time );
      //  This method simulates the effects of proposed restoration options
      if (m_checkApplySedOptions || m_checkIncrFlow)
         ApplyRestorationOptions();
      //  Wetland additions change the structure of the model.  These structural changes must be 
      //  in the CalcWatershedInfo method.  Look there for the call...
      if (m_checkPesticide)
         GetPesticideAppRateSimple();  
         //GetPesticideAppRate();

      double nextTimeStep = m_timeStep;
      m_timeStep = curTimeStep;

      int nodeCount = RunInstreamModel( time);

      if (m_checkTracerModel || m_checkPesticide || m_checkUplandSediment)
         RunAuxillaryInstreamModel(time); // solutes (tracer or sediment) and the method is attentive to 
      // whether they are simulated and the kind of routing used.



      m_timeStep = curTimeStep;//ignore the fact that RunAuxInstreamMod may have reset m_timeStep...
		m_time = time;
		int type = 3;
//		SaveState(type); //This code writes current data to an external array (and appends each time through)
 //     step++;
      time += curTimeStep;
      m_timeStep = nextTimeStep;      
      }
   
	EndRun();
   clock_t finish = clock();
   m_simulationTime = ((float)(finish-start)/CLOCKS_PER_SEC)/60.0f;
   if (m_checkMonteCarlo==WH_SINGLE_SIMULATION)
      Notify( WH_ENDRUN, 0.0f, 0.0f, m_runNumber , m_runNumber);

	m_runNumber += 1;

   }



//-------------------------------------------------------------------------------------
//  WET_Hydro::EstimateParams(float x, float a[], float *y, float dyda[], int na )
//
//  This function is called repeatedly by the optimization routine.  It returns a new value of q
//  based on given parameters.  Note that this function runs the hydro model between two times, which
//  represent the beginning and ending of an optimization step.  Also note that the function must return
//  estimates of the derivative of q with respect to all parameters.  These are calculated as forward differences
//
//  pass the function the time at which we want to upgrade the parameter vector.
//
//--------------------------------------------------------------------------------------

void WET_Hydro::EstimateParams(float x, float a[], int ia[], float *ymod, float dyda[], int ma, long extra, int init )
	{
	// Calculate q at the final time for this call to the function, without parameter adjustment
   // This is the function "*funcs" named in the NRC code...

   WET_Hydro *pHydroModel = (WET_Hydro*) extra;
   ASSERT( pHydroModel != NULL );

	if (init != pHydroModel->m_optNumber)
		{
		int type = 2;
		pHydroModel->RestoreState( type );		
		pHydroModel->m_optNumber = init;
		}

	if (a[ 1 ] < gpOptimize->m_minm)
		a[1]=gpOptimize->m_minm;
	if (a[ 1 ] > gpOptimize->m_maxm)
		a[1]=gpOptimize->m_m*0.95f;

	if (a[ 2 ] < gpOptimize->m_minis)
		a[2]=gpOptimize->m_minis;
	if (a[ 2 ] > gpOptimize->m_maxis)
		a[2]=gpOptimize->m_maxis;

	if (a[ 3 ] < gpOptimize->m_minn)
		a[3]=gpOptimize->m_minn;
	if (a[ 3 ] > gpOptimize->m_maxn)
		a[3]=gpOptimize->m_maxn;

	if (a[ 4 ] < gpOptimize->m_minks)
		a[4]=gpOptimize->m_minks;
	if (a[ 4 ] > gpOptimize->m_maxks)
		a[4]=gpOptimize->m_maxks;

	if (a[ 5 ] < gpOptimize->m_minsoildepth)
		a[5]=gpOptimize->m_minsoildepth;
	if (a[ 5 ] > gpOptimize->m_maxsoildepth)
		a[5]=gpOptimize->m_maxsoildepth;

	if (a[ 6 ] < gpOptimize->m_minkdepth)
		a[6]=gpOptimize->m_minkdepth;
	if (a[ 6 ] > gpOptimize->m_maxkdepth)
		a[6]=gpOptimize->m_maxkdepth;

	if (a[ 7 ] < gpOptimize->m_minet)
		a[7]=gpOptimize->m_minet;
	if (a[ 7 ] > gpOptimize->m_maxet)
		a[7]=gpOptimize->m_maxet;


   int step = 0;
	int subNodeCount = 0;

	int nodeCount = pHydroModel->m_reachTree.m_nodeCount;
   for ( int i=0; i < nodeCount; i++ )  // loop to count the number of sub-nodes (needed to SaveState)
      {
      ReachNode *pNode = pHydroModel->m_reachTree.m_nodeArray[ i ];
      
		subNodeCount += pNode->m_reachInfo.subnodeCount;
		}
	int type = 1;
	pHydroModel->SaveState( type );

	ASSERT(pHydroModel->m_time <= x);

	while ( pHydroModel->m_time < x )
      {
      double curTimeStep = pHydroModel->m_timeStep;

      pHydroModel->RunUpslopeModel( pHydroModel->m_time );

      double nextTimeStep = pHydroModel->m_timeStep;
      pHydroModel->m_timeStep = curTimeStep;

      int nodeCount = pHydroModel->RunInstreamModel( pHydroModel->m_time );

      step++;
      pHydroModel->m_time += curTimeStep;

      pHydroModel->m_timeStep = nextTimeStep;				
		}

	int last = pHydroModel->m_reachTree.m_nodeArray[ 0 ]->m_reachInfo.subnodeCount - 1;
	*ymod = pHydroModel->m_reachTree.m_nodeArray[ 0 ]->m_reachInfo.qArray[ last ] ;

	// Next, given the base case q calculated above, calculate parameter derivates

	pHydroModel->CalculateParameterDerivatives( x, a, ymod, dyda, ma );

	}


void WET_Hydro::CalculateParameterDerivatives(float x, float a[], float *y, float dyda[], int ma  )
	{
	float *aAdjusted = vector(1,ma);
   int step = 0;
	for (int i=1; i <= ma; i++ )
		{
		int type = 1;
		RestoreState( type );

		for (int j=1;j<=ma;j++)
			aAdjusted[j] = a[j];

		aAdjusted[i] = a[i] * 0.999f;  // Calculate derivatives based on a 1% change in parameter value
		 
		while ( m_time <= x )
			{
			double timeStep = m_timeStep;
			double curTimeStep = timeStep;

			RunUpslopeModel( m_time );

			double nextTimeStep = m_timeStep;
			timeStep = curTimeStep;

			int nodeCount = RunInstreamModel( m_time );

			step++;
			m_time += curTimeStep;

			timeStep = nextTimeStep;		
			}

		int last = m_reachTree.m_nodeArray[ 0 ]->m_reachInfo.subnodeCount - 1;
		float perturbedQ = m_reachTree.m_nodeArray[ 0 ]->m_reachInfo.qArray[ last ] ;

		dyda[i] = ( perturbedQ - *y ) / ( aAdjusted[i] - a[i] );
		}
	

	free_vector(aAdjusted,1,ma);	
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//  Type = 1  --  Data is being stored for used within the optimization calulations
//  Type = 2  --  Data is being stored as Model initial Conditions, again in the optimization (go all the way back to the starting time)
//  Type = 3  --  Data is being stored for output reasons and multiple times are saved (other options save a single time)
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void WET_Hydro::SaveState( int type)
	{/*
	SAVED_STATE_INFO *pData = new SAVED_STATE_INFO;

	if ( type == 1 )
		{
		m_pSavedData = new SAVED_STATE_INFO;
		pData = m_pSavedData;
		}

	if ( type == 2 )
		{
		m_pInitialConditions= new SAVED_STATE_INFO;
		pData = m_pInitialConditions;
		}

	if ( type == 3 ) //but if we are simply saving data, we need a struc to save all states through time (call this method each time)
		{
		// make sure we have a hydro data
		if ( m_pDataStore == NULL )
			m_pDataStore = new DataStore;
		// allocate a new state info struct
		pData = m_pDataStore->AddSavedStateInfo() ;//which creates a SAVED_STATE_INFO
		}

	pData->timeStep = (float)m_timeStep;
	pData->time = (float)m_time;
	
	int nodeCount = m_reachTree.m_nodeCount;
	int currentSubNode = 0;
*/
	//ASSERT(m_modelStructureMethod == WH_DISTRIBUTE_WATERSHEDS);
 int count=0;
 int nodeCount = m_reachTree.m_nodeCount;
 FILE *fp1 = fopen("c:\\temp\\Andrews.csv", "wb" );  
   for ( int i=0; i < nodeCount-1; i++ )
      {
	  ReachNode *pNode = m_reachTree.m_nodeArray[ i ];
	  int reachID = pNode->m_reachInfo.reachID;
      REACH_INFO_HYDRO *pHydro = GetReachInfoHydroFromReachID( reachID );
	  ASSERT(pHydro != NULL);
   //  m_savedDataArray.Add(pHydro->lateralInflow);
     fwrite(&pHydro->lateralInflow,1, sizeof(float),fp1 );
	//	pData->savedReachInfo.Add(pHydro->lateralInflow);

      for (int k=0; k<pHydro->upslopeInfoArray.GetSize();k++)
         {
    //     m_savedDataArray.Add(pHydro->upslopeInfoArray[k]->volumeSat);
  //       m_savedDataArray.Add(pHydro->upslopeInfoArray[k]->lateralOutflowVol);
   //      m_savedDataArray.Add(pHydro->upslopeInfoArray[k]->saturatedDepth) ;
         fwrite(&pHydro->upslopeInfoArray[k]->volumeSat,1, sizeof(float),fp1 );
         fwrite(&pHydro->upslopeInfoArray[k]->lateralOutflowVol,1, sizeof(float),fp1 );
         fwrite(&pHydro->upslopeInfoArray[k]->saturatedDepth,1, sizeof(float),fp1 );
		   //pData->savedUpslopeInfo.Add(pHydro->upslopeInfoArray[k]->volumeSat);
		   //pData->savedUpslopeOutflowInfo.Add(pHydro->upslopeInfoArray[k]->lateralOutflowVol);
        // pData->savedUpslopeOutflowInfo.Add(pHydro->upslopeInfoArray[k]->saturatedDepth) ;
         }
		//pData->checkSum += pData->savedUpslopeInfo[ i ] ;
		
		for ( int j=0; j < pNode->m_reachInfo.subnodeCount; j++ )
			{
         m_savedDataArray.Add(pNode->m_reachInfo.qArray[ j ]) ;
         fwrite(&pNode->m_reachInfo.qArray[ j ],1, sizeof(float),fp1) ;
			//pData->savedSubnodeInfo.Add(pNode->m_reachInfo.qArray[ j ]);
	//		currentSubNode++;
			}
   	}
      fclose(fp1);
	}

void WET_Hydro::RestoreState(int type)
	{/*
	SAVED_STATE_INFO *pData = new SAVED_STATE_INFO;
	if ( type == 1 )
		pData = m_pSavedData;

	if ( type == 2 )
		pData = m_pInitialConditions;

	ASSERT( pData != NULL );

	m_timeStep = pData->timeStep;
	m_time = pData->time;
	int nodeCount = m_reachTree.m_nodeCount;

	int subNodeCount = 0;
	int currentSubNode = 0;
	float checkSum = 0.0f;*/
    FILE *fp1 = fopen("c:\\temp\\Andrews.csv", "rb" );  
int nodeCount = m_reachTree.m_nodeCount;
   for ( int i=0; i < nodeCount; i++ )
		{
		// allocate a node
		ReachNode *pNode = m_reachTree.m_nodeArray[ i ];
	//	subNodeCount += pNode->m_reachInfo.subnodeCount;
		}
	
//   ASSERT(m_modelStructureMethod == WH_DISTRIBUTE_WATERSHEDS);
  int count=0;
   for ( i=0; i < nodeCount-1; i++ )
      {
		ReachNode *pNode = m_reachTree.m_nodeArray[ i ];
		int reachID    = pNode->m_reachInfo.reachID;
		REACH_INFO_HYDRO *pHydro = GetReachInfoHydroFromReachID( reachID );
		ASSERT(pHydro != NULL);

	//	pHydro->lateralInflow = m_savedDataArray[count] ;
      fread(&pHydro->lateralInflow,1, sizeof(float),fp1);
      count++;
      for (int k=0; k<pHydro->upslopeInfoArray.GetSize();k++)
         {
		//   pHydro->upslopeInfoArray[k]->volumeSat = m_savedDataArray[count] ;
         fread(&pHydro->upslopeInfoArray[k]->volumeSat,1, sizeof(float),fp1);
         count++;
		//   pHydro->upslopeInfoArray[k]->lateralOutflowVol = m_savedDataArray[count] ;
         fread(&pHydro->upslopeInfoArray[k]->lateralOutflowVol,1, sizeof(float),fp1);
         count++;
     //    pHydro->upslopeInfoArray[k]->saturatedDepth = m_savedDataArray[count] ;
         fread(&pHydro->upslopeInfoArray[k]->saturatedDepth,1, sizeof(float),fp1);
         count++;
         }

//		checkSum += pHydro->upslopeInfoArray[0]->volumeSat;

		for ( int j=0; j < pNode->m_reachInfo.subnodeCount; j++ )
			{
		//	pNode->m_reachInfo.qArray[ j ] = m_savedDataArray[count] ; 
         fread(&pNode->m_reachInfo.qArray[j],1, sizeof(float),fp1);
         count++;
		//	currentSubNode++;
			}
		}
	//	ASSERT( checkSum == pData->checkSum);
      fclose(fp1);
	}



MapLayer *WET_Hydro::SetDEMLayer( LPCSTR AsciiDEMFileName )
	{
	m_pDEMLayer = gpMapWnd->m_pMap->GetLayer( "DEM" );
	//m_pDEMLayer->SetBinColorFlag( BCF_BLUE );
   //m_pDEMLayer->SetBins( 0, 1000, 5 );  
   //m_pDEMLayer->ClassifyData();
   float width=10.0f;float height=10.0f;
  m_pDEMLayer->GetCellSize(width,height);
  m_gridResolution=width;
  m_pDEMLayer->Hide();
	if (m_pDEMLayer == NULL )
		return NULL;

	return m_pDEMLayer;
	}


   //  This is a gridded watershed
MapLayer *WET_Hydro::SetWatershedLayer(LPCSTR filename)
   {
	m_pWatershedLayer = gpMapWnd->m_pMap->GetLayer( "CATCH" );
   m_pWatershedLayer->Hide();
   MapLayer *pStream = gpMapWnd->m_pMap->GetLayer( "STRGRID" );
   pStream->Hide();
	if (m_pWatershedLayer == NULL )
		return NULL;


   return m_pWatershedLayer;
   }

MapLayer *WET_Hydro::SetFDLayer(LPCSTR filename)
   {
	MapLayer *pLayer = gpMapWnd->m_pMap->GetLayer( "Slope" );
	//pLayer->SetBinColorFlag( BCF_BLUE );
   //pLayer->SetBins( 0.3, 1.00, 10 ); 
   //pLayer->ClassifyData();

	m_pFlowDirLayer = gpMapWnd->m_pMap->GetLayer( "FLOWDIR" );
	if (m_pFlowDirLayer == NULL )
		return NULL;

   return m_pFlowDirLayer;
   }

MapLayer *WET_Hydro::SetCellLayer( LPCSTR shapeFileName )
   {
   m_pCellLayer = gpMapWnd->m_pMap->GetLayer( 0 );  // DO load data as well...

   if ( m_pCellLayer == NULL )
      return NULL;
   m_pCellLayer->Hide();

   DataObj *pCellData = m_pCellLayer->m_pData;
   ASSERT( pCellData != NULL );

   //------------------------------------------------------------------------
   //
   // These columns MUST be in the CELL shapefile for the model to run.  

   m_colArea = pCellData->GetCol("Area");
   ASSERT( m_colArea >= 0);

   m_colSlope = pCellData->GetCol("slope");     //return reference from the dbf
   ASSERT( m_colSlope >= 0 );// Did it work?
	
   m_colLengthSlope = pCellData->GetCol("LS");     //return reference from the dbf
   ASSERT( m_colLengthSlope >= 0 );// Did it work?

   m_colZ = pCellData->GetCol("Depmax");     //return reference from the dbf CHECK!!!





   ASSERT( m_colZ >= 0 );// Did it work?

   m_colWatershedID = pCellData->GetCol("hydro_id"); 

   ASSERT( m_colWatershedID >= 0 );       // Did it work?

   m_colAbbrev = pCellData->GetCol("Abbrev"); 
   ASSERT( m_colAbbrev >= 0 );       // Did it work?

   m_colLulc= pCellData->GetCol("Lulc_a"); 
   ASSERT( m_colLulc >= 0 );       // Did it work?

   if (m_checkSoilDepthMap)
      {
      m_colSoilDepth= pCellData->GetCol("Depth_Cl"); 
      ASSERT( m_colSoilDepth >= 0 );       // Did it work?
      }

   m_colLulc_b= pCellData->GetCol("Lulc_b"); 
   ASSERT( m_colLulc_b >= 0 );       // Did it work?

   m_colCentroid= pCellData->GetCol("CENTROID"); 
   ASSERT( m_colCentroid >= 0 );       // Did it work?

   m_colIsRiparian= pCellData->GetCol("ISRIPARIAN"); 
   ASSERT( m_colIsRiparian >= 0 );       // Did it work?

   m_colBuffDist= pCellData->GetCol("BUFFDIST"); 
   ASSERT( m_colBuffDist >= 0 );       // Did it work?

   m_colSideOfStream= pCellData->GetCol("SIDE"); 
   ASSERT( m_colSideOfStream >= 0 );       // Did it work?

   //m_colLtPrecip = pCellData->GetCol("LTPREC"); 
   //ASSERT( m_colLtPrecip >= 0 );       // Did it work?

   m_colOCarbon = pCellData->GetCol("OCARBON"); 
   ASSERT( m_colOCarbon >= 0 );       // Did it work?

   m_colKffact = pCellData->GetCol("KFFACT"); 
   ASSERT( m_colKffact >= 0 );       // Did it work?

  // m_colGeoCode = pCellData->GetCol("GEOCODE"); 
  // ASSERT( m_colGeoCode >= 0 );       // Did it work?

   m_colAltLulc = pCellData->GetCol("ALT_LULC"); 
   ASSERT( m_colAltLulc >= 0 );       // Did it work?

   m_colAltArea = pCellData->GetCol("ALT_AREA"); 
   ASSERT( m_colAltArea >= 0 );       // Did it work?

   m_colAltern = pCellData->GetCol("ALTERN"); 
   ASSERT( m_colAltern >= 0 );       // Did it work?
  //----------------------------------------------------------------------

   // allocate cell info structures
   m_cellInfoArray.SetSize( m_pCellLayer->GetRecordCount() );

   for ( int i=0; i < m_pCellLayer->GetRecordCount(); i++ )
      m_cellInfoArray[ i ] = new CELL_INFO;
   //if (! m_pCellLayer->m_3D)
  //    m_pCellLayer->Project3D(m_pDEMLayer);

//   m_pCellLayer->CalcSlopes( m_colSlope );
//   m_pCellLayer->CalcLengthSlopes(m_colSlope,m_colLengthSlope);
   m_areaTotal=0.0f;

   int cellCount  = GetCellCount();

   for ( int j=0; j < cellCount; j++ )
		{
      float area=GetCellArea(j);
      m_areaTotal+=area;
      }

   
   return m_pCellLayer;

   }


MapLayer *WET_Hydro::SetStreamLayer( LPCSTR shapeFileName )
   {
   m_pStreamLayer = gpMapWnd->m_pMap->GetLayer( "Streams" );  
   if ( m_pStreamLayer == NULL )
      return NULL;   
   ASSERT( m_pStreamLayer != NULL );
   DataObj *pStreamData = m_pStreamLayer->m_pData;
   ASSERT( pStreamData != NULL );

   m_colStreamID = pStreamData->GetCol("hydro_id");
   ASSERT( m_colStreamID >= 0 );  
   if (!m_pStreamLayer->m_3D)
	   m_pStreamLayer->Project3D(m_pDEMLayer, false);
//		{
	int nodeCount = m_reachTree.BuildTree( m_pStreamLayer, m_colStreamID );  // nodes does NOT include phantoms, does include extra root node
	switch ( nodeCount )
		{
		case -1:
		ErrorMsg( "No stream reach layer has been loaded..." );
		break;
		case -2: 
		ErrorMsg( "No vectors in stream coverage..." );
		break;

		case -3:
		ErrorMsg( "Too many downstream nodes - 1 max allowed..." );
		break;

		case -4:
		ErrorMsg( "Topology error..." );
		break;
		case -5:
		ErrorMsg( "Too many unresolved downstream node pointers found while building reach tree..." );
		break;
		case -6:
		ErrorMsg( "No streams upstream of the root node found while building reach tree..." );
		break;
		}

	int temp = m_reachTree.CalculateStreamOrder( );
   // tree is built, initialize reach nodes
   for ( int i=0; i < nodeCount; i++ )
      {
      // allocate a node
      ReachNode *pNode = m_reachTree.m_nodeArray[ i ];

      // initialize its contents
      if ( i != nodeCount-1)      // skip last one!  (it the root node, has no pline)
         {
         // for each REACH_INFO, (except root), make a REACH_INFO_HYDRO extension
         REACH_INFO_HYDRO *pHydro = new REACH_INFO_HYDRO;
         pNode->m_reachInfo.pData = (void*) pHydro;      // set the REACH_INFO data to point to these
         m_reachInfoHydroArray.Add( pHydro );            // add to our internal store
			pHydro->streamOrder = pNode->m_reachInfo.streamOrder;

         int reachID;
         bool ok = m_pStreamLayer->GetData( i, m_colStreamID, reachID );
         m_pStreamLayer->SetData( i, m_pStreamLayer->GetFieldCol("ORDER_"), pNode->m_reachInfo.streamOrder );
         ASSERT( ok );
         pNode->m_reachInfo.reachID = reachID;  // grid code for this reach
         // all done with this REACH_INFO_HYDRO struct, upslope stucts are allocated in InitRun();     
         }
      }
	return m_pStreamLayer;
   }

void WET_Hydro::UpdateDeltaX()
	{
	int reachCount = GetReachCount();   // doesn't include root node (last node) or phantom nodes
	
   for (int i=0; i < reachCount; i++ )
      {
      ReachNode *pNode = m_reachTree.m_nodeArray[ i ];
      pNode->m_reachInfo.CreateSubnodeArray( (float)m_deltaX );
      }
	}



int WET_Hydro::GetReachCount( void )
   {
   if ( m_pStreamLayer == NULL )
      return -1;
   else
      return m_pStreamLayer->GetRecordCount();
   }


REACH_INFO_HYDRO *WET_Hydro::GetReachInfoHydroFromReachID( int reachID )
   {
   ReachNode *pNode = m_reachTree.GetReachNodeFromReachID( reachID );

   if ( pNode == NULL )
      return NULL;

   return (REACH_INFO_HYDRO*) pNode->m_reachInfo.pData;
	}

int WET_Hydro::GetCellCount()
   {
   if ( m_pCellLayer == NULL )
      return -1;
   else
      return m_pCellLayer->GetRecordCount();
   }


int WET_Hydro::GetCellReachID( int cell )
   {
   if ( m_pCellLayer == NULL )
      return -1;
   int reachID;
   bool ok = m_pCellLayer->GetData( cell, m_colWatershedID, reachID );
   ASSERT( ok );
   return reachID;
   }


float WET_Hydro::GetCellArea( int cell )
   {
   if ( m_pCellLayer == NULL )
      return -1;
   float area;
   bool ok = m_pCellLayer->GetData( cell, m_colArea, area );
   ASSERT( ok );
   return area;
   }


float WET_Hydro::GetCellSlope( int cell )
   {
   if ( m_pCellLayer == NULL )
      return -1;
   float slope;
   bool ok = m_pCellLayer->GetData( cell, m_colSlope, slope );
   ASSERT( ok );

   return slope;
   }

float WET_Hydro::GetCellDepth( int cell )
   {
   if ( m_pCellLayer == NULL )
      return -1;
	float depth = 0.0f;
   bool ok = m_pCellLayer->GetData( cell, m_colZ, depth );
   ASSERT( ok );
//	depth = m_soildepth ;// in meters
	depth=depth / 12.0f / 3.28f;
   return depth;
   }
float WET_Hydro::GetCellCentroidDistance( int cell )
   {
   if ( m_pCellLayer == NULL )
      return -1;
   float centroidDist;
   bool ok = m_pCellLayer->GetData( cell, m_colCentroid, centroidDist );
   ASSERT( ok );
   return centroidDist;
   }
int WET_Hydro::GetCellIsRiparian( int cell )
   {
   if ( m_pCellLayer == NULL )
      return -1;
   int isRiparian;
   bool ok = m_pCellLayer->GetData( cell, m_colIsRiparian, isRiparian );
   ASSERT( ok );
   return isRiparian;
   }
float WET_Hydro::GetCellBufferDistance( int cell )
   {
   if ( m_pCellLayer == NULL )
      return -1;
   float buffDist;
   bool ok = m_pCellLayer->GetData( cell, m_colBuffDist, buffDist );
   ASSERT( ok );
   return buffDist;
   }
float WET_Hydro::GetCellLtPrecip( int cell )
   {
   if ( m_pCellLayer == NULL )
      return -1;
   float ltPrecip;
   ASSERT(cell>=0);
   bool ok = m_pCellLayer->GetData( cell, m_colLtPrecip, ltPrecip );
   ASSERT( ok );
  // ltPrecip*=0.0254f;// LTprecip exists in cells as inches - this converts it to meters
   //ltPrecip/=1000.0f;// here it is in mm,and we convert to meters;
   return ltPrecip;
   }
float WET_Hydro::GetCellOCarbon( int cell )
   {
   if ( m_pCellLayer == NULL )
      return -1;
   float oCarbon;
   bool ok = m_pCellLayer->GetData( cell, m_colOCarbon, oCarbon);
   ASSERT( ok );
  
   return oCarbon;
   }

int WET_Hydro::GetCellAltern( int cell )
   {
   if ( m_pCellLayer == NULL )
      return -1;
   int altern;
   bool ok = m_pCellLayer->GetData( cell, m_colAltern, altern );
   ASSERT( ok );
   return altern;
   }


float WET_Hydro::GetCellLengthSlope( int cell )
   {
   if ( m_pCellLayer == NULL )
      return -1;
   float lengthSlope;
   bool ok = m_pCellLayer->GetData( cell, m_colLengthSlope, lengthSlope );
   ASSERT( ok );
   return lengthSlope;
   }

float WET_Hydro::GetCellKffact( int cell )
   {
   if ( m_pCellLayer == NULL )
      return -1;
   float kFfact;
   bool ok = m_pCellLayer->GetData( cell, m_colKffact, kFfact );
   ASSERT( ok );
   return kFfact;
   }


int WET_Hydro::GetCellSideOfStream( int cell )
   {
   if ( m_pCellLayer == NULL )
      return -1;
   int sideOfStream;
   bool ok = m_pCellLayer->GetData( cell, m_colSideOfStream, sideOfStream );
   ASSERT( ok );
   return sideOfStream;
   }

int WET_Hydro::GetCellLulc( int cell )
   {
   if ( m_pCellLayer == NULL )
      return -1;
   int lulc;
   bool ok = m_pCellLayer->GetData( cell, m_colLulc, lulc );
   ASSERT( ok );
   return lulc;
   }
float WET_Hydro::GetCellSoilDepth( int cell )
   {
   if ( m_pCellLayer == NULL )
      return -1;
   float soildepth = 0.3f;
   if (cell != -1)
      bool ok = m_pCellLayer->GetData( cell, m_colSoilDepth, soildepth );
   //ASSERT( ok );
   return soildepth;
   }
int WET_Hydro::GetCellLulc_b( int cell )
   {
   if ( m_pCellLayer == NULL )
      return -1;
   int lulc_b;
   bool ok = m_pCellLayer->GetData( cell, m_colLulc_b, lulc_b );
   ASSERT( ok );
   return lulc_b;
   }

int WET_Hydro::GetCellAltLulc( int cell )
   {
   if ( m_pCellLayer == NULL )
      return -1;
   int altLulc;
   bool ok = m_pCellLayer->GetData( cell, m_colAltLulc, altLulc );
   ASSERT( ok );
   return altLulc;
   }


float WET_Hydro::GetCellAltArea( int cell )
   {
   if ( m_pCellLayer == NULL )
      return -1;
   float altArea;
   bool ok = m_pCellLayer->GetData( cell, m_colAltArea, altArea );
   ASSERT( ok );
   return altArea;
   }
int WET_Hydro::GetCellGeoCode(int cell)
   {
   int geoCode;
   bool ok = m_pCellLayer->GetData( cell, m_colGeoCode, geoCode );
   ASSERT( ok );
   return geoCode;
   }


UPSLOPE_INFO *WET_Hydro::GetCellUpslopeInfo( int cell )
   {
   return m_cellInfoArray[ cell ]->pUpslope;
   }


float WET_Hydro::GetDegSatFromSWC( UPSLOPE_INFO *pUpslope )
   {

	float degSat = pUpslope->swc / (1-pUpslope->pHydroParam->phi);

	return degSat;

   }

void WET_Hydro::ProcessWETCells( void )
	{
	}


//--------------------------------------------------------------------------------------
// WET_Hydro::CalcWatershedInfo
//
//  
// This method calculates static information about the watershed by summarizing the data
// from the maplayer as UPSLOPE_INFO structures, which are stored in the reachTree.
//
//--------------------------------------------------------------------------------------
void WET_Hydro::CalcWatershedInfo(  ) 
   {
   //////////////////////////////////////////////////////////////////////////////////////// 
   // set layer pointers
   ASSERT( m_pCellLayer != NULL );
   ////////////////////////////////////////////////////////////
   // Calculate the slope of each polygon and add the value to the matrix layer data
   m_pCellLayer->CalcSlopes( m_colSlope );
   DataObj *pCellData = m_pCellLayer->m_pData;
   ASSERT( pCellData != NULL );
   // create upslope information structures for each reach
   int reachCount = GetReachCount();
   int cellCount  = GetCellCount();
	int upslopeCount = 0;
   
 
	m_cellStreamIndexArray.RemoveAll();
	m_streamCellIndexArray.RemoveAll();

	if ( m_colCentroid == NULL && m_modelStructureMethod != WH_DISTRIBUTE_WATERSHEDS)
		{
		CString msg ;
		msg.Format("No Centroid Distance available in Database.  Use a semi-distributed approach?");
      int retCode = AfxGetMainWnd()->MessageBox( msg, "Warning", MB_OKCANCEL );

      if( retCode == IDOK )
         m_modelStructureMethod = WH_DISTRIBUTE_WATERSHEDS;
		}
	m_streamCellIndexArray.SetSize(reachCount);

   //  First loop through streams and Cells.  This is also the longest loop because
   //  we build the streamCellIndexArray here.  This array is 2 dimensions.  The first
   //  dimension is of size reachCount. The second dimension varies and is equal to 
   //  the number of cells draining into each stream.

   //  The maximum flowpath distance for each UPLAND_INFO is also built here.

   for (int i=0; i < reachCount; i++)
		{
      
		REACH_INFO       *pInfo  = &m_reachTree.m_nodeArray[ i ]->m_reachInfo;
		REACH_INFO_HYDRO *pHydro = m_reachInfoHydroArray[ i ];
      if (m_checkPesticide)
         {
         //PESTICIDE_INFO *pPesticide = NULL;
         pHydro->instreamPesticideArray.SetSize(m_numberPesticides);
         for (int k=0; k<m_numberPesticides;k++)
            pHydro->instreamPesticideArray[k] = new INSTREAM_PESTICIDE_INFO;
         }
      for (int offset=0; offset < m_instreamHydroParamInfoArray.GetSize(); offset++)
         {
         INSTREAM_HYDRO_PARAM_INFO *pIHydroParam = m_instreamHydroParamInfoArray[offset]; 
         if (pIHydroParam->landscapeType=="Instream")
            pHydro->pInstreamHydroParam = pIHydroParam;
         }


		for ( int j=0; j < cellCount; j++ )
			{
			int reachID = GetCellReachID( j );
			if ( reachID == pInfo->reachID )  // find a match?
            {
				m_streamCellIndexArray[i].Add( j ) ;
            if (m_modelStructureMethod != WH_DISTRIBUTE_WATERSHEDS && m_modelStructureMethod != WH_DISTRIBUTE_HILLRIPARIAN && m_modelStructureMethod != WH_DISTRIBUTE_GEOMORPH )
               {
			      float cellDistance=0;
               int sideOfStream = 0;

               if (m_modelStructureMethod==WH_DISTRIBUTE_CENTROIDDISTANCEBINS)
				      m_pCellLayer->m_pDbTable->GetData(j,m_colCentroid,cellDistance);
               else if (m_modelStructureMethod==WH_DISTRIBUTE_BUFFERDISTANCEBINS)
				      m_pCellLayer->m_pDbTable->GetData(j,m_colBuffDist,cellDistance);

               if (m_checkSideOfStream)
                  {
				      m_pCellLayer->m_pDbTable->GetData(j,m_colSideOfStream,sideOfStream);

                  if (sideOfStream==0) //  a first order stream
                     {
                     if (pHydro->maxFPDistSide1 < cellDistance )
				            pHydro->maxFPDistSide1 = cellDistance;
                     }
                  if (sideOfStream==1)//  the left side of the stream
                     {
                     if (pHydro->maxFPDistSide1 < cellDistance )
				            pHydro->maxFPDistSide1 = cellDistance;
                     }
                  if (sideOfStream==2) // the right side of the stream
                     {
                     if (pHydro->maxFPDistSide2 < cellDistance )
				            pHydro->maxFPDistSide2 = cellDistance;
                     }
                  } // end of if (m_checkSideOfStream )
               else
                  {
                  if ( pHydro->maxFPDistSide1 < cellDistance )
				         pHydro->maxFPDistSide1 = cellDistance;
                  }
               } // end of m_modelStructureMethod != WH_USE_WATERSHEDS
            } //  end of reachID==cellID
         }//  end of cellCount
      } //  end of reachCount


//Next step is to create the array of upslope info
     
   switch( m_modelStructureMethod )
      {
      case WH_DISTRIBUTE_WATERSHEDS:    // 
         CreateSemiDistributed();
         break;

      case WH_DISTRIBUTE_HILLRIPARIAN:
         CreateHillslopeRiparian();
         break;

      case WH_DISTRIBUTE_GEOMORPH:
         CreateGeomorphologic();
         break;

      case WH_DISTRIBUTE_CENTROIDDISTANCEBINS: 
         CreateSimpleDistributed();
         break;

      case WH_DISTRIBUTE_BUFFERDISTANCEBINS: 
         CreateSimpleDistributed();
         break;

      case WH_DISTRIBUTE_LUMPED: 
         CreateLumped();
         break;

      case WH_DISTRIBUTE_FPGRID: 
         CreateFPGrid();
         break;

      case WH_DISTRIBUTE_WTHEIGHT: 
         CreateFPGrid();
         break;

    /*  case WH_DIST_WTHEIGHTANDDEPTH
         CreateWTandDepth();
         break;*/

      }

   CreateSubnodeArray();
   BuildTopology();
   }

void WET_Hydro::CreateSemiDistributed()
   {
   int reachCount = GetReachCount();
   int upslopeCount=0;
   for (int i=0; i < reachCount; i++ )
      {
    //  Notify( WH_ALLOCATEOBJECTS, 0.0f, 0.0f, i, reachCount );
      REACH_INFO       *pInfo  = &m_reachTree.m_nodeArray[ i ]->m_reachInfo;
      REACH_INFO_HYDRO *pHydro = m_reachInfoHydroArray[ i ];
		UPSLOPE_INFO *pUpslope = NULL; 
      pHydro->upslopeInfoArray.RemoveAllElements();
		pUpslope = new UPSLOPE_INFO;
      if (m_checkUnsaturated)
         pUpslope->pUnsat = new UPSLOPE_UNSAT_INFO;

      pHydro->upslopeInfoArray.Add( pUpslope );  // add it to the reach
		pUpslope->index = upslopeCount;
		upslopeCount++;
      for (int offset=0; offset < m_hydrologicParamInfoArray.GetSize(); offset++)
         {
         HYDROLOGIC_PARAM_INFO *pHydroParam = m_hydrologicParamInfoArray[offset]; 
         if (pHydroParam->landscapeType=="Semi-Distributed")
            pUpslope->pHydroParam = pHydroParam;
         }

		for ( int j=0; j<m_streamCellIndexArray[i].GetSize();j++)
    	   {
			int index = m_streamCellIndexArray[i][j];
			pUpslope->cellsRepresentedArray.Add(index);
         float area = GetCellArea( index );
         pUpslope->area += area;
			m_cellInfoArray[ index ]->pUpslope = pUpslope;   // store this upslope ptr in the cell info
			}
   float largestArea=0.0f;
   for ( j=0; j<pUpslope->cellsRepresentedArray.GetSize();j++)
		{
		int index = pUpslope->cellsRepresentedArray[j];
      
		float cellArea = GetCellArea(index);
      if (cellArea > largestArea)
         {
         largestArea=cellArea; //  Here we assign lulc_b to the landuse covering the largest area within the unit.
         pUpslope->lulc_b = GetCellLulc_b(index);
         }
		float weight = cellArea / pUpslope->area;

	   float slope = GetCellSlope( index );
		pUpslope->slope += weight * slope;

		if (m_checkPesticide)
         {
         float oCarbon = GetCellOCarbon(index);
         pUpslope->oCarbon += weight * oCarbon;
         }
      float lengthSlope = GetCellLengthSlope(index);
      pUpslope->lengthSlope += weight * lengthSlope;
      float kFfact = GetCellKffact( index);
      pUpslope->kFfact += weight * kFfact;
      int altern = GetCellAltern(index);


      //  if the model is running change scenarios, we adjust some values of c
      //  based on ALTERN, a landuse/management practice alternative specified 
      //  by RESTORE.
      if (m_checkUplandSediment)
         {
         AUXILLARY_MODEL_INFO *pAuxillaryModelInfo = GetAuxillaryModelParameters( index );
			ASSERT( pAuxillaryModelInfo != NULL );   
         if (m_checkApplySedOptions )
            {
            if (altern == 400 || altern==401 || altern==402 || altern == 403 || altern == 404 )
            pUpslope->musleC += weight * 0.001f;  
            else
				pUpslope->musleC     += weight * pAuxillaryModelInfo->c;
            }
         if (!m_checkApplySedOptions)
			   {
            pUpslope->musleC     += weight * pAuxillaryModelInfo->c;
            pUpslope->musleP     += weight * pAuxillaryModelInfo->p;
            }
         }
               //  If the simulation includes tile drains, calculate the UPSLOPE_INFO tileoutflowCoeff here...
      if (m_checkModelTileDrains)
         {
         int landUse = GetCellLulc(index);
         if (landUse == 3)
            pUpslope->tileOutflowCoef += weight * m_tileOutflowCoef;
         }
		float z=0.0f;

      
      if (m_checkPesticide)
         {
         pUpslope->pesticideArray.SetSize(m_numberPesticides);
         for (int i=0; i<m_numberPesticides; i++)
            {
            pUpslope->pesticideArray[i] = new PESTICIDE_INFO;
            pUpslope->pesticideArray[i]->oCarbon = pUpslope->oCarbon;
            int offset = m_pesticideOffsets[i];
            pUpslope->pesticideArray[i]->offset = offset;
            pUpslope->pesticideArray[i]->pVeg = new VEG_INFO;
            pUpslope->pesticideArray[i]->pSurfWater = new SURF_WATER_INFO;
            pUpslope->pesticideArray[i]->pUnsatWater = new UNSAT_WATER_INFO;
            pUpslope->pesticideArray[i]->pSatWater = new SAT_WATER_INFO;
            UpdatePesticideParameters(pUpslope->pesticideArray[i], pUpslope);
            }  
         }
      }
   }
}




void WET_Hydro::CreateHillslopeRiparian()
   {
   int reachCount = GetReachCount();
   int upslopeCount=0;
   for (int i=0; i < reachCount; i++ )
      {
      REACH_INFO       *pInfo  = &m_reachTree.m_nodeArray[ i ]->m_reachInfo;
      REACH_INFO_HYDRO *pHydro = m_reachInfoHydroArray[ i ];
		UPSLOPE_INFO *pUpslope = NULL;
      pHydro->upslopeInfoArray.RemoveAllElements(); 
      int buildRiparianBox = 1;
      for ( int j=0; j < m_streamCellIndexArray[i].GetSize(); j++) // go through all polys in subwatershed
		   {
         UINT index = m_streamCellIndexArray[i][j];
         int isRiparian = GetCellIsRiparian( index );
         if (isRiparian==1)
            {
            buildRiparianBox=2;
            break;
            }
         }
      for (int k=0; k<buildRiparianBox;k++)
         {
         pUpslope = new UPSLOPE_INFO;
         if (m_checkUnsaturated)
            pUpslope->pUnsat = new UPSLOPE_UNSAT_INFO;
         for ( int j=0; j < m_streamCellIndexArray[i].GetSize(); j++) // go through all polys in subwatershed
            {
            UINT index = m_streamCellIndexArray[i][j];
				int isRiparian = GetCellIsRiparian( index );
            if (k==0) // then we are building a hillside box
               {
   				if (isRiparian==0)//then this is a hillside polygon
                  {
					   pUpslope->cellsRepresentedArray.Add(index);
						float area = GetCellArea( index );
						pUpslope->area += area; // calculate the total area of this upslope to weight averages
	   				m_cellInfoArray[ index ]->pUpslope = pUpslope;   // store this upslope ptr in the cell info
                  }
               }
            else // this is a riparian box, we need to find riparian polygons
               { //  assume that we just built the hillslope box for this subwatershed.
               if (isRiparian==1) //this is a riparian polygon
                  {
						pUpslope->cellsRepresentedArray.Add(index);
						float area = GetCellArea( index );
						pUpslope->area += area; // calculate the total area of this upslope to weight averages
	   				m_cellInfoArray[ index ]->pUpslope = pUpslope;   // store this upslope ptr in the cell info
                  pUpslope->pHydroParam->soilDepth = 5.0f;
                  } 
               }  
            } // end of j loop (we have gone through all polys in this subwatershed
         pHydro->upslopeInfoArray.Add( pUpslope );  // add it to the reach
         } //end of the k loop (we must make 2 bins, one for upslope, 1 for riparian
            
 	      // We now have total area and can calculate area weighted averages.
      for ( int j=0; j < pHydro->upslopeInfoArray.GetSize(); j++ )
         {
         UPSLOPE_INFO *pUpslope = pHydro->upslopeInfoArray[ j ];
			for (int m=0; m < pUpslope->cellsRepresentedArray.GetSize(); m++)
            {
				float cellArea = GetCellArea( pUpslope->cellsRepresentedArray[m] );
				float weight = cellArea / pUpslope->area;

			   float slope = GetCellSlope( pUpslope->cellsRepresentedArray[m] );
			   pUpslope->slope += weight * slope;

            float lengthSlope = GetCellLengthSlope(pUpslope->cellsRepresentedArray[m]);
            pUpslope->lengthSlope += weight * lengthSlope;
            float kFfact = GetCellKffact(pUpslope->cellsRepresentedArray[m]);
            pUpslope->kFfact += weight * kFfact;
            int altern = GetCellAltern(pUpslope->cellsRepresentedArray[m]);
            int lulc = GetCellLulc(pUpslope->cellsRepresentedArray[m]);
            int isRiparian = GetCellIsRiparian(pUpslope->cellsRepresentedArray[m]);

            if (m_checkUplandSediment)
                {
                AUXILLARY_MODEL_INFO *pAuxillaryModelInfo = GetAuxillaryModelParameters( pUpslope->cellsRepresentedArray[m] );
		          ASSERT( pAuxillaryModelInfo != NULL );
                if (m_checkApplySedOptions )
                   {
                   if (altern == 400 || altern==401 || altern==402 || altern == 403 || altern == 404 )
                      pUpslope->musleC += weight * 0.001f;
                 else
			           pUpslope->musleC     += weight * pAuxillaryModelInfo->c;
                    }
                 if (!m_checkApplySedOptions)
                    {
                    pUpslope->musleC     += weight * pAuxillaryModelInfo->c;
                    pUpslope->musleP     += weight * pAuxillaryModelInfo->p;
                    }
    //  If the simulation includes tile drains, calculate the UPSLOPE_INFO tileoutflowCoeff here...
                  if (m_checkModelTileDrains)
                     {
                     int landUse = GetCellLulc(pUpslope->cellsRepresentedArray[m]);
                     if (landUse == 3)
                        pUpslope->tileOutflowCoef += weight * m_tileOutflowCoef;
                     }
                  } // end of m
               } // end of upslopeInfoArray             
            } //end of case
         }
   }


void WET_Hydro::CreateSimpleDistributed()
   {
     
   int reachCount = GetReachCount();
   int upslopeCount=0;
 
   for (int i=0; i < reachCount; i++ )
      {
      Notify( WH_ALLOCATEOBJECTS, 0.0f, 0.0f, i, reachCount );
      REACH_INFO       *pInfo  = &m_reachTree.m_nodeArray[ i ]->m_reachInfo;
      REACH_INFO_HYDRO *pHydro = m_reachInfoHydroArray[ i ];
		UPSLOPE_INFO *pUpslope = NULL;
      int numSides=1;
      int streamOrder = pInfo->streamOrder;
      if (streamOrder > 1)
         numSides = 2;
      if (m_checkSideOfStream)
         {
	      pHydro->upslopeInfoArray.RemoveAllElements();
//create a new upslope_info for each side of the stream and each band
         for (int z=0; z < numSides; z++)
            {
				for (int l=0; l < m_numBands; l++)
					{
               if ( z==0 )
				      m_widthBand = pHydro->maxFPDistSide1 / (float)m_numBands;
               if ( z==1 )
                  m_widthBand = pHydro->maxFPDistSide2 / (float)m_numBands;
               ASSERT(z==0 ||  z==1);
	   	      int bandMin = 0;
			   	int bandMax = 0;
   		      pUpslope = new UPSLOPE_INFO;
               if (m_checkUnsaturated)
                  pUpslope->pUnsat = new UPSLOPE_UNSAT_INFO;


               for (int offset=0; offset < m_hydrologicParamInfoArray.GetSize(); offset++)
                  {
                  HYDROLOGIC_PARAM_INFO *pHydroParam = m_hydrologicParamInfoArray[offset]; 
                  if (pHydroParam->landscapeType=="Semi-Distributed")
                     pUpslope->pHydroParam = pHydroParam;
                  }

               if (m_checkPesticide)
                  {
                  pUpslope->pesticideArray.SetSize(m_numberPesticides);
                  for (int i=0; i<m_numberPesticides; i++)
                     {
                     pUpslope->pesticideArray[i] = new PESTICIDE_INFO;
                     pUpslope->pesticideArray[i]->offset = m_pesticideOffsets[i];
                     pUpslope->pesticideArray[i]->oCarbon = pUpslope->oCarbon;
                     pUpslope->pesticideArray[i]->pVeg = new VEG_INFO;
                     pUpslope->pesticideArray[i]->pSurfWater = new SURF_WATER_INFO;
                     pUpslope->pesticideArray[i]->pUnsatWater = new UNSAT_WATER_INFO;
                     pUpslope->pesticideArray[i]->pSatWater = new SAT_WATER_INFO;
                     UpdatePesticideParameters(pUpslope->pesticideArray[i], pUpslope);
                     }  
                  }
			      pUpslope->minFPDist = (float)l*(float)m_widthBand;
			      pUpslope->maxFPDist = pUpslope->minFPDist+(float)m_widthBand;
			      int size = m_streamCellIndexArray[i].GetSize();
   		      for ( int j=0; j < m_streamCellIndexArray[i].GetSize(); j++)
	   		      {
		   	      UINT index = m_streamCellIndexArray[i][j];
				      int cellDistance;
				      if (m_modelStructureMethod==WH_DISTRIBUTE_CENTROIDDISTANCEBINS)
							cellDistance = (int)GetCellCentroidDistance( index );
						else
							cellDistance = (int)GetCellBufferDistance( index );
                  pUpslope->sideOfStream = GetCellSideOfStream( index );
                  int side = -1;
                  if (pUpslope->sideOfStream == 0 || pUpslope->sideOfStream==1)
                     side=0;
                  if (pUpslope->sideOfStream == 2 )
                     side = 1;
                  ASSERT(side != -1);
                  if (z==0)
                     {
						   if (cellDistance > pUpslope->minFPDist && cellDistance <= pUpslope->maxFPDist && side==0)
							   {
							   pUpslope->cellsRepresentedArray.Add(index);
							   float area = GetCellArea( index );
							   pUpslope->area += area; // calculate the total area of this upslope to weight averages
	   					   m_cellInfoArray[ index ]->pUpslope = pUpslope;   // store this upslope ptr in the cell info
                        }
                     }
                  if (z==1)
                     {
						   if (cellDistance > pUpslope->minFPDist && cellDistance <= pUpslope->maxFPDist && side==1)
							   {
							   pUpslope->cellsRepresentedArray.Add(index);
							   float area = GetCellArea( index );
							   pUpslope->area += area; // calculate the total area of this upslope to weight averages
	   					   m_cellInfoArray[ index ]->pUpslope = pUpslope;   // store this upslope ptr in the cell info
                        }
                     }
                  } // end of j (m_streamCellIndexArray)
					   pHydro->upslopeInfoArray.Add( pUpslope );  // add it to the reach
					   pUpslope->index = upslopeCount;
					   upslopeCount++;  
               } //  end of l (numbands)
            }  // end of z (numSides)
         } // end of if (m_checkSideOfStream)
      else // the check box indicating use both sides independently was not checked
         {
		   pHydro->upslopeInfoArray.RemoveAllElements();
	      //create a new upslope_info for each side of the stream and each band
         for (int l=0; l < m_numBands; l++)
	         {
				m_widthBand = pHydro->maxFPDistSide1 / (float)m_numBands;
			   int bandMin = 0;
			   int bandMax = 0;
		      pUpslope = new UPSLOPE_INFO;
            if (m_checkUnsaturated)
               pUpslope->pUnsat = new UPSLOPE_UNSAT_INFO;
            if (m_checkPesticide)
               {
               pUpslope->pesticideArray.SetSize(m_numberPesticides);
               for (int i=0; i<m_numberPesticides; i++)
                  {
                  pUpslope->pesticideArray[i] = new PESTICIDE_INFO;
                  pUpslope->pesticideArray[i]->offset = m_pesticideOffsets[i];
                  pUpslope->pesticideArray[i]->pVeg = new VEG_INFO;
                  pUpslope->pesticideArray[i]->pSurfWater = new SURF_WATER_INFO;
                  pUpslope->pesticideArray[i]->pUnsatWater = new UNSAT_WATER_INFO;
                  pUpslope->pesticideArray[i]->pSatWater = new SAT_WATER_INFO;
                  UpdatePesticideParameters(pUpslope->pesticideArray[i], pUpslope);
                  }  
               }
            pUpslope->minFPDist = (float)l*(float)m_widthBand;
		      pUpslope->maxFPDist = pUpslope->minFPDist+(float)m_widthBand;
   		   int size = m_streamCellIndexArray[i].GetSize();   
			   for ( int j=0; j < m_streamCellIndexArray[i].GetSize(); j++)
				   {
				   UINT index = m_streamCellIndexArray[i][j];
				   int cellDistance;
				   if (m_modelStructureMethod==WH_DISTRIBUTE_BUFFERDISTANCEBINS)
					   cellDistance = (int)GetCellCentroidDistance( index );
				   else
					   cellDistance = (int)GetCellBufferDistance( index );
               if (cellDistance > pUpslope->minFPDist && cellDistance <= pUpslope->maxFPDist )
		            {
			         pUpslope->cellsRepresentedArray.Add(index);
			         float area = GetCellArea( index );
			         pUpslope->area += area; // calculate the total area of this upslope to weight averages
     			      m_cellInfoArray[ index ]->pUpslope = pUpslope;   // store this upslope ptr in the cell info
                  }            
               } // end of j
					pHydro->upslopeInfoArray.Add( pUpslope );  // add it to the reach
				   pUpslope->index = upslopeCount;
					upslopeCount++;
            } // end of l
         } // end of else

	// We now have total area and can calculate area weighted averages.
      for ( int j=0; j < pHydro->upslopeInfoArray.GetSize(); j++ )
         {
         UPSLOPE_INFO *pUpslope = pHydro->upslopeInfoArray[ j ];
         float largestArea=0.0f;
			for (int m=0; m < pUpslope->cellsRepresentedArray.GetSize(); m++)
            {
				float cellArea = GetCellArea( pUpslope->cellsRepresentedArray[m] );
				float weight = cellArea / pUpslope->area;

            if (m_checkPesticide)
               for (int i=0; i<m_numberPesticides; i++)
                  pUpslope->pesticideArray[i]->oCarbon = pUpslope->oCarbon;
				ASSERT ( pUpslope->pHydroParam->phi != 0.0f );
				float slope = GetCellSlope( pUpslope->cellsRepresentedArray[m] );
				pUpslope->slope += weight * slope;

            float lengthSlope = GetCellLengthSlope(pUpslope->cellsRepresentedArray[m]);
            pUpslope->lengthSlope += weight * lengthSlope;

            float oCarbon = GetCellOCarbon(pUpslope->cellsRepresentedArray[m]);
            pUpslope->oCarbon += weight * oCarbon;
            // update the pesticide database (in WET_Hydro)
            if (m_checkPesticide)
               for (int i=0; i<m_numberPesticides; i++)
                  UpdatePesticideParameters(pUpslope->pesticideArray[i], pUpslope);

            float kFfact = GetCellKffact(pUpslope->cellsRepresentedArray[m]);
            pUpslope->kFfact += weight * kFfact;
            int altern = GetCellAltern(pUpslope->cellsRepresentedArray[m]);
            int lulc = GetCellLulc(pUpslope->cellsRepresentedArray[m]);

            if (cellArea>largestArea)
               {
               largestArea=cellArea; //  Here we assign lulc_b to the landuse covering the largest area within the unit.
               pUpslope->lulc_b = GetCellLulc_b(pUpslope->cellsRepresentedArray[m]);
               }                  
                     
                     
            if (m_checkUplandSediment)
               {
               AUXILLARY_MODEL_INFO *pAuxillaryModelInfo = GetAuxillaryModelParameters( pUpslope->cellsRepresentedArray[m] );
			      ASSERT( pAuxillaryModelInfo != NULL );
               if (m_checkApplySedOptions )
                  {
                  if (altern == 400 || altern==401 || altern==402 || altern == 403 || altern == 404 )
                     pUpslope->musleC += weight * 0.001f;
                        
                  else
			            pUpslope->musleC     += weight * pAuxillaryModelInfo->c;
                  }
               if (!m_checkApplySedOptions)
                  pUpslope->musleC     += weight * pAuxillaryModelInfo->c;
                  pUpslope->musleP     += weight * pAuxillaryModelInfo->p;
               }

                  //  If the simulation includes tile drains, calculate the UPSLOPE_INFO tileoutflowCoeff here...
            if (m_checkModelTileDrains)
               {
               int landUse = GetCellLulc(pUpslope->cellsRepresentedArray[m]);
               if (landUse == 3)
                  pUpslope->tileOutflowCoef += weight * m_tileOutflowCoef;
               }
            } // end of 
         } // end of case
      }
   }


void WET_Hydro::CreateGeoHillRip()
   {
   int reachCount = GetReachCount();
   int upslopeCount=0;
   for (int i=0; i < reachCount; i++ )
      {
      REACH_INFO       *pInfo  = &m_reachTree.m_nodeArray[ i ]->m_reachInfo;
      REACH_INFO_HYDRO *pHydro = m_reachInfoHydroArray[ i ];
		UPSLOPE_INFO *pUpslope = NULL;
      int numSides=1;
      int streamOrder = pInfo->streamOrder;
      if (streamOrder > 1)
         numSides = 2;
      }
   }

int WET_Hydro::CreateLumped(void)
   {
      return 0;
   }

int WET_Hydro::CreateFPGrid(void)
{
   int count=0;
   int reachCount = GetReachCount();

   for (int reach=0; reach < reachCount; reach++ )
      {
      if (m_checkMonteCarlo != WH_MONTE_CARLO)
         Notify( WH_ALLOCATEOBJECTS, (float)reach, (float)reachCount, reach, reachCount );
      float minElev=1E6; float maxStreamElev = -1; int lowCol=-1; int lowRow=-1;float highCol=-1;float highRow=-1;
      REACH_INFO       *pInfo  = &m_reachTree.m_nodeArray[ reach ]->m_reachInfo;
      REACH_INFO_HYDRO *pHydro = m_reachInfoHydroArray[ reach ];
      pHydro->upslopeInfoArray.RemoveAllElements();
		UPSLOPE_INFO *pUpslope = NULL; 

      for (int j=0;j<m_pDEMLayer->GetRowCount();j++)
         {
      //   UpslopeInfoArray *x = new UpslopeInfoArray ;

         for (int i=0;i<m_pDEMLayer->GetColCount();i++)
            {    
            int value;
            MapLayer *pWatershed = gpMapWnd->m_pMap->GetLayer( "CATCH" );
             MapLayer *pStream = gpMapWnd->m_pMap->GetLayer( "STRGRID" );
            bool ok = pWatershed->GetData( j, i, value );
            ASSERT( ok );     
               {
               if (pInfo->reachID==value)//if not, then it is outside this reach
                  {
                  float el=0;
                  m_pDEMLayer->GetData(j,i,el);
                  if (el<minElev)
                     {
                     minElev=el;
                     lowCol = i;
                     lowRow=j;
                     }
                  float value2=-1;
                  pStream->GetData( j, i, value2 );
                  if (value2!=pStream->GetNoDataValue())//if it is a tream (we already know it is in this watershed) we are looking for the hightest stream cell in this watershed
                     {
                     if (el > maxStreamElev)
                        {
                        maxStreamElev = el;
                        highCol = i;
                        highRow = j;
                        }
                     }
                  }
               }
            }
         }
      
      for (int j=0;j<m_pDEMLayer->GetRowCount();j++)
         {
         UpslopeInfoArray *x = new UpslopeInfoArray ;

         for (int i=0;i<m_pDEMLayer->GetColCount();i++)
            {    
            int value;
            MapLayer *pWatershed = gpMapWnd->m_pMap->GetLayer( "CATCH" );
            bool ok = pWatershed->GetData( j, i, value );
            bool addedZero=false;

            ASSERT( ok );     
               {
               if (pInfo->reachID==value)//if not, then it is outside this reach
                  {
                  pUpslope = new UPSLOPE_INFO;
                  if (m_checkUnsaturated)
                     pUpslope->pUnsat = new UPSLOPE_UNSAT_INFO;
                  if (m_checkPesticide)
                     {
                     pUpslope->pesticideArray.SetSize(m_numberPesticides);
                     for (int i=0; i<m_numberPesticides; i++)
                        {
                        pUpslope->pesticideArray[i] = new PESTICIDE_INFO;
                        pUpslope->pesticideArray[i]->offset = m_pesticideOffsets[i];
                        pUpslope->pesticideArray[i]->pVeg = new VEG_INFO;
                        pUpslope->pesticideArray[i]->pSurfWater = new SURF_WATER_INFO;
                        pUpslope->pesticideArray[i]->pUnsatWater = new UNSAT_WATER_INFO;
                        pUpslope->pesticideArray[i]->pSatWater = new SAT_WATER_INFO;
                        pUpslope->oCarbon = 2;                       
                        pUpslope->pesticideArray[i]->oCarbon = pUpslope->oCarbon;
                        UpdatePesticideParameters(pUpslope->pesticideArray[i], pUpslope);
                        }  
                     }
                  count++;
              //    x->Add(pUpslope); 
		            pUpslope->index = count;
                  m_pDEMLayer->GetData(j,i,pUpslope->elev);
                  pUpslope->colIndex = i;
                  pUpslope->rowIndex = j;
                  float x=0;float y=0;
                  int index=-1;int depthClass=0;
                  m_pDEMLayer->GetGridCellCenter(pUpslope->rowIndex,pUpslope->colIndex,x,y);
                  m_pCellLayer->GetPolygonFromCoord(x,y,&index);
                  if (index != -1)
                     {
                     pUpslope->cellsRepresentedArray.Add(index);
                     pUpslope->oCarbon=GetCellOCarbon(index);
                     pUpslope->lulc=GetCellLulc(index);
                     }
                  if (m_modelParamMethod == WH_PARAM_WATERSHEDS)
                     {
                     for (int offset=0; offset < m_hydrologicParamInfoArray.GetSize(); offset++)
                        {
                        HYDROLOGIC_PARAM_INFO *pHydroParam = m_hydrologicParamInfoArray[offset]; 
                        if (pHydroParam->landscapeType=="Semi-Distributed")
                           pUpslope->pHydroParam = pHydroParam;
                        }
                     }
                  if (m_modelParamMethod == WH_PARAM_GEOMORPHOLOGY)
                     {
                     if (index !=-1)
                        depthClass = (int)GetCellSoilDepth(index);

                     switch( depthClass )
                        {
                        case 1:
                           {
                           for (int offset=0; offset < m_hydrologicParamInfoArray.GetSize(); offset++)
                              {
                              HYDROLOGIC_PARAM_INFO *pHydroParam = m_hydrologicParamInfoArray[offset]; 
                              if (pHydroParam->landscapeType=="SHALLOW")
                                 pUpslope->pHydroParam =  pHydroParam;
                              }
                           break;
                           }
                        case 2:
                           {
                           for (int offset=0; offset < m_hydrologicParamInfoArray.GetSize(); offset++)
                              {
                              HYDROLOGIC_PARAM_INFO *pHydroParam = m_hydrologicParamInfoArray[offset]; 
                              if (pHydroParam->landscapeType=="MEDIUM")
                                 pUpslope->pHydroParam =  pHydroParam;
                              }
                           break;
                           }

                        case 3: 
                           {
                           for (int offset=0; offset < m_hydrologicParamInfoArray.GetSize(); offset++)
                              {
                              HYDROLOGIC_PARAM_INFO *pHydroParam = m_hydrologicParamInfoArray[offset]; 
                              if (pHydroParam->landscapeType=="DEEP")
                                 pUpslope->pHydroParam =  pHydroParam;
                              }
                           break;
                           }

                        default:
                           {
                           for (int offset=0; offset < m_hydrologicParamInfoArray.GetSize(); offset++)
                              {
                              HYDROLOGIC_PARAM_INFO *pHydroParam = m_hydrologicParamInfoArray[offset]; 
                              if (pHydroParam->landscapeType=="SHALLOW")
                                 pUpslope->pHydroParam =  pHydroParam;
                              }
                           }
                        }
                     }


                  pUpslope->area = (float)m_gridResolution * (float)m_gridResolution;   
                 // pUpslope->slope = 0.4f; 


                  if (m_checkPesticide)
                     for (int i=0; i<m_numberPesticides; i++)
                        UpdatePesticideParameters(pUpslope->pesticideArray[i], pUpslope);
                  

                  MapLayer *pStream = gpMapWnd->m_pMap->GetLayer( "STRGRID" );
                  if (pStream->IsDataValid(j,i))
                     {
                     pUpslope->isStream=true;
                     //pUpslope->pHydroParam->soilDepth=pUpslope->pHydroParam->soilDepth;   
                     }
                     // we need to grab info from the cells shapefile and update each UPSLOPE_INFO, which in this case
                     // was derived from the grids, not from the cell shapefile...
                  if (m_radar)
                     {
                     MapLayer *pRain = gpMapWnd->m_pMap->GetLayer("RADAR");

                     float x1=0.0f;
                     float y1=0.0f;
                     pStream->GetGridCellCenter(j,i,x1,y1);//this could be any grid(not the demlayer), assuming they start in the same corner
                     pRain->GetGridCellFromCoord(x1,y1,pUpslope->radarRow,pUpslope->radarCol);             
                     }

                  
                  if (pUpslope->colIndex == lowCol && pUpslope->rowIndex == lowRow)
                     {
                     pHydro->upslopeInfoArray.InsertAt(0,pUpslope);
                     addedZero=true;
                     }
                  else if (pUpslope->colIndex == highCol && pUpslope->rowIndex == highRow)
                     {
                     if (addedZero==true)
                        pHydro->upslopeInfoArray.InsertAt(1,pUpslope);
                     else
                        pHydro->upslopeInfoArray.InsertAt(0,pUpslope);
                     }
                  else
                     pHydro->upslopeInfoArray.Add(pUpslope);
                  }
               }
            }
       //  pHydro->upslopeFor2DArray.Add(x);
         }
      }
   return 0;
   }

void WET_Hydro::UpdateParameterVector()
   {
   int reachCount = GetReachCount();

   for (int reach=0; reach < reachCount; reach++ )
      {

      REACH_INFO       *pInfo  = &m_reachTree.m_nodeArray[ reach ]->m_reachInfo;
      REACH_INFO_HYDRO *pHydro = m_reachInfoHydroArray[ reach ];
      for (int offset=0; offset < m_instreamHydroParamInfoArray.GetSize(); offset++)
         {
         INSTREAM_HYDRO_PARAM_INFO *pIHydroParam = m_instreamHydroParamInfoArray[offset]; 
         if (pIHydroParam->landscapeType=="Instream")
            pHydro->pInstreamHydroParam = pIHydroParam;
         }
      for ( int j=pHydro->upslopeInfoArray.GetSize()-1;j >= 0; j--)
         {       
         UPSLOPE_INFO *pUpslope = pHydro->upslopeInfoArray[ j ];
         if (m_modelParamMethod == WH_PARAM_WATERSHEDS)
            {
            for (int offset=0; offset < m_hydrologicParamInfoArray.GetSize(); offset++)
               {
               HYDROLOGIC_PARAM_INFO *pHydroParam = m_hydrologicParamInfoArray[offset]; 
               if (pHydroParam->landscapeType=="Semi-Distributed")
                  pUpslope->pHydroParam = pHydroParam;
               }
            }
/*         if (m_modelParamMethod == WH_PARAM_GEOMORPHOLOGY)
            {
            
               int depthClass = (int)GetCellSoilDepth(index);

            switch( depthClass )
               {
               case 1:
                  {
                  for (int offset=0; offset < m_hydrologicParamInfoArray.GetSize(); offset++)
                     {
                     HYDROLOGIC_PARAM_INFO *pHydroParam = m_hydrologicParamInfoArray[offset]; 
                     if (pHydroParam->landscapeType=="SHALLOW")
                        pUpslope->pHydroParam =  pHydroParam;
                     }
                  break;
                  }
               case 2:
                  {
                  for (int offset=0; offset < m_hydrologicParamInfoArray.GetSize(); offset++)
                     {
                     HYDROLOGIC_PARAM_INFO *pHydroParam = m_hydrologicParamInfoArray[offset]; 
                     if (pHydroParam->landscapeType=="MEDIUM")
                        pUpslope->pHydroParam =  pHydroParam;
                     }
                  break;
                  }

               case 3: 
                  {
                  for (int offset=0; offset < m_hydrologicParamInfoArray.GetSize(); offset++)
                     {
                     HYDROLOGIC_PARAM_INFO *pHydroParam = m_hydrologicParamInfoArray[offset]; 
                     if (pHydroParam->landscapeType=="DEEP")
                        pUpslope->pHydroParam =  pHydroParam;
                     }
                  break;
                  }

               default:
                  {
                  for (int offset=0; offset < m_hydrologicParamInfoArray.GetSize(); offset++)
                     {
                     HYDROLOGIC_PARAM_INFO *pHydroParam = m_hydrologicParamInfoArray[offset]; 
                     if (pHydroParam->landscapeType=="SHALLOW")
                        pUpslope->pHydroParam =  pHydroParam;
                     }
                  }
               }
            }*/
         }
      }
   }  



void WET_Hydro::CreateGeomorphologic()
   {
   int reachCount = GetReachCount();
   int upslopeCount=0;
   for (int i=0; i < reachCount; i++ )
      {
      REACH_INFO       *pInfo  = &m_reachTree.m_nodeArray[ i ]->m_reachInfo;
      REACH_INFO_HYDRO *pHydro = m_reachInfoHydroArray[ i ];
		UPSLOPE_INFO *pUpslope = NULL;
      pHydro->upslopeInfoArray.RemoveAllElements(); 
      int buildGeoBox = 0;
      CArray <int, int& > geoList;
     
      //First step is to figure out how many boxes are required.  Here we simply count the number of geomorph units (for each reach)
      for ( int j=0; j < m_streamCellIndexArray[i].GetSize(); j++) // go through all polys in subwatershed
		   {
         UINT index = m_streamCellIndexArray[i][j];
         int geoCode = GetCellGeoCode( index );

         bool foundNew=true;
         for (int k=0;k<geoList.GetSize();k++)
            if (geoCode == geoList[k])
               foundNew=false;           
         if (foundNew==true)
            {
            geoList.Add(geoCode);  
            buildGeoBox++;
            }
         }

         //The next step is to allocate and parameterize the number of boxes
         // This is for the first subwatershed (still in the i loop)
         int crap=geoList.GetSize();
         for (int k=0; k<geoList.GetSize();k++)
            {
            pUpslope = new UPSLOPE_INFO;
            if (m_checkUnsaturated)
               pUpslope->pUnsat = new UPSLOPE_UNSAT_INFO;
            for ( int j=0; j < m_streamCellIndexArray[i].GetSize(); j++) // go through all polys in subwatershed
               {
               UINT index = m_streamCellIndexArray[i][j];
				   int geoCode = GetCellGeoCode( index );
               // Does this polygon belong in the current box?
               if (geoCode==geoList[k]) 
                  {
					   pUpslope->cellsRepresentedArray.Add(index);
				      float area = GetCellArea( index );
				      pUpslope->area += area; // calculate the total area of this upslope to weight averages
	   			   m_cellInfoArray[ index ]->pUpslope = pUpslope;   // store this upslope ptr in the cell info
                  }  
               } // end of j loop (we have gone through all polys in this subwatershed
            pHydro->upslopeInfoArray.Add( pUpslope );  // add it to the reach     
            //Now parameterize this box...
            CString name(_T(""));
            if (geoList[k]==1)
               name="Earthflows";
            if (geoList[k]==3)
               name="Volcanics";
            if (geoList[k]==6)
               name="Upland";
            if (geoList[k]==7)
               name="Disturbed";
          // Parameterize this box based upon it Geomorph Type
            for (int offset=0; offset < m_hydrologicParamInfoArray.GetSize(); offset++)
               {
               HYDROLOGIC_PARAM_INFO *pHydroParam = m_hydrologicParamInfoArray[offset]; 
               if (pHydroParam->landscapeType==name)
                  pUpslope->pHydroParam = pHydroParam;        
               }                  
            //  Calculate the slope of this box, based upon the GIS data it represents.
            float largestArea=0.0f;
            for ( j=0; j<pUpslope->cellsRepresentedArray.GetSize();j++)
		         {
		         int index = pUpslope->cellsRepresentedArray[j];
               
		         float cellArea = GetCellArea(index);
               if (cellArea>largestArea)
                  {
                  largestArea=cellArea; //  Here we assign lulc_b to the landuse covering the largest area within the unit.
                  pUpslope->lulc_b = GetCellLulc_b(index);
                  }
		         float weight = cellArea / pUpslope->area;

	            float slope = GetCellSlope( index );
		         pUpslope->slope += weight * slope;

               }
         } //end of the k loop 
      }//end of the reach loop (i)
   }
 
void WET_Hydro::CreateSubnodeArray()
   {
   int reachCount = GetReachCount();
   int upslopeCount=0;
   // for each reach node, create a subnode array for numerical purposes
//   int reachCount = GetReachCount();   // doesn't include root node (last node) or phantom nodes
   for (int i=0; i < reachCount; i++ )
      {
      ReachNode *pNode = m_reachTree.m_nodeArray[ i ];
//      pNode->m_reachInfo.length = 3000.0f;
      pNode->m_reachInfo.CreateSubnodeArray( (float)m_deltaX );
      int subnodeCount = pNode->m_reachInfo.subnodeCount;
      REACH_INFO_HYDRO *pInfo = (REACH_INFO_HYDRO*)pNode->m_reachInfo.pData;
      pInfo->tracerMassArray = new float[subnodeCount];
     
      pInfo->waterVolumeArray = new float[subnodeCount];
      pInfo->tracerDischargeArray = new float[subnodeCount];
  
      pInfo->qArrayPrevious = new float[subnodeCount];
      pInfo->sedimentDischargeArray = new float[subnodeCount];
      pInfo->sedimentMassArray = new float[subnodeCount];

      if (m_checkPesticide)
         {
         for (int k=0; k< m_numberPesticides;k++)
            {
            pInfo->instreamPesticideArray[k]->pesticideMassArray = new float[subnodeCount];
            pInfo->instreamPesticideArray[k]->pesticideDischargeArray = new float[subnodeCount];
            pInfo->instreamPesticideArray[k]->pesticideMassArrayUpstream = new float[subnodeCount];
            for (int i=0;i<subnodeCount;i++)
               {
               pInfo->instreamPesticideArray[k]->pesticideMassArray[i]=0.0f;
               pInfo->instreamPesticideArray[k]->pesticideDischargeArray[i]=0.0f;
               pInfo->instreamPesticideArray[k]->pesticideMassArrayUpstream[i]=0.0f;
               }
            }
         }
  
      for (int i=0;i<subnodeCount;i++)
         {
            pInfo->waterVolumeArray[i]=pNode->m_reachInfo.qInit;
    //     pInfo->tracerMassArray[i]=pInfo->waterVolumeArray[i]*m_initTracerConc/1000;
         pInfo->tracerMassArray[i] = 0.0f;
         pInfo->tracerDischargeArray[i]=0.0f;
         pInfo->qArrayPrevious[i] = pNode->m_reachInfo.qInit;
         pInfo->sedimentDischargeArray[i] = 1.0f;
         pInfo->sedimentMassArray[i] = 1.0f;
         }
      }
   }

void WET_Hydro::ResetVariableValues()
   {
   m_time = m_startTime;
   int reachCount = GetReachCount();
   for (int i=0; i < reachCount; i++ )
      {     
      ReachNode *pNode =   m_reachTree.m_nodeArray[ i ];    
      REACH_INFO       *pInfo  = &m_reachTree.m_nodeArray[ i ]->m_reachInfo;     
      REACH_INFO_HYDRO *pHydro = m_reachInfoHydroArray[ i ];

      pHydro->lateralInflow = 0.0f;      // flow rate into this reach
      pHydro->cumInflow     = 0.0f;
      pHydro->cumOutFlowVol    = 0.0f;
      pHydro->cumLateralInflow = 0.0f;     
      pHydro->tracerConc=0.0f;
      for (int k=0;k<pInfo->subnodeCount; k++) 
         {
         pInfo->qArray[k]=pInfo->qInit;
         pHydro->tracerDischargeArray[k] = 0.0f;
         }

		for (int row=0;row<pHydro->upslopeInfoArray.GetSize(); row++) 
		   {
		   UPSLOPE_INFO *pUpslope = pHydro->upslopeInfoArray[ row ]; 
       //  pUpslope->volumeSat=  0.0f ;
       //  pUpslope->saturatedDepth= 0.0f ;
       //  pUpslope->voidVolume= 0.0f ;
         pUpslope->initUpslopeTracerMass= 0.0f ;
         pUpslope->ssfVol= 0.0f ;
         pUpslope->sdmm= 0.0f ;
         pUpslope->swcUnsat= 0.0f ; 
         pUpslope->groundWaterChange= 0.0f ;
         pUpslope->deltaSat= 0.0f ;
         pUpslope->deltaUnsat= 0.0f ;
         pUpslope->lateralOutflowVol=  0.0f ;
         pUpslope->lateralInflow= 0.0f ; 
        // pUpslope->swc= 0.0f ; 
         pUpslope->pondedVolume= 0.0f ;
         pUpslope->tracerConc= 0.0f ;
         pUpslope->tracerMass= 0.0f ;
         pUpslope->tracerOutflowMassSurface= 0.0f ; 
         pUpslope->tracerInflowMassSubsurface= 0.0f ;
         pUpslope->currentPrecip= 0.0f ;
         pUpslope->currentET= 0.00f ;
         pUpslope->currentAirTemp= 10.0f ; 
         pUpslope->cumPrecipVol= 0.0f ;
         pUpslope->precipVol= 0.0f ;
         pUpslope->etVol= 0.0f ; 
         pUpslope->cumETVol= 0.0f ; 
         pUpslope->cumSWC= 0.0f ; 
         pUpslope->cumGroundLoss= 0.0f ;
         pUpslope->cumTracerMassInPrecip= 0.0f ; 
         pUpslope->tracerOutflowMassSubsurface= 0.0f ; 
         pUpslope->surfaceOutflowVol= 0.0f ;
        // pUpslope->initTracerMass= 0.0f ; 
         pUpslope->volumeSWE= 0.0 ; 
         pUpslope->meltRate= 0.0f ; 
         pUpslope->currentSnow= 0.0f ; 
         pUpslope->volumeVeg= 0.0f ;
         pUpslope->vegDrainRate= 0.0f ;
       //  pUpslope->pUnsat->volumeUnsat= 0.0f;
       //  pUpslope->voidVolume= 0.0f;
      //   pUpslope->pUnsat->unsaturatedDepth= 0.0f;
       //  pUpslope->pUnsat->usfVol= 0.0f;
     //    pUpslope->swcUnsat= 0.0f;

         CalcVoidVolume(pUpslope);
         pUpslope->volumeSat =  pUpslope->pHydroParam->initSat*pUpslope->voidVolume;
         pUpslope->tracerConc = m_initTracerConc/1000.0f;
         
         pUpslope->tracerMass = m_initTracerConc/1000.0f * pUpslope->volumeSat;
         
         pUpslope->initTracerMass= (m_initTracerConc/1000.0f * pUpslope->volumeSat);         
         pUpslope->saturatedDepth = pUpslope->volumeSat / (pUpslope->area * pUpslope->pHydroParam->phi) ;
		   pUpslope->swc = pUpslope->volumeSat / (pUpslope->area * pUpslope->saturatedDepth);        
         float c = pUpslope->pHydroParam->fieldCapac/pUpslope->pHydroParam->phi;
         float d = pUpslope->pHydroParam->wiltPt/pUpslope->pHydroParam->phi;
        
         if (m_checkUnsaturated)
            {
            pUpslope->pUnsat->unsaturatedDepth = (pUpslope->pHydroParam->soilDepth - pUpslope->saturatedDepth);
            pUpslope->pUnsat->volumeUnsat = pUpslope->area * pUpslope->pUnsat->unsaturatedDepth * pUpslope->pHydroParam->phi * pUpslope->pHydroParam->initUnSat;
            pUpslope->swcUnsat=pUpslope->pHydroParam->wiltPt + ((pUpslope->pHydroParam->phi - pUpslope->pHydroParam->wiltPt)/2.0f);
            pUpslope->pUnsat->tracerMass = m_initTracerConc/1000.0f * pUpslope->pUnsat->volumeUnsat;
            pUpslope->pUnsat->tracerConc = m_initTracerConc/1000.0f;
            pUpslope->initTracerMass+=pUpslope->pUnsat->tracerMass;
            }
        
         pUpslope->tileDrainageVol =0.0f;
         pUpslope->tracerWeightedMeanTime =0.0f;  
         }
      pHydro->firstMoment =0.0f;
      pHydro->zerothMoment = 0.0f;
      }
      
   int dataToCollect = m_reachDataArray.GetSize()+2;  //extra cols for time and measured data

 /*  if ( m_pInstreamData != NULL && m_checkMonteCarlo==WH_MONTE_CARLO)
      {
      delete m_pInstreamData;
      m_pInstreamData = NULL;
      if (m_checkTracerModel)
         {
         delete m_pTracerConcChannelData;
         m_pTracerConcChannelData = NULL;
         }
      }
   m_pInstreamData = new FDataObj( dataToCollect , 0 );  
   m_pTracerConcChannelData = new FDataObj( dataToCollect , 0 ); 
*/
   }


void WET_Hydro::BuildTopology()
   {
   int reachCount = GetReachCount();
   int upslopeCount=0;
   for (int i=0; i < reachCount; i++ )
      {
      
      int countYes=0; int countNo=0;
      
      ReachNode *pNode =   m_reachTree.m_nodeArray[ i ];    
      REACH_INFO       *pInfo  = &m_reachTree.m_nodeArray[ i ]->m_reachInfo;     
      REACH_INFO_HYDRO *pHydro = m_reachInfoHydroArray[ i ];
      int streamOrder = pInfo->streamOrder;

      pHydro->lateralInflow = 0.0f;      // flow rate into this reach
      pHydro->cumInflow     = 0.0f;
      pHydro->cumOutFlowVol    = 0.0f;
      pHydro->cumLateralInflow = 0.0f;

      if (m_modelStructureMethod==WH_DISTRIBUTE_FPGRID)
         {
         int col = 0;
         
		   for (int row=0;row<pHydro->upslopeInfoArray.GetSize(); row++) 
				{
            int rowUp = -1; int colUp=-1; 
				UPSLOPE_INFO *pUpslope = pHydro->upslopeInfoArray[ row ]; 
            float myElev = pUpslope->elev;
            float upElev = 0.0f;

		         for (int upCheck=0;upCheck<pHydro->upslopeInfoArray.GetSize(); upCheck++) 
				      {
				      UPSLOPE_INFO *pUpCheck = pHydro->upslopeInfoArray[ upCheck ];
					  if ( (pUpCheck->rowIndex==pUpslope->rowIndex-1 || pUpCheck->rowIndex==pUpslope->rowIndex || pUpCheck->rowIndex==pUpslope->rowIndex+1) && pUpCheck->colIndex==pUpslope->colIndex-1) 
                     if (pUpCheck->elev > upElev)
                        {
                        upElev = pUpCheck->elev;
                        pUpslope->pUpslope = pUpCheck;
                        }
                  if ((pUpCheck->rowIndex==pUpslope->rowIndex-1 || pUpCheck->rowIndex==pUpslope->rowIndex+1) && pUpCheck->colIndex==pUpslope->colIndex ) 
                     if (pUpCheck->elev > upElev)
                        {
                        upElev = pUpCheck->elev;
                        pUpslope->pUpslope = pUpCheck;
                        }
                  if ((pUpCheck->rowIndex==pUpslope->rowIndex-1 || pUpCheck->rowIndex==pUpslope->rowIndex || pUpCheck->rowIndex==pUpslope->rowIndex+1) && pUpCheck->colIndex==pUpslope->colIndex+1 ) 
                     if (pUpCheck->elev > upElev)
                        {
                        upElev = pUpCheck->elev;
                        pUpslope->pUpslope = pUpCheck;
                        }
                  }
                  pUpslope->slope = (pUpslope->pUpslope->elev - pUpslope->elev) / m_gridResolution;

            }
         }
      if (m_modelStructureMethod==WH_DISTRIBUTE_WTHEIGHT)
         {
         int rowCount = pHydro->upslopeInfoArray.GetSize();
		   for (int row=0;row<rowCount; row++) 
				{
            m_extra=row; m_extra1=rowCount;
            if ( m_checkMonteCarlo != WH_MONTE_CARLO)
               Notify( WH_BUILDTOPOLOGY, (float) i, (float) reachCount, row, rowCount );
				UPSLOPE_INFO *pUpslope = pHydro->upslopeInfoArray[ row ]; 
		      for (int upCheck=0;upCheck<pHydro->upslopeInfoArray.GetSize(); upCheck++) 
				   {
				   UPSLOPE_INFO *pUpCheck = pHydro->upslopeInfoArray[ upCheck ];
               if ( (pUpCheck->rowIndex==pUpslope->rowIndex-1 || pUpCheck->rowIndex==pUpslope->rowIndex || pUpCheck->rowIndex==pUpslope->rowIndex+1) && pUpCheck->colIndex==pUpslope->colIndex-1) 
                     pUpslope->neighborArray.Add(pUpCheck);
               if ((pUpCheck->rowIndex==pUpslope->rowIndex-1 || pUpCheck->rowIndex==pUpslope->rowIndex+1) && pUpCheck->colIndex==pUpslope->colIndex ) 
                     pUpslope->neighborArray.Add(pUpCheck);
               if ((pUpCheck->rowIndex==pUpslope->rowIndex-1 || pUpCheck->rowIndex==pUpslope->rowIndex || pUpCheck->rowIndex==pUpslope->rowIndex+1) && pUpCheck->colIndex==pUpslope->colIndex+1 ) 
                     pUpslope->neighborArray.Add(pUpCheck);
               }

       
/*
            if (row==0)//this is the subbasin's downslope cell - it is 'connected' to the next subbasins furthest upslope cell
               {        
               ReachNode *pDown = pNode->m_pDown;
               if (pDown->m_reachInfo.index!=-1)//it is NOT the root node;
                  {
                  REACH_INFO_HYDRO *pHydroDown = (REACH_INFO_HYDRO*)pDown->m_reachInfo.pData;
                  UPSLOPE_INFO *pUpslopeDown = pHydroDown->upslopeInfoArray[1];
                  pUpslope->neighborArray.Add(pUpslopeDown);
                  }
               }
            if (row==1) // this is the subbasins furthest upslope stream cell, and it is connected to the upslope subbasins lowest stream cell
                        // there may be 2 upslope cells, but no more than that (generally)   
               {
               if (pNode->m_pLeft != NULL)
                  {
                  ReachNode *pLeft = pNode->m_pLeft;
                  REACH_INFO_HYDRO *pHydroLeft = (REACH_INFO_HYDRO*)pLeft->m_reachInfo.pData;
                  UPSLOPE_INFO *pUpslopeLeft = pHydroLeft->upslopeInfoArray[0];
                  pUpslope->neighborArray.Add(pUpslopeLeft);
                  }
               if (pNode->m_pRight != NULL)
                  {
                  ReachNode *pRight = pNode->m_pRight;
                  REACH_INFO_HYDRO *pHydroRight = (REACH_INFO_HYDRO*)pRight->m_reachInfo.pData;
                  UPSLOPE_INFO *pUpslopeRight = pHydroRight->upslopeInfoArray[0];
                  pUpslope->neighborArray.Add(pUpslopeRight);
                  }
               }  */
               
            int test = pUpslope->neighborArray.GetSize();
            ASSERT(pUpslope->neighborArray.GetSize()<9);

       
            }
         }
           
      if (m_modelStructureMethod==WH_DISTRIBUTE_HILLRIPARIAN)
         {//the 0th upslopeInfo is always the hillslope.  It may drain through the 1st. There are never more than 2
 			for (int m=pHydro->upslopeInfoArray.GetSize()-1; m > 0; m--) //don't add upslope to the highest bin - there isn't one!
				{
				UPSLOPE_INFO *pUpslope = pHydro->upslopeInfoArray[ m ];
				pUpslope->pUpslope = pHydro->upslopeInfoArray[m-1];
				}   
         }

		if (m_modelStructureMethod==WH_DISTRIBUTE_CENTROIDDISTANCEBINS )
			{
         if (! m_checkSideOfStream || pInfo->streamOrder==1 ) //sides of streams are the same, or it is first order
            {
		      for (int m=0; m < pHydro->upslopeInfoArray.GetSize()-1; m++) //don't add upslope to the highest bin - there isn't one!
		         {
			      UPSLOPE_INFO *pUpslope = pHydro->upslopeInfoArray[ m ];
			      pUpslope->pUpslope = pHydro->upslopeInfoArray[m+1];
			      }
            }
            
         else //if you do want side of streams treated differently, the upslope pointer is more complicated
            {
            int secondSideOfStream=-1;
			   for (int m=0; m < m_numBands-1; m++) //don't add upslope to the highest bin - there isn't one!
				   {
				   UPSLOPE_INFO *pUpslope = pHydro->upslopeInfoArray[ m ];
				   pUpslope->pUpslope = pHydro->upslopeInfoArray[m+1];
               secondSideOfStream = m;
   			   }
            for (m=secondSideOfStream+2; m < (m_numBands*2)-1; m++) // add 2 since the above for loop is a POST test loop
               {
				   UPSLOPE_INFO *pUpslope = pHydro->upslopeInfoArray[ m ];
				   pUpslope->pUpslope = pHydro->upslopeInfoArray[m+1];
               }
            }
			}
      // reset the discharge values in the reach...
      for (int ms=0; ms<pInfo->subnodeCount;ms++)
         pInfo->qArray[ms]=pInfo->qInit;

      for ( int j=0; j < pHydro->upslopeInfoArray.GetSize(); j++ )
         {
         UPSLOPE_INFO *pUpslope = pHydro->upslopeInfoArray[ j ];
         CalcVoidVolume(pUpslope);
         if (pUpslope->area==1.0f)
            {
            pUpslope->swc=0.2f;
            pUpslope->swcUnsat=0.2f;
            }
         else 
            {
           // pUpslope->volumeSat = pUpslope->area  * pUpslope->pHydroParam->phi * pUpslope->pHydroParam->initSat* pUpslope->pHydroParam->soilDepth;  
            pUpslope->volumeSat =  pUpslope->pHydroParam->initSat*pUpslope->voidVolume;
            pUpslope->tracerConc = m_initTracerConc/1000.0f;
            
            pUpslope->tracerMass = m_initTracerConc/1000.0f * pUpslope->volumeSat;
            
            pUpslope->initTracerMass= (m_initTracerConc/1000.0f * pUpslope->volumeSat);
            pUpslope->sedimentOutflowMass = 0.0f;
            pUpslope->sedimentMass = 1E20f;
   	
            
            pUpslope->saturatedDepth = pUpslope->volumeSat / (pUpslope->area * pUpslope->pHydroParam->phi) ;
		      pUpslope->swc = pUpslope->volumeSat / (pUpslope->area * pUpslope->saturatedDepth);        


            float c = pUpslope->pHydroParam->fieldCapac/pUpslope->pHydroParam->phi;
            float d = pUpslope->pHydroParam->wiltPt/pUpslope->pHydroParam->phi;

            
            if (m_checkUnsaturated)
               {
               pUpslope->pUnsat->unsaturatedDepth = (pUpslope->pHydroParam->soilDepth - pUpslope->saturatedDepth);
               pUpslope->pUnsat->volumeUnsat = pUpslope->area * pUpslope->pUnsat->unsaturatedDepth * pUpslope->pHydroParam->phi * pUpslope->pHydroParam->initUnSat;
               pUpslope->swcUnsat=pUpslope->pHydroParam->wiltPt + ((pUpslope->pHydroParam->phi - pUpslope->pHydroParam->wiltPt)/2.0f);
               pUpslope->pUnsat->tracerMass = m_initTracerConc/1000.0f * pUpslope->pUnsat->volumeUnsat;
               pUpslope->pUnsat->tracerConc = m_initTracerConc/1000.0f;
               pUpslope->initTracerMass+=pUpslope->pUnsat->tracerMass;
               }
            else
               {
//               pUpslope->pUnsat->unsaturatedDepth = 0.0f;
              // pUpslope->pUnsat->volumeUnsat=0.0f;
              // pUpslope->pUnsat->swcUnsat=0.0f;
               }
            }

        if (m_checkPesticide)
           for (int i=0; i<m_numberPesticides; i++)
              pUpslope->pesticideArray[i]->pVeg->cumMassInput = 0.0f;
			}
      } // end of reach count

   RestoreToModelStructure();


   if (m_checkSimulateWetlands || m_checkIncrFlow)
      ApplyStructuralRestorationOptions();

   /*
      FILE *fp1 = fopen("c:\\temp\\crap.csv", "w" );    
      int count=-1;
      for (int j=0; j < GetReachCount(); j++ )
         {

         ReachNode *pNode =   m_reachTree.m_nodeArray[ j ];    
         REACH_INFO       *pInfo  = &m_reachTree.m_nodeArray[ j ]->m_reachInfo;     
         REACH_INFO_HYDRO *pHydro = m_reachInfoHydroArray[ j ];
         for (int i=0; i< pHydro->upslopeInfoArray.GetSize();i++)
            {
            UPSLOPE_INFO *pUpslope = pHydro->upslopeInfoArray[ i ]; 
            count++;
            for (int l=0;l<pUpslope->neighborArray.GetSize();l++) 
            {
               
               fprintf(fp1,"%i,  %i,%i, %i,%i \n", count,pUpslope->colIndex, pUpslope->rowIndex, pUpslope->neighborArray[l]->colIndex,pUpslope->neighborArray[l]->rowIndex ); // and write them to fp
            }
            }
         }
         fclose(fp1);*/

   }  // end of CalcWatershedInfo

  


void WET_Hydro::UpdatePesticideParameters(PESTICIDE_INFO *pPesticide, UPSLOPE_INFO *pUpslope)
   {  
      //  be certain that the offset is already set (it might make more sense to pass it, then set it here?)
   int offset = pPesticide->offset;
   pPesticide->pesticideName = m_chemicalParamInfoArray[offset]->name;
   pPesticide->pVeg->kf = m_chemicalParamInfoArray[offset]->kf; //foliar rate constant
   float exponent = ((log(m_chemicalParamInfoArray[offset]->koc) + 0.21f - 1.78f)*(log(m_chemicalParamInfoArray[offset]->koc) + 0.21f - 1.78f))/2.44f;
   pPesticide->pVeg->epsilon = 0.784f * (float)exp(-exponent) ; //equation 38 PRZM 1.0 manual(uptake efficiency)
   pPesticide->koc = m_chemicalParamInfoArray[offset]->koc;
   pPesticide->pVeg->e=0.1f; //  washoff fraction, as suggested in PRZM1 manual on page 63

   pPesticide->pSurfWater->kd = pPesticide->koc*pPesticide->oCarbon/1000.0f;
   pPesticide->pSurfWater->ks = (float) m_randUnif1.RandValue(m_chemicalParamInfoArray[offset]->ksMin,m_chemicalParamInfoArray[offset]->ksMax);
   pPesticide->pSurfWater->ks = m_chemicalParamInfoArray[offset]->ksMin;
   pPesticide->pUnsatWater->kd = pPesticide->koc*pPesticide->oCarbon/1000.0f;
   pPesticide->pUnsatWater->ks = (float) m_randUnif2.RandValue(m_chemicalParamInfoArray[offset]->ksMin,m_chemicalParamInfoArray[offset]->ksMax);
   pPesticide->pUnsatWater->ks = m_chemicalParamInfoArray[offset]->ksMin;
   pPesticide->pSatWater->kd = pPesticide->koc*pPesticide->oCarbon/1000.0f;
   pPesticide->pSatWater->ks = (float) m_randUnif3.RandValue(m_chemicalParamInfoArray[offset]->ksMin,m_chemicalParamInfoArray[offset]->ksMax);
   pPesticide->pSatWater->ks = m_chemicalParamInfoArray[offset]->ksMin;
   // Go through the pesticide application database and assign application rates and timing based on input database and
   // local soil and landuse characteristics...

   for (int i=0; i<m_chemicalAppInfoArray.GetSize(); i++)//find the correct name
      {
      if (pPesticide->pesticideName==m_chemicalAppInfoArray[i]->name)
         {
         if (pUpslope->lulc_b==m_chemicalAppInfoArray[i]->lulc_b)
            {
            pPesticide->appRate=(float) m_randUnif1.RandValue(m_chemicalAppInfoArray[i]->rateMin,m_chemicalAppInfoArray[i]->rateMax);
            pPesticide->appDay= (int) m_randUnif2.RandValue(m_chemicalAppInfoArray[i]->beginDay1,m_chemicalAppInfoArray[i]->endDay1);
            pPesticide->appMonth= (int) m_randUnif3.RandValue(m_chemicalAppInfoArray[i]->beginMonth1,m_chemicalAppInfoArray[i]->endMonth1);
            }
         }
      }
   }

float WET_Hydro::CalculateTotalArea( void )
   {
   
   if (m_modelStructureMethod == WH_DISTRIBUTE_FPGRID || m_modelStructureMethod == WH_DISTRIBUTE_WTHEIGHT )
      {//go through the grid and get upslope areas
      MapLayer *pDEM = m_pMap->GetLayer("FLOWDIRX");
      for ( int i=0; i < GetReachCount(); i++ )
		   {
         REACH_INFO       *pInfo  = &m_reachTree.m_nodeArray[ i ]->m_reachInfo;
         REACH_INFO_HYDRO *pHydro = m_reachInfoHydroArray[ i ];
         ASSERT( pHydro == (REACH_INFO_HYDRO*) pInfo->pData );
		   int numUpslopeBins = pHydro->upslopeInfoArray.GetSize()-1;      
		   for ( int j=numUpslopeBins ; j >= 0; j-- )  //be sure to solve upslope first (the are stored downslope first)
            {
            UPSLOPE_INFO *pUpslope = pHydro->upslopeInfoArray[ j ];     
            pUpslope->areaTotal = pDEM->GetTotalUpslopeArea( pUpslope->rowIndex, pUpslope->colIndex );
            if (pUpslope->areaTotal==0.0f)
               pUpslope->areaTotal = 10.0f;
            }
         }
      }
   else
      {
   // get leftmost leaf
      ReachNode *pNode = m_reachTree.FindLeftLeaf();  // find leftmost leaf of entire tree (no params defaults to m_pRoot )
	   REACH_INFO_HYDRO *pHydro = GetReachInfoHydroFromReachID( pNode->m_reachInfo.reachID );
      pHydro->upslopeInfoArray[ 0 ]->areaTotal = pHydro->upslopeInfoArray[ 0 ]->area/1000000.0f;
      pHydro->upslopeInfoArray[ 0 ]->upstreamLength = pNode->m_reachInfo.length/1000.0f;
   	
      while ( pNode->m_pDown->m_reachInfo.index != ROOT_NODE )
         {
         ReachNode *pDown = pNode->m_pDown;

         // if we're coming from the left and a right branch exists, solve the right branch first
         // are we coming from the left? and right branch exists?

         if (( pDown->m_pLeft == pNode ) && ( pDown->m_pRight != NULL ))
            {
            pNode = m_reachTree.FindLeftLeaf( pDown->m_pRight );
			   int reachID = pNode->m_reachInfo.reachID;
			   REACH_INFO_HYDRO *pHydro = GetReachInfoHydroFromReachID( reachID );
			   pHydro->upslopeInfoArray[ 0 ]->areaTotal = pHydro->upslopeInfoArray[ 0 ]->area/1000000.0f;
            pHydro->upslopeInfoArray[ 0 ]->upstreamLength = pNode->m_reachInfo.length/1000.0f;
			   }
         else // We have all upstream orders, so can move downstream.
            {
		      pNode = pDown;
			   int reachID = pNode->m_reachInfo.reachID;
			   if ( reachID == -1 )  //phantom node - we need our right's right and left pointers!
				   {
				   pNode = pNode->m_pDown;    // just move downstream one node
				   REACH_INFO_HYDRO *pHydro = GetReachInfoHydroFromReachID( pNode->m_reachInfo.reachID );
				   pHydro->upslopeInfoArray[ 0 ]->areaTotal = pHydro->upslopeInfoArray[ 0 ]->area/1000000.0f + ( GetReachInfoHydroFromReachID( pNode->m_pRight->m_pRight->m_reachInfo.reachID ))->upslopeInfoArray[ 0 ]->areaTotal + ( GetReachInfoHydroFromReachID( pNode->m_pRight->m_pLeft->m_reachInfo.reachID ))->upslopeInfoArray[ 0 ]->areaTotal + ( GetReachInfoHydroFromReachID( pNode->m_pLeft->m_reachInfo.reachID ))->upslopeInfoArray[ 0 ]->areaTotal;
               pHydro->upslopeInfoArray[ 0 ]->upstreamLength = pNode->m_reachInfo.length/1000.0f + ( GetReachInfoHydroFromReachID( pNode->m_pRight->m_pRight->m_reachInfo.reachID ))->upslopeInfoArray[ 0 ]->upstreamLength+ ( GetReachInfoHydroFromReachID( pNode->m_pRight->m_pLeft->m_reachInfo.reachID ))->upslopeInfoArray[ 0 ]->upstreamLength + ( GetReachInfoHydroFromReachID( pNode->m_pLeft->m_reachInfo.reachID ))->upslopeInfoArray[ 0 ]->upstreamLength;
				   
               }
			   else //Not a phantom so just add up the left, right and current area.
				   {
				   REACH_INFO_HYDRO *pHydro = GetReachInfoHydroFromReachID( pNode->m_reachInfo.reachID );
				   if ( pNode->m_pRight != NULL )
                  {
					   pHydro->upslopeInfoArray[ 0 ]->areaTotal = pHydro->upslopeInfoArray[ 0 ]->area/1000000.0f + ( GetReachInfoHydroFromReachID( pNode->m_pRight->m_reachInfo.reachID ))->upslopeInfoArray[ 0 ]->areaTotal + ( GetReachInfoHydroFromReachID( pNode->m_pLeft->m_reachInfo.reachID ))->upslopeInfoArray[ 0 ]->areaTotal;
					   pHydro->upslopeInfoArray[ 0 ]->upstreamLength = pNode->m_reachInfo.length/1000.0f + ( GetReachInfoHydroFromReachID( pNode->m_pRight->m_reachInfo.reachID ))->upslopeInfoArray[ 0 ]->upstreamLength + ( GetReachInfoHydroFromReachID( pNode->m_pLeft->m_reachInfo.reachID ))->upslopeInfoArray[ 0 ]->upstreamLength;
                  }
               else
                  {
					   pHydro->upslopeInfoArray[ 0 ]->areaTotal = pHydro->upslopeInfoArray[ 0 ]->area/1000000.0f + ( GetReachInfoHydroFromReachID( pNode->m_pLeft->m_reachInfo.reachID ))->upslopeInfoArray[ 0 ]->areaTotal;
					   pHydro->upslopeInfoArray[ 0 ]->upstreamLength = pNode->m_reachInfo.length/1000.0f + ( GetReachInfoHydroFromReachID( pNode->m_pLeft->m_reachInfo.reachID ))->upslopeInfoArray[ 0 ]->upstreamLength;
                  
                  }				  
               }
            }
         }
      }

		for ( int i=0; i < m_pStreamLayer->m_polyArray.GetSize(); i++ )
			{
         REACH_INFO       *pInfo  = &m_reachTree.m_nodeArray[ i ]->m_reachInfo;
   		REACH_INFO_HYDRO *pHydroData = m_reachInfoHydroArray[ i ];
			for ( int j=0; j < pHydroData->upslopeInfoArray.GetSize(); j++)
				{
				UPSLOPE_INFO *pUpslope = pHydroData->upslopeInfoArray[ j ];
				for ( int k=0; k<pUpslope->cellsRepresentedArray.GetSize();k++)
               {
                  m_pCellLayer->SetData(pUpslope->cellsRepresentedArray[k], m_pCellLayer->GetFieldCol("ORDER_") , pInfo->streamOrder);
               m_pCellLayer->SetData(pUpslope->cellsRepresentedArray[k], m_pCellLayer->GetFieldCol("DIST_PEST") , pUpslope->areaTotal);
               m_pCellLayer->SetData(pUpslope->cellsRepresentedArray[k], m_pCellLayer->GetFieldCol("DIST_SED") , pUpslope->upstreamLength);
               }
            }
         }

   return -5;
   }


void WET_Hydro::InitUpslopeDataCollection()
   {
   m_timeOffset=0;
   float initSoilVol=0.0f;
   float initTracerMass=0.0f;
   CalcInitSoilVol(initSoilVol,initTracerMass);
   if (m_checkTracerModel)
      CalcInitGridTracerMass();
   m_massBalanceInfo.initialTracerMass=initTracerMass;
   //  Set up Upslope level Data Collection
   int dataToCollect=0;
   if (m_modelStructureMethod==WH_DISTRIBUTE_FPGRID || m_modelStructureMethod==WH_DISTRIBUTE_WTHEIGHT)
      {
      dataToCollect=(m_cellDataArray.GetSize()/2);
      dataToCollect++;
      }

   else
      {
      dataToCollect = m_cellDataArray.GetSize();
      dataToCollect++;  // include time
      }

   if ( m_pSwcData != NULL && m_checkMonteCarlo==WH_MONTE_CARLO )
      {
      delete m_pSwcData;
      m_pSwcData=NULL;
      }
   m_pSwcData = new FDataObj( dataToCollect, 0 );
   ASSERT( m_pSwcData != NULL );



   if ( m_pRateData != NULL && m_checkMonteCarlo==WH_MONTE_CARLO )
      {
      delete m_pRateData;
      m_pRateData=NULL;
      }
   m_pRateData = new FDataObj( ((dataToCollect -1)*3) + 1, 0 );
   ASSERT( m_pRateData != NULL );



   if ( m_pPestVegData != NULL && m_checkMonteCarlo==WH_MONTE_CARLO )
      {
      delete m_pPestVegData;
      m_pPestVegData=NULL;
      }
   m_pPestVegData = new FDataObj( ((dataToCollect -1)*m_numberPesticides) + 1, 0 );
   ASSERT( m_pPestVegData != NULL );




   if ( m_pPestSurfData != NULL && m_checkMonteCarlo==WH_MONTE_CARLO )
      {
      delete m_pPestSurfData;
      m_pPestSurfData=NULL;
      }
   m_pPestSurfData = new FDataObj( ((dataToCollect -1)*m_numberPesticides) + 1, 0 );
   ASSERT( m_pPestSurfData != NULL );

   if ( m_pPestUnsatData != NULL && m_checkMonteCarlo==WH_MONTE_CARLO )
      {
      delete m_pPestUnsatData;
      m_pPestUnsatData=NULL;
      }
   m_pPestUnsatData = new FDataObj( ((dataToCollect -1)*m_numberPesticides) + 1, 0 );
   ASSERT( m_pPestUnsatData != NULL );

   if ( m_pPestSatData != NULL && m_checkMonteCarlo==WH_MONTE_CARLO )
      {
      delete m_pPestSatData;
      m_pPestSatData=NULL;
      }
   m_pPestSatData = new FDataObj( ((dataToCollect -1)*m_numberPesticides) + 1, 0 );
   ASSERT( m_pPestSatData != NULL );

   if ( m_pPestMassBalanceData != NULL && m_checkMonteCarlo==WH_MONTE_CARLO )
      {
      delete m_pPestMassBalanceData;
      m_pPestMassBalanceData=NULL;
      }
   m_pPestMassBalanceData = new FDataObj( ((dataToCollect -1)*m_numberPesticides * 9) + 1, 0 );
   ASSERT( m_pPestMassBalanceData != NULL );


   if ( m_pUnsatDepthData != NULL && m_checkMonteCarlo==WH_MONTE_CARLO )
      {
      delete m_pUnsatDepthData;
      m_pUnsatDepthData=NULL;
      }
   m_pUnsatDepthData = new FDataObj( dataToCollect, 0 );
   ASSERT( m_pUnsatDepthData != NULL );

   if ( m_pSatDepthData != NULL && m_checkMonteCarlo==WH_MONTE_CARLO )
      {
      delete m_pSatDepthData;
      m_pSatDepthData=NULL;
      }
   m_pSatDepthData = new FDataObj( dataToCollect, 0 );
   ASSERT( m_pSatDepthData != NULL );

   if ( m_pSatDeficitData != NULL && m_checkMonteCarlo==WH_MONTE_CARLO )
      {
      delete m_pSatDeficitData;
      m_pSatDeficitData=NULL;
      }
   m_pSatDeficitData = new FDataObj( dataToCollect, 0 );
   ASSERT( m_pSatDeficitData != NULL );

   if ( m_pTracerData != NULL && m_checkMonteCarlo==WH_MONTE_CARLO )
      {
      delete m_pTracerData;
      m_pTracerData=NULL;
      }
   m_pTracerData = new FDataObj( dataToCollect, 0 );
   ASSERT( m_pTracerData != NULL );
   
   if ( m_pTracerConcData != NULL && m_checkMonteCarlo==WH_MONTE_CARLO )
      {
      delete m_pTracerConcData;
      m_pTracerConcData=NULL;
      }
   m_pTracerConcData = new FDataObj( dataToCollect, 0 );
   ASSERT( m_pTracerConcData != NULL );

   if ( m_pETData != NULL && m_checkMonteCarlo==WH_MONTE_CARLO )
      {
      delete m_pETData;
      m_pETData = NULL;
      }
   m_pETData = new FDataObj( dataToCollect, 0 );
   ASSERT( m_pETData != NULL );

   if ( m_pUplandSedimentData != NULL && m_checkMonteCarlo==WH_MONTE_CARLO )
      {
      delete m_pUplandSedimentData;
      m_pUplandSedimentData=NULL;
      }
   m_pUplandSedimentData = new FDataObj( dataToCollect, 0);
   ASSERT( m_pUplandSedimentData != NULL) ;

   if ( m_pModelClimateData != NULL && m_checkMonteCarlo==WH_MONTE_CARLO )
      {
      delete m_pModelClimateData;
      m_pModelClimateData = NULL;
      }
   m_pModelClimateData = new FDataObj( dataToCollect, 0 );
   ASSERT( m_pModelClimateData != NULL );

   // add labels   
   m_pSwcData->SetLabel( 0, "Time (Days)" );
   m_pRateData->SetLabel( 0, "Time (Days)" );
   m_pPestVegData->SetLabel( 0, "Time (Days)");
   m_pPestSurfData->SetLabel( 0, "Time (Days)");
   m_pPestUnsatData->SetLabel( 0, "Time (Days)");
   m_pPestSatData->SetLabel( 0, "Time (Days)");
   m_pPestMassBalanceData->SetLabel( 0, "Time (Days)");
   m_pUnsatDepthData->SetLabel(0, "Time (Days)");
   m_pSatDepthData->SetLabel(0, "Time (Days)");
   m_pSatDeficitData->SetLabel(0, "Time (Days)");
   m_pTracerData->SetLabel( 0, "Time (Days)" );
   m_pTracerConcData->SetLabel( 0, "Time (Days)" );
   m_pETData->SetLabel( 0, "Time (Days) " );
   m_pUplandSedimentData->SetLabel(0, "Time (Days)");
   m_pModelClimateData->SetLabel(0, "Time (Days)");

   int pestCount=0;
   int pestCount1=0;
   for (int i=1; i < dataToCollect ; i++ )
      {
      for (int l=0; l<m_numberPesticides;l++)
         {
         CString label;
         CString pestLabel;
         CString Pest = m_chemicalParamInfoArray[m_pesticideOffsets[l]]->name;
         label.Format( "C%i ", m_cellDataArray[ i-1 ] );
         pestLabel.Format( "C%i ", m_cellDataArray[ i-1 ] );
         label+=Pest;
         pestLabel+=Pest;
         pestCount++;

         m_pPestVegData->SetLabel( pestCount, (char*)(LPCSTR) label );
         m_pPestSurfData->SetLabel( pestCount, (char*)(LPCSTR) label );
         m_pPestUnsatData->SetLabel( pestCount, (char*)(LPCSTR) label );
         m_pPestSatData->SetLabel( pestCount, (char*)(LPCSTR) label );  

         CString mass;
         mass.Format("Erosion");
         pestLabel=label+mass;
         pestCount1++;
         m_pPestMassBalanceData->SetLabel(pestCount1, (char*)(LPCSTR) pestLabel);
         mass.Format("Surface Input");
         pestLabel=label+mass;
         pestCount1++;
         m_pPestMassBalanceData->SetLabel(pestCount1, (char*)(LPCSTR) pestLabel);
         mass.Format("Veg Input");
         pestLabel=label+mass;
         pestCount1++;
         m_pPestMassBalanceData->SetLabel(pestCount1, (char*)(LPCSTR) pestLabel);
         mass.Format("Veg Transform");
         pestLabel=label+mass;
         pestCount1++;
         m_pPestMassBalanceData->SetLabel(pestCount1, (char*)(LPCSTR) pestLabel);
         mass.Format("Surface Runoff");
         pestLabel=label+mass;
         pestCount1++;
         m_pPestMassBalanceData->SetLabel(pestCount1, (char*)(LPCSTR) pestLabel);
         mass.Format("Root Zone Transform");
         pestLabel=label+mass;
         pestCount1++;
         m_pPestMassBalanceData->SetLabel(pestCount1, (char*)(LPCSTR) pestLabel);
         mass.Format("Leached Transform");
         pestLabel=label+mass;
         pestCount1++;
         m_pPestMassBalanceData->SetLabel(pestCount1, (char*)(LPCSTR) pestLabel);
         mass.Format("Surface Transform");
         pestLabel=label+mass;
         pestCount1++;
         m_pPestMassBalanceData->SetLabel(pestCount1, (char*)(LPCSTR) pestLabel);
         mass.Format("Root Zone Uptake");
         pestLabel=label+mass;
         pestCount1++;
         m_pPestMassBalanceData->SetLabel(pestCount1, (char*)(LPCSTR) pestLabel);
         mass.Format("Surface Zone Uptake");
         pestLabel=label+mass;
         pestCount1++;
         m_pPestMassBalanceData->SetLabel(pestCount1, (char*)(LPCSTR) pestLabel);
         }
      }
   if (m_modelStructureMethod==WH_DISTRIBUTE_FPGRID || m_modelStructureMethod==WH_DISTRIBUTE_WTHEIGHT)
      {
      int pestCount=0;
      int j=1;
      for (int i=1; i < dataToCollect; i++ )
         {
         for (int l=0; l<m_numberPesticides;l++)
            {
            //CString pestLabel;
            CString Pest = m_chemicalParamInfoArray[m_pesticideOffsets[l]]->name;
            CString label;
            label.Format( "C%i,%i  ", m_cellDataArray[ j-1 ],m_cellDataArray[ j ] );
            //pestLabel.Format( "C%i,%i  ", m_cellDataArray[ j-1 ],m_cellDataArray[ j ]  );
            label+=Pest;
            //pestLabel+=Pest;
            pestCount++;

            m_pPestVegData->SetLabel( pestCount, (char*)(LPCSTR) label );
            m_pPestSurfData->SetLabel( pestCount, (char*)(LPCSTR) label );
            m_pPestUnsatData->SetLabel( pestCount, (char*)(LPCSTR) label );
            m_pPestSatData->SetLabel( pestCount, (char*)(LPCSTR) label );           

            
            }
         j=j+2;
         }
      }


   if (m_modelStructureMethod==WH_DISTRIBUTE_FPGRID || m_modelStructureMethod==WH_DISTRIBUTE_WTHEIGHT)
      {
      int j=1;
      for (int i=1; i < dataToCollect; i++ )
         {
         CString label;
         label.Format( "C%i,%i  ", m_cellDataArray[ j-1 ],m_cellDataArray[ j ] );
         
         m_pSwcData->SetLabel( i, (char*)(LPCSTR) label );    
         m_pUnsatDepthData->SetLabel( i, (char*)(LPCSTR) label );
         m_pSatDepthData->SetLabel( i, (char*)(LPCSTR) label );
         m_pSatDeficitData->SetLabel( i, (char*)(LPCSTR) label );
		   m_pModelClimateData->SetLabel( i, (char*)(LPCSTR) label );
         m_pTracerData->SetLabel(i, (char*)(LPCSTR) label);
         m_pTracerConcData->SetLabel(i, (char*)(LPCSTR) label);
         m_pETData->SetLabel( i, (char*)(LPCSTR) label );
         m_pUplandSedimentData->SetLabel( i, (char*)(LPCSTR) label );
         j=j+2;
        
         }
      }
   int count=1;

      for (int i=1; i < dataToCollect; i++ )
         {
         CString label;
         label.Format( "C%i min", m_cellDataArray[ i-1 ] );
         m_pRateData->SetLabel(count, (char*)(LPCSTR) label);
         count++;
         label.Format( "C%i max", m_cellDataArray[ i-1 ] );
         m_pRateData->SetLabel(count, (char*)(LPCSTR) label);
         count++;
         label.Format( "C%i actual", m_cellDataArray[ i-1 ] );
         m_pRateData->SetLabel(count, (char*)(LPCSTR) label);
         count++;
         }
      
}

void WET_Hydro::InitInstreamDataCollection()
{
   //////////////////////////////////////////////////////////////
   // set up data collection for runoff data (reach-level)

   int dataToCollect = m_reachDataArray.GetSize()+1;  


   if ( m_pInstreamSedimentData != NULL && m_checkMonteCarlo==WH_MONTE_CARLO )
      {
      delete m_pInstreamSedimentData;
      m_pInstreamSedimentData = NULL;
      }
   m_pInstreamSedimentData = new FDataObj( dataToCollect, 0);
   ASSERT( m_pInstreamSedimentData != NULL );
   m_pInstreamSedimentData->SetLabel( 0, "Time (Days) " );
   for (int p=1; p < dataToCollect; p++ )
      {
      CString label;
      label.Format( "R%p ", m_reachDataArray[ p-1 ]);
      m_pInstreamSedimentData->SetLabel( p, (char*)(LPCSTR) label );
      }


// set up pesticide instream data - remember there are potentially >1 pesticides simulated!
   if ( m_pPestInstreamData != NULL && m_checkMonteCarlo==WH_MONTE_CARLO )
      {
      delete m_pPestInstreamData;
      m_pPestInstreamData = NULL;
      }
   m_pPestInstreamData = new FDataObj( ((dataToCollect-1)*m_numberPesticides )+1, 0);
   ASSERT( m_pPestInstreamData != NULL );
   m_pPestInstreamData->SetLabel( 0, "Time (Days) " );
   int pestCount=0;
   for ( int i=1; i < dataToCollect; i++ )
      {
      for (int l=0; l<m_numberPesticides;l++)
         {
         CString label;
         CString Pest = m_chemicalParamInfoArray[m_pesticideOffsets[l]]->name;
         label.Format( "R%i ", m_reachDataArray[ i-1 ]);
         label+=Pest;
         pestCount++;
         m_pPestInstreamData->SetLabel( pestCount, (char*)(LPCSTR) label );

         }
      }

   //////////////////////////////////////////////////////////////
   // set up data collection for Discharge DATA and Tracer Concentration Data for each reach
   dataToCollect++;  //an extra column for measured data
   
   if ( m_pInstreamData != NULL && m_checkMonteCarlo==WH_MONTE_CARLO)
      {
      delete m_pInstreamData;
      m_pInstreamData = NULL;
      if (m_checkTracerModel)
         {
         delete m_pTracerConcChannelData;
         m_pTracerConcChannelData = NULL;
         }
      }
   m_pInstreamData = new FDataObj( dataToCollect , 0 );  
   m_pTracerConcChannelData = new FDataObj( dataToCollect , 0 );   
   if ( m_pTracerConcChannelData != NULL && m_checkMonteCarlo==WH_MONTE_CARLO )
      {
      delete m_pTracerConcChannelData;
      delete m_pTracerMassChannelData;
      m_pTracerConcChannelData = NULL;
      m_pTracerMassChannelData=NULL;
      }
   m_pTracerConcChannelData = new FDataObj( dataToCollect, 0 ); //asssuming we will add measured 
   m_pTracerMassChannelData = new FDataObj( dataToCollect, 0 ); //asssuming we will add measured 
   ASSERT( m_pTracerConcChannelData != NULL );
   ASSERT( m_pInstreamData != NULL );
   m_pInstreamData->SetLabel( 0, "Time (Days)" );
   m_pInstreamData->SetLabel(dataToCollect-1, "Measured");
   m_pTracerConcChannelData->SetLabel( 0, "Time (Days)" );
   m_pTracerMassChannelData->SetLabel( 0, "Time (Days)" );
   m_pTracerConcChannelData->SetLabel(dataToCollect-1, "Measured");
   
   for (int q=1; q < dataToCollect-1; q++ )
      {
      CString label;
      label.Format( "R%i ", m_reachDataArray[ q-1 ]);
      m_pInstreamData->SetLabel( q, (char*)(LPCSTR) label );
      m_pTracerConcChannelData->SetLabel( q, (char*)(LPCSTR) label );
      m_pTracerMassChannelData->SetLabel( q, (char*)(LPCSTR) label );
      }

   //////////////////////////////////////////////////////////////
   // set up data collection for TIMESTEP data 
   if ( m_pGeneralData != NULL && m_checkMonteCarlo==WH_MONTE_CARLO )
      {
      delete m_pGeneralData;
      m_pGeneralData = NULL;
      }
   m_pGeneralData = new FDataObj( 2, 0 );  // time, timeStep
   ASSERT( m_pGeneralData != NULL );

   // add labels   
   m_pGeneralData->SetLabel( 0, "Time (Days)" );
   m_pGeneralData->SetLabel( 1, "Time Step" );

   if ( m_pMassBalanceData != NULL && m_checkMonteCarlo ==WH_MONTE_CARLO )
      {
      delete m_pMassBalanceData;   
      m_pMassBalanceData = NULL;
      }
   m_pMassBalanceData = new FDataObj( 4, 0 );  // time, timeStep
   ASSERT( m_pMassBalanceData != NULL );

   // add labels   
   m_pMassBalanceData->SetLabel( 0, "Time (Days)" );
   m_pMassBalanceData->SetLabel( 1, "ET" );
   m_pMassBalanceData->SetLabel( 2, "Precip" );
   m_pMassBalanceData->SetLabel( 3, "Discharge" );

   if ( m_pTotalSedimentOutflowData != NULL && m_checkMonteCarlo==WH_MONTE_CARLO )
      {
      delete m_pTotalSedimentOutflowData;
      m_pTotalSedimentOutflowData = NULL;
      }
   m_pTotalSedimentOutflowData = new FDataObj(2,0);
   ASSERT( m_pTotalSedimentOutflowData != NULL );
   m_pTotalSedimentOutflowData->SetLabel(0, "Time (Days)");
   m_pTotalSedimentOutflowData->SetLabel(1, "Sediment (kg)");

   if ( m_pSedimentOutflowData != NULL && m_checkMonteCarlo==WH_MONTE_CARLO )
      {
      delete m_pSedimentOutflowData;
      m_pSedimentOutflowData = NULL;
      }
   m_pSedimentOutflowData = new FDataObj(2,0);
   ASSERT( m_pSedimentOutflowData != NULL );
   m_pSedimentOutflowData->SetLabel(0, "Time (Days)");
   m_pSedimentOutflowData->SetLabel(1, "Sediment (kg)");

   if ( m_pUplandStorageData != NULL && m_checkMonteCarlo==WH_MONTE_CARLO )
      {
      delete m_pUplandStorageData;
      m_pUplandStorageData = NULL;
      }
   m_pUplandStorageData = new FDataObj( 6, 0 );  // time, timeStep
   ASSERT( m_pUplandStorageData != NULL );

   // add labels   
   m_pUplandStorageData->SetLabel( 0, "Time (Days)" );
   m_pUplandStorageData->SetLabel( 1, "Soil Water Vol" );
   m_pUplandStorageData->SetLabel( 2, "Ponded Vol" );
   m_pUplandStorageData->SetLabel( 3, "Precip Vol" );
   m_pUplandStorageData->SetLabel( 4, "ET Vol" );
   m_pUplandStorageData->SetLabel( 5, "Ground Vol" );

   if ( m_pReachStatistics != NULL && m_checkMonteCarlo==WH_MONTE_CARLO )
      {
      delete m_pReachStatistics;
      m_pReachStatistics = NULL;
      }
   m_pReachStatistics = new FDataObj( 9, 0 );  
   ASSERT( m_pReachStatistics != NULL );

   // add labels   
   m_pReachStatistics->SetLabel( 0, "ReachOffset" );
	m_pReachStatistics->SetLabel( 1, "ReachID" );
   m_pReachStatistics->SetLabel( 2, "Order" );
   m_pReachStatistics->SetLabel( 3, "Area" );
   m_pReachStatistics->SetLabel( 4, "AreaTotal" );
   m_pReachStatistics->SetLabel( 5, "TimeToPeak" );
   m_pReachStatistics->SetLabel( 6, "LateralInflow" );
   m_pReachStatistics->SetLabel( 7, "ChannelInflow" );
	m_pReachStatistics->SetLabel( 8, "ChannelOutflow" );

 
   if ( m_pClimateData == NULL && m_checkMonteCarlo==WH_MONTE_CARLO )
		{
		CString msg;
		msg.Format("No Climate Data found in ini file - precip will be set to 0. ");
		int retCode = gpMain->MessageBox( msg,"Check Ini file",MB_OK );
		}
		
	if ( m_pMeasuredEtData == NULL && m_checkMonteCarlo==WH_MONTE_CARLO)
		{
	/*	CString msg;
		msg.Format("The ini file did not specify measured ET data.  Will Simulate.");
		if (m_checkMonteCarlo)
			int retCode = gpMain->MessageBox( msg,"Check Ini file",MB_OK);
		
        */}
         }

void WET_Hydro::InitMovieCapture()
         {

     if (m_saveEmfOutput)
         {
         // init movie capturing
         
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
         }

void WET_Hydro::GetStateVarCount()
  {
  //Now we are going to allocate the Arrays of state variables.  The size of these arrays
   //depend on what is being simulated.

   m_svCount=0;
   m_svCountStream=0; 
   m_uplandUnitCount = 0;
   for (int i=0; i < GetReachCount(); i++ ) 
      {
      int numUpslopeInfo = m_reachInfoHydroArray[i]->upslopeInfoArray.GetSize();
      int subnodeCount = m_reachTree.m_nodeArray[ i ]->m_reachInfo.subnodeCount;
      m_uplandUnitCount += numUpslopeInfo;// We are going to integrate volumeSat
      m_svCount+=numUpslopeInfo; 
      if (m_checkMassBalance)
         m_svCount+=(numUpslopeInfo*2) ; // and also the accumulated loss to groundwater and accumulative precipitation volumeSat !

      if (m_checkUnsaturated)      // and maybe volumeUnsat in the saturated zone                         
          m_svCount += numUpslopeInfo;

      if (m_pMeasuredEtData != NULL || m_checkEtModel)     // only one additional state variable - accumulated et volumeSat
         {
         if (m_checkMassBalance)
            m_svCount+= numUpslopeInfo;
         }

      if (m_checkTracerModel)//There are 2 state variables from the tracer model (mass and accumulated mass in precip)
         {
         m_svCount+=numUpslopeInfo;//this one for the mass
        
         if (m_checkMassBalance)
            m_svCount+= numUpslopeInfo;//this one for the accumulated mass
         m_svCountStream+=subnodeCount;
         if (m_checkUnsaturated) // there is also the possibility of a 3rd - if the unsaturated zone (and tracer in it) is simulated
            m_svCount+= numUpslopeInfo;
         }
      if (m_checkSnow)
         m_svCount+= numUpslopeInfo;
      if (m_checkInterception)
         m_svCount+= numUpslopeInfo;
                 
      if (m_checkUplandSediment)  // sediment mass and cumulative sediment outflow
         {
         m_svCount+=numUpslopeInfo;
         m_svCountStream+=subnodeCount;
         }

      if (m_checkPesticide)  // there are 4 states in the pesticide model (massVeg, MassSurf, MassUnsat,MassSat
         {                        //  .  each for m_numberPesticides
         m_svCount+=numUpslopeInfo*4*m_numberPesticides;
         m_svCountStream+=(subnodeCount*m_numberPesticides);
         }

      if (m_checkTill)
		m_svCount+= numUpslopeInfo;
      }
   }


void WET_Hydro::AllocateStateVarArray()
  {

//  The state var array is an array of pointers to floats:
   if (m_stateVar != NULL)
      {
      delete m_stateVar;
      m_stateVar = NULL;
      }
   
   m_stateVar = new float*[ m_svCount ] ;
   
   if (m_stateVarStream != NULL)
      {
      delete m_stateVarStream;
      m_stateVarStream = NULL;
      }
   
   m_stateVarStream = new float*[ m_svCountStream ];
   int reachCount = GetReachCount();
   int counter=0;
   int countStream=0;


//  Initialize this array with values from Upslope_Info structures
   for (int i=0; i < reachCount; i++ )
		{
      REACH_INFO_HYDRO *pHydro = m_reachInfoHydroArray[ i ];

      for (int j=pHydro->upslopeInfoArray.GetSize()-1; j>=0 ; j--)
         {
         if (m_checkUnsaturated)
            {
            m_stateVar[counter] = &pHydro->upslopeInfoArray[j]->pUnsat->volumeUnsat;
            counter++;
            }
         m_stateVar[counter] = &pHydro->upslopeInfoArray[j]->volumeSat;
         counter++;
         if (m_checkMassBalance)
            {
            m_stateVar[counter] = &pHydro->upslopeInfoArray[j]->cumPrecipVol;
            counter++;
            m_stateVar[counter] = &pHydro->upslopeInfoArray[j]->cumGroundLoss;
            counter++;
            }
         if (m_pMeasuredEtData != NULL || m_checkEtModel)
            {
            if (m_checkMassBalance)
               {
               m_stateVar[counter] = &pHydro->upslopeInfoArray[j]->cumETVol;
               counter++;
               }
            }

         if (m_checkTracerModel)
            {
            if (m_checkMassBalance)
               {
               m_stateVar[counter] = &pHydro->upslopeInfoArray[j]->cumTracerMassInPrecip;
               counter++;
               }
            m_stateVar[counter] = &pHydro->upslopeInfoArray[j]->tracerMass;
            counter++;

            if (m_checkUnsaturated)
               {
               m_stateVar[counter] = &pHydro->upslopeInfoArray[j]->pUnsat->tracerMass;
               counter++;
               }

            }
         if (m_checkSnow)
            {
            m_stateVar[counter] = &pHydro->upslopeInfoArray[j]->volumeSWE;
            counter++;
            }
         if (m_checkInterception)
            {
            m_stateVar[counter] = &pHydro->upslopeInfoArray[j]->volumeVeg;
            counter++;
            }

         if (m_checkUplandSediment)
            {
            m_stateVar[counter] = &pHydro->upslopeInfoArray[j]->sedimentMass;            
            counter++;
            }
         if (m_checkPesticide)
            {
            for (int i=0; i < m_numberPesticides; i++)
               {
               m_stateVar[counter] = &pHydro->upslopeInfoArray[j]->pesticideArray[i]->pVeg->mass;
               counter++;
               m_stateVar[counter] = &pHydro->upslopeInfoArray[j]->pesticideArray[i]->pSurfWater->mass;
               counter++;
               m_stateVar[counter] = &pHydro->upslopeInfoArray[j]->pesticideArray[i]->pUnsatWater->mass;
               counter++;
               m_stateVar[counter] = &pHydro->upslopeInfoArray[j]->pesticideArray[i]->pSatWater->mass;
               counter++;
               }

            }
         if (m_checkTill)
            {
            m_stateVar[counter] = &pHydro->upslopeInfoArray[j]->pTill->volumeTill;
            counter++;
            }
         }// end UpslopeInfoArray
      } // end of reachCount
      
  //  now populate a different statevar array with state variables related to the
  //  Instream model.  Q is calculated separately because it is a pde.  This array maintains
  //  the state of other instream variables that are dependent upon Q.

   ReachNode *pNode = m_reachTree.FindLeftLeaf();  // find leftmost leaf of entire tree
   while ( pNode != NULL )  
      {
      REACH_INFO_HYDRO *pHydro = (REACH_INFO_HYDRO*)pNode->m_reachInfo.pData;
      if (m_checkTracerModel)
         {
         for (int j=0; j<pNode->m_reachInfo.subnodeCount;j++)
            {  
            m_stateVarStream[countStream]=&pHydro->tracerMassArray[j];
            countStream++;
            }  
         }
      if (m_checkUplandSediment)
         {
         for (int j=0; j<pNode->m_reachInfo.subnodeCount;j++)
            {
            m_stateVarStream[countStream]=&pHydro->sedimentMassArray[j];
            countStream++;
            }
         }

      if (m_checkPesticide)
         {
         for (int i=0; i<m_numberPesticides; i++)
            {
            for (int j=0; j<pNode->m_reachInfo.subnodeCount;j++)
               {
               m_stateVarStream[countStream]=&pHydro->instreamPesticideArray[i]->pesticideMassArray[j];
               countStream++;
               }
            }
         }   
      // next, move to the next downstream node
      ReachNode *pDown = pNode->m_pDown;

      if ( pDown == NULL ) // root of tree reached?
         break;

      // if we're coming from the left and a right branch exists, solve the right branch first
      // are we coming from the left?  and    right branch exists?
      if ( ( pDown->m_pLeft == pNode ) && ( pDown->m_pRight != NULL ) )
         pNode = m_reachTree.FindLeftLeaf( pDown->m_pRight );
      else
         pNode = pDown;
      }
      }
//--------------------------------------------------------------------------------------
// WET_Hydro::InitRun
//
//  
// This method sets the model to collect output data at user specified points.
//
//--------------------------------------------------------------------------------------void WET_Hydro::InitRun( void ) 

void WET_Hydro::InitRun()
{
   InitUpslopeDataCollection();
   InitInstreamDataCollection();
   GetStateVarCount();
   AllocateStateVarArray();
   if (m_saveEmfOutput)
      InitMovieCapture();

   m_massBalanceInfo.freqSat=0.0f;

//  Allocate the structures and array to contain pesticide mass balance info...
   if (m_checkPesticide)
      {
      for (int i=0; i<m_numberPesticides; i++)
         {
         PESTICIDE_MASS_BALANCE_INFO *pPest = new PESTICIDE_MASS_BALANCE_INFO;
         m_pesticideMassBalanceArray.Add(pPest);
         }
      }
   }


float WET_Hydro::GetDepthFromQ( ReachNode *pNode, float Q, float wdRatio )  // ASSUMES A SPECIFIC CHANNEL GEOMETRY
   {
  // from kellie:  d = ( 5/9 Q n s^2 ) ^ 3/8   -- assumes width = 3*depth, rectangular channel
	REACH_INFO_HYDRO *pHydro = GetReachInfoHydroFromReachID( pNode->m_reachInfo.reachID );   
//	float depth = (float) sqrt( Q / ( 3 * 1.0f ) );
//	float depth = (float) pow( ( 0.028f * Q * pHydro->n * pow(pNode->m_reachInfo.slope, 2 ) ),  3.0f/8.0f );
//float depth = (float) pow( (Q * pHydro->n / sqrt(pNode->m_reachInfo.slope) ) / (float)(pow(wdRatio/(wdRatio + 2.0f), 2.0f/3.0f)*wdRatio ) ,  3.0f/8.0f );
//	float velocity = Q/(depth*wdRatio*depth);
   float wdr = wdRatio;
   float wdterm = (float) pow( (wdr/( 2 + wdr )), 2.0f/3.0f)*wdr;
   float depth  = (float) pow(((Q*pHydro->pInstreamHydroParam->n)/((float)sqrt(pNode->m_reachInfo.slope)*wdterm)), 3.0f/8.0f);
//   float depth = (float) pow( (Q * pHydro->n / sqrt(pNode->m_reachInfo.slope) ) / (float)(pow(wdRatio/(wdRatio + 2.0f), 2.0f/3.0f)*wdRatio ) ,  3.0f/8.0f );
   return (float) depth;
   }
float WET_Hydro::GetVolumeFromQ( ReachNode *pNode, float Q, float wdRatio )  // ASSUMES A SPECIFIC CHANNEL GEOMETRY
   {
	REACH_INFO_HYDRO *pHydro = GetReachInfoHydroFromReachID( pNode->m_reachInfo.reachID );   
   float depth = (float) pow( (Q * pHydro->pInstreamHydroParam->n / sqrt(pNode->m_reachInfo.slope) ) / (float)(pow(wdRatio/(wdRatio + 2.0f), 2.0f/3.0f)*wdRatio ) ,  3.0f/8.0f );
   float width = depth * wdRatio;
   float volumeSat = width * depth * pNode->m_reachInfo.length;
   return (float) volumeSat;
   }

float WET_Hydro::GetKinematicWaveCelerityFromDepth( ReachNode *pNode , float depth)
	{
	REACH_INFO_HYDRO *pHydro = GetReachInfoHydroFromReachID( pNode->m_reachInfo.reachID );  
   float KinematicCelerity = (float)sqrt(pNode->m_reachInfo.slope)* (float)pow(depth,0.66f) * (5.0f/3.0f) / pHydro->pInstreamHydroParam->n ;
	return KinematicCelerity;
	}

void WET_Hydro::SetReachGeometry( ReachNode *pNode, int node, float wdRatio )
   {
   REACH_INFO_HYDRO *pHydro = (REACH_INFO_HYDRO*) pNode->m_reachInfo.pData;
   pHydro->depth = GetDepthFromQ( pNode, pNode->m_reachInfo.qArray[ node ], wdRatio );
   pHydro->width = wdRatio * pHydro->depth;
   }

//  Restoration Options are defined within the Cell Coverage. This method converts
//  restoration options for cells into values that are meaningful within a model
//  structure context.  Called from within the CalcWatershedInfo method.

void WET_Hydro::RestoreToModelStructure()
   {
   int reachCount = GetReachCount();
   for (int i=0; i < reachCount; i++ )
      {
      REACH_INFO       *pInfo  = &m_reachTree.m_nodeArray[ i ]->m_reachInfo;
      REACH_INFO_HYDRO *pHydro = m_reachInfoHydroArray[ i ];
      float areaAltered = 0.0f;
      float areaTotal = pInfo->length * m_bufferWidth*2;//area both sides buffered?
      for ( int j=0; j < pHydro->upslopeInfoArray.GetSize(); j++ )
         {
         UPSLOPE_INFO *pUpslope = pHydro->upslopeInfoArray[ j ];
         for (int k=0; k < pUpslope->cellsRepresentedArray.GetSize();k++)
            {
            int altLulc = GetCellAltLulc(pUpslope->cellsRepresentedArray[k]);
//            int altern = GetCellAltern(pUpslope->cellsRepresentedArray[k]);
            float altArea = GetCellAltArea(pUpslope->cellsRepresentedArray[k]);
            if (altArea == 0.0f)
               altArea = GetCellArea(pUpslope->cellsRepresentedArray[k]);
            if (altLulc == 100 || altLulc == 101 || altLulc == 102 )  //buffered landuse
               areaAltered += altArea;//assuming altArea was multiply by 2 in RESTORE (so isn't here)
            }
         }
      //  length of stream over which buffers were applied
      
      pHydro->percentLengthAltered = areaAltered / areaTotal;
      if (pHydro->percentLengthAltered > 1.0f)
         pHydro->percentLengthAltered = 1.0f;
      }
   
   }

void WET_Hydro::GetPesticideAppTiming()
   {
   COleDateTime t( m_time );
   int simMonth = t.GetMonth();
   int simDay = t.GetDay();
   int simHour = t.GetHour();
   int incrFlowBeginMonth = 12;
   int incrFlowBeginDay = 8;
   if ( simMonth == incrFlowBeginMonth && simDay == incrFlowBeginDay )
      GetPesticideAppRateSimple();
   

   }
void WET_Hydro::GetPesticideAppRate()
   {
   COleDateTime t( m_time );
   int simMonth = t.GetMonth();
   int simDay = t.GetDay();
   int simHour = t.GetHour();
   int reachCount = GetReachCount();
   for (int i=0; i < reachCount; i++ )
      {
      REACH_INFO       *pInfo  = &m_reachTree.m_nodeArray[ i ]->m_reachInfo;
      REACH_INFO_HYDRO *pHydro = m_reachInfoHydroArray[ i ];

      for ( int j=0; j < pHydro->upslopeInfoArray.GetSize(); j++ )
         {
         UPSLOPE_INFO *pUpslope = pHydro->upslopeInfoArray[ j ];
         
         for (int k=0;k<pUpslope->pesticideArray.GetSize();k++)
            {
            PESTICIDE_INFO *pPest = pUpslope->pesticideArray[k];
            
            if (pPest->appMonth == simMonth && pPest->appDay == simDay)
               {
               float percentGroundApplied=0.7f;
               float percentVegApplied = 1.0f-percentGroundApplied;
               float appRatem2= (pPest->appRate/10000.0f) ; //kg/m2
               pUpslope->pesticideArray[k]->pSurfWater->rateMassApp = appRatem2*pUpslope->area* percentGroundApplied; //  in kg/day!
               pUpslope->pesticideArray[k]->pVeg->rateMassApp = appRatem2*pUpslope->area * percentVegApplied;
               }
            else // reset app values back to zero 
               {
               pUpslope->pesticideArray[k]->pSurfWater->rateMassApp = 0.0f; //  in kg/day!
               pUpslope->pesticideArray[k]->pVeg->rateMassApp = 0.0f;
               }
            }
         }
      }
   }
// this is simple because we specify the timing of application, and the landuse to which it is applied.

   //  in the more complex case, this data comes from from a read in  spreadsheet

void WET_Hydro::GetPesticideAppRateSimple()
   {
   COleDateTime t( m_time );
   int simMonth = t.GetMonth();
   int simDay = t.GetDay();
   int simHour = t.GetHour();
   int incrFlowBeginMonth = 12;
   int incrFlowBeginDay = 10;
   if ( simMonth == incrFlowBeginMonth && simDay == incrFlowBeginDay  )
      {
      int reachCount = GetReachCount();

      for (int i=0; i < reachCount; i++ )
         {
         REACH_INFO       *pInfo  = &m_reachTree.m_nodeArray[ i ]->m_reachInfo;
         REACH_INFO_HYDRO *pHydro = m_reachInfoHydroArray[ i ];

         for ( int j=0; j < pHydro->upslopeInfoArray.GetSize(); j++ )
            {
            UPSLOPE_INFO *pUpslope = pHydro->upslopeInfoArray[ j ];
            int cellCount = pUpslope->cellsRepresentedArray.GetSize();

            for (int k=0; k < cellCount ;k++) 
               {
               int landuse=GetCellLulc(pUpslope->cellsRepresentedArray[k]);
               if (landuse==3 || landuse==22 || landuse==23) // this is 
                 
			   {
                  for (int l=0; l<m_numberPesticides; l++)
                     {
                        //  Applications are assumed to occur in l day.  This assumptions means we can take applications rates
                        //  which are generally mass/area and covert them to mass/area/time or mass/time.  Which is what we are
                        //  looking for
                      //if (pUpslope->pesticideArray[l]->beenApplied)
                      //  pUpslope->pesticideArray[l]->pSurfWater->rateMassApp = 0.0f;
                     // else
                      //  {
                        float appRate = 2.50f;  // kg/ha assumed application
                        float appRatem2= appRate/10000.0f; //kg/m2
                        pUpslope->pesticideArray[l]->pSurfWater->rateMassApp = appRatem2*pUpslope->area; //  in kg/day!
                        pUpslope->pesticideArray[l]->pVeg->rateMassApp = appRatem2*pUpslope->area;
                        //pUpslope->pesticideArray[l]->pSurfWater->mass = appRatem2*pUpslope->area;

                        pUpslope->pesticideArray[l]->beenApplied=true;
                        //  pUpslope->pesticideArray[l]->pVeg->rateMassApp = 10; //  in kg/ha
                      //  }
   
                     }
                     break;
                  }

               } // end of k
               // OR MAYBE HERE!
            } // end of j
         }
      }
      else
         {
         int reachCount = GetReachCount();

         for (int i=0; i < reachCount; i++ )
            {
            REACH_INFO       *pInfo  = &m_reachTree.m_nodeArray[ i ]->m_reachInfo;
            REACH_INFO_HYDRO *pHydro = m_reachInfoHydroArray[ i ];

            for ( int j=0; j < pHydro->upslopeInfoArray.GetSize(); j++ )
               {
               UPSLOPE_INFO *pUpslope = pHydro->upslopeInfoArray[ j ];
               for (int l=0; l<m_numberPesticides; l++)
                  {
                    pUpslope->pesticideArray[l]->pSurfWater->rateMassApp = 0.0f; //  in kg/ha
                    pUpslope->pesticideArray[l]->pVeg->rateMassApp = 0.0f; 

                  //pUpslope->pesticideArray[l]->pVeg->rateMassApp = 0; //  in kg/ha
                  }
               }
            }

      }//end of if simMonth
   }
 
// Options that are applied AFTER Upslope Volumes have already been calculated
// These include sediment discharge from upland areas (and the stream when no explicit
// routing algorithm is used.
void WET_Hydro::ApplyRestorationOptions()
   {
   int reachCount = GetReachCount();
   for ( int i=0; i < reachCount; i++ )
		{
      REACH_INFO       *pInfo  = &m_reachTree.m_nodeArray[ i ]->m_reachInfo;
      REACH_INFO_HYDRO *pHydro = m_reachInfoHydroArray[ i ];

      ASSERT( pHydro == (REACH_INFO_HYDRO*) pInfo->pData );

      //This is the flow augmentation model.
      if (m_checkIncrFlow)
         if (pHydro->incrFlow)
            {
            COleDateTime t( m_time );
            int simMonth = t.GetMonth();
            int incrFlowBeginMonth = m_incrFlowBegin.GetMonth();
            int incrFlowEndMonth = m_incrFlowEnd.GetMonth();
            if ( simMonth >= incrFlowBeginMonth && simMonth <= incrFlowEndMonth)
               for (int k=0; k<pInfo->subnodeCount;k++)
                  pInfo->qArray[k]+=m_incrFlowPercent/pInfo->subnodeCount;
            }

      UPSLOPE_INFO *pUpslope = pHydro->upslopeInfoArray[ 0 ];

      if (pHydro->percentLengthAltered != 0.0f)
         {
         float reduction = (m_reduceSedPercent/100.0f) * pHydro->percentLengthAltered;
         pUpslope->sedimentOutflowMass -= (pUpslope->sedimentOutflowMass * reduction)  ;

         UPSLOPE_INFO *pUpslopeOtherSide  = NULL;
         if (m_checkSideOfStream && pHydro->streamOrder>1)
            {  
            int secondSide = (pHydro->upslopeInfoArray.GetSize() / 2 ) ; 
            pUpslopeOtherSide = pHydro->upslopeInfoArray[ secondSide ];
            pUpslopeOtherSide->sedimentOutflowMass = pUpslopeOtherSide->sedimentOutflowMass - (pUpslopeOtherSide->sedimentOutflowMass * pHydro->percentLengthAltered * (m_reduceSedPercent/100.0f));
            }
         }     
      }
   }


//  Structural Restoration options change the dimensions of the problem in some way.
//  A good example:  Wetlands change slope and hydraulic conductivity, and the routing
//  of water.  For this reason, these options are applied to the model before the
//  hydrologic components are calculated.  The hydrologic models (upland and instream)
//  can then be run, and will reflect the effects of the restoration.

//  The method recalculates the area weighted slope and kSat, based on additional
//   restored to wetland cells, assuming the effects on the cells...

//  This should be called in the CalcWatershedInfo method, as it only needs to happen once, for each
//  simulation run/

void WET_Hydro::ApplyStructuralRestorationOptions()
   {
   int reachCount = GetReachCount();
   for ( int i=0; i < reachCount; i++ )
		{
      REACH_INFO       *pInfo  = &m_reachTree.m_nodeArray[ i ]->m_reachInfo;
      REACH_INFO_HYDRO *pHydro = m_reachInfoHydroArray[ i ];
      for (int j = 0; j < pHydro->upslopeInfoArray.GetSize(); j++)
         {
         UPSLOPE_INFO *pUpslope = pHydro->upslopeInfoArray[j];
  //       int altLulc = GetCellAltLulc(j);
  //       int altern = GetCellAltern(j);
         float oldKsat = pUpslope->pHydroParam->kSat;
         float oldSlope = pUpslope->slope;
         float kSatNew = 0.0f;
         float slopeNew = 0.0f;
         for (int k = 0; k <  pUpslope->cellsRepresentedArray.GetSize(); k++)
            {
				float cellArea = GetCellArea( pUpslope->cellsRepresentedArray[k] );
				float weight = cellArea / pUpslope->area;
            float kSat = pUpslope->pHydroParam->kSat;
            float slope = pUpslope->slope;   
            int altLulc = GetCellAltLulc(pUpslope->cellsRepresentedArray[k] );
            int altern = GetCellAltern(pUpslope->cellsRepresentedArray[k] );            
            if (altLulc == 103 || altLulc == 104 || altLulc == 105) // a restoration option meaning 'wetland' - 103, 104, 105
               {
               // if any cell has a wetland, the entire upland unit is affected....
               //  so is kSat, but we are using a constant value, specified in UpdateAuxillaryVars()...
               slope = slope*0.8f;
               pUpslope->hasWetlands = 1;
               }
            if (altern == 109 ) // increase Late Summer Flows
               pHydro->incrFlow = 1;

            kSatNew  += weight * kSat;
            slopeNew += weight * slope;
            }
         pUpslope->pHydroParam->kSat = kSatNew;
         pUpslope->slope = slopeNew;
         }     
      }
   }

//--------------------------------------------------------------------------------------
// WET_Hydro::RunUpslopeModel
//  
// This method is implemented for each cell and time step.
//
// This method is designed to work on the Upslope units
//--------------------------------------------------------------------------------------

int WET_Hydro::RunUpslopeModel( double time )
	{
   if ( m_k1 == NULL )
      {
      m_k1 = new double [ m_svCount ] ;
      m_k2 = new double [ m_svCount ] ;
      m_k3 = new double [ m_svCount ] ;
      m_k4 = new double [ m_svCount ] ;
      m_k5 = new double [ m_svCount ] ;
      m_k6 = new double [ m_svCount ] ;
      m_derivative = new double[ m_svCount ];
      m_svTemp     = new double[ m_svCount ];
      }

	IntRKF( time , m_svCount, m_stateVar, 0);
   UpdateAuxillaryVars( );

   delete m_k1  ;
   m_k1 = NULL;
   delete m_k2  ;
   delete m_k3 ;
   delete m_k4 ;
   delete m_k5 ;
   delete m_k6  ;
   delete m_derivative ;

   delete m_svTemp  ;
      
   return 0;
   }

void WET_Hydro::SimulateET(int method, UPSLOPE_INFO *pUpslope)
   {
   float etmm = 0.0f;
   if (method == 0 )
      {
      float swc = 0.2f;
      if (m_checkUnsaturated)
         swc = pUpslope->swcUnsat;
      else
         swc = pUpslope->swc;

      float etRc= CalcET(pUpslope->currentPrecip, pUpslope->currentAirTemp );
      if (swc > 0.7f * pUpslope->pHydroParam->fieldCapac)
         pUpslope->currentET = etRc;
      else if (swc <= pUpslope->pHydroParam->wiltPt)
         pUpslope->currentET=0.0f;
      else
         {
         float slope = (1-0)/(0.7f*pUpslope->pHydroParam->fieldCapac - pUpslope->pHydroParam->wiltPt);
         float fTheta = slope*(swc - pUpslope->pHydroParam->wiltPt);
         
         pUpslope->currentET = etRc * fTheta;
         }
      }
   if (method == 1)
      pUpslope->currentET = PenmanMonteith((float)m_time);
   }


float WET_Hydro::CalcDUplandTracer_dt(UPSLOPE_INFO *pUpslope)
   {
   float surfaceInflowVolume = 0.0f;
   float subsurfaceInflowVolume = 0.0f;   
   float tracerMass = pUpslope->tracerMass;
   float volumeUp = 0.0f;
   float tracerUp = 0.0f;
   float concUp = 0.0f;
   float conc = 0.0f;

   conc = pUpslope->tracerConc;

   if (m_modelStructureMethod!=WH_DISTRIBUTE_WTHEIGHT)
      {
	   if (pUpslope->pUpslope != NULL ) //if it is null, it is a 'headwater' and has no inflow
         {
		   subsurfaceInflowVolume = pUpslope->pUpslope->lateralOutflowVol;
         surfaceInflowVolume = pUpslope->pUpslope->surfaceOutflowVol;
         volumeUp = pUpslope->pUpslope->volumeSat;
         tracerUp = pUpslope->pUpslope->tracerMass;
         pUpslope->tracerInflowMassSubsurface = pUpslope->pUpslope->tracerConc *subsurfaceInflowVolume;
         }
      pUpslope->tracerOutflowMassSubsurface = pUpslope->lateralOutflowVol*pUpslope->tracerConc ;
      }
   else // we are using a grid to route water 
      {
      subsurfaceInflowVolume = pUpslope->lateralInflow;
      if (!pUpslope->isStream)
         surfaceInflowVolume = pUpslope->surfaceOutflowVol;
      }          
   pUpslope->tracerOutflowMassSubsurface = pUpslope->lateralOutflowVol * pUpslope->tracerConc ;
 //  pUpslope->tracerOutflowMassSurface = pUpslope->surfaceOutflowVol * pUpslope->tracerConc;

   float tracerInConc = 0.0f;
   if (m_radioTracerInput==1 && m_time > m_startTime+30)
   {
      tracerInConc=0.0f;
    //  tracerInConc=m_editTracerMgL/1000.0f;  
   }
   else
      {
      if (m_pMeasuredTracerInputData != NULL )
         tracerInConc = m_pMeasuredTracerInputData->IGet(m_time,1,IM_LINEAR)/1000.0f;
      }

   float verticalInput=0.0f;
   if (m_checkUnsaturated && pUpslope->isSaturated==0)
      verticalInput=(pUpslope->pUnsat->usfVol + (pUpslope->deltaSat*pUpslope->area))*pUpslope->pUnsat->tracerConc;
   else
      verticalInput=((pUpslope->currentPrecip - pUpslope->currentET )* pUpslope->area)*tracerInConc ;


   float dUplandTracer_dt =  verticalInput +
                             pUpslope->tracerInflowMassSubsurface - 
                           //  pUpslope->tracerOutflowMassSurface - 
                             pUpslope->tracerOutflowMassSubsurface -                       
                             (pUpslope->pHydroParam->kDepth*pUpslope->area*pUpslope->tracerConc);
   
  // if (dUplandTracer_dt < 0.00000000001f && dUplandTracer_dt > -0.00000000001f)
   //   dUplandTracer_dt=0.0f;

   return dUplandTracer_dt;
   }

float WET_Hydro::CalcDUnsatTracer_dt(UPSLOPE_INFO *pUpslope)
   {
   float tracerInConc = 0.0f;
   if (m_radioTracerInput==1 && m_time > m_startTime+30)
   {
      tracerInConc=0.0f;
    //  tracerInConc=m_editTracerMgL/1000.0f;  
   }
   else 
      tracerInConc = m_pMeasuredTracerInputData->IGet(m_time,1,IM_LINEAR)/1000.0f;

   float verticalInput=0.0f;
   float satChange = 0.0f;
   satChange = pUpslope->deltaUnsat*pUpslope->area*pUpslope->pUnsat->tracerConc;

   verticalInput=(pUpslope->currentPrecip-pUpslope->currentET)*pUpslope->area*tracerInConc;
   float dUnsatTracer_dt =  verticalInput-
                           (pUpslope->pUnsat->tracerConc * pUpslope->pUnsat->usfVol)+
                           satChange;
                           ;

  // if (dUnsatTracer_dt < 0.00000000001f && dUnsatTracer_dt > -0.00000000001f)
    //  dUnsatTracer_dt=0.0f;
                              
   return dUnsatTracer_dt;
   }
float WET_Hydro::CalcDTill_dt(UPSLOPE_INFO *pUpslope)
   {
   float dTill_dt =  pUpslope->volumeSat*pUpslope->pHydroParam->k1 - pUpslope->pHydroParam->kDepth;                    
   return dTill_dt;
   }

float WET_Hydro::CalcDUplandSediment_dt(UPSLOPE_INFO *pUpslope)
   {
   float tracerMass = pUpslope->sedimentMass;
   float sedimentInflowMass=0.0f;
   float subsurfaceInflowVolume=0.0f;
   float surfaceInflowVolume=0.0f;
	if (pUpslope->pUpslope != NULL ) //if it is null, it is a 'headwater' and has no inflow
      {
		subsurfaceInflowVolume = pUpslope->pUpslope->lateralOutflowVol;
     surfaceInflowVolume = pUpslope->pUpslope->surfaceOutflowVol;
      sedimentInflowMass = pUpslope->pUpslope->sedimentOutflowMass;
      }
   if (pUpslope->area == 0.0f)
      pUpslope->area = 1.0f;
   float q = pUpslope->lateralOutflowVol/pUpslope->area*(float)m_timeStep; // m - runoff volumeSat
   float Q = q; // m/timeStep - peak runoff rate (well, runoff rate = runoff volumeSat over timestep)
   float R = (float)pow(q*Q,0.56f);
   // sedimentOutflowMass in Tonnes/m2/timeStep

   
   float sedimentOutflow = R * /*pUpslope->kFfact*/0.3f * pUpslope->lengthSlope * pUpslope->musleC *pUpslope->musleP;
   // convert that to kg/day
   float sedimentOutflowMass = sedimentOutflow*1000.0f*pUpslope->area/(float)m_timeStep;

   float dUplandSediment_dt = sedimentInflowMass - sedimentOutflowMass ;
   pUpslope->sedimentOutflowMass = sedimentOutflowMass;
   return dUplandSediment_dt;


   }

float WET_Hydro::CalcDVegVolume_dt(UPSLOPE_INFO *pUpslope)
   {
   float dVegVolume_dt = 0.0f;
   float maxStorage = 2.5f/1000.0f*pUpslope->area; //maximum amount of storage in canopy

   //pUpslope->vegDrainRate=(pUpslope->pHydroParam->k1*pUpslope->volumeVeg) / (maxStorage + pUpslope->volumeVeg);
   if (pUpslope->volumeVeg >= maxStorage)
      pUpslope->vegDrainRate = (pUpslope->currentPrecip*pUpslope->area)+(pUpslope->pHydroParam->k1*pUpslope->volumeVeg);
   else
      pUpslope->vegDrainRate = pUpslope->pHydroParam->k1*pUpslope->volumeVeg;

   return dVegVolume_dt = (pUpslope->currentPrecip*pUpslope->area) - pUpslope->vegDrainRate;
   }

float WET_Hydro::CalcDSWE_dt(UPSLOPE_INFO *pUpslope)
   {
   float meltRate=0.0f;
   float refreezeRate = 0.0f;
   float snowAccumRate = 0.0f;
   float dSWE_dt = 0.0f;

   if( pUpslope->currentAirTemp > pUpslope->pHydroParam->tt )//too warm to snow
      {
       
      if (pUpslope->volumeSWE/pUpslope->area*1000.0f>1.0)//greater than 1mm
         pUpslope->meltRate = pUpslope->pHydroParam->cfmax * (pUpslope->currentAirTemp - pUpslope->pHydroParam->tt)/1000.0f;
      else
         pUpslope->meltRate = 0.0f;
      }
   else // cold enough to snow and to refreeze melt water
      {
      refreezeRate = (pUpslope->pHydroParam->cfr * pUpslope->pHydroParam->cfmax * (pUpslope->pHydroParam->tt-pUpslope->currentAirTemp))/1000.0f;
      //convert from mm/d to m/d

      pUpslope->meltRate=0.0f;

      }
   return dSWE_dt =  (pUpslope->currentSnow + refreezeRate - pUpslope->meltRate)*pUpslope->area;       
   }

float WET_Hydro::CalcDVolume_dt(UPSLOPE_INFO *pUpslope)
   {
	float dVolume_dt=0.0f;

   float subsurfaceInflowVolume = 0.0f;
   float surfaceInflowVolume = 0.0f;
//  here we need to account for the possibity that wetland areas may have changed the way ponded volumeSat
//  is routed.  If there are wetland areas, we assume ponded volumes stay on the land, and will infiltrate
//  when conditions allow.  If there are not wetlands, then the ponded volumeSat is shunted dirrectly
//  to the nearest stream.  The calculation of how much ponded volumeSat, etc occurs AFTER the derivative
//  calculation, in the function "GetAuxillaryVARS (or something like that)

   if (m_modelStructureMethod!=WH_DISTRIBUTE_WTHEIGHT)
      {
	   if (pUpslope->pUpslope != NULL ) //if it is null, it is a 'headwater' and has no inflow
         {
         subsurfaceInflowVolume = pUpslope->pUpslope->lateralOutflowVol;
         surfaceInflowVolume = pUpslope->pUpslope->surfaceOutflowVol;
        // pUpslope->lateralInflow = pUpslope->pUpslope->lateralOutflowVol;
        // subsurfaceInflowVolume = pUpslope->lateralInflow;
         }
      }
   else
      {
      subsurfaceInflowVolume = pUpslope->lateralInflow;
  //    if (!pUpslope->isStream)
      //   surfaceInflowVolume = pUpslope->surfaceOutflowVol;
      
      }
      
   float verticalInput=0.0f;
   if (m_checkUnsaturated)
      verticalInput=pUpslope->pUnsat->usfVol + (pUpslope->deltaSat*pUpslope->area);
 //  else if (m_checkInterception)
  //    verticalInput=pUpslope->vegDrainRate + ((pUpslope->meltRate - pUpslope->currentET )* pUpslope->area) ;
   else
      verticalInput=(pUpslope->currentPrecip + pUpslope->meltRate - pUpslope->currentET )* pUpslope->area ;
  
   dVolume_dt =  verticalInput + subsurfaceInflowVolume + surfaceInflowVolume
      - pUpslope->lateralOutflowVol - (pUpslope->pHydroParam->kDepth*pUpslope->area) ; 
        // m3 of water in the cell in 1 day

   if (dVolume_dt < 0.00000000001f && dVolume_dt > -0.00000000001f)
      dVolume_dt=0.0f;
   
   return dVolume_dt;
   }

   
void WET_Hydro::UpdatePesticideWaterVolume(UPSLOPE_INFO *pUpslope)
   {
      
   for (int i=0;i<m_numberPesticides;i++)
      {

      SURF_WATER_INFO *pSurfWater = pUpslope->pesticideArray[i]->pSurfWater;
      UNSAT_WATER_INFO *pUnsatWater = pUpslope->pesticideArray[i]->pUnsatWater;
      SAT_WATER_INFO *pSatWater = pUpslope->pesticideArray[i]->pSatWater;
      float depthToSaturatedZone = pUpslope->pHydroParam->soilDepth - pUpslope->saturatedDepth;
      float bottomOfSurfaceZone = pUpslope->pesticideArray[i]->surfaceZoneDepth;
      float bottomOfRootZone = pUpslope->pesticideArray[i]->rootZoneDepth ;
      float volumeSurfaceZone = 0.0f;
      float volumeRootZone = 0.0f;
      float volumeWater = 0.0f;

      if (depthToSaturatedZone <= 0.005f)//if the water table is within 5 mm of the ground surface...
         {
         volumeSurfaceZone = pUpslope->swc * bottomOfSurfaceZone * pUpslope->area;
         volumeRootZone = pUpslope->swc * bottomOfRootZone * pUpslope->area;
         volumeWater = pUpslope->swc * (pUpslope->pHydroParam->soilDepth - bottomOfRootZone) * pUpslope->area; //the leached zone is fully saturated
         }

                                          //  if it is in the middle of the surface zone...
      if (depthToSaturatedZone > 0.005f && depthToSaturatedZone <= bottomOfSurfaceZone)
         {
         volumeSurfaceZone = ((depthToSaturatedZone * pUpslope->swcUnsat ) +
                           ((bottomOfSurfaceZone-depthToSaturatedZone)* pUpslope->swc)) * pUpslope->area;
         volumeRootZone = pUpslope->swc * (bottomOfRootZone-bottomOfSurfaceZone) * pUpslope->area;
         volumeWater = pUpslope->swc * (pUpslope->pHydroParam->soilDepth - bottomOfRootZone) * pUpslope->area; //the leached zone is fully saturated
         }

                                          //  if the water table is in the middle of the root zone
      if (depthToSaturatedZone > bottomOfSurfaceZone && depthToSaturatedZone <= bottomOfRootZone) 
         {
         volumeSurfaceZone = pUpslope->swcUnsat * bottomOfSurfaceZone * pUpslope->area;
      
         volumeRootZone = (((depthToSaturatedZone-bottomOfSurfaceZone) * pUpslope->pUnsat->swcUnsat) +
                        ((bottomOfRootZone - depthToSaturatedZone)* pUpslope->swc)) * pUpslope->area;

         volumeWater = pUpslope->swc * (pUpslope->pHydroParam->soilDepth - bottomOfRootZone) * pUpslope->area; //the leached zone is fully saturated
         }

                                    //  if the water table is in the middle of the zone below the root zone
      if (depthToSaturatedZone > bottomOfRootZone)
         {
         volumeSurfaceZone = pUpslope->swcUnsat * bottomOfSurfaceZone * pUpslope->area;
         volumeRootZone = pUpslope->pUnsat->swcUnsat * (bottomOfRootZone-bottomOfSurfaceZone) * pUpslope->area;
         volumeWater = (pUpslope->pUnsat->swcUnsat * (depthToSaturatedZone - bottomOfRootZone)
                        + pUpslope->swc * (pUpslope->pHydroParam->soilDepth - depthToSaturatedZone))*pUpslope->area;
         }

      ASSERT(volumeSurfaceZone>0.0f);
      ASSERT(volumeRootZone>0.0f);
      ASSERT(volumeWater>0.0f);

      pSurfWater->volumeWater = volumeSurfaceZone;
      pUnsatWater->volumeWater = volumeRootZone;
      pSatWater->volumeWater = volumeWater;
      }
   }



//////////////////////////////////////////////////////////////////////////////////////////////////
   //  Includes application of pesticide and foliar flux, where pesticide washes off plants
   //  extraction coefficient (m-1)
   //  daily rainfall rate (m/d)
   //  M mass of pesticide on plant surface (kg) per cross sectional area
   //  Kf lumped first-order foliar rate constant (day-1)
   //  Af application rate (g/ha)

float WET_Hydro::CalcDPlantPesticide_dt(UPSLOPE_INFO *pUpslope, int i)
   {
   VEG_INFO *pVeg = pUpslope->pesticideArray[i]->pVeg;
   if (pVeg->mass < 1E-20f)
      pVeg->mass = 0.0f;
    pVeg->rateFoliarRunoff = pVeg->epsilon * pUpslope->currentPrecip * pVeg->mass /** pUpslope->area*/ ;
    if (pVeg->rateFoliarRunoff < 0.0f)
       pVeg->rateFoliarRunoff = 0.0f;
    pVeg->rateTransformation = pVeg->kf * pVeg->mass;
    float dPlantPesticide_dt = pVeg->rateMassApp - pVeg->rateFoliarRunoff - pVeg->rateTransformation; 
    if (dPlantPesticide_dt < 0.000000001f && dPlantPesticide_dt > -0.000000001f)
       dPlantPesticide_dt = 0.0f;
    
   return dPlantPesticide_dt;
   }

//  The surface layer is the upper 10 cm of soil
float WET_Hydro::CalcDSurfacePesticideWater_dt(UPSLOPE_INFO *pUpslope,int i)
   {
   SURF_WATER_INFO *pSurfWater = pUpslope->pesticideArray[i]->pSurfWater;
   float Cpest = pSurfWater->mass / pSurfWater->volumeWater; 
   pSurfWater->rateAdvection = Cpest * pUpslope->pUnsat->usfVol ; // kg/d.  be certain that usfVol is m3/d
   //  rate at which pesticide transforms to alternate phases
   pSurfWater->rateTransformation  = pSurfWater->ks * pSurfWater->mass ; // degradation of dissolved
   pSurfWater->rateTransformation += (Cpest * pSurfWater->kd * pSurfWater->ks * pSurfWater->bulkDensity);  // degradation of adsorption
  // pSurfWater->rateTransformation += pSurfWater->kg * (pUpslope->pHydroParam->phi - pUpslope->swcUnsat)* pSurfWater->kh;//volatilization
   pSurfWater->rateRunoff = pUpslope->surfaceOutflowVol*Cpest;
   pSurfWater->rateUptake = (pUpslope->pesticideArray[i]->pUnsatWater->mass) * pUpslope->pesticideArray[i]->pUnsatWater->uptakeEfficiency * pUpslope->currentET;
  
   pSurfWater->rateErosionLoss = pUpslope->sedimentOutflowMass * pSurfWater->rom * pSurfWater->kd * Cpest;
   float input = pUpslope->pesticideArray[i]->pVeg->rateFoliarRunoff + pSurfWater->rateMassApp;
   float output = pSurfWater->rateAdvection + pSurfWater->rateTransformation + pSurfWater->rateRunoff
      + pSurfWater->rateUptake + pSurfWater->rateErosionLoss;
   
   pSurfWater->rateMassOutflow = pSurfWater->rateAdvection;

   float dSurfacePesticideWater_dt = input - output;
   if (dSurfacePesticideWater_dt < 0.000000001f && dSurfacePesticideWater_dt > -0.000000001f)
      dSurfacePesticideWater_dt = 0.0f;

   return dSurfacePesticideWater_dt;
   }

// actually, this is the change in the 'root zone', the depth of which must be defined....
// The water table certainly moves up and down, but for our vertical discretization we need to set a depth
// and use that as the definition of the boxes (the water content will change across that depth)

float WET_Hydro::CalcDUnsatPesticideWater_dt(UPSLOPE_INFO *pUpslope,int i)
   {
   SURF_WATER_INFO *pSurfWater = pUpslope->pesticideArray[i]->pSurfWater;
   UNSAT_WATER_INFO *pUnsatWater = pUpslope->pesticideArray[i]->pUnsatWater;
    float Cpest = pUnsatWater->mass / pUnsatWater->volumeWater; 
   pUnsatWater->rateAdvection = (pUnsatWater->mass/pUnsatWater->volumeWater) * pUpslope->pUnsat->usfVol ;
   pUnsatWater->rateTransformation = pUpslope->pesticideArray[i]->pSurfWater->ks * pUnsatWater->mass;
   pUnsatWater->rateTransformation += (Cpest * pSurfWater->kd * pSurfWater->ks * pSurfWater->bulkDensity);  // degradation of adsorption
  // pUnsatWater->rateTransformation += pUnsatWater->kg * (pUpslope->pHydroParam->phi - pUpslope->pUnsat->swcUnsat)* pUnsatWater->kh;
   pUnsatWater->rateUptake = (pUnsatWater->mass) * pUnsatWater->uptakeEfficiency * pUpslope->currentET;

   float input = pUpslope->pesticideArray[i]->pSurfWater->rateMassOutflow;
   float output = pUnsatWater->rateAdvection + pUnsatWater->rateTransformation + pUnsatWater->rateUptake;
   pUnsatWater->rateMassOutflow = pUnsatWater->rateAdvection;

   float dUnsatPesticideWater_dt = input - output;
   if (dUnsatPesticideWater_dt < 0.000000001f && dUnsatPesticideWater_dt > -0.000000001f)
      dUnsatPesticideWater_dt = 0.0f;
   return dUnsatPesticideWater_dt;
   }
//  This is the volume that is below the root zone.  It will generally be saturated, but not always.  
float WET_Hydro::CalcDSatPesticideWater_dt(UPSLOPE_INFO *pUpslope,int i)
   {
   SURF_WATER_INFO *pSurfWater = pUpslope->pesticideArray[i]->pSurfWater;
   SAT_WATER_INFO *pSatWater = pUpslope->pesticideArray[i]->pSatWater;
   float Cpest = pSatWater->mass/pSatWater->volumeWater;
   pSatWater->rateAdvection = pUpslope->ssfVol * Cpest;
   pSatWater->rateTransformation = pUpslope->pesticideArray[i]->pSurfWater->ks * pSatWater->mass;
   pSatWater->rateTransformation += (Cpest * pSurfWater->kd * pSurfWater->ks * pSurfWater->bulkDensity);  // degradation of adsorption
   float inflow = 0.0f;
   float outflow = 0.0f;
   if (m_modelStructureMethod==WH_DISTRIBUTE_WTHEIGHT)
      {
      inflow = pUpslope->pesticideArray[i]->pSatWater->rateMassInflow;
      outflow= pUpslope->pesticideArray[i]->pSatWater->rateMassOutflow;
      pSatWater->rateAdvection = 0.0f;
      }
   else
      {
      if (pUpslope->pUpslope != NULL)
         inflow = pUpslope->pUpslope->pesticideArray[i]->pSatWater->rateMassOutflow;
      }

   float input = pUpslope->pesticideArray[i]->pUnsatWater->rateMassOutflow + inflow;
   float output = pSatWater->rateAdvection + pSatWater->rateTransformation + outflow;
   //pSatWater->rateMassOutflow = pSatWater->rateAdvection + outflow;
   
   float dSatPesticideWater_dt = input - output; 
   if (dSatPesticideWater_dt < 0.000000001f && dSatPesticideWater_dt > -0.000000001f)
      dSatPesticideWater_dt = 0.0f;
   return dSatPesticideWater_dt;
   }

   //  These 3 methods would be nice to implement, but we need to think more about
   //  Adsorption/Desorption to include them.  Instead of explicitly maintaining
   //  these states, the sorbed concentration is written in terms of the dissolved
   //  concentration, and included in the 3 derivatives above.
float WET_Hydro::CalcDSurfacePesticideSoil_dt(UPSLOPE_INFO *pUpslope, int i)
   {
   return 0.0f;
   }
float WET_Hydro::CalcDUnsatPesticideSoil_dt(UPSLOPE_INFO *pUpslope, int i)
   {
   return 0.0f;
   }
float WET_Hydro::CalcDSatPesticideSoil_dt(UPSLOPE_INFO *pUpslope, int i)
   {
   return 0.0f;
   }
float WET_Hydro::CalcDVolumeUnsat_dt(UPSLOPE_INFO *pUpslope)
   {
   float dVolumeUnsat_dt = 0.0f;

  /* if (m_checkInterception)
      {
      if (pUpslope->pUnsat->swcUnsat < pUpslope->pHydroParam->wiltPt)
         dVolumeUnsat_dt = pUpslope->vegDrainRate +(( pUpslope->meltRate + pUpslope->deltaUnsat)*pUpslope->area);
      else
         dVolumeUnsat_dt = pUpslope->vegDrainRate - ((pUpslope->currentET + pUpslope->deltaUnsat)*pUpslope->area) 
                              -(pUpslope->pUnsat->usfVol);      
      }*/
   //else//if not simulating interception
    //  {
      if (pUpslope->pUnsat->swcUnsat < pUpslope->pHydroParam->wiltPt)
         dVolumeUnsat_dt = (pUpslope->currentPrecip + pUpslope->meltRate + pUpslope->deltaUnsat)*pUpslope->area;
      else
         dVolumeUnsat_dt = ( (pUpslope->currentPrecip-pUpslope->currentET+pUpslope->deltaUnsat)*pUpslope->area) 
                              -pUpslope->pUnsat->usfVol;      
    //  }
   return dVolumeUnsat_dt;
   }

void WET_Hydro::GetDerivatives(  double *derivative, double curTime )
   {
   int reachCount = GetReachCount();
   int count = 0;

   for ( int i=0; i < reachCount; i++ )
		{
      REACH_INFO       *pInfo  = &m_reachTree.m_nodeArray[ i ]->m_reachInfo;
      REACH_INFO_HYDRO *pHydro = m_reachInfoHydroArray[ i ];

      ASSERT( pHydro == (REACH_INFO_HYDRO*) pInfo->pData );

      pHydro->lateralInflow = 0.0f;
		int numUpslopeBins = pHydro->upslopeInfoArray.GetSize()-1;
 
		for ( int j=numUpslopeBins ; j >= 0; j-- )  //be sure to solve upslope first (the are stored downslope first)
         {
         UPSLOPE_INFO *pUpslope = pHydro->upslopeInfoArray[ j ];
         ASSERT(pUpslope!=NULL);
         float precip = 0.0f;  
         float temp = 10.0f;
         float et=0.0f;

         if (m_radar)
            { //go through all model boxes, collect rainfall from the rainfall grid...
            
            m_pMap->GetLayer("RADAR")->GetData(pUpslope->radarRow,pUpslope->radarCol,precip);
            }
         
         else if ( m_pClimateData != NULL )
            {  
            m_stationLtPrecip=20.0f;
            float precipAdjustment = pUpslope->ltPrecip / m_stationLtPrecip;
            precipAdjustment=1.0f;
		      precip = m_pClimateData->IGetInternal((float)curTime,1,IM_LINEAR, m_timeOffset, true) ;
  		      m_tempPrimet = m_pClimateData->IGetInternal((float)curTime,2,IM_LINEAR,m_timeOffset, false);
        
            if (m_checkInterception)
               precip = precip * 0.8f;
           // if (m_checkPrism)
               //precip *= precipAdjustment;
            if (m_checkGridMet)
               GetUpslopeMetData(pUpslope); 
            else
               pUpslope->currentAirTemp = m_tempPrimet;

            if( pUpslope->currentAirTemp <= pUpslope->pHydroParam->tt )//snowing
               {
               pUpslope->currentSnow = precip;
               pUpslope->currentPrecip = 0.0f;
               }
            else
               {
               pUpslope->currentPrecip = precip;
               pUpslope->currentSnow = 0.0f;
               }
            
		      }
         if ( m_pMeasuredEtData != NULL )
	         pUpslope->currentET = m_pMeasuredEtData->IGetInternal((float)curTime,1,IM_LINEAR,m_timeOffset, false);
         else if (m_checkEtModel)
            SimulateET( m_etMethod, pUpslope );


         if (m_checkUnsaturated)
            {
            derivative[count] = CalcDVolumeUnsat_dt(pUpslope);
            count++;
            }

         derivative[ count ] = CalcDVolume_dt(pUpslope);    
         count++;  
         if (m_checkMassBalance)
            {
            derivative[count] = CalcDPrecipVol_dt(pUpslope);
            count++;

            derivative[count] = CalcDGroundVol_dt(pUpslope);
            count++;
            }
        
         if (m_pMeasuredEtData != NULL || m_checkEtModel)
            {
            if (m_checkMassBalance)
               {
               derivative[count] = CalcDEtVol_dt(pUpslope);
               count++;
               }
            }
//            ASSERT(count!=19679 && pUpslope->tracerMass>0.0f);
         if (m_checkTracerModel)
            {
            if (m_checkMassBalance)
               {
               derivative[count] = CalcDTracerMassInPrecip_dt(pUpslope);
               count++;
               }
            derivative[count] = CalcDUplandTracer_dt(pUpslope);
            count++;
            if (m_checkUnsaturated)
               {
               derivative[count] = CalcDUnsatTracer_dt(pUpslope);
               count++;
               }
            }
         if (m_checkSnow)
            {
            derivative[count] = CalcDSWE_dt(pUpslope);
            count++;
            }
         if (m_checkInterception)
            {
            derivative[count] = CalcDVegVolume_dt(pUpslope);
            count++;
            }

         if (m_checkUplandSediment)
            {
            derivative[count]=CalcDUplandSediment_dt(pUpslope);
            count++;
            }

         if (m_checkPesticide)
            {
            for (int i=0; i<m_numberPesticides; i++)
               {        
               derivative[count]=CalcDPlantPesticide_dt(pUpslope,i);
               count++;
               derivative[count]=CalcDSurfacePesticideWater_dt(pUpslope,i);              
               count++;
               derivative[count]=CalcDUnsatPesticideWater_dt(pUpslope,i);
               count++;
               derivative[count]=CalcDSatPesticideWater_dt(pUpslope,i);
               count++;
               }
            }
         }  // end of: for ( j < # of upslope regions
      }  // end of:  for ( i < # of reaches )
   }


//--------------------------------------------------------------------------------------
// WET_Hydro::RunInstreamModel
//  
// Runs the instream routing model.  Assumes that BuildReachTree() has been previously
//  called to build up the reach tree data structure, and that RunUpslopeModel() has been
//  called to provide upslope drainage information to the model.  This method iterates
//  from the leaves of the reach tree downstream, using an implicit solution procedure
//  to efficiently compute instream routing based on the kinematic wave function.
//--------------------------------------------------------------------------------------

int WET_Hydro::RunInstreamModel( double time )
   {
   if (m_routeChannelWaterMethod == WH_NETWORK_EXPLICIT)
      {
      // get leftmost leaf
      ReachNode *pNode = m_reachTree.FindLeftLeaf();  // find leftmost leaf of entire tree

      int solvedReachCount = 0;

      while ( SolveReach( pNode ) )  // solves the KW equations for the reach downstream from pNode
         {
         solvedReachCount++;

         // next, move to the next downstream node
         ReachNode *pDown = pNode->m_pDown;

         if ( pDown == NULL ) // root of tree reached?
            break;

         // if we're coming from the left and a right branch exists, solve the right branch first
         // are we coming from the left?  and    right branch exists?
         if ( ( pDown->m_pLeft == pNode ) && ( pDown->m_pRight != NULL ) )
            pNode = m_reachTree.FindLeftLeaf( pDown->m_pRight );
         else
            pNode = pDown;
         }
      }

      else //Just add up the inflows (from Uplands) and skip the explicit routing procedure
         {
         float totalDischarge = 0.0f;
         float lateralInflow=0.0f;
         for (int i=0; i<m_reachTree.m_nodeCount-1; i++)
            {
            ReachNode *pNode = m_reachTree.m_nodeArray[i]; 
            REACH_INFO_HYDRO *pHydro = (REACH_INFO_HYDRO*) pNode->m_reachInfo.pData;  

            if (m_modelStructureMethod==WH_DISTRIBUTE_FPGRID || m_modelStructureMethod==WH_DISTRIBUTE_WTHEIGHT)
               {
               lateralInflow = pHydro->upslopeInfoArray[0]->surfaceOutflowVol; // m3 water per day (rates)
               pHydro->upslopeInfoArray[0]->surfaceOutflowVol=0.0f;
               }
            else
	            lateralInflow =pHydro->upslopeInfoArray[0]->lateralOutflowVol + pHydro->upslopeInfoArray[0]->surfaceOutflowVol; // m3 water per day (rates)


            totalDischarge += (lateralInflow)/86400;
            if (m_checkSideOfStream && pHydro->streamOrder>1)
               {
               int secondSide = (pHydro->upslopeInfoArray.GetSize() / 2 ) ; 
               totalDischarge += (pHydro->upslopeInfoArray[ secondSide ]->lateralOutflowVol+pHydro->upslopeInfoArray[ secondSide ]->surfaceOutflowVol)/86400;
               }
            }
         ReachNode *pRoot = m_reachTree.m_pRoot;
         ReachNode *pLast = pRoot->m_pLeft;
         pLast->m_reachInfo.qArray[0] = totalDischarge;
         REACH_INFO_HYDRO *pHydroLast = (REACH_INFO_HYDRO*)pLast->m_reachInfo.pData;
         pHydroLast->cumOutFlowVol += totalDischarge * 86400 * (float)m_timeStep;

         }

   return 1;
   }

void WET_Hydro::RunAuxillaryInstreamModel(double time )
   {  

      
      if (m_routeChannelSolutesMethod == WH_NETWORK_EXPLICIT) // if we are explicitly routing channel flows..
                                    // then assume we route both tracer and sediment.
         {
         ReachNode *pNode = m_reachTree.FindLeftLeaf();  // find leftmost leaf of entire tree

         int solvedReachCount = 0;

         while ( SolveReachSolute( pNode ) )  // solves the KW equations for the reach downstream from pNode
            {
            solvedReachCount++;

            // next, move to the next downstream node
            ReachNode *pDown = pNode->m_pDown;

            if ( pDown == NULL ) // root of tree reached?
               break;

            // if we're coming from the left and a right branch exists, solve the right branch first
            // are we coming from the left?  and    right branch exists?
            if ( ( pDown->m_pLeft == pNode ) && ( pDown->m_pRight != NULL ) )
               pNode = m_reachTree.FindLeftLeaf( pDown->m_pRight );
            else
               pNode = pDown;
            }
         if (m_checkPesticide)
            {
            ReachNode *pNode = m_reachTree.FindLeftLeaf();  // find leftmost leaf of entire tree

            int solvedReachCount = 0;
            while ( SolveReachPesticide( pNode ) )  // solves the KW equations for the reach downstream from pNode
               {

               solvedReachCount++;

               // next, move to the next downstream node
               ReachNode *pDown = pNode->m_pDown;

               if ( pDown == NULL ) // root of tree reached?
                  break;

               // if we're coming from the left and a right branch exists, solve the right branch first
               // are we coming from the left?  and    right branch exists?
               if ( ( pDown->m_pLeft == pNode ) && ( pDown->m_pRight != NULL ) )
                  pNode = m_reachTree.FindLeftLeaf( pDown->m_pRight );
               else
                  pNode = pDown;
               }            
            }
         }

/*
         {
         if ( m_k1 == NULL )
            {
            m_k1 = new double [ m_svCountStream ] ;
            m_k2 = new double [ m_svCountStream ] ;
            m_k3 = new double [ m_svCountStream ] ;
            m_k4 = new double [ m_svCountStream ] ;
            m_k5 = new double [ m_svCountStream ] ;
            m_k6 = new double [ m_svCountStream ] ;
            m_derivative = new double[ m_svCountStream ];
            m_svTemp     = new double[ m_svCountStream ];
            }
	      IntRKF( time , m_svCountStream, m_stateVarStream,1);



         for (int i=0; i<m_reachTree.m_nodeCount-1; i++)
            {
            ReachNode *pNode = m_reachTree.m_nodeArray[i]; 
            REACH_INFO_HYDRO *pHydro = (REACH_INFO_HYDRO*) pNode->m_reachInfo.pData;      
            for (int j=0; j < pNode->m_reachInfo.subnodeCount; j++)
               {
               float conc = pHydro->tracerMassArray[j]/pHydro->waterVolumeArray[j];
               if (m_checkPesticide)
                  {
                  for (int k=0; k<m_numberPesticides;k++)
                     {
                     float concPest = pHydro->instreamPesticideArray[k]->pesticideMassArray[j]/pHydro->waterVolumeArray[j];
                     pHydro->instreamPesticideArray[k]->pesticideDischargeArray[j] = concPest*pNode->m_reachInfo.qArray[j]*86400;
                     if (j==pNode->m_reachInfo.subnodeCount-1)
                        {
                        pHydro->instreamPesticideArray[k]->pesticideConc = concPest*(float)1E6;
                        pHydro->instreamPesticideArray[k]->cumPesticideOutflow += pHydro->instreamPesticideArray[k]->pesticideDischargeArray[j]*(float)m_timeStep;
                        }
                     }
                  }
               pHydro->tracerDischargeArray[j] = conc*pNode->m_reachInfo.qArray[j]*86400;
               
               if (j==pNode->m_reachInfo.subnodeCount-1)
                  {
                  pHydro->tracerConc=conc*1000.0f;
                  pHydro->cumTracerOutFlow += pHydro->tracerDischargeArray[j]*(float)m_timeStep;
                 

                  }
               }
            } 
            delete m_k1  ;
            m_k1 = NULL;
            delete m_k2  ;
            delete m_k3 ;
            delete m_k4 ;
            delete m_k5 ;
            delete m_k6  ;
            delete m_derivative ;
            delete m_svTemp  ;
         }
         
   */   
      // If we are NOT routing instream Auxillary flows explicitly,simply add them up
      else
         {
         float totalNetworkVolume = 0.0f;
         for (int i=0; i<m_reachTree.m_nodeCount-1; i++)
               {
               ReachNode *pNode = m_reachTree.m_nodeArray[i]; 
               REACH_INFO_HYDRO *pHydro = (REACH_INFO_HYDRO*) pNode->m_reachInfo.pData; 
             //for (int y=0; y<pNode->m_reachInfo.subnodeCount; y++)
              //  totalNetworkVolume += pHydro->waterVolumeArray[y];
              
               totalNetworkVolume += (pHydro->upslopeInfoArray[0]->lateralOutflowVol+pHydro->upslopeInfoArray[0]->surfaceOutflowVol);
               if (m_checkSideOfStream && pHydro->streamOrder>1)
                  {
                  int secondSide = (pHydro->upslopeInfoArray.GetSize() / 2 ) ; 
                  totalNetworkVolume += (pHydro->upslopeInfoArray[secondSide]->lateralOutflowVol+pHydro->upslopeInfoArray[secondSide]->surfaceOutflowVol);
                  
                  }
               }

         if (m_checkTracerModel) //If tracer is being simulated
            {
            float totalTracerDischarge = 0.0f;
            
            for (int i=0; i<m_reachTree.m_nodeCount-1; i++)
               {
               ReachNode *pNode = m_reachTree.m_nodeArray[i]; 
               REACH_INFO_HYDRO *pHydro = (REACH_INFO_HYDRO*) pNode->m_reachInfo.pData;  

               totalTracerDischarge += pHydro->upslopeInfoArray[0]->tracerOutflowMassSurface;
               totalTracerDischarge += pHydro->upslopeInfoArray[0]->tracerOutflowMassSubsurface;
              // pHydro->cumTracerOutFlow += totalTracerDischarge;
            
          //     totalNetworkVolume += (pHydro->upslopeInfoArray[0]->lateralOutflowVol+pHydro->upslopeInfoArray[0]->surfaceOutflowVol);
               if (m_checkSideOfStream && pHydro->streamOrder>1)
                  {
                  int secondSide = (pHydro->upslopeInfoArray.GetSize() / 2 ) ; 
                  totalTracerDischarge += pHydro->upslopeInfoArray[ secondSide ]->tracerOutflowMassSurface;
                  totalTracerDischarge += pHydro->upslopeInfoArray[secondSide]->tracerOutflowMassSubsurface;
                 // pHydro->cumTracerOutFlow += totalTracerDischarge;
           //       totalNetworkVolume += (pHydro->upslopeInfoArray[secondSide]->lateralOutflowVol+pHydro->upslopeInfoArray[secondSide]->surfaceOutflowVol);
                  }
               }
            if (totalNetworkVolume == 0.0f)
               totalNetworkVolume = 0.10f;
            ReachNode *pRoot = m_reachTree.m_pRoot;
            ReachNode *pLast = pRoot->m_pLeft;
         
            REACH_INFO_HYDRO *pHydroLast = (REACH_INFO_HYDRO*)pLast->m_reachInfo.pData;

            pHydroLast->tracerConc = totalTracerDischarge/totalNetworkVolume  ;//kg/m3
            pHydroLast->tracerMassArray[0]=totalTracerDischarge * (float)m_timeStep;
            pHydroLast->cumTracerOutFlow += (totalTracerDischarge * (float)m_timeStep);//the tracer discharges are rates (kg/day) so this is a simple integration
            
            
            }
         if (m_checkUplandSediment)//  if sediment is being simulated
            {
            float totalSedimentDischarge = 0.0f;
            for (int i=0; i<m_reachTree.m_nodeCount-1; i++)
               {
               ReachNode *pNode = m_reachTree.m_nodeArray[i]; 
               REACH_INFO_HYDRO *pHydro = (REACH_INFO_HYDRO*) pNode->m_reachInfo.pData;  
               totalSedimentDischarge += pHydro->upslopeInfoArray[0]->sedimentOutflowMass/86400.0f;
               if (m_checkSideOfStream && pHydro->streamOrder>1)
                  {
                  int secondSide = (pHydro->upslopeInfoArray.GetSize() / 2 ) ; 
                  totalSedimentDischarge += pHydro->upslopeInfoArray[ secondSide ]->sedimentOutflowMass/86400.0f;
                  }
               }
            ReachNode *pRoot = m_reachTree.m_pRoot;
            ReachNode *pLast = pRoot->m_pLeft;
         
            REACH_INFO_HYDRO *pHydroLast = (REACH_INFO_HYDRO*)pLast->m_reachInfo.pData;
            pHydroLast->sedimentDischargeArray[0] = totalSedimentDischarge*86400.0f;//kg/day
            pHydroLast->cumSedimentOutflow += totalSedimentDischarge * 86400.0f* (float)m_timeStep;
            
            }

         if (m_checkPesticide)//  if Pesticide is being simulated
            {   
            for (int k=0; k<m_numberPesticides;k++)
               {
               float totalPesticideDischarge = 0.0f;
               for (int i=0; i<m_reachTree.m_nodeCount-1; i++)
                  {
                  ReachNode *pNode = m_reachTree.m_nodeArray[i]; 
                  REACH_INFO_HYDRO *pHydro = (REACH_INFO_HYDRO*) pNode->m_reachInfo.pData;  
                  totalPesticideDischarge += (pHydro->upslopeInfoArray[0]->pesticideArray[k]->pSurfWater->rateMassOutflow
                     +pHydro->upslopeInfoArray[0]->pesticideArray[k]->pSatWater->rateMassOutflow);

                  if (m_checkSideOfStream && pHydro->streamOrder>1)
                     {
                     int secondSide = (pHydro->upslopeInfoArray.GetSize() / 2 ) ; 
                     totalPesticideDischarge += (pHydro->upslopeInfoArray[ secondSide ]->pesticideArray[k]->pSurfWater->rateMassOutflow
                        +pHydro->upslopeInfoArray[ secondSide ]->pesticideArray[k]->pSatWater->rateMassOutflow);
                     }
                  }

               ReachNode *pRoot = m_reachTree.m_pRoot;
               ReachNode *pLast = pRoot->m_pLeft;
               REACH_INFO_HYDRO *pHydroLast = (REACH_INFO_HYDRO*)pLast->m_reachInfo.pData;
            // pHydroLast->instreamPesticideArray[k]->pesticideMassArray[i] = totalPesticideDischarge;//kg/day
               pHydroLast->instreamPesticideArray[k]->pesticideConc=float( totalPesticideDischarge*m_timeStep/totalNetworkVolume*1E6 );
               pHydroLast->instreamPesticideArray[k]->cumPesticideOutflow += totalPesticideDischarge * 86400.0f*(float)m_timeStep;
               }
            } // end of pesticide
         }   // end of else
      
   }
void WET_Hydro::UpdateInstreamAuxillaryVars()
   {
   for (int i=0; i<m_reachTree.m_nodeCount-1; i++)
      {
      ReachNode *pNode = m_reachTree.m_nodeArray[i]; 
      REACH_INFO_HYDRO *pHydro = (REACH_INFO_HYDRO*) pNode->m_reachInfo.pData;      
      for (int j=0; j < pNode->m_reachInfo.subnodeCount; j++)
         {
         if (m_checkPesticide)
            {
            for (int k=0; k<m_numberPesticides;k++)
               {
               float concPest = pHydro->instreamPesticideArray[k]->pesticideMassArray[j]/pHydro->waterVolumeArray[j];
             //  pHydro->instreamPesticideArray[k]->pesticideDischargeArray[j] = concPest*pNode->m_reachInfo.qArray[j]*86400;

               }
            }
         }
      }
   }

void WET_Hydro::GetInstreamDerivatives(  double *derivativeInstream, double curTime )
   {
      ReachNode *pNode = m_reachTree.FindLeftLeaf();  // find leftmost leaf of entire tree
      ReachNode *pDown = pNode->m_pDown;
      int count=0;
      while( pDown != NULL)
         {

         if ( pNode->m_reachInfo.index < 0 )  // phantom node or the root node?
         return ;

         REACH_INFO_HYDRO *pHydro = (REACH_INFO_HYDRO*) pNode->m_reachInfo.pData;
         UPSLOPE_INFO *pUpslope = pHydro->upslopeInfoArray[ 0 ];
         UPSLOPE_INFO *pUpslopeOtherSide = NULL;  

         float dx = pNode->m_reachInfo.deltaX;
         double dt = m_timeStep * 86400.0f;
         float length = pNode->m_reachInfo.length;
    
         // calculate kg/day tracer into each subnode
         int subNodeCount = pNode->m_reachInfo.subnodeCount;
         float pesticideInUpslope=0.0f;
         float tracerInUpslope = ((pUpslope->tracerOutflowMassSurface+pUpslope->tracerOutflowMassSubsurface)/subNodeCount); //kg tracer/ day into each subReach
         if (m_checkSideOfStream && pHydro->streamOrder>1)
            {
            int secondSide = (pHydro->upslopeInfoArray.GetSize() / 2 ) ; ///CHECK THIS TO BE SURE!!!!!!!!!!!!!
            UPSLOPE_INFO *pUpslopeOtherSide = pHydro->upslopeInfoArray[ secondSide ];
            ASSERT(pUpslopeOtherSide != NULL );
            float tracerInflowOtherSide = ((pUpslopeOtherSide->tracerOutflowMassSurface+pUpslopeOtherSide->tracerOutflowMassSubsurface)/subNodeCount);
            tracerInUpslope += tracerInflowOtherSide; // rate of input from Upslope Areas
            }
                  
         for (int k=0;k<m_numberPesticides;k++)
            {
            pHydro->instreamPesticideArray[k]->upslopeContribution=((pUpslope->pesticideArray[k]->pSatWater->rateMassOutflow+pUpslope->pesticideArray[k]->pSurfWater->rateMassOutflow)/subNodeCount); 
            if (m_checkSideOfStream && pHydro->streamOrder>1)
               {
               int secondSide = (pHydro->upslopeInfoArray.GetSize() / 2 ) ; ///CHECK THIS TO BE SURE!!!!!!!!!!!!!
               UPSLOPE_INFO *pUpslopeOtherSide = pHydro->upslopeInfoArray[ secondSide ];
               ASSERT(pUpslopeOtherSide != NULL );
               float pesticideInflowOtherSide = ((pUpslopeOtherSide->pesticideArray[k]->pSatWater->rateMassOutflow + pUpslopeOtherSide->pesticideArray[k]->pSurfWater->rateMassOutflow)/subNodeCount);
               pHydro->instreamPesticideArray[k]->upslopeContribution += pesticideInflowOtherSide;//per subnode basis
               }
            }
            
            
         float Qin = 0.0f;
         float tracerInUpstream = 0.0f;
         float derivative=0.0f;

            if (m_checkTracerModel)  
               {
               for ( int i=0; i < pNode->m_reachInfo.subnodeCount; i++ )
                  {
                  derivativeInstream[count]=CalcDInstreamTracer_dt( pNode, i, tracerInUpslope);  // solves the KW equations for the reach downstream from pNode
                  count++;
                  }
               }
            if (m_checkUplandSediment)
               {
               for ( int i=0; i < pNode->m_reachInfo.subnodeCount; i++ )
                  {
                  derivativeInstream[count]=CalcDInstreamSediment_dt(pNode, i);
                  count++;
                  }
               }
            if (m_checkPesticide)
               {
               for (int k=0; k<m_numberPesticides;k++)
                  {
                  for ( int i=0; i < pNode->m_reachInfo.subnodeCount; i++ )
                     {
                     float test = pHydro->instreamPesticideArray[k]->upslopeContribution;
                     derivativeInstream[count]=CalcDInstreamPesticide_dt(pNode, i, pHydro->instreamPesticideArray[k]->upslopeContribution, k);
                     count++;
                     }
                  }
               }
        
         
         // next, move to the next downstream node
         pDown = pNode->m_pDown;

         if ( pDown == NULL ) // root of tree reached?
            break;

            // if we're coming from the left and a right branch exists, solve the right branch first
            // are we coming from the left?  and    right branch exists?
         if ( ( pDown->m_pLeft == pNode ) && ( pDown->m_pRight != NULL ) )
            pNode = m_reachTree.FindLeftLeaf( pDown->m_pRight );
         else
            pNode = pDown;
         }

   }

float WET_Hydro::CalcDInstreamTracer_dt(ReachNode *pNode,int i, float tracerInUpslope)
   {
   REACH_INFO_HYDRO *pHydro = (REACH_INFO_HYDRO*)pNode->m_reachInfo.pData;
   float qRateOut = pNode->m_reachInfo.qArray[ i ];
  // qRateOut = pHydro->qArrayPrevious[i];

   float tracerInUpstream=0.0f;
   float dTracerMass_dt = 0.0f;
   if (i==0)  // the first subnode in a reach
      {
      if ( pNode->m_pLeft != NULL && pNode->m_pRight != NULL)
         {
         REACH_INFO_HYDRO *pLeft = (REACH_INFO_HYDRO*)pNode->m_pLeft->m_reachInfo.pData;
         REACH_INFO_HYDRO *pRight = (REACH_INFO_HYDRO*)pNode->m_pRight->m_reachInfo.pData;
         int nodeLeft = pNode->m_pLeft->m_reachInfo.subnodeCount-1;
         int nodeRight = pNode->m_pRight->m_reachInfo.subnodeCount-1;
         tracerInUpstream = pLeft->tracerDischargeArray[nodeLeft] + pRight->tracerDischargeArray[nodeRight];
         }
      if ( pNode->m_pLeft != NULL && pNode->m_pRight == NULL )
         {
         REACH_INFO_HYDRO *pLeft = (REACH_INFO_HYDRO*)pNode->m_pLeft->m_reachInfo.pData;
         int nodeLeft = pNode->m_pLeft->m_reachInfo.subnodeCount-1;
         tracerInUpstream = pLeft->tracerDischargeArray[nodeLeft];
         }
      }
   else  // not at boundary - at an internal node in the reach
      {
      tracerInUpstream = pHydro->tracerDischargeArray[i-1];
      }
   float tracerMassIn = tracerInUpslope + tracerInUpstream;
   float tracerMassEstimate = pHydro->tracerMassArray[i];
   float concOut = tracerMassEstimate / pHydro->waterVolumeArray[i];
   float tracerMassOut = qRateOut*86400.0f*concOut;
     
   dTracerMass_dt = tracerMassIn - tracerMassOut; // rate equation (kg/d) 

   return dTracerMass_dt;
   }

   //  i is the current sub node, and j is the current pesticide.  A little confusing, I know.

float WET_Hydro::CalcDInstreamPesticide_dt(ReachNode *pNode,int i, float pesticideInUpslope, int j)
   {
   REACH_INFO_HYDRO *pHydro = (REACH_INFO_HYDRO*)pNode->m_reachInfo.pData;
   float qRateOut = pNode->m_reachInfo.qArray[ i ];
  // qRateOut = pHydro->qArrayPrevious[i];

   float pesticideInUpstream=0.0f;
   float dPesticideMass_dt = 0.0f;
   if (i==0)  // the first subnode in a reach
      {
      if ( pNode->m_pLeft != NULL && pNode->m_pRight != NULL)
         {
         REACH_INFO_HYDRO *pLeft = (REACH_INFO_HYDRO*)pNode->m_pLeft->m_reachInfo.pData;
         REACH_INFO_HYDRO *pRight = (REACH_INFO_HYDRO*)pNode->m_pRight->m_reachInfo.pData;
         int nodeLeft = pNode->m_pLeft->m_reachInfo.subnodeCount-1;
         int nodeRight = pNode->m_pRight->m_reachInfo.subnodeCount-1;
         pesticideInUpstream = pLeft->instreamPesticideArray[j]->pesticideDischargeArray[nodeLeft] + pRight->instreamPesticideArray[j]->pesticideDischargeArray[nodeRight];
         
         pHydro->instreamPesticideArray[j]->pesticideMassArrayUpstream[i]=(pLeft->instreamPesticideArray[j]->pesticideMassArray[nodeLeft] + pRight->instreamPesticideArray[j]->pesticideMassArray[nodeRight])/2;
         
         }
      if ( pNode->m_pLeft != NULL && pNode->m_pRight == NULL )
         {
         REACH_INFO_HYDRO *pLeft = (REACH_INFO_HYDRO*)pNode->m_pLeft->m_reachInfo.pData;
         int nodeLeft = pNode->m_pLeft->m_reachInfo.subnodeCount-1;
         pesticideInUpstream = pLeft->instreamPesticideArray[j]->pesticideDischargeArray[nodeLeft];

         pHydro->instreamPesticideArray[j]->pesticideMassArrayUpstream[i]=pLeft->instreamPesticideArray[j]->pesticideMassArray[nodeLeft];
         }
      }
   else  // not at boundary - at an internal node in the reach
      {
      pesticideInUpstream = pHydro->instreamPesticideArray[j]->pesticideDischargeArray[i-1];
      pHydro->instreamPesticideArray[j]->pesticideMassArrayUpstream[i]=pHydro->instreamPesticideArray[j]->pesticideMassArray[i-1];
      }

      
   float pesticideMassIn = pesticideInUpslope + pesticideInUpstream;
   float pesticideMassEstimate = pHydro->instreamPesticideArray[j]->pesticideMassArray[i];/*(pHydro->instreamPesticideArray[j]->pesticideMassArrayUpstream[i] + 
      pHydro->instreamPesticideArray[j]->pesticideMassArray[i] )/ 2;*/
   float concOut = pesticideMassEstimate / pHydro->waterVolumeArray[i];
   if (concOut < 0.000000001f)
      concOut=0.0f;
   float pesticideMassOut = qRateOut*86400*concOut;  
   dPesticideMass_dt = pesticideMassIn - pesticideMassOut; // rate equation (kg/d)
       
   if (dPesticideMass_dt < 1E-10 && dPesticideMass_dt > -1E-10)
         dPesticideMass_dt = 0.0f;

   return dPesticideMass_dt;
   }


float WET_Hydro::CalcDInstreamSediment_dt(ReachNode *pNode, int i)
   {
   if ( pNode->m_reachInfo.index < 0 )  // phantom node or the root node?
      return true;

   REACH_INFO_HYDRO *pHydro = (REACH_INFO_HYDRO*) pNode->m_reachInfo.pData;
   UPSLOPE_INFO *pUpslope = pHydro->upslopeInfoArray[ 0 ];
   UPSLOPE_INFO *pUpslopeOtherSide = NULL;  

   float dx = pNode->m_reachInfo.deltaX;
   double dt = m_timeStep * 86400;
   float length = pNode->m_reachInfo.length;
    
   // calculate kg/day tracer into each subnode
   int subNodeCount = pNode->m_reachInfo.subnodeCount;
   float sedimentInUpslope = (pUpslope->sedimentOutflowMass/subNodeCount); //kg tracer/ day into each subReach
   if (m_checkSideOfStream && pHydro->streamOrder>1)
      {
      int secondSide = (pHydro->upslopeInfoArray.GetSize() / 2 ) ; ///CHECK THIS TO BE SURE!!!!!!!!!!!!!
      UPSLOPE_INFO *pUpslopeOtherSide = pHydro->upslopeInfoArray[ secondSide ];
      ASSERT(pUpslopeOtherSide != NULL );
      float sedimentInflowOtherSide = (pUpslopeOtherSide->sedimentOutflowMass/subNodeCount);
      sedimentInUpslope += sedimentInflowOtherSide; // rate of input from Upslope Areas
      }
   float Qin = 0.0f;
   float sedimentInUpstream = 0.0f;
   float derivative =0.0f;

	for ( i=0; i < pNode->m_reachInfo.subnodeCount; i++ )
      {
      float qRateOut = pNode->m_reachInfo.qArray[ i ];
      float depth = (float) pow( (qRateOut * pHydro->pInstreamHydroParam->n / sqrt(pNode->m_reachInfo.slope) ) / (float)(pow(pHydro->pInstreamHydroParam->wd/(pHydro->pInstreamHydroParam->wd + 2.0f), 2.0f/3.0f)*pHydro->pInstreamHydroParam->wd ) ,  3.0f/8.0f );
      float width = depth * pHydro->pInstreamHydroParam->wd;
      float volume = width * depth * dx;
      if (i==0)  // the first subnode in a reach
         {
         if ( pNode->m_pLeft != NULL && pNode->m_pRight != NULL)
            {

            REACH_INFO_HYDRO *pLeft = (REACH_INFO_HYDRO*)pNode->m_pLeft->m_reachInfo.pData;
            REACH_INFO_HYDRO *pRight = (REACH_INFO_HYDRO*)pNode->m_pRight->m_reachInfo.pData;
            int nodeLeft = pNode->m_pLeft->m_reachInfo.subnodeCount-1;
            int nodeRight = pNode->m_pRight->m_reachInfo.subnodeCount-1;
            sedimentInUpstream = pLeft->sedimentDischargeArray[nodeLeft] + pRight->sedimentDischargeArray[nodeRight];
            }
         if ( pNode->m_pLeft != NULL && pNode->m_pRight == NULL )
            {
            REACH_INFO_HYDRO *pLeft = (REACH_INFO_HYDRO*)pNode->m_pLeft->m_reachInfo.pData;
            int nodeLeft = pNode->m_pLeft->m_reachInfo.subnodeCount-1;
            sedimentInUpstream = pLeft->sedimentDischargeArray[nodeLeft];
            }
         }
      else  // not at boundary - at an internal node in the reach
         sedimentInUpstream = pHydro->sedimentDischargeArray[i-1];
    
      float sedimentMassIn = sedimentInUpslope + sedimentInUpstream;
    
      float concOut = pHydro->sedimentMassArray[i]/pHydro->waterVolumeArray[i];
      float sedimentMassOut = pHydro->qArrayPrevious[i]*86400*concOut;
      pHydro->sedimentDischargeArray[i] = sedimentMassOut;
      float sedimentMass = pHydro->sedimentMassArray[i];
     
      derivative = sedimentMassIn - sedimentMassOut;    
  
      float conc = pHydro->sedimentMassArray[i]/pHydro->waterVolumeArray[i];

      } // end of the i loop
   return derivative;
   }


// WET_Hydro::SolveReach() ----------------------------------------------
//
// solves the KW equations for the reach downstream from pNode
//------------------------------------------------------------------------

bool WET_Hydro::SolveReach( ReachNode *pNode )
   {
   if ( pNode->m_reachInfo.index < 0 )  // phantom node or the root node?
      return true;
   float beta  = 3.0f/5.0f;
   // first,  surface inflow
   REACH_INFO_HYDRO *pHydro = (REACH_INFO_HYDRO*) pNode->m_reachInfo.pData;
   UPSLOPE_INFO *pUpslope = pHydro->upslopeInfoArray[ 0 ];
   UPSLOPE_INFO *pUpslopeOtherSide = NULL;  

   // compute Qin for this reach
   float dx = pNode->m_reachInfo.deltaX;
   double dt = m_timeStep * 86400.0f;
   float length = pNode->m_reachInfo.length;
   float lateralInflow=0.0f;
   
   if (m_modelStructureMethod==WH_DISTRIBUTE_FPGRID || m_modelStructureMethod==WH_DISTRIBUTE_WTHEIGHT)
      {
      lateralInflow = pUpslope->surfaceOutflowVol /*+ pUpslope->lateralOutflowVol*/; // m3 water per day (rates)
   //   pUpslope->surfaceOutflowVol=0.0f;
      }

   else
	   lateralInflow =pUpslope->lateralOutflowVol + pUpslope->surfaceOutflowVol; // m3 water per day (rates)

   if (m_checkSideOfStream && pHydro->streamOrder>1 && m_modelStructureMethod==WH_DISTRIBUTE_CENTROIDDISTANCEBINS )
      {
      int secondSide = (pHydro->upslopeInfoArray.GetSize() / 2 ) ; ///CHECK THIS TO BE SURE!!!!!!!!!!!!!
      UPSLOPE_INFO *pUpslopeOtherSide = pHydro->upslopeInfoArray[ secondSide ];
      ASSERT(pUpslopeOtherSide != NULL );
      float lateralInflowOtherSide = pUpslopeOtherSide->lateralOutflowVol+pUpslopeOtherSide->surfaceOutflowVol;
      lateralInflow += lateralInflowOtherSide;
      }
   float qsurface = ((lateralInflow/86400.0f) / length ) * float(dt) ;  //  units on differentials are m2/s hence surfaceInflow/length CHECK
   float Qnew = 0.0f;		


	for ( int i=0; i < pNode->m_reachInfo.subnodeCount; i++ )
      {
      float Q = pNode->m_reachInfo.qArray[ i ];
      SetReachGeometry( pNode, i, pHydro->pInstreamHydroParam->wd );
      float width = pHydro->width;
      float depth = pHydro->depth;
      pHydro->waterVolumeArray[i]=width*depth*dx;
      float n = pHydro->pInstreamHydroParam->n;
      float slope = pNode->m_reachInfo.slope;
		float wp = width + depth + depth;

	   // casts added by MG for VS 2003
      float alph =  n * (float)pow( (long double) wp, (long double) (2/3.)) / (float)sqrt( slope );
      float alpha = (float) pow((long double) alph, (long double) 0.6);

		float Celerity = GetKinematicWaveCelerityFromDepth(pNode,depth);
      float deltaT=dx/Celerity/86400.0f;
		// Qin is the value upstream at the current time
		// Q is the value at the current location at the previous time
      float Qin = 0.0f;


      if ( i == 0 )  // are we computing the most upstream node? (remember there are multiple nodes per reach)
         {
         int count = 0;
         count += pNode->m_pRight == NULL ? 0 : 1;
         count += pNode->m_pLeft  == NULL ? 0 : 1;
         // are we at a reach other than a headwater?  If so, we have discharge flowing into us from
         // an upstream reach.
         if ( pNode->m_pLeft != NULL && pNode->m_pRight != NULL)
            
            Qin = pNode->m_pLeft->GetOutputFlow() + pNode->m_pRight->GetOutputFlow();
            
         if ( pNode->m_pLeft != NULL && pNode->m_pRight == NULL )
            Qin = pNode->m_pLeft->GetOutputFlow();

         pHydro->cumInflow  += Qin * 86400.0f * (float)m_timeStep ;
         }
      else  // not at boundary - at an internal node in the reach
         Qin = pNode->m_reachInfo.qArray[ i-1 ];
      float Qstar = ( Q + Qin ) / 2.0f;   // from Chow, eqn. 9.6.4
      float z = alpha * beta * (float) pow( Qstar, beta-1.0f );
      //// start computing new Q value ///
      // next, inflow term
      float Qin_dx = Qin / dx * (float)dt;
      // next, current flow rate
      float Qcurrent_z_dt = Q * z ;
      // last, divisor
      float divisor = z + (float)dt/dx;

      // compute new Q
      float qtemp = qsurface/divisor;
      float qtemp1 = Qin_dx/divisor;
      float qtemp2 = Qcurrent_z_dt/divisor;

      Qnew = (qsurface + Qin_dx + Qcurrent_z_dt )/divisor; //m3/s
		float storage = Qin + lateralInflow - Qnew;
      pHydro->qArrayPrevious[i] = pNode->m_reachInfo.qArray[i];
      pNode->m_reachInfo.qArray[ i ] = Qnew;

      SetReachGeometry( pNode, i, pHydro->pInstreamHydroParam->wd );    
      width = pHydro->width;
      depth = pHydro->depth;
      pHydro->waterVolumeArray[i]=width*depth*dx;
      
      }  // end of:  for ( i < subNodeCount )
   int lastSubNode = pNode->m_reachInfo.subnodeCount-1;
   pHydro->cumOutFlowVol += pNode->m_reachInfo.qArray[lastSubNode] * 86400.0f * (float)m_timeStep;

   return true;
   }



bool WET_Hydro::SolveReachSolute( ReachNode *pNode )
   {
   if ( pNode->m_reachInfo.index < 0 )  // phantom node or the root node?
      return true;
   float beta  = 3.0f/5.0f;
   // first,  surface inflow
   REACH_INFO_HYDRO *pHydro = (REACH_INFO_HYDRO*) pNode->m_reachInfo.pData;
   UPSLOPE_INFO *pUpslope = pHydro->upslopeInfoArray[ 0 ];
   UPSLOPE_INFO *pUpslopeOtherSide = NULL;  

   // compute Qin for this reach
   float dx = pNode->m_reachInfo.deltaX;
   double dt = m_timeStep * 86400.0f;
   float length = pNode->m_reachInfo.length;
   
   float lateralInflow = pUpslope->tracerOutflowMassSubsurface + pUpslope->tracerOutflowMassSurface; // m3 water per day (rates)

   if (m_checkSideOfStream && pHydro->streamOrder>1 && m_modelStructureMethod==WH_DISTRIBUTE_CENTROIDDISTANCEBINS )
      {
      int secondSide = (pHydro->upslopeInfoArray.GetSize() / 2 ) ; ///CHECK THIS TO BE SURE!!!!!!!!!!!!!
      UPSLOPE_INFO *pUpslopeOtherSide = pHydro->upslopeInfoArray[ secondSide ];
      ASSERT(pUpslopeOtherSide != NULL );
      float lateralInflowOtherSide = pUpslopeOtherSide->tracerOutflowMassSubsurface+pUpslopeOtherSide->tracerOutflowMassSubsurface;
      lateralInflow += lateralInflowOtherSide;
      }
   float qsurface = ((lateralInflow/86400.0f) / length ) * float(dt) ;  //  units on differentials are m2/s hence surfaceInflow/length CHECK
   float Qnew = 0.0f;		

   if (m_modelStructureMethod==WH_DISTRIBUTE_WTHEIGHT)
      {
      //find the stream cells;
      //sum all inputs, and treat that as lateralInflow
      }

	for ( int i=0; i < pNode->m_reachInfo.subnodeCount; i++ )
      {
      float Q = pHydro->tracerDischargeArray[ i ];
    //  SetReachGeometry( pNode, i, pHydro->pInstreamHydroParam->wd );
      float width = pHydro->width;
      float depth = pHydro->depth;
    //  pHydro->waterVolumeArray[i]=width*depth*dx;
      float n = pHydro->pInstreamHydroParam->n;
      float slope = pNode->m_reachInfo.slope;
		float wp = width + depth + depth;

	   // casts added by MG for VS 2003
      float alph =  n * (float)pow( (long double) wp, (long double) (2/3.)) / (float)sqrt( slope );
      float alpha = (float) pow((long double) alph, (long double) 0.6);

		//float Celerity = GetKinematicWaveCelerityFromDepth(pNode,depth);
     // float deltaT=dx/Celerity/86400.0f;
		// Qin is the value upstream at the current time
		// Q is the value at the current location at the previous time
      float Qin = 0.0f;


      if ( i == 0 )  // are we computing the most upstream node? (remember there are multiple nodes per reach)
         {
         int count = 0;
         count += pNode->m_pRight == NULL ? 0 : 1;
         count += pNode->m_pLeft  == NULL ? 0 : 1;
         // are we at a reach other than a headwater?  If so, we have discharge flowing into us from
         // an upstream reach.
         if ( pNode->m_pLeft != NULL && pNode->m_pRight != NULL)
            {
            REACH_INFO_HYDRO *pLeft = (REACH_INFO_HYDRO*)pNode->m_pLeft->m_reachInfo.pData;
            REACH_INFO_HYDRO *pRight = (REACH_INFO_HYDRO*)pNode->m_pRight->m_reachInfo.pData;
            Qin = pLeft->tracerDischargeArray[i]+pRight->tracerDischargeArray[i];
            }
            
         if ( pNode->m_pLeft != NULL && pNode->m_pRight == NULL )
            {
            REACH_INFO_HYDRO *pLeft = (REACH_INFO_HYDRO*)pNode->m_pLeft->m_reachInfo.pData;
            Qin = pLeft->tracerDischargeArray[i];
            }

         pHydro->cumTracerInput  += Qin * 86400.0f * (float)m_timeStep ;
         }
      else  // not at boundary - at an internal node in the reach
         {
         
         Qin = pHydro->tracerDischargeArray[i-1];
 
         }
      float Qstar = ( Q + Qin ) / 2.0f;   // from Chow, eqn. 9.6.4
      float z=1.0f;
      if (Qstar!=0.0f)
         float z = alpha * beta * (float) pow( Qstar, beta-1.0f );

      //// start computing new Q value ///
      // next, inflow term
      float Qin_dx = Qin / dx * (float)dt;
      // next, current flow rate
      float Qcurrent_z_dt = Q * z ;
      // last, divisor
      float divisor = z + (float)dt/dx;

      // compute new Q
      float qtemp = qsurface/divisor;
      float qtemp1 = Qin_dx/divisor;
      float qtemp2 = Qcurrent_z_dt/divisor;

      Qnew = (qsurface + Qin_dx + Qcurrent_z_dt )/divisor; //m3/s
		float storage = Qin + lateralInflow - Qnew;
      //pHydro->qArrayPrevious[i] = pNode->m_reachInfo.qArray[i];
      pHydro->tracerDischargeArray[ i ] = Qnew; //kg/s

     // SetReachGeometry( pNode, i, pHydro->pInstreamHydroParam->wd );    
    //  width = pHydro->width;
    //  depth = pHydro->depth;
    //  pHydro->waterVolumeArray[i]=width*depth*dx;
      
      }  // end of:  for ( i < subNodeCount )
   int lastSubNode = pNode->m_reachInfo.subnodeCount-1;
   pHydro->cumTracerOutFlow += pHydro->tracerDischargeArray[ lastSubNode ]* 86400.0f * (float)m_timeStep;
   pHydro->tracerConc=pHydro->tracerDischargeArray[lastSubNode]/pNode->m_reachInfo.qArray[ lastSubNode ]*1000.0f;

   return true;
   }

bool WET_Hydro::SolveReachPesticide( ReachNode *pNode )
   {
   if ( pNode->m_reachInfo.index < 0 )  // phantom node or the root node?
      return true;
   float beta  = 3.0f/5.0f;
   // first,  surface inflow
   REACH_INFO_HYDRO *pHydro = (REACH_INFO_HYDRO*) pNode->m_reachInfo.pData;
   UPSLOPE_INFO *pUpslope = pHydro->upslopeInfoArray[ 0 ];
   UPSLOPE_INFO *pUpslopeOtherSide = NULL;  

   // compute Qin for this reach
   float dx = pNode->m_reachInfo.deltaX;
   double dt = m_timeStep * 86400.0f;
   float length = pNode->m_reachInfo.length;
   for (int k=0;k<m_numberPesticides;k++)
      {
      float lateralInflow = pUpslope->pesticideArray[k]->pSurfWater->contributeToStream; // m3 water per day (rates)

      if (m_checkSideOfStream && pHydro->streamOrder>1 && m_modelStructureMethod==WH_DISTRIBUTE_CENTROIDDISTANCEBINS )
         {
         int secondSide = (pHydro->upslopeInfoArray.GetSize() / 2 ) ; ///CHECK THIS TO BE SURE!!!!!!!!!!!!!
         UPSLOPE_INFO *pUpslopeOtherSide = pHydro->upslopeInfoArray[ secondSide ];
         ASSERT(pUpslopeOtherSide != NULL );
         float lateralInflowOtherSide =pUpslope->pesticideArray[k]->pSurfWater->contributeToStream; // m3 water per day (rates)
         lateralInflow += lateralInflowOtherSide;
         }
      float qsurface = ((lateralInflow/86400.0f) / length ) * float(dt) ;  //  units on differentials are m2/s hence surfaceInflow/length CHECK
      float Qnew = 0.0f;		


	   for ( int i=0; i < pNode->m_reachInfo.subnodeCount; i++ )
         {
         float Q = pHydro->tracerDischargeArray[ i ];
      //  SetReachGeometry( pNode, i, pHydro->pInstreamHydroParam->wd );
         float width = pHydro->width;
         float depth = pHydro->depth;
      //  pHydro->waterVolumeArray[i]=width*depth*dx;
         float n = pHydro->pInstreamHydroParam->n;
         float slope = pNode->m_reachInfo.slope;
		   float wp = width + depth + depth;

	      // casts added by MG for VS 2003
         float alph =  n * (float)pow( (long double) wp, (long double) (2/3.)) / (float)sqrt( slope );
         float alpha = (float) pow((long double) alph, (long double) 0.6);
		   // Qin is the value upstream at the current time
		   // Q is the value at the current location at the previous time
         float Qin = 0.0f;


         if ( i == 0 )  // are we computing the most upstream node? (remember there are multiple nodes per reach)
            {
            int count = 0;
            count += pNode->m_pRight == NULL ? 0 : 1;
            count += pNode->m_pLeft  == NULL ? 0 : 1;
            // are we at a reach other than a headwater?  If so, we have discharge flowing into us from
            // an upstream reach.
            if ( pNode->m_pLeft != NULL && pNode->m_pRight != NULL)
               {
               REACH_INFO_HYDRO *pLeft = (REACH_INFO_HYDRO*)pNode->m_pLeft->m_reachInfo.pData;
               REACH_INFO_HYDRO *pRight = (REACH_INFO_HYDRO*)pNode->m_pRight->m_reachInfo.pData;
               Qin = pLeft->instreamPesticideArray[k]->pesticideDischargeArray[i]+pRight->instreamPesticideArray[k]->pesticideDischargeArray[i];
               }
               
            if ( pNode->m_pLeft != NULL && pNode->m_pRight == NULL )
               {
               REACH_INFO_HYDRO *pLeft = (REACH_INFO_HYDRO*)pNode->m_pLeft->m_reachInfo.pData;
               Qin = pLeft->instreamPesticideArray[k]->pesticideDischargeArray[i];
               }

           // pHydro->cumTracerInput  += Qin * 86400.0f * (float)m_timeStep ;
            }
         else  // not at boundary - at an internal node in the reach
            {
            
            Qin = pHydro->instreamPesticideArray[k]->pesticideDischargeArray[i-1];
    
            }
         float Qstar = ( Q + Qin ) / 2.0f;   // from Chow, eqn. 9.6.4
         float z=1.0f;
         if (Qstar!=0.0f)
            float z = alpha * beta * (float) pow( Qstar, beta-1.0f );

         //// start computing new Q value ///
         // next, inflow term
         float Qin_dx = Qin / dx * (float)dt;
         // next, current flow rate
         float Qcurrent_z_dt = Q * z ;
         // last, divisor
         float divisor = z + (float)dt/dx;

         // compute new Q
         float qtemp = qsurface/divisor;
         float qtemp1 = Qin_dx/divisor;
         float qtemp2 = Qcurrent_z_dt/divisor;

         Qnew = (qsurface + Qin_dx + Qcurrent_z_dt )/divisor; //m3/s
		   float storage = Qin + lateralInflow - Qnew;
         pHydro->instreamPesticideArray[ k ]->pesticideDischargeArray[i] = Qnew; //kg/
         
         }  // end of:  for ( i < subNodeCount )
      int lastSubNode = pNode->m_reachInfo.subnodeCount-1;
      pHydro->instreamPesticideArray[ k ]->pesticideConc = pHydro->instreamPesticideArray[ k ]->pesticideDischargeArray[ lastSubNode ];

      
   
  // pHydro->cumTracerOutFlow += pHydro->instreamPesticideArray[ k ]->pesticideDischargeArray[ lastSubNode ]* 86400.0f * (float)m_timeStep;   
      } // end of: for (k=numberofpesticides)
   return true;
   }



//  Currently this requires that reaches be selected in the same order in which they appear
//  in the measurement (m_pMeasuredDischargeDAta).  When there is only one, that is probably
//  ok but becomes more confusing if there are multiple measurement sites (like we have for the 
//  San Jose Basin....

void WET_Hydro::CollectMonteCarloData(double time)
   {
   int dataToCollect = m_reachDataArray.GetSize() + 1;  // include time
   ASSERT(dataToCollect==2);
   float *discharge = new float[ dataToCollect ];
   float *tracer = new float[dataToCollect];
   discharge[0] = (float)time ;
   tracer[0]=m_time;
   ASSERT( m_pStreamLayer != NULL );
   DataObj *pStreamData = m_pStreamLayer->m_pData;

   //  Discharge is stored in the REACHTREE!
   //  Assume that the last reach in the dataset is the reach that corresponds to the 
   //  point represented by measurements.
   for (int i=1; i < dataToCollect; i++ )
      {
      int reachIndex = m_reachDataArray[ i-1 ];

      // First find the last subNode in the selected Reach
      int last = m_reachTree.m_nodeArray[ reachIndex ]->m_reachInfo.subnodeCount - 1;
      REACH_INFO_HYDRO * pInfo = (REACH_INFO_HYDRO*)m_reachTree.m_nodeArray[reachIndex]->m_reachInfo.pData;
      // Then get the discharge data for that reach and assign it to the array...
      discharge[ i ] = m_reachTree.m_nodeArray[ reachIndex ]->m_reachInfo.qArray[ last ] ;
      tracer[i] = pInfo->tracerDischargeArray[last]/m_reachTree.m_nodeArray[ reachIndex ]->m_reachInfo.qArray[ last ]*1000.0f;
      float crap = discharge[ i ];
      int test=1;
      }
   m_pInstreamData->AppendRow( discharge );  
   if (m_checkTracerModel)
      m_pTracerConcChannelData->AppendRow(tracer);
   delete discharge;
   delete tracer;
   CollectDataMassBalance();
   }

void WET_Hydro::CollectDataSingleRun( )
   {
//Collect Upslope Data
   if (m_modelStructureMethod==WH_DISTRIBUTE_FPGRID || m_modelStructureMethod==WH_DISTRIBUTE_WTHEIGHT)
       CollectDataGrid();
   else
       CollectDataSemi();

   CollectDataInstream();
   CollectDataMassBalance();
   }

int WET_Hydro::CollectGetNumGrid(void)
   {
   m_collectDataCount++;
   int dataToCollect = (m_cellDataArray.GetSize()/2) + 1; // include time
   return dataToCollect;
   }
 
int WET_Hydro::CollectGetNumSemi(void)
   {
   m_collectDataCount++;
   int dataToCollect = m_cellDataArray.GetSize() + 1; // include time
   return dataToCollect;
   }
int WET_Hydro::CollectGetNumInstream(void)
   {
   int dataToCollect = m_reachDataArray.GetSize() + 1;  // include time
   return dataToCollect;
   }


UPSLOPE_INFO *WET_Hydro::CollectDataGetGridUpslope(int i, int &count )
   {
   UPSLOPE_INFO *pUpslope=NULL;   
//NEED to go to m_cellARRAY and find out where we are
   int reachCount = GetReachCount();
   bool found=false;
   ASSERT(m_cellDataArray.GetSize()>1);
   int colI=m_cellDataArray[count-1];
   int rowI=m_cellDataArray[count];
   for (int l=0; l < reachCount; l++ )
      {
      REACH_INFO_HYDRO *pHydro = m_reachInfoHydroArray[ l ];
      for ( int x=pHydro->upslopeInfoArray.GetSize()-1;x >= 0; x--)
         {
         pUpslope = pHydro->upslopeInfoArray[ x ];
         if (pUpslope->colIndex==m_cellDataArray[count-1] && pUpslope->rowIndex==m_cellDataArray[count])
            {
            found=true;
            count=count+2;
            break;
            }
         }
      if (found)
            break;
      }
 
   return pUpslope;
   }
UPSLOPE_INFO *WET_Hydro::CollectDataGetSemiUpslope(int i)
   {
   UPSLOPE_INFO *pUpslope=NULL;
   int cell = m_cellDataArray[ i-1 ];
   pUpslope = GetCellUpslopeInfo( cell );
   ASSERT( pUpslope != NULL );
   return pUpslope;
   }
UPSLOPE_INFO *WET_Hydro::CollectDataGetUpslope(int i, int &count)
   {
//Collect Upslope Data
   UPSLOPE_INFO *pUpslope=NULL;
   if (m_modelStructureMethod==WH_DISTRIBUTE_FPGRID || m_modelStructureMethod==WH_DISTRIBUTE_WTHEIGHT)
      pUpslope = CollectDataGetGridUpslope(i, count);

   else
   
       pUpslope = CollectDataGetSemiUpslope(i);

   return pUpslope;

   }


void WET_Hydro::CollectDataGrid()
   {
   int dataToCollect = CollectGetNumGrid();

   CollectDataUpslopeWater( dataToCollect);
   if (m_checkTracerModel)
      CollectDataUpslopeTracer( dataToCollect);
         
   if (m_checkPesticide)
      CollectDataUpslopePesticide(dataToCollect); 

   }

void WET_Hydro::CollectDataSemi()
   {
   int dataToCollect = CollectGetNumSemi();

   CollectDataUpslopeWater( dataToCollect);
   if (m_checkTracerModel)
      CollectDataUpslopeTracer( dataToCollect);
         
   if (m_checkPesticide)
      CollectDataUpslopePesticide(dataToCollect); 

   }

void WET_Hydro::CollectDataInstream( )
   {
  //Collect Instream Data
      int dataToCollect = CollectGetNumInstream();
      CollectDataInstreamWater( dataToCollect);
      if (m_checkTracerModel)
         CollectDataInstreamTracer( dataToCollect);
         
      if (m_checkPesticide)
         CollectDataInstreamPesticide(dataToCollect); 
   }




void WET_Hydro::CollectDataUpslopeWater(  int dataToCollect )
   {
   float *data = new float[dataToCollect];
   float *unsatData = new float[dataToCollect];
   float *satDepth = new float[dataToCollect];
   float *satDeficit = new float[dataToCollect];
	float *climate = new float[ dataToCollect ];
   float *x = new float[ dataToCollect ];
   float *etData = new float[dataToCollect];
   float *discharge = new float[dataToCollect];
 //  The first element (or column since this is called through time) is time.   
   climate[ 0 ] = (float)m_time;
   data[ 0 ] = (float)m_time-32000.0f;
   unsatData[0] = (float)m_time;
   satDepth[0] = (float)m_time;
   satDeficit[0] = (float)m_time;
   x[ 0 ] = (float)m_time;
   etData[0] = (float)m_time;
   int count=1;
   for (int i=1;i<dataToCollect;i++)
      {
      UPSLOPE_INFO *pUpslope = CollectDataGetUpslope(i,count);
      float swc=0.0f;
      if (m_checkUnsaturated)
         swc = pUpslope->pUnsat->swcUnsat;
      else
         swc = pUpslope->swc;
      float etRate = pUpslope->currentET;
      data[ i ] = swc;
      if (m_checkUnsaturated)
         {
         unsatData[i] = pUpslope->pUnsat->unsaturatedDepth * 1000.0f;
         satDeficit[i] = float( pUpslope->pUnsat->usfVol/m_timeStep );
         }
      else
         {
         unsatData[i]=pUpslope->volumeSWE/pUpslope->area*1000.0f;
         satDeficit[i] = pUpslope->meltRate ;
         }
      satDepth[i] = pUpslope->saturatedDepth;
      x[ i ] =  pUpslope->pHydroParam->soilDepth;
      etData[i]=pUpslope->currentAirTemp;
      if (m_pMeasuredTracerInputData!=NULL)
         climate[ i ] = pUpslope->currentPrecip*1000/24;
      else
         climate[i] = pUpslope->currentPrecip*1000/24;
      }
      
  
   m_pSwcData->AppendRow(data );  
   m_pUnsatDepthData->AppendRow(unsatData);
   m_pSatDepthData->AppendRow(satDepth);
   m_pSatDeficitData->AppendRow(satDeficit);
   m_pUplandStorageData->AppendRow( x );
   m_pModelClimateData->AppendRow( climate );
   m_pETData->AppendRow(etData);
   delete x;
   delete data;
   delete unsatData;
   delete satDepth;
   delete satDeficit;
   delete climate;
   delete etData;

       /*  
   if (m_checkUnsaturated)
      {
      int dataToCollect2 = m_cellDataArray.GetSize();
      int number = (dataToCollect2*3 ) + 1;
      float *unsatRates = new float[number];
      unsatRates[ 0 ] = (float)m_time;
      int count = 1;
      for ( i=0; i < dataToCollect2; i++ )
         {
         int cell = m_cellDataArray[ i ];
        // UPSLOPE_INFO *pUpslope = GetCellUpslopeInfo( cell );
         UPSLOPE_INFO *pUpslope = CollectDataGetUpslope(i,count);
         float c = pUpslope->pHydroParam->fieldCapac/pUpslope->pHydroParam->phi;
         float d = pUpslope->pHydroParam->wiltPt/pUpslope->pHydroParam->phi;
         float uMax = c*((pUpslope->pHydroParam->soilDepth*pUpslope->pHydroParam->phi) - (pUpslope->volumeSat/pUpslope->area));
         float uMin = d*((pUpslope->pHydroParam->soilDepth*pUpslope->pHydroParam->phi) - (pUpslope->volumeSat/pUpslope->area));
         unsatRates[ count ] = pUpslope->pHydroParam->wiltPt;
         count++;
         unsatRates[count] = pUpslope->pHydroParam->fieldCapac;
         count++;
         unsatRates[count] = pUpslope->pUnsat->swcUnsat;
         count++;
         }   
     m_pRateData->AppendRow(unsatRates);
     delete unsatRates;
     }*/
         
 ////////////////////////////////////////////////////////////
      // Collect Data for the Time Step
   float *timeStepData = new float[ 2 ];
   timeStepData[ 0 ] = (float)m_time;
   timeStepData[ 1 ] = (float)m_timeStep;
   m_pGeneralData->AppendRow( timeStepData );
   delete timeStepData;

         ////////////////////////////////////////////////////////////
         // Collect Data for SedimentOutflow

   ReachNode *pNode = NULL;
   for ( int j=0; j < m_reachTree.m_nodeCount-1; j++ )        // so we must figure out what is upstream of it
      {
      pNode = m_reachTree.m_nodeArray[ j ];
      if ( pNode->m_pDown == m_reachTree.m_nodeArray[ m_reachTree.m_nodeCount-1 ] )        
         break;
      }

   REACH_INFO_HYDRO *pHydro = (REACH_INFO_HYDRO*)pNode->m_reachInfo.pData;
   float sedOut = pHydro->sedimentDischargeArray[0];;  
   float *sedimentOutflow = new float[ 2 ];
   sedimentOutflow[ 0 ] = (float)m_time;
   sedimentOutflow[ 1 ] = sedOut;
   m_pSedimentOutflowData->AppendRow( sedimentOutflow );
   delete sedimentOutflow  ;
   }

void WET_Hydro::CollectDataInstreamWater( int dataToCollect )
   {
  ////////////////////////////////////////////////////////
   // Go through the same process but collect reach-level data
   float *discharge = new float[ dataToCollect ];
   discharge[0] = (float)m_time ;
   ASSERT( m_pStreamLayer != NULL );
   DataObj *pStreamData = m_pStreamLayer->m_pData;
   //  Discharge is stored in the REACHTREE!

   for (int i=1; i < dataToCollect; i++ )//discharge[0] is time, so start at discharge[1]
      {
      int reachIndex = m_reachDataArray[ i-1 ];//A list of reaches to be sampled
      int last = m_reachTree.m_nodeArray[ reachIndex ]->m_reachInfo.subnodeCount - 1;
      discharge[ i ] = m_reachTree.m_nodeArray[ reachIndex ]->m_reachInfo.qArray[ last ];
      }
   m_pInstreamData->AppendRow( discharge ); 
   }

void WET_Hydro::CollectDataUpslopePesticide(   int dataToCollect )
   {
   float *pestVegData = new float[(dataToCollect-1)*m_numberPesticides +1  ];
   float *pestSurfData = new float[(dataToCollect-1)*m_numberPesticides + 1];
   float *pestUnsatData = new float[(dataToCollect-1)*m_numberPesticides + 1];
   float *pestSatData = new float[(dataToCollect-1)*m_numberPesticides + 1  ];
   float *pestMassBalanceData = new float[((dataToCollect-1)* m_numberPesticides * 10 ) + 1  ];
   pestVegData[0] = (float)m_time;
   pestSurfData[0] = (float)m_time;
   pestUnsatData[0] = (float)m_time;
   pestSatData[0] = (float)m_time;
   pestMassBalanceData[0] = (float)m_time;
   int pestNum=0;
   int pestNum2=1;
   int count=1;
   for (int i=1;i<dataToCollect;i++)
      {
      UPSLOPE_INFO *pUpslope = CollectDataGetUpslope(i,count);
      for (int k=0; k < m_numberPesticides; k++)
         {
         pestNum++;
         pestVegData[pestNum] = pUpslope->pesticideArray[k]->pSurfWater->concentration/*pesticideArray[k]->pVeg->rateFoliarRunoff*/;
         pestSurfData[pestNum] = pUpslope->pesticideArray[k]->pSatWater->rateMassOutflow;
         pestUnsatData[pestNum] = pUpslope->pesticideArray[k]->pUnsatWater->concentration;
         pestSatData[pestNum] = pUpslope->pesticideArray[k]->pSatWater->concentration;
         }     
      for (int k=0; k < m_numberPesticides; k++)
         {
         PESTICIDE_INFO *pMass = pUpslope->pesticideArray[k];
         pestMassBalanceData[pestNum2++]=pMass->pSurfWater->cumErosionLoss;
         pestMassBalanceData[pestNum2++]=pMass->pSurfWater->cumMassApp;
         pestMassBalanceData[pestNum2++]=pMass->pVeg->cumMassInput;
         pestMassBalanceData[pestNum2++]=pMass->pVeg->cumTransformation;
         pestMassBalanceData[pestNum2++]=pMass->pSurfWater->cumRunoff;
         pestMassBalanceData[pestNum2++]=pMass->pUnsatWater->cumTransformation;
         pestMassBalanceData[pestNum2++]=pMass->pSatWater->cumTransformation;
         pestMassBalanceData[pestNum2++]=pMass->pSurfWater->cumTransformation;
         pestMassBalanceData[pestNum2++]=pMass->pUnsatWater->cumUptake;
         pestMassBalanceData[pestNum2++]=pMass->pSurfWater->cumUptake;
         }
      }
   
   m_pPestVegData->AppendRow(pestVegData);
   m_pPestSurfData->AppendRow(pestSurfData);
   m_pPestUnsatData->AppendRow(pestUnsatData);
   m_pPestSatData->AppendRow(pestSatData);
   m_pPestMassBalanceData->AppendRow(pestMassBalanceData);
   delete pestVegData;
   delete pestSurfData;
   delete pestUnsatData;
   delete pestSatData;
   delete pestMassBalanceData;
}

void WET_Hydro::CollectDataInstreamPesticide(  int dataToCollect )
   {
   float *pestDischarge = new float[((dataToCollect-1)*m_numberPesticides)+1];
   pestDischarge[0]=(float)m_time;
 // pesticide (instream) are stored in the InstreamPesticideArray (and array of INSTREAM_PESTICIDE_INFOs)
   int pestCount=1;
   for (int i=1; i < dataToCollect; i++ )
      {
      int reachIndex = m_reachDataArray[ i-1 ];
      REACH_INFO_HYDRO *pInfo = (REACH_INFO_HYDRO*)m_reachTree.m_nodeArray[reachIndex]->m_reachInfo.pData;
      for (int k = 0; k<m_numberPesticides; k++)
         {
         float pest = pInfo->instreamPesticideArray[k]->pesticideConc*1E6f;//converts to ug/l        
         pestDischarge[pestCount] = pest;
         pestCount++;
         }    
      }
   m_pPestInstreamData->AppendRow( pestDischarge ); 
   delete pestDischarge;
   }

void WET_Hydro::CollectDataUpslopeTracer(  int dataToCollect)
   {

   float *tracerConc = new float[ dataToCollect];
   float *tracer = new float[dataToCollect];
   float *sediment = new float[dataToCollect];
   float *cumSed = new float[dataToCollect];
   float *sedimentRunoff = new float[dataToCollect];
   tracerConc[0] = (float)m_time;
   sedimentRunoff[0] = (float)m_time;
   int count=1;
   for (int i=1; i< dataToCollect; i++)
      {

      UPSLOPE_INFO *pUpslope = CollectDataGetUpslope(i,count);
      float tracer = pUpslope->cumTracerMassInPrecip;
      float tracerCon = pUpslope->tracerConc*1000.0f;
      float sedimentRunoffRate = pUpslope->surfaceOutflowVol;
   //   if (pUpslope->tracerWeightedMeanTime > 0.0f)
     //    sedimentRunoffRate = pUpslope->pUnsat->tracerConc*1000.0f;
    //  else
      //   sedimentRunoffRate=0.0f;
      if (pUpslope->pUnsat != NULL)
         sedimentRunoffRate=pUpslope->pUnsat->tracerConc*1000.0f;
      
        
      tracerConc[ i ] = tracerCon;
      sedimentRunoff[i]=sedimentRunoffRate;
      }
   m_pTracerConcData->AppendRow( tracerConc );
   m_pUplandSedimentData->AppendRow(sedimentRunoff);
   delete sedimentRunoff;
   }

void WET_Hydro::CollectDataInstreamTracer(  int dataToCollect )
   {
   float *tracer = new float[dataToCollect];
   float *sediment = new float[dataToCollect];
   float *tracerMass = new float[dataToCollect];
   float *cumSed = new float[dataToCollect]; 
   tracer[0] = (float)m_time;
   sediment[0] = (float)m_time;
   cumSed[0] = (float)m_time;

   tracerMass[0]=(float)m_time;
//////////////////////////////////////////////////////////////////
   //  And tracer and sediment data is stored in REACH_INFO_HYDRO! (it is model specific)
   for (int i=1; i < dataToCollect; i++ )
      {
      int reachIndex = m_reachDataArray[ i-1 ];
      REACH_INFO_HYDRO * pInfo = (REACH_INFO_HYDRO*)m_reachTree.m_nodeArray[reachIndex]->m_reachInfo.pData;
      if (m_time<=m_startTime+2.0)
         tracer[i]=1.0f;//starts out,before rain, as all old water
      else
      {
      //tracer[i] = ((pInfo->tracerConc*1000.0f) - m_initTracerConc)/(m_editTracerMgL - m_initTracerConc)*discharge[i];
      float perOld = ((pInfo->tracerConc*1000.0f) - m_editTracerMgL )/(m_initTracerConc -  m_editTracerMgL);
      tracer[i]=perOld;
      }
     // tracer[i] = pInfo->tracerConc*1000.0f;
     // sediment[i]=pInfo->sedimentConc;
      float prev=0.0f;
      if (m_pTracerMassChannelData->GetRowCount()==0)
         prev=0.0f;
      else 
         prev=m_pTracerMassChannelData->GetAsFloat(1,m_pTracerMassChannelData->GetRowCount()-1);
      if ((m_massBalanceInfo.tracerInputMass+m_massBalanceInfo.initialTracerMass)==0.0f )
         tracerMass[i] = prev + 0.0f;
      else
         tracerMass[i] =  prev+((pInfo->tracerMassArray[0]/(float)m_timeStep)/(m_massBalanceInfo.tracerInputMass+m_massBalanceInfo.initialTracerMass ));
 
      //tracerMass[i] = (pInfo->tracerMassArray[0]);
    //  sediment[i] = tracer[i]*discharge[i];
      m_rtIntegral += (tracerMass[i]*m_collectionInterval);
      int last = m_reachTree.m_nodeArray[ reachIndex ]->m_reachInfo.subnodeCount - 1;
      sediment[i]=pInfo->tracerDischargeArray[last]/m_reachTree.m_nodeArray[ reachIndex ]->m_reachInfo.qArray[ last ]*1000.0f; //kg/s/m3
      }
   m_pTracerConcChannelData->AppendRow( tracer ); 
   m_pTracerMassChannelData->AppendRow(tracerMass);
   m_pInstreamSedimentData->AppendRow( sediment ); 

   delete tracer;
   delete sediment; 
   delete tracerMass;
   }


void WET_Hydro::CollectDataMassBalance()
   {
  ////////////////////////////////////////////////////////////
   // Collect Data Mass Balance. These methods accumulate mass across the entire modeled watershed

   float cumETVol = 0.0f;
   float cumGroundLoss = 0.0f;
   float cumPrecipVol = 0.0f;
   float cumOutflowVol = 0.0f;
   float cumTracerMassInPrecip = 0.0f;
   float cumTracerOutflowMass = 0.0f;
   float cumSedimentOutflowMass = 0.0f;

  

   CalcCumulativeUplandVolume(cumETVol, cumPrecipVol, cumTracerMassInPrecip, cumGroundLoss);  // This does the same as the following code, but is a simpler integration
   m_massBalanceInfo.tracerInputMass = cumTracerMassInPrecip;
   CalcCumulativeChannelVolume(cumOutflowVol,cumTracerOutflowMass, cumSedimentOutflowMass);

	float *massBalanceData = new float[ 5 ];
   massBalanceData[ 0 ] = (float)m_time;
   massBalanceData[ 1 ] = cumETVol/m_areaTotal*1000.0f;
   massBalanceData[ 2 ] = cumPrecipVol/m_areaTotal*1000.0f;
   massBalanceData[ 3 ] = cumOutflowVol/m_areaTotal*1000.0f;

   massBalanceData[ 4 ]   = cumGroundLoss/m_areaTotal*1000.0f;

   m_pMassBalanceData->AppendRow( massBalanceData );
   delete massBalanceData;

   float *cumSedData = new float[2];
   cumSedData[ 0 ] = (float)m_time;
   cumSedData[ 1 ] = cumSedimentOutflowMass;
   m_pTotalSedimentOutflowData->AppendRow(cumSedData);
   delete cumSedData;
   }


void WET_Hydro::CollectData( double time )
   {
   if (m_checkMonteCarlo==WH_SINGLE_SIMULATION)
      {
      m_collectDataCount++;
      int dataToCollect=-1;
      ////////////////////////////////////////////////////////
      // Collect soil water content data (cell-level)
      if (m_modelStructureMethod==WH_DISTRIBUTE_FPGRID || m_modelStructureMethod==WH_DISTRIBUTE_WTHEIGHT)
         {
         dataToCollect = (m_cellDataArray.GetSize()/2) + 1; // include time
         }
      else
         dataToCollect = m_cellDataArray.GetSize() + 1; // include time

      //  Set up float arrays to store the data for current time (many different places)

      float *data = new float[dataToCollect];

      float *pestVegData = new float[(dataToCollect-1)*m_numberPesticides +1  ];
      float *pestSurfData = new float[(dataToCollect-1)*m_numberPesticides + 1];
      float *pestUnsatData = new float[(dataToCollect-1)*m_numberPesticides + 1];
      float *pestSatData = new float[(dataToCollect-1)*m_numberPesticides + 1  ];
      float *pestMassBalanceData = new float[((dataToCollect-1)* m_numberPesticides * 10 ) + 1  ];


      float *unsatData = new float[dataToCollect];
      float *satDepth = new float[dataToCollect];
      float *satDeficit = new float[dataToCollect];
	   float *climate = new float[ dataToCollect ];

      float *x = new float[ dataToCollect ];

      float *tracerConc = new float[ dataToCollect];
      float *etData = new float[dataToCollect];
      float *sedimentRunoff = new float[dataToCollect];
      float *discharge = new float[dataToCollect];
      float *tracer = new float[dataToCollect];
      float *sediment = new float[dataToCollect];
      float *cumSed = new float[dataToCollect];

      //  The first element (or column since this is called through time) is time.
	   climate[ 0 ] = (float)time;
      data[ 0 ] = (float)time-32000.0f;
      pestVegData[0] = (float)time;
      pestSurfData[0] = (float)time;
      pestUnsatData[0] = (float)time;
      pestSatData[0] = (float)time;
      pestMassBalanceData[0] = (float)time;

      unsatData[0] = (float)time;
      satDepth[0] = (float)time;
      satDeficit[0] = (float)time;
      x[ 0 ] = (float)time;
      tracerConc[0] = (float)time;
      etData[0] = (float)time;
      sedimentRunoff[0] = (float)time;


      //  Next get the data for this time for each of the selected upslope elements (these were
      //  stored in the m_cellDataArray)
      UPSLOPE_INFO *pUpslope=NULL;
      int pestNum = 0;
      int pestNum2 = 1;
      int count=1;
      for ( int i=1; i < dataToCollect; i++ )
         {
         ASSERT( m_cellDataArray[ i-1 ] >= 0 );       
         
         if (m_modelStructureMethod==WH_DISTRIBUTE_FPGRID || m_modelStructureMethod==WH_DISTRIBUTE_WTHEIGHT)
            {
            int reachCount = GetReachCount();
            bool found=false;
            int colI=m_cellDataArray[count-1];
            int rowI=m_cellDataArray[count];
            for (int l=0; l < reachCount; l++ )
		         {

               REACH_INFO_HYDRO *pHydro = m_reachInfoHydroArray[ l ];
               for ( int x=pHydro->upslopeInfoArray.GetSize()-1;x >= 0; x--)
                  {
                  pUpslope = pHydro->upslopeInfoArray[ x ];
                  if (pUpslope->colIndex==m_cellDataArray[count-1] && pUpslope->rowIndex==m_cellDataArray[count])
                     {
                     found=true;
                     count=count+2;
                     break;
                     }
                  }
               if (found)
                   break;
               }
            }
         else //model structure is not a grid...
            {      
            int cell = m_cellDataArray[ i-1 ];
            pUpslope = GetCellUpslopeInfo( cell );
            ASSERT( pUpslope != NULL );
            }
         float swc=0.0f;
         if (m_checkUnsaturated)
            swc = pUpslope->pUnsat->swcUnsat;
         else
            swc = pUpslope->swc;

         float tracer = pUpslope->cumTracerMassInPrecip;
         float tracerCon = pUpslope->tracerMass/pUpslope->volumeSat*1000.0f;
         float etRate = pUpslope->currentET;
         float sedimentRunoffRate = pUpslope->surfaceOutflowVol;
         

         data[ i ] = swc;
         
         if (m_checkPesticide)
            {
            for (int k=0; k < m_numberPesticides; k++)
               {
               pestNum++;
               pestVegData[pestNum] = pUpslope->pesticideArray[k]->pVeg->rateFoliarRunoff;
               pestSurfData[pestNum] = pUpslope->pesticideArray[k]->pSurfWater->concentration;
               
               if (pUpslope->pondedVolume != 0.0f)
                  pestSurfData[pestNum] = (pUpslope->pesticideArray[k]->pSurfWater->mass) / pUpslope->pondedVolume;
               // Below the surface is the root zone.  The depth of this zone must be defined.  It is likely quite dependent upon landcover
               // For now, it is 0.25 meter!
               // in that length, we must calculate a volume of water and pesticide mass
               pestUnsatData[pestNum] = pUpslope->pesticideArray[k]->pUnsatWater->concentration;
               pestSatData[pestNum] = pUpslope->pesticideArray[k]->pSatWater->concentration;
               }
          
            for (int k=0; k < m_numberPesticides; k++)
               {
               PESTICIDE_INFO *pMass = pUpslope->pesticideArray[k];
               pestMassBalanceData[pestNum2++]=pMass->pSurfWater->cumErosionLoss;
               pestMassBalanceData[pestNum2++]=pMass->pSurfWater->cumMassApp;
               pestMassBalanceData[pestNum2++]=pMass->pVeg->cumMassInput;
               pestMassBalanceData[pestNum2++]=pMass->pVeg->cumTransformation;
               pestMassBalanceData[pestNum2++]=pMass->pSurfWater->cumRunoff;
               pestMassBalanceData[pestNum2++]=pMass->pUnsatWater->cumTransformation;
               pestMassBalanceData[pestNum2++]=pMass->pSatWater->cumTransformation;
               pestMassBalanceData[pestNum2++]=pMass->pSurfWater->cumTransformation;
               pestMassBalanceData[pestNum2++]=pMass->pUnsatWater->cumUptake;
               pestMassBalanceData[pestNum2++]=pMass->pSurfWater->cumUptake;
               }
            }
         if (m_checkUnsaturated)
            {
            unsatData[i] = pUpslope->pUnsat->unsaturatedDepth * 1000.0f;
            satDeficit[i] = float( pUpslope->pUnsat->usfVol/m_timeStep );
            }
         else
            {
            unsatData[i]=0.0f;
            satDeficit[i] = pUpslope->volumeSat ;
            }
         satDepth[i] = pUpslope->saturatedDepth;
         
         x[ i ] = tracer /*pUpslope->lateralOutflowVol/m_timeStep*/;
         tracerConc[ i ] = tracerCon;
         etData[i]=/*etRate*/ pUpslope->pondedVolume;
         sedimentRunoff[i]=sedimentRunoffRate;
		   climate[ i ] = /*pUpslope->currentPrecip*/ pUpslope->surfaceOutflowVol;
      } //ends for ( int i=1; i < dataToCollect; i++ )
      
      m_pSwcData->AppendRow(data );  
      m_pPestVegData->AppendRow(pestVegData);
      m_pPestSurfData->AppendRow(pestSurfData);
      m_pPestUnsatData->AppendRow(pestUnsatData);
      m_pPestSatData->AppendRow(pestSatData);
      m_pPestMassBalanceData->AppendRow(pestMassBalanceData);
      m_pUnsatDepthData->AppendRow(unsatData);
      m_pSatDepthData->AppendRow(satDepth);
      m_pSatDeficitData->AppendRow(satDeficit);
      m_pTracerConcData->AppendRow( tracerConc );
      m_pUplandStorageData->AppendRow( x );
	   m_pModelClimateData->AppendRow( climate );
      m_pETData->AppendRow(etData);
      m_pUplandSedimentData->AppendRow(sedimentRunoff);

      delete x;

      delete data;
      delete pestVegData;
      delete pestSurfData;
      delete pestUnsatData;
      delete pestSatData;
      delete pestMassBalanceData;
      delete unsatData;
      delete satDepth;
      delete satDeficit;
      delete climate;
      delete etData;
      delete sedimentRunoff;

   // Collect unsaturated zone rate data (there are more than 1 value for each time, so it requires a bit
      //  different loop...

      if (m_modelStructureMethod!=WH_DISTRIBUTE_FPGRID && m_modelStructureMethod!=WH_DISTRIBUTE_WTHEIGHT)
         {
            if (m_checkUnsaturated)
            {
            int dataToCollect2 = m_cellDataArray.GetSize();
            int number = (dataToCollect2*3 ) + 1;
            float *unsatRates = new float[number];
            unsatRates[ 0 ] = (float)m_time;
            int count = 1;
            for (int i=0; i < dataToCollect2; i++ )
               {
                  int cell = m_cellDataArray[ i ];
                  UPSLOPE_INFO *pUpslope = GetCellUpslopeInfo( cell );
                  float c = pUpslope->pHydroParam->fieldCapac/pUpslope->pHydroParam->phi;
                  float d = pUpslope->pHydroParam->wiltPt/pUpslope->pHydroParam->phi;

                  float uMax = c*((pUpslope->pHydroParam->soilDepth*pUpslope->pHydroParam->phi) - (pUpslope->volumeSat/pUpslope->area));
                  float uMin = d*((pUpslope->pHydroParam->soilDepth*pUpslope->pHydroParam->phi) - (pUpslope->volumeSat/pUpslope->area));

                  unsatRates[ count ] = pUpslope->pHydroParam->wiltPt;
                  count++;
                  unsatRates[count] = pUpslope->pHydroParam->fieldCapac;
                  count++;
                  unsatRates[count] = pUpslope->pUnsat->swcUnsat;
                  count++;
                  }
            
            m_pRateData->AppendRow(unsatRates);
            
            delete unsatRates;
            }
         }

 ////////////////////////////////////////////////////////////
      // Collect Data for the Time Step
	   float *timeStepData = new float[ 2 ];
      timeStepData[ 0 ] = (float)time;
      timeStepData[ 1 ] = (float)m_timeStep;

      m_pGeneralData->AppendRow( timeStepData );
      delete timeStepData;

         ////////////////////////////////////////////////////////////
         // Collect Data for SedimentOutflow

      ReachNode *pNode = NULL;
      for ( int j=0; j < m_reachTree.m_nodeCount-1; j++ )        // so we must figure out what is upstream of it
         {
         pNode = m_reachTree.m_nodeArray[ j ];
         if ( pNode->m_pDown == m_reachTree.m_nodeArray[ m_reachTree.m_nodeCount-1 ] )        
            break;
         }

      REACH_INFO_HYDRO *pHydro = (REACH_INFO_HYDRO*)pNode->m_reachInfo.pData;
      float sedOut = pHydro->sedimentDischargeArray[0];;  

	   float *sedimentOutflow = new float[ 2 ];
      sedimentOutflow[ 0 ] = (float)time;
      sedimentOutflow[ 1 ] = sedOut;

      m_pSedimentOutflowData->AppendRow( sedimentOutflow );
      delete sedimentOutflow;

      }// ends if single simulation!
   ////////////////////////////////////////////////////////
   // Go through the same process but collect reach-level data

   int dataToCollect = m_reachDataArray.GetSize() + 1;  // include time

   float *discharge = new float[ dataToCollect ];
   float *tracer = new float[dataToCollect];
   float *sediment = new float[dataToCollect];
   float *tracerMass = new float[dataToCollect];
   float *cumSed = new float[dataToCollect];
   float *pestDischarge = new float[((dataToCollect-1)*m_numberPesticides)+1];
      
  
   discharge[0] = (float)time ;
   tracer[0] = (float)time;
   sediment[0] = (float)time;
   cumSed[0] = (float)time;
   pestDischarge[0]=(float)time;
   tracerMass[0]=(float)time;

   ASSERT( m_pStreamLayer != NULL );
   DataObj *pStreamData = m_pStreamLayer->m_pData;

   //  Discharge is stored in the REACHTREE!

   for (int i=1; i < dataToCollect; i++ )//discharge[0] is time, so start at discharge[1]
      {
      int reachIndex = m_reachDataArray[ i-1 ];//A list of reaches to be sampled
      int last = m_reachTree.m_nodeArray[ reachIndex ]->m_reachInfo.subnodeCount - 1;
      discharge[ i ] = m_reachTree.m_nodeArray[ reachIndex ]->m_reachInfo.qArray[ last ];
      }
   m_pInstreamData->AppendRow( discharge ); 
  if (m_checkMonteCarlo==WH_SINGLE_SIMULATION)
      {
      int row = m_pInstreamData->GetRowCount(); //  here we grab the last modeled result, which is also the current modeled result
  //    m_pMeasuredDischargeData->Add(0,row,time); //this adds to an updating plot of Q
    //  m_pMeasuredDischargeData->Add(1,row,discharge[1]); //this adds to an updating plot of Q
    //  m_pMeasuredDischargeData->Add(2,row,discharge[2]); //this adds to an updating plot of Q
      }
   
//////////////////////////////////////////////////////////////////
   //  And tracer and sediment data is stored in REACH_INFO_HYDRO! (it is model specific)
   for ( i=1; i < dataToCollect; i++ )
      {
      int reachIndex = m_reachDataArray[ i-1 ];
      REACH_INFO_HYDRO * pInfo = (REACH_INFO_HYDRO*)m_reachTree.m_nodeArray[reachIndex]->m_reachInfo.pData;
      if (m_time<=m_startTime+2.0)
         tracer[i]=1.0f;//starts out,before rain, as all old water
      else
      {
      //tracer[i] = ((pInfo->tracerConc*1000.0f) - m_initTracerConc)/(m_editTracerMgL - m_initTracerConc)*discharge[i];
      float perOld = ((pInfo->tracerConc*1000.0f) - m_editTracerMgL )/(m_initTracerConc -  m_editTracerMgL);
      tracer[i]=perOld;
      }
     // tracer[i] = pInfo->tracerConc*1000.0f;
     // sediment[i]=pInfo->sedimentConc;
      float prev=0.0f;
      if (m_pTracerMassChannelData->GetRowCount()==0)
         prev=0.0f;
      else 
         prev=m_pTracerMassChannelData->GetAsFloat(1,m_pTracerMassChannelData->GetRowCount()-1);
      if ((m_massBalanceInfo.tracerInputMass+m_massBalanceInfo.initialTracerMass)==0.0f )
         tracerMass[i] = prev + 0.0f;
      else
         tracerMass[i] =  prev+((pInfo->tracerMassArray[0]/(float)m_timeStep)/(m_massBalanceInfo.tracerInputMass+m_massBalanceInfo.initialTracerMass ));
 
      //tracerMass[i] = (pInfo->tracerMassArray[0]);
    //  sediment[i] = tracer[i]*discharge[i];
      m_rtIntegral += (tracerMass[i]*m_collectionInterval);
      int last = m_reachTree.m_nodeArray[ reachIndex ]->m_reachInfo.subnodeCount - 1;
      sediment[i]=pInfo->tracerDischargeArray[last]/m_reachTree.m_nodeArray[ reachIndex ]->m_reachInfo.qArray[ last ]*1000.0f; //kg/s/m3
      }
   m_pTracerConcChannelData->AppendRow( tracer ); 
   m_pTracerMassChannelData->AppendRow(tracerMass);
   m_pInstreamSedimentData->AppendRow( sediment ); 

   delete tracer;
   delete sediment;
   delete discharge;
   delete tracerMass;

 // pesticide (instream) are stored in the InstreamPesticideArray (and array of INSTREAM_PESTICIDE_INFOs)
   int pestCount=1;
   for ( i=1; i < dataToCollect; i++ )
      {
      int reachIndex = m_reachDataArray[ i-1 ];
      REACH_INFO_HYDRO *pInfo = (REACH_INFO_HYDRO*)m_reachTree.m_nodeArray[reachIndex]->m_reachInfo.pData;
      for (int k = 0; k<m_numberPesticides; k++)
         {
         float pest = pInfo->instreamPesticideArray[k]->pesticideConc;
         
         pestDischarge[pestCount] = pInfo->instreamPesticideArray[k]->pesticideConc;
         pestCount++;
         }    
      }
   m_pPestInstreamData->AppendRow( pestDischarge ); 
   delete pestDischarge;


  
   ////////////////////////////////////////////////////////////
   // Collect Data Mass Balance. These methods accumulate mass across the entire modeled watershed

   float cumETVol = 0.0f;
   float cumGroundLoss = 0.0f;
   float cumPrecipVol = 0.0f;
   float cumOutflowVol = 0.0f;
   float cumTracerMassInPrecip = 0.0f;
   float cumTracerOutflowMass = 0.0f;
   float cumSedimentOutflowMass = 0.0f;

   CalcCumulativeUplandVolume(cumETVol, cumPrecipVol, cumTracerMassInPrecip, cumGroundLoss);  // This does the same as the following code, but is a simpler integration
   m_massBalanceInfo.tracerInputMass = cumTracerMassInPrecip;
   CalcCumulativeChannelVolume(cumOutflowVol,cumTracerOutflowMass, cumSedimentOutflowMass);

	float *massBalanceData = new float[ 5 ];
   massBalanceData[ 0 ] = (float)time;
   massBalanceData[ 1 ] = cumETVol/m_areaTotal*1000.0f;
   massBalanceData[ 2 ] = cumPrecipVol/m_areaTotal*1000.0f;
   massBalanceData[ 3 ] = cumOutflowVol/m_areaTotal*1000.0f;

   massBalanceData[ 4 ]   = cumGroundLoss/m_areaTotal*1000.0f;

   m_pMassBalanceData->AppendRow( massBalanceData );
   delete massBalanceData;

   float *cumSedData = new float[2];
   cumSedData[ 0 ] = (float)time;
   cumSedData[ 1 ] = cumSedimentOutflowMass;
   m_pTotalSedimentOutflowData->AppendRow(cumSedData);
   delete cumSedData;


   }

void WET_Hydro::PlotOutput()
   {
	SaveData();
	CTreeCtrl& tree = gpResultPlotWnd->m_pResultTree->GetTreeCtrl();

	tree.DeleteAllItems();
	HTREEITEM hAnimals = tree.InsertItem("Model Result Data");
	int NumberofRuns = m_resultDataArray.GetSize();

	for (int j=0; j < NumberofRuns; j++ )
		{
		char run[ 32 ];
		lstrcpy( run, "" );
		itoa( j, run+lstrlen( run ), 10 );
		HTREEITEM hRunNumber = tree.InsertItem( run, hAnimals);
      HTREEITEM hHydrology = tree.InsertItem("Hydrology", hRunNumber);
      HTREEITEM hTracer = tree.InsertItem("Tracer", hRunNumber);
      HTREEITEM hSediment = tree.InsertItem("Sediment Data", hRunNumber);
      HTREEITEM hPesticide = tree.InsertItem("Pesticide Data", hRunNumber);
      HTREEITEM hGeneral  = tree.InsertItem("General Data", hRunNumber);
         
   		HTREEITEM hClimateData = tree.InsertItem( "Climate Data", hGeneral);
			HTREEITEM hInstreamData = tree.InsertItem( "Stream Discharge", hHydrology);
			HTREEITEM hETData = tree.InsertItem( "ET Rate", hHydrology);
			HTREEITEM hSwcData = tree.InsertItem( "Average Soil Water Content", hHydrology);
         
            
            HTREEITEM hUnsatData = tree.InsertItem( "Unsaturated Zone Depth", hHydrology);
            HTREEITEM hRateData = tree.InsertItem( "Unsat Volumes", hHydrology);
            
         HTREEITEM hSatData = tree.InsertItem( "Saturated Zone Depth", hHydrology);
         HTREEITEM hSatDeficitData = tree.InsertItem( "Vert.(unsat) Flow Rate", hHydrology);
			HTREEITEM hMassBalanceData = tree.InsertItem( "Mass Balance", hHydrology);
			HTREEITEM hUplandStorageData = tree.InsertItem( "Horiz. (sat) Flow Rate", hHydrology);
         
			HTREEITEM hTracerData = tree.InsertItem( "Tracer Mass Upslope", hTracer);
			HTREEITEM hTimeStepData = tree.InsertItem( "Time Step", hGeneral);	
			HTREEITEM hTracerConcData = tree.InsertItem( "Tracer Conc", hTracer);
			HTREEITEM hTracerConcChannelData = tree.InsertItem( "Percent Old Water in Channel", hTracer);	
         HTREEITEM hTracerMassChannelData = tree.InsertItem( "Tracer Mass in Channel", hTracer);
			HTREEITEM hUplandSedimentData = tree.InsertItem( "Upland Sediment", hSediment);
			HTREEITEM hInstreamSedimentData = tree.InsertItem( "Instream Tracer Conc. (mgl)", hTracer);	
			HTREEITEM hCumSedimentOutflowData = tree.InsertItem( "Cumulative Sediment Outflow", hSediment);	
			HTREEITEM hSedimentOutflowData = tree.InsertItem( "Instream Tracer Conc", hTracer);	
			HTREEITEM hPestVegData = tree.InsertItem( "Foliar Runoff Rate", hPesticide);	
         HTREEITEM hPestSurfData = tree.InsertItem( "Surface Concentration" , hPesticide);
         HTREEITEM hPestUnsatData = tree.InsertItem( "Unsat Zone Concentration", hPesticide);
         HTREEITEM hPestSatData = tree.InsertItem( "Sat Zone Concentration (leached)", hPesticide);
         HTREEITEM hPestInstreamData = tree.InsertItem( "Instream Concentration", hPesticide);
         HTREEITEM hPestMassBalanceData = tree.InsertItem( "Pest Mass Balance", hPesticide);

      }
	DWORD dwStyle = GetWindowLong(tree.GetSafeHwnd(),GWL_STYLE);
	dwStyle |= TVS_HASLINES + TVS_HASBUTTONS + TVS_LINESATROOT  ;	
	SetWindowLong(gpResultPlotWnd->m_pResultTree->GetTreeCtrl().GetSafeHwnd(), GWL_STYLE, dwStyle);
	}



void WET_Hydro::EndRun(  )
   {

      // MATT
   if(m_saveEmfOutput)
      {
      for(int i = 0; i < numMovies; i++)
         {
         AVIGenerators[i]->ReleaseEngine();
         delete AVIGenerators[i];
         AVIGenerators[i] = 0;

         ReleaseDC(NULL, auxMovieStuff[i].bitmapDC);
         delete [] auxMovieStuff[i].bitAddress;
         }
      delete [] AVIGenerators;
      delete [] auxMovieStuff;
      AVIGenerators = 0;
      }

   float initSoilVol=0.0f;
   float initTracerMass=0.0f;
   float initChannelVol = CalcInitChannelVol();
   CalcInitSoilVol(initSoilVol,initTracerMass);
   float finalChannelVol=0.0f;
   float finalChannelTracerMass=0.0f;
   CalcFinalChannelVol( finalChannelVol,  finalChannelTracerMass);
   float finalSoilVol=0.0f;
   float finalPondedVol = 0.0f;
   float finalTracerMass = 0.0f;
   CalcFinalSoilVol(finalSoilVol, finalPondedVol, finalTracerMass);
   float channelStorage = finalChannelVol - initChannelVol;
   float soilStorage = finalSoilVol + finalPondedVol - initSoilVol;
   
   float cumETVol = 0.0f;
   float cumGroundLoss = 0.0f;
   float cumPrecipVol = 0.0f;
   float cumOutflowVol = 0.0f;
   float cumTracerMassInPrecip = 0.0f;
   float cumTracerOutflowMass = 0.0f;
   float cumSedimentOutflowMass = 0.0f;
   
   
   CalcCumulativeUplandVolume(cumETVol, cumPrecipVol,cumTracerMassInPrecip, cumGroundLoss);
   
   CalcCumulativeChannelVolume(cumOutflowVol, cumTracerOutflowMass, cumSedimentOutflowMass);
   if (m_checkPesticide)
      {
      CalcCumulativeUplandPest();
      CalcCumulativeInstreamPest();
      }


   m_massBalanceInfo.etVolume = cumETVol;
   m_massBalanceInfo.groundLossVol = cumGroundLoss;
   m_massBalanceInfo.finalChannelVolume = finalChannelVol;
   m_massBalanceInfo.finalSoilVolume = finalSoilVol;
   m_massBalanceInfo.finalPondedVolume = finalPondedVol;
   m_massBalanceInfo.initChannelVolume = initChannelVol;
   m_massBalanceInfo.initSoilVolume = initSoilVol;
   m_massBalanceInfo.rainInputVolume = cumPrecipVol;
   if (cumOutflowVol == 0.0f)
      cumOutflowVol = 0.0001f;
   m_massBalanceInfo.streamflowVolume = cumOutflowVol;

   m_massBalanceInfo.initialTracerMass = initTracerMass;
   m_massBalanceInfo.finalTracerMass = finalTracerMass;
   m_massBalanceInfo.tracerInputMass = cumTracerMassInPrecip;
   m_massBalanceInfo.tracerOutputMass = cumTracerOutflowMass;
   m_massBalanceInfo.sedimentOutputMass = cumSedimentOutflowMass;

   float HRT = float( (initSoilVol + cumPrecipVol)/(cumOutflowVol/(m_stopTime-m_startTime)));
   m_massBalanceInfo.ns3 = HRT;
  
// Calculate the percent new water in the volumeSat that left over the simulation period.
   float newWaterInput = m_massBalanceInfo.tracerInputMass/m_massBalanceInfo.rainInputVolume;

   float initialConc = m_massBalanceInfo.initialTracerMass/m_massBalanceInfo.initSoilVolume;
   float finalConc = m_massBalanceInfo.finalTracerMass/m_massBalanceInfo.finalSoilVolume;
   float concChange = initialConc-finalConc;               

   float outputTotal = m_massBalanceInfo.tracerOutputMass/m_massBalanceInfo.streamflowVolume;

  // float newWaterOutput = outputTotal * newWaterInput/(oldWaterInput+newWaterInput);

   //float newWaterOutput = outputTotal-(outputTotal*finalMass/(newWaterInput+oldWaterInput));
   float newWaterOutput = (outputTotal-initialConc)/(newWaterInput - initialConc);
   float cOld = (outputTotal-newWaterInput)/(initialConc - newWaterInput)*100;
   m_massBalanceInfo.perNewWater = newWaterOutput * 100.0f;
   // the percent of time some place in the water was saturated
   m_massBalanceInfo.freqSat = m_massBalanceInfo.freqSat/((float)m_stopTime-(float)m_startTime)*100.0f;

//  Data in the reachtree (as opposed to upland info) data strucs are not reinitialized when
//  recalculating model structure.  This  next code resets the accumulated mass balance type info
   //  back to 0.0f
   int reachCount = GetReachCount();
   float areaOver = 0.0f; //the area in the watershed that produced overland flow at some point
   
   // during the simulation
   float area = 0.0f; //the total area in the watershed
   for ( int i=0; i < reachCount; i++ )
		{
      ReachNode *pNode = m_reachTree.m_nodeArray[i];
      REACH_INFO       *pInfo  = &m_reachTree.m_nodeArray[ i ]->m_reachInfo;
      REACH_INFO_HYDRO *pHydro = m_reachInfoHydroArray[ i ];
      if (pNode->m_pDown->m_reachInfo.index == ROOT_NODE)
         {
         m_massBalanceInfo.hrt = pHydro->resTime;
         m_massBalanceInfo.areaOver = pHydro->firstMoment;
         m_massBalanceInfo.ns2 = pHydro->zerothMoment;
         }
      pHydro->cumSedimentOutflow = 0.0f;
      pHydro->cumTracerOutFlow = 0.0f;
      pHydro->cumTracerInput = 0.0f;
      for (int j=0; j< pHydro->upslopeInfoArray.GetSize(); j++)
         {
         area+=pHydro->upslopeInfoArray[j]->area;
         if (pHydro->upslopeInfoArray[j]->producedOverlandFlow == 1)
            areaOver+=pHydro->upslopeInfoArray[j]->area;
         }
      }
   m_massBalanceInfo.areaOver = areaOver/area*100.0f;
   
   //add the measured data to m_pInstreamData, in the last column
	if (m_pMeasuredDischargeData != NULL)
		{
		   for (int row=0; row < m_pInstreamData->GetRowCount(); row++ ) 
			   {
			   float time = m_pInstreamData->Get(0, row);
			   float measuredDischarge = m_pMeasuredDischargeData->IGet(time, 1, IM_LINEAR);
            float modelDischarge = m_pInstreamData->IGet(time, 1, IM_LINEAR);
			   UINT numCols = m_pInstreamData->GetColCount();
			   m_pInstreamData->Set( numCols - 1, row, measuredDischarge );
			   }
      GetGoodnessOfFitValues();
      for (int k=0;k<m_goodnessOfFitInfoArray.GetSize();k++)
         {
         GOODNESS_OF_FIT_INFO *pGoodness = m_goodnessOfFitInfoArray[k];
         if (k==0)
            {
            m_massBalanceInfo.rmse1 = pGoodness->rmse;
            m_massBalanceInfo.ns1   = pGoodness->ns;
            m_massBalanceInfo.d =     pGoodness->d;
            m_massBalanceInfo.r2 =    pGoodness->r2;
            }
         if (k==1)
            {
            m_massBalanceInfo.rmse2 = pGoodness->rmse;
            m_massBalanceInfo.ns2   = pGoodness->ns;
            }
         if (k==2)
            {
            m_massBalanceInfo.rmse3 = pGoodness->rmse;
            m_massBalanceInfo.ns3   = pGoodness->ns; 
            }
         }
         
         
      // if we have measured tracer data, it is assumed to be in the 3rd column of the discharge data
      if (m_checkTracerModel  /*&& m_radioTracerInput==0*/)
         {
         for (int row=0; row < m_pTracerConcChannelData->GetRowCount(); row++ ) 
			   {
			   float time = m_pTracerConcChannelData->Get(0, row);
			   float measuredConc = m_pMeasuredDischargeData->IGet(time, 1, IM_LINEAR);
            float simuConc = m_pInstreamData->IGet(time,1,IM_LINEAR);
			   UINT numCols = m_pTracerConcChannelData->GetColCount();
			   m_pTracerConcChannelData->Set( numCols - 1, row, 0 );
         //   m_pTracerMassChannelData->Set( numCols - 1, row, 0 );
			   }
         }

    /*  if (m_pMeasuredDischargeData != NULL)
         {
         float crap=1.0f;
         }
      else
         {
		   for (int row=0; row < m_pInstreamData->GetRowCount(); row++ ) 
			   {
			   UINT numCols = m_pInstreamData->GetColCount();
			   m_pInstreamData->Set( numCols - 1, row, 0 );
			   }
         }*/
      }

		
   //  collect Monte Carlo Results

   if (m_checkMonteCarlo==WH_MONTE_CARLO )
      {
      if (m_runNumber==0 && m_checkTracerModel) //add time to the first column in the data storage
         {
         m_pMonteCarloQTimeSeries->SetLabel(0,"Time");
         m_pMonteCarloQTimeSeries->SetLabel(1,"Measured");
         if (m_checkTracerModel)
            m_pMonteCarloTracerTimeSeries->SetLabel(0,"Time");   
         for (int i=0; i < m_pInstreamData->GetRowCount(); i++ )//discharge[0] is time, so start at discharge[1]
            {
            float *time = new float[ 2 ];
            float *data = new float[1];
          //  float value=0.0f;
            m_pInstreamData->Get(0,i,time[0]);
            m_pInstreamData->Get(0,i,data[0]);
            m_pInstreamData->Get(m_pInstreamData->GetColCount()-1,i,time[1]);
            m_pMonteCarloQTimeSeries->AppendRow(time);

           if (m_checkTracerModel)
              {
              m_pMonteCarloTracerTimeSeries->AppendRow(data);
              
              }
            delete time;
            delete data;
            }      
         }
      if (m_massBalanceInfo.ns1 >= 0.75f && m_checkTracerModel)
         {      
         CString msg;
         msg.Format( "Run%i", m_runNumber );
         m_pMonteCarloQTimeSeries->AppendCol(msg);
         if (m_checkTracerModel)
            m_pMonteCarloTracerTimeSeries->AppendCol(msg);

         for (int i=0; i < m_pInstreamData->GetRowCount(); i++ )//discharge[0] is time, so start at discharge[1]
            {
            float value=0.0f;
            m_pInstreamData->Get(1,i,value);
            //if (m_pMonteCarloQTimeSeries->GetColCount() > i)
               m_pMonteCarloQTimeSeries->Set(m_pMonteCarloQTimeSeries->GetColCount()-1,i,value);

           if (m_checkTracerModel)
              {
              m_pTracerConcChannelData->Get(1,i,value);
         //     if (m_pMonteCarloQTimeSeries->GetColCount() > i)
                 m_pMonteCarloTracerTimeSeries->Set(m_pMonteCarloTracerTimeSeries->GetColCount()-1,i,value);
              
              }
            }      

         }
      if (m_massBalanceInfo.ns1 >= -10.0f)
         {
         if (m_modelParamMethod != WH_PARAM_WATERSHEDS) //then each upslope unit might have a different set of parameters!
            {
            for (i=0; i < reachCount; i++ )
               {
               REACH_INFO       *pInfo  = &m_reachTree.m_nodeArray[ i ]->m_reachInfo;
               REACH_INFO_HYDRO *pHydro = m_reachInfoHydroArray[ i ];        
               pHydro->cumSedimentOutflow = 0.0f;
               pHydro->cumTracerOutFlow = 0.0f;
               for (int j=0; j< pHydro->upslopeInfoArray.GetSize(); j++)
                  {
                  UPSLOPE_INFO *pUpslope = pHydro->upslopeInfoArray[j];
                  float *monte = new float[17];
                  monte[0] = (float)m_runNumber;
                  monte[1] = m_massBalanceInfo.rmse1;
                  monte[2] = m_massBalanceInfo.ns1;
                  monte[3] = m_massBalanceInfo.areaOver;
                  monte[4] = m_massBalanceInfo.r2;

                  monte[5]= pUpslope->pHydroParam->phi;
                  monte[6] = pUpslope->pHydroParam->m;            
                  monte[7] = pUpslope->pHydroParam->initSat;
                  monte[8] = m_massBalanceInfo.ns2;
                  monte[9] = pUpslope->pHydroParam->kSat;
                  monte[10] = pUpslope->pHydroParam->soilDepth;
                  monte[11] = pUpslope->pHydroParam->kDepth;
                  monte[12] = m_massBalanceInfo.perNewWater;   
                  monte[13] = m_massBalanceInfo.hrt;
                  monte[14]=  pUpslope->pHydroParam->vanGenN;
                  monte[15]= pUpslope->pHydroParam->powerLExp;
                  monte[16]= pUpslope->pHydroParam->wiltPt;
                  m_pMonteCarloResults->AppendRow(monte);
                  delete monte;
                 
                  }
               }
            }
               
         else // if not, then the parameters are going to be the same everywhere, so just collect the first set!
            {
            REACH_INFO       *pInfo  = &m_reachTree.m_nodeArray[ 0 ]->m_reachInfo;
            REACH_INFO_HYDRO *pHydro = m_reachInfoHydroArray[ 0 ];  
            UPSLOPE_INFO *pUpslope = pHydro->upslopeInfoArray[0];
            float *monte = new float[17];
            monte[0] = (float)m_runNumber;
            monte[1] = m_massBalanceInfo.rmse1;
            monte[2] = m_massBalanceInfo.ns1;
            monte[3] = m_massBalanceInfo.ns3;
            monte[4] = m_massBalanceInfo.r2;

            monte[5]= pUpslope->pHydroParam->phi;
            monte[6] = pUpslope->pHydroParam->m;            
            monte[7] = pUpslope->pHydroParam->initSat;
            monte[8] = m_massBalanceInfo.ns2;
            monte[9] = pUpslope->pHydroParam->kSat;
            monte[10] = pUpslope->pHydroParam->soilDepth;
            monte[11] = pUpslope->pHydroParam->kDepth;
            monte[12] = m_massBalanceInfo.perNewWater;   
            monte[13] = m_massBalanceInfo.hrt;
            monte[14]=  pUpslope->pHydroParam->vanGenN;
            monte[15]= pUpslope->pHydroParam->powerLExp;
            monte[16]= pUpslope->pHydroParam->wiltPt;
            m_pMonteCarloResults->AppendRow(monte);
            delete monte;              
            }
           
         }
      }
      m_rtIntegral=0.0f;
   }

   //  m_goodnessOfFitInfoArray has a GOODNESS_OF_FIT for each column in the 
   //  measuredDischarge dataobject.

void WET_Hydro::GetGoodnessOfFitValues()
   {
   // measuredDischargeData was read from a textfile referenced in the ini file.
   // it has the number of columns corresponding to the number of measurements.
   // usually this is one, but it may be more than one. (when there's more than 1 hydrograph

   for (int i=0; i<m_goodnessOfFitInfoArray.GetSize();i++)
      delete m_goodnessOfFitInfoArray[i];
   m_goodnessOfFitInfoArray.RemoveAll();
   int crap = m_pMeasuredDischargeData->GetColCount();
   for ( i=0;i<1;i++)//subtract 2 for the time col and the measured col
      {
      GOODNESS_OF_FIT_INFO *pGoodness = new GOODNESS_OF_FIT_INFO;
      int n = m_pInstreamData->GetRowCount();
      float obsMean=0.0f, predMean=0.0f;

      for (int j=20;j<n;j++) //150this skips the first 30 days (assuming collection every 0.2). Assuming those values
         // might be unacceptable due to initial conditions...
         {
			float time = m_pInstreamData->Get(0, j);
			obsMean += m_pMeasuredDischargeData->IGet(time,i+1 ,IM_LINEAR); //add up all the discharge values and then...
		   predMean += m_pInstreamData->IGet(time, i+1, IM_LINEAR);
         }
      obsMean = obsMean/(float)n; // divide them by the sample size to generate the average.
      predMean = predMean/(float)n; // divide them by the sample size to generate the average.

      float errorSum=0.0f, errorSumDenom=0.0f, dDenom=0.0f, r2Num=0.0f, r2Denom1=0.0f, r2Denom2=0.0f;
		for (int row=0; row < n; row++ ) 
			{
         float pred =0.0f, obs=0.0f;
			float time = m_pInstreamData->Get(0, row);
			obs = m_pMeasuredDischargeData->IGet(time, i+1, IM_LINEAR);

         m_pInstreamData->Get(i+1,row,pred); // Assuming col i holds data for the reach corresponding to measurement

//  THIS WILL FAIL IF m_pInstreamData->count < m_pMeasuredData->count!!!!!!!!1111
         errorSum+=((obs-pred)*(obs-pred));
         errorSumDenom+=(obs-obsMean)*(obs-obsMean);
         dDenom += (float)pow((fabs(pred-obsMean) + fabs(obs - obsMean)),2);
         r2Num += ((obs-obsMean)*(pred-predMean));
         r2Denom1 +=(obs-obsMean)*(obs-obsMean);
         r2Denom2 += (pred-predMean)*(pred-predMean);
			}
      float oneOverN = 1.0f/n;
      pGoodness->rmse = (float)sqrt((oneOverN * errorSum));
      pGoodness->ns   = 1 - (errorSum/errorSumDenom);
      pGoodness->d = 1-(errorSum)/(dDenom);
      pGoodness->r2=(float)pow(r2Num/((float)sqrt(r2Denom1)*(float)sqrt(r2Denom2)),2);
      m_goodnessOfFitInfoArray.Add(pGoodness);
      }
   }


float WET_Hydro::CalcDPrecipVol_dt(UPSLOPE_INFO *pUpslope)
   {
  //    pHydroLast->cumTracerOutFlow += (totalTracerDischarge * (float)m_timeStep);
   float dPrecipVol_dt = pUpslope->currentPrecip  * pUpslope->area;
   return dPrecipVol_dt ;
   }
float WET_Hydro::CalcDGroundVol_dt(UPSLOPE_INFO *pUpslope)
   {
   float dGroundVol_dt = pUpslope->pHydroParam->kDepth  * pUpslope->area;
   return dGroundVol_dt ;
   }

float WET_Hydro::CalcDEtVol_dt(UPSLOPE_INFO *pUpslope)
   {
   float dETVol_dt = pUpslope->currentET  * pUpslope->area;
   return dETVol_dt;
   }
float WET_Hydro::CalcDTracerMassInPrecip_dt(UPSLOPE_INFO *pUpslope)
   {
   float tracerInConc=0.0f;
   if (m_radioTracerInput==1)
      {
     // if ( m_time>m_startTime+2.1f && m_time<m_startTime+2.6f )//don't add tracer for the first day
// this app procedure is useful for long term residence time calculations
    //  if ( m_time<m_startTime+10.0f )
         tracerInConc=m_editTracerMgL/1000.0f;
    //  else
        // tracerInConc=0.0f;
      }
   else //we are reading in time series data
      tracerInConc=0.0f;

   float dTracerMassInPrecip_dt = (pUpslope->currentPrecip*pUpslope->area*tracerInConc);
   return dTracerMassInPrecip_dt;
   }

float WET_Hydro::CalcDPesticideMassInput_dt(UPSLOPE_INFO *pUpslope, int i)
   {
   float dPesticideMassInput_dt = pUpslope->pesticideArray[i]->pVeg->rateMassApp*pUpslope->area/10000.0f;
   if (dPesticideMassInput_dt > -0.0000000001 && dPesticideMassInput_dt < 0.0000000001)
      dPesticideMassInput_dt =0.0f;
   return dPesticideMassInput_dt;
   }

float WET_Hydro::CalcInitChannelVol()
   {
	float initChannelVol=0.0f;
   for ( int i=0; i < m_reachInfoHydroArray.GetSize(); i++ )
      {
      ReachNode *pNode = m_reachTree.m_nodeArray[ i ];
      REACH_INFO_HYDRO *pHydro = (REACH_INFO_HYDRO*)pNode->m_reachInfo.pData;

      // get initial volumeSat
      float qInit = pNode->m_reachInfo.qInit; 
      float depthInit = GetDepthFromQ( pNode, qInit, pHydro->pInstreamHydroParam->wd );
      float widthInit = 3 * depthInit;   // ASSUMES RECTANGULAR CHANNEL GEOMETRY
      float vInit = depthInit * widthInit * pNode->m_reachInfo.length;
		initChannelVol+=vInit;
      }
   return initChannelVol;
   }

void WET_Hydro::CalcInitGridTracerMass()
   {
    //  go through all grid cells, multiply total area by concentration...
   int reachCount = GetReachCount();
   int count = 0;

   for ( int i=0; i < reachCount; i++ )
		{
      REACH_INFO       *pInfo  = &m_reachTree.m_nodeArray[ i ]->m_reachInfo;
      REACH_INFO_HYDRO *pHydro = m_reachInfoHydroArray[ i ];
		int numUpslopeBins = pHydro->upslopeInfoArray.GetSize()-1;
      
		for ( int j=numUpslopeBins ; j >= 0; j-- )  //be sure to solve upslope first (the are stored downslope first)
         {
         UPSLOPE_INFO *pUpslope = pHydro->upslopeInfoArray[ j ];
         pUpslope->initUpslopeTracerMass = pUpslope->areaTotal*pUpslope->pHydroParam->soilDepth*pUpslope->pHydroParam->initSat*pUpslope->pHydroParam->phi*(m_initTracerConc/1000);
         //not perfect - if tracer input varies spatially, this is wrong.  But it shouldn't!
         }
      }
   }
void WET_Hydro::CalcInitSoilVol(float &initSoilVol, float &initTracerMass)
   {

   for ( int i=0; i < m_reachInfoHydroArray.GetSize(); i++ )
      {
      REACH_INFO_HYDRO *pHydro = m_reachInfoHydroArray[i];
      for(int j=0; j<pHydro->upslopeInfoArray.GetSize(); j++)
         {
         UPSLOPE_INFO *pUpslope = pHydro->upslopeInfoArray[j];
         if (pUpslope->area==1.0f)
            {
            initSoilVol = 0.0001f;
            initTracerMass = 0.0001f;
            }
         else
            {
           // float volumeSat =  pUpslope->area * pUpslope->pHydroParam->soilDepth * pUpslope->pHydroParam->phi * pUpslope->pHydroParam->initSat;  
               float volumeSat =  pUpslope->voidVolume*pUpslope->pHydroParam->initSat;
               initSoilVol += volumeSat;
            float volumeUnsat=0.0f;
            if (m_checkUnsaturated)
               {
               float saturatedDepth = volumeSat / (pUpslope->area * pUpslope->pHydroParam->phi) ;
               float unsaturatedDepth = (pUpslope->pHydroParam->soilDepth - saturatedDepth);
               volumeUnsat = pUpslope->area * unsaturatedDepth * pUpslope->pHydroParam->phi * pUpslope->pHydroParam->initUnSat;
               initSoilVol += volumeUnsat;
               }
            
            
            initTracerMass += pUpslope->area * pUpslope->pHydroParam->soilDepth * pUpslope->pHydroParam->phi * pUpslope->pHydroParam->initSat*m_initTracerConc/1000.0f;
            
            if (m_checkUnsaturated)
               initTracerMass += volumeUnsat*m_initTracerConc/1000.0f;           
            
            }
         }
      }
   }

bool WET_Hydro::CalcFinalChannelVol(float &finalChannelVol, float &finalChannelTracerMass)
   {

   for ( int i=0; i < m_reachInfoHydroArray.GetSize(); i++ )
      {
      ReachNode *pNode = m_reachTree.m_nodeArray[ i ];

      REACH_INFO_HYDRO *pHydro = m_reachInfoHydroArray[i];
      float vCurrent = pHydro->depth * pHydro->width * pNode->m_reachInfo.length;
		finalChannelVol += vCurrent;
      finalChannelTracerMass += vCurrent * pHydro->tracerConc;
      }
   return true;
   }

float WET_Hydro::CalcFinalSoilVol(float &finalSoilVol, float &finalPondedVol, float &finalTracerMass)
   {
   for ( int i=0; i < m_reachInfoHydroArray.GetSize(); i++ )
      {
      REACH_INFO_HYDRO *pHydro = m_reachInfoHydroArray[i];
      for(int j=0; j<pHydro->upslopeInfoArray.GetSize(); j++)
         {
         UPSLOPE_INFO *pUpslope = pHydro->upslopeInfoArray[j];
         finalSoilVol += pUpslope->volumeSat;  
         if (m_checkUnsaturated)
            finalSoilVol += pUpslope->pUnsat->volumeUnsat;
         finalPondedVol += pUpslope->pondedVolume;
         finalTracerMass += pUpslope->tracerMass;
         
         }
      }
   return finalSoilVol;
   }

// This method accumulates mass or volumeSat for each UPSLOPE_INFO to generate
//  values for the ENTIRE simulation area

void WET_Hydro::CalcCumulativeUplandVolume(float &cumETVol, float &cumPrecipVol, float &cumTracerMassInPrecip, float &cumGroundLoss)
   {
   float totalTracerInput=0.0f;
   for ( int i=0; i < m_reachInfoHydroArray.GetSize(); i++ )
      {
      REACH_INFO_HYDRO *pHydro = m_reachInfoHydroArray[i];
      
      pHydro->cumTracerInput = 0.0f;
      for(int j=0; j<pHydro->upslopeInfoArray.GetSize(); j++)
         {
         UPSLOPE_INFO *pUpslope = pHydro->upslopeInfoArray[j];

         // Note that the values on the left represent the entire watershed, while those on the right
         // represent single upslope infos.  This is the reason for the +=
         cumETVol += pUpslope->cumETVol;
         cumGroundLoss += pUpslope->cumGroundLoss;
         cumPrecipVol += pUpslope->cumPrecipVol;
         cumTracerMassInPrecip += pUpslope->cumTracerMassInPrecip;  

         pUpslope->tileDrainageVol += (pUpslope->tracerConc*(m_time-m_startTime)*m_timeStep);
         pUpslope->tracerWeightedMeanTime += (pUpslope->tracerConc*m_timeStep);
         
         }
      pHydro->resTime=0.0f;
      if (pHydro->tracerConc!= 0.0f)
         {
         pHydro->firstMoment += (pHydro->tracerConc*(m_time-m_startTime)*m_timeStep);
         pHydro->zerothMoment += (pHydro->tracerConc*m_timeStep);
         pHydro->resTime = pHydro->firstMoment / pHydro->zerothMoment;
         }
      pHydro->cumTracerInput = cumTracerMassInPrecip;//for each reach we need to tracer Input total
      totalTracerInput+=cumTracerMassInPrecip;
      }  
   }

void WET_Hydro::CalcCumulativeUplandPest()
   {
   for ( int i=0; i < m_reachInfoHydroArray.GetSize(); i++ )
      {
      REACH_INFO_HYDRO *pHydro = m_reachInfoHydroArray[i];
      for(int j=0; j<pHydro->upslopeInfoArray.GetSize(); j++)
         {
         UPSLOPE_INFO *pUpslope = pHydro->upslopeInfoArray[j];
         
         for (int k=0; k< m_numberPesticides; k++)
            {
            PESTICIDE_INFO *pPest = pUpslope->pesticideArray[k];
            PESTICIDE_MASS_BALANCE_INFO *pMass = m_pesticideMassBalanceArray[k];
            pMass->inVeg+=0;
            pMass->inSurf+=pPest->pSurfWater->cumMassApp;
            pMass->transVeg+=pPest->pVeg->cumTransformation;
            pMass->transSurf+=pPest->pSurfWater->cumTransformation;
            pMass->transRoot+=pPest->pUnsatWater->cumTransformation;
            pMass->transSat+=pPest->pSatWater->cumTransformation;
            pMass->upSurf+=pPest->pSurfWater->cumUptake;
            pMass->upRoot+=pPest->pUnsatWater->cumUptake;
            pMass->runoffSurf+=pPest->pSurfWater->cumRunoff;
            pMass->erodeSurf+=pPest->pSurfWater->cumErosionLoss;
            }
         }
      }
   }

void WET_Hydro::CalcCumulativeInstreamPest()
   {
   ReachNode *pNode = NULL;
   for ( int j=0; j < m_reachTree.m_nodeCount-1; j++ )        // so we must figure out what is upstream of it
      {
      pNode = m_reachTree.m_nodeArray[ j ];
      if ( pNode->m_pDown == m_reachTree.m_nodeArray[ m_reachTree.m_nodeCount-1 ] )        
         break;
      }
 
   REACH_INFO_HYDRO *pHydro = (REACH_INFO_HYDRO*)pNode->m_reachInfo.pData;
   for (int k=0; k< m_numberPesticides; k++)
      {
      PESTICIDE_MASS_BALANCE_INFO *pMass = m_pesticideMassBalanceArray[k];
      INSTREAM_PESTICIDE_INFO *pPest = pHydro->instreamPesticideArray[k]; 
      pMass->cumOut = pPest->cumPesticideOutflow  ;
      }
   }
// This method accumulates volumeSat leaving the network
// The method assumes that quanties have already been accumulated as the model processed time.
//  this simply finds the last reach, and assumes that accumulated value represents the watershed.

void WET_Hydro::CalcCumulativeChannelVolume(float &cumOutflowVol, float &cumTracerOutflowMass, float &cumSedimentOutflowMass)
   {
   ReachNode *pNode = NULL;
   for ( int j=0; j < m_reachTree.m_nodeCount-1; j++ )        // so we must figure out what is upstream of it
      {
      pNode = m_reachTree.m_nodeArray[ j ];
      if ( pNode->m_pDown == m_reachTree.m_nodeArray[ m_reachTree.m_nodeCount-1 ] )        
         break;
      }

   REACH_INFO_HYDRO *pHydro = (REACH_INFO_HYDRO*)pNode->m_reachInfo.pData;
   cumOutflowVol = pHydro->cumOutFlowVol;  
   cumTracerOutflowMass = pHydro->cumTracerOutFlow;
   cumSedimentOutflowMass = pHydro->cumSedimentOutflow;
   }




//-- IntRKF ( ) -------------------------------------------------------
//
//-- Runge-Kutta-Fehlberg Method
//
//-- This is a fifth order adaptive method. It is best
//   to start with small timeStep and let the function find the right 
//   timeStep, as it proceeds it will come to the right step size.
//
//   Based on the six seperate calls to subroutine State, (RKF45) computes
//   an estimate for each state variable for one timeStep, an estimate of
//   the error for this step, and a new proposed size for the next step.
//
//   The step is usually specified with minimum and maximum values. (RKF45)
//   tries to use the largest stepsize, but the next proposed size may be
//   smaller in order to meet the minimum desired accuracy for the state
//   variables.
//
//   If the error is too large, the integration is repeated with a smaller
//   stepsize.
//---------------------------------------------------------------------

bool WET_Hydro::IntRKF( double curTime, int svCount, float **stateVar, bool modelToIntegrate )
   {

   double error;
   double sum;
   double delta    = 0;
   double timeStep = m_timeStep;

   //-- get derivative values from object at current time --//  

   GetDerivatives( m_derivative, curTime );
   for (int i=0; i < svCount; i++ )
      m_svTemp[i] = *(stateVar[i]);

   for ( i=0; i < svCount; i++ )
      {
      m_k1[ i ] = timeStep * m_derivative[i];
      *(stateVar[i]) = float( m_svTemp[i] + m_k1[i] / 4 );
      }  

   //-- get derivative values from object at current time + 1/4--//

   GetDerivatives( m_derivative, curTime+timeStep/4  );
   for (i=0; i < svCount; i++)
      {
      m_k2[i] = timeStep * m_derivative[i];
      *(stateVar[i]) = float( m_svTemp[i] + ( m_k1[i]*3 + m_k2[i]*9 ) / 32 );
      }
 
   //-- get derivative values from object at current time + -//
   GetDerivatives( m_derivative, curTime+timeStep*3/8 );
   for (i=0; i < svCount; i++)
      {
      m_k3[i] = timeStep * m_derivative[i];
      *(stateVar[i]) = float( m_svTemp[i]
                  + ( m_k1[i]*1932 - m_k2[i]*7200 + m_k3[i]*7296) / 2197 );
      }

   //-- get derivative values from object at current time + -//
   GetDerivatives( m_derivative, curTime+timeStep*12/13);
   for (i=0; i < svCount; i++)
      {
      m_k4[i] = timeStep * m_derivative[i];
      *(stateVar[i]) = float( m_svTemp[i] 
         - (m_k1[i]*439/216 - 8*m_k2[i] + m_k3[i]*3680/513 - m_k4[i]*845/4104) );
      }

   //-- get derivative values from object at current time + -//
   GetDerivatives( m_derivative, curTime+timeStep);
   for (i=0; i < svCount; i++)
      {
      m_k5[i] = timeStep * m_derivative[i];
      *(stateVar[i]) = float( m_svTemp[i] 
        - m_k1[i]*8/27 + 2*m_k2[i]+m_k3[i]*3544/2565-m_k4[i]*1859/4104-m_k5[i]*11/40 );

      }

   //-- get derivative values from object at current time + -//
   GetDerivatives( m_derivative, curTime+timeStep/2);
   for ( i=0; i< svCount; i++ )
      m_k6[i] = timeStep * m_derivative[i];


   //--------------- done with k values ------------//
   sum = 0.0f;
   error = 0.0f;
   float errorTe=0.0f;
   int maxError=-1;

   //-- all k values calculated, calculate 4th and 5th degree terms --//
   for ( i=0; i < svCount; i++ )
      {
      sum = fabs ( m_k1[i]/360 - m_k3[i]*128/4275 - m_k4[i]*2197/75204 +
                   m_k5[i]/50 + m_k6[i]*2/55) ;

      if (sum < 1E-30f)
         sum = 0.0f;

      errorTe=error;
      if ( *(stateVar)[ i ] != 0.0 )
         error = max( error, fabs( sum / *(stateVar)[ i ] ) );

      if (errorTe != error)
         maxError=i;
      }

   //-- update state variables --//
   for ( i=0; i < svCount; i++ )
      {
      *(stateVar[ i ]) = float( m_svTemp[ i ] + (float)( m_k1[i]*16/135 + m_k3[i]*6656/12825 + m_k4[i]* 
            28561/56430 - m_k5[i]*9/50 + m_k6[i]*2/55 ) );
      if (*stateVar[i] < 1E-30f )
         *stateVar[i] = 1E-30f;

      if (*stateVar[i] < 0.0f)
         *stateVar[i]= 0.0f;
      }
   if ( m_useVariableStep )
      {
      //-- increment time  --//
      curTime = (curTime+timeStep);

      if ( error == 0 )
         delta = 5;
      else
         delta = pow( m_rkvTolerance / ( 2*fabs( error ) ),  0.25 );  /// todo

      //-- don't change the timeStep dramatically because it makes a
      //-- difference by factor of 16

      //-- never increase delta more than 5 --//
      if ( delta > 5 )        // get time step multiplier value...
         delta = 5;
      else if ( delta < 0.1 )
         delta = 0.1;

      //-- calculate new timeStep --//  
      timeStep *= delta;

      //ASSERT( timeStep > 0 );

      //-- check the sign --//
      if (  timeStep > m_rkvMaxTimeStep )
        timeStep = m_rkvMaxTimeStep;

      else if ( timeStep < m_rkvMinTimeStep )
        timeStep = m_rkvMinTimeStep;

      //-- check singular differential equation case --//
      //ASSERT( fabs( timeStep ) >= m_rkvMinTimeStep );


      if ( ( timeStep > 0 ) && ((curTime+timeStep) > m_stopTime ) )         
         {
         //-- step just past end of simulation --//
   //      timeStep = m_stopTime - curTime;
           curTime = m_stopTime; 
         }

      ASSERT( timeStep > 0.0f );
   
      m_timeStep =  timeStep;
      }  // end of: if ( m_variableStep == true )


  return TRUE;
  }


  void WET_Hydro::UpdateAuxillaryVars()
   {
	int counter=0;
   int reachCount = GetReachCount();
   for (int i=0; i < reachCount; i++ )
		{
      float accum=0.0f;
      float accumTracer=0.0f;
      float *accumPesticide = new float[m_numberPesticides];
      for (int j=0;j<m_numberPesticides;j++)
         accumPesticide[j] = 0.0f;

      REACH_INFO_HYDRO *pHydro = m_reachInfoHydroArray[ i ];
      int size = pHydro->upslopeInfoArray.GetSize()-1;
      for ( int j=pHydro->upslopeInfoArray.GetSize()-1;j >= 0; j--)
         {       
         UPSLOPE_INFO *pUpslope = pHydro->upslopeInfoArray[ j ];
         if (pUpslope->area == 1.0f)
            CalcAccountForZeroArea(pUpslope); 
         else
            {
            CalcPondedVolume(pUpslope);
            CalcSatDeficit(pUpslope);
            CalcKSat(pUpslope);
            CalcSatZone(pUpslope);     
            if ( m_modelStructureMethod==WH_DISTRIBUTE_WTHEIGHT || m_modelStructureMethod==WH_DISTRIBUTE_FPGRID)
               {
          //   if (pUpslope->isStream)
          //        {
                  accumTracer+=pUpslope->tracerOutflowMassSurface;
                  accum += pUpslope->surfaceOutflowVol;

                  if (m_checkPesticide)
                     {
                     for (int k=0;k<m_numberPesticides;k++)
                        {
                        accumPesticide[k] += (pUpslope->pesticideArray[k]->pSatWater->concentration/1E6f*pUpslope->surfaceOutflowVol);
                        }
               //      } 
                  }
               //   pUpslope->surfaceOutflowVol=0.0f;
               //   pUpslope->pondedVolume=0.0f;
               }
            if (m_checkUnsaturated)
               {
               CalcUnsatZone(pUpslope);
               CalcStorage(pUpslope); 
               }
            }
         if (m_checkPesticide)
            {
            UpdatePesticideConcentration(pUpslope);
            UpdatePesticideMassBalance(pUpslope);
            UpdatePesticideWaterVolume(pUpslope);
            }
         if (m_checkTracerModel)
            {
            if (m_time<m_startTime+30.0f)
               pUpslope->tracerMass=0.0f;

            if (m_time>=(m_startTime+30.0f) && m_time<(m_startTime+30.0f+m_timeStep))
               {
              
               pUpslope->tracerMass=(m_editTracerMgL/1000.0f)*pUpslope->volumeSat;
               if (m_checkUnsaturated)
                  pUpslope->pUnsat->tracerMass = (m_editTracerMgL/1000.0f)*pUpslope->pUnsat->volumeUnsat;
               }                       
            if (pUpslope->volumeSat > 0.000010f)
               pUpslope->tracerConc=pUpslope->tracerMass/pUpslope->volumeSat; // in kg/m3
            else
               pUpslope->tracerConc=0.000001f;
            if (m_checkUnsaturated)
               {
               if (pUpslope->pUnsat->volumeUnsat >= 0.0f)
                  pUpslope->pUnsat->tracerConc = pUpslope->pUnsat->tracerMass/pUpslope->pUnsat->volumeUnsat;
               else
                  pUpslope->pUnsat->tracerConc=0.0f;
               }
            }
         counter++;
      }//end of upslopes

      if (m_modelStructureMethod==WH_DISTRIBUTE_FPGRID || m_modelStructureMethod==WH_DISTRIBUTE_WTHEIGHT)
         {
         pHydro->upslopeInfoArray[0]->surfaceOutflowVol=accum;
         accum=0.0f;
         pHydro->upslopeInfoArray[0]->tracerOutflowMassSurface=accumTracer;
         accumTracer=0.0f;
         
         if (m_checkPesticide)
            { 
            for (int k=0;k<m_numberPesticides;k++)
               pHydro->upslopeInfoArray[0]->pesticideArray[k]->pSurfWater->contributeToStream=accumPesticide[k]; //kg/d
            }

         }

      }   //  end of reaches

      // go through one more time to identify and change the very last reach.  without this, it has no where to drain and piles up.
         ReachNode *pNode = NULL;
            for ( int j=0; j < m_reachTree.m_nodeCount-1; j++ )        // so we must figure out what is upstream of it
               {
               pNode = m_reachTree.m_nodeArray[ j ];
               if ( pNode->m_pDown == m_reachTree.m_nodeArray[ m_reachTree.m_nodeCount-1 ] )        
                  break;
               }
            REACH_INFO_HYDRO *pHydro = (REACH_INFO_HYDRO*)pNode->m_reachInfo.pData;
            UPSLOPE_INFO *pLast = pHydro->upslopeInfoArray[0];
            pHydro->upslopeInfoArray[0]->lateralOutflowVol=pHydro->upslopeInfoArray[0]->lateralInflow;
            pHydro->upslopeInfoArray[0]->tracerOutflowMassSubsurface=pHydro->upslopeInfoArray[0]->tracerInflowMassSubsurface;
            if (m_checkPesticide)
               { 
               for (int k=0;k<m_numberPesticides;k++)
                  pHydro->upslopeInfoArray[0]->pesticideArray[k]->pSatWater->rateMassOutflow=pHydro->upslopeInfoArray[0]->pesticideArray[k]->pSatWater->rateMassInflow;
               }

      
}

void WET_Hydro::ApplyTracerToGridCell(int row, int col)
   {
   if (m_checkTracerModel)
      {
      UPSLOPE_INFO *pUpslope = GetUpslopeInfoFromRowCol(row,col);
      pUpslope->tracerMass=m_editTracerMgL/1000.0f*pUpslope->volumeSat*150.0f;//should be about equal to the correct concentration
      }
   }

UPSLOPE_INFO *WET_Hydro::GetUpslopeInfoFromRowCol(int row,int col)
   {
   int reachCount = GetReachCount();

   for (int i=0; i < reachCount; i++ )
      {     
      REACH_INFO       *pInfo  = &m_reachTree.m_nodeArray[ i ]->m_reachInfo;
      REACH_INFO_HYDRO *pHydro = m_reachInfoHydroArray[ i ];
      for (int j=0;j<pHydro->upslopeInfoArray.GetSize(); j++) 
	      {
		   UPSLOPE_INFO *pUpslope = pHydro->upslopeInfoArray[ j ]; 
         if (pUpslope->rowIndex==row && pUpslope->colIndex==col)
            return pUpslope;
         }
      }
   }

void WET_Hydro::UpdatePesticideMassBalance(UPSLOPE_INFO *pUpslope)
   {
   for (int i=0; i<m_numberPesticides;i++)
      {
      PESTICIDE_INFO *pPest=pUpslope->pesticideArray[i];

      pPest->pVeg->cumMassInput+=pPest->pVeg->rateMassApp*(float)m_timeStep;
      pPest->pVeg->cumTransformation+=pPest->pVeg->rateTransformation*(float)m_timeStep;
      pPest->pSurfWater->cumMassApp+=pPest->pSurfWater->rateMassApp*(float)m_timeStep; //rateMassApp is in kg/day..
      pPest->pSurfWater->cumTransformation+=pPest->pSurfWater->rateTransformation*(float)m_timeStep;
      pPest->pSurfWater->cumUptake+=pPest->pSurfWater->rateUptake*(float)m_timeStep; 
      pPest->pSurfWater->cumRunoff+= pPest->pSurfWater->rateRunoff*(float)m_timeStep;
      pPest->pSurfWater->cumErosionLoss+=pPest->pSurfWater->rateErosionLoss*(float)m_timeStep;
      
      pPest->pUnsatWater->cumTransformation+=pPest->pUnsatWater->rateTransformation*(float)m_timeStep;
      pPest->pUnsatWater->cumUptake+=pPest->pUnsatWater->rateUptake*(float)m_timeStep; 
//      ASSERT(pPest->pUnsatWater->rateUptake==0.0f);
      pPest->pSatWater->cumTransformation+=pPest->pSatWater->rateTransformation*(float)m_timeStep;

   
  
      }
   }

void WET_Hydro::UpdatePesticideConcentration(UPSLOPE_INFO *pUpslope)
   {
   for (int i=0;i<m_numberPesticides;i++)
      {
      float mass = pUpslope->pesticideArray[i]->pSurfWater->mass ;
  
      float volume = pUpslope->pesticideArray[i]->pSurfWater->volumeWater;
      float conc = mass/volume * 1E6f;
      
      pUpslope->pesticideArray[i]->pSurfWater->concentration =conc;

      mass = pUpslope->pesticideArray[i]->pUnsatWater->mass;
    
      volume = pUpslope->pesticideArray[i]->pUnsatWater->volumeWater;
      conc = mass/volume * 1E6f;
      pUpslope->pesticideArray[i]->pUnsatWater->concentration = conc;

      mass = pUpslope->pesticideArray[i]->pSatWater->mass;
  
      volume = pUpslope->pesticideArray[i]->pSatWater->volumeWater;
      conc = mass/volume * 1E6f;
      pUpslope->pesticideArray[i]->pSatWater->concentration = conc;

      }
   }


void WET_Hydro::CalcPondedVolume(UPSLOPE_INFO *pUpslope)
   {
   pUpslope->pondedVolume=0.0f;
   int sat=0;
   float totalVolume = pUpslope->volumeSat;
   if (totalVolume >= pUpslope->voidVolume*0.95f)//within 95% of saturation
	   {
      pUpslope->pondedVolume = totalVolume - (pUpslope->voidVolume*0.95f);
      if (m_checkTracerModel)
         {
         float pondedMass=pUpslope->tracerConc*pUpslope->pondedVolume;
         pUpslope->tracerMass = pUpslope->tracerMass-pondedMass;
         }
      if (m_checkPesticide)
         {
         for (int k=0;k<m_numberPesticides;k++)
            {
            float pondedMass=pUpslope->pesticideArray[k]->pSurfWater->concentration/1E6f*pUpslope->pondedVolume;
            pUpslope->pesticideArray[k]->pSurfWater->mass = pUpslope->pesticideArray[k]->pSurfWater->mass-pondedMass;
            }
         }
      pUpslope->volumeSat = pUpslope->voidVolume*0.95f;

	  if (pUpslope->pUnsat!=NULL)
		  if (pUpslope->pUnsat->volumeUnsat > pUpslope->voidVolume*0.05f)
			{
            pUpslope->pondedVolume+=(pUpslope->pUnsat->volumeUnsat - (pUpslope->voidVolume*0.05f));
			pUpslope->pUnsat->volumeUnsat = pUpslope->voidVolume*0.05f;
			}
				
      }
   if (sat==1)
      m_massBalanceInfo.freqSat+=(float)m_timeStep;

   }


void WET_Hydro::CalcSatZone(UPSLOPE_INFO *pUpslope)
   {
   float saturatedDepth=0.0f;
   if (m_radioPorosity==DRAIN_P_CONSTANT)
       saturatedDepth = pUpslope->volumeSat / (pUpslope->area * pUpslope->pHydroParam->phi );
   
   else if (m_radioPorosity==DRAIN_P_EXPONENTIAL)
      {
         m_drainShape = pUpslope->pHydroParam->vanGenN;
      float length = pUpslope->volumeSat/pUpslope->area;
      float num = m_drainShape*pUpslope->pHydroParam->phi;
      
      saturatedDepth = -m_drainShape*log((length-num)/-num);
      }

      
  // current depth - previous depth - a positive value is a rise, negative is a fall

   pUpslope->groundWaterChange = saturatedDepth - pUpslope->saturatedDepth ; 
   pUpslope->saturatedDepth = saturatedDepth;

   if (pUpslope->saturatedDepth > 0.0f)
      pUpslope->swc = pUpslope->volumeSat / (pUpslope->area * pUpslope->saturatedDepth);//better equal phi
   else 
      pUpslope->swc = pUpslope->pHydroParam->phi;
         //  saturated depth is a depth of water+ sediment   
   if (m_satMethod==0)
      {
      float m=pUpslope->pHydroParam->m;
      float ssfmmd = pUpslope->pHydroParam->kSat*1000.0f*pUpslope->slope*(float)exp( -pUpslope->sdmm / pUpslope->pHydroParam->m);
	   pUpslope->ssfVol = (ssfmmd / 1000.0f) * pUpslope->area;//note these are volumetric RATES   
      }
   if (m_satMethod==2)//power law
      {
      float ssfmmd = pUpslope->pHydroParam->kSat*1000.0f*pUpslope->slope*(float)exp( -pUpslope->sdmm / pUpslope->pHydroParam->m);
	   pUpslope->ssfVol = (ssfmmd / 1000.0f) * pUpslope->area;//note these are volumetric RATES   
      float hOut = (pUpslope->pHydroParam->soilDepth-pUpslope->saturatedDepth)/pUpslope->pHydroParam->soilDepth;
      float iOut = 1-hOut; 
      float n = pUpslope->pHydroParam->powerLExp;      
      float T = ((pUpslope->pHydroParam->kSat*pUpslope->pHydroParam->soilDepth)/n)*pow(iOut,n);
      pUpslope->ssfVol = T*pUpslope->slope*(float)sqrt(pUpslope->area);       
      }

   pUpslope->lateralOutflowVol = pUpslope->ssfVol /*+ pUpslope->tileDrainageVol*/;
   pUpslope->surfaceOutflowVol = pUpslope->pondedVolume/(float)m_curTimeStep;//note these are volumetric RATES
   pUpslope->pondedVolume=0.0f;
      //  to convert from pondedVolume (which is NOT a rate) to a rate we assume it all runs off
      //  during the timeStep, ie rate=pv/timestep.  This converts it to m3/day.
      

   if (m_modelStructureMethod==WH_DISTRIBUTE_WTHEIGHT)
         {
         UpdateGridSlope(pUpslope);
         }
   if (m_modelStructureMethod==WH_DISTRIBUTE_FPGRID)
      CalcD8Flux(pUpslope);
   

   }

void WET_Hydro::UpdateGridSlope(UPSLOPE_INFO *pUpslope)
   {
 //        FILE *fp1 = fopen("c:\\temp\\crap4.csv", "a" );  
   float myElev = pUpslope->elev -( pUpslope->pHydroParam->soilDepth - pUpslope->saturatedDepth);
   // iterate through neighbors to calc. the sum of the slope
   float lamdaSum=0.0f;
   float w = m_gridResolution; // in some cases 14.4 (diagonal)
   float n = pUpslope->pHydroParam->powerLExp;
   pUpslope->lateralInflow=0.0f;
   pUpslope->lateralOutflowVol=0.0f;
   float outflow=0.0f;
   float inflow=0.0f;
   float tracerInflow=0.0f;
   float tracerOutflow=0.0f; 

   float *pestIn = new float[m_numberPesticides];
   float *pestOut = new float[m_numberPesticides];
   if (m_checkPesticide)
      {
      for (int i=0; i<m_numberPesticides;i++)
         {
         pestIn[i]=0.0f;
         pestOut[i]=0.0f;
         }
      }

   int countDown=0;
//   ASSERT(pUpslope->neighborArray.GetSize()>0);
   int downCells=0;
   int upCells=0;
   int flatCells=0;
	for (int upCheck=0;upCheck<pUpslope->neighborArray.GetSize(); upCheck++) 
		{    
		UPSLOPE_INFO *pUpCheck = pUpslope->neighborArray[ upCheck ];
      if (pUpCheck->rowIndex==pUpslope->rowIndex || pUpCheck->colIndex==pUpslope->colIndex)
         w=m_gridResolution;
      else
         w=(float)sqrt((m_gridResolution*m_gridResolution)+(m_gridResolution*m_gridResolution));
      float otherElev = pUpCheck->elev -( pUpCheck->pHydroParam->soilDepth - pUpCheck->saturatedDepth);


      if ( myElev > otherElev ) // calculate my outputs
         {
         float hOut = (pUpslope->pHydroParam->soilDepth-pUpslope->saturatedDepth)/pUpslope->pHydroParam->soilDepth;
         float iOut = 1-hOut; 
         float slopeAll=0.0f;
         float slope=(myElev - otherElev) / w;
         countDown++;
         float T = ((pUpslope->pHydroParam->kSat*pUpslope->pHydroParam->soilDepth)/n)*pow(iOut,n);
         float out = -T*slope*w;
         outflow += T*slope*w;  

  
      //   fprintf(fp1,"%f, %f,  %i,%i, %i,%i \n", out, outflow, pUpslope->colIndex, pUpslope->rowIndex, pUpCheck->colIndex,pUpCheck->rowIndex ); // and write them to fp


         tracerOutflow += pUpslope->tracerConc*(T*slope*w);
         if (m_checkPesticide)
            {
            for (int i=0; i<m_numberPesticides; i++)   
               pestOut[i] += (pUpslope->pesticideArray[i]->pSatWater->concentration/1E6f) * (T*slope*w);
            }         
         downCells++;
         ASSERT(downCells>0);
         }
      if (myElev < otherElev) //calculate my inputs
         {
         float hIn = (pUpCheck->pHydroParam->soilDepth-pUpCheck->saturatedDepth)/pUpCheck->pHydroParam->soilDepth;
         float iIn = 1-hIn;   
         float slope=(otherElev - myElev) / w;
         float T = ((pUpCheck->pHydroParam->kSat*pUpCheck->pHydroParam->soilDepth)/n)*pow(iIn,n);
         float in = T*slope*w;
         inflow += in;  
 
    //     fprintf(fp1,"%f, %f,  %i,%i, %i,%i \n", in, inflow, pUpslope->colIndex, pUpslope->rowIndex, pUpCheck->colIndex,pUpCheck->rowIndex ); // and write them to fp

         tracerInflow += pUpCheck->tracerConc*(T*slope*w);
         if (m_checkPesticide)
            {
            for (int i=0; i<m_numberPesticides; i++)
               pestIn[i] += (pUpCheck->pesticideArray[i]->pSatWater->concentration/1E6f) * (T*slope*w);      
            }         
         upCells++;
         }   
      }    
   


      pUpslope->lateralInflow=inflow;
      inflow=0.0f;
      pUpslope->lateralOutflowVol=outflow;
      outflow=0.0f;
      pUpslope->tracerInflowMassSubsurface =  tracerInflow;
      pUpslope->tracerOutflowMassSubsurface = tracerOutflow;
      if (m_checkPesticide)
         {
         for (int i=0; i<m_numberPesticides; i++)
            {    
            pUpslope->pesticideArray[i]->pSatWater->rateMassInflow = pestIn[i];
            pUpslope->pesticideArray[i]->pSatWater->rateMassOutflow = pestOut[i];
            
            }
         }     
      
      ASSERT(inflow >= 0);
      ASSERT(outflow >= 0);
   // if (countDown==0)
      //  pUpslope->lateralOutflowVol=pUpslope->lateralInflow;
   delete pestIn;
   delete pestOut;
   
 //  fclose(fp1);
   }

void WET_Hydro::CalcD8Flux(UPSLOPE_INFO *pUpslope)
   {
   float myElev = pUpslope->elev ;
   // iterate through neighbors to calc. the sum of the slope
   float lamdaSum=0.0f;
   float w = m_gridResolution; // in some cases 14.4 (diagonal)
   float n = pUpslope->pHydroParam->powerLExp;
   pUpslope->lateralInflow=0.0f;
   pUpslope->lateralOutflowVol=0.0f;
   float outflow=0.0f;
   float inflow=0.0f;
   float tracerInflow=0.0f;
   float tracerOutflow=0.0f;
   //if (m_checkPesticide)
      // {
      float *pestIn = new float[pUpslope->pesticideArray.GetSize()];
      float *pestOut = new float[pUpslope->pesticideArray.GetSize()];
      //  }
   if (m_checkPesticide)
      {
      for (int i=0; i<m_numberPesticides;i++)
         {
         pestIn[i]=0.0f;
         pestOut[i]=0.0f;
         }
      }
   int countDown=0;
//   ASSERT(pUpslope->neighborArray.GetSize()>0);
   int downCells=0;
   int upCells=0;
   int flatCells=0;
   float maxDrop=1E6;
	for (int upCheck=0;upCheck<pUpslope->neighborArray.GetSize(); upCheck++) 
		{
         
		UPSLOPE_INFO *pUpCheck = pUpslope->neighborArray[ upCheck ];

      if (pUpCheck->rowIndex==pUpslope->rowIndex || pUpCheck->colIndex==pUpslope->colIndex)
         w=m_gridResolution;
      else
         w=(float)sqrt((m_gridResolution*m_gridResolution)+(m_gridResolution*m_gridResolution));
      float otherElev = pUpCheck->elev ;
      float hIn = (pUpslope->pHydroParam->soilDepth-pUpCheck->saturatedDepth)/pUpslope->pHydroParam->soilDepth;
      float iIn = 1-hIn;   
      float hOut = (pUpslope->pHydroParam->soilDepth-pUpslope->saturatedDepth)/pUpslope->pHydroParam->soilDepth;
      float iOut = 1-hOut; 
      float slopeAll=0.0f;

      if ( myElev > otherElev && otherElev < maxDrop) // calculate my outputs - only output to 1 cell
         {
         maxDrop=otherElev;
         float slope=(myElev - otherElev) / w;
         countDown++;
         float T = ((pUpslope->pHydroParam->kSat*pUpslope->pHydroParam->soilDepth)/n)*pow(iOut,n);

         outflow = (T*slope*w);  
         tracerOutflow = pUpslope->tracerConc*(T*slope*w);
         if (m_checkPesticide)
            {
            for (int i=0; i<m_numberPesticides; i++)
               {    
               pestOut[i] += (pUpslope->pesticideArray[i]->pSatWater->concentration/1E6f) * (T*slope*w);
               }
            }
            
         downCells++;
         }
      if (myElev < otherElev) //calculate my inputs
         {
         float slope=(otherElev - myElev) / w;
         float T = ((pUpCheck->pHydroParam->kSat*pUpCheck->pHydroParam->soilDepth)/n)*pow(iIn,n);
         inflow += (T*slope*w);  
         tracerInflow += pUpCheck->tracerConc*(T*slope*w);
         if (m_checkPesticide)
            {
            for (int i=0; i<m_numberPesticides; i++)
               {    
               pestIn[i] += (pUpCheck->pesticideArray[i]->pSatWater->concentration/1E6f) * (T*slope*w);
               }
            }

            
         upCells++;
         }
      if (myElev==otherElev)
         {
         flatCells++;
         }
      }
      
      pUpslope->lateralInflow=inflow;
      pUpslope->lateralOutflowVol=outflow;
      pUpslope->tracerInflowMassSubsurface =  tracerInflow;
      pUpslope->tracerOutflowMassSubsurface = tracerOutflow;
      if (m_checkPesticide)
         {
         for (int i=0; i<m_numberPesticides; i++)
            {    
            pUpslope->pesticideArray[i]->pSatWater->rateMassInflow = pestIn[i];
            pUpslope->pesticideArray[i]->pSatWater->rateMassOutflow = pestOut[i];
            }
         }
      
      
      ASSERT(inflow >= 0);
      ASSERT(outflow >= 0);

   delete pestIn;
   delete pestOut;
   
   }
void WET_Hydro::CalcUnsatZone(UPSLOPE_INFO *pUpslope)
   {
   if (pUpslope->isSaturated == 0) // unsaturated
      {
      pUpslope->pUnsat->unsaturatedDepth = pUpslope->pHydroParam->soilDepth - pUpslope->saturatedDepth;

      pUpslope->pUnsat->swcUnsat = pUpslope->pUnsat->volumeUnsat / (pUpslope->area*pUpslope->pUnsat->unsaturatedDepth);  
     // ASSERT(pUpslope->swcUnsat < 1.0f);
     // CalcVanGenuctenKTheta(pUpslope);
      CalcBrooksCoreyKTheta(pUpslope);
      }
   else
      {
      pUpslope->pUnsat->unsaturatedDepth = pUpslope->pHydroParam->soilDepth - pUpslope->saturatedDepth;;
      pUpslope->pUnsat->swcUnsat = pUpslope->pHydroParam->phi;  
      pUpslope->pUnsat->swcUnsat = pUpslope->pUnsat->volumeUnsat / (pUpslope->area*pUpslope->pUnsat->unsaturatedDepth);  
     // pUpslope->pUnsat->volumeUnsat = 1.0f;
      //pUpslope->pUnsat->usfVol=0.0f;

      CalcBrooksCoreyKTheta(pUpslope);
     
      }

   }
void WET_Hydro::CalcKSat(UPSLOPE_INFO *pUpslope)
   {
	float kSat=0.0f;
   if ( m_ksAsParam )
      {
         kSat   = pUpslope->pHydroParam->kSat ;
      if (m_checkSimulateWetlands)
         kSat = kSat*0.1f;
      pUpslope->pHydroParam->kSat = kSat;
      }   

	else
   kSat = pUpslope->pHydroParam->kSat ;
  //       int m = 4;
  //       if (!pUpslope->pUpslope || !pUpslope->pUpslope->pUpslope || !pUpslope->pUpslope->pUpslope->pUpslope)
  //          m=12;

   }


void WET_Hydro::CalcStorage(UPSLOPE_INFO *pUpslope)
   {
  //  calculates the actual length of water that exchanges between sat/unsat based on the change in water table.

//  pUpslope->groundWaterChange is the total distance over which the water table moved between this and the previous time.
   if (pUpslope->groundWaterChange > 0.0f) // a wetting  condition - we must convert Unsaturated water directly to sat
      pUpslope->deltaSat = pUpslope->groundWaterChange*pUpslope->pUnsat->swcUnsat;
      
   else // drying condition, where previously saturated area is now unsaturated..
      pUpslope->deltaSat = pUpslope->groundWaterChange*pUpslope->swc;
   
   pUpslope->deltaUnsat = pUpslope->deltaSat * -1.0f;

   }

void WET_Hydro::CalcVoidVolume(UPSLOPE_INFO *pUpslope)
   {
   if (m_radioPorosity==DRAIN_P_CONSTANT)
      pUpslope->voidVolume = pUpslope->pHydroParam->phi*pUpslope->pHydroParam->soilDepth*pUpslope->area;
   else if (m_radioPorosity==DRAIN_P_EXPONENTIAL)
   //float b = 10.0f;
   pUpslope->voidVolume = ((-m_drainShape*pUpslope->pHydroParam->phi*exp(-pUpslope->pHydroParam->soilDepth/m_drainShape)) + m_drainShape*pUpslope->pHydroParam->phi)*pUpslope->area;

   }

void  WET_Hydro::CalcVanGenuctenKTheta(UPSLOPE_INFO *pUpslope)
   {//  if the current unsaturated water content is greater than the wilting point, calculate a rate of loss
   if (pUpslope->pUnsat->swcUnsat > pUpslope->pHydroParam->wiltPt)
      {
      float ratio = 1.0f; // SWCUNSAT shouldn't be bigger than swcSat, but sometimes is very slightly do to large time steps
                          //  This check keeps us from sqrt(negative)...
      if (pUpslope->pUnsat->swcUnsat < pUpslope->swc)
         ratio = (pUpslope->pUnsat->swcUnsat - pUpslope->pHydroParam->wiltPt)/(pUpslope->pHydroParam->phi-pUpslope->pHydroParam->wiltPt);
      float left = pUpslope->pHydroParam->kSat*sqrt(ratio);
      float m = 1-(1/pUpslope->pHydroParam->vanGenN);
      float ratioRaised = pow(ratio,(1/m));
      float next = pow(1-ratioRaised,m);
      float right = (1-next)*(1-next);
      float kTheta =  left * right;
      pUpslope->pUnsat->usfVol = kTheta * pUpslope->area;
      }
   else  //  but if it is not, assume no vertical flow of water.
      pUpslope->pUnsat->usfVol = 0.0f;
   ASSERT(pUpslope->pUnsat->usfVol >= 0.0f);
   }
void  WET_Hydro::CalcBrooksCoreyKTheta(UPSLOPE_INFO *pUpslope)
   {//  if the current unsaturated water content is greater than the wilting point, calculate a rate of loss
   if (pUpslope->pUnsat->swcUnsat > pUpslope->pHydroParam->wiltPt)
      {
      float ratio = 1.0f; // SWCUNSAT shouldn't be bigger than swcSat, but sometimes is very slightly do to large time steps
                          //  This check keeps us from sqrt(negative)...
      if (pUpslope->pUnsat->swcUnsat < pUpslope->swc)
         ratio = (pUpslope->pUnsat->swcUnsat - pUpslope->pHydroParam->wiltPt)/(pUpslope->pHydroParam->phi-pUpslope->pHydroParam->wiltPt);
      float power = 3.0f+(2.0f/pUpslope->pHydroParam->vanGenN);
      float kTheta = pUpslope->pHydroParam->kSat * pow(ratio,power);

      pUpslope->pUnsat->usfVol = kTheta * pUpslope->area;
      }
   else  //  but if it is not, assume no vertical flow of water.
      pUpslope->pUnsat->usfVol = 0.0f;     

   ASSERT(pUpslope->pUnsat->usfVol >= 0.0f);
   }



void WET_Hydro::CalcSatDeficit(UPSLOPE_INFO *pUpslope)
   {

     //float saturatedDepth = pUpslope->volumeSat / (pUpslope->area * pUpslope->pHydroParam->phi);
      pUpslope->sdmm = (pUpslope->pHydroParam->soilDepth -pUpslope->saturatedDepth)*1000.0f;
   }
void WET_Hydro::CalcAccountForZeroArea(UPSLOPE_INFO *pUpslope)
   {

   if (pUpslope->area == 1.0f)
      {
      if (pUpslope->pUpslope) // if there is something upslope, just pass those values through
         {
         pUpslope->lateralOutflowVol = pUpslope->pUpslope->lateralOutflowVol;
         pUpslope->ssfVol = pUpslope->pUpslope->ssfVol;
         if (m_checkUnsaturated)

            {
            pUpslope->pUnsat->usfVol = pUpslope->pUpslope->pUnsat->usfVol;
            pUpslope->pUnsat->swcUnsat = pUpslope->pUpslope->pUnsat->swcUnsat;
            }
         pUpslope->swc = pUpslope->pUpslope->swc;
         
         pUpslope->surfaceOutflowVol = pUpslope->pUpslope->surfaceOutflowVol;
         pUpslope->pondedVolume = pUpslope->pUpslope->pondedVolume;
         pUpslope->deltaSat=pUpslope->pUpslope->deltaSat;
         pUpslope->deltaUnsat = pUpslope->pUpslope->deltaUnsat;
         pUpslope->tracerOutflowMassSubsurface = pUpslope->pUpslope->tracerOutflowMassSubsurface;
         pUpslope->tracerOutflowMassSurface = pUpslope->pUpslope->tracerOutflowMassSurface;
//         pUpslope->tracerOutflowMassPrevious = pUpslope->pUpslope->tracerOutflowMassPrevious;
         }
      else //if I am the highest pUpslope, set my rates to 0
         {
         pUpslope->lateralOutflowVol=0.0f;
         pUpslope->surfaceOutflowVol=0.0f;
         pUpslope->ssfVol = 1.0f;
         if (m_checkUnsaturated)
            {
            pUpslope->pUnsat->usfVol = 1.0f;
            pUpslope->pUnsat->swcUnsat = 0.1f;
            }
         pUpslope->swc = 0.1f;
         
         pUpslope->pondedVolume = 0.0f;
         pUpslope->deltaSat=0.0f;
         pUpslope->deltaUnsat = 0.0f;
         pUpslope->tracerOutflowMassSubsurface = 0.0f;
         pUpslope->tracerOutflowMassSurface = 0.0f;
        // pUpslope->tracerOutflowMassPrevious = 0.0f;
         }         
      }
   }

float WET_Hydro::PenmanMonteith(float temp)
   {
      
   float lamda = 2.501f - (0.00236f * 10.0f); // Latent heat of evaporation (MJ/kg) NOTE we assume surface water temp = 10 C!
   
   
   // Estimate the NET Incoming short wave radiation
   // The total radiation is essentially an atmospheric property, and is reduced by cloud cover 
   // and by reflection of radiation by surface roughness (albedo)

   float so = 118.104f; // MJ/day, extraterrestrial radiation
   float as = 0.25f; // fracton of extraterrestrial radiation S0 on overcast days (set n=0)
   float bs = 0.50f; //fraction of extraterrestrial radiation S0 on clear days
   float n = 10.0f;  // number of bright sunshine hours per day
   float N = 12.0f;  // number of hours of daylight
   float st =  (as + bs * (n/N)) * so; // Total incoming Short Wave Radiation
   
   float alpha = 0.23f; // Albedo - the short wave reflection coefficient that IS SENSITIVE TO LANDUSE
   float sn = st * (1.0f - alpha); // where alpha is the albedo, equal to .23 NET SHORT WAVE RADIATION


   // Estimate the long wave radiation
   // the exchange of long wave radiation between vegetation / soil and atmosphere/clouds is represented
   // by the ln equation below.
   
   float ac = 0.34f;  //correlation coefficient ranges from 0.34 to 0.44
   float bc = -0.14f; //correlation coefficient ranges from -0.14 to -0.25
   float sto = (as + bs) * so; //solar radiation for clear skies (st with n/N = 1)
   float f = (ac * (st / sto)) + bc;  // cloudiness factor
   float epsilon = (float)pow((-0.02f + 0.261f), (-0.000777f * temp * temp));  //net emmissivity between atmosphere and ground
   float ph = 0.000000004903f; //  Stefan - Boltzmann Constant MJ/m2 K-4 day
   float ln = -f * epsilon * ph * (temp + 273.2f); //exchange of long wave radiation between ground and air
 
   
   //  Calculate the net Radiation at the ground surface.
   //  The difference between incoming and relected solar radiation plus the difference between the incoming
   //  long wave radiation and outgoing long wave radiation

   float Rn = sn + ln;  // Net radiation at the ground surface

   // Next, an energy budget for a unit area including soils, veg and a small portion of atmosphere
  
   
   float G = 0.0f;  // Outgoing heat conduction to the soil
   float S = 0.0f;  // Energy stored within the volumeSat 
   float P = 0.02f * Rn; // energy absorbed by biochemical processes in plants
   float A = Rn - G - S - P; //Available energy, or energy available for partioning into latent or sensible heat
  
   float rhoa = 1.2f; // density of air at about 20 degrees C (kg/m3)
   float cp = 1.013f; // specific heat of moist air ( kJ/kg degree C)

   // The model relies on the vapor pressure deficit in kPa at some height above the canopy
   
   float es = 3.169f;    //saturated vapor pressure
   float grad = 0.189f;  // gradient of saturated vapor pressure
   float gamma = 0.067f; //psychrometric constant
   float RH = 0.5f;   //Relative Humidity
   float ea = RH * es; // RH = relative humidity
   float D = es - ea;  //  vapor pressure deficit (Kpa)

   //  Next, calculate the aerodynamic resistance, which is inversely proportional to the wind speed and
   //  changes with the gigh of the vegetation coverin the ground.
   //  This model provides an additional resistance term (atmospheric) which is in addition to stomatal
   //  or surface resistance.
   
   float hc = 0.5f;  // mean height of crop
   float zom = 0.123f * hc; //  
   float zov = 0.0123f * hc; //
   float d = 0.67f * hc;  // 

   float zu = 2.0f; // height at which wind speed is measured
   float ze = 2.0f; // height at which humidity is measured
   float Uz = 1.0f; // wind speed = 1 m/s

   float ra = (float)log((zu - d) / zom ) * (float)log((ze - d) / zov) / (0.41f * 0.41f) * Uz;  //aerodyamic resistance is inversley proportional to wind speed and changes with the height of veg covering ground
  
   //  The last term is the stomatal resistance to evaporation.  Currently it is only depenedent
   //  on the height of the crop.  This may change.
   
   float L = 24 * hc;  // leaf area index for the crop - note that it's dependent of the height
   float rs = 200 / L;  // surface resistance of the land cover


   //  Evaporation estimated from a combination of the above terms.

   float evap = (1 / lamda) * ((grad * A) + ((rhoa * cp * D) / ra)) / (grad + (gamma * (1 + (rs / ra))));
      // and this is evaportation in mm/day given the parameters
   return evap;
   }

int WET_Hydro::GetDateJulFromTime(float time)
   {
   COleDateTime t( time );
   int julDate = t.GetDayOfYear();
   return julDate;
   }


float WET_Hydro::CalcET( float precip, float temp ) //hargreaves equation
   {
   float phi = m_latitude; // latitude
   int julDate = GetDateJulFromTime((float)m_time);

   float gamma = 0.4039f * (float) sin((2*3.1415 / 365 * julDate) - 1.405); 
   //solar declination (radians)
   float ws = (float) acos (-tan(phi) * tan(gamma)); //sunset hour angle (radians)
   
   float dr = 1 + 0.033f * (float) cos((2*3.1415/365 * julDate) );
            //relative distance betweem earth and sun

   float s0 = 15.392f * dr * (float) ( ws * sin(phi) * sin(gamma) + cos(phi) * cos(gamma) * sin(ws));
    //s0 = extraterrestrial solar radiation  mm/day

   float deltaT = 2;  //difference between mean monthly min and max temperatures - an estimate!

   float etrc = 0.0023f * s0 * deltaT * ( temp + 17.8f ) / 1000; //gives reference crop et in m / d
   return etrc;

   }

//--------------------------------------------------------------------------------------
// WET_Hydro::CalcHoltanF -- Holtan method for Infiltration
//
// The Holtan method is purely empirical and does NOT account for soil moisture change and its
// effect on infiltration capacity.  It is crop and soil dependant and I need to add tables 
// of values for these different dependancies.
//
// Awa is the available water capacity in the first horizon.  It is provided in ssurgo soils
//
//--------------------------------------------------------------------------------------

float WET_Hydro::CalcHoltanF(  float awa  )
   {
   
   float gi = 0.5f;  // growth index of the crop range form .1 to 1.0
   float a  = 0.5f;  // infiltration capacity of available storage - and index representing
                     // suface-connected porosity and the density of roots affecting infiltration.
   float s  = awa;
   float fc = 0.5f;  // final infiltration capacity (in/hr)

   float f = (gi * a * (float) pow((long double) s, (long double) 1.4) + fc) * 24 * 25.4f;  //infiltration in mm/day.

   return f;
   }

void WET_Hydro::LoadDistributedAirTemp()
   {
   m_metGridInfo.pMaxT1 = m_pMap->AddGridLayer("c:\\research\\andrews\\prism\\tmax01.air", DOT_INT, -1, 0);
   m_metGridInfo.pMinT1 = m_pMap->AddGridLayer("c:\\research\\andrews\\prism\\tmin01.air", DOT_INT, -1, 0);
   m_metGridInfo.pMaxT1->m_isVisible=false;
   m_metGridInfo.pMinT1->m_isVisible=false;
   m_metGridInfo.pMaxT2 = m_pMap->AddGridLayer("c:\\research\\andrews\\prism\\tmax02.air", DOT_INT, -1, 0);
   m_metGridInfo.pMinT2 = m_pMap->AddGridLayer("c:\\research\\andrews\\prism\\tmin02.air", DOT_INT, -1, 0);
   m_metGridInfo.pMaxT2->m_isVisible=false;
   m_metGridInfo.pMinT2->m_isVisible=false;   m_metGridInfo.pMaxT3 = m_pMap->AddGridLayer("c:\\research\\andrews\\prism\\tmax03.air", DOT_INT, -1, 0);
   m_metGridInfo.pMinT3 = m_pMap->AddGridLayer("c:\\research\\andrews\\prism\\tmin03.air", DOT_INT, -1, 0);
   m_metGridInfo.pMaxT3 = m_pMap->AddGridLayer("c:\\research\\andrews\\prism\\tmax03.air", DOT_INT, -1, 0);
  
   m_metGridInfo.pMaxT3->m_isVisible=false;
   m_metGridInfo.pMinT3->m_isVisible=false;
   m_metGridInfo.pMaxT4 = m_pMap->AddGridLayer("c:\\research\\andrews\\prism\\tmax04.air", DOT_INT, -1, 0);
   m_metGridInfo.pMinT4 = m_pMap->AddGridLayer("c:\\research\\andrews\\prism\\tmin04.air", DOT_INT, -1, 0);
   m_metGridInfo.pMaxT4->m_isVisible=false;
   m_metGridInfo.pMinT4->m_isVisible=false;
   m_metGridInfo.pMaxT5 = m_pMap->AddGridLayer("c:\\research\\andrews\\prism\\tmax05.air", DOT_INT, -1, 0);
   m_metGridInfo.pMinT5 = m_pMap->AddGridLayer("c:\\research\\andrews\\prism\\tmin05.air", DOT_INT, -1, 0);
   m_metGridInfo.pMaxT5->m_isVisible=false;
   m_metGridInfo.pMinT5->m_isVisible=false;
   m_metGridInfo.pMaxT6 = m_pMap->AddGridLayer("c:\\research\\andrews\\prism\\tmax06.air", DOT_INT, -1, 0);
   m_metGridInfo.pMinT6 = m_pMap->AddGridLayer("c:\\research\\andrews\\prism\\tmin06.air", DOT_INT, -1, 0);
   m_metGridInfo.pMaxT6->m_isVisible=false;
   m_metGridInfo.pMinT6->m_isVisible=false;
   m_metGridInfo.pMaxT7 = m_pMap->AddGridLayer("c:\\research\\andrews\\prism\\tmax07.air", DOT_INT, -1, 0);
   m_metGridInfo.pMinT7 = m_pMap->AddGridLayer("c:\\research\\andrews\\prism\\tmin07.air", DOT_INT, -1, 0);
   m_metGridInfo.pMaxT7->m_isVisible=false;
   m_metGridInfo.pMinT7->m_isVisible=false;
   m_metGridInfo.pMaxT8 = m_pMap->AddGridLayer("c:\\research\\andrews\\prism\\tmax08.air", DOT_INT, -1, 0);
   m_metGridInfo.pMinT8 = m_pMap->AddGridLayer("c:\\research\\andrews\\prism\\tmin08.air", DOT_INT, -1, 0);
   m_metGridInfo.pMaxT8->m_isVisible=false;
   m_metGridInfo.pMinT8->m_isVisible=false;
   m_metGridInfo.pMaxT9 = m_pMap->AddGridLayer("c:\\research\\andrews\\prism\\tmax09.air", DOT_INT, -1, 0);
   m_metGridInfo.pMinT9 = m_pMap->AddGridLayer("c:\\research\\andrews\\prism\\tmin09.air", DOT_INT, -1, 0);
   m_metGridInfo.pMaxT9->m_isVisible=false;
   m_metGridInfo.pMinT9->m_isVisible=false;
   m_metGridInfo.pMaxT10 = m_pMap->AddGridLayer("c:\\research\\andrews\\prism\\tmax10.air", DOT_INT, -1, 0);
   m_metGridInfo.pMinT10 = m_pMap->AddGridLayer("c:\\research\\andrews\\prism\\tmin10.air", DOT_INT, -1, 0);
   m_metGridInfo.pMaxT10->m_isVisible=false;
   m_metGridInfo.pMinT10->m_isVisible=false;
   m_metGridInfo.pMaxT11 = m_pMap->AddGridLayer("c:\\research\\andrews\\prism\\tmax11.air", DOT_INT, -1, 0);
   m_metGridInfo.pMinT11 = m_pMap->AddGridLayer("c:\\research\\andrews\\prism\\tmin11.air", DOT_INT, -1, 0);
   m_metGridInfo.pMaxT11->m_isVisible=false;
   m_metGridInfo.pMinT11->m_isVisible=false;
   m_metGridInfo.pMaxT12 = m_pMap->AddGridLayer("c:\\research\\andrews\\prism\\tmax12.air", DOT_INT, -1, 0);
   m_metGridInfo.pMinT12 = m_pMap->AddGridLayer("c:\\research\\andrews\\prism\\tmin12.air", DOT_INT, -1, 0);
   m_metGridInfo.pMaxT12->m_isVisible=false;
   m_metGridInfo.pMinT12->m_isVisible=false;   
   }

void WET_Hydro::GetUpslopeMetData(UPSLOPE_INFO *pUpslope)
   {   
   if (m_checkGridMet)
      {
      COleDateTime t( m_time );
      int hour = t.GetHour();
      int month = t.GetMonth();

      MapLayer *pMin = GetMonthlyGrid(month, 0);
      MapLayer *pMax = GetMonthlyGrid(month, 1);
      float ltTempMaxPrimet=0.0f;float ltTempMinPrimet=0.0f; int primetRow=-1; int primetCol=-1;
      pMin->GetGridCellFromCoord(559443,4895557.5,primetRow,primetCol);
      pMax->GetData(primetRow,primetCol,ltTempMaxPrimet);
      pMin->GetData(primetRow,primetCol,ltTempMinPrimet);
      float slopePrimet = ((ltTempMaxPrimet/100.0f) - (ltTempMinPrimet/100.0f))/(16.0f-3.0f);
     
      
      float griddedAirTempPrimet =( -slopePrimet*( 3 - hour)) + (ltTempMinPrimet/100.0f);
      if (hour >= 16 )
         {
         slopePrimet =-((ltTempMaxPrimet/100.0f) - (ltTempMinPrimet/100.0f))/(11.0f);
         griddedAirTempPrimet =(-slopePrimet*(16-hour)) + (ltTempMaxPrimet/100.0f);
         }
      if (hour < 3)
         {
         slopePrimet =-((ltTempMaxPrimet/100.0f) - (ltTempMinPrimet/100.0f))/(11.0f);
         griddedAirTempPrimet =(slopePrimet * (hour-3)) + (ltTempMinPrimet/100.0f) ;
         }
      float x=0.0f; float y=0.0f; int row=-1; int col=-1;float ltTempMax=0.0f; float ltTempMin=0.0f;
      m_pDEMLayer->GetGridCellCenter(pUpslope->rowIndex,pUpslope->colIndex, x, y);
      pMin->GetGridCellFromCoord(x,y,row,col);
      pMax->GetData(row,col,ltTempMax);
      pMin->GetData(row,col,ltTempMin);
      float slope = ((ltTempMax/100.0f) - (ltTempMin/100.0f))/(16.0f - 3.0f);
      float LTTH =( -slope*( 3 - hour)) + (ltTempMin/100.0f);
      if (hour >= 16 )
         {
         slope = -((ltTempMax/100.0f) - (ltTempMin/100.0f))/(11.0f);
         LTTH =(-slope*(16-hour)) + (ltTempMax/100.0f);
         }
      if (hour < 3)
         {
         slope= -((ltTempMax/100.0f) - (ltTempMin/100.0f))/(11.0f);
         LTTH =(slope * (hour-3)) + (ltTempMin/100.0f) ;
         }

         
       
         
    //  ASSERT(LTTH<=ltTempMax/100.0f && LTTH >= ltTempMin/100.0f);
      pUpslope->currentAirTemp = m_tempPrimet/griddedAirTempPrimet*LTTH;
    //  pUpslope->currentAirTemp = LTTH;
      int crap = 1;
      }
   else
      {
//		Upslope->currentAirTemp = m_pClimateData->IGetInternal((float)m_time,2,IM_LINEAR,m_timeOffset, false);

      }
         
      
   }
MapLayer *WET_Hydro::GetMonthlyGrid(int month, int minOrMax)
   {
   MapLayer *pMap = NULL;
   switch( month )
      {
      case 1:
         {
         if (minOrMax==0)
            pMap = m_metGridInfo.pMinT1;
         else
            pMap = m_metGridInfo.pMaxT1;
         break;
         }
      case 2:
         {
         if (minOrMax==0)
            pMap = m_metGridInfo.pMinT2;
         else
            pMap = m_metGridInfo.pMaxT2;
         break;
         }

      case 3: 
         {
         if (minOrMax==0)
            pMap = m_metGridInfo.pMinT3;
         else
            pMap = m_metGridInfo.pMaxT3;
         break;
         }
      case 4:
         {
         if (minOrMax==0)
            pMap = m_metGridInfo.pMinT4;
         else
            pMap = m_metGridInfo.pMaxT4;
         break;
         }
      case 5:
         {
         if (minOrMax==0)
            pMap = m_metGridInfo.pMinT5;
         else
            pMap = m_metGridInfo.pMaxT5;
         break;
         }
      case 6: 
         {
         if (minOrMax==0)
            pMap = m_metGridInfo.pMinT6;
         else
            pMap = m_metGridInfo.pMaxT6;
         break;
         }
      case 7:
         {
         if (minOrMax==0)
            pMap = m_metGridInfo.pMinT7;
         else
            pMap = m_metGridInfo.pMaxT7;
         break;
         }
      case 8:
         {
         if (minOrMax==0)
            pMap = m_metGridInfo.pMinT8;
         else
            pMap = m_metGridInfo.pMaxT8;
         break;
         }

      case 9: 
         {
         if (minOrMax==0)
            pMap = m_metGridInfo.pMinT9;
         else
            pMap = m_metGridInfo.pMaxT9;
         break;
         }
      case 10:
         {
         if (minOrMax==0)
            pMap = m_metGridInfo.pMinT10;
         else
            pMap = m_metGridInfo.pMaxT10;
         break;
         }
      case 11:
         {
         if (minOrMax==0)
            pMap = m_metGridInfo.pMinT11;
         else
            pMap = m_metGridInfo.pMaxT11;
         break;
         }

      case 12: 
         {
         if (minOrMax==0)
            pMap = m_metGridInfo.pMinT12;
         else
            pMap = m_metGridInfo.pMaxT12;
         break;
         }
      }  
   return pMap;
   }

int WET_Hydro::LoadDpaData( LPCTSTR filename )
   {
      if (m_saveEmfOutput)
         {
         // init movie capturing
         
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
      int day=0;
      int month=0;
      int year = 0;
      int hour = 0;
      int min = 0;
      int sec = 0;
      int jul=0;

   FILE *fp = fopen( "c:\\research\\matawai\\ppz2701b.ind", "rb" );
   FILE *fp1 = fopen( "c:\\research\\matawai\\ppz2701b.ima", "rb" );
   FILE *output = fopen( "c:\\research\\matawai\\read.txt", "w");
 FILE *output2 = fopen( "c:\\research\\matawai\\read1.txt", "w");

  /** Time in seconds since 0:00 1 January 1970 */
  long int time ;
  /** Offset from the start of the .ima file to the start of this scan/pulse */
  long int offset ;
  /** Number of range bins */
   long int noBytes;
   char flag;
   char space;
   int count=0;
   struct tm *newtime;
   int crap=sizeof(char);
   int crap2=sizeof(long int);
   int currentpos;
   rewind(fp);
   while ( ! feof( fp ) )
      {
      fread( &time, sizeof( long int ), 1, fp );
      currentpos=ftell(fp);
      newtime = gmtime( &time );
     // asctime( newtime );
      day=newtime->tm_mday;
      month=newtime->tm_mon;
      year = newtime->tm_year;
      hour = newtime->tm_hour;
      min = newtime->tm_min;
      sec = newtime->tm_sec;
      jul=newtime->tm_yday;

      fread( &offset, sizeof( long int ), 1, fp );
      currentpos=ftell(fp);

      fread( &noBytes, sizeof( long int ), 1, fp );
      currentpos=ftell(fp);
      fread( &flag, sizeof( char ), 1, fp );
      currentpos=ftell(fp);
      fread( &space, sizeof( char ), 1, fp );
      currentpos=ftell(fp);
      count++;
     // fprintf(output,"%i, %i, %i, %i, %i, %i, %i, %i, %i, %i, %i, %i, %i, %i\n",jul,month,day,year,hour,min,sec,time, offset, noBytes, flag, space, count, currentpos);

      //write this record somewhere
      //then continue with the next one
      } 
   fclose(output);

// next read the ima file (which contains the data)

           /** Azimuth angle of the antenna, in tenths of a degree */
  short int azimuth;
           /** Elevation angle of the antenna, in tenths of a degree */
  short int elevation;
            /** Number of lines in the processed radar image */
  unsigned short int noLines;
          /* Number of columns in the processed radar image */
  unsigned short int noSamples;
         /** Size of each pixel in the processed radar image, in metres */
  unsigned short int pixelSize;
  // if there is compression (assume there is - then this (the following 2) also exists in the file:
  /** Offset of the start of this block within the image */
  unsigned long int runOffset;
  /** Number of pixels (bytes) of data in this block (following
  this header, before the next header) */
  unsigned short int runLength;

  char data;
  //seek(fp,4,SEEK_SET);
  rewind(fp);
  rewind(fp1);
  int value=0;
  fread( &azimuth, sizeof( short int ), 1, fp1 );
  fread( &elevation, sizeof( short int ), 1, fp1 );
  fread( &noLines, sizeof( unsigned short int ), 1, fp1 );
  fread( &noSamples, sizeof( unsigned short int ), 1, fp1 );
  fread( &pixelSize, sizeof( unsigned short int ), 1, fp1 );
  MapLayer *pRain = gpMapWnd->m_pMap->GetLayer("RADAR");
  float vxMin=0.0f;
  float vxMax=0.0f;
  float vyMin=0.0f;
  float vyMax=0.0f;
  pRain->GetExtents(vxMin,vxMax,vyMin,vyMax);
  pRain->CreateGrid(noLines,noSamples,vxMin,vyMin,pixelSize,0,DOT_FLOAT);
  m_pMap->UpdateMapExtents();
  m_pMap->ZoomFull();
  m_pMap->RedrawWindow();
  rewind(fp1);
  int timecount=0;
 // while ( ! feof( fp1 ) )
  while (timecount<(count-2))
      {      
    
      fread( &time, sizeof( long int ), 1, fp );
      newtime = gmtime( &time );
      Notify( WH_UPDATERADARRAIN, (float)time, (float)count, (long)time,(long)0 );
      int curpos1=ftell(fp1);
      fread( &azimuth, sizeof( short int ), 1, fp1 );
      fread( &elevation, sizeof( short int ), 1, fp1 );
      fread( &noLines, sizeof( unsigned short int ), 1, fp1 );
      fread( &noSamples, sizeof( unsigned short int ), 1, fp1 );
      fread( &pixelSize, sizeof( unsigned short int ), 1, fp1 );

      timecount++;
      int rec = 0;
      int col = -1;
      int imageCount=0;
     // after this is the data - all of it (through all time)as one big array with the strange compression   
      fread( &offset, sizeof( long int ), 1, fp );
      fread( &noBytes, sizeof( long int ), 1, fp );     
      int currentByte=0;
      int counter=0;
      int curpos=ftell(fp1);
      while (currentByte<noBytes)//we know that the current time takes up noBytes in this file (not the same as total #pixels)
         {
         int curpos=ftell(fp1);
         fread( &runOffset, sizeof( unsigned long int ), 1, fp1 );//then read the header for the next compressed bit
         fread( &runLength, sizeof( unsigned short int ), 1, fp1 ); 
         currentByte=currentByte+6;
       
         int numberOfZeros=runOffset-imageCount;
         for(int off=0;off<numberOfZeros;off++)
            {           
            if (col < (noSamples-1))
               col++;            
            else
               {
               rec++;
               col=0;               
               }     
            ASSERT(col<pRain->GetColCount());
            ASSERT(rec<pRain->GetRecordCount());
            pRain->SetData(rec,col,-999.0f);
            int x=0;
            imageCount++;
            counter++;
           // fprintf(output2,"%i, %i, %i, %i, %i, %i\n",rec, col, x, currentByte, imageCount, counter);
            }

//         ASSERT(runOffset==imageCount);
         for (int y=0;y<runLength;y++) //and it is strangely compressed
            {
            fread(&data,1,1,fp1); //read actual data  
            float Z = pow(10.0f,(data/10.0f));
            float R = pow((Z/200.0f),(1.0f/1.6f));

            if (col < (noSamples-1))
               col++;                  
            else
               {
               rec++;
               col=0;
               }  
            ASSERT(col<pRain->GetColCount());
            ASSERT(rec<pRain->GetRecordCount());
            pRain->SetData(rec,col,R);
            int x=(int)R;
            imageCount++;           
            currentByte++;
            counter++;
          //  fprintf(output2,"%i, %i, %i, %i, %i, %i\n",rec, col, x, currentByte, imageCount, counter);
            }

         }

         while (counter<(pRain->GetRecordCount()*pRain->GetColCount()))//if we are not at the last column, add 0s till we get there.
            {
            if (col < (noSamples-1))
               col++;                  
            else
               {
               rec++;
               col=0;
               } 
            ASSERT(col<pRain->GetColCount());
            ASSERT(rec<pRain->GetRecordCount());
            pRain->SetData(rec,col,-999.0f);
            imageCount++;
            counter++;
           
            int x=0;
          //  fprintf(output2,"%i, %i, %i, %i, %i, %i\n",rec, col, x, currentByte, imageCount, counter);
            }                  
            
           m_pFlowDirLayer->SetBinColorFlag( BCF_MIXED);
          
         pRain->SetBins(0,35,20);
         pRain->m_isVisible=true;
			pRain->ClassifyData(); 
        
 //now we are done reading this time, and we move on to the next.
      fseek(fp,2,SEEK_CUR);//we need to get the noBytes from the correct position in the ind file.  This is 10 from 
      // where we are right now.
   }
   fclose(fp1);
   fclose(output2);
   fclose(fp);

   if(m_saveEmfOutput)
      {
      for(int i = 0; i < numMovies; i++)
         {
         AVIGenerators[i]->ReleaseEngine();
         delete AVIGenerators[i];
         AVIGenerators[i] = 0;

         ReleaseDC(NULL, auxMovieStuff[i].bitmapDC);
         delete [] auxMovieStuff[i].bitAddress;
         }
      delete [] AVIGenerators;
      delete [] auxMovieStuff;
      AVIGenerators = 0;
      }
   return -1;
}

   void WET_Hydro::ReverseByteOrder(short *in_array,int arraysize)
      {

 
      }

void WET_Hydro::WriteFrame()
   {

   // get a device context to the entire application window (view)
   CDC* dc = gpView->GetWindowDC();

   CRect rect;
   gpView->GetWindowRect(rect);

   for(int i = 0; i < numMovies; i++)
      {
      // no need to save old bitmap from bitmapDC (for now)
      /*
      CBitmap* oldBitmap = bitmapDC.SelectObject(&bitmap);
      bitmapDC.SelectObject(&bitmap);
      */

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

int WET_Hydro::GetNeighbors(int colToSubset, LPCSTR filename1 , LPCSTR filename2, LPCSTR filename3,LPCSTR filename4)
   {
   int recordCount = m_pCellLayer->GetRecordCount();
    m_fpDistributionArray.SetSize(recordCount);
  // int keepCol= m_pCellLayer->m_pData->GetCol("keep");
   int include1=-1;
   int keeper1=-1;
      int polyCount=0;
      int polyNeighbor=0;
   for ( int i=0; i < recordCount; i++ )
      {
      m_pMap->Notify( NT_CALCDIST, i, recordCount );  
      Poly *pPoly = m_pCellLayer->GetPolygon( i );
      int neighbor=-1;
      int include=-1;
      int keeper=-1;


   //   m_pCellLayer->GetData(i,/*m_pCellLayer->m_pData->GetCol("LULC_C")*/ colToSubset, include1);
   //   m_pCellLayer->GetData(i,keepCol, keeper1);
    
    //  if (keeper1==0 || include1==0 )
       //  continue;
      polyCount++;
      for ( int j=0; j < recordCount; j++ )
         {
      //   m_pCellLayer->GetData(j,/*m_pCellLayer->m_pData->GetCol("LULC_C")*/ colToSubset, include);
      //    m_pCellLayer->GetData(j,keepCol, keeper);
        // if (keeper==0 || include==0)
        //    continue;
         polyNeighbor++;
         Poly *pToPoly = m_pCellLayer->GetPolygon( j );

         if ( i==j ) // skip this polygon
            continue;
         if (pPoly->DoesBoundingBoxIntersect(pToPoly)) //check vertices
            {
            int thisVertexCount = pPoly->GetVertexCount();
	         int otherVertexCount = pToPoly->GetVertexCount();
            for ( int u=0; u<thisVertexCount; u++)
               {
               bool found=false;
               Vertex thisVertex = pPoly->GetVertex( u );
	            for ( int v=0; v < otherVertexCount; v++ )
	               {
                  
                  Vertex otherVertex = pToPoly->GetVertex( v );
                  if (otherVertex.x==thisVertex.x && otherVertex.y==thisVertex.y) //then the two polys are adjacent...
                     {
                     //add the areakey from pToPoly to pPoly's array of neighbors.
                     m_fpDistributionArray[i].Add(j);
                     //add i to maxtrix - this means i will be repeated for each j
                     //add j to matrix
                     found=true;
                     break;

                     
                     }                   
		            }  // end of:  for ( i < otherVertexCount-1   
               if (found)
                  break;
               }
            }
         }
      }

      FILE *fp1 = fopen(filename1, "w" );

      if ( fp1 == NULL )
        {
         CString msg;
         msg.Format( "Unable to open file %s", filename1 );
         ErrorMsg( msg );
         //return -1;
         }
      int count = 0;
      for (int k=0; k<m_fpDistributionArray.GetCount();k++) //for each watershed
         {
         for (int l=0;l<m_fpDistributionArray[k].GetCount();l++) //collect all flowpaths
            {
            count = fprintf(fp1,"%i ",m_fpDistributionArray[k][l]+1); // and write them to fp
            }
         count = fprintf(fp1,"\n");//this watershed is written, add a newline and do the next
         }
fclose(fp1);


     FILE *fp = fopen( filename2, "w" );

      if ( fp == NULL )
        {
         CString msg;
         msg.Format( "Unable to open file %s", filename2);
         ErrorMsg( msg );
         //return -1;
         }

      for (int k=0; k<m_fpDistributionArray.GetCount();k++) //for each watershed
         {
         for (int l=0;l<m_fpDistributionArray[k].GetCount();l++) //collect all flowpaths
            {
           count = fprintf(fp,"%i ",k+1); // and write them to fp  This one is Mindy's original
            }
         }
      fclose(fp);
 

 FILE *fp2 = fopen( filename3, "w" );

      if ( fp == NULL )
        {
         CString msg;
         msg.Format( "Unable to open file %s", filename3 );
         ErrorMsg( msg );
         //return -1;
         }
     
         
      for (int k=0; k<m_fpDistributionArray.GetCount();k++) //for each watershed
         {
         for (int l=0;l<m_fpDistributionArray[k].GetCount();l++) //collect all flowpaths
            {
          // count = fprintf(fp,"%i ",k); // and write them to fp
               int cd = m_fpDistributionArray[k][l]+1;
            count = fprintf(fp2,"%i ",cd); // and write them to fp
            }
         //count = fprintf(fp,"\n");//this watershed is written, add a newline and do the next
         }


      fclose( fp2 );

 FILE *fp3 = fopen( filename4, "w" );

      if ( fp3 == NULL )
        {
         CString msg;
         msg.Format( "Unable to open file %s", filename3 );
         ErrorMsg( msg );
         //return -1;
         }
     
         
      for (int k=0; k<m_fpDistributionArray.GetCount();k++) //for each watershed
         {
         for (int l=0;l<m_fpDistributionArray[k].GetCount();l++) //collect all flowpaths
            {

            count = fprintf(fp3,"%4.2f ",1.0f/m_fpDistributionArray[k].GetSize()); // and write them to fp
            }
        
         }


      fclose( fp3 );



      m_fpDistributionArray.RemoveAll();

   return 0;
   }
