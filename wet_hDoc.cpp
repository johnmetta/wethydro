// wet_hDoc.cpp : implementation of the CWet_hDoc class
//

#include "stdafx.h"
#pragma hdrstop

#include "wet_hDoc.h"


#include <Wet_hView.h>
#include <mainfrm.h>

#include "maplayer.h"
#include "map.h"
#include "ModelDefinition.h"
#include "ModelEvaluation.h"
#include "WizUserInteraction.h"
#include "Extensions.h"
#include "Restoration.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CWet_hDoc  *gpDoc;
extern MapWnd       *gpMapWnd;
extern CWet_hView *gpView;
extern MapLayer     *gpCellLayer;
extern MapLayer    *gpStreamLayer;
extern CMainFrame   *gpMain;
extern ReachTree    *gpReachTree;
extern WET_Hydro *gpHydroModel;
extern WizUserInteraction *gpWizUserInteraction;
extern ModelDefinition  *gpModelDefinition;
extern ModelEvaluation  *gpModelEvaluation;
extern Extensions       *gpExtensions;
extern Restoration      *gpRestoration;

/////////////////////////////////////////////////////////////////////////////
// CWet_hDoc

IMPLEMENT_DYNCREATE(CWet_hDoc, CDocument)

BEGIN_MESSAGE_MAP(CWet_hDoc, CDocument)
	//{{AFX_MSG_MAP(CWet_hDoc)
	ON_COMMAND(ID_PROCESSCELL_GO, OnProcesscellGo)

	ON_COMMAND(ID_FILE_SAVEPARAMETERS, OnFileSaveparameters)
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWet_hDoc construction/destruction

CWet_hDoc::CWet_hDoc()
:    m_currentRun( 0 ),
 
     m_evalsCompleted(0) // flags indicating wich evaluative model results are available

