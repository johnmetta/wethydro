//////////////////////////////////////////////////////////////////////
// DecisionManager.cpp: implementation of the DecisionManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "restore.h"
#include "DecisionManager.h"
#include "wet_hdoc.h"
#include "mainfrm.h"

#include <colors.hpp>


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#include <dbtable.h>
#include <map.h>
#include <maplayer.h>
#include <msgwnd.h>
#include <ruleengine.h>
#include "math.h"

extern CMainFrame   *gpMain;
extern MsgWnd       *gpMsgWnd;
extern MapLayer     *gpCellLayer;
//extern RuleEngine   *gpRuleEngine;
extern CWet_hDoc  *gpDoc;

DecisionManager *gpDecisionManager = NULL;

//#define RESTORE_DATABASE "restore.mdb"



// NOTE:  the following must match the ID's in RESTORE.INI
enum { 
/*   AGRICULTURAL_RIPARIAN_BUFFER=100, 
   URBAN_RIPARIAN_BUFFER=101, 
   FOREST_RIPARIAN_BUFFER=102,
   WETLANDS_CONSERVATION=103,
   WETLANDS_CONSTRUCTION=104,
   WETLANDS_RESTORATION=105,
   CREATE_CONDITIONS_FAVORABLE_NATIVE_SPECIES=106,
   ENHANCE_STREAM_COMPLEXITY=108,
   INCREASE_LATE_SUMMER_FLOW=109,
   STREAMBANK_STABILIZATION=110,
   CREEKSIDE_MANAGEMENT=111,
   HETEROGENEOUS_DEVELOPMENT_PATTERNS=301,
   NON_RIPARIAN_FILTER_STRIPS_GRASSED_WATERWAYS=400,
   FIELD_BORDERS=401,
   AG_SOIL_AND_WATER_BMPS=402,
   AG_CHEMICAL_BMPS=403,
   AG_HABITAT_BMPS=404,
   REALLOCATE_WATER_RIGHTS=405,
   FOREST_HARVEST_TYPE_SCALE_MODIFICATION=500*/
   };


//---------------------------------------------------------------------
// Construction/Destruction
//---------------------------------------------------------------------

DecisionManager::DecisionManager()
   : m_col_attrib_description( -1 ),
     m_col_attrib_useInFramework( -1 ),
     m_col_attrib_isCellVariant ( -1 ),
     m_col_attrib_isCompensatory( -1 ),
     m_col_attrib_cellTableColLabel( -1 ),
     m_col_cell_restorationAlternative( -1 ),
     m_col_cell_restorationScore( -1 ),
     m_col_cell_cf( -1 ),
     m_scoreThreshold( 0.0f ),
     m_cfThreshold( 0.0f ),
     m_socialSubgoals        ( 0xFFFF ),  // all
     m_habitatQualitySubgoals( 0xFFFF ),     
     m_waterQuantitySubgoals ( 0xFFFF ),
     m_waterQualitySubgoals  ( 0xFFFF ),

     m_runNumber( 0 )
   {
   gpDecisionManager = this;

   // note: the ID's must sync up with the defines in the rule files
 /*  AddObjective( "Water Quality",   1, 0.1f );
   AddObjective( "Water Quantity ", 2, 0.2f );
   AddObjective( "Habitat Quality", 3, 0.3f );
   AddObjective( "Economic",        4, 0.5f );
   AddObjective( "Social",          5, 0.8f );
   NormalizeObjectiveWeights();*/
   }


DecisionManager::~DecisionManager()
   { }



bool DecisionManager::SetObjectiveWeight( int objID, float weight )
   {
   for ( int i=0; i < m_objectiveArray.GetSize(); i++ )
      {
      if ( m_objectiveArray[ i ].m_id == objID )
         {
         m_objectiveArray[ i ].m_weight = weight;
         return true;
         }
      }

   return false;
   }
   

float DecisionManager::GetObjectiveWeight( int objID )
   {
   for ( int i=0; i < m_objectiveArray.GetSize(); i++ )
      {
      if ( m_objectiveArray[ i ].m_id == objID )
         return m_objectiveArray[ i ].m_weight;         
      }

   return -1.0f;
   }


