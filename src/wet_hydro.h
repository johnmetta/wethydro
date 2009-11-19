// WET_Hydro.h: interface for the WET_Hydro class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WET_HYDRO_H__C9203D32_B4B1_11D3_95C1_00A076B0010A__INCLUDED_)
#define AFX_WET_HYDRO_H__C9203D32_B4B1_11D3_95C1_00A076B0010A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
class DataTable;
#include <DataStore.h>

#include <maplayer.h>   // for RowColArray;
#include <reachtree.h>
#include <fdataobj.h>
#include <vdataobj.h>
#include <nr.h>
#include <nrutil.h>
#include <randgen\randunif.hpp>
#include <map3d/avigenerator.h>

enum WH_NOTIFY_TYPE { WH_UPDATE, WH_STARTRUN, WH_ENDRUN, WH_UPDATE_MONTE_CARLO, WH_ALLOCATEOBJECTS, WH_BUILDTOPOLOGY, WH_UPDATERADARRAIN};

class WET_Hydro;
typedef bool (*WH_NOTIFYPROC)( WET_Hydro*, WH_NOTIFY_TYPE, float, float, long, long );

const int WH_DISTRIBUTE_WATERSHEDS =           0;
const int WH_DISTRIBUTE_HILLRIPARIAN =         1;
const int WH_DISTRIBUTE_CENTROIDDISTANCEBINS = 2;
const int WH_DISTRIBUTE_BUFFERDISTANCEBINS   = 3;

const int WH_DISTRIBUTE_GEOMORPH              =4;
const int WH_DISTRIBUTE_LUMPED                =5;
const int WH_DISTRIBUTE_FPGRID                =6; 
const int WH_DISTRIBUTE_WTHEIGHT              =7;

const int WH_PARAM_WATERSHEDS      =            0;
const int WH_PARAM_HILLRIPARIAN     =           1;
const int WH_PARAM_GEOMORPHOLOGY =              2;
const int WH_PARAM_GEOMHILLRIP =                3;
const int WH_PARAM_SOILS =                      4;

const int ET_HARGREAVES = 0;
const int ET_PENMAN     = 1;
const int ET_PENMAN_MONTEITH = 2;
const int ET_MEASURED = 3;
const int UNSAT_VANGEN = 0;
const int UNSAT_BROOKSCOREY= 1;
const int SAT_TOPMODEL = 0;
const int SAT_SEIBERTMCD= 1;
const int DRAIN_P_CONSTANT = 0;
const int DRAIN_P_EXPONENTIAL = 1;

const int WH_NETWORK_SUM = 0;  // instream routing is a simple sum (assumed instantaneous)
const int WH_NETWORK_EXPLICIT = 1; //  instream routing is distributed.

const  int WH_SINGLE_SIMULATION = 0;
const  int WH_MONTE_CARLO = 1;

typedef float REAL; 

class StreamCellIndexArray : public CArray< CUIntArray, CUIntArray& >
   {

   };
class FPDistributionArray : public CArray< CUIntArray, CUIntArray& >
   {

   };
class SavedDataArray : public CArray< float, float& >
   {

   };

struct DYNAMIC_MAP_INFO
	{
   CString fieldname;
	float minVal;
	float maxVal;
   DYNAMIC_MAP_INFO( void ) : fieldname(_T("")),minVal(0.5f), maxVal(1.0f) {}
	};

struct SOIL_INFO
   {
   char   texture[ 64 ];
   char   abbrev[5];
   float  c;
   float  phi;
   float  kSat;
   float  wiltPt;
   float  fieldCapac;

   SOIL_INFO( void ) : texture(), abbrev(), c( 0.0f ), phi( 0.0f ), kSat(0.0f), wiltPt(0.0f),fieldCapac(0.0f) { }
   };


class SoilInfoArray : public CArray< SOIL_INFO*, SOIL_INFO* >
   {
   public:
      // destructor cleans up any allocated structures
     ~SoilInfoArray( void ) { for ( int i=0; i < GetSize(); i++ ) delete GetAt( i ); }      
   };

struct GOODNESS_OF_FIT_INFO
   {
   float  r2;
   float  rmse;
   float  ns;
   float  d;


   GOODNESS_OF_FIT_INFO( void ) : r2( 0.0f ), rmse( 0.0f ), ns(0.0f), d(0.0f) { }
   };


class GoodnessOfFitInfoArray : public CArray< GOODNESS_OF_FIT_INFO*, GOODNESS_OF_FIT_INFO* >
   {
   public:
      // destructor cleans up any allocated structures
     ~GoodnessOfFitInfoArray( void ) { for ( int i=0; i < GetSize(); i++ ) delete GetAt( i ); }      
   };

struct COLLECT_DATA_CELL_OFFSET
   { //in a semi distributed, not grid model, we only use the first.  With a grid, first=x and second=y
   int firstOffset;
   int secondOffset;


   COLLECT_DATA_CELL_OFFSET( void ) : firstOffset( 0 ), secondOffset( 0 ) { }
   };


class CollectDataCellOffsetArray : public CArray< COLLECT_DATA_CELL_OFFSET*, COLLECT_DATA_CELL_OFFSET* >
   {
   public:
      // destructor cleans up any allocated structures
     ~CollectDataCellOffsetArray( void ) { for ( int i=0; i < GetSize(); i++ ) delete GetAt( i ); }      
   };



struct AUXILLARY_MODEL_INFO
   {
   char   name[ 64 ];
   int    landuse;
   float  c;
   float  p;

   AUXILLARY_MODEL_INFO( void ) : name(), landuse(0), c( 1.0f ), p( 1.0f ) { }
   };


class AuxillaryModelInfoArray : public CArray< AUXILLARY_MODEL_INFO*, AUXILLARY_MODEL_INFO* >
   {
   public:
      // destructor cleans up any allocated structures
     ~AuxillaryModelInfoArray( void ) { for ( int i=0; i < GetSize(); i++ ) delete GetAt( i ); }      
   };

struct VEG_INFO
   {
   float mass; // Units are kg/m2, where m2 is an area of plant.  From PRZM.
   float e ; // extraction coefficient (cm-1) (smith &carsel 85 suggest 0.1 as good
   float epsilon; //uptake efficiency factor (dimensionless)
   float kf; // first-order foliar degradation rate constant (day-1) 
   float ks; // degration constation
   float rateMassApp;
   float rateFoliarRunoff;
   float cumMassInput;
   float rateTransformation;
   float cumTransformation;
   float volumeWater;
   VEG_INFO( void ) : mass( 0.00f ), volumeWater(1.0f), e(0.1f), kf(0.7f),ks(0.1f), epsilon(0.1f),
      rateMassApp(0.0f), rateFoliarRunoff(0.0f), cumMassInput(0.0f) , cumTransformation(0.0f), rateTransformation(0.0f){ }
   };

