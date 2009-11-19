// MapLayer.h: interface for the MapLayer class.
//
//////////////////////////////////////////////////////////////////////
    
#if !defined(AFX_MAPLAYER_H__A368F5C1_7C92_11D2_821E_444553540000__INCLUDED_)
#define AFX_MAPLAYER_H__A368F5C1_7C92_11D2_821E_444553540000__INCLUDED_
    
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#if !defined _BIN_H
#include <bin.h>
#endif

#if !defined _DBTABLE_H
#include <dbtable.h>
#endif

#if !defined _TYPEDEFS_HPP
#include <typedefs.hpp>
#endif

#include <spatialIndex.h>


/////////////////////////////////////////////////////////////////////////////
// Misc critters...

class Map;
class Map3d;
class DbTable;

enum BCFLAG  { BCF_MIXED=0, BCF_RED=1, BCF_GREEN=2, BCF_BLUE=3, 
               BCF_GRAY=4, BCF_SINGLECOLOR=5, BCF_TRANSPARENT=6,
               BCF_BLUEGREEN=7, BCF_BLUERED=8, BCF_REDGREEN=9 };

enum LAYER_TYPE { LT_POLYGON, LT_LINE, LT_POINT, LT_GRID, LT_NULL };

const long NO_OUTLINE = 0xff000000;
const long NO_LABELS  = 0xff000000;

const int E  = 1;
const int SE = 2;
const int S  = 4;
const int SW = 8;
const int W  = 16;
const int NW = 32;
const int N  = 64;
const int NE = 128;


struct ROW_COL
   {
   int row;
   int col;

   ROW_COL() : row( -1 ), col( -1 ) { }

   ROW_COL( int _row, int _col ) : row ( _row ), col( _col ) { }
   };



struct Vertex : public COORD3d
   {
   Vertex( REAL x, REAL y ) : COORD3d( x, y, 0.0 ) { }
   Vertex( REAL x, REAL y, REAL z) : COORD3d( x, y, z ) {}
   Vertex( ) : COORD3d( REAL( 0.0 ), REAL( 0.0 ), REAL( 0.0 ) ) { }
   };



class RowColArray : public CArray< ROW_COL, ROW_COL& >
   {
   public:
      int Add( int row, int col ) { return CArray< ROW_COL, ROW_COL& >::Add( ROW_COL( row, col ) ); }
    
      int Find( int row, int col ) 
         {
         for ( int i=0; i < GetSize(); i++ )
            if ( GetAt(i).row == row && GetAt(i).col == col )
               return i;
         return -1 ;
         }
   };


struct MAP_STATE
   {
   BinArray   m_binArray;   // CArray of classification bins
   int    m_activeField;   // column in data object
   bool   m_isVisible;

   // legend formatting
   bool   m_showLegend;
   char   m_legendFormat[ 32 ];    // sprintf format string
   bool   m_showBinCount;          // display bin count info?
   bool   m_showSingleValue;       // one item on each legend line?
   bool   m_ascending;             // sort order
   int    m_useDisplayThreshold;   // neg = only display values below threshold, 0 =ignore threshold, pos = only display values above threshold
   float  m_displayThresholdValue; // value to use, see above for usage
   
   // display settings
   BCFLAG m_binColorFlag;         // how to auto-gen colors...
   bool   m_useVarWidth;          // for line coverages, specifies variable width line segments based on classified value
   int    m_maxVarWidth;          // upper scalar for var Width lines, ignored if m_useVarWidth = false;
   int    m_lineWidth;            // line width for drwing lines, poly outlines
   long   m_outlineColor;         // polygon outline for cell (NO_OUTLINE for no outline)
   long   m_fillColor;            // only used if BCFLAG=BCF_SINGLECOLOR
   long   m_hatchColor;
   int    m_hatchStyle;           // -1 for no hatch, or, HS_BDIAGONAL, HS_CROSS, HS_DIAGCROSS, 
                                         //    HS_FDIAGONAL, HS_HORIZONTAL, HS_VERTICAL
   long   m_labelColor;           // color to draw labels (NO_LABELS for no label)
   int    m_labelCol;
   bool   m_useNoDataBin;
   };


/////////////////////////////////////////////////////////////////////////////////////
// Poly - encapsulating a polygon
    