{
   gpDoc = this;  // initially
   LoadLulcInfo( "c:\\temp\\lulc.dat" );


   m_fieldInfoArray.AddFieldInfo( "ABBREV",    0, "Soil Type",   TYPE_INT, BCF_GRAY ,0.0f,0.0f  );
	m_fieldInfoArray.AddFieldInfo( "HYDRO_ID",  0, "Sub-Basins (classified continously)", TYPE_FLOAT, BCF_MIXED,0.0f,0.0f);
	m_fieldInfoArray.AddFieldInfo( "DEPMAX",    0, "Soil Depth", TYPE_FLOAT, BCF_BLUE,0.0f,0.0f);
   m_fieldInfoArray.AddFieldInfo( "SLOPE",     0, "Max Cell Slope", TYPE_FLOAT, BCF_GRAY,0.0f,0.0f);	
	m_fieldInfoArray.AddFieldInfo( "SWC",       0, "Soil Water Content", TYPE_FLOAT, BCF_GRAY,0.0f,0.0f);
	m_fieldInfoArray.AddFieldInfo( "CENTROID",  0, "Flow Path Distance", TYPE_FLOAT, BCF_GRAY,0.0f,0.0f);
	m_fieldInfoArray.AddFieldInfo( "PHI",       0, "Phi", TYPE_FLOAT, BCF_GRAY,0.0f,0.0f);
	m_fieldInfoArray.AddFieldInfo( "KSAT",      0, "Conductivity", TYPE_FLOAT, BCF_GRAY,0.0f,0.0f);
	m_fieldInfoArray.AddFieldInfo( "Z",         0, "Depth", TYPE_FLOAT, BCF_GRAY,0.0f,0.0f);
	m_fieldInfoArray.AddFieldInfo( "AREA",      0, "Area", TYPE_FLOAT, BCF_GRAY,0.0f,0.0f);
	m_fieldInfoArray.AddFieldInfo( "MAXFPDIST", 0, "Max Flow Path Distance", TYPE_FLOAT, BCF_GRAY,0.0f,0.0f);
	m_fieldInfoArray.AddFieldInfo( "MINFPDIST", 0, "Min Flow Path Distance", TYPE_FLOAT, BCF_GRAY,0.0f,0.0f);
	m_fieldInfoArray.AddFieldInfo( "LTPREC",    0, "Long Term Precip (PRISM)", TYPE_FLOAT, BCF_GRAY,0.0f,0.0f);
	m_fieldInfoArray.AddFieldInfo( "BUFFDIST",  0, "Buffered Distance", TYPE_FLOAT, BCF_GRAY,0.0f,0.0f);

	m_fieldInfoArray.AddFieldInfo( "EXTRA_1", 1, "Upland Identifier"    , TYPE_INT,  BCF_MIXED,0,0);	
	m_fieldInfoArray.AddFieldInfo( "EXTRA_1", 1, "Upslope UPSLOPE_INFO" , TYPE_INT,  BCF_MIXED,0,0);
	m_fieldInfoArray.AddFieldInfo( "EXTRA_1", 1, "Upslope Area"         , TYPE_FLOAT,BCF_MIXED,0,0);
	m_fieldInfoArray.AddFieldInfo( "EXTRA_1", 1, "Phi"                  , TYPE_FLOAT,  BCF_MIXED,0.2f,0.6f);
	m_fieldInfoArray.AddFieldInfo( "EXTRA_1", 1, "Upland Depth"         , TYPE_FLOAT,BCF_MIXED,0.0f,2.0f);
	m_fieldInfoArray.AddFieldInfo( "EXTRA_1", 1, "Upland Slope"         , TYPE_FLOAT,BCF_MIXED,0,0);
	m_fieldInfoArray.AddFieldInfo( "EXTRA_1", 1, "Min Flowpath Distance", TYPE_FLOAT,BCF_MIXED,0,0);
	m_fieldInfoArray.AddFieldInfo( "EXTRA_1", 1, "Max Flow Path Dist"   , TYPE_FLOAT,BCF_MIXED,0,0);
	m_fieldInfoArray.AddFieldInfo( "EXTRA_1", 1, "ET"                   , TYPE_INT,  BCF_GRAY ,0.0f,25.0f);	//mm/hr
	m_fieldInfoArray.AddFieldInfo( "EXTRA_1", 1, "Air Temp"             , TYPE_FLOAT,BCF_GRAY ,-5.0f,40.0f);  // degrees c
	m_fieldInfoArray.AddFieldInfo( "EXTRA_1", 1, "Unsat SWC"            , TYPE_FLOAT,BCF_GRAY ,0.1f,0.4f);
	m_fieldInfoArray.AddFieldInfo( "EXTRA_1", 1, "Saturated Depth"      , TYPE_INT,  BCF_GRAY ,50.f,800.f); //mm
	m_fieldInfoArray.AddFieldInfo( "EXTRA_1", 1, "Saturation Deficit"   , TYPE_FLOAT,BCF_GRAY ,50.0f,300.f);
	m_fieldInfoArray.AddFieldInfo( "EXTRA_1", 1, "Unsaturated Depth"    , TYPE_FLOAT,BCF_GRAY ,0.100f,0.40f);
	m_fieldInfoArray.AddFieldInfo( "EXTRA_1", 1, "Tracer Concentration" , TYPE_FLOAT,BCF_GRAY ,0.0f,50.0f);
	m_fieldInfoArray.AddFieldInfo( "EXTRA_1", 1, "Sediment Output"      , TYPE_FLOAT,BCF_GRAY ,0,0);
	m_fieldInfoArray.AddFieldInfo( "EXTRA_1", 1, "Foliar Runoff Rate"   , TYPE_FLOAT,BCF_GRAY ,0.0f,1.0f);
	m_fieldInfoArray.AddFieldInfo( "EXTRA_1", 1, "Surf Pest"            , TYPE_FLOAT,BCF_GRAY ,0.0f,100.0f);
	m_fieldInfoArray.AddFieldInfo( "EXTRA_1", 1, "Unsat Pest"           , TYPE_FLOAT,BCF_GRAY ,0.0f,8.0f);
	m_fieldInfoArray.AddFieldInfo( "EXTRA_1", 1, "Sat Pest"             , TYPE_FLOAT,BCF_GRAY ,0.0f,1.0f);
	m_fieldInfoArray.AddFieldInfo( "EXTRA_1", 1, "Sat Pest Rate"        , TYPE_FLOAT,BCF_GRAY ,0.0f,0.010f);
	}

		