struct SURF_WATER_INFO
   {
   float mass;
   float volumeWater;
   float concentration;
   float rateMassApp;
   float cumMassApp;
   float rateAdvection;
   float rateTransformation;
   float cumTransformation;
   float rateUptake;
   float cumUptake;
   float rateRunoff;
   float cumRunoff;
   float rateMassOutflow;
   float rateErosionLoss;
   float cumErosionLoss;
   float kg;  // lumped first order decay constant for the gas phase (1/day)
   float kh;  // henry's law constant
   float rom; // enrichment ratio for organic matter (g/g)
   float kd;  // adsorption partion coefficient (cm3/g)
   float contributeToStream;
   float bulkDensity;

   float ks;  // first order decay constant for solid and dissolved phases (1/day)
   SURF_WATER_INFO( void ) : mass( 0.00000f ),volumeWater(1.0f),concentration(0.00f), rateMassApp(0.0f), rateAdvection(0.0f), rateTransformation(0.0f), rateUptake(0.0f), rateRunoff(0.0f),rateMassOutflow(0.0f),
      ks(0.1f),kg(0.01f),kh(0.01f), rateErosionLoss(0.0f), rom(0.1f), kd(0.1f), bulkDensity(1350.0f) ,contributeToStream(0.0f),
                             cumMassApp(0.0f),cumTransformation(0.0f),cumUptake(0.0f), cumRunoff(0.0f), cumErosionLoss(0.0f){ }
   };
struct SURF_SOIL_INFO
   {
   float mass;
   float rateTransformation;
   float rateErosionLoss;
   float rateMassOutflow;
   float rom; // enrichment ratio for organic matter (g/g)
   float kd;  // adsorption partion coefficient (cm3/g)

   float bulkDensity;

   SURF_SOIL_INFO( void ) : mass( 0.0f ), rateTransformation(0.0f), rateErosionLoss(0.0f), rateMassOutflow(0.0f), rom(0.5f), kd(0.5f), bulkDensity(1350.0f) { }
   };
struct UNSAT_WATER_INFO
   {
   float mass;
   float volumeWater;
   float concentration;
   float rateMassApp;
   float rateAdvection;
   float rateTransformation;
   float cumTransformation;
   float rateUptake;
   float cumUptake;
   float rateMassOutflow;
   float uptakeEfficiency;
   float kg;  // lumped first order decay constant for the gas phase (1/day)
   float kh;  // henry's law constant
   float kd;
   float ks;

   UNSAT_WATER_INFO( void ) : mass( 0.0f ), volumeWater(1.0f),concentration(0.0f),rateMassApp(0.0f), rateAdvection(0.0f), rateTransformation(0.0f), 
      rateUptake(0.0f), kg(0.01f), kh(0.01f), kd(0.1f), ks(0.1f),rateMassOutflow(0.0f), uptakeEfficiency(0.5f),
      cumUptake(0.0f), cumTransformation(0.0f){ }
   };
struct UNSAT_SOIL_INFO
   {
   float mass;
   float rateTransformation;
   float rateMassOutflow;
   UNSAT_SOIL_INFO( void ) : mass( 0.0f ), rateTransformation(0.0f), rateMassOutflow(0.0f){ }
   };
struct SAT_WATER_INFO
   {
   float mass;
   float volumeWater;
   float concentration;
   float kd;
   float ks;
   float rateAdvection;
   float rateTransformation;
   float rateMassOutflow;
   float rateMassInflow;

   float cumTransformation;
   SAT_WATER_INFO( void ) : mass( 0.0f ), volumeWater(1.0f),concentration(0.0f), kd(0.01f),ks(0.01f),rateAdvection(0.0f), 
      rateTransformation(0.0f), rateMassOutflow(0.0f), rateMassInflow(0.0f), cumTransformation(0.0f){ }
   };
struct SAT_SOIL_INFO
   {
   float mass;
   float rateTransformation;
   float rateMassOutflow;
   SAT_SOIL_INFO( void ) : mass( 0.1f ), rateTransformation(0.0f), rateMassOutflow(0.0f){ }
   };

struct PESTICIDE_INFO
   {
   float appRate;
   int   appMonth;
   int   appDay;
   bool beenApplied;
   int offset;
   float oCarbon;
   float koc;
   CString pesticideName;
   float surfaceZoneDepth;
   float rootZoneDepth;
   VEG_INFO *pVeg;
   SURF_WATER_INFO *pSurfWater;
   SURF_SOIL_INFO  *pSurfSoil;
   UNSAT_WATER_INFO *pUnsatWater;
   UNSAT_SOIL_INFO *pUnsatSoil;
   SAT_WATER_INFO *pSatWater;
   SAT_SOIL_INFO *pSatSoil;
   PESTICIDE_INFO( void ) : offset(0), beenApplied(false), oCarbon(0.5f),koc(0.1f),surfaceZoneDepth(0.15f), rootZoneDepth(0.5f), 
      pVeg(NULL), pSurfWater(NULL), pSurfSoil(NULL), pUnsatWater(NULL), pesticideName(_T("")),
      pUnsatSoil(NULL), pSatWater(NULL), pSatSoil(NULL), appRate(0.0f), appMonth(1), appDay(1)  { }
   };

class PesticideArray : public CArray< PESTICIDE_INFO*, PESTICIDE_INFO* >
   {
   public:
      // destructor cleans up any allocated structures
     ~PesticideArray( void ) { for ( int i=0; i < GetSize(); i++ ) delete GetAt( i ); }      
   };

struct CHEMICAL_APP_INFO
   {
   CString name;
   int    lulc_c;
   int    lulc_b;
   int    soil;
   float  rateMin;
   float  rateMax;
   int    beginMonth1;
   int    beginDay1;
   int    endMonth1;
   int    endDay1;
   CHEMICAL_APP_INFO( void ) : name(_T("")), lulc_c(0), lulc_b(0), soil(0),rateMin( 1.0f ),
      rateMax(10.0f),beginMonth1(10), beginDay1(1), endMonth1(1), endDay1(3) { }
   };
class ChemicalAppInfoArray : public CArray< CHEMICAL_APP_INFO*, CHEMICAL_APP_INFO* >
   {
   public:
      // destructor cleans up any allocated structures
     ~ChemicalAppInfoArray( void ) { for ( int i=0; i < GetSize(); i++ ) delete GetAt( i ); }      
   };

struct CHEMICAL_PARAM_INFO
   {
   CString name;
   float  e;
   float  kf;
   float  kg; //ks values are degradation rates, but kow/kd/koc have to do with sorption..
   float  kh;
   float  ksMin;
   float  ksMax;
   float  koc;
   float  rom;

   CHEMICAL_PARAM_INFO( void ) : name(_T("")), e(0.01f), kf(0.01f), ksMin(0.0f), ksMax(0.0f),kg( 1.0f ), 
      kh(10.0f), koc(0.0f),rom(10.0f)  { }
   };

class ChemicalParamInfoArray : public CArray< CHEMICAL_PARAM_INFO*, CHEMICAL_PARAM_INFO* >
   {
   public:
      // destructor cleans up any allocated structures
     ~ChemicalParamInfoArray( void ) { for ( int i=0; i < GetSize(); i++ ) delete GetAt( i ); }      
   };