class __EXPORT__ Poly
   {
   public:
      //-- data members --//
      // note: for closed polygons, the last vertex must be the same as the first one.
      CArray< Vertex, Vertex& > m_vertexArray; 
      CDWordArray m_vertexPartArray;

#if ! defined _ML_IGNORE_BOUNDING_RECT
      float m_xMin;
      float m_xMax;
      float m_yMin;
      float m_yMax;
#endif
    
      int    m_id;
      Bin   *m_pBin;
      POINT *m_ptArray;
      long   m_extra;      // user-defined
          
      //--- methods ---//
      // constructor
      Poly( void ) 
         : m_vertexArray(),
           m_vertexPartArray(),
           m_id( -1 ),
           m_pBin( NULL ), 
           m_ptArray( NULL ), 
           m_extra( 0 )
           {
#if !defined _ML_INGNORE_BOUNDING_RECT
      m_xMin = m_xMax = m_yMin = m_yMax = 0;
#endif
           }
    
      Poly( Poly& );
          
      // destructor
      ~Poly();
          
      // methods
      void  InitLogicalPoints( Map* );
      float CalcSlope( void );
      float CalcElevation( void );
      float CalcDistance( void );  //straight line distance between beginning and end of the reach
      float CalcLengthSlope( float area, float slope ); // slope is RISE/RUN coming in, area is HECTARES
    
      // vertex managment
      int   GetVertexCount() const { return m_vertexArray.GetSize(); }
      const Vertex &GetVertex( int i ) const { return m_vertexArray[ i ]; }
    
      void AddVertex( const Vertex &v ) { m_vertexArray.Add( (Vertex&) v ); }
    
    	bool  DoesBoundingBoxIntersect( const Poly *pPoly ) const;
      bool  GetBoundingRect( float &xMin, float &yMin, float &xMax, float &yMax ) const;
      bool  GetBoundingZ( float &zMin, float &zMax ) const;
      REAL  NearestDistanceToPoly ( Poly *pToPoly, REAL threshold=-1, int *retFlag=NULL ) const;
      REAL  NearestDistanceToVertex( Vertex *pToVertex ) const; // returns negative distance if vertex is inside the poly
      REAL  CentroidDistanceToPoly( Poly *pToPoly ) const;
    	REAL  GetIntersectionLength( const Poly *pPoly ) const;
      bool  IsPointInPoly( Vertex &point ) const;
      bool Poly::IsPointInPoly( POINT &point ) const;
		const Vertex GetCentroid() const; 
		float GetArea() const;

   protected:
      // helper function for IsPointInPoly(.)
      void _IsPointInPoly( const Vertex *v0, const Vertex *v1, const Vertex &point, bool &isIn ) const
         {
         if ( (((v0->y<=point.y) && (point.y<v1->y))  ||
               ((v1->y<=point.y) && (point.y<v0->y))) &&
               (point.x < (v1->x - v0->x) * (point.y - v0->y) / (v1->y - v0->y) + v0->x))
            isIn = !isIn;
         }
      void _IsPointInPoly( const POINT *v0, const POINT *v1, const POINT &point, bool &isIn ) const
         {
         if ( (((v0->y<=point.y) && (point.y<v1->y))  ||
               ((v1->y<=point.y) && (point.y<v0->y))) &&
               (point.x < (v1->x - v0->x) * float(point.y - v0->y) / (v1->y - v0->y) + v0->x))
            isIn = !isIn;
         }
	};
    

class PolyArray : public CArray< Poly*, Poly* >
   {
   public:
      PolyArray( void ) { }
      PolyArray( const PolyArray & );
   };
    
    
    
//--------------------------------- MapLayer -----------------------------------
//
// implements a map layer.  Data can be polygon, line, point or grid.
// for polygons and lines, features are stored in the m_polyArray, and data is stored in m_pData.
// for grids,  data and extents are stored in m_pData.
//------------------------------------------------------------------------------
    