CWet_hDoc::~CWet_hDoc()
{
   gpDoc = NULL;
}


BOOL CWet_hDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
   
   if ( gpWizUserInteraction != NULL )  // move to OnCLoseDocument()?????
      delete gpWizUserInteraction;

   gpWizUserInteraction = NULL;

   if ( gpModelDefinition != NULL )  // move to OnCLoseDocument()?????
      delete gpModelDefinition;

   gpModelDefinition = NULL;

   if ( gpModelEvaluation != NULL )  // move to OnCLoseDocument()?????
      delete gpModelEvaluation;

   gpModelEvaluation = NULL;



	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CWet_hDoc serialization

void CWet_hDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CWet_hDoc diagnostics

#ifdef _DEBUG
void CWet_hDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CWet_hDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWet_hDoc commands

BOOL CWet_hDoc::OnOpenDocument(LPCTSTR lpszPathName) 
   {
   if (gpHydroModel != NULL)
      delete gpHydroModel;
   // create a hydro model
   gpHydroModel = new WET_Hydro;

   gpHydroModel->SetMapPtr(  gpMapWnd->m_pMap );


   gpDoc = this;

	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

//////////////////////




 //  gpHydroModel->LoadDpaData("c:\\research\\dpa\\ak\\PAJK_SDUS87_DPAACG_200301102025.x");




////////////

   ASSERT ( gpMapWnd != NULL );

   CWaitCursor c;
   // extract the name from the inifile
   CFileFind file;
   BOOL ok = file.FindFile( lpszPathName );
   ASSERT( ok );
   file.FindNextFile();
   m_name = file.GetFileTitle();

   FILE *fp = fopen( lpszPathName, "r" );
   if ( fp == NULL )
      {
      CString msg( "Unable to open file: ");
      msg  += lpszPathName;
      return FALSE;
      }

   char buffer[ 255 ];
   while( !feof( fp ) )
      {
      fgets( buffer, 255, fp );

      switch( buffer[ 0 ] )
         {
         case '%':
         case '\n':
         case ' ':
		 case '/':
            break;

         case '[':
            break;

         default: // assume a file spec
            {
            char _name[ 128 ], _path[ 128];
            int type, includeData, red, green, blue, extraCols, dataType;												// dataType 0 = Map
																																				// dataType 1 = climate
            int count = sscanf( buffer, "%[^,],%[^,],%i,%i,%i,%i,%i,%i,%i",											// dataType 2 = soil data	
                  _name, _path, &type, &includeData, &red, &green, &blue, &extraCols, &dataType );				// dataType 3 = measured discharge

            if ( count != 9 )
               {
               CString msg( "Error reading line: " );
               msg += buffer;
               msg += ", skipping...";
               ErrorMsg( msg );
               break;
               }

            CString name( _name );
            name.TrimLeft();
            name.TrimRight();

            CString path( _path );
            path.TrimLeft();
            path.TrimRight();

            // note: pFileName will point to a partial path (no root)  eg: basin/basin.jpg
            //       fileInfo will contain the label,  eg: South Santiam Basin
            //       path will be the fully qualified path/filename

				if ( dataType <= 0 )
					{
					MapLayer *pLayer = AddMapLayer( path, type, includeData, extraCols );

					if ( pLayer != NULL )      // map successfully added?
						{
						// add in any other relevent information
						pLayer->m_name = name; 
                  if (pLayer->GetType()==LT_POINT)
                     {
                    // pLayer->SetBins(0,100000,1);
                     pLayer->SetBinColorFlag(BCF_BLUE);
                  //   pLayer->ClassifyData();
                     }
                  
						pLayer->SetOutlineColor( RGB( red,green,blue ) );
						ASSERT ( gpMapWnd != NULL );
						Map *pMap = gpMapWnd->m_pMap;
						int i = pMap->GetLayerCount();
                
						if ( i == 1 )  // first map?
							{
							gpCellLayer = pLayer;

								// classify on LULC_C data;
							int col = pLayer->GetFieldCol( "LULC_A" );
							pLayer->SetActiveField( col );

							LulcNode *pNode = m_lulcTree.GetRootNode();     // get the root

							while ( ( pNode = m_lulcTree.GetNextNode() ) != NULL )
								{
								if ( pNode->GetNodeLevel() == 1 )
									{
									CString label;
									label.Format( "%s (%i)", pNode->m_name, pNode->m_id );

									pLayer->AddBin( pNode->m_color, label, BT_NUMERIC, pNode->m_id-0.1f, pNode->m_id+0.1f );
									}
								}
							}
						
					
			//         pLayer->AddNoDataBin();
				//		pLayer->ClassifyData();

						if ( includeData )
						   {
							ASSERT( pLayer->m_pDbTable != NULL );
							}

                  if ( dataType == -4 )   // buffer file?
                     pLayer->SetHatch( HS_DIAGCROSS, YELLOW );
						} //end of ( if pLayer != NULL )
					}// end of ( if dataType == 0)       

					else if ( dataType == 1 )
						gpHydroModel->LoadClimateData( path );	

					else if ( dataType == 2 )
						gpHydroModel->LoadSoilData( path );

					else if ( dataType == 3 )
						gpHydroModel->LoadMeasuredDischargeData( path );
					else if ( dataType == 4 )
						gpHydroModel->LoadMeasuredEtData( path );
               else if ( dataType == 5 )
                  gpHydroModel->LoadAuxillaryModelData(path);
					else if (dataType==6)
						gpHydroModel->LoadParameters(path);
               else if (dataType==7)
                  gpHydroModel->LoadTracerInputData( path );
					else
						{
						CString msg( "Error reading DataType in line: " );
						msg += buffer;
						ErrorMsg( msg );
						}

            }  // end of default:
         }  // end of: switch()
      }  // end of:  while()

   gpHydroModel->m_pDynamicMapInfo = new DYNAMIC_MAP_INFO;
   if (gpMapWnd->m_pMap->GetLayerCount() > 2)
      {
	   gpHydroModel->SetDEMLayer("DEM.dem");
      gpHydroModel->SetWatershedLayer("catch.cth");
      gpHydroModel->SetFDLayer("fd.fd");
      }


	gpHydroModel->SetCellLayer( "Cells.shp" );
	gpHydroModel->SetStreamLayer( "Stream.shp" );
	//gpStreamLayer = gpHydroModel->m_pStreamLayer;
	gpHydroModel->m_pStreamLayer->SetBinColorFlag( BCF_BLUE );
    gpHydroModel->m_pStreamLayer->SetBins( 0, 5, 5 );
    gpHydroModel->m_pStreamLayer->SetActiveField( gpHydroModel->m_pStreamLayer->GetFieldCol( "ORDER_" ) );
 //   gpHydroModel->m_pStreamLayer->ClassifyData();

    if ( gpMapWnd != NULL )
       gpMapWnd->RefreshList();




   if (gpWizUserInteraction != NULL)
      delete gpWizUserInteraction;
   gpWizUserInteraction = new WizUserInteraction( "Model Interaction" );
   gpWizUserInteraction->Create( gpMapWnd->m_pMap, WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME | WS_MAXIMIZEBOX );
   
   if (gpModelDefinition != NULL)
      delete gpModelDefinition;
   gpModelDefinition = new ModelDefinition( "Model Definition" );
   gpModelDefinition->Create( gpMapWnd->m_pMap, WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME | WS_MAXIMIZEBOX );
   
   if (gpModelEvaluation != NULL)
      delete gpModelEvaluation;
   gpModelEvaluation = new ModelEvaluation( "Model Evaluation" );
   gpModelEvaluation->Create( gpMapWnd->m_pMap, WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME | WS_MAXIMIZEBOX );

   if (gpExtensions != NULL)
      delete gpExtensions;
   gpExtensions = new Extensions( "Model Extensions" );
   gpExtensions->Create( gpMapWnd->m_pMap, WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME | WS_MAXIMIZEBOX );

   if (gpRestoration!= NULL)
      delete gpRestoration;
   gpRestoration = new Restoration( "Restoration" );
   gpRestoration->Create( gpMapWnd->m_pMap, WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME | WS_MAXIMIZEBOX );

   return TRUE;
   }