struct HYDROLOGIC_PARAM_INFO
   {
   CString landscapeType;
   float  m;
   float  kSat;
   float  wiltPt;
   float  fieldCapac;
   float  thetaTrace;  //Seibert and McDonnell horizontal drainage model
   float  k2;  //Deep reservoir discharge coefficient
   float  initSat;
   float  soilDepth;
   float  phi;
   float kDepth;
   float  vanGenN;  //unsaturated vertical rate equation (from VanGenucten, the m is based on this value)
   float   powerLExp;
   float initUnSat;
   float tt;  //threshold temperature
   float cfmax;//degree day factor
   float cfr; //refreeze coeficient
   float condense;
   float cwh;//threshold for storage of melt water in snowpack
   HYDROLOGIC_PARAM_INFO( void ) : landscapeType(_T("")),m(40.0f), kSat(5.0f), wiltPt(0.15f), fieldCapac(0.19f),thetaTrace( 1.0f ), 
      k2(1.0f), initSat(0.8f), soilDepth(1.0f),phi(0.2f), vanGenN(0.1f),kDepth(0.0001f), powerLExp(4.0f),initUnSat(0.2f),
      tt(0.1f),cfmax(2.0f),cfr(0.05f),cwh(0.1f),condense(0.0f)
 { }
   };

//This is an array of HYDROLOGIC_PARAM_INFO.  It is necessary because different landscape Types may have different
//  parameter combinations (landscape type might be HILLSLOPE and RIPARIAN, for instanc...)

class HydrologicParamInfoArray : public CArray< HYDROLOGIC_PARAM_INFO*, HYDROLOGIC_PARAM_INFO* >
   {
   public:
      // destructor cleans up any allocated structures
     ~HydrologicParamInfoArray( void ) { for ( int i=0; i < GetSize(); i++ ) delete GetAt( i ); }      
   };
struct INSTREAM_HYDRO_PARAM_INFO
   {
   CString landscapeType;
   float  n;
   float  wd; // channel width-depth ratio
   INSTREAM_HYDRO_PARAM_INFO( void ) : landscapeType(_T("")),n(0.04f), wd(15.0f) { }
   };

//This is an array of HYDROLOGIC_PARAM_INFO.  It is necessary because different landscape Types may have different
//  parameter combinations (landscape type might be HILLSLOPE and RIPARIAN, for instanc...)

class InstreamHydroParamInfoArray : public CArray< INSTREAM_HYDRO_PARAM_INFO*, INSTREAM_HYDRO_PARAM_INFO* >
   {
   public:
      // destructor cleans up any allocated structures
     ~InstreamHydroParamInfoArray( void ) { for ( int i=0; i < GetSize(); i++ ) delete GetAt( i ); }      
   };

struct UPSLOPE_UNSAT_INFO
   {
   float volumeUnsat;
   float voidVolume;
   float unsaturatedDepth;
   float usfVol;
   float swcUnsat;
   float tracerConc;
	float tracerMass;
   float tracerOutflowMass;
   UPSLOPE_UNSAT_INFO( void ) : volumeUnsat(0.0f),voidVolume(0.0f),unsaturatedDepth( 0.0f ), usfVol(0.0f), swcUnsat(0.0f), tracerConc(0.0f) ,
         tracerMass(0.0f), tracerOutflowMass(0.0f) {}
   };

struct TILL_INFO
   {
   float volumeTill;
   float lateralOutflowVol;
   float lateralInflowVol;

   TILL_INFO( void ) : volumeTill(0.0f),lateralOutflowVol(0.0f), lateralInflowVol(0.0f) {}
   };


struct UPSLOPE_INFO     // contains upslope unit information
   {
   float volumeSat;     // volume in the saturated zone (m3)
   float voidVolume;   // volume in the unsaturated zone (m3)
   float volumeSWE;
   float volumeVeg;
   float vegDrainRate;
   float meltRate;
   float saturatedDepth;
  // float unsaturatedDepth;
   float ssfVol;
//   float usfVol;
   float sdmm;
   float area;       // area (m2)
	int index;
	float areaTotal;
   float upstreamLength;
   int   basinOrder;
//   float kDepth;
   float c;          // 
   float musleP;
   float oCarbon;
   float slope;      // rise/run
   float lengthSlope;
   float kFfact;
   int   isSaturated;
   float ltPrecip;     // long term yearly precip in m
	float minFPDist;
	float maxFPDist;
   int   sideOfStream;
   float lateralOutflowVol; 
   float surfaceOutflowVol;
	float lateralInflow;
   int lulc;
   int lulc_b;
	float swc;
   float swcUnsat;
   float pondedVolume;
	float tracerConc;
	float tracerMass;
   float initUpslopeTracerMass;
   float tracerOutflowMassSubsurface;
   float tracerOutflowMassSurface;
   float tracerInflowMassSubsurface;
   float tracerWeightedMeanTime;
   float sedimentMass;
   float sedimentOutflowMass;
   float musleC;
   int   colIndex;
   int   rowIndex;
   int   radarRow;
   int   radarCol;
   float currentPrecip;
   float currentSnow;
   float currentET;
   float currentAirTemp;
	float cumPrecipVol;
   float precipVol;
	float cumETVol;
   float cumGroundLoss;
   float etVol;
	float cumSWC;
   int hasWetlands; // are there wetland areas in the cell?
   float tileOutflowCoef;
   float tileDrainageVol;
   float cumTracerMassInPrecip;
   int producedOverlandFlow;
   float groundWaterChange; // the change (m) of the groundwater table (+ or -)
   float deltaSat;
   float deltaUnsat;
   float elev;
   bool isStream;
   float initTracerMass;
   float resTime;
   float firstMoment;
   float zerothMoment;
   TILL_INFO *pTill;
   UPSLOPE_UNSAT_INFO *pUnsat;
	CUIntArray cellsRepresentedArray;
	UPSLOPE_INFO *pUpslope;
   PesticideArray pesticideArray;
   HYDROLOGIC_PARAM_INFO *pHydroParam;
   class UpslopeInfoArray1 : public CArray< UPSLOPE_INFO*, UPSLOPE_INFO* >
      {
      public:
         ~UpslopeInfoArray1( void ) { RemoveAll(); }

         void RemoveAllElements( ) 
            { 
            for (int i=0; i < GetSize(); i++) 
               delete GetAt( i );
   			
			   RemoveAll();
			   }
            
      };
   UpslopeInfoArray1 neighborArray;
   

   UPSLOPE_INFO( void ) : volumeSat( 0.0f),  saturatedDepth(0.0f), voidVolume(0.0f),initUpslopeTracerMass(0.0f),
      ssfVol(0.0f),sdmm(0.0f), area( 1.0f ), index(0), isSaturated(0), oCarbon(0.1f),
      areaTotal( 0.0f ),  swcUnsat(0.0f), groundWaterChange(0.0f),upstreamLength(0.0f),basinOrder(0),
      c( 0.0f ),  slope( 0.0f ), lengthSlope(0.0f), kFfact(0.32f),lulc_b(0),
      ltPrecip(1.5f), minFPDist(0.0f), musleP(0.0f), deltaSat(0.0f), deltaUnsat(0.0f),
      maxFPDist(-1.0f), sideOfStream(0), lateralOutflowVol( 0.0f), lateralInflow(0.0f), lulc( 0 ),
      swc(0.0f), pondedVolume(0.0f),tracerConc(0.0f), tracerMass(0.0f),tracerOutflowMassSurface(0.0f), tracerInflowMassSubsurface(0.0f),
      sedimentMass(1E20f), sedimentOutflowMass(0.0f), musleC(0.0f),
      currentPrecip(0.0f),currentET(0.00f),currentAirTemp(10.0f), cumPrecipVol(0.0f),precipVol(0.0f),
      etVol(0.0f), cumETVol(0.0f), cumSWC(0.0f), cumGroundLoss(0.0f),
      hasWetlands(0), tileOutflowCoef(0.0f), tileDrainageVol(0.0f),tracerWeightedMeanTime(0.0f),
      pUpslope(NULL), pUnsat(NULL), pTill(NULL),cumTracerMassInPrecip(0.0f), tracerOutflowMassSubsurface(0.0f), surfaceOutflowVol(0.0f),
      producedOverlandFlow(0),  pesticideArray(), colIndex(0), rowIndex(0), elev(0.0f), isStream(false), neighborArray(),
      radarRow(0),radarCol(0),initTracerMass(0.0f), volumeSWE(0.0f), meltRate(0.0f), currentSnow(0.0f), volumeVeg(0.0f),vegDrainRate(0.0f), resTime(0.0f),
	  firstMoment(0.0f), zerothMoment(0.0f)
   { }
   };