class  __EXPORT__  MapLayer
   {
   friend class Map;
   friend class Map3d;
    
   public:
      Map *m_pMap;
    
      CString    m_name;       // name of the layer (defaults to path name)
      CString    m_path;       // full path name for vector file
      CString    m_database;   // full path for database file (no filename for DBase)
      CString    m_tableName;  // table name (file name for DBase)
    
      LAYER_TYPE m_layerType;  // LT_POLYGON, LT_LINE, LT_POINT, LT_GRID
      PolyArray  m_polyArray;  // empty for grid, defiend for polygons, lines, points
      BinArray   m_binArray;   // CArray of classification bins
      DbTable   *m_pDbTable;   // table of data for this layer
      DataObj   *m_pData;      // pointer to the DataObj of the m_pDbTable (virtual class)
    
      int    m_activeField;   // column in data object
      bool   m_isVisible;
      bool m_3D;
      
   protected:
      // array of flags for Field information
      REAL m_vxMin;    // virtual coords
      REAL m_vxMax;
      REAL m_vyMin;
      REAL m_vyMax;
      REAL m_vzMin;
      REAL m_vzMax;
      REAL m_vExtent;
   
      // grid-only information
      float m_cellWidth;
      float m_cellHeight;
      float m_noDataValue;
    
      // query results information
      CArray< int, int > m_queryResults;

      SpatialIndex *m_pSpatialIndex;
    
   protected:
      // miscellaneous protected functions
      bool _GetDataMinMax( int col, float *pMin, float *pMax ) const;  // helper for GetDataMinMax();
      bool CheckDisplayThreshold( int rec );
    
   public:
      void  SetNoDataValue( float noDataValue ) { m_noDataValue = noDataValue; }
      float GetNoDataValue( void ) const { return m_noDataValue; }
      bool  IsDataValid( int row, int col ) const;
          
   public:
      // legend formatting
      bool   m_showLegend;
      char   m_legendFormat[ 32 ];    // sprintf format string
      bool   m_showBinCount;          // display bin count info?
      bool   m_showSingleValue;       // one item on each legend line?
      bool   m_ascending;             // sort order
      int    m_useDisplayThreshold;   // neg = only display values below threshold, 0 =ignore threshold, pos = only display values above threshold
      float  m_displayThresholdValue; // value to use, see above for usage
    
   protected:
      // display settings
      BCFLAG m_binColorFlag;         // how to auto-gen colors...
      bool   m_useVarWidth;          // for line coverages, specifies variable width line segments based on classified value
      int    m_maxVarWidth;          // upper scalar for var Width lines, ignored if m_useVarWidth = false;
      int    m_lineWidth;            // line width for drawing lines, poly outlines
      long   m_outlineColor;         // polygon outline for cell (NO_OUTLINE for no outline)
      long   m_fillColor;            // only used if BCFLAG=BCF_SINGLECOLOR
      long   m_hatchColor;
      int    m_hatchStyle;           // -1 for no hatch, or, HS_BDIAGONAL, HS_CROSS, HS_DIAGCROSS, 
                                         //    HS_FDIAGONAL, HS_HORIZONTAL, HS_VERTICAL
      long   m_labelColor;           // color to draw labels (NO_LABELS for no label)
      int    m_labelCol;             // -1 for no label col
      bool   m_useNoDataBin;         // automatically create a no data bin?
   
   protected:
      MapLayer( Map* );
      MapLayer( const MapLayer& );  // clone method
      virtual ~MapLayer();
    
   public:
      // general
      Map *GetMapPtr( void ) const { return m_pMap; }
      bool GetState( MAP_STATE& ) const;
      bool RestoreState( MAP_STATE& );
    
      // general data managment/information
      void ClearPolygons();  // deletes all polygons
      void ClearData();  // deletes all associated data
      int  GetRecordCount( void ) const;
      int  GetFieldCount( void ) const;
    
      int  GetRowCount( void ) const { return GetRecordCount(); }
      int  GetColCount( void ) const { return GetFieldCount(); }
    
      LAYER_TYPE GetType( void ) const { return m_layerType; }
          
      // vector management
      int   LoadVectorsAscii( LPCTSTR filename );
      int   LoadShape       ( LPCTSTR filename, bool loadDB=true, int extraCols=0, int records=-1 );
      int   SaveShapeFile   ( LPCTSTR filename, bool selectedPolysOnly );
      void  GetExtents      ( float &vxMin, float &vxMax, float &vyMin, float &vyMax ) const;
      bool  GetZExtents     ( float &zMin, float &zMax );  // also sets them internally
      void  UpdateExtents   ( bool initPolyPts=true );
      Poly *FindPolygon     ( int x, int y, int *pIndex=NULL ) const;   // assumes x, y are device coords
      Poly *GetPolygonFromCoord( float x, float y, int *pIndex=NULL ) const;
      Poly *FindPoint( int x, int y, int *pIndex ) const;   // assumes x, y are device coords, works on point coverages
		bool  IsPolyInMap( Poly *pPoly ) const;
      int   GetNearestPoly( float x, float y ) const;
      int   GetNearestPoly( Poly *pPoly, float maxDistance, SI_METHOD method=SIM_NEAREST, MapLayer *pToLayer=NULL ) const;

      int   GetNearbyPolys( Poly *pPoly, int *neighbors, float *distances, int maxCount, float maxDistance, 
                            SI_METHOD method=SIM_NEAREST, MapLayer *pToLayer=NULL ) const;

      int   GetNearbyPolysFromIndex( Poly *pPoly, int *neighbors, float *distances, int maxCount, float maxDistance, 
                            SI_METHOD method=SIM_NEAREST, MapLayer *pToLayer=NULL ) const;

      int   CreateSpatialIndex( LPCSTR filename, int maxCount, float maxDistance, SI_METHOD method, MapLayer *pToLayer=NULL );

      int   MergePolys( Poly *polysToMerge, int polyCount, MapLayer *pToLayer, bool includeData );


      //Poly *GetPolygonFromCoord( float x, float y, int *pIndex=NULL );
      Poly *GetPolygon      ( int i ) const { return m_polyArray[ i ]; }
      int   GetPolygonCount ( void ) const { return m_polyArray.GetSize(); }
      int   CalcSlopes      ( int colToSet );
      int   CalcLengthSlopes( int slopeCol, int colToSet );
      int   ClonePolygons   ( MapLayer *pLayerToCopyFrom );
      int   PopulateNearestDistanceToPoly( Poly *pToPoly, int col, REAL threshold, LPCSTR label=NULL );
      int   PopulateNearestDistanceToPolyIndex( Poly *pToPoly, int col, REAL threshold, int maxCount, LPCSTR label=NULL );
      int   PopulateNearestDistanceToCoverage( MapLayer *pToLayer, int col, REAL threshold, LPCSTR label=NULL );
    	int   PopulateLengthOfIntersection( MapLayer *pToLayer, int col, LPCSTR label= NULL );
		int   PopulateSelectedLengthOfIntersection( MapLayer *pToLayer, int col, int colToSelectFrom, LPCSTR label=NULL  );
		int   PopulateNearestDistanceToSelectedCoverage( MapLayer *pToLayer, int col, REAL threshold, int colToSelectFrom, VData &value, LPCSTR label=NULL );
		int   PopulateFlowPathDistanceFromCentroid( MapLayer *pFlowDirectionLayer,MapLayer *pToLayer, int colToSet );
		int   PopulateSideOfStream( MapLayer *pFlowDirectionLayer, MapLayer *pToLayer, int colToSet );		
      int   PopulateFlowPathDistance( MapLayer *pFlowDir , MapLayer *pStreamGrid, int method) ;     
      int   PopulateElevation( int colToSet );
      int   Translate( float dx, float dy );
      //int   Scale( float xMin, float yMin, float xMax, float yMax );
      int   DissolvePolys( int rec0, int rec1 );
    
      // point management
      int   AddPoint( REAL x, REAL y );
      bool  GetPointCoords( int i, REAL &x, REAL &y ) const;
    
      // grid management
      int   CreateGrid( int rows, int cols, float xLLCorner, float yLLCorner, float cellsize, float initialValue, DO_TYPE );
      int   LoadGrid( LPCTSTR filename, DO_TYPE type, int maxLineWidth=-1, int binCount=10 );  // use binCount=0 for no classification
      bool  GetGridCellFromCoord( float x, float y, int &row, int &col ); // note: coords are virtual
      void  GetGridCellCenter( int row, int col, float &x, float &y ) const;
      float GetGridCellWidth ( void ) const { return m_cellWidth;  }
      float GetGridCellHeight( void ) const { return m_cellHeight; }
      void  SetGridToNoData( void );
      bool  SaveGridFile( LPCSTR filename );
      bool  FindMinValue( int &row, int &col ) const;
      int   FindAdjacentRegion( int row, int col, RowColArray &coordArray ) const; // find all cells nth-adjacent to row, col with same attribute
    
      bool  GetLowerLeft( REAL &xLL, REAL &yLL ) const { xLL = m_vxMin; yLL = m_vyMin; return m_layerType == LT_GRID; }
      bool  GetLowerLeft( COORD2d &ll ) const { ll.x = m_vxMin; ll.y = m_vyMin; return m_layerType == LT_GRID; }
      void  SetLowerLeft( COORD2d ll );
      void  SetLowerLeft( REAL x, REAL y ) { COORD2d ll; ll.x=x; ll.y = y; SetLowerLeft( ll ); }
      bool  GetNeighborCoords( int direction, int &row, int &col ) const;
    
      void  SetCellSize ( float width, float height ) { m_cellWidth = width; m_cellHeight = height; }
      bool  GetCellSize ( float &width, float &height ) const { width = m_cellWidth; height = m_cellHeight; return m_layerType == LT_GRID; }
      void  SetDataDim  ( int cols, int rows );
      bool  GetDataDim  ( int &cols, int &rows ) const;
    
      // conversion functions
      int   GridToPoly  ( MapLayer *pPolyLayer );  // not implemented yet
      int   DEMtoFlowDir( MapLayer *pFlowLayer );  // "this" grid must be a DEM grid
          
      // grid-based watershed functions
          
      // FlowDir Grid layer functions (this layer must be a Flow Direction Grid!)
      float GetDownFlowPathDistance( int row, int col, MapLayer *pStreamLayer ) const;
      float GetDownFlowPathGradient( int row, int col, MapLayer *pStreamLayer, MapLayer *pDEMLayer, int &count ) const;
		float GetCentroidFlowPathDistance( Poly *pPoly, MapLayer *pFlowDirectionLayer, MapLayer *pToLayer) const;
      bool  FollowFlowPath    ( int &row, int &col ) const;
      bool  FindPourPoint     ( int &row, int &col, MapLayer *pStreamLayer=NULL, bool firstTime=true ) const;
      int   FindUpstreamBranchPoints( int row, int col, MapLayer *pStreamLayer, RowColArray &upstreamBranchCoordArray ) const;
      bool  GetStreamIntersectionPoint( int &row, int &col, MapLayer *pStreamLayer ) const;
      int   GetUpstreamCells  ( int row, int col, MapLayer *pStreamLayer, RowColArray &coordArray ) const;
      bool  IsCellUpstream    ( int neighborDir, int &row, int &col, MapLayer *pStreamLayer ) const;
    
      int   GetUpslopeCells( int row, int col, RowColArray &coordArray, MapLayer *pStreamLayer=NULL ) const;
      bool  GetUpslopeCell( int row, int col,  int &rowUp, int &colUp, MapLayer *pStreamLayer=NULL ) const;

      bool  GetUpstreamCell( int row, int col, MapLayer *pStreamLayer, int &rowUp, int &colUp) const;
      int   GetTotalUpslopeCells( int row, int col ) const;
      float GetTotalUpslopeArea ( int row, int col ) const;
      bool  IsCellUpslope  ( int neighborDir, int &row, int &col, MapLayer *pStreamLayer=NULL ) const;
      int   DelineateWatershedFromPoint( int row, int col, MapLayer *pWatershedLayer, int watershedID ) const;
      int   DelineateWatershedsFromStream( int startRow, int startCol, MapLayer *pWatershedLayer,
                                                 MapLayer *pStreamLayer, int &watershedID ) const;
		int   GetSideOfStream( int row, int col, MapLayer *pStreamLayer ) const;
      int   FlowDirToPower(int flowDirection) const;
    
      int   DelineateStreams( float maxUpslopeArea, MapLayer *pStreamLayer );
      int   DelineateStreamsFromPoint( int row, int col, float maxUpslopeArea, MapLayer *pStreamLayer, int branchCount=0 );
          
      // DEM layer functions (this layer must be a DEM grid
      int   GetCellFlowDir ( int row, int col ) const;
      int   FlatAreaFlowDir(int row, int col, MapLayer *pFlowLayer );
      float FillDEMCell    (int row, int col);
      int   DEMtoFilledDEM( MapLayer *pFlowLayer );  
    	int   Create3dShape( MapLayer *pDemLayer );      			
        
      int   Project3D( MapLayer *pGrid, bool interpolates=true );
    
      // Grid Stream layer functions (this layer must be a stream Grid!)
      bool  IsCellInStream( int row, int col ) const;
             
      // database management
      int  LoadDataAscii ( LPCTSTR filename );
      int  LoadDataDB    ( LPCTSTR database, LPCTSTR connectStr, LPCTSTR sql, int extraCols=0, int records=-1 );
      int  LoadDataDBF   ( LPCTSTR database, int extraCols=0, int records=-1 );
      int  SaveDataAscii ( LPCTSTR filename, bool includeID, LPCTSTR colToWrite=NULL ) const;
      int  SaveDataDB    ( void ) const;
      int  SaveDataDB    ( LPCSTR path ) const { return m_pDbTable->SaveDataDB( path ); }
      int  SaveDataDB    ( LPCTSTR databaseName, LPCTSTR tableName, LPCTSTR connectStr, bool selectedOnly=false ) const;
      int  SaveDataColDB ( int col, LPCTSTR databaseName, LPCTSTR tableName, LPCTSTR connectStr ) const;
       
      int  InitData      ( int cols, int rows, float value=0 );
      void SetField      ( int col, LPCTSTR label, COleVariant &v, bool show=true );
    
      // query management (
      int  AddQueryResult( int i )   { return m_queryResults.Add( i ); }
      void SetQueryResult( int index, int i ) { m_queryResults.SetAt( index, i ); }
      void ClearQueryResults( void ) { m_queryResults.RemoveAll(); }
      void SetQuerySize( int size )  { m_queryResults.SetSize( size ); }
      int  GetQueryCount( void ) const { return m_queryResults.GetSize(); }
      int  GetQueryResult( int i ) const { return m_queryResults[ i ]; }
      int  QueryArea( POINT ll, POINT ur, bool clear=true );  // ll, ur are logical coords
      int  QueryPoint( POINT pt, bool clear=true );           // pt is a logical coord
      int  QueryValue( int col, int value, bool all=true );

      int FindIndex( int col, int value, int startIndex=0 ) const; // gets the first record number (offset) with the specified value
    
      // get an array of Field labels...
      void  LoadFieldStrings  ( CStringArray &strArray );
      bool  GetSortedPolyArray( PolyArray& polyArray, int col, bool ascending=true );
      Poly *GetDataAtPoint( int x, int y, int col, CString &value ) const;
    
      // data access
      DataObj  *CreateDataTable( int rows, int cols, DO_TYPE type=DOT_VDATA );    
      bool HasData( void ) const { return m_pData != NULL ? true : false; }
      void SetData( int rec, int col, float value );
      void SetData( int rec, int col, COleVariant &value );
      void SetData( int rec, int col, int value );
      void SetData( int rec, int col, VData &value );
      void SetNoData( int rec, int col ) { SetData( rec, col, GetNoDataValue() ); }
      void SetAllData( float value, bool includeNoData );
      bool SetColData( int col, float value, bool includeNoData );
      bool SetColNoData( int col ) { return SetColData( col, m_noDataValue, true ); }

    
      bool GetData( int rec, int col, COleVariant &value ) const;
      bool GetData( int rec, int col, float &value ) const;
      bool GetData( int rec, int col, VData &value ) const;
      bool GetData( int rec, int col, int &value ) const;
      bool GetData( int rec, int col, CString &value ) const;
       
      bool GetDataMinMax( int col, float *pMin, float *pMax ) const;  // note: col=-1 gets min/max for ALL columns
      bool GetGridDataMinMax( float *pMin, float *pMax ) const;      // specially for grids
    
      // Field information
      void    SetActiveField  ( int col ) { m_activeField = col; }
      int     GetActiveField  ( void ) const { return m_activeField; }
      LPCTSTR GetFieldLabel   ( int col=-1 ) const;
      void    SetFieldLabel   ( int col, LPCSTR label );
      int     GetFieldCol     ( LPCTSTR label ) const;
      void    ShowField       ( int col, bool show );
      bool    IsFieldVisible  ( int col ) const;
      TYPE    GetFieldType    ( int col ) const;
      void    SetFieldType    ( int col, TYPE );
    
      // display management
      bool IsVisible() const { return m_isVisible; }
      void Show() { m_isVisible = true; }
      void Hide() { m_isVisible = false; }
    
      long GetOutlineColor( void ) const { return m_outlineColor; }
      void SetOutlineColor( long color ) { m_outlineColor = color; }
      void SetNoOutline   ( void ) { m_outlineColor = NO_OUTLINE; }

      void SetSolidFill( long color ) { m_fillColor = color; m_binColorFlag = BCF_SINGLECOLOR; }
    
      // -1 for no hatch, or, HS_BDIAGONAL, HS_CROSS, HS_DIAGCROSS, 
      void SetHatch( int style, int color ) { m_hatchColor = color; m_hatchStyle = style; }
      long GetHatchColor() const { return m_hatchColor; }
      int  GetHatchStyle() const { return m_hatchStyle; } 

      void ClearHatch() { m_hatchStyle = -1; }      
      bool ShowLabels     ( void ) { return ( m_labelColor != NO_LABELS && m_labelCol >= 0 ) ? true : false; }
      void HideLabels     ( void ) { m_labelColor = NO_LABELS; }
      long GetLabelColor  ( void ) const { return m_labelColor; }
      void SetLabelColor  ( long color ) { m_labelColor = color; }
      void SetLabelField  ( int col ) { m_labelCol = col; }
      bool GetRecordLabel ( int rec, CString& ) const;
    
      void ShowLegend  ( bool show=true ) { m_showLegend   = show; }
      void ShowBinCount( bool show=true ) { m_showBinCount = show; }
      void SortBins( bool increasingOrder );

      void SetLineWidth( int width ) { m_lineWidth = width; }
      void UseVarWidth( int maxWidthPixels ) { m_useVarWidth = true; m_maxVarWidth = maxWidthPixels; }
      void ClearVarWidth( void ) { m_useVarWidth = false; }
                  
      // classification stuff
      void SetBinColorFlag( BCFLAG flag ) { m_binColorFlag = flag; }
      BCFLAG GetBinColorFlag( void ) const { return m_binColorFlag; }
    
      void SetBins     ( float binMin, float binMax, int binCount, BinArray *pExtBinArray=NULL, LPCSTR units=NULL );
      void SetBins     ( CStringArray &, BinArray *pExtBinArray=NULL, BINTYPE type=BT_STRING, LPCSTR units=NULL );  // for string bins
      int  SetUniqueBins( int col=-1, BINTYPE type=BT_STRING, LPCSTR units=NULL );
      int  GetBinCount ( void ) const { return m_binArray.GetSize(); }
      void AddNoDataBin( void );

      Bin &GetBin( int i ) { return m_binArray[ i ]; }
      int GetUniqueValues( int col, CStringArray& ) const;  // col=-1 for unique values over all columns
      int GetUniqueValues( int col, CUIntArray& ) const;    // col=-1 for unique values over all columns
      int GetUniqueValues( int col, CArray< VData, VData& >& ) const;   // col=-1 for unique values over all columns
    
      bool ClassifyPoly( int col, int index );
      bool ClassifyData( int col=-1, int classifyType=0 );   // uses existing bin structure
      Bin *GetDataBin  ( float value );
      Bin *GetDataBin  ( int value );
      Bin *GetDataBin  ( LPCTSTR value );
    
      void ClearBins( bool resetPolys=false ); 
      void AddBin( COLORREF color, LPCTSTR label, BINTYPE binType, float binMin, float binMax );
    
   protected:
      int  LoadRecords( CDaoDatabase &database, LPCTSTR sql, int extraCols=0, int records=-1 );
      bool CreateTable( CDaoDatabase &database, LPCTSTR tableName, LPCTSTR connectStr ) const;
      int  SaveRecords( CDaoDatabase &database, LPCTSTR tableName ) const;
      void InitPolyLogicalPoints(); 
      void ExtendGridPoly( Poly *pPoly, int row, int col, int value );

    };
    
    