bool DecisionManager::RunAnalysis( bool selectedCellsOnly )
   {
  /*  // get the columns in the databses to hold alternative information
   ASSERT( gpCellLayer != NULL );
   m_col_cell_restorationAlternative = gpCellLayer->GetFieldCol( "Altern" );
   ASSERT( m_col_cell_restorationAlternative >= 0 );

   m_col_cell_restorationScore = gpCellLayer->GetFieldCol( "Score" );
   ASSERT( m_col_cell_restorationScore >= 0 );

   m_col_cell_cf = gpCellLayer->GetFieldCol( "CF" );
   ASSERT( m_col_cell_cf >= 0 );

   m_col_cell_area = gpCellLayer->GetFieldCol( "AREA" );
   ASSERT( m_col_cell_area >= 0 );
   
   m_col_cell_lulc_c = gpCellLayer->GetFieldCol( "LULC_C" );
   ASSERT( m_col_cell_lulc_c >= 0 );
   
   m_col_cell_altLulc = gpCellLayer->GetFieldCol( "ALT_LULC" );
   ASSERT( m_col_cell_altLulc >= 0 );

   m_col_cell_altArea = gpCellLayer->GetFieldCol( "ALT_AREA" );
   ASSERT( m_col_cell_altArea >= 0 );

   m_col_cell_streamLength = gpCellLayer->GetFieldCol( "STRLENGTH" );
   ASSERT( m_col_cell_streamLength >= 0 );

   m_col_cell_hydric = gpCellLayer->GetFieldCol( "HYDRIC" );
   ASSERT( m_col_cell_hydric >= 0 );

   int col_cell_altern2 = gpCellLayer->GetFieldCol( "Altern_2" );
   ASSERT( col_cell_altern2 >= 0 );

   int col_cell_altern3 = gpCellLayer->GetFieldCol( "Altern_3" );
   ASSERT( col_cell_altern3 >= 0 );

   // reset all the values in the restoration alternative/score columns
  float noDataValue = gpCellLayer->GetNoDataValue();
   gpCellLayer->SetColData( m_col_cell_restorationAlternative, noDataValue, false );
   gpCellLayer->SetColData( m_col_cell_restorationScore, 0.0f, true );
   gpCellLayer->SetColData( m_col_cell_altLulc, noDataValue, false );
   gpCellLayer->SetColData( m_col_cell_altArea, 0.0f, true );
   gpCellLayer->SetColData( col_cell_altern2, noDataValue, false );
   gpCellLayer->SetColData( col_cell_altern3, noDataValue, false );   
   
   // make sure all objective weights are normalized to sum to 1
   NormalizeObjectiveWeights();

   // load up the subgoals into the rule engine
   gpRuleEngine->AddVariable( "Water_Quality_subgoal",  VData( m_waterQualitySubgoals ) );
   gpRuleEngine->AddVariable( "Water_Quantity_subgoal", VData( m_waterQuantitySubgoals ) );
   gpRuleEngine->AddVariable( "Habitat_subgoal",        VData( m_habitatQualitySubgoals ) );
   gpRuleEngine->AddVariable( "Social_subgoal",         VData( m_socialSubgoals ) );
   gpRuleEngine->AddVariable( "RiparianBuffer_Width",   VData( gpDoc->m_riparianBufferWidth ) );
   //gpRuleEngine->AddVariable( "FieldBorder_Width", VData ( gpDoc->m_fieldBorderWidth ) );

   // get the number of alternatives 
   int numAlternatives = GetAlternativeCount();

   // get the number of cells to iterate on
   ASSERT( gpCellLayer != NULL );
   int cellCount;
   if ( selectedCellsOnly )
      cellCount = gpCellLayer->GetQueryCount();
   else
      cellCount = gpCellLayer->GetRecordCount();

   // Make a map of "UseInAnalysis" alternatives.  Each element is either -1 (meaning 
   // don't use that offset's alternative) or the position that that alternative 
   // occupies in the altScoreArray
   int *useMapArray = new int[ numAlternatives ];

   int numAltsInUse = 0;
 
   for ( int alt=0; alt < numAlternatives; alt++ )
      {
      m_alternativeArray[ alt ].m_applied = false;
      
      if ( UseAlternative( alt ) )
         useMapArray[ alt ] = numAltsInUse++;
      else
         useMapArray[ alt ] = -1;
      }
      
   // allocate and initialize a temporary array to hold total scores of each alternative
   //    being considered
   ALT_SCORE *altScoreArray = new ALT_SCORE[ numAltsInUse ];
 
   // cycle through each cell, getting cell attributes and running through
   //    the decision manager
   int cellsModified = 0;

   for ( int cell=0; cell < cellCount; cell++ )
      {
      if ( cell % 100 == 0 )
         {
         CString msg;
         msg.Format( "Evaluating alternatives for cell %i", cell );
         gpMain->SetStatusMsg( msg );
         }

      int currentCell = cell;
      if ( selectedCellsOnly )
         currentCell = gpCellLayer->GetQueryResult( cell );

      // load up the cell-variant attribute information
      gpRuleEngine->RefreshDbVariables( 0, currentCell );  // load up cell-variant info from rulesets DBMAP statements
   
      // for each cell, run through all the alternatives, collecting scores for each one
      int altInUse = 0;
      for ( int alt=0; alt < numAlternatives; alt++ )
         {
         // first, determine if this alternative is to be used in the analysis
         if ( useMapArray[ alt ] >= 0 )
            {
            // this alternative is a go.  Get an overall (average) score for all objectives,
            //     using multi-objective balancing
            float cfAlternative;
            float score = GetMultiObjectiveScore( alt, currentCell, cfAlternative );

            altScoreArray[ altInUse ].score    = score;           // the score of this alternative
            altScoreArray[ altInUse ].altIndex = alt;             // the index of the alternative
            altScoreArray[ altInUse ].altID    = m_alternativeArray[ alt ].m_id;
            altScoreArray[ altInUse ].cf       = cfAlternative;   // the confidence value for the alternative
            //altScoreArray[ altInUse ].area     = ... not needed here

            altInUse++;

            // write message to log file
            //CString msg;
            //msg.Format( "Evaluated cell %i for alternative %i, score = %g\n", currentCell, alt, score );
            //FileMessage( msg );
            }
         }  // end of:  for ( alt < numAlternatives )

      // all alternatives have been evaluated - pick one...
      // first, sort the list...
      qsort( (void*) altScoreArray, numAltsInUse, sizeof( ALT_SCORE ), SortAlternativeScores );

      // list is now sorted, pick best alternative.  Note:  altIndex contains the 
      // array offset for this alternative

      if ( altScoreArray[ 0 ].score > m_scoreThreshold && altScoreArray[ 0 ].cf > m_cfThreshold )
         {
         // store the alternative in the cell database table
         int count = 0, index = 0;
         while ( altScoreArray[ count ].score == altScoreArray[ count+1 ].score )
            count++;    // get count with same score

         // at this point, count should hold the number of score that scored equally well and were "best"
         if ( count == 0 )
            {
            SetAltFieldData( currentCell, altScoreArray[ 0 ].altID, altScoreArray[ 0 ].score, altScoreArray[ 0 ].cf );
            m_alternativeArray[ altScoreArray[ 0 ].altIndex ].m_applied = true;
            }

         else
            {
            double randVal = m_randUnif.GetUnif01() * (count+0.999999);
            index = int( randVal );

            SetAltFieldData( currentCell, altScoreArray[ index ].altID, altScoreArray[ index ].score, altScoreArray[ index ].cf );
            m_alternativeArray[ altScoreArray[ index ].altIndex ].m_applied = true;

            // swap this one with the first one
            if ( index != 0 )
               {
               ALT_SCORE temp( altScoreArray[ index ] );
               altScoreArray[ index ] = altScoreArray[ 0 ];
               altScoreArray[ 0 ] = temp;
               }
            }

         // check on the next best (take any within 15% of the best score)
         if ( altScoreArray[ 0 ].score - altScoreArray[ 1 ].score < 0.15*altScoreArray[ 0 ].score )
            gpCellLayer->SetData( currentCell, col_cell_altern2, altScoreArray[ 1 ].altID );

         if ( altScoreArray[ 0 ].score - altScoreArray[ 2 ].score < 0.15*altScoreArray[ 0 ].score )
            gpCellLayer->SetData( currentCell, col_cell_altern3, altScoreArray[ 2 ].altID );

         cellsModified++;
         }
      else
         {
         //gpCellLayer->SetNoData( currentCell, m_col_cell_restorationAlternative );
         //gpCellLayer->SetNoData( currentCell, m_col_cell_restorationScore );
         //gpCellLayer->SetNoData( currentCell, m_col_cell_cf );
         }

      if ( m_notifyProc != NULL )
         m_notifyProc( this, cell, m_extra );

      }  // end of:  for ( cell < cellCount )

   delete altScoreArray;

   CString report;
   report.Format( "Applied restoration options to %i cells out of a total of %i cells considered\n", 
                     cellsModified, cellCount );
   AfxGetMainWnd()->MessageBox( report, "Report", MB_OK );
   FileMessage( report );

   // update the map.  Note:  the "Alternative" field (indicated by the m_col_cell_restorationAlternative column 
   //   in the cell database), will contain the index) applied alternative when a restoration 
   // alternative was applied, or noDataValue if no restoration alternative was applied.
   gpCellLayer->SetActiveField( m_col_cell_restorationAlternative );

   // create an appropriate set of restoration a legends
   gpCellLayer->ClearBins();
   for ( int i=0; i < numAlternatives; i++ )
      {
      if ( useMapArray[ i ] >= 0 ) // alternative in use?
         {
         COLORREF color = m_alternativeArray[ i ].m_color;
         int altID = m_alternativeArray[ i ].m_id;
         gpCellLayer->AddBin( color, m_alternativeArray[ i ].m_name, BT_NUMERIC, altID-0.1f, altID+0.1f );
         }
      }

   delete useMapArray;

   // reset the map to display results of analysis
   Map *pMap = gpCellLayer->GetMapPtr();         // pointers to the map
   //gpCellLayer->ShowLegend();
   gpCellLayer->ShowBinCount();
   gpCellLayer->ClassifyData();
   pMap->RedrawWindow();
   
   // all done
   CString msg;
   msg.Format( "Completed evaluation of %i alternatives (%i in use)...", numAlternatives, numAltsInUse );
   gpMain->SetStatusMsg( msg );

   m_runNumber++;
	*/
   return true;

   }