MapLayer *CWet_hDoc::AddMapLayer( LPCSTR dataset, int type, int includeData, int extraCols )
   {
   // add the layer to the map
   ASSERT ( gpMapWnd != NULL );
   ASSERT ( gpMapWnd->m_pMap != NULL );

   Map *pMap = gpMapWnd->m_pMap;

   VData::m_useWideChar = false;
/*
   switch ( type )
     {
      case 0:     // shape file
         return pMap->AddShapeLayer( dataset, includeData ? true : false );
         
      case 1:     // grid file
         MapLayer *pLayer = pMap->AddGridLayer( dataset,DOT_FLOAT, -1, 0); 
			pLayer->m_isVisible=true;
			return pLayer;

      }*/
   switch ( type )
      {
      case 0:     // shape file
         return pMap->AddShapeLayer( dataset, includeData ? true : false, extraCols );
         
      case 1:     // grid file
         return pMap->AddGridLayer( dataset,DOT_FLOAT, -1, 0 );  // load data as well...  
      
      case 2:
         return pMap->AddGridLayer(dataset, DOT_INT, -1, 0);
      }




   pMap->m_name = dataset;

   return NULL;
   }


void CWet_hDoc::OnFileLoadmap() 
   {
   CFileDialog dlg( TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
         "Shape files|*.shp|Grid Files|*.grd|All files|*.*||");

   if ( dlg.DoModal() == IDOK )
      {
      CString filename( dlg.GetPathName() );

      ASSERT ( gpMapWnd != NULL );
      ASSERT ( gpMapWnd->m_pMap != NULL );

      Map *pMap = gpMapWnd->m_pMap;

      CString ext = dlg.GetFileExt();

      MapLayer *pLayer = NULL;

      if ( ext.CompareNoCase( "shp" ) == 0 )
         pLayer = pMap->AddShapeLayer( filename, true );  // load data as well...

      else if ( ext.CompareNoCase( "grd" ) == 0 )
         pLayer = pMap->AddGridLayer( filename,DOT_FLOAT, -1 , 0);

      else
         WarningMsg( "Unsupported Map Type - Only Shape (*.shp) and Grid (*.grd) files are currently supported..." );

      // layer added succesfully?
      if ( pLayer != NULL )
         {
         //ASSERT ( gpMapWnd != NULL );
         //ASSERT ( gpMapWnd->m_pMapTree != NULL );
         //gpMapWnd->m_pMapTree->AddNode( pLayer );            
         //ASSERT ( gpMapWnd->m_pMapList != NULL );
         //gpMapWnd->m_pMapList->AddLayer( pLayer->m_name );
         
//         SetModifiedFlag( TRUE );    // let the framework now the internal doc data has been changed
         }

      if ( gpMapWnd != NULL )
         gpMapWnd->RefreshList();
      }
}