//////--------- inline functions ---------------------------
    
inline
void MapLayer::GetExtents( float &vxMin, float &vxMax, float &vyMin, float &vyMax ) const
   {
   vxMin = (float) m_vxMin;
   vxMax = (float) m_vxMax;
   vyMin = (float) m_vyMin; 
   vyMax = (float) m_vyMax;
   }   
    
inline
void MapLayer::SetData( int rec, int col, float value )
   {
   COleVariant v( value );
   SetData( rec, col, v );   
   }
       
inline
void MapLayer::SetData( int rec, int col, int value )
   {
   VData v( value );
   SetData( rec, col, v );   
   }
    
    
inline
void MapLayer::SetData( int rec, int col, VData &value )
   {
   ASSERT( m_pData != NULL );
   m_pData->Set( col, rec, value );
   }
    
inline
void MapLayer::SetData( int rec, int col, COleVariant &value )
   {
   ASSERT( m_pData != NULL );
   m_pData->Set( col, rec, value );
   }
    
inline
bool MapLayer::GetData( int rec, int col, COleVariant &value ) const
   {
   ASSERT( m_pData != NULL );
   return m_pData->Get( col, rec, value );
   }
    
inline
bool MapLayer::GetData( int rec, int col, float &value ) const
   {
   COleVariant v;
   GetData( rec, col, v );
   v.ChangeType( VT_R4 );
   value = v.fltVal;
   return true;
   }
    
    