void DecisionManager::SetAltFieldData( int cell, int bestAlt, float score, float cf )
   {
	/*
   gpCellLayer->SetData( cell, m_col_cell_restorationAlternative, bestAlt );
   gpCellLayer->SetData( cell, m_col_cell_restorationScore, score );
   gpCellLayer->SetData( cell, m_col_cell_cf, cf );
   
   float area;
   bool ok = gpCellLayer->GetData( cell, m_col_cell_area, area );
   ASSERT( ok );

   // add additional attribute information to the database
   switch( bestAlt )
      {
      case AGRICULTURAL_RIPARIAN_BUFFER:  // ag riparian buffer
         {
         double randVal = m_randUnif.GetUnif01();

         if ( randVal < 0.5 )
            gpCellLayer->SetData( cell, m_col_cell_altLulc, 101 );  // wet shrub
         else
            gpCellLayer->SetData( cell, m_col_cell_altLulc, 86 );  // natural grass

         float streamLength;
         bool ok = gpCellLayer->GetData( cell, m_col_cell_streamLength, streamLength );
         ASSERT( ok );

         float altArea = gpDoc->m_riparianBufferWidth * 2 * streamLength; // buffer both sides

         if ( altArea > area )
            altArea = area;

         gpCellLayer->SetData( cell, m_col_cell_altArea, altArea );
         }
         break;

      case URBAN_RIPARIAN_BUFFER:
         {
         double randVal = m_randUnif.GetUnif01();

         if ( randVal < 0.5 )
            gpCellLayer->SetData( cell, m_col_cell_altLulc, 49 );  //
         else
            gpCellLayer->SetData( cell, m_col_cell_altLulc, 96 );  //

         // note: no area converted
         }
         break;

      case FOREST_RIPARIAN_BUFFER:
         {
         gpCellLayer->SetData( cell, m_col_cell_altLulc, 101 );  // wet shrub
         
         float streamLength;
         bool ok = gpCellLayer->GetData( cell, m_col_cell_streamLength, streamLength );
         ASSERT( ok );

         float altArea = gpDoc->m_riparianBufferWidth * 2 * streamLength; // buffer both sides

         if ( altArea > area )
            altArea = area;

         gpCellLayer->SetData( cell, m_col_cell_altArea, altArea );
         }
         break;

      case WETLANDS_CONSERVATION:
      case WETLANDS_RESTORATION:
         {
         double randVal = m_randUnif.GetUnif01();

         if ( randVal < 0.33 )
            gpCellLayer->SetData( cell, m_col_cell_altLulc, 89 );  //
         else if ( randVal < 0.66 )
            gpCellLayer->SetData( cell, m_col_cell_altLulc, 99 );  //
         else
            gpCellLayer->SetData( cell, m_col_cell_altLulc, 101 );  // wet shrub

         // assumes full polygon converted
         gpCellLayer->SetData( cell, m_col_cell_altArea, area );
         }
         break;

      case WETLANDS_CONSTRUCTION:
         {
         gpCellLayer->SetData( cell, m_col_cell_altLulc, 33 );  // open water
         
         // assumes full polygon converted
         gpCellLayer->SetData( cell, m_col_cell_altArea, area );
         }
         break;

      case CREATE_CONDITIONS_FAVORABLE_NATIVE_SPECIES:
         {
         int lulc_c;
         bool ok = gpCellLayer->GetData( cell, m_col_cell_lulc_c, lulc_c );
         ASSERT( ok );

         int hydric;
         ok = gpCellLayer->GetData( cell, m_col_cell_hydric, hydric );
         ASSERT( ok );

         if ( lulc_c == 87 )    // dry shrub???  convert a portion to oak savahana
            {
            double randVal = m_randUnif.GetUnif01();

            if ( randVal < 0.5 )
               {
               gpCellLayer->SetData( cell, m_col_cell_altLulc, 98 );  // oak savahana
               
               // assumes full polygon converted
               gpCellLayer->SetData( cell, m_col_cell_altArea, area );
               }
            }
         }
         break;

      default:
		 // for anything else, set the alt area to the area, assumes full cell used for restoration
         //gpCellLayer->SetData( cell, m_col_cell_altArea, area );
		;

      }  // end of:  switch( bestAlt )
		*/
   }