int CWet_hDoc::SelectWatershed( MapLayer *pCellLayer, int reachID, int colReachID )
   {
   pCellLayer->ClearQueryResults();

   // find the ReachNode with this id
   ReachNode *pNode = m_reachTree.GetReachNodeFromReachID( reachID );

   // no node found...
   if ( pNode == NULL )
      return -1;

   CArray< ReachNode*, ReachNode* > upstreamReachArray;

   FindUpstreamReaches( pNode, upstreamReachArray );

   int upstreamReachCount = upstreamReachArray.GetSize();
   if ( upstreamReachCount == 0 )
      return 0;

   // found all upstream reaches, select all polygons that have each rach as it id
   for ( int i=0; i < pCellLayer->GetRecordCount(); i++ )
      {
      int polyReachID;
      bool ok = pCellLayer->GetData( i, colReachID, polyReachID );

      if ( ! ok ) // field not found, so bail with an error code
         return -2;

      // see if this watershed id is in the upstream
      for ( int j=0; j < upstreamReachCount; j++ )
         {
         if ( upstreamReachArray[ j ]->m_reachInfo.reachID == polyReachID )
            {
            pCellLayer->AddQueryResult( i );
            break;
            }
         }
      }

   return pCellLayer->GetQueryCount();
   }


void CWet_hDoc::FindUpstreamReaches( ReachNode *pNode, CArray< ReachNode*, ReachNode* > &upstreamReachArray )
   {
   if (  pNode == NULL )
      return;

   upstreamReachArray.Add( pNode );

   FindUpstreamReaches( pNode->m_pLeft, upstreamReachArray );
   FindUpstreamReaches( pNode->m_pRight, upstreamReachArray );
   return;
   }