class UpslopeInfoArray : public CArray< UPSLOPE_INFO*, UPSLOPE_INFO* >
   {
   public:
      ~UpslopeInfoArray( void ) { RemoveAll(); }

      void RemoveAllElements( ) 
         { 
         for (int i=0; i < GetSize(); i++) 
            delete GetAt( i );
			
			RemoveAll();
			}
         
   };

class UpslopeFor2D : public CArray< UpslopeInfoArray*, UpslopeInfoArray* >
   {
   public:
      ~UpslopeFor2D( void ) { RemoveAll(); }

      void RemoveAllElements( ) 
         { 
         for (int i=0; i < GetSize(); i++) 
            delete GetAt( i );
			
			RemoveAll();
			}
         
   };

struct INSTREAM_PESTICIDE_INFO
   { 
   CString pesticideName;
   float *pesticideMassArray;
   float *pesticideMassArrayUpstream;
   float *pesticideDischargeArray;
   float pesticideConc;
   float upslopeContribution;
   float cumPesticideOutflow;

   INSTREAM_PESTICIDE_INFO( void ) : pesticideName(_T("")),pesticideMassArray(NULL), pesticideMassArrayUpstream(NULL),pesticideDischargeArray(NULL), 
      upslopeContribution(0.0f),pesticideConc(0.0), cumPesticideOutflow(0.0)  { }
   };

class InstreamPesticideArray : public CArray< INSTREAM_PESTICIDE_INFO*, INSTREAM_PESTICIDE_INFO* >
   {
   public:
      // destructor cleans up any allocated structures
     ~InstreamPesticideArray( void ) { for ( int i=0; i < GetSize(); i++ ) delete GetAt( i ); }      
   };


struct REACH_INFO_HYDRO   // extends the REACH_INFO structure
   {
   float width;      // width of the stream reach (m)   ASSSUMES RECTANGULAR
   float depth;      // depth of the stream reach (m)
  
   float alpha;
   float beta;
   float lateralInflow;      // flow rate into this reach
   float cumInflow;
   float cumOutFlowVol;
   float cumLateralInflow;
	int   streamOrder;
	float ttp;
	float maxFPDistSide1;
   float maxFPDistSide2;
   float tracerConc;
   float resTime;
   float firstMoment;
   float zerothMoment;
   float sedimentConc;
   float percentLengthAltered;
   int   incrFlow;
   float cumSedimentOutflow;
   InstreamPesticideArray instreamPesticideArray;
   float *sedimentMassArray;
   float *tracerMassArray;//correpsonds to reachTree qArray
   float cumTracerInput;
   float *waterVolumeArray;
   float cumTracerOutFlow;
   float *tracerDischargeArray;

   float *qArrayPrevious;
   float *sedimentDischargeArray;
   INSTREAM_HYDRO_PARAM_INFO *pInstreamHydroParam;
   UpslopeInfoArray upslopeInfoArray;
   UpslopeFor2D upslopeFor2DArray;

   REACH_INFO_HYDRO( void ) : width( 0.0f ), depth( 0.0f ), 
       alpha( 1.0f) , beta( 3/5.0f), lateralInflow( 0.0f), 
      cumInflow( 0.0f ), cumOutFlowVol( 0.0f ), cumLateralInflow( 0.0f ), streamOrder( 0 ), 
      ttp( 0.0f ), maxFPDistSide1(0.0f), maxFPDistSide2(0.0f), tracerConc(0.0f),
      percentLengthAltered(0.0f), incrFlow(0), cumSedimentOutflow(0.0f), resTime(0.0f), firstMoment(0.0f),zerothMoment(0.0f),
      sedimentConc(0.0f), sedimentMassArray(NULL), cumTracerInput(0.0f),
      tracerMassArray(NULL),cumTracerOutFlow(0.0f), waterVolumeArray(NULL), 
      tracerDischargeArray(NULL), qArrayPrevious(NULL), sedimentDischargeArray(NULL),
      upslopeInfoArray(), instreamPesticideArray(), pInstreamHydroParam(NULL), upslopeFor2DArray()
      { }

   };


class ReachInfoHydroArray : public CArray< REACH_INFO_HYDRO*, REACH_INFO_HYDRO* >
   {
   public:
      ~ReachInfoHydroArray( void ) {for ( int i=0; i < GetSize(); i++) delete GetAt( i ); }

   };
struct MET_GRID_INFO
   {
   MapLayer *pMinT1;
   MapLayer *pMaxT1;
   MapLayer *pMinT2;
   MapLayer *pMaxT2;
   MapLayer *pMinT3;
   MapLayer *pMaxT3;
   MapLayer *pMinT4;
   MapLayer *pMaxT4;
   MapLayer *pMinT5;
   MapLayer *pMaxT5;
   MapLayer *pMinT6;
   MapLayer *pMaxT6;
   MapLayer *pMinT7;
   MapLayer *pMaxT7;
   MapLayer *pMinT8;
   MapLayer *pMaxT8;
   MapLayer *pMinT9;
   MapLayer *pMaxT9;
   MapLayer *pMinT10;
   MapLayer *pMaxT10;
   MapLayer *pMinT11;
   MapLayer *pMaxT11;
   MapLayer *pMinT12;
   MapLayer *pMaxT12;
  
   MET_GRID_INFO( void ) : pMinT1(NULL), pMaxT1(NULL), pMinT2(NULL), pMaxT2(NULL), pMinT3(NULL),
      pMaxT3(NULL), pMinT4(NULL), pMaxT4(NULL), pMinT5(NULL), pMaxT5(NULL), pMinT6(NULL), pMaxT6(NULL), 
      pMinT7(NULL), pMaxT7(NULL), pMinT8(NULL), pMaxT8(NULL), pMinT9(NULL), pMaxT9(NULL), 
      pMinT10(NULL), pMaxT10(NULL), pMinT11(NULL), pMaxT11(NULL), pMinT12(NULL), pMaxT12(NULL)  
   { }
   };

