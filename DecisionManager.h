// DecisionManager.h: interface for the DecisionManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DECISIONMANAGER_H__B0F78C74_559E_11D3_95B4_00A076B0010A__INCLUDED_)
#define AFX_DECISIONMANAGER_H__B0F78C74_559E_11D3_95B4_00A076B0010A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//#include <dm.h>
#include <dbTable.h>
//#include <randgen\randunif.hpp>

// NOTE: the follwoing constants must match those in var_def.inc!!!
const int SS_PERCEIVED_BENEFIT_COST_RATIO = 1;
const int SS_PROTECT_PRIVATE_PROPERTY_RIGHTS = 2;
const int SS_AESTHETICS = 4;
const int SS_PUBLIC_SUPPORT = 8;
const int SS_VOLUNTARY_ACTION = 16;
const int SS_FAIRNESS = 32;
const int SS_SCIENTIFIC_MERIT = 64;
const int SS_HIGH_IMPACT_RECREATION = 128;
const int SS_LOW_IMPACT_RECREATION = 256;
const int SS_PUBLIC_EDUCATION = 512;


const int HS_SHRUB_USERS            = 1;
const int HS_GRASS_USERS            = 2;
const int HS_INSECT_PREDATORS       = 4;
const int HS_OTHER_PREDATORS        = 8;
const int HS_HARDWOOD_FOREST_USERS  = 16;
const int HS_CONIFER_FOREST_USERS   = 32;
const int HS_COOLWATER_FISH         = 64;
const int HS_STREAM_INVERTS         = 128;
const int HS_RIPARIAN_INVERTS       = 256;
const int HS_AQUATIC_VERTS          = 512;
const int HS_WETLAND_USERS          = 1024;


const int WQUAN_LOW_SUMMER_FLOWS = 1;
const int WQUAN_WINTER_FLOODING  = 2;

const int WQUAL_NITROGEN      = 1;
const int WQUAL_PHOSPHORUS    = 2;
const int WQUAL_SEDIMENTS     = 4; 
const int WQUAL_FECAL_COLIFORM= 8; 
const int WQUAL_TEMPERATURE   = 16; 
const int WQUAL_PESTICIDES    = 32; 
const int WQUAL_DO            = 64; 
const int WQUAL_OTHER         = 128; 

class DecisionManager;

typedef void (*DM_NOTIFYPROC)( DecisionManager*, int, long );

struct ALT_SCORE 
   { 
   int altIndex;
   int altID; 
   float score; 
   float cf; 
   float extra;

   ALT_SCORE( ) :  altIndex( -1 ), altID( -1 ), score( -1 ), cf( -1 ), extra( -1 ) { }   
   ALT_SCORE( ALT_SCORE &a ) { *this = a; }  

   ALT_SCORE& operator = (ALT_SCORE &a)
      { altIndex = a.altIndex; altID = a.altID; score = a.score; cf= a.cf; extra = a.extra; return *this; }
   };

int cdecl SortAlternativeScores(const void *elem1, const void *elem2 );


class Objective
   {
   public:
      CString m_name;
      int     m_id;
      float   m_weight;    // weights vary from 

      Objective( LPCSTR name, int id, float weight ) 
         : m_name( name ), m_id( id ), m_weight( weight ) {}

      Objective() : m_name(), m_id( -1 ), m_weight( 0.0f ) {}
   };


class Alternative
   {
   public:
      CString  m_name;
      int      m_id;
      bool     m_use;
      bool     m_applied;
      COLORREF m_color; // color to draw with

      Alternative( void ) : m_name(), m_id( -1 ), m_use( false ), m_applied( false ), m_color( 0L ) {}

      Alternative( LPCSTR name, int id, bool use, COLORREF color ) : m_name( name ), m_id( id ), m_use( use ),
         m_applied( false ), m_color( color ) { }
   };


class DecisionManager  
{
public:
	DecisionManager();
	virtual ~DecisionManager();

   bool RunAnalysis( bool selectedCellsOnly );