int SortAlternativeScores(const void *elem1, const void *elem2 )
   {
	
   float score1 = ((ALT_SCORE*) elem1)->score;
   float score2 = ((ALT_SCORE*) elem2)->score;

   return ( score1 > score2 ) ? -1 : 1;
   }


// DecisionManager::GetMultiObjectiveScore() ----------------------------
//
// for the specified cell, the method gathers scores for each objective,
//     and then blends the scores according to objective weights, for the
//     specified alternative (alt)
//
// Note:  it assumes all relevant cell data has already been loaded!
//-----------------------------------------------------------------------

float DecisionManager::GetMultiObjectiveScore( int alt, int cell, float &cfAlternative )
   {
   static bool showNoMatch = true;

   // get the number of objectives
   int numObjectives = m_objectiveArray.GetSize();

   float *objectiveScoreArray = new float[ numObjectives ];

   cfAlternative = 1.0f;

   // for each objective, run the rule set
   for ( int obj=0; obj < numObjectives; obj++ )
      {
      float score;
      float cfObjective;
      int retVal = GetObjectiveScore( obj, alt, score, cfObjective );

      if ( retVal > 0 && score > 0.0f )
         {
         objectiveScoreArray[ obj ] = score;
         cfAlternative = min( cfAlternative, cfObjective );
         }

      else
         {
         switch( retVal )
            {
            case -1:  // constraints not satsified
               break;

            case -2:  // no matching predicates found
               {
               CString msg;
               msg.Format( "No predicates found for alternative %s when evaluating the %s objective for cell %i.",
                        m_alternativeArray[ alt ].m_name, m_objectiveArray[ obj ].m_name, cell );

               AfxGetMainWnd()->MessageBox( msg, "Probable Error", MB_OKCANCEL );
               break;
               }

            default:
               {
               if ( showNoMatch )
                  {
                  CString msg;
                  msg.Format( "No rules where found which were satisfied for alternative %s when evaluating the %s objective for cell %i.  Hit 'Cancel' to avoid this message...",
                           m_alternativeArray[ alt ].m_name, m_objectiveArray[ obj ].m_name, cell );

                  int retCode = AfxGetMainWnd()->MessageBox( msg, "Warning", MB_OKCANCEL );

                  if( retCode == IDCANCEL )
                     showNoMatch = false;
                  }
               }
            }  // end of: switch( retVal )

         objectiveScoreArray[ obj ] = 0.0f;
         }  // end of: else no match
      }  // end of:  for ( obj < numObjectives )

   // all objectives evaluated, now apply MO method (just SAW for now )
   // note that total score will evaluate to the same scale as the individual scores,
   //  since the objective weights have already be scaled to sum to one by NormalizeObjectiveWieghts()
   float totalScore = 0.0;
   for ( obj=0; obj < numObjectives; obj++ )
      {
      float objectiveScore = objectiveScoreArray[ obj ] * m_objectiveArray[ obj ].m_weight;
      totalScore += objectiveScore;
      }

   delete objectiveScoreArray;

   return totalScore;
   }