class MetGridInfoArray : public CArray< MET_GRID_INFO*, MET_GRID_INFO* >
   {
   public:
      // destructor cleans up any allocated structures
     ~MetGridInfoArray ( void ) { for ( int i=0; i < GetSize(); i++ ) delete GetAt( i ); }      
   };

struct CELL_INFO
   {
   UPSLOPE_INFO *pUpslope;

   CELL_INFO( void ) : pUpslope( NULL ) { }
   };

struct PESTICIDE_MASS_BALANCE_INFO
   { 
   CString pesticideName;
   float inVeg;
   float inSurf;
   float transSurf;
   float transRoot;
   float transSat;
   float transVeg;
   float upSurf;
   float upRoot;
   float runoffSurf;
   float erodeSurf;
   float cumOut;//discharge of pesticide from the stream

   PESTICIDE_MASS_BALANCE_INFO( void ) : pesticideName(_T("")),inVeg(0.0f),inSurf(0.0f), transSurf(0.0f),transRoot(0.0f),transSat(0.0f),
      upSurf(0.0f), upRoot(0.0f), runoffSurf(0.0f), erodeSurf(0.0f), cumOut(0.0f), transVeg(0.0f)  { }
   };

class PesticideMassBalanceArray : public CArray< PESTICIDE_MASS_BALANCE_INFO*, PESTICIDE_MASS_BALANCE_INFO* >
   {
   public:
      // destructor cleans up any allocated structures
     ~PesticideMassBalanceArray( void ) { for ( int i=0; i < GetSize(); i++ ) delete GetAt( i ); }      
   };

struct MASS_BALANCE_INFO
	{
	float rainInputVolume;
	float initChannelVolume;
	float initSoilVolume;
	float finalSoilVolume;
   float finalPondedVolume;
	float finalChannelVolume;
	float streamflowVolume;
	float etVolume;
   float groundLossVol;
   float initialTracerMass;
   float finalTracerMass;
   float tracerInputMass;
   float tracerOutputMass;
   float sedimentOutputMass;
   float r2;
   float rmse1;
	float ns1;
   float rmse2;
	float ns2;
   float rmse3;
	float ns3;
   float rmseMulti;
	float nsMulti;
   float perNewWater;
   float freqSat;
   float areaOver;
   float d;
   float hrt;
   float catchmentMrt;
   float upslopeMeanMrt;
   float upslopeMinMrt;
   float upslopeMaxMrt;

   MASS_BALANCE_INFO( void ) : rainInputVolume(0.0f), initChannelVolume(0.0f), initSoilVolume(0.0f),
      finalPondedVolume(0.0f), finalChannelVolume(0.0f), streamflowVolume(0.0f), etVolume(0.0f),
      initialTracerMass(0.0f), finalTracerMass(0.0f),tracerInputMass(0.0f),tracerOutputMass(0.0f),
      sedimentOutputMass(0.0f), rmse1(0.0f), ns1(0.0f), rmse2(0.0f), ns2(0.0f), 
      rmse3(0.0f), ns3(0.0f), rmseMulti(0.0f), nsMulti(0.0f),  perNewWater(0.0f), 
      freqSat(0.0f), areaOver(0.0f), d(0.0f), r2(0.0f), hrt(0.0f), catchmentMrt(0.0f), upslopeMeanMrt(0.0f),upslopeMinMrt(1E6f),upslopeMaxMrt(0.0f)
      {}
   
	};




class CellInfoArray : public CArray< CELL_INFO*, CELL_INFO* >
   {
   public:
      ~CellInfoArray( void ) { Clear(); }

      void Clear( void ) {for ( int i=0; i < GetSize(); i++) delete GetAt( i ); RemoveAll(); }
   };

class ResultDataArray : public CArray< DataObj**, DataObj** >
   {
   public:
      // destructor cleans up any allocated structures
      ~ResultDataArray( void ) {Clear();}
      void Clear(void){
        for ( int i=0; i < GetSize(); i++ )

               delete GetAt(i); 
         RemoveAll(); 
        }      
   };

class WET_Hydro  
{
public:
	WET_Hydro();
	virtual ~WET_Hydro();

   int LoadDpaData( LPCTSTR filename );
   void ReverseByteOrder(short *in_array,int arraysize);

   RandUniform m_randUnif1;
   RandUniform m_randUnif2;
   RandUniform m_randUnif3;

   void  SetMapPtr( Map *pMap ) { m_pMap = pMap; }
   int   LoadClimateData( LPCSTR filename );
   int   LoadSoilData( LPCSTR filename );
   int   LoadMeasuredDischargeData( LPCSTR filename );
	int   LoadMeasuredEtData( LPCSTR  filename );
   int   LoadTracerInputData( LPCSTR  filename );
   int   LoadAuxillaryModelData( LPCSTR filename );
	int   LoadSyntheticqData( LPCSTR filename );
	int   LoadParameters( LPCSTR filename );
	int   SaveParameters( LPCSTR filename );
	int   SaveData( int runNumber );
   void  InitUpslopeDataCollection();
   void  InitInstreamDataCollection();
   void  GetStateVarCount();
   void  AllocateStateVarArray();
   void  InitMovieCapture();

   void  SetTimeStep( double timeStep ) { m_timeStep = timeStep; }
	void  SetDeltaX( float deltaX ) { m_deltaX = deltaX; }
	//void  SetWDRatio(float wdRatio) { m_wdRatio = wdRatio; }
   double GetTimeStep( void ) { return m_timeStep; }

	void  SetStartTime( double startTime ) { m_startTime = startTime; }
   void  SetStopTime( double stopTime ) { m_stopTime = stopTime; }
   double GetStopTime( void ) { return m_stopTime; }

   void  Run( );
	void  RunSynthetic(  );
	void  CalcInitialConditions( double endCalib,  bool save=true );
	int   CalcElevSurface( void );



   //Methods translating maplayer data into model configurations
	void  CalcWatershedInfo(  );
   void  CreateSemiDistributed();
   void  CreateHillslopeRiparian();
   void  CreateSimpleDistributed();
   void  CreateGeomorphologic();
   void  CreateGeoHillRip();
   void  CreateSubnodeArray();
   void  BuildTopology();
   void ResetVariableValues();
///////////////////////////////////////////////////
   
   UPSLOPE_INFO *GetUpslopeInfoFromRowCol(int row,int col);
   void ApplyTracerToGridCell(int row, int col);

	float CalculateTotalArea( void );

