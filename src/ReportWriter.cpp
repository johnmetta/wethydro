// ReportWriter.cpp: implementation of the ReportWriter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#pragma hdrstop
//#include "restore.h"
#include "ReportWriter.h"
#include "mapwnd.h"

#include "wet_hdoc.h"
#include "decisionmanager.h"

#include <map.h>
#include <maplayer.h>

extern CWet_hDoc     *gpDoc;
extern MapLayer        *gpCellLayer;
extern MapLayer        *gpStreamLayer;
extern DecisionManager *gpDecisionManager;
extern MapWnd          *gpMapWnd;
extern WET_Hydro       *gpHydroModel;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ReportWriter::ReportWriter( LPCSTR templateName )
   {
/*
   int count = gpCellLayer->GetRecordCount();
   float area          = 0;
   float areaRuralRes  = 0;
   float areaAg        = 0;
   float areaUrban     = 0;
   float areaForest    = 0;
   float areaWetlands  = 0;
   float areaNatVeg    = 0;
   float areaWater     = 0;
   float areaRoads     = 0;
   float lengthStreams = 0;

   int colArea = gpCellLayer->GetFieldCol( "AREA" );   
   int colLulcA = gpCellLayer->GetFieldCol( "LULC_A" );
   int colStrLength = gpStreamLayer->GetFieldCol( "LENGTH" );

   ASSERT( colArea >= 0 );
   ASSERT( colLulcA >= 0 );
   ASSERT( colStrLength >= 0 );

   for ( int i=0; i < count; i++ )
      {
      float _area;
      gpCellLayer->GetData( i, colArea, _area );
      area += _area;

      int lulcA;
      gpCellLayer->GetData( i, colLulcA, lulcA );

      switch( lulcA )
         {
         case 1:  areaRuralRes += _area;     break;  // Rural Residential
         case 2:  areaUrban    += _area;     break;  // Urban
         case 3:  areaAg       += _area;     break;  // Agriculture
         case 4:  areaForest   += _area;     break;  // Forest
         case 5:  areaWetlands += _area;     break;  // Wetlands
         case 6:  areaNatVeg   += _area;     break;  // Natural Vegetation
         case 7:  areaWater    += _area;     break;  // Water
         case 8:  areaRoads    += _area;     break;  // roads
         default: areaRoads;
         }
      }

   int reachCount = gpStreamLayer->GetRecordCount();

   for ( i=0; i < reachCount; i++ )
      {
      float length;
      gpStreamLayer->GetData( i, colStrLength, length );
      lengthStreams += length;
      }

   areaRuralRes = areaRuralRes*100/area;
   areaUrban    = areaUrban*100/area;
   areaAg       = areaAg*100/area;
   areaForest   = areaForest*100/area;
   areaWetlands = areaWetlands*100/area;
   areaNatVeg   = areaNatVeg*100/area;
   areaWater    = areaWater*100/area;
   areaRoads    = areaRoads*100/area;
   
   // get path info
   // set up all the directory paths
   SetCurrentDirectory("c:\\temp\\");
   GetCurrentDirectory( 255, m_outputDir );   
   lstrcat( m_outputDir, "\\" );             // e.g. j:\restore\source\ 
   lstrcpy( m_reportDir, m_outputDir );
   lstrcat( m_reportDir, "report\\" );       // e.g. j:\restore\source\report\ 

   // output directory is report directory + watershed name
   lstrcpy( m_outputDir, m_reportDir );
   lstrcat( m_outputDir, gpDoc->m_name );
   lstrcat( m_outputDir, "\\" );             // e.g. j:\restore\source\report\bear\ 

   lstrcpy( m_templatePath, m_reportDir );   // assumes the template is in the "report" directory
   lstrcat( m_templatePath, templateName );

   lstrcpy( m_reportPath, m_outputDir );
   lstrcat( m_reportPath, "report.htm" );    // e.g. j:\restore\source\report\bear\report.htm

   char _check[ 280 ], _uncheck[ 280 ];
   //lstrcpy( _check, "<IMG alt=\"\" align=\"left\" src=\"file:///" );
   lstrcpy( _check, "<IMG alt=\"\" src=\"file:///" );
   lstrcat( _check, m_reportDir );
   lstrcpy( _uncheck, _check );
   lstrcat( _check, "check.gif" );
   lstrcat( _uncheck, "uncheck.gif" );
   lstrcat( _check,   "\">" );
   lstrcat( _uncheck, "\">" );
   LPSTR check   = m_strings.AddString( _check );
   LPSTR uncheck = m_strings.AddString( _uncheck );

   CreateDirectory( m_outputDir, NULL );

   // done gathering required info, make fields
   m_map[ "filename"      ] = gpDoc->m_name;
   
   m_map[ "area"          ] = m_strings.AddString( area/(10000.0f) );     // convert to ha
   m_map[ "area_ruralRes" ] = m_strings.AddString( areaRuralRes );
   m_map[ "area_ag"       ] = m_strings.AddString( areaAg );
   m_map[ "area_urban"    ] = m_strings.AddString( areaUrban );
   m_map[ "area_forest"   ] = m_strings.AddString( areaForest );
   m_map[ "area_wetlands" ] = m_strings.AddString( areaWetlands );
   m_map[ "area_natVeg"   ] = m_strings.AddString( areaNatVeg );
   m_map[ "area_water"    ] = m_strings.AddString( areaWater );
   m_map[ "area_roads"    ] = m_strings.AddString( areaRoads );
   m_map[ "stream_length"] = m_strings.AddString( lengthStreams/1000 );  // convert to km
   m_map[ "number_of_cells"] = m_strings.AddString( gpHydroModel->m_pCellLayer->GetRecordCount() );  
   m_map[ "number_of_reaches"] = m_strings.AddString( gpHydroModel->m_pStreamLayer->GetRecordCount() ); 
   m_map[ "number_of_upslope"] = m_strings.AddString( gpHydroModel->m_uplandUnitCount); 
   int minutes = (int)gpHydroModel->m_simulationTime;
   float seconds = (gpHydroModel->m_simulationTime-minutes)*60.0f;
   m_map[ "simulation_time_minutes"] = m_strings.AddString( minutes );
   m_map["simulation_time_seconds"] = m_strings.AddString(seconds);
   m_map["rmse"] = m_strings.AddString(gpHydroModel->m_massBalanceInfo.rmse1*1000);
   m_map["ns"] = m_strings.AddString(gpHydroModel->m_massBalanceInfo.ns1*1000);
   m_map["d"] = m_strings.AddString(gpHydroModel->m_massBalanceInfo.d*1000);
   m_map["r2"] = m_strings.AddString(gpHydroModel->m_massBalanceInfo.r2*1000);
   m_map["freqSat"] = m_strings.AddString(gpHydroModel->m_massBalanceInfo.freqSat);
   m_map["areaOver"] = m_strings.AddString(gpHydroModel->m_massBalanceInfo.hrt);


   m_map[ "init_channel_vol"      ] = m_strings.AddString( gpHydroModel->m_massBalanceInfo.initChannelVolume/area*1000.0f);
   m_map[ "final_channel_vol"          ] = m_strings.AddString( gpHydroModel->m_massBalanceInfo.finalChannelVolume/area*1000.0f );     // convert to sq km
   m_map[ "channel_storage" ] = m_strings.AddString( (gpHydroModel->m_massBalanceInfo.finalChannelVolume - gpHydroModel->m_massBalanceInfo.initChannelVolume)/area*1000.0f);
   
   m_map[ "init_soil_vol"       ] = m_strings.AddString( gpHydroModel->m_massBalanceInfo.initSoilVolume/area*1000.0f );
   m_map[ "final_soil_vol"       ] = m_strings.AddString( gpHydroModel->m_massBalanceInfo.finalSoilVolume /area*1000.0f);
   m_map[ "soil_storage" ] = m_strings.AddString( (gpHydroModel->m_massBalanceInfo.finalSoilVolume -  gpHydroModel->m_massBalanceInfo.initSoilVolume)/area*1000.0f);
   m_map[ "final_ponded_vol"] = m_strings.AddString(gpHydroModel->m_massBalanceInfo.finalPondedVolume/area*1000.0f);

   m_map[ "rainfall_vol"    ] = m_strings.AddString( gpHydroModel->m_massBalanceInfo.rainInputVolume/area*1000.0f );
   m_map[ "et_vol"   ] = m_strings.AddString( gpHydroModel->m_massBalanceInfo.etVolume /area*1000.0f);
   m_map[ "streamflow_vol" ] = m_strings.AddString( gpHydroModel->m_massBalanceInfo.streamflowVolume /area*1000.0f);
   m_map[ "ground_vol"   ] = m_strings.AddString( gpHydroModel->m_massBalanceInfo.groundLossVol /area*1000.0f);
   

   // volume of water input to the system
   float input = gpHydroModel->m_massBalanceInfo.rainInputVolume /area*1000.0f;

      
   // volume of water that left the system
   float output = (gpHydroModel->m_massBalanceInfo.etVolume+gpHydroModel->m_massBalanceInfo.groundLossVol+gpHydroModel->m_massBalanceInfo.streamflowVolume)/area*1000.0f;
   // volume of water still in the system
   float storage = ((gpHydroModel->m_massBalanceInfo.finalChannelVolume - 
                   gpHydroModel->m_massBalanceInfo.initChannelVolume) +
                  (gpHydroModel->m_massBalanceInfo.finalSoilVolume -
                  gpHydroModel->m_massBalanceInfo.initSoilVolume ) )/area*1000.0f                 
                   ;
               
   //  should be about 0.  If not, mass was not conserved.
   float unaccounted = (input - (output + storage) );
   m_map[ "total_input"   ] = m_strings.AddString( input );
   m_map[ "total_output"    ] = m_strings.AddString( output );
   m_map[ "storage"    ] = m_strings.AddString( storage );
   m_map[ "volume_unaccounted"] = m_strings.AddString( unaccounted );
   m_map[ "percent_unaccounted"] = m_strings.AddString( (unaccounted / input) * 100); 

   float newWaterInput = gpHydroModel->m_massBalanceInfo.tracerInputMass;
   float oldWaterInput = gpHydroModel->m_massBalanceInfo.initialTracerMass;
   float finalMass = gpHydroModel->m_massBalanceInfo.finalTracerMass;

   float outputTotal = gpHydroModel->m_massBalanceInfo.tracerOutputMass;

   float perNewWater = gpHydroModel->m_massBalanceInfo.perNewWater;

   m_map["perNewWater"] = m_strings.AddString(perNewWater);
   m_map[ "init_channel_tracer_mass"      ] = m_strings.AddString("None");
   m_map[ "final_channel_tracer_mass"          ] = m_strings.AddString( "Little" );     // convert to sq km
   m_map[ "channel_tracer_storage" ] = m_strings.AddString("Little" );
   m_map[ "init_soil_tracer_mass"       ] = m_strings.AddString(oldWaterInput );
   m_map[ "final_soil_tracer_mass"    ] = m_strings.AddString( finalMass);
   m_map[ "soil_tracer_storage"   ] = m_strings.AddString( finalMass-oldWaterInput );
   m_map[ "rainfall_tracer_mass" ] = m_strings.AddString( newWaterInput );
   m_map[ "et_tracer_mass"   ] = m_strings.AddString( "None");
   m_map[ "streamflow_tracer_mass"    ] = m_strings.AddString( outputTotal );
   m_map[ "total_tracer_input"    ] = m_strings.AddString( newWaterInput+oldWaterInput );
   m_map[ "total_tracer_output"] = m_strings.AddString( outputTotal ); 
   m_map[ "tracer_storage"    ] = m_strings.AddString( finalMass-oldWaterInput);
   float unaccountedTracer = newWaterInput+oldWaterInput - outputTotal - finalMass;
   m_map[ "volume_tracer_unaccounted"] = m_strings.AddString( unaccountedTracer );
   m_map[ "percent_tracer_unaccounted"] = m_strings.AddString( (unaccountedTracer/(newWaterInput+oldWaterInput))*100 );


   CString restoreScreen( "<IMG alt=\"\" src=\"file:///" );
   restoreScreen += m_reportDir;        
   restoreScreen += "wet_h_screen.gif\">";
   m_map[ "restore_screen" ] = m_strings.AddString( (LPCSTR) restoreScreen );
   */
   }