// get the single objective score for an objective, alternative.  Assumes the cells dbmap variables
// have already been refreshed with RuleEngine::RefreshDBMap();

int DecisionManager::GetObjectiveScore( int obj, int alt, float &objScore, float &cf )
   {
	/*
   ASSERT( obj < m_objectiveArray.GetSize() );

   int id = m_objectiveArray[ obj ].m_id;

   ArgList arglist;               // create an arglist with 2 argument
   arglist.Add( Argument( id ) ); // objective id
   arglist.Add( Argument() );     // score (empty initially)
   //arglist.Add( Argument() );     // weight (empty initially)

   // solve for this restoration alternative.   Note: Assumes rules of the form
   //  restorationAlternative( objective, score ) : weight ; e.g.
   //  riparianForestedBuffer( #wq, 3 ) : 0.2;
   CString altName = m_alternativeArray[ alt ].m_name;

   RulePtrArray matchingRuleArray;
   cf= 0;     // confidence factor

   int retVal = gpRuleEngine->FindAll( altName, arglist, matchingRuleArray );
   if ( matchingRuleArray.GetSize() > 0 )      // found a solution?
      {
      // get combined score of all rules
      int   bestRule=-1;
      objScore = 0.0f;

      for ( int i=0; i< matchingRuleArray.GetSize(); i++ )
         {
         // currently, score is calculated as the best of the matching rules
         Rule *pRule = matchingRuleArray[ i ];
         ASSERT( pRule != NULL );
            
         // recover arguments and calculate a score for this alternative
         double score;
         bool ok = pRule->m_pConsequent->m_argList[ 1 ].GetAsDouble( score );
         ASSERT( ok );

         // take care of score (remeber the maximum score - overall score is the max score
         if ( score > objScore )
            {
            objScore = (float) score;
            bestRule  = i;
            }

         // take care fo confidence factor
         float cfRule = pRule->GetWeight();
         cf = cf + ( cfRule * ( 1-cf ) );

         //float weight = pRule->GetWeight();
         //ASSERT( ok );

         // calculate and store the score in the temporary score array
         //float weightedScore = (float)(score*weight);

         //if ( weightedScore > objScore )
         //   {
         //   objScore = weightedScore;
         //   bestRule  = i;
         //   }
         }  // end of: for ( matchingRuleArray.GetSize() )
      }  // end of:  if ( matchingRuleArray.GetSize() > 0 )

   else  // no rule found, set score to 0.0 and return
      objScore = 0.0f;
		*/

   return true;
   }