   float CalcDInstreamTracer_dt(ReachNode *pNode, int i, float tracerInUpslope);
   float CalcDInstreamPesticide_dt(ReachNode *pNode, int i, float pesticideInUpslope, int j);
   float CalcDUplandTracer_dt(UPSLOPE_INFO *pUpslope);
   float CalcDUnsatTracer_dt(UPSLOPE_INFO *pUpslope);
   float CalcDInstreamSediment_dt(ReachNode *pNode, int i);
   float CalcDUplandSediment_dt(UPSLOPE_INFO *pUpslope);
   float CalcDVolume_dt(UPSLOPE_INFO *pUpslope);
   float CalcDVolumeUnsat_dt(UPSLOPE_INFO *pUpslope);
   float CalcDSWE_dt(UPSLOPE_INFO *pUpslope);
   float CalcDVegVolume_dt(UPSLOPE_INFO *pUpslope); 
   float CalcDTill_dt(UPSLOPE_INFO *pUpslope);

   float CalcDPlantPesticide_dt(UPSLOPE_INFO *pUpslope,int i);
   float CalcDSurfacePesticideSoil_dt(UPSLOPE_INFO *pUpslope,int i);
   float CalcDSurfacePesticideWater_dt(UPSLOPE_INFO *pUpslope,int i);
   float CalcDUnsatPesticideSoil_dt(UPSLOPE_INFO *pUpslope,int i);
   float CalcDUnsatPesticideWater_dt(UPSLOPE_INFO *pUpslope,int i);
   float CalcDSatPesticideSoil_dt(UPSLOPE_INFO *pUpslope,int i);
   float CalcDSatPesticideWater_dt(UPSLOPE_INFO *pUpslope,int i);

   void UpdatePesticideParameters(PESTICIDE_INFO *pPesticide,UPSLOPE_INFO *pUpslope);
   void UpdatePesticideMassBalance(UPSLOPE_INFO *pUpslope);

	static void EstimateParams(float x, float a[], int ia[], float *y, float dyda[], int na, long extra, int init );
	void CalculateParameterDerivatives( float z, float a[], float *y, float dyda[], int na);
	
	void ProcessWETCells( void );

	void UpdateDeltaX();

   MapLayer *SetStreamLayer( LPCSTR shapeFileName );
   MapLayer *SetCellLayer( LPCSTR shapeFileName );
	MapLayer *SetDEMLayer( LPCSTR shapeFileName );
   MapLayer *SetWatershedLayer(LPCSTR filename);
   MapLayer *SetFDLayer(LPCSTR filename);
   MapLayer *FlowDirection( MapLayer *pDEMLayer);
   float m_tempPrimet;
	//ScatterWnd *CreateScatter( DataObj *pScatterData);
   MapLayer *GetMonthlyGrid(int month, int minOrMax);
	void SaveData();
	void SaveState( int type ); //  Type represents the reason behind the save.  See implem. for more details
	void RestoreState( int type );
	void PlotOutput();
   SavedDataArray m_savedDataArray;
	bool SaveToFile(LPCSTR filename, bool append=false );

	ResultDataArray m_resultDataArray;
	DataStore  *m_pDataStore; 
	SAVED_STATE_INFO *m_pSavedData;
	SAVED_STATE_INFO *m_pInitialConditions;

	int GetFieldColFromInterfaceValue(int model=1);
   int CopyStateFromReachTreeToMapLayer(int model, int stateVar);

protected:
   

   // various map layers
public:
   Map *m_pMap;
   MapLayer *m_pDEMLayer;
   MapLayer *m_pWatershedLayer;
   MapLayer *m_pFlowDirLayer;
   MapLayer *m_pStreamLayer;
   MapLayer *m_pCellLayer;
	int m_runNumber;
	int m_optNumber;
   ReachTree m_reachTree;
	double m_time;
   double m_timeStep;
   double m_curTimeStep;
   int   m_timeOffset;
	float  m_deltaX;
   double m_stopTime;
   double m_stopTimeHolder;
	double m_startTime;
	float **m_output;
	float m_tracerAppLagTime;

   int m_collectDataCount;
   float m_areaTotal;

   CString m_dataToMap;
   DYNAMIC_MAP_INFO *m_pDynamicMapInfo;
   void  UpdateParameterVector();
protected:
   MapLayer *m_pSurfaceWaterLayer;
   MapLayer *m_pSubSurfaceWaterLayer;

   // tree to contain reach information
   CellInfoArray       m_cellInfoArray;

   // simulation control

   float m_cellSize;
   float m_maxReachDeltaX;

   // miscellaneous
   float m_noDataValue;  // for all constructed layers
   float m_surfaceWaterThreshold;  // mm  
   ROW_COL m_pourPoint;     // col/row
   float m_gridResolution;

public:
	// model parameters available to the optimization routines
   float m_beta;                   // surface roughness???
   float m_ddExp;                  // depth/discharge exponent (see Bedient and Huber, 1992,  pg 276)
	float m_et;
   float m_stationLtPrecip;
	float m_collectionInterval;
   int m_spinUp;
   float m_initTracerConc;
   float m_bufferWidth;
	BOOL m_updateStream;
   BOOL m_updateUpland;
   BOOL m_checkSideOfStream;
   BOOL m_checkUseSoilList ;
   BOOL m_checkMassBalance;
   int m_routeChannelWaterMethod;
   BOOL m_checkSimulateWetlands;
   BOOL m_checkThreeBasins;
   BOOL m_checkUnsaturated;
   BOOL m_checkSnow;
   BOOL m_checkGridMet;
   BOOL m_radar;
   BOOL m_checkDistributeInitCond;

   //Tracer Interface
   float m_editTracerMgL;
   int m_radioTracerInput;
   BOOL m_checkTracerModel;
   BOOL m_checkPesticide;
   BOOL m_checkTill;
   BOOL m_pesticideApp;  //flag to indicate that a pesticide application is currently happening
   void GetPesticideAppRateSimple();
   void GetPesticideAppRate();
   void GetPesticideAppTiming();
   CUIntArray m_pesticideOffsets;

	int  m_streamDataToCollect;  //interface code
	int  m_cellDataToCollect;   //interface code
   BOOL m_checkPrism;

	float m_maxCentroidDistance;
	int   m_numBands;
	float m_widthBand;

   int  m_modelStructureMethod; //how the model is to be run (cells, watersheds, or distributed)
	int  m_modelParamMethod ; 
	BOOL m_ksAsParam;
	BOOL m_sdAsParam;
   
   int m_etMethod;
   int m_satMethod;
   int m_unsatMethod;
   float m_drainShape;
   BOOL m_checkEtModel;
   BOOL m_radioPorosity;
   BOOL m_checkInterception;
   BOOL m_checkSoilDepthMap;
   int  m_routeChannelSolutesMethod;

   BOOL m_checkUplandSediment;
//	BOOL m_checkReduceSediment;
   BOOL m_checkApplySedOptions;
   BOOL m_saveEmfOutput;
   BOOL m_checkIncrFlow;
   int m_checkMonteCarlo;
   int m_numIterations;
   CTime m_incrFlowBegin;
   CTime m_incrFlowEnd;
   float m_incrFlowPercent;
	float m_reduceSedPercent;
   BOOL  m_checkModelTileDrains;
   float m_tileOutflowCoef;
   float m_tileThreshold;