   // objective management
   void  AddObjective( LPCSTR name, int id, float weight=0.0f ) { m_objectiveArray.Add( Objective( name, id, weight ) ); }
   int   GetObjectiveCount( void )               { return m_objectiveArray.GetSize(); }
   Objective &GetObjective( int i )              { return m_objectiveArray[ i ]; }

   // alternative management
   void  AddAlternative( LPCSTR name, int id, bool use, COLORREF color ) { m_alternativeArray.Add( Alternative( name, id, use, color ) ); }
   int   GetAlternativeCount( void )             { return m_alternativeArray.GetSize(); }
   bool  UseAlternative( int i )                 { return m_alternativeArray[ i ].m_use; }
   Alternative &GetAlternative( int i )          { return m_alternativeArray[ i ]; }
   Alternative *GetAlternativeFromID( int id )   { for ( int i=0; i < m_alternativeArray.GetSize(); i++ )
                                                      if ( m_alternativeArray[ i ].m_id == id ) return &m_alternativeArray[ i ]; 
                                                   return NULL; }
   int GetAlternativeIndexFromID( int id )   { for ( int i=0; i < m_alternativeArray.GetSize(); i++ )
                                                      if ( m_alternativeArray[ i ].m_id == id ) return i; 
                                                   return -1; }

   // decision management
   float GetScoreThreshold( void )        { return m_scoreThreshold;  }  // 0-10?
   void  SetScoreThreshold( float value ) { m_scoreThreshold = value; }  // 0-10?
   float GetCfThreshold   ( void )        { return m_cfThreshold;     }  // 0-1
   void  SetCfThreshold   ( float value ) { m_cfThreshold = value;    }  // 0-1
   bool  SetObjectiveWeight( int objID, float weight );                  // 0-1
   float GetObjectiveWeight( int objID );                                // 0-1

   float GetMultiObjectiveScore( int alt, int cell, float &cf ); 
   int   GetObjectiveScore( int objOffset, int alt, float &score, float &cf ); // compute a score for the current cell

   void  NormalizeObjectiveWeights();
   bool  ExplainDecision( int cell, CString &explanation );

   void SetNotifyProc( DM_NOTIFYPROC proc, long extra ) { m_notifyProc = proc; m_extra = extra; }
   
   // public subgoals (see constants above)
   int m_waterQualitySubgoals;
   int m_waterQuantitySubgoals;
   int m_habitatQualitySubgoals;
   int m_socialSubgoals;
   int m_economicSubgoals;

   int m_runNumber;


protected:
   //CDM m_dm;      // an instance of the MADM decision tool

   // various tables in the Restore application database
   DbTable m_attributeData;            // attribute descriptors
   DbTable m_attributeScoreData;       // attribute/option score table

   int m_col_attrib_description;
   int m_col_attrib_useInFramework;
   int m_col_attrib_isCellVariant;
   int m_col_attrib_isCompensatory;
   int m_col_attrib_cellTableColLabel;
   int m_col_cell_area;                      // area of a cell polygon
   int m_col_cell_lulc_c;                    // lulc_C value
   int m_col_cell_hydric;                    // hydric soil?
   int m_col_cell_restorationAlternative;
   int m_col_cell_restorationScore;
   int m_col_cell_streamLength;
   int m_col_cell_altLulc;
   int m_col_cell_altArea;
   int m_col_cell_cf;

//   RandUniform m_randUnif;

   CDWordArray m_cellTableColArray;

   // objectives information
   CArray< Objective, Objective& > m_objectiveArray;

   CArray< Alternative, Alternative& > m_alternativeArray;

   // other data
   float m_scoreThreshold;       // use to determine whether the "best" alt is "good enough"
   float m_cfThreshold;          // " for confidence values

   DM_NOTIFYPROC m_notifyProc;
   long          m_extra;

   
   void SetAltFieldData( int cell, int bestAlt, float score, float cf );

};

#endif // !defined(AFX_DECISIONMANAGER_H__B0F78C74_559E_11D3_95B4_00A076B0010A__INCLUDED_)