inline
bool MapLayer::GetData( int rec, int col, VData &value ) const
   {
   ASSERT( m_pData != NULL );
   ASSERT( rec < m_pData->GetRowCount() );
   ASSERT( col < m_pData->GetColCount() );
   //if ( m_pData == NULL ) return false;
   //if ( rec >= m_pData->GetRowCount() ) return false;
   //if ( col >= m_pData->GetColCount() ) return false;
   bool ok = m_pData->Get( col, rec, value );
   ASSERT( ok ) ;
   return true;
   }
    
    
inline
bool MapLayer::GetData( int rec, int col, CString &value ) const
   {
   ASSERT( m_pData != NULL );
   ASSERT( rec < m_pData->GetRowCount() );
   ASSERT( col < m_pData->GetColCount() );
   value = m_pData->GetAsString( col, rec );
   return true;
   }
    
    
inline
bool MapLayer::GetData( int rec, int col, int &value ) const
   {
   ASSERT( m_pData != NULL );
   ASSERT( rec < m_pData->GetRowCount() );
   ASSERT( col < m_pData->GetColCount() );
	bool ok = m_pData->Get( col, rec, value );      // NULL type returns false
   return true;
   }
    
inline
void MapLayer::ShowField( int col, bool show )
   {
   ASSERT( col < m_pDbTable->GetColCount() );
   m_pDbTable->ShowField( col, show );
   }
       