   int m_numberPesticides;

   float m_latitude;
	float m_ac ;
	float m_airDensity ;
	float m_bc ;
	float m_bioGeoAbsorp ;
	float m_extraTerrRadiation ;
	float m_clearFraction ;
	float m_cloudFraction ;
	float m_gradientSatVapor ;
	float m_heatCond ;
	float m_heightOfHumid ;
	float m_heightOfWind ;
	float m_shortWave ;
	float m_latentHeatEvap ;
	float m_psychrometric ;
	float m_relativeHumid ;
	float m_satVapPres ;
	float m_stephanBoltz ;
	float m_specificHeat ;
	float m_storedE ;
	float m_windSpeed ;

   float m_rtIntegral;


public:
   // data storage
   FDataObj *m_pClimateData;
   FDataObj *m_pMeasuredDischargeData;
	FDataObj *m_pMeasuredEtData;
   FDataObj *m_pMeasuredTracerInputData;
   FDataObj *m_pTracerData;
   FDataObj *m_pSwcData;
   FDataObj *m_pRateData;
   FDataObj *m_pPestVegData;
   FDataObj *m_pPestInstreamData;

   FDataObj *m_pPestSurfData;
   FDataObj *m_pPestUnsatData;
   FDataObj *m_pPestSatData;
   FDataObj *m_pPestMassBalanceData;
   FDataObj *m_pTracerConcData;
   FDataObj *m_pTracerConcChannelData;
   FDataObj *m_pTracerMassChannelData;
	FDataObj *m_pModelClimateData;
   FDataObj *m_pETData;
   FDataObj *m_pInstreamData;
   FDataObj *m_pGeneralData;
   FDataObj *m_pMassBalanceData;
   FDataObj *m_pUplandStorageData;
	FDataObj *m_pSyntheticqData;
	FDataObj *m_pReachStatistics;
   FDataObj *m_pMonteCarloResults;
   FDataObj *m_pMonteCarloTracerTimeSeries;
   FDataObj *m_pMonteCarloQTimeSeries;
   FDataObj *m_pInstreamSedimentData;
   FDataObj *m_pUplandSedimentData;
   FDataObj *m_pTotalSedimentOutflowData;
   FDataObj *m_pSedimentOutflowData;
   FDataObj *m_pUnsatDepthData;
   FDataObj *m_pSatDepthData;
   FDataObj *m_pSatDeficitData;

   ReachInfoHydroArray m_reachInfoHydroArray;
   MASS_BALANCE_INFO m_massBalanceInfo;
//   void UpdateMassBalance();
   float CalcDPrecipVol_dt(UPSLOPE_INFO *pUpslope);
   float CalcDEtVol_dt(UPSLOPE_INFO *pUpslope);
   float CalcDGroundVol_dt(UPSLOPE_INFO *pUpslope); 
   float CalcDTracerMassInPrecip_dt(UPSLOPE_INFO *pUpslope);
   float CalcDPesticideMassInput_dt(UPSLOPE_INFO *pUpslope, int i);

   float CalcInitChannelVol();
   void CalcInitSoilVol(float &initSoilVol, float &initTracerMass);
   bool CalcFinalChannelVol(float &finalChannelVol, float &finalChannelTracerMass);
   float CalcFinalSoilVol(float &finalSoilVol, float &finalPondedVol, float &finalTracerMass);
   void CalcCumulativeUplandVolume(float &cumETVol, float &cumPrecipVol, float &cumTracerTracerMassInPrecip, float &cumGroundLoss);
   void CalcCumulativeChannelVolume(float &cumOutflowVol,float &cumTracerOutFlowMass, float &cumSedimentOutflowMass);
   void CalcCumulativeUplandPest();
   void CalcCumulativeInstreamPest();
   //  Landuse Change affects on water quality and hydrology
   void RestoreToModelStructure();
   void ApplyRestorationOptions();
   void ApplyStructuralRestorationOptions();

public:
	CWordArray m_cellDataArray;         // cell offsets to collect data on
   
   //CollectDataCellOffsetArray m_cellDataArray;
	CWordArray m_reachDataArray;        // reach offsets to collect data on

	CUIntArray m_cellStreamIndexArray;
	StreamCellIndexArray m_streamCellIndexArray;
   FPDistributionArray m_fpDistributionArray;
   FPDistributionArray m_fpDistributionArrayStream;
   MET_GRID_INFO m_metGridInfo;
 

   ChemicalParamInfoArray m_chemicalParamInfoArray;
   ChemicalAppInfoArray   m_chemicalAppInfoArray;

   HydrologicParamInfoArray  m_hydrologicParamInfoArray;
   InstreamHydroParamInfoArray m_instreamHydroParamInfoArray;
   void UpdatePesticideConcentration(UPSLOPE_INFO *pUpslope);

protected:
   // soils information
   SoilInfoArray m_soilInfoArray;
   AuxillaryModelInfoArray m_auxillaryModelInfoArray;
   GoodnessOfFitInfoArray m_goodnessOfFitInfoArray;
   PesticideMassBalanceArray m_pesticideMassBalanceArray;


public:
   int m_colArea;
   int m_colSlope;
   int m_colLengthSlope;
   int m_colZ;
   int m_colWatershedID;
   int m_colStreamID;
   int m_colAbbrev;
   int m_colLulc;
   int m_colLulc_b;
   int m_colSoilDepth;
   int m_colIsRiparian;
	int m_colCentroid;
	int m_colBuffDist;
   int m_colSideOfStream;
   int m_colLtPrecip;
   int m_colOCarbon;
   int m_colKffact;
   int m_colAltLulc;
   int m_colAltArea;
   int m_colAltern;
   int m_colGeoCode;

   BOOL m_checkCStruc;
   BOOL m_checkCEq;
   BOOL m_checkCM;
   BOOL m_checkCKsat;
   BOOL m_checkCInit;
   BOOL m_checkCPhi;
   BOOL m_checkCFC;
   BOOL m_checkCSD;
   BOOL m_checkCKD;
   BOOL m_checkCVan;
   BOOL m_checkCK1;
   BOOL m_checkCK2;
   BOOL m_checkCWD;
   BOOL m_checkCN;
   BOOL m_CEditNumIter;
   BOOL m_CNumIterText;
   BOOL m_checkWP;
   BOOL m_checkPLE2;
   
   // integration support
   BOOL   m_useVariableStep;
   double m_rkvMaxTimeStep; 
   double m_rkvMinTimeStep;
   double m_rkvTolerance;

protected:
   double *m_k1;
   double *m_k2;
   double *m_k3;
   double *m_k4;
   double *m_k5;
   double *m_k6;
   double *m_derivative;
   double *m_svTemp;
   float **m_stateVar;
   float **m_stateVarStream;
   double *m_surfaceFlow;
public:
	int m_svCount;
   int m_svCountStream;
   int m_uplandUnitCount;
   float m_simulationTime;