void DecisionManager::NormalizeObjectiveWeights()
   {
   int i;
   int numObjectives = m_objectiveArray.GetSize();
   float sum = 0.0f;

   for ( i=0; i < numObjectives; i++ )
      sum += m_objectiveArray[ i ].m_weight;

   for ( i=0; i < numObjectives; i++ )
      m_objectiveArray[ i ].m_weight /= sum;
   }




//-- DecisionManager::ExplainDecision() ----------------------------------
//
// Generates a string with an explanation of why a particular restoration
//  option was selected, as follows:
//
//  1) get the applied alternative
//  2) for each objective, find matching rules that are applicable
//  3) report the rules in a readable form
//------------------------------------------------------------------------

bool DecisionManager::ExplainDecision( int cell, CString &explanation )
   {
/*
   // get the alternative for this cell
   if ( m_col_cell_restorationAlternative < 0 )
      return false;

   ASSERT( gpCellLayer != NULL );

   int alternative;
   bool ok = gpCellLayer->GetData( cell, m_col_cell_restorationAlternative, alternative );
   ASSERT( ok );

   CString altName;
   if ( alternative == gpCellLayer->GetNoDataValue() )
      {
      explanation =  "No alternative was selected, because no rules were found that, ";
      explanation += "after weighting for the objectives, exceeded the specified ";
      explanation += "threshold value.\n";
      }
   else
      {
      ok = m_optionData.GetData( alternative, m_col_option_name, altName );
      ASSERT( ok );
      explanation =  "The selected restoration alternative is ";
      explanation += alternative;
      explanation += " This alternative was selected for the following reasons, ";
      explanation +="listed by objective.\n";
      }

   gpRuleEngine->RefreshDbVariables( 0, cell );  // load up cell-variant info from 
                             // rulesets DBMAP statements (using table 0 - the cell table)

   // cycle through each objective, looking for matching rules
   int numObjectives = m_objectiveArray.GetSize();

   float *objectiveScoreArray = new float[ numObjectives ];
 
   // for each objective, find mathing rules
   for ( int obj=0; obj < numObjectives; obj++ )
      {
      int id = m_objectiveArray[ obj ].m_id;

      ArgList arglist;               // create an arglist with 3 argument
      arglist.Add( Argument( id ) ); // objective id
      arglist.Add( Argument() );     // score (empty initially)
      arglist.Add( Argument() );     // weight (empty initailly)

      // solve for this restoration option.   Note: Assumes rules of the form
      //  restorationOption( objective, score, weight ); e.g.
      //  riparianForestedBuffer( #wq, 3, 0.2 );
      RulePtrArray matchingRuleArray;
      int retVal = gpRuleEngine->FindAll( altName, arglist, matchingRuleArray );

      if ( matchingRuleArray.GetSize() > 0 )      // found a solution?
         {
         float bestScore = -10.0f;

         CString buffer;
         buffer.Format( "\n\n%i rules were found for the %s objective as follows: ", 
            (int) matchingRuleArray.GetSize(), (LPCSTR) m_objectiveArray[ obj ].m_name );

         explanation += buffer;

         for ( int i=0; i< matchingRuleArray.GetSize(); i++ )
            {
            // currently, score is calculated as the best of the matching rules
            Rule *pRule = matchingRuleArray[ i ];
            ASSERT( pRule != NULL );
            
            // recover arguments and calculate a score for this alternative
            double score, weight;
            bool ok = pRule->m_pConsequent->m_argList[ 1 ].GetAsDouble( score );
            ASSERT( ok );

            ok = pRule->m_pConsequent->m_argList[ 2 ].GetAsDouble( weight );
            ASSERT( ok );

            float weightedScore = (float)( score*weight );

            // have rule and score for this objective, spit it out
            CString ruletext;
            explanation += "\nThe rule:\n";
            pRule->GetText( ruletext, false );
            explanation += ruletext;
            buffer.Format( " matched with a score of %f and a weight of %f, for a combined score of %f.",
                     (float) score, (float) weight, weightedScore );                     
            explanation  += buffer;

            if ( weightedScore > bestScore )
               bestScore = weightedScore;
            }

         buffer.Format( "\n\nThe total score for this objective was %f.", bestScore );
         explanation += buffer;

         objectiveScoreArray[ obj ] = bestScore;
         }  // end of: if ( matchingRuleArray.GetSize() > 0 )
      else  // no matching rules for this objective
         {
         explanation += "\n\nNo matching rules were found for the ";
         explanation += m_objectiveArray[ obj ].m_name;
         explanation += " objective...";

         objectiveScoreArray[ obj ] = 0.0f;
         }  // end of: else no match
      }  // end of:  for ( obj < numObjectives )

   // all objectives evaluated, now apply MO method (just SAW for now )
   // note that total score will evaluate to the same scale as the individual scores,
   //  since the objective weights have already be scaled to sum to one by NormalizeObjectiveWieghts()
   float totalScore = 0.0;
   for ( obj=0; obj < numObjectives; obj++ )
      {
      float objectiveScore = objectiveScoreArray[ obj ] * m_objectiveArray[ obj ].m_weight;
      totalScore += objectiveScore;
      }

   delete objectiveScoreArray;
*/
   return true;
   }