inline
int MapLayer::GetFieldCol( LPCTSTR label ) const
   {
   ASSERT ( m_pData != NULL );
   return m_pData->GetCol( label );
   }
    
inline
bool MapLayer::IsFieldVisible( int col ) const
   {
   ASSERT( col < m_pDbTable->GetColCount() );
   return m_pDbTable->IsFieldVisible( col );
   }
    
inline
TYPE MapLayer::GetFieldType ( int col ) const
   {
   ASSERT( m_pDbTable != NULL );
   ASSERT( col < m_pDbTable->GetColCount() );
   return m_pDbTable->GetFieldType( col );
   }
       
inline   
void MapLayer::SetFieldType( int col, TYPE type )
   {
   ASSERT( m_pDbTable != NULL );
   ASSERT( col < m_pDbTable->GetColCount() );
   m_pDbTable->SetFieldType( col, type );
   }
    
inline
void MapLayer::SetField ( int col, LPCTSTR label, COleVariant &v, bool show )
   {
   m_pData->SetLabel( col, label );
   m_pDbTable->SetFieldType( col, VData::MapOleType( v.vt ) );
   m_pDbTable->ShowField( col, show );
    
   for ( int i=0; i < m_pData->GetRowCount(); i++ )
      SetData( i, col, v );
   }
    
    