  void LoadDistributedAirTemp(); 
     REACH_INFO_HYDRO *GetReachInfoHydroFromReachID( int reachID );
protected:
   int   GetReachCount();


	float GetKinematicWaveCelerityFromDepth( ReachNode *pNode , float depth);
   int   GetCellCount();
   int   GetCellReachID( int cell );
   float GetCellArea( int cell );
   float GetCellSlope( int cell );
   float GetCellDepth( int cell );
   int   GetCellLulc( int cell );
   int   GetCellLulc_b( int cell );
	float GetCellCentroidDistance( int cell );
   float GetCellSoilDepth( int cell );
   int   GetCellIsRiparian( int cell );
	float GetCellBufferDistance( int cell );
   int   GetCellSideOfStream( int cell );
   float GetCellLtPrecip( int cell );
   float GetCellOCarbon( int cell );
   float GetCellLengthSlope( int cell );
   float GetCellKffact( int cell );
   int   GetCellAltLulc( int cell );
   float GetCellAltArea( int cell );
   int   GetCellAltern( int cell );

   UPSLOPE_INFO *GetCellUpslopeInfo( int cell );

   void CalcGridFlowFractions();
   void CalcCellFlowFractions( int row, int col, bool useSurface );

   // upslope model stuff
   int   RunUpslopeModel( double time );
   float GetSurfaceInOutFlows( int row, int col, float &totalInflow, float &totalOutflow );
   void  GetSurfaceFlowFractions( int row, int col, float inflowFraction[], float outflowFraction[] );
   float CalcET ( float precip, float temp );
   float PenmanMonteith(float T);
   float CalcHoltanF( float awa );
   float CalcSW( float precip, float temp, int datejul, int i  );
   int GetDateJulFromTime(float time);
   void SimulateET( int method, UPSLOPE_INFO *pUpslope);
   // instream model stuff

   int   RunInstreamModel( double time );
   bool  SolveReach( ReachNode *pNode );
   bool  SolveReachSolute( ReachNode *pNode );
   bool  SolveReachPesticide( ReachNode *pNode );

   // model setup, etc.
   void  InitRun( void ) ;
   void  CollectData( double time );
   void  CollectDataSingleRun( );
   void  CalcInitGridTracerMass();
   void  CalcInstreamResidenceTime();
   void  CalcUpslopeResidenceTime();
   void CollectDataGrid();
   void CollectDataSemi(); 
   void CollectDataInstream();

   UPSLOPE_INFO *CollectDataGetUpslope(int i, int &count);
   UPSLOPE_INFO *CollectDataGetGridUpslope(int i, int &count );
   UPSLOPE_INFO *CollectDataGetSemiUpslope(int i );

   

   int CollectGetNumGrid(void);
   int CollectGetNumInstream(void);
   int CollectGetNumSemi(void);
   


   void  CollectDataUpslopeWater(  int dataToCollect );
   void  CollectDataUpslopeTracer(   int dataToCollect );
   void  CollectDataUpslopePesticide(   int dataToCollect );
   void  CollectDataInstreamWater(  int dataToCollect );
   void  CollectDataInstreamTracer(   int dataToCollect );
   void  CollectDataInstreamPesticide(   int dataToCollect );
   void  CollectDataMassBalance(  );

   void  CollectMonteCarloData(double time);
   void  EndRun( );
   void  GetGoodnessOfFitValues();
   float GetDepthFromQ( ReachNode *pNode, float Q, float wdRatio );   // note: this function is closely coupled with SetReachGeometry
   float GetVolumeFromQ( ReachNode *pNode, float Q, float wdRatio);
   void  SetReachGeometry( ReachNode *pNode, int node, float wdRatio );
   SOIL_INFO *GetSoilParameters( int cell );
   AUXILLARY_MODEL_INFO * GetAuxillaryModelParameters( int cell);

   bool  IntRKF( double curTime, int svCount, float **stateVar, bool modelToIntegrate );
   void  GetDerivatives(  double *derivative, double curTime );
   void  GetInstreamDerivatives(  double *derivativeInstream, double curTime );
   void  UpdateAuxillaryVars();
   void  UpdateInstreamAuxillaryVars();
   void  RunAuxillaryInstreamModel(double time);


   //  After integrating, these methods calculate auxillary values based on volumes
   void  CalcSatDeficit(UPSLOPE_INFO *pUpslope) ;  
 
   float GetDegSatFromSWC( UPSLOPE_INFO *pUpslope );
   void  CalcStorage(UPSLOPE_INFO *pUpslope);
   void  CalcVoidVolume(UPSLOPE_INFO *pUpslope);
   void  CalcVanGenuctenKTheta(UPSLOPE_INFO *pUpslope);
   void  CalcBrooksCoreyKTheta(UPSLOPE_INFO *pUpslope);
   void  CalcSatZone(UPSLOPE_INFO *pUpslope) ; 
   void  CalcUnsatZone(UPSLOPE_INFO *pUpslope) ; 
   void  CalcKSat(UPSLOPE_INFO *pUpslope) ; 
   void  CalcAccountForZeroArea(UPSLOPE_INFO *pUpslope) ; 
   void  CalcPondedVolume(UPSLOPE_INFO *pUpslope) ; 
   void  UpdateGridSlope(UPSLOPE_INFO *pUpslope);
   void  CalcD8Flux(UPSLOPE_INFO *pUpslope);
   void UpdatePesticideWaterVolume(UPSLOPE_INFO *pUpslope);
   
   void GetUpslopeMetData(UPSLOPE_INFO *pUpslope);

protected:
   WH_NOTIFYPROC m_notifyFn;
   long m_extra;
   long m_extra1;
   bool Notify( WH_NOTIFY_TYPE t, float time, float timeStep, long l, long l1) { if ( m_notifyFn != NULL ) return m_notifyFn( this, t, (float)time, (float)timeStep , m_extra , m_extra1); else return true; }

public:
   WH_NOTIFYPROC InstallNotifyHandler( WH_NOTIFYPROC p, long extra=0L ) { WH_NOTIFYPROC temp = m_notifyFn; m_notifyFn = p; m_extra = extra; return temp; }
/*   int   GetUpslopeDerivatives( double *derivative , double curTime);*/
int GetNeighbors(int colToSubset, LPCSTR filename1 , LPCSTR filename2, LPCSTR filename3,LPCSTR filename4);

   // movie stuff follows
   CAVIGenerator** AVIGenerators;
   struct AuxMovieStuff* auxMovieStuff;
   int numMovies, movieFrameRate;

   void WriteFrame();

   int GetCellGeoCode(int cell);
   int CreateLumped(void);
   int CreateFPGrid(void);

};

// this is all just for precalculating and storing things to avoid doing it per-frame
struct AuxMovieStuff
{
   CDC bitmapDC;
   CBitmap bitmap;
   BITMAPINFO bi;
   LPBITMAPINFO lpbi;
   BYTE* bitAddress;
};


#endif // !defined(AFX_WET_Hydro_H__C9203D32_B4B1_11D3_95C1_00A076B0010A__INCLUDED_)