ReportWriter::~ReportWriter()
   {}



bool ReportWriter::GenerateReport()
   {
      /*
   FILE *fpTemplate = fopen( m_templatePath, "rt" );
   if ( fpTemplate == NULL )
      return false;

   FILE *fpReport= fopen( m_reportPath, "wt" );
   if ( fpReport == NULL )
      return false;

   char tbuffer[ 256 ];
   char rbuffer[ 512 ];

   while ( fgets( tbuffer, 255, fpTemplate ) != NULL )
      {
      memset( rbuffer, 0, 512*sizeof( char ) );    // clear it out

      if ( Substitute( tbuffer, rbuffer ) == false )
         break;

      fputs( rbuffer, fpReport );
      }

   fclose( fpReport );
   fclose( fpTemplate );
/*
   // now generate maps
   Map      *pMap   = gpMapWnd->m_pMap;
   MapLayer *pLayer = pMap->GetLayer( 0 );  // cell layer

   int activeCol = pLayer->GetActiveField();

   pLayer->SetActiveField( pLayer->GetFieldCol( "Altern" ) );
   pLayer->ClassifyData();
   CString emf( m_outputDir );
   emf += "altern_legend.emf";
   pMap->SaveAsEnhancedMetafile( emf, ME_LEGEND );

   emf = m_outputDir;
   emf += "altern.emf";
   pMap->SaveAsEnhancedMetafile( emf, ME_MAP );

   pLayer->SetActiveField( pLayer->GetFieldCol( "Altern_2" ) );
   pLayer->ClassifyData();
   emf = m_outputDir;
   emf += "altern_2.emf";
   pMap->SaveAsEnhancedMetafile( emf, ME_MAP );


   pLayer->SetActiveField( pLayer->GetFieldCol( "Altern_3" ) );
   pLayer->ClassifyData();
   emf = m_outputDir;
   emf += "altern_3.emf";
   pMap->SaveAsEnhancedMetafile( emf, ME_MAP );
 
   pLayer->SetActiveField( activeCol );
   pLayer->ClassifyData();

*/

   return true;
   }


bool ReportWriter::Substitute( LPSTR tbuffer, LPSTR rbuffer )
   {/*
   char *p = tbuffer;
   int copied = 0;

   while ( *p != NULL )
      {
      if ( *p == '@' )  // a substitute start character found?
         {
         char *end = strchr( p+1, '@' );     // look for end

         if ( end == NULL )
            {
            CString msg( "Bad report file at line: " );
            msg += tbuffer;
            AfxMessageBox( msg );
            return false;
            }

         *end = NULL;

         CString value;
         BOOL found = m_map.Lookup( p+1, value );

         if ( found )
            lstrcat( rbuffer, (LPCSTR) value );
         else
            {
            lstrcat( rbuffer, "??" );
            lstrcat( rbuffer, p+1 );
            lstrcat( rbuffer, "??" );
            }
         p = end+1;
         copied = lstrlen( rbuffer );
         }
      else  // p != '@'
         {
         rbuffer[ copied++ ] = *p;
         p++;
         }
      }  // end of:  while ( *p != NULL )
*/
   return true;
   }