WET_FIELD_INFO *CWet_hDoc::FindFieldInfo( LPCSTR fieldname )
   {
   for ( int i=0; i < m_fieldInfoArray.GetSize(); i++ )
      {
      if ( lstrcmpi( fieldname, m_fieldInfoArray[ i ]->fieldname ) == 0 )
         return m_fieldInfoArray[ i ];
      }

   return NULL;
   }



void CWet_hDoc::OnCloseDocument() 
   {
//   if ( gpHydroModel != NULL )
  //    delete gpHydroModel;
	
	//CDocument::OnCloseDocument();
   }

int CWet_hDoc::LoadLulcInfo( LPCSTR filename )
   {
   m_lulcTree.RemoveAll();

   FILE *fp = fopen( filename, "rt" );

   if ( fp == NULL )
      {
      CString msg;
      msg.Format( "Unable to open LULC file %s", filename );

      ErrorMsg( msg );
      return -1;
      }

   char buffer[ 256 ];
   int  currentLevel = 0;

   m_lulcTree.m_pRootNode = new LulcNode;

   //LULC_INFO *pInfo = new LULC_INFO;
   //m_lulcCostTree.m_pRootNode->m_pData = (void*) pInfo;

   strcpy( m_lulcTree.m_pRootNode->m_name, "Land Use/Land Cover classes" );
   
   LulcNode *pLastNode = m_lulcTree.m_pRootNode;

   // get initial comment line
   fgets( buffer, 255, fp );


   // add a blank root node to the cost tree
   while ( ! feof( fp ) )
      {
      fgets( buffer, 255, fp );

      // skip any bogus lines
      if ( buffer[ 0 ] != '\t' && ! isalpha( buffer[ 0 ] ) )
         continue;

      // see what level is required
      int leadingTabCount = 0;
      if ( buffer[ 0 ] == '\t' )
         leadingTabCount++;
      if ( buffer[ 1 ] == '\t' )
         leadingTabCount++;

      LulcNode *pNode = new LulcNode;
      LULC_INFO *pInfo = new LULC_INFO;
      pNode->m_pData = (void*) pInfo;
      pNode->m_pParentNode = NULL;

      m_lulcInfoArray.Add( pInfo ); // store the info locally

      int id = -1;
      float landValue, soilValue, income, rentalRate;
      char name[ 64 ];
      int red, green, blue;

      int count = sscanf( buffer, "%[^,],%i,%i,%i,%i,%f,%f,%f,%f", name, &id, &red, &green,&blue, &landValue, &soilValue, &income, &rentalRate );
      if ( count != 9 )
         {
         CString msg( "Error parsing LULC information at: " );
         msg += buffer, 
         ErrorMsg( msg);
         fclose( fp );
         return -3;
         }
      
      // have information from file, load into node
      pNode->m_id = id;
      strcpy( pNode->m_name, name+leadingTabCount );

      pInfo->income = income;
      pInfo->landValue = landValue;
      pInfo->rentalRate = rentalRate;
      pInfo->soilValue = soilValue;

      if ( red < 0 )
         red = 255*rand()/RAND_MAX;
      if ( green < 0 )
         green = 255*rand()/RAND_MAX;
      if ( blue  < 0 )
         blue = 255*rand()/RAND_MAX;

      pNode->m_color = RGB( red, green, blue );

      // place the node at an appropriate location in the tree;
      int change = leadingTabCount - currentLevel;
      currentLevel = leadingTabCount;

      if ( leadingTabCount == 0 ) // a root of the tree?
         {
         m_lulcTree.m_pRootNode->m_childNodeArray.Add( pNode );
         pNode->m_pParentNode = m_lulcTree.m_pRootNode;
         }
      else
         {
         switch ( change ) 
            {
            case 1:     // going one level deeper, add node to last node
               pLastNode->m_childNodeArray.Add( pNode );
               pNode->m_pParentNode = pLastNode;
               break;

            case 0:     // same as current level, add node to last nodes parent node array
               {
               LulcNode *pParentNode = pLastNode->m_pParentNode;
               pParentNode->m_childNodeArray.Add( pNode );
               pNode->m_pParentNode = pParentNode;
               }
               break;

            case -1:    // one level higher in the heirarchy, add node to grandparent node array
               {
               LulcNode *pParentNode = pLastNode->m_pParentNode;
               pParentNode = pParentNode->m_pParentNode;      // get grandparent
               pParentNode->m_childNodeArray.Add( pNode );
               pNode->m_pParentNode = pParentNode;
               }
               break;
         
            default:
               {
               ErrorMsg( "Illegal heirarchy found loading LULC economic data" );
               fclose( fp );
               return -2;
               }
            }  // end of:  switch( change )
         }  // end of:  else( leadingTabCount != 0 )

      pLastNode = pNode;
      }  // end of:  while ( ! feof( fp ) )

   fclose( fp );

   return m_lulcTree.m_pRootNode->m_childNodeArray.GetSize();
   }


bool CWet_hDoc::GetLulcInfo( int level, int landUse, LULC_INFO &info )
   {
   LulcNode *pNode = m_lulcTree.FindNode( level, landUse );

   if ( pNode != NULL )
      {
//      pNode = GetCostToUse( pNode );   // move up heirarchy as necessary
      if ( pNode == NULL )
         return false;

      LULC_INFO *pInfo = (LULC_INFO*) pNode->m_pData;
      info = *pInfo;
      return true;
      }

   else
      return false;
   }





void CWet_hDoc::OnProcesscellGo() 
{

	
}



void CWet_hDoc::OnUpdateModelanalysisOptimization(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CWet_hDoc::OnFileSaveparameters() 
{

   CFileDialog dlg( TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
         "ModelFiles|*.wet|All files|*.*||");

   if ( dlg.DoModal() == IDOK )
      {
      CString filename( dlg.GetPathName() );
		gpHydroModel->SaveParameters(filename);	
		}
	}