inline
void MapLayer::AddBin( COLORREF color, LPCTSTR label, BINTYPE binType, float binMin, float binMax )
   {
   Bin bin;
   bin.SetLabel( label );
   bin.SetColor( color );
   bin.m_minVal = binMin;
   bin.m_maxVal = binMax;
    
   m_binArray.Add( bin );
    
   if ( binMin < m_binArray.m_binMin )
      m_binArray.m_binMin = binMin;
    
   if ( binMax > m_binArray.m_binMax )
      m_binArray.m_binMax = binMax;
    
   m_binArray.m_binType = binType;   // 0 - numeric, 1 - string
   }
       
    
    
    
/////////////////////////////////////////////////////////////////////////////
// PourPointDlg dialog
    
#if !defined IDD_POURPOINT
#define IDD_POURPOINT 0
#endif
    
class PourPointDlg : public CDialog
{
// Construction
public:
   PourPointDlg(CWnd* pParent = NULL);   // standard constructor
    
   // Dialog Data
   //{{AFX_DATA(PourPointDlg)
   enum { IDD = IDD_POURPOINT };
   int		m_col;
   int		m_row;
   //}}AFX_DATA
    
    
   // Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(PourPointDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL
    
   // Implementation
protected:
    
   // Generated message map functions
   //{{AFX_MSG(PourPointDlg)
   virtual BOOL OnInitDialog();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};
    
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
    
    
    
    
    #endif // !defined(AFX_MAPLAYER_H__A368F5C1_7C92_11D2_821E_444553540000__INCLUDED_)
