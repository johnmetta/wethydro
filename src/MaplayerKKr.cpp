// MapLayer.cpp: implementation of the MapLayer class.
//
//////////////////////////////////////////////////////////////////////

#include <stdafx.h>

#include <maplayer.h>
#include <map.h>
#include <crack.h>
#include <math.h>
#include <geometry.hpp>
#include <unitconv.h>
#include <error.hpp>

#include <vdataobj.h>
#include <idataobj.h>
#include <fdataobj.h>

#include <float.h>



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


MapLayer *pMapLayer;

COLORREF GetColor( int count, int maxCount, BCFLAG bcFlag );
int __cdecl Compare(const void *elem0, const void *elem1 );
int __cdecl CompareCoords( const void *arg0, const void *arg1 );

void SortStringArray( CStringArray &array );
int __cdecl SortStringProc( const void *elem1, const void *elem2 );

WORD   SwapTwoBytes   ( WORD w );
DWORD  SwapFourBytes  ( DWORD dw );

WORD   GetBigWord     ( FILE *fp );
DWORD  GetBigDWORD    ( FILE *fp );
double GetBigDouble   ( FILE *fp );

WORD   GetLittleWord  ( FILE *fp );
DWORD  GetLittleDWORD ( FILE *fp );
double GetLittleDouble( FILE *fp );

void   PutBigWord     ( WORD w, FILE *fp);
void   PutBigDWORD    ( DWORD dw, FILE *fp );

void   PutLittleWord  ( WORD w, FILE *fp );
void   PutLittleDWORD ( DWORD dw, FILE *fp );
void   PutLittleDouble( double d, FILE *fp );

void SET_FLAG( int *flag, int value ) { if ( flag != NULL ) *flag=value; }
void  __EXPORT__ CHECK_VALID_NUMBER( double value ) { ASSERT( _isnan( value ) == 0 ); }


////////////////////////////////////////////////////////////////////////////
// Poly

Poly::Poly( Poly &poly )
:  m_id( poly.m_id ),
   m_pBin( NULL ), //poly.m_pBin ),
   m_extra( poly.m_extra )
   {
   int vertexCount = poly.m_vertexArray.GetSize();

   // take care of copying the m_ptArray
   if ( vertexCount == 0 )
      m_ptArray = NULL;
   else
      m_ptArray = new POINT[ vertexCount ];

   // take care of copying the vertex array
   for ( int i=0; i < vertexCount; i++ )
      {
      m_vertexArray.Add( poly.m_vertexArray[ i ] );
      m_ptArray[ i ] = poly.m_ptArray[ i ];
      }

   // take care of copying the vertexPart array
   for ( i=0; i < poly.m_vertexPartArray.GetSize(); i++ )
      m_vertexPartArray.Add( poly.m_vertexPartArray[ i ] );
  
   }
  


Poly::~Poly() 
   { 
   m_vertexArray.RemoveAll();

   if ( m_ptArray != NULL )
      delete m_ptArray; 
   }

      
void Poly::InitLogicalPoints( Map *pMap )
   {
   int vertexCount = m_vertexArray.GetSize();

   if ( m_ptArray == NULL )
      m_ptArray = new POINT[ vertexCount ];  // allocate point array

   for ( int i=0; i < vertexCount; i++ )
      {
      // get world coords
      REAL x = m_vertexArray[ i ].x;
      REAL y = m_vertexArray[ i ].y;
      pMap->VPtoLP( x, y, m_ptArray[ i ] );
      }
   }


bool Poly::GetBoundingRect( float &xMin, float &yMin, float &xMax, float &yMax ) const
   {
#if defined _ML_IGNORE_BOUNDING_RECT

   int count = GetVertexCount();

   if ( count == 0 )
      return false;

   xMin = yMin =  10e16f;
   xMax = yMax = -10e16f;

   for ( int i=0; i < count; i++ )
      {
      Vertex &v = GetVertex( i );

      if ( v.x < xMin ) xMin = (float) v.x;
      if ( v.y < yMin ) yMin = (float) v.y;
      if ( v.x > xMax ) xMax = (float) v.x;
      if ( v.y > yMax ) yMax = (float) v.y;
      }

#else

   xMin = m_xMin;
   xMax = m_xMax;
   yMin = m_yMin;
   yMax = m_yMax;
   
#endif

   return true;
   }


bool Poly::GetBoundingZ( float &zMin, float &zMax ) const
   {
   int count = GetVertexCount();

   if ( count == 0 )
      return false;

   zMin =  10e16f;
   zMax = -10e16f;

   for ( int i=0; i < count; i++ )
      {
      const Vertex &v = GetVertex( i );

      if ( v.z < zMin ) zMin = (float) v.z;
      if ( v.z > zMax ) zMax = (float) v.z;
      }

   return true;
   }


  // Calculate  Area of a polygon.
  // The Polygon array is assumed to be closed, i.e.,
  // Polygon[0] = Polygon[High(Polygon)]  
  //
  // The algebraic sign of the area is negative for counterclockwise
  // ordering of vertices in the X-Y plane, and positive for
  // clockwise ordering.  Hence the absolute value
  //
  // Area calculation is also provided in GetArea()
  //	
  // This is an implementation of 'Greens Theorem'.	
  //
  // Reference:  "Centroid of a Polygon" in Graphics Gems IV,
  // Paul S. Heckbert (editor), Academic Press, 1994, pp. 3-6.

float Poly::GetArea() const
	{
	int vertexCount = GetVertexCount();
	double aSum = 0.0f;
	Vertex centroid;
	for (int i=0; i < vertexCount; i++)
		{
		int j = (i + 1) % vertexCount;
		Vertex vi = GetVertex( i );
		Vertex vi1 = GetVertex( j );

      double Term = vi.x * vi1.y  -  vi1.x * vi.y;
      aSum += Term;
		}
	float area = (float) fabs(0.5f * aSum);
	return area;
	}	


  // Calculate Centroid and Area of a polygon.
  // The Polygon array is assumed to be closed, i.e.,
  // Polygon[0] = Polygon[High(Polygon)]  
  //
  // The algebraic sign of the area is negative for counterclockwise
  // ordering of vertices in the X-Y plane, and positive for
  // clockwise ordering.  Hence the absolute value
  //
  // Area calculation is also provided in GetArea()
  //	
  // This is an implementation of 'Greens Theorem'.	
  //
  // Reference:  "Centroid of a Polygon" in Graphics Gems IV,
  // Paul S. Heckbert (editor), Academic Press, 1994, pp. 3-6.

const Vertex Poly::GetCentroid() const
	{
	int vertexCount = GetVertexCount();
	double xSum = 0.0f;
	double ySum = 0.0f;
	double aSum = 0.0f;
	Vertex centroid;
	for (int i=0; i < vertexCount; i++)
		{
		int j = (i + 1) % vertexCount;
		Vertex vi = GetVertex( i );
		Vertex vi1 = GetVertex( j );

      double Term = vi.x * vi1.y  -  vi1.x * vi.y;
      aSum += Term;
      xSum += (vi1.x + vi.x) * Term;
      ySum += (vi1.y + vi.y) * Term;

		}

   centroid.x = (float) fabs(xSum / (3.0f * aSum));
   centroid.y = (float) fabs(ySum / (3.0f * aSum));
	return centroid;
	}	


float Poly::CalcSlope( void )
   {
   int vertexCount = m_vertexArray.GetSize();

   ASSERT( vertexCount > 1 );

   Vertex vmin = m_vertexArray[ 0 ];
   Vertex vmax = m_vertexArray[ 0 ];
   float maxz = -1E20f;
   float maxx = -1E20f;
   float maxy = -1E20f;

   float minz = 1E20f;
   float minx = 1E20f;
   float miny = 1E20f;

   for ( int i=0; i < vertexCount; i++ )
      {
      if ( m_vertexArray[ i ].z > maxz )
         {
         vmax = m_vertexArray[ i ];
         maxz = (float) vmax.z;
         maxx = (float) vmax.x;
         maxy = (float) vmax.y;

         }

      if ( m_vertexArray[ i ].z < minz )
         {
         vmin = m_vertexArray[ i ];
         minz = (float) vmin.z;
         minx = (float) vmin.x;
         miny = (float) vmin.y;
         }
        
      }

   float distance = (( minx - maxx ) * (minx - maxx ))  + (( miny - maxy ) * (miny - maxy ));
   distance = (float) sqrt( distance );

	float slope;

	if ( distance > 0.0f  )
		slope = ( maxz - minz ) / distance;
	else
		slope = 0.001f;

   if (slope <= 0.0f)
      slope = 0.001f;

   return slope;
   }


float Poly::CalcElevation( void )
   {
   int vertexCount = m_vertexArray.GetSize();
   ASSERT( vertexCount > 0 );

   float z = 0;

   for ( int i=0; i < vertexCount; i++ )
      z +=  m_vertexArray[ i ].z;

   z /= vertexCount;
      
   return z;
   }


float Poly::CalcDistance( void )
   {
   int vertexCount = m_vertexArray.GetSize();
   ASSERT( vertexCount > 1 );
   Vertex vmin = m_vertexArray[ 0 ];
   Vertex vmax = m_vertexArray[ 0 ];
   float maxz = -1E20f;
   float maxx = -1E20f;
   float maxy = -1E20f;

   float minz = 1E20f;
   float minx = 1E20f;
   float miny = 1E20f;

   for ( int i=0; i < vertexCount; i++ )
      {
      if ( m_vertexArray[ i ].z > maxz )
         {
         vmax = m_vertexArray[ i ];
         maxz = (float) vmax.z;
         maxx = (float) vmax.x;
         maxy = (float) vmax.y;
         }

      if ( m_vertexArray[ i ].z < minz )
         {
         vmin = m_vertexArray[ i ];
         minz = (float) vmin.z;
         minx = (float) vmin.x;
         miny = (float) vmin.y;
         }
        
      }

   float distance = (( minx - maxx ) * (minx - maxx ))  + (( miny - maxy ) * (miny - maxy ));
   distance = (float) sqrt( distance );

   return distance;
   }


float Poly::CalcLengthSlope( float area, float slope )  // slope is non-decimal percent, area is HECTARES
   {
   //slope = PERCENT_DEGREE( slope );
   slope = RISERUN_DEGREE( slope ); // convert to degrees
   //ASSERT ( slope < 180.0f ); //???
   //ASSERT ( slope >= 0.0f );
     
   float lengthSlope = (float) ( pow( sin( slope * RAD_DEGREE ) / 0.0896, 1.3 ) 
                               * pow( area/22.13, 0.4 ) );
   return lengthSlope;
   }


bool Poly::DoesBoundingBoxIntersect( const Poly *pPoly) const
	{
	float xMinOther = 0.0f, yMinOther = 0.0f, xMaxOther = 0.0f, yMaxOther = 0.0f ;
	float xMinThis = 0.0f, yMinThis = 0.0f, xMaxThis = 0.0f, yMaxThis = 0.0f ;
	bool retVal = false;

   // get the bouding rect for the "other" polygon
	bool ok = pPoly->GetBoundingRect( xMinOther, yMinOther, xMaxOther, yMaxOther );

	ok = this->GetBoundingRect( xMinThis, yMinThis, xMaxThis, yMaxThis );

   // note: there are 2 cases where an intersection does *NOT* occur:
   //
   //   this:              _______
   // --------------------------------------------
   //   other   1) _____                  ( xMaxOther < xMinThis )
   //           2)                  _____ ( xMinOther > xMaxThis )
   //
   // anything else is an intersection
   //
   // repeates with y, and both x and y must intersect for the bounding boxes to intersect

   // check X intersection first
   if ( ( xMaxOther < xMinThis )    // case 1 
     || ( xMinOther > xMaxThis ) )  // case 2
      return false;  // no reason to look further

   // check y intersection next
   if ( ( yMaxOther < yMinThis )    // case 1 
     || ( yMinOther > yMaxThis ) )  // case 2
     return false;

   return true;
   }



REAL Poly::GetIntersectionLength( const Poly *pPoly ) const
	{/*
	if ( DoesBoundingBoxIntersect( pPoly ) == false )
      return 0.0f;
	
   int thisVertexCount = GetVertexCount();
	int otherVertexCount = pPoly->GetVertexCount();

   COORD3d  coordCache[ 32 ];      // temporary cache for vertex intersections
   COORD3d *coordCachePtr[ 32 ];   // temporary cache for vertex intersections
   
	// check segments on this poly with vertices on the other poly
	REAL length = 0.0f;

   // interate though every edge of the the passed in line segment, check for intersections   
	for ( int i=0; i < otherVertexCount - 1; i++ )
	   {
      const Vertex &otherVertex0 = pPoly->GetVertex( i );
		const Vertex &otherVertex1 = pPoly->GetVertex( i + 1 );

      // build cache of intersection points by interating through this polygon.  First point in cache is the first vertex of this edge
      coordCache[ 0 ] = pPoly->m_vertexArray[ i ];
      int count = 1;    // one coord cached so far

      // now, loop throught this poly's edges, looking for intersection pts.  If found, add to the cache
		for ( int j=0; j < thisVertexCount - 1; j++ )		// polygon segments
	   	{
			const Vertex &thisVertex0 = GetVertex( j );
			const Vertex &thisVertex1 = GetVertex( j + 1 );
			Vertex intersectionVertex;

		   bool ok  = GetIntersectionPt( thisVertex0, thisVertex1, otherVertex0, otherVertex1, intersectionVertex );

         ASSERT( count < 32 );

		   if ( ok )
      	   {
            coordCache[ count ] = intersectionVertex;
            count++;
				}
		   }  // end of:  for ( j < thisVertexCount-1 )

      // added all intersection points, add last end point
      coordCache[ count ] = pPoly->m_vertexArray[ i+1 ];
      count++;

      // sort cache
      for ( j=0; j < count; j++ )
         coordCachePtr[ j ] = coordCache+j;

      qsort( (void*) coordCachePtr, (size_t) count, sizeof( COORD3d* ), CompareCoords );

      // coords are sorted, now iterate through to check midpoints
      for ( j=0; j < count-1; j++ )
         {
         Vertex midPt;
         midPt.x = ( coordCachePtr[ j ]->x + coordCachePtr[ j+1 ]->x )/2;
         midPt.y = ( coordCachePtr[ j ]->y + coordCachePtr[ j+1 ]->y )/2;
     		bool ok = IsPointInPoly( midPt );

         if ( ok )
            {
            REAL segmentLength = (float) DistancePtToPt( coordCachePtr[j]->x, coordCachePtr[j]->y,  coordCachePtr[j+1]->x, coordCachePtr[j+1]->y);
            length += segmentLength;
            }
         } 

      // done with one segment of the passed in polygon
		}  // end of:  for ( i < otherVertexCount-1

   // done with all segments of the passed in poly, return*/
  REAL length=1;
   return length;
   }


int CompareCoords( const void *arg0, const void *arg1 )
   {
   COORD3d **pC0 = (COORD3d**) arg0;
   COORD3d **pC1 = (COORD3d**) arg1;

   if ( (*pC0)->x != (*pC1)->x )
      return ( ( (*pC1)->x - (*pC0)->x ) > 0 ) ? 1 : -1;

   else
      return ( ( (*pC1)->y - (*pC0)->y ) > 0 ) ? 1 : -1;
   }


bool Poly::IsPointInPoly( Vertex &point ) const
   {
   // check bounding rect first
#if defined _ML_IGNORE_BOUNDING_RECT

   float m_xMin, m_yMin, m_xMax, m_yMax;
   GetBoundingRect( m_xMin, m_yMin, m_xMax, m_yMax );
#endif

  if ( point.x < m_xMin || point.x > m_xMax )
      return false;
   if ( point.y < m_yMin || point.x > m_yMax )
      return false;


   // NOTE:  Doesn't deal with PARTS, probably should
   int xflag0, yflag1;
   int numverts = m_vertexArray.GetSize();
   REAL  dv0;

   //-- intermediate verticies --//
   const Vertex *v0 = &m_vertexArray[ numverts-1 ];
   Vertex *v1;

   //-- get test bit for above/below Y axis --//
   int yflag0 = ( dv0 = v0->y - point.y ) >= 0.0f;

   int crossings = 0;
   for ( int j=0; j < numverts; j++ )
      {
      // cleverness:  bobble between filling endpoints of edges, so
      // that the previous edge's shared endpoint is maintained.
      if ( j & 0x1 )
         {
         v0 = (Vertex*) &m_vertexArray[ j ];
         yflag0 = ( dv0 = v0->y - point.y ) >= 0.0;
         }
      else
         {
         v1 = (Vertex*) &m_vertexArray[ j ];
         yflag1 = ( v1->y >= point.y );
         }

      //-- check if points not both above/below X axis - can't hit ray  --//
      if ( yflag0 != yflag1 ) 
         {
         // check if points on same side of Y axis 
         if ( ( xflag0 = ( v0->x >= point.x ) ) == ( v1->x >= point.x ) )
            {
            if ( xflag0 ) crossings++;
            }
         else 
            {
            // compute intersection of poly segment with X ray, note
            // if > point's X.
            crossings += ( v0->x - dv0*( v1->x-v0->x)/(v1->y-v0->y) ) >= point.x;
            }
         }
      }  // end of: for ( j < numverts )

   return ( crossings & 0x0001 ) ? true : false;   // is # of crossings odd? then pt inside
   }



//-- Poly::NearestDistanceToPoly(Poly *pPoly ) -----------------------------------------------------
//
//  computes the shortest distance between two polygons, using a 4 part methodology
//
//  0) See if any of the vertex match.  return 0 distance if they do.
//  1) see if any of the segments cross, and return a distance of zero if they do
//  2) see if any of "this" polygons' line segments have perpendiculars that intersect any
//     of the passed in polygons vertices
//  3) see if any of the passed in polygons' line segments have perpendiculars that intersect any
//     of "this" polygons vertices
//  4) if 0, 1, 2 and 3 don't work, return the closest distance between any vertices in the two polygons
//
//  The threshold value indicates a maximum distance to consider (by checking bounding boxes).
//  If the bounding boxes are more than 'threshold' distance, compute an apprximate distance.
//  Specifying any negative value (the default) computes an actual distance in all circumstances
//---------------------------------------------------------------------------------------------------

REAL Poly::NearestDistanceToPoly(Poly *pToPoly, REAL threshold, int *retFlag ) const
   {
   int thisVertexCount = GetVertexCount();
   int otherVertexCount = pToPoly->GetVertexCount();

   SET_FLAG( retFlag, -1 );

   if ( threshold > 0 ) // check bounding box
      {
      float xMinThis, yMinThis, xMaxThis, yMaxThis;
      GetBoundingRect( xMinThis, yMinThis, xMaxThis, yMaxThis );

      float xMinOther, yMinOther, xMaxOther, yMaxOther;
      pToPoly->GetBoundingRect( xMinOther, yMinOther, xMaxOther, yMaxOther );

      // have both bounding boxes, compute the distance between the two bounding boxes.
      float distance = DistanceRectToRect( xMinThis, yMinThis, xMaxThis, yMaxThis, 
                                           xMinOther, yMinOther, xMaxOther, yMaxOther );
      if ( distance > threshold )
         {
         SET_FLAG( retFlag, 0 );
         return distance;
         }
      }

   //---- closer than the threshold distance, so do a closer examination ----//

   // Step 0: do any of the vertex's match ( are they the same?)
	for ( int i=0; i < thisVertexCount; i++ )
      for ( int j=0; j < otherVertexCount; j++ )
         {
         if ( m_vertexArray[i].x == pToPoly->m_vertexArray[j].x && m_vertexArray[i].y == pToPoly->m_vertexArray[j].y )
            {
            SET_FLAG( retFlag, 1 );
            return (REAL) 0;
            }
         }

   // Step 1:  check segments on this poly with vertices on the other poly
   // see if any segments cross         
   
   float distance = threshold;      // start large, try to find smaller distances

	for ( i=0; i < thisVertexCount-1; i++ )
      {
      const Vertex &thisVertex0 = this->GetVertex( i );
      const Vertex &thisVertex1 = this->GetVertex( i+1 );

      for ( int j=0; j < otherVertexCount-1; j++ )
         {
         const Vertex &otherVertex0 = pToPoly->GetVertex( j );
         const Vertex &otherVertex1 = pToPoly->GetVertex( j+1 );

         // compute slopes, intersect for the two line segments

         // is the "other" line vertical or horizontal?
         bool verticalOther = false, verticalThis = false, horizontalThis = false, horizontalOther = false; 
         REAL mOther = -999.0;

         if ( otherVertex0.x == otherVertex1.x )
            verticalOther = true;
         else if ( otherVertex0.y == otherVertex1.y )
            {
            horizontalOther = true;
            mOther = 0.0f;
            }
         else  // other is not vertical OR horizontal, so compute slope
            mOther = ( otherVertex0.y-otherVertex1.y ) / ( otherVertex0.x - otherVertex1.x );  // it isn't vertical, so calculate the slope of the stream segment

         // check "this" line - vertical of horizontal?
         REAL mThis = -999.0;
         if ( thisVertex0.x == thisVertex1.x )   //vertical polygon segment??????????
            verticalThis = true;
         else if ( thisVertex0.y == thisVertex1.y )
            {
            horizontalThis = true;
            mThis = 0.0f;
            }
         else
            mThis = ( thisVertex0.y-thisVertex1.y ) / ( thisVertex0.x - thisVertex1.x ); //calc slope of polygon segment

         // get intercepts for "this" and "other" vertex
         REAL bOther = -999.0;
         if ( verticalOther == false  ) 
            bOther =  otherVertex0.y - mOther * otherVertex0.x;         // get intercept of stream line

         REAL bThis = -999.0;
         if ( verticalThis == false ) 
            bThis = thisVertex0.y - mThis * thisVertex0.x;  // get intercept of polygon side

         ASSERT( ( verticalThis  && horizontalThis  ) == 0 );  // can't be both horizontal and vertical
         ASSERT( ( verticalOther && horizontalOther ) == 0 );

         // compute the intersection point
         REAL yMinThis = -999.0, yMaxThis = -999.0, yMinOther = -999.0, yMaxOther = -999.0;
            
         if ( thisVertex0.y < thisVertex1.y )
            {
            yMinThis = thisVertex0.y;
            yMaxThis = thisVertex1.y;
            }
         else
            {
            yMaxThis = thisVertex0.y;
            yMinThis = thisVertex1.y;
            }

         if ( otherVertex0.y < otherVertex1.y )
            {
            yMinOther = otherVertex0.y;
            yMaxOther = otherVertex1.y;
            }
         else
            {
            yMaxOther = otherVertex0.y;
            yMinOther = otherVertex1.y;
            }

         REAL xMinThis = -999.0, xMaxThis = -999.0, xMinOther = -999.0, xMaxOther = -999.0;

         if ( otherVertex0.x < otherVertex1.x )
            {
            xMinOther = otherVertex0.x;
            xMaxOther = otherVertex1.x;
            }
         else
            {
            xMinOther = otherVertex1.x;
            xMaxOther = otherVertex0.x;
            }

         if ( thisVertex0.x < thisVertex1.x )
            {
            xMinThis = thisVertex0.x;
            xMaxThis = thisVertex1.x;
            }
         else
            {
            xMinThis = thisVertex1.x;
            xMaxThis = thisVertex0.x;
            }


         if ( verticalThis && verticalOther )   // both vertical?
            {
				if ( yMinThis <= yMaxOther && yMinOther <= yMaxThis  ) // case where they DON'T overlap
					{
					float _distance = (float) fabs( thisVertex0.x - otherVertex0.x );

               if ( _distance < distance )
                  distance = _distance;
					}
				}

         else if ( verticalThis )  // polygon side is vertical, line segment is not
            {
            // get the intersection coordinate
            REAL yIntersect = mOther*thisVertex0.x + bOther;

            if ( horizontalOther )
               {
               if ( xMinOther <= xMinThis && xMinThis <= xMaxOther && yMinThis <= yMinOther && yMinOther <= yMaxThis )
                  {
                  SET_FLAG( retFlag, 2 );
                  return 0.0;
                  }
               }
            else
               {
               // see if the intersection is in the bounds of this both segments
   				if ( yMinOther <= yIntersect && yIntersect <= yMaxOther  && yMinThis <= yIntersect && yIntersect <= yMaxThis )
                  {
                  SET_FLAG( retFlag, 3 );
                  return 0.0f;
                  }
               }
            }  // end of: else (this vertical, other note)
       
         else if ( verticalOther )    // and NOT vertical this
            {
            // get the intersection coordinate
            REAL yIntersect = mThis*otherVertex0.x + bThis;

            if ( horizontalThis )
               {
               if ( xMinThis <= xMinOther && xMinOther <= xMaxThis  && yMinOther <= yMinThis && yMinThis <= yMaxOther  )
                  {
                  SET_FLAG( retFlag, 4 );
                  return 0.0f;
                  }
               }
            else
               {
               // see if the intersection is in the bounds both segments
               if ( yMinThis <= yIntersect && yIntersect <= yMaxThis && yMinOther <= yIntersect && yIntersect <= yMaxOther ) 
                  {
                  SET_FLAG( retFlag, 5 );
                  return 0.0f;
                  }
               }
            }

         else   // neither of the line segments are vertical, so proceed...
            {
            // get the x intersect         
            REAL xIntersect = (mThis - mOther) / ( bOther - bThis );

            // same slopes?  need to check to see if the points are colinear (same y intercepts)
            if ( mThis == mOther )
               {
               if ( bThis == bOther )  // colinear
                  {
                  if ( horizontalThis )
                     {
                     if ( xMinThis <= xMaxOther && xMinOther <= xMaxThis )
                        {
                        SET_FLAG( retFlag, 6 );
                        return 0;
                        }
                     }
                  else
                     {
                     if ( yMinThis <= yMaxOther && yMinOther <= yMaxThis )  // case where they DO overlap
                        {
                        SET_FLAG( retFlag, 7 );
                        return 0.0;
                        }
                     }
                  }
               }
            else  // different slopes...do the lines intersect in the bounds of the segments?
               {
               REAL xIntersect = ( bOther-bThis )/( mThis - mOther );

               if ( xIntersect >= xMinThis && xIntersect >= xMinOther && xIntersect <= xMaxThis && xIntersect <= xMaxOther )
                  {
                  SET_FLAG( retFlag, 8 );
                  return 0.0;
                  }
               }
            }  // end of:  else (this not vertical and other not vertical)
         }  // end of:  for ( j < otherVertexCount-1 )
      }  // end of:  for ( i < thisVertexCount-1 )

   // if we get this far, the segments do not cross - move on to step two
   //
   //  2) see if any of "this" polygons' line segments have perpendiculars that intersect any
   //     of the passed in polygons vertices

   //  First calculate distances from vertices in the linear coverage to lines in the polygon coverage
   bool foundPtSegment = false;

   for ( i=0; i < thisVertexCount-1; i++ )
      {
      const Vertex &thisVertexStart = this->GetVertex( i );
      const Vertex &thisVertexEnd   = this->GetVertex( i+1 );

      for ( int j=0; j < otherVertexCount; j++ )
         {
         const Vertex &otherVertex = pToPoly->GetVertex( j );
         float _distance;

         // get a distances to the "other" vertex (and see if it's perpendicular to this segment)
         bool onSegment = DistancePtToLineSegment( 
                                 otherVertex.x, otherVertex.y, 
                                 thisVertexStart.x, thisVertexStart.y, thisVertexEnd.x, thisVertexEnd.y,
                                 _distance );

         if ( onSegment )
            {
            if ( _distance < 0.0f )
               _distance = -_distance;

            if ( foundPtSegment == false )
               {
               distance = _distance;
               foundPtSegment = true;
               }
            else if ( _distance < distance )
               distance = _distance;
            }
         }  // end of:  for ( j < otherVertexCount )
      }  // end of:  for ( i < thisVertexCount-1 )

   // repeat for segments on other poly and vertices on this one
   for ( i=0; i < otherVertexCount-1; i++ )
      {
      const Vertex &otherVertexStart = pToPoly->GetVertex( i );
      const Vertex &otherVertexEnd   = pToPoly->GetVertex( i+1 );

      for ( int j=0; j < thisVertexCount; j++ )
         {
         const Vertex &thisVertex = this->GetVertex( j );
         float _distance;

         bool onSegment = DistancePtToLineSegment(
                                 thisVertex.x, thisVertex.y,
                                 otherVertexStart.x, otherVertexStart.y, otherVertexEnd.x, otherVertexEnd.y,
                                 _distance );

         if ( onSegment )
            {
            if ( _distance < 0.0f )
               _distance = -_distance;

            if ( foundPtSegment == false )
               {
               distance = _distance;
               foundPtSegment = true;
               }
            else if ( _distance < distance )
               distance = _distance;
            }
         }  // end of:  for ( j < thisVertexCount )
      }  // end of:  for ( i < otherVertexCount-1 )

   // did we find a perpendicular segment?  if so, we're done...
   if ( foundPtSegment == true )
      {
      SET_FLAG( retFlag, 9 );
      //return distance;
      }

   // Step 3. no perpendicular segments found, so check all the vertex-to-vertex distance's
   for ( i=0; i < thisVertexCount; i++ )
      {
      for ( int j=0; j < otherVertexCount; j++ )
         {
         const Vertex &otherVertex = pToPoly->GetVertex( j );

         // calculate the distance between these two vertices
         float _distance = DistancePtToPt( otherVertex.x, otherVertex.y,
                                           m_vertexArray[ i ].x, m_vertexArray[ i ].y );

         if ( _distance < 0.0 )
            _distance = -_distance;

         if ( _distance < distance )
            {
            distance = _distance;
            SET_FLAG( retFlag, 10 );
            }
         }
		}

   //SET_FLAG( retFlag, 10 );
   return distance;
   }

REAL Poly::NearestDistanceToVertex( Vertex *pToVertex ) const
   {
 
   int parts = m_vertexPartArray.GetSize();

   ASSERT( parts > 0 ); //must have atleast one part

   int startPart = m_vertexPartArray[ 0 ];
   int vertexCount = 0;
   int nextPart = 0;

   REAL dist  = FLT_MAX;
   REAL _dist;
   REAL L, h;

   for ( int part=0; part < parts; part++ )
      {
      if ( part < parts-1 )
         {
         nextPart = m_vertexPartArray[ part+1 ];
         vertexCount =  nextPart - startPart;
         }
      else
         vertexCount = GetVertexCount() - startPart;

      ASSERT( vertexCount >= 4 ); // a ring is composed of 4 or more points

      for ( int i=startPart; i<startPart+vertexCount - 1; i++ )
         {
         const Vertex &vertex0 = m_vertexArray.GetAt(i);
         const Vertex &vertex1 = m_vertexArray.GetAt(i+1);

         // Distance from endpoint of segment to vertex ( the other endpoint will be tested in the next loop )
         _dist = sqrt( (vertex0.x-pToVertex->x)*(vertex0.x-pToVertex->x) + (vertex0.y-pToVertex->y)*(vertex0.y-pToVertex->y) );
         if ( _dist < dist )
            dist = _dist;

         // Distance from interior of segment to vertex
         if ( vertex0.x != vertex1.x || vertex0.y != vertex1.y ) // check for degenerate segment
            {
            L = sqrt( ( vertex1.x - vertex0.x )*( vertex1.x - vertex0.x ) + ( vertex1.y - vertex0.y )*( vertex1.y - vertex0.y ) );
            h = ( ( pToVertex->x - vertex0.x )*( vertex1.x - vertex0.x ) + ( pToVertex->y - vertex0.y )*( vertex1.y - vertex0.y ) )/L;
            if ( 0 < h && h < L ) // if not, one of the endpoints is closest so dont bother with this calculation
               {
               _dist = fabs( ( ( pToVertex->y - vertex0.y )*( vertex1.x - vertex0.x ) - ( pToVertex->x - vertex0.x )*( vertex1.y - vertex0.y ) )/L );
               if ( _dist < dist )
                  dist = _dist;
               }
            }
         }

      startPart = nextPart;
      }

   if ( IsPointInPoly( *pToVertex ) )
      return -dist;
   else 
      return dist;
   }

REAL Poly::CentroidDistanceToPoly( Poly *pToPoly ) const
   {
   Vertex vThis  = GetCentroid();
   Vertex vOther = pToPoly->GetCentroid();

   return (REAL) sqrt( (vThis.x-vOther.x)*(vThis.x-vOther.x) + (vThis.y-vOther.y)*(vThis.y-vOther.y) );
   }



PolyArray::PolyArray( const PolyArray &polyArray ) 
   {
   for ( int i=0; i < polyArray.GetSize(); i++ )
      Add( polyArray[ i ] );
   }



MapLayer::MapLayer( Map *pMap )
: m_pMap( pMap ),
  m_pSpatialIndex( NULL ),
  m_layerType( LT_POLYGON ),
  m_polyArray(),
  m_binArray(),
//  m_attribInfoArray(),
  m_pDbTable( NULL ),
  m_pData( NULL ),
  m_activeField( -1 ),
  m_showLegend( false ),
  m_showBinCount( false ),
  m_showSingleValue( false ),
  m_binColorFlag( BCF_MIXED ),
  m_isVisible( true ),
  m_vxMin( 0 ),
  m_vxMax( 0 ),
  m_vyMin( 0 ),
  m_vyMax( 0 ),
  m_vzMin( 0 ),
  m_vzMax( 0 ),
  m_vExtent( 0 ),
  m_3D( false ),
  m_noDataValue( -99.0f ),
  m_useVarWidth( false ),  // for line coverages, specifies variable width line segments based on classified value
  m_maxVarWidth( 20 ),     // upper scalar for var Width lines, ignored if m_useVarWidth = false;
  m_lineWidth( 0 ),
  m_outlineColor( RGB( 127, 127, 127 ) ),
  m_hatchColor( 0 ),
  m_hatchStyle( -1 ),      // no hatch
  m_labelColor( NO_LABELS ),
  m_labelCol( -1 ),
  m_useNoDataBin( true ),
  m_ascending( false ),
  m_useDisplayThreshold( 0 ),
  m_displayThresholdValue( 0.0f )
   {
   lstrcpy( m_legendFormat, "%g" );
   }
   

MapLayer::MapLayer( const MapLayer &layer )
:  m_pMap( layer.m_pMap ),
   m_pSpatialIndex( NULL ),
   m_name( layer.m_name ),
   m_path( layer.m_path ),
   m_database( layer.m_database ),
   m_tableName( layer.m_tableName ),
   m_layerType( layer.m_layerType ),
   m_polyArray( layer.m_polyArray ),
  // m_binArray ( layer.m_binArray ),
   m_pDbTable( NULL ),
   m_pData( NULL ),
   m_activeField( layer.m_activeField ),
   m_isVisible( layer.m_isVisible ),
   m_vxMin( layer.m_vxMin ),
   m_vxMax( layer.m_vxMax ),
   m_vyMin( layer.m_vyMin ),
   m_vyMax( layer.m_vyMax ),
   m_vzMin( layer.m_vzMin ),
   m_vzMax( layer.m_vzMax ),
   m_vExtent( layer.m_vExtent ),
   m_3D( layer.m_3D ),
   m_cellWidth( layer.m_cellWidth ),
   m_cellHeight( layer.m_cellHeight ),
   m_noDataValue( layer.m_noDataValue ),
   m_showLegend( layer.m_showLegend ),
   m_showBinCount( layer.m_showBinCount ),
   m_showSingleValue( layer.m_showSingleValue ),
   m_ascending( layer.m_ascending ),
   m_binColorFlag( layer.m_binColorFlag ),
   m_useVarWidth( layer.m_useVarWidth ),
   m_maxVarWidth( layer.m_maxVarWidth ),
   m_lineWidth( layer.m_lineWidth ),
   m_outlineColor( layer.m_outlineColor ),
   m_hatchColor( layer.m_hatchColor ),
   m_hatchStyle( layer.m_hatchStyle ),
   m_labelColor( layer.m_labelColor ),
   m_labelCol( layer.m_labelCol ),
   m_useNoDataBin( layer.m_useNoDataBin ),
   m_useDisplayThreshold( layer.m_useDisplayThreshold ),
   m_displayThresholdValue( layer.m_displayThresholdValue )
   {
   lstrcpy( m_legendFormat, layer.m_legendFormat );

   if ( layer.m_pDbTable != NULL )
      {
      m_pDbTable = new DbTable( *layer.m_pDbTable );
      m_pData = m_pDbTable->m_pData;
      }
   else
      {
      m_pDbTable = NULL;
      m_pData = NULL;
      }
   }



MapLayer::~MapLayer()
   {
   if ( m_pDbTable )
      delete m_pDbTable;

   for ( int i=0; i < m_polyArray.GetSize(); i++ )
      delete m_polyArray[ i ];

   if ( m_pSpatialIndex != NULL )
      delete m_pSpatialIndex;
   }



bool MapLayer::GetState( MAP_STATE &ms ) const
   {
//   ms.m_binArray    = m_binArray;   // CArray of classification bins
   ms.m_activeField = m_activeField;   // column in data object
   ms.m_isVisible   = m_isVisible;

   // legend formatting
   ms.m_showLegend  = m_showLegend;
   lstrcpy( ms.m_legendFormat, m_legendFormat );    // sprintf format string
   ms.m_showBinCount    = m_showBinCount;          // display bin count info?
   ms.m_showSingleValue = m_showSingleValue;       // one item on each legend line?
   ms.m_ascending       = m_ascending;             // sort order
   ms.m_useDisplayThreshold   = m_useDisplayThreshold;   // neg = only display values below threshold, 0 =ignore threshold, pos = only display values above threshold
   ms.m_displayThresholdValue = m_displayThresholdValue; // value to use, see above for usage
   
   // display settings
   ms.m_binColorFlag = m_binColorFlag;         // how to auto-gen colors...
   ms.m_useVarWidth  = m_useVarWidth;          // for line coverages, specifies variable width line segments based on classified value
   ms.m_maxVarWidth  = m_maxVarWidth;          // upper scalar for var Width lines, ignored if ms.m_useVarWidth = false;
   ms.m_lineWidth    = m_lineWidth;            // line width for drwing lines, poly outlines

   ms.m_outlineColor = m_outlineColor;         // polygon outline for cell (NO_OUTLINE for no outline)
   ms.m_fillColor    = m_fillColor;            // only used if BCFLAG=BCF_SINGLECOLOR
   ms.m_hatchColor   = m_hatchColor;
   ms.m_hatchStyle   = m_hatchStyle;           // -1 for no hatch, or, HS_BDIAGONAL, HS_CROSS, HS_DIAGCROSS, 
                                               //    HS_FDIAGONAL, HS_HORIZONTAL, HS_VERTICAL
   ms.m_labelColor   = m_labelColor;           // color to draw labels (NO_LABELS for no label)
   ms.m_labelCol     = m_labelCol;
   ms.m_useNoDataBin = m_useNoDataBin;

   return true;
   }


bool MapLayer::RestoreState( MAP_STATE &ms )
   {
   m_binArray    = ms.m_binArray;        // CArray of classification bins
   m_activeField = ms.m_activeField;   // column in data object
   m_isVisible   = ms.m_isVisible;

   // legend formatting
   m_showLegend  = ms.m_showLegend;
   lstrcpy( m_legendFormat, ms.m_legendFormat );    // sprintf format string
   m_showBinCount    = ms.m_showBinCount;          // display bin count info?
   m_showSingleValue = ms.m_showSingleValue;       // one item on each legend line?
   m_ascending       = ms.m_ascending;             // sort order
   m_useDisplayThreshold   = ms.m_useDisplayThreshold;   // neg = only display values below threshold, 0 =ignore threshold, pos = only display values above threshold
   m_displayThresholdValue = ms.m_displayThresholdValue; // value to use, see above for usage
   
   // display settings
   m_binColorFlag = ms.m_binColorFlag;         // how to auto-gen colors...
   m_useVarWidth  = ms.m_useVarWidth;          // for line coverages, specifies variable width line segments based on classified value
   m_maxVarWidth  = ms.m_maxVarWidth;          // upper scalar for var Width lines, ignored if ms.m_useVarWidth = false;
   m_lineWidth    = ms.m_lineWidth;
   m_outlineColor = ms.m_outlineColor;         // polygon outline for cell (NO_OUTLINE for no outline)
   m_fillColor    = ms.m_fillColor;            // only used if BCFLAG=BCF_SINGLECOLOR
   m_hatchColor   = ms.m_hatchColor;
   m_hatchStyle   = ms.m_hatchStyle;           // -1 for no hatch, or, HS_BDIAGONAL, HS_CROSS, HS_DIAGCROSS, 
                                               //    HS_FDIAGONAL, HS_HORIZONTAL, HS_VERTICAL
   m_labelColor   = ms.m_labelColor;           // color to draw labels (NO_LABELS for no label)
   m_labelCol     = ms.m_labelCol;
   m_useNoDataBin = ms.m_useNoDataBin;

   return true;
   }

int MapLayer::GetRecordCount( void ) const
   {
   switch( m_layerType )
      {  
      case LT_POLYGON:
      case LT_LINE:
      case LT_POINT:
      case LT_GRID:
         if ( m_pData == NULL )
            return -1;
         else
            return m_pData->GetRowCount();
      
      default:
         ASSERT( 0 );   // invalid type
         break;
      }

   return -2;
   }


int MapLayer::GetFieldCount( void ) const
   {
   switch( m_layerType )
      {  
      case LT_POLYGON:
      case LT_LINE:
      case LT_POINT:
      case LT_GRID:
         if ( m_pData == NULL )
            return -1;
         else
            return m_pData->GetColCount();

      default:
         ASSERT( 0 );   // invalid type
         break;
      }

   return -2;
   }


int MapLayer::LoadVectorsAscii( LPCTSTR filename )
   {
   ClearPolygons();   // clear any existing polygons
   ClearData();   // add associated data
   
   FILE *fp = fopen( filename, "r" );

   if ( fp == NULL )
      {
      CString msg( "Unable to find file " );
      msg += filename;
      ErrorMsg( msg );
      return -1;
      }

   // start loading
   const int MAXLINEWIDTH = 128;    // max line width ( for now )
   char *buffer = new char[ MAXLINEWIDTH ]; 

   Vertex v;
   int count = 0;

   // set extents to bounding values...
   m_vxMin = m_vyMin = (REAL) LONG_MAX;
   m_vxMax = m_vyMax = (REAL) LONG_MIN;
   
	while ( ! feof( fp ) )
      {
      //-- start getting data --//
      fgets( buffer, MAXLINEWIDTH, fp );
      
      if ( *buffer == 'E' )      // end of file found (two "ENDS")
         break;

      // first line of a poly def has an id, first vertex points
      Poly *pPoly = new Poly;
      int id;
      if ( sscanf( buffer, "%i %g %g", &id, &v.x, &v.y ) != 3 )
         {
         delete pPoly;
         delete buffer;
         return -2;
         }

      pPoly->m_id = id;
      //pPoly->Add( v );  // center point, don't add...

      fgets( buffer, MAXLINEWIDTH, fp );
      while ( *buffer != 'E' )    // END found?
         {
			if ( sscanf( buffer, "%g %g", &v.x, &v.y ) != 2 )
            {
            delete pPoly;
            delete buffer;
            return -3;
            }

         //-- add a vertex to the polygon --//
         pPoly->AddVertex( v );

         if ( v.x > m_vxMax ) m_vxMax = v.x;     // check bounds
         if ( v.y > m_vyMax ) m_vyMax = v.y;
         if ( v.x < m_vxMin ) m_vxMin = v.x;
         if ( v.y < m_vyMin ) m_vyMin = v.y;

         fgets( buffer, MAXLINEWIDTH, fp );     // get next line         
         }  // end of: while buffer != 'E' (adding additional vertexes)

      m_polyArray.Add( pPoly );  // add the polygon to the Map's collection
      count++;

      m_pMap->Notify( NT_LOADVECTOR, count, 0 );
      }  // end of file reached, continue
	

   if ( ( m_vxMax-m_vxMin ) > ( m_vyMax - m_vyMin ) )
      m_vExtent = m_vxMax - m_vxMin;
   else
      m_vExtent = m_vyMax - m_vyMin;

   InitPolyLogicalPoints();   // create logical points for this set

   //-- clean up --// 
   delete buffer;
   m_pMap->Notify( NT_LOADVECTOR, -count, 0 );

   return count;
   }


int MapLayer::LoadShape( LPCTSTR filename, bool loadDB, int extraCols /* =0 */, int records /*=-1*/ )
   {
   ClearPolygons();   // clear any existing polygons
   ClearData();   // add associated data
   
   FILE *fp = fopen( filename, "rb" );

   if ( fp == NULL )
      {
      CString msg( "Unable to find file " );
      msg += filename;
      ErrorMsg( msg );
      return -1;
      }

   // start loading
   int count = 0;
	DWORD crap = GetBigDWORD( fp );
   fseek( fp, 24, SEEK_SET );
   DWORD fileLength = GetBigDWORD( fp );
   DWORD version    = GetLittleDWORD( fp );
   DWORD shapeType  = GetLittleDWORD( fp );

   m_3D = false;

   switch ( shapeType )
      {
      case 15: // 3d
         m_3D = true;         
      case 5:  // 2d
         m_layerType = LT_POLYGON;
         break;

      case 13: // 3d
         m_3D = true;
      case 3:  // 2d
         m_layerType = LT_LINE;
         break;

      case 11: // 3d
         m_3D = true;
      case 1:
         m_layerType = LT_POINT;
         break;

      default:
         ErrorMsg( "Can only read polygon, line and point shape files" );
         fclose( fp );
         return -2;
      }

   if ( m_3D )
      {
      m_vzMin = 1.0e31f;    // set unreasonable bounds, to be overwritten below...
      m_vzMax = -1.0e31f;
      }

   // get the bounding box
   m_vxMin = (float) GetLittleDouble( fp );
   m_vyMin = (float) GetLittleDouble( fp );
   m_vxMax = (float) GetLittleDouble( fp );
   m_vyMax = (float) GetLittleDouble( fp );

   CHECK_VALID_NUMBER( m_vxMin );
   CHECK_VALID_NUMBER( m_vyMin );
   CHECK_VALID_NUMBER( m_vxMax );
   CHECK_VALID_NUMBER( m_vyMax );

   // done with header, get records...
   fseek( fp, 100, SEEK_SET );

   int lastRecNo = 0;

   while ( feof( fp ) == 0 )
      {
      DWORD recordNumber  = GetBigDWORD( fp );

      if ( recordNumber != ++lastRecNo )
         {
         //CString msg;
         //msg.Format( "Bad record number found in shape file at record # %i", lastRecNo );
         //WarningMsg( msg );
         break;
         }

      DWORD contentLength = GetBigDWORD( fp );
      DWORD shapeType     = GetLittleDWORD( fp );

      if ( shapeType != 5 && shapeType != 3 && shapeType != 15 && shapeType != 13 && shapeType != 1 && shapeType != 11 )
         {
         if ( shapeType != 0 )
            {
            CString msg;
            msg.Format( "Invalid shape type (%i) found at record %d, aborting load...", shapeType, count );
            WarningMsg( msg );
            break;
            }

         // NULL shape encoutered, insert empty polygon and continue
         contentLength -= 2;
         for ( int skip=0; skip < (int) contentLength; skip++ )
            GetLittleWord(fp);

         Poly *pPoly = new Poly;
         pPoly->m_id = count;       // changed - jpb (9/9/02)
         m_polyArray.Add( pPoly );  // add the polygon to the Map's collection
         count++;
         continue;
         }

      // create a poly...
      Poly *pPoly = new Poly;
      pPoly->m_id = count;       // changed - jpb (9/9/02)
      
      // is it a polygon/line object?
      if ( shapeType == 5 || shapeType == 15 || shapeType == 3 || shapeType == 13 )
         {
         // ignore the polygons bounding box...
#if defined _ML_IGNORE_BOUNDING_RECT
         double box = GetLittleDouble( fp );
         box = GetLittleDouble( fp );
         box = GetLittleDouble( fp );
         box = GetLittleDouble( fp );
#else
         pPoly->m_xMin = (float) GetLittleDouble( fp );
         pPoly->m_yMin = (float) GetLittleDouble( fp );
         pPoly->m_xMax = (float) GetLittleDouble( fp );
         pPoly->m_yMax = (float) GetLittleDouble( fp );
#endif
         DWORD numParts  = GetLittleDWORD( fp );
         DWORD numPoints = GetLittleDWORD( fp );
         ASSERT( numParts > 0 );

         pPoly->m_vertexPartArray.SetSize( numParts );

         for ( int i=0; i < (int) numParts; i++ )
            {
            DWORD parts = GetLittleDWORD( fp );
            pPoly->m_vertexPartArray[ i ] = parts;
            }

         // and start populating it with vertexes
         Vertex v;
         for ( i=0; i < (int) numPoints; i++ )
            {
            v.x = (REAL) GetLittleDouble( fp );
            v.y = (REAL) GetLittleDouble( fp );
            pPoly->AddVertex( v );

            CHECK_VALID_NUMBER( v.x );
            CHECK_VALID_NUMBER( v.y );
            }

         if ( shapeType == 15 || shapeType == 13 )  // 3d polygons or lines?
            {
            REAL zMin = (REAL) GetLittleDouble( fp );
            REAL zMax = (REAL) GetLittleDouble( fp );

            CHECK_VALID_NUMBER( zMin );
            CHECK_VALID_NUMBER( zMax );

            if ( zMin < m_vzMin ) m_vzMin = zMin;
            if ( zMax > m_vzMax ) m_vzMax = zMax;

            for ( i=0; i < (int) numPoints; i++ )
               {
               REAL z = (REAL) GetLittleDouble( fp );
               if ( _isnan( z ) )
                  {
                  pPoly->m_vertexArray[ i ].z = 0;
                  TRACE( "Found NAN: Poly %i, point %i (z)\n", pPoly->m_id, i );
                  }

               else
                  pPoly->m_vertexArray[ i ].z = z;
               }

            // skip measures..
            GetLittleDouble( fp );  // M min
            GetLittleDouble( fp );  // M max

            for ( i=0; i < (int) numPoints; i++ )
               GetLittleDouble( fp );  // M array
            }

#ifdef _DEBUG
         // check for valid part descriptors (start vertex must be same is end vertex)
         if ( m_layerType == LT_POLYGON )
            {
            int start, end;
            end = -1;
            for ( i=0; i < (int) numParts; i++ )
               {
               start = end+1;

               if ( i == (int) numParts-1) // last part?
                  end = numPoints-1;
               else 
                  end = pPoly->m_vertexPartArray[i+1]-1;

               const Vertex &vStart = pPoly->GetVertex( start );
               const Vertex &vEnd   = pPoly->GetVertex( end   );

               if ( vStart.x != vEnd.x || vStart.y != vEnd.y || vStart.z != vEnd.z )
                  {
                  // add back if numParts == 1
                  if ( numParts == 1 )
                     pPoly->AddVertex( vStart );
                  else
                     {
                     TRACE("Unfixable part (unmatched vertices): Part number %i, Poly id: %i\n", i, pPoly->m_id );
                     TRACE("Vertex Dump:\n  Start= %f, %f, %f\n", (float) vStart.x, (float) vStart.y, (float) vStart.z );
                     TRACE(" End =    %f, %f, %f\n", (float) vEnd.x, (float) vEnd.y, (float) vEnd.z );

                     for ( int j=0; j < (int) numPoints; j++ )
                        {
                        for ( int k=0; k < (int) numParts; k++ )
                           {
                           if ( j == (int) pPoly->m_vertexPartArray[ k ] ) // is this the start of a part?
                              {
                              TRACE( "  Part %i\n", j );
                              break;
                              }

                           TRACE("    %.1f, %.1f, %.1f\n", (float) pPoly->m_vertexArray[j].x, (float) pPoly->m_vertexArray[j].y, (float) pPoly->m_vertexArray[j].z );
                           }
                        }
                     }
                  }
               }

            //ASSERT( vStart.x == vEnd.x );
            //ASSERT( vStart.y == vEnd.y );
            //ASSERT( vStart.z == vEnd.z );
            }
#endif
         }  // end of:  if ( polygon/line)

      else  // point coverage
         {
         ASSERT( shapeType == 1 || shapeType == 11 );
         
         pPoly->m_vertexPartArray.SetSize( 1 );
         pPoly->m_vertexPartArray[ 0 ] = 1;

         // populate a single vertex
         Vertex v;
         v.x = (REAL) GetLittleDouble( fp );
         v.y = (REAL) GetLittleDouble( fp );
         pPoly->AddVertex( v );

         if ( shapeType == 11 )  // 3d point?
            {
            pPoly->m_vertexArray[ 0 ].z = (REAL) GetLittleDouble( fp );
            GetLittleDouble( fp );    // skip measures
            }
         }  // end of:  if ( polygon/line)

      m_polyArray.Add( pPoly );  // add the polygon to the Map's collection
      count++;

      m_pMap->Notify( NT_LOADVECTOR, count, 0 );

      if ( count == records )    // count is the number of records loaded so far
         break;
      }  // end of: while ( feof() == 0 )  
      
   // end of file reached, continue


   //-- first layer? --//
   if ( m_pMap->GetLayerCount() == 1 )
      {
      m_pMap->SetMapExtents( m_vxMin, m_vxMax, m_vyMin, m_vyMax );
      m_pMap->ZoomFull( false );  // no redraw
      }
   else
      m_pMap->AddMapExtents( m_vxMin, m_vxMax, m_vyMin, m_vyMax );   // must do this prior to  InitPolyLogicalPoints()

   InitPolyLogicalPoints();   // create logical points for this set   

   //-- clean up --// 
   m_pMap->Notify( NT_LOADVECTOR, -count, 0 );

   fclose( fp );

   m_name = filename;
   m_path = filename;

   if ( loadDB )
      {
      // get corresponding dbf file
      char *dbf = new char[ lstrlen( filename ) + 1 ];
      lstrcpy( dbf, filename );
      char *p = strrchr( dbf, '.' );
      p++;
      lstrcpy( p, _T( "dbf" ) );

      CString sql( "SELECT * FROM " );
      sql += dbf;
      m_tableName = dbf;

      // get path of shape file
      p = strrchr( dbf, '\\' );

      if ( p == NULL )  // no path
         {
         LoadDataDB( ".", "DBASE IV;", sql, extraCols, records );
         }

      else
         {
         p++;
         *p = NULL;
         LoadDataDB( dbf, "DBASE IV;", sql, extraCols, records );
         }

      delete []  dbf;
      }

   return count;
   }


int MapLayer::ClonePolygons( MapLayer *pLayerToCopyFrom )
   {
   if ( pLayerToCopyFrom->GetType() != LT_POLYGON && pLayerToCopyFrom->GetType() != LT_LINE && pLayerToCopyFrom->GetType() != LT_POINT )
      return -1;

   // clean anthing existing out
   ClearPolygons();
   ClearData();

   // copy polygons
   int polyCount = pLayerToCopyFrom->GetPolygonCount();
   m_polyArray.SetSize( polyCount );

   // clone all the polygons
   for ( int i=0; i < polyCount; i++ )
      {
      m_polyArray[ i ] = new Poly( *pLayerToCopyFrom->GetPolygon( i ) );  // no bins, everything else ok
      ASSERT( m_polyArray[ i ] != NULL );
      }


   m_layerType = pLayerToCopyFrom->GetType();
   m_vxMin = pLayerToCopyFrom->m_vxMin;    // virtual coords
   m_vxMax = pLayerToCopyFrom->m_vxMax;
   m_vyMin = pLayerToCopyFrom->m_vyMin;
   m_vyMax = pLayerToCopyFrom->m_vyMax;
   m_vzMin = pLayerToCopyFrom->m_vzMin;
   m_vzMax = pLayerToCopyFrom->m_vzMax;
   m_vExtent = pLayerToCopyFrom->m_vExtent;

   // grid-only information
   m_cellWidth = m_cellHeight = m_noDataValue = 0.0f;
   m_queryResults.RemoveAll();

   // note this function doesn't take care of teh following...
   //  Map *m_pMap;
   //   CString    m_name;       // name of the layer (defaults to path name)
   //   CString    m_path;       // full path name for vector file
   //   CString    m_database;   // full path for database file (no filename for DBase)
   //   CString    m_tableName;  // table name (file name for DBase)

   //   BinArray   m_binArray;   // CArray of classification bins
   //   DbTable   *m_pDbTable;   // table of data for this layer
   //   VDataObj  *m_pData;      // pointer to the VDataObj of the m_pDbTable

   //   int    m_activeField;   // column in data object
   //   bool   m_isVisible;

   return polyCount;
   }


int MapLayer::CreateGrid( int rows, int cols, float xLLCorner, float yLLCorner, float cellsize, float initialValue, DO_TYPE type )
   {
   ClearPolygons();       // clear any existing polygons
   ClearData();   // and any associated data

   m_layerType = LT_GRID;

   // allocate data object
   switch( type )
      {
      case DOT_INT:
         m_pData = new IDataObj( cols, rows );
         break;

      case DOT_FLOAT:
         m_pData = new FDataObj( cols, rows );
         break;

      default:
         m_pData = new VDataObj( cols, rows );
         break;
      }

	COORD2d ll( xLLCorner, yLLCorner );
	SetLowerLeft   ( ll );
	SetCellSize    ( cellsize, cellsize );  // same size (square) ???
	SetNoDataValue( -9999 );

   for ( int row=0; row < rows; row++ )
      for ( int col=0; col < cols; col++ )
         m_pData->Set( col, row, initialValue );
      
   // attach data to a dbtable object 
   m_pDbTable = new DbTable( m_pData );

   // data loaded, clean up
   m_vxMax = m_vxMin + m_pData->GetColCount() * m_cellWidth;
   m_vyMax = m_vyMin + m_pData->GetRowCount() * m_cellHeight;

   // first layer?
   if ( m_pMap->GetLayerCount() == 1 )
      {
      m_pMap->SetMapExtents( m_vxMin, m_vxMax, m_vyMin, m_vyMax );
      m_pMap->ZoomFull( false );  // no redraw
      }
   else
      m_pMap->AddMapExtents( m_vxMin, m_vxMax, m_vyMin, m_vyMax );

   m_name = "Grid";
   m_path = "Grid.grd";

   // do default grid classification
   m_pMap->Notify( NT_CLASSIFYING, 0, 0 );
   float minVal, maxVal;
   GetDataMinMax( -1, &minVal, &maxVal );
   
   SetBins( minVal, maxVal, 10 );
   
   return TRUE;
   }



//-- MapLayer::LoadGrid() -------------------------------------------
//
//-- loads an ArcInfo-exported ascii grid file
//-------------------------------------------------------------------

int MapLayer::LoadGrid( LPCTSTR filename, DO_TYPE type, int maxLineWidth, int binCount /*=10*/ )
   {
   ClearPolygons();       // clear any existing polygons
   ClearData();   // and any associated data

   m_layerType = LT_GRID;

   FILE *fp = fopen( filename, "rb" );

   if ( fp == NULL )
      {
      CString msg( "Unable to find file " );
      msg += filename;
      ErrorMsg( msg );
      return -1;
      }

   char *buffer = new char[ 100 ]; 
   
   //-- get header information -----------------------
   //
   //-- NOTE: Header looks like: (starts in col 0)
   //
   //   NCOLS 200
   //   NROWS 120
   //   XLLCORNER 60
   //   YLLCORNER -10
   //   CELLSIZE 0.5
   //   NODATA_VALUE -99.9
   //-------------------------------------------------
   int   nCols, nRows, xLLCorner, yLLCorner;
   float cellSize, noDataValue;
   char varname[ 32 ];

   char *err = fgets( buffer, 100, fp );
   ASSERT( err != NULL );
   sscanf( buffer, "%s %d", varname, &nCols );

   err = fgets( buffer, 100, fp );
   ASSERT( err != NULL );
   sscanf( buffer, "%s %d", varname, &nRows );

   err = fgets( buffer, 100, fp );
   ASSERT( err != NULL );
   sscanf( buffer, "%s %d", varname, &xLLCorner );

   err = fgets( buffer, 100, fp );
   ASSERT( err != NULL );
   sscanf( buffer, "%s %d", varname, &yLLCorner );

   err = fgets( buffer, 100, fp );
   ASSERT( err != NULL );
   sscanf( buffer, "%s %f", varname, &cellSize );

   fgets( buffer, 100, fp );
	sscanf( buffer, "%s %f", varname, &noDataValue );

   // allocate data object
   switch( type )
      {
      case DOT_INT:
         m_pData = new IDataObj( nCols, nRows );
         break;

      case DOT_FLOAT:
         m_pData = new FDataObj( nCols, nRows );
         break;

      case DOT_VDATA:
         m_pData = new VDataObj( nCols, nRows );
         break;

      default:
         ErrorMsg( "Unsupported grid data type specified in LoadGrid()" );
         return -2;
      }

	COORD2d ll( (float) xLLCorner, (float) yLLCorner );
	SetLowerLeft   ( ll );
	SetCellSize    ( cellSize, cellSize );  // same size (square) ???
	//SetDataDim     ( nCols, nRows );
	SetNoDataValue( noDataValue );

   delete [] buffer;

   // if not specified, figure out line width 
   if ( maxLineWidth <= 0 )
      {
      fpos_t pos;
      if ( fgetpos( fp, &pos ) != 0 )
         {
         ErrorMsg( " Error setting file position" );
         return -1;
         }

      int count = 0;
      int c = fgetc( fp );
      while (  c != '\n' && c != '\r' )
         {
         c = fgetc( fp );
         count++;
         }

      fsetpos( fp, &pos );
      maxLineWidth = count*2;
      }

   buffer = new char[ maxLineWidth ];


   //-- have header, proceed with grid data --//
   int row = 0;
   for ( row=0; row < nRows; row++ )
      {
      m_pMap->Notify( NT_LOADGRIDROW, row, 0 );

      fgets( buffer, maxLineWidth, fp );

      char *p = strtok( buffer, " \n" );

      for ( int col=0; col < nCols; col++ )
         {
         if ( p == NULL )
            {
            char msg[ 128 ];
            sprintf( msg, "Premature end-of-line encountered at row %d, col %d (of %d) (Line Width=%i)", row, col, nCols, maxLineWidth );
            ErrorMsg( msg );
            return FALSE;
            }

         switch ( type )
            {
            case DOT_INT:
               {
               int value = atoi( p );
               m_pData->Set( col, row, value );
               }
               break;

            default:
               {
               float value = (float) atof( p );
               m_pData->Set( col, row, value );
               }
               break;
            }

         p = strtok( NULL, " \n" );  // parse next value

         }  // end of:  for ( col < nCols )

      }  // end of:  for ( row < nRows )

   // attach data to a dbtable object 
   m_pDbTable = new DbTable( m_pData );

   // data loaded, clean up
   m_vxMax = m_vxMin + m_pData->GetColCount() * m_cellWidth;
   m_vyMax = m_vyMin + m_pData->GetRowCount() * m_cellHeight;

   // first layer?
   if ( m_pMap->GetLayerCount() == 1 )
      {
      m_pMap->SetMapExtents( m_vxMin, m_vxMax, m_vyMin, m_vyMax );
      m_pMap->ZoomFull( false );  // no redraw
      }
   else
      m_pMap->AddMapExtents( m_vxMin, m_vxMax, m_vyMin, m_vyMax );   // must do this prior to  InitPolyLogicalPoints()

   //InitPolyLogicalPoints();   // create logical points for this set   

   //-- clean up --// 
   delete [] buffer;
   m_pMap->Notify( NT_LOADGRIDROW, -row, 0 );

   fclose( fp );

   m_name = filename;
   m_path = filename;

   if ( binCount > 0 )
      {
      // do default grid classification
      m_pMap->Notify( NT_CLASSIFYING, 0, 0 );
      float minVal, maxVal;

      GetDataMinMax( -1, &minVal, &maxVal );
      SetBins( minVal, maxVal, 10 );
      }

   return TRUE;
   }



int MapLayer::SaveShapeFile( LPCTSTR filename, bool selectedPolysOnly )
   {   
   FILE *fp = fopen( filename, "wb" );

   if ( fp == NULL )
      {
      CString msg( "Unable to find file " );
      msg += filename;
      ErrorMsg( msg );
      return -1;
      }

//   return 1;

   // get file type
   int fileType = -1;

   switch( GetType() )
      {
      case LT_LINE:
         fileType = 3;
         break;

      case LT_POLYGON:
         fileType = 5;
         break;

      case LT_POINT:
         fileType = 1;
      }

   if ( fileType < 0 )
      {
      ErrorMsg( "Unsupported file type - only 2D and 3D polygon and line types are saveable..." );
      return -1;
      }

   if ( m_3D )
      fileType += 10;

   // get file size
	int fileSize = 50;  // for header

   int count = selectedPolysOnly ? GetQueryCount() : GetRecordCount();

   fileSize +=  4 * count;    // record number + content length

   for ( int i=0; i < count; i++ )
      {
      switch( GetType() )
         {
         case LT_POLYGON:
         case LT_LINE:
            {
            fileSize +=  2;    // shape type
            fileSize += 16;    // bounding box
            fileSize +=  4;    // number of part/points

            int offset = selectedPolysOnly ? GetQueryResult( i ) : i;
            Poly *pPoly = GetPolygon( offset );

            int numParts = pPoly->m_vertexPartArray.GetSize();
            fileSize += 2 * numParts;    // 2 for part   

            int numPoints = pPoly->GetVertexCount();
            fileSize += 8 * numPoints;

            if ( m_3D )
               {
               fileSize += 8;    // z range
               fileSize += 4 * numPoints;  // z values

               fileSize += 8;    // M range
               fileSize = 4 * numPoints;
               }
            }
            break;
         
         case LT_POINT:
            fileSize += 2;    // shape type
            fileSize += 8;    // x,y coords

            if ( m_3D )
               fileSize += 8;    // z,m

            break;
         }
      }  // end of:  for ( i < numRecords )

   // have file size, start writing file   

   fseek( fp, 0, SEEK_SET );

   PutBigDWORD( 9994, fp );      // file code
   PutBigDWORD( 0, fp );         // 20 bytes of padding
   PutBigDWORD( 0, fp );
   PutBigDWORD( 0, fp );
   PutBigDWORD( 0, fp );
   PutBigDWORD( 0, fp );

   PutBigDWORD( fileSize, fp );     // file length (in 16-bit words)
   PutLittleDWORD( 1000, fp );      // version

   PutLittleDWORD( fileType, fp );

   PutLittleDouble( m_vxMin, fp );      // xmin
   PutLittleDouble( m_vyMin, fp );      // ymin
   PutLittleDouble( m_vxMax, fp );      // xmax
   PutLittleDouble( m_vyMax, fp );      // ymax
   PutLittleDouble( m_vzMin, fp );      // zmin
   PutLittleDouble( m_vzMax, fp );      // zmax
   PutLittleDouble( 0.0, fp );          // mmin
   PutLittleDouble( 0.0, fp );          // mmax

   // file header finished, now start writing record information
   fseek( fp, 100, SEEK_SET );
   for ( i=0; i < count; i++ )
      {
      //---- record header ---------//
      int offset = selectedPolysOnly ? GetQueryResult( i ) : i;
      Poly *pPoly = GetPolygon( offset );

      // first, do record header
      PutBigDWORD( i+1, fp );             // record number

      // get content length of this record (16-bit words)
      int contentLength = 2;  // shape type

      switch( GetType() )
         {
         case LT_POLYGON:
         case LT_LINE:
            contentLength += 16;    // bounding box
            contentLength += 4;     // num parts + num Points
            contentLength += pPoly->m_vertexPartArray.GetSize() * 2;    // parts array
            contentLength += pPoly->GetVertexCount() * 8;               // points array

            if ( m_3D )
               {
               contentLength += 8;  // z range
               contentLength += 4 * pPoly->GetVertexCount();   // each z value
               contentLength += 8;  // m range
               contentLength += 4 * pPoly->GetVertexCount();   // each m value
               }
            break;

         case LT_POINT:
            contentLength += m_3D ? 16 : 8;
            break;
         }

      PutBigDWORD( contentLength, fp );   // content length

      //------- now, do the record contents --------------
      // type
      PutLittleDWORD( fileType, fp );

      switch( GetType() )
         {
         case LT_POLYGON:
         case LT_LINE:
            {
            // bounding box
            float xMin, yMin, xMax, yMax;
            pPoly->GetBoundingRect( xMin, yMin, xMax, yMax );
            PutLittleDouble( xMin, fp );
            PutLittleDouble( yMin, fp );
            PutLittleDouble( xMax, fp );
            PutLittleDouble( yMax, fp );

            // number of parts
            int numParts = pPoly->m_vertexPartArray.GetSize();
            PutLittleDWORD( numParts, fp );

            int numPoints = pPoly->GetVertexCount();
            PutLittleDWORD( numPoints, fp );

            for ( int k=0; k < numParts; k++ )
               PutLittleDWORD( pPoly->m_vertexPartArray[ k ], fp );
      
            for ( k=0; k < numPoints; k++ )
               {
               const Vertex &vertex = pPoly->GetVertex( k );
               PutLittleDouble( vertex.x, fp );
               PutLittleDouble( vertex.y, fp );
               }

            // done with 2D shapes.  For 3D shapes, add the following
            if ( m_3D )
               {
               // z range
               float zMin, zMax;
               pPoly->GetBoundingZ( zMin, zMax );
               PutLittleDouble( zMin, fp );
               PutLittleDouble( zMax, fp );

               // z Array
               for ( k=0; k < numPoints; k++ )
                  PutLittleDouble( pPoly->GetVertex( k ).z, fp );
            
               // M range
               PutLittleDouble( 0.0, fp );
               PutLittleDouble( 0.0, fp );

               // M Array
               for ( k=0; k < numPoints; k++ )
                  PutLittleDouble( 0.0, fp );
               }  // end of:  if ( m_3d )
            }
            break;   // end of:  case LT_POLY, LT_LINE

         case LT_POINT:
            PutLittleDouble( pPoly->GetVertex( 0 ).x, fp );
            PutLittleDouble( pPoly->GetVertex( 0 ).y, fp );
            if ( m_3D )
               {
               PutLittleDouble( pPoly->GetVertex( 0 ).z, fp );
               PutLittleDouble( 0.0, fp );     // measure
               }
            break;
         }
      }  // end of:  for ( i < numRecords )

   fclose( fp );

   // get corresponding dbf file
   char *dbf = new char[ lstrlen( filename ) + 1 ];
   lstrcpy( dbf, filename );
   char *p = strrchr( dbf, '.' );
   p++;
   lstrcpy( p, _T( "dbf" ) );

   // get path of shape file
   p = strrchr( dbf, '\\' );

   if ( p == NULL )  // no path
      SaveDataDB( filename );
   else
      {
      p++;

      char dbfilename[ 128 ];
      lstrcpy( dbfilename, p );
      *p = NULL;
      SaveDataDB( dbfilename );
      }

   delete [] dbf;

	// write corresponding index file

   // get corresponding shx file
   char *shx = new char[ lstrlen( filename ) + 1 ];
   lstrcpy( shx, filename );
   char *pindex = strrchr( shx, '.' );
   pindex++;
   lstrcpy( pindex, _T( "shx" ) );

   FILE *fpi = fopen( shx, "wb" );

   delete [] shx;
   
   fseek( fpi, 0, SEEK_SET );

   if ( fpi == NULL )
      {
      CString msg( "Unable to find file " );
      msg += filename;
      ErrorMsg( msg );
      return -1;
      }

//   return 1;

   // get file size
   fileSize = 50;  // This is the file size of the header, not the main file
   count = selectedPolysOnly ? GetQueryCount() : GetRecordCount();
	fileSize += count * 4;  //  The total size of the index file is 50 (for the header) + 4 * the number of records 
  
   PutBigDWORD( 9994, fpi );      // file code
   PutBigDWORD( 0, fpi );         // 20 bytes of padding
   PutBigDWORD( 0, fpi );
   PutBigDWORD( 0, fpi );
   PutBigDWORD( 0, fpi );
   PutBigDWORD( 0, fpi );

   PutBigDWORD( fileSize, fpi );     // file length (in 16-bit words)
   PutLittleDWORD( 1000, fpi );      // version

   PutLittleDWORD( fileType, fpi );

   PutLittleDouble( m_vxMin, fpi );      // xmin
   PutLittleDouble( m_vyMin, fpi );      // ymin
   PutLittleDouble( m_vxMax, fpi );      // xmax
   PutLittleDouble( m_vyMax, fpi );      // ymax
   PutLittleDouble( m_vzMin, fpi );      // zmin
   PutLittleDouble( m_vzMax, fpi );      // zmax
   PutLittleDouble( 0.0, fpi );          // mmin
   PutLittleDouble( 0.0, fpi );          // mmax

   fseek( fpi, 100, SEEK_SET );

   // file header finished, now start writing record information

	int offset = 50;

	PutBigDWORD( offset, fpi );  //  There are 50 16 bit words in the first record (the header) so...

   for ( i=0; i < count; i++ )
      {
      int numPolys = selectedPolysOnly ? GetQueryResult( i ) : i;
      Poly *pPoly = GetPolygon( numPolys );


      // get content length of this record (16-bit words)
      int contentLength = 2;  // shape type
      contentLength += 16;    // bounding box
      contentLength += 4;     // num parts + num Points
      contentLength += pPoly->m_vertexPartArray.GetSize() * 2;    // parts array
      contentLength += pPoly->GetVertexCount() * 8;               // points array

      if ( m_3D )
         {
         contentLength += 8;  // z range
         contentLength += 4 * pPoly->GetVertexCount();   // each z value
         contentLength += 8;  // m range
         contentLength += 4 * pPoly->GetVertexCount();   // each m value
         }

		offset += contentLength + 4;
      PutBigDWORD( contentLength, fpi );   // content length

		PutBigDWORD( offset, fpi );
 
		
		} //end of record count, index file is finished...
	
	fclose( fpi );

   
   return fileSize;
   }



//-- MapLayer::SaveGridFile() -------------------------------------
//
//-- save a grid layer in ArcInfo-exported ascii grid file format
//-------------------------------------------------------------------

bool MapLayer::SaveGridFile( LPCSTR filename ) //, int maxLineLength )
   {
   FILE *fp = fopen( filename, "wt" );

   if ( fp == NULL )
      return false;

   char buffer[ 128 ];

   //-- set header information -----------------------
   //
   //-- NOTE: Header looks like: (starts in col 0)
   //
   //   NCOLS 200
   //   NROWS 120
   //   XLLCORNER 60
   //   YLLCORNER -10
   //   CELLSIZE 0.5
   //   NODATA_VALUE -99.9
   //-------------------------------------------------
	int cols, rows;
   GetDataDim( cols, rows );
	COORD2d ll;
   GetLowerLeft( ll );

   float width, height;
   GetCellSize( width, height );

   sprintf( buffer, "NCOLS %d\n", cols );
   fputs( buffer, fp );

   sprintf( buffer, "NROWS %d\n", rows );
   fputs( buffer, fp );

	sprintf( buffer, "XLLCORNER %d\n", (int) ll.x );
   fputs( buffer, fp );

   sprintf( buffer, "YLLCORNER %d\n", (int) ll.y );
   fputs( buffer, fp );

   sprintf( buffer, "CELLSIZE %f\n", width );  // assumes square cells
   fputs( buffer, fp );

	sprintf( buffer, "NODATA_VALUE %f\n", GetNoDataValue() );
   fputs( buffer, fp );

   //-- have header, proceed with grid data --//
   for ( int row=0; row < rows; row++ )
      {
      for ( int col=0; col < cols; col++ )
         {
         CString str( m_pData->GetAsString( col, row ) );
         fputs( (LPCSTR) str, fp );

         if ( col != cols-1 )
            fputs( " ", fp );
         else
            fputc( '\n', fp );
         }  // end of:  for ( col < nCols )
      }  // end of:  for ( row < nRows )

   fclose( fp );

   return true;
   }



void MapLayer::SetGridToNoData( void )
   {
   ASSERT( m_layerType == LT_GRID );

   int rows = GetRecordCount();
   int cols = GetFieldCount();

   //-- have header, proceed with grid data --//
   for ( int row=0; row < rows; row++ )
      for ( int col=0; col < cols; col++ )
       m_pData->Set( col, row, m_noDataValue );

   //SetBins( m_noDataValue, m_noDataValue, 1 );
   ClassifyData();

   return;
   }


DataObj *MapLayer::CreateDataTable( int rows, int cols, DO_TYPE type /*= DOT_VDATA*/ )
   {
   ASSERT( m_pDbTable == NULL );
   ASSERT( m_pData == NULL );

   // allocate data object
   switch( type )
      {
      case DOT_INT:
         m_pData = new IDataObj( cols, rows );
         break;

      case DOT_FLOAT:
         m_pData = new FDataObj( cols, rows );
         break;

      default:
         m_pData = new VDataObj( cols, rows );
         break;
      }

   //for ( int row=0; row < rows; row++ )
   //   for ( int col=0; col < cols; col++ )
   //      m_pData->Set( col, row, initialValue );
      
   // attach data to a dbtable object 
   m_pDbTable = new DbTable( m_pData );

   return m_pData;
   }



void  MapLayer::SetDataDim( int nCols, int nRows )
   {
   m_pData->SetSize( nCols, nRows );
   UpdateExtents();

   m_pMap->UpdateMapExtents();
   }


int MapLayer::AddPoint( REAL x, REAL y )
   {
   if ( m_layerType != LT_POINT )
      return -1;

   Poly *pPoly = new Poly;
   pPoly->AddVertex( Vertex( x, y ) );
   pPoly->InitLogicalPoints( m_pMap );

   // adjust layer extents if necessary
   if ( x < m_vxMin ) m_vxMin = x;
   if ( x > m_vxMax ) m_vxMax = x;
   if ( y < m_vyMin ) m_vyMin = y;
   if ( y > m_vyMax ) m_vyMax = y;

   if ( ( m_vxMax-m_vxMin ) > ( m_vyMax - m_vyMin ) )
      m_vExtent = m_vxMax - m_vxMin;
   else
      m_vExtent = m_vyMax - m_vyMin;

   return m_polyArray.Add( pPoly );
   }


bool MapLayer::GetPointCoords( int index, REAL &x, REAL &y ) const
   {
   if ( m_layerType != LT_POINT )
      return false;

   if ( index >= m_polyArray.GetSize() )
      return false;

   Poly *pPoly = GetPolygon( index );

   x = pPoly->GetVertex(0).x;
   y = pPoly->GetVertex(0).y;

   return true;
   }


bool  MapLayer::GetDataDim( int &cols, int &rows ) const
   { 
   if ( m_pData == NULL ) 
      return false; 
   
   cols = m_pData->GetColCount();
   rows = m_pData->GetRowCount();
   return true; 
   }
      

void  MapLayer::SetLowerLeft( COORD2d ll )
   {
   m_vxMin = ll.x;
   m_vyMin = ll.y;

   // data loaded, clean up
   m_vxMax = m_vxMin + m_pData->GetColCount() * m_cellWidth;
   m_vyMax = m_vyMin + m_pData->GetRowCount() * m_cellHeight;

   if ( m_pMap->GetLayerCount() == 1 )
      {
      m_pMap->SetMapExtents( m_vxMin, m_vxMax, m_vyMin, m_vyMax );
      m_pMap->ZoomFull( false );  // no redraw
      }
   }



int MapLayer::LoadDataDB( LPCTSTR databaseName, LPCTSTR connectStr, LPCTSTR sql, int extraCols /* =0 */, int records /*=-1*/)
   {
   // note:  connectStr = "" for Access, "DBASE IV;" for dbase
   int recordCount = 0;

   CDaoDatabase database;

	try
		{
		// open the database ( non-exclusive, read/write )
		database.Open( databaseName, FALSE, FALSE, connectStr );
      recordCount = LoadRecords( database, sql, extraCols, records ); 
      ASSERT( recordCount >= 0 ); 
      }

	catch( CDaoException *p )
		{
		CString msg("Error opening database " );
      msg += databaseName;
      msg += ":  ";
      msg += p->m_pErrorInfo->m_strDescription;
		m_pMap->MessageBox(msg, "Exception", MB_OK );
		}

   database.Close();
   m_database = databaseName;

   return recordCount;
   }
   
   
int MapLayer::LoadRecords( CDaoDatabase &database, LPCTSTR sql, int extraCols /* =0 */, int records /*=-1*/ )
   {
	CDaoRecordset rs( &database );

   if ( m_pData )
      delete m_pData;

   //switch( type )
   //   {
   //   case DOT_INT:
   //      m_pData = new IDataObj;
   //      break;

   //   case DOT_FLOAT:
   //      m_pData = new FDataObj;
   //      break;

   //   default:
         m_pData = new VDataObj;
   //      break;
   //   }

   try
		{
		rs.Open( dbOpenSnapshot, sql);
		rs.MoveFirst();    // move to first record

      // figure out what the table structure of the query results are.
      // For each float field, create a corresponding data object column
      int fieldCount = rs.GetFieldCount( );

      m_pData->SetSize( fieldCount+extraCols, 0 );  // make a data column for each field
      m_pDbTable = new DbTable( m_pData ); // make a corresponding DbTable and attach to this data object

      // set the labels --//
      for ( int i=0; i < fieldCount; i++ )
         {
         CDaoFieldInfo info;
         rs.GetFieldInfo( i, info );

         m_pData->AddLabel( (LPCTSTR) info.m_strName );
         
         switch ( info.m_nType )
            {
            case dbBoolean:  m_pDbTable->SetFieldType( i, TYPE_BOOL );   break;
            case dbByte:     m_pDbTable->SetFieldType( i, TYPE_CHAR );   break;
            case dbLong:     m_pDbTable->SetFieldType( i, TYPE_LONG );   break;
            case dbInteger:  m_pDbTable->SetFieldType( i, TYPE_INT );    break;
            case dbSingle:   m_pDbTable->SetFieldType( i, TYPE_FLOAT );  break;
            case dbDouble:   m_pDbTable->SetFieldType( i, TYPE_DOUBLE ); break;
            case dbText:     m_pDbTable->SetFieldType( i, TYPE_STRING ); break;
				case dbDate:     m_pDbTable->SetFieldType( i, TYPE_FLOAT  ); break;
            default:
               {
               CString msg;
               msg.Format( "Unsupported type in MapLayer::LoadRecords() reading file %s, field %s", (LPCSTR) m_path, (LPCSTR) info.m_strName ); 
               WarningMsg( msg );
               }
            }

         m_pDbTable->ShowField( i, true );
         //m_attribInfoArray[ i ].show = true;
         }

      // add extra fields
      for ( i=0; i < extraCols; i++ )
         {
         CString label;
         label.Format( "EXTRA_%i", i+1 );

         m_pData->AddLabel( (LPCTSTR) label );         
         m_pDbTable->SetFieldType( fieldCount+i, TYPE_LONG );  // make long by default
         m_pDbTable->GetFieldInfo( fieldCount+i ).save = false;   // make saving extra cols off by default
         }
      

      // have basic structure, now populate fields
      COleVariant *array = new COleVariant[ fieldCount+extraCols ];
      int count = 0;

		while ( rs.IsEOF() == FALSE )
			{
         for ( i=0; i < fieldCount; i++ )
            {
			   try 
               {
               rs.GetFieldValue( i, array[ i ] );
   				}

	   		catch( CDaoException* )
		   		{
			   	CString msg("DAO Exception getting field value." );
				   m_pMap->MessageBox( msg, "Exception", MB_OK );
               return false;
				   }
			   catch ( CMemoryException* )
				   {
				   CString msg("DAO Memory Exception getting field value." );
				   m_pMap->MessageBox( msg, "Exception", MB_OK );
               return false;
				   }
            }  // end of for:

         for ( i=0; i < extraCols; i++ )
            array[ fieldCount+i ] = 0L;

			((VDataObj*) m_pData)->AppendRow( array );
			rs.MoveNext();
         count++;
         m_pMap->Notify( NT_LOADRECORD, count, 0 );

         if ( count == records )
            break;
			}  // end of: while( rs.IsEOF() )

      delete [] array;
		rs.Close();            
		}  // end of: try ( rsTables.Open )

	catch( CDaoException *p )
		{
		CString msg("Database Error opening table: SQL=" );
      msg += sql;
      msg += ":  ";
      msg += p->m_pErrorInfo->m_strDescription;

		m_pMap->MessageBox( msg, "Exception", MB_OK );
      return -1;
		}
	catch( CMemoryException* )
		{
		CString msg("Memory Error opening table: SQL=" );
      msg += sql;
		m_pMap->MessageBox( msg, "Exception", MB_OK );
      return -2;
		}

   return m_pData->GetRowCount();
   }


int MapLayer::SaveDataColDB( int col, LPCTSTR databaseName, LPCTSTR tableName, LPCTSTR connectStr ) const
   {
   ASSERT( m_pData != NULL );
   ASSERT( col < m_pData->GetColCount() );

   // note:  connectStr = "" for Access, "DBASE IV;" for dbase
   CDaoDatabase database;

	try
		{
		// open the database ( non-exclusive, read/write )
		database.Open( databaseName, FALSE, FALSE, connectStr );

      // get a recordset corresponding to the table
      CDaoTableDef td( &database );

      try{
         td.Open( tableName );
         }
      catch ( CDaoException* )
         {
  		   CString msg("Database Error opening table " );
         msg += tableName;
     	   m_pMap->MessageBox( msg, "Exception", MB_OK );
         return -2;
         }

      // have the table defition, open the table into a recordset
	   CDaoRecordset rs( &database );

	   try
		   {
		   rs.Open( &td );

         // start a transaction
         database.m_pWorkspace->BeginTrans();
         int recordCount = m_pData->GetRowCount();
         COleVariant value;
         
         rs.MoveFirst();
         for ( int row=0; row < recordCount; row++ )
            {
            rs.Edit();
            m_pData->Get( col, row, value );
            rs.SetFieldValue( col, value );
            rs.Update();
            rs.MoveNext();
            }
			
         database.m_pWorkspace->CommitTrans();
         rs.Close();
		   }  // end of: try ( rs.Open )

   	catch( CDaoException* )
		   {
		   CString msg("Database Error opening/writing table: " );
         msg += tableName;
		   m_pMap->MessageBox( msg, "Exception", MB_OK );
         return -3;
		   }
      td.Close();
      }

	catch( CDaoException* )
		{
		CString msg("Database Error opening database: " );
      msg += databaseName;
		m_pMap->MessageBox( msg, "Exception", MB_OK );
      return -1;
		}

	catch( CMemoryException* )
		{
		CString msg("Memory Error opening database: " );
      msg += databaseName;
		m_pMap->MessageBox( msg, "Exception", MB_OK );
      return -2;
		}
   
   database.Close();
   return m_pData->GetRowCount();
   }


int MapLayer::SaveDataDB() const
   {
   //CString    m_database;   // full path for database file (no filename for DBase)
   //CString    m_tableName;  // table name (file name for DBase)
   return SaveDataDB( m_database, m_tableName, "DBASE IV;", false );
   }


int MapLayer::SaveDataDB( LPCTSTR databaseName, LPCTSTR tableName, LPCTSTR connectStr, bool selectedPolysOnly ) const
   {
   ASSERT( m_pData != NULL );

   if ( lstrcmpi( connectStr, "DBase IV;" ) == 0 )
      {
      m_pDbTable->SaveDataDB( databaseName );

      return m_pData->GetRowCount();
      }

   // note:  connectStr = "" for Access, "DBASE IV;" for dbase
   CDaoDatabase database;

	try
		{
		// open the database ( non-exclusive, read/write )
      database.Create( databaseName );

		//database.Open( databaseName, FALSE, FALSE, connectStr );
      bool ok = CreateTable( database, tableName, connectStr );

      if ( ok )
         SaveRecords( database, tableName );
      }

	catch( CDaoException *e )
		{
		CString msg("Error creating database " );
      msg += databaseName;
      msg += ": ";
      msg += e->m_pErrorInfo->m_strDescription;
		m_pMap->MessageBox(msg, "Exception", MB_OK );
		}

   database.Close();
   return m_pData->GetRowCount();
   }
   

  // helper function
bool MapLayer::CreateTable( CDaoDatabase &database, LPCTSTR tableName, LPCTSTR connectStr ) const
   {
   // delete the specified table from the database
   //char *p = (char*) tableName;
   char p[ 16 ];
   lstrcpy( p, "cell" );
   /*
   try 
      {
      database.DeleteTableDef( "cell.dbf" );  // tableName
      }

   catch( CDaoException *e )
      {
		CString msg("Database Error deleting table " );
      msg += tableName;
      msg += ": ";
      msg += e->m_pErrorInfo->m_strDescription;
		m_pMap->MessageBox( msg, "Warning", MB_OK );
      }
*/
   // current table deleted, create a new one corresponding to the data object
   CDaoTableDef td( &database );
   try
      {
      td.Create( tableName, 0, NULL, connectStr );
      }

   catch( CDaoException *e )
      {
		CString msg("Database Error creating table " );
      msg += tableName;
      msg += ": ";
      msg += e->m_pErrorInfo->m_strDescription;
		m_pMap->MessageBox( msg, "Exception", MB_OK );
      return false;
      }

   // table created, start adding fields
   int rows = m_pData->GetRowCount();

   FILE *fp = fopen( "fields.txt", "wt" );

   for ( int i=0; i < m_pData->GetColCount(); i++ )
      {
      LPCTSTR fieldName = m_pData->GetLabel( i );
      fprintf( fp, "%s\n", fieldName );

      short type = -1;
      //int   fieldType = m_pData->GetType( i );

      TYPE fieldType = m_pDbTable->GetFieldType ( i );

      switch( fieldType )
         {
         case TYPE_INT:
         case TYPE_UINT:
         case TYPE_LONG:
         case TYPE_ULONG:     type = dbLong;      break; 
         case TYPE_FLOAT:     type = dbSingle;    break;
         case TYPE_DOUBLE:    type = dbDouble;    break;
         case TYPE_STRING:
         case TYPE_DSTRING:   type = dbText;      break;
         case TYPE_BOOL:      type = dbBoolean;   break;  // Boolean; True=-1, False=0.
         default:
            {
            CString msg;
            msg.Format( "Unsupported field type at col %d", i );
            ErrorMsg( msg );
            continue;
            }
         }

      int size = 0;
      if ( type == dbText )
         {
         // make sure it will hold the longest text
         for ( int j=0; j < rows; j++ )
            {
            CString val = m_pData->GetAsString( i, j );
            if ( val.GetLength() > size )
               size = val.GetLength();
            }

         size += 10; // for good measure
         if ( size > 255 )
            size = 255;
         }

      try
         {
         td.CreateField( fieldName, type, size, dbUpdatableField | dbFixedField );
         }

      catch( CDaoException *e )
         {
     		CString msg("Database Error creating field definition " );
         msg += tableName;
         msg += ", ";
         msg += fieldName;
         msg += ": ";
         msg += e->m_pErrorInfo->m_strDescription;
	   	m_pMap->MessageBox( msg, "Exception", MB_OK );
         continue;
         }
      }  // end of field creation

   try
      {
      td.Append();      // add the table to the database table collection
      }

   catch( CDaoException *e )
      {
     	CString msg("Database Error appending table :  " );
      msg += tableName;
      msg += ": ";
      msg += e->m_pErrorInfo->m_strDescription;
	   m_pMap->MessageBox( msg, "Exception", MB_OK );
      }

   fclose( fp );
   return true;
   }



int MapLayer::SaveRecords( CDaoDatabase &database, LPCTSTR tableName ) const
   {
   CDaoTableDef td( &database );

   try{
      td.Open( tableName );
      }
   catch ( CDaoException *e )
      {
  		CString msg("Database Error opening table " );
      msg += tableName;
      msg += ": ";
      msg += e->m_pErrorInfo->m_strDescription;
     	m_pMap->MessageBox( msg, "Exception", MB_OK );
      return -1;
      }

   // have the table defition, open the table into a recordset
	CDaoRecordset rs( &database );

	try
		{
		rs.Open( &td, dbOpenTable, dbAppendOnly );

      // start a transaction
      //database.m_pWorkspace->BeginTrans();

      int recordCount = m_pData->GetRowCount();
      COleVariant value;

      for ( int row=0; row < recordCount; row++ )
         {
         rs.AddNew();
         //rs.SetBookmark( rs.GetLastModifiedBookmark( ) );
         //rs.MoveLast();

         // add data to the recordset
         int cols = m_pData->GetColCount();
         ASSERT( cols == rs.GetFieldCount() );

         for ( int col=0; col < cols; col++ )
            {
            m_pData->Get( col, row, value );
            rs.SetFieldValue( col, value );
            }

			rs.Update();
         rs.MoveLast();
			}  // end of: for ( row < recordCount )

      //database.m_pWorkspace->CommitTrans();
      rs.Close();
		}  // end of: try ( rsTables.Open )


	catch( CDaoException *e )
		{
		CString msg("Database Error on : " );
      msg += tableName;
      msg += ": ";
      msg += e->m_pErrorInfo->m_strDescription;

		m_pMap->MessageBox( msg, "Exception", MB_OK );
      return -1;
		}
	catch( CMemoryException* )
		{
		CString msg("Memory Error opening table: " );
      msg += tableName;
		m_pMap->MessageBox( msg, "Exception", MB_OK );
      return -2;
		}

   td.Close();

   return m_pData->GetRowCount();
   }




int MapLayer::SaveDataAscii( LPCTSTR filename, bool includeID, LPCTSTR colToWrite ) const
   {
   FILE *fp = fopen( filename, "wt" );

   if ( fp == NULL )
      return -1;

   // start loading
   int recordCount = m_pData->GetRowCount();

   if ( recordCount != m_polyArray.GetSize() )
      {
      m_pMap->MessageBox( "Database record count different that polygon count...Aborting", "Error", MB_OK );
      return -1;
      }
 
   char *id = new char[ 64 ]; // max size...
   int colID = -1;

   if ( colToWrite != NULL )
      colID = m_pData->GetCol( colToWrite );

   int colCount = m_pData->GetColCount();

   for ( int i=0; i < recordCount; i++ )
      {      
      m_pMap->Notify( NT_WRITEDATA, i, 0 );
      
      if ( includeID )
         {
         itoa( m_polyArray[ i ]->m_id, id, 10 );
         fputs( id, fp );
         fputs( ", ", fp );
         }

      if ( colToWrite == NULL )
         {
         for ( int j=0; j < colCount; j++ )
            {
            CString data = m_pData->GetAsString( j, i );   // col, row
            fputs( data, fp );

            if ( j != colCount-1 )
               fputs( ", ", fp );
            else
               fputc( '\n', fp );
            }
         }
      else  // col == NULL
         {
         CString data = m_pData->GetAsString( colID, i );   // col, row
         fputs( data, fp );
         fputc( '\n', fp );
         }
      }  // end of: for ( i < recordCount ) 
   
   delete [] id;
   fclose( fp );

   m_pMap->Notify( NT_WRITEDATA, -recordCount, 0 );
      
   return recordCount;
   }



void MapLayer::InitPolyLogicalPoints()
   {
   ASSERT( m_pMap != NULL );

   for ( int i=0; i < m_polyArray.GetSize(); i++ )
      m_polyArray[ i ]->InitLogicalPoints( m_pMap );
   }


int MapLayer::InitData( int cols, int rows, float value )
   {
   ASSERT( m_layerType != LT_GRID );  // Note:  this code assume a dataobj of type float - needs to be updated if ever called

   if ( m_pDbTable )    // clear out any existing data
      delete m_pDbTable;

   COleVariant *data = new COleVariant[ cols ];  // create temp array of COleVariants

   for ( int i=0; i < cols; i++ )    // load up column info
      {
      data[ i ].vt = VT_R4;
      data[ i ].fltVal = value;
      }

   m_pData = new VDataObj( cols, 0 );   // create a new data object to store data

   for ( i=0; i < rows; i++ )
      m_pData->AppendRow( data );       // add as many rows as needecd

   m_pDbTable = new DbTable( m_pData );

   delete [] data;

   return rows;
   }


void MapLayer::ClearPolygons()
   {
   for ( int i=0; i < m_polyArray.GetSize(); i++ )
      delete m_polyArray[ i ];

   m_polyArray.RemoveAll();
   }



void MapLayer::ClearData()
   {
   if ( m_pDbTable != NULL )
      delete m_pDbTable;

   m_pDbTable = NULL;
   m_pData = NULL;  
   }



void MapLayer::SetBins( float binMin, float binMax, int binCount, BinArray *pExtBinArray, LPCSTR units )
   {
   BinArray *pBinArray;

   if ( pExtBinArray == NULL )    // should we used our internal BinArray
      pBinArray = &m_binArray;
   else
      pBinArray = pExtBinArray;

   //-- resize the existing array to the new size.  Note:  If the old
   //   BinArray is shorter than the new array, extra elements will not
   //   be pointing to anything in particular.
   pBinArray->SetSize( binCount );
   pBinArray->m_binType = BT_NUMERIC;
  
   pBinArray->m_binMin = binMin;
   pBinArray->m_binMax = binMax;

   //-- okay, new bin array set up, now build up values and labels --//
   float binWidth = ( binMax - binMin ) / binCount;

	for ( int i=0; i < binCount; i++ )
		{
		Bin &bin = pBinArray->ElementAt( i );
		bin.m_minVal  = binMin + ( i * binWidth );
		bin.m_maxVal  = binMin + ( ( i+1 ) * binWidth );
      bin.m_popSize = 0;
      }

   // do nodata bin
   AddNoDataBin();
   ClassifyData();

   for ( i=0; i < binCount; i++ )      // set labels (except last nodata bin)
      {
		Bin &bin = pBinArray->ElementAt( i );
      BPOS binPos;
		if      ( i == 0 )          binPos = BP_FIRST;
		else if ( i == binCount-1 ) binPos = BP_LAST;
		else                        binPos = BP_MID;
      
      bin.SetDefaultLabel( BT_NUMERIC, m_legendFormat, binPos, m_showBinCount, m_showSingleValue, units );

      if ( m_binColorFlag == BCF_SINGLECOLOR )
         bin.SetColor( m_fillColor );
      else
         {
         COLORREF color = GetColor( i, binCount, m_binColorFlag );
         bin.SetColor( color );
         }
      }

   return;
   }


void MapLayer::SetBins( CStringArray &valueArray, BinArray *pExtBinArray/*=NULL*/, BINTYPE type/*=BT_STRING*/, LPCSTR units/*=NULL*/  )
   {
   BinArray *pBinArray;

   if ( pExtBinArray == NULL )    // should we used our internal BinArray
      pBinArray = &m_binArray;
   else
      pBinArray = pExtBinArray;

   //-- resize the existing array to the new size.  Note:  If the old
   //   BinArray is shorter than the new array, extra elements will not
   //   be pointing to anything in particular.
   int binCount = valueArray.GetSize(); // add one for no data
   pBinArray->SetSize( binCount );
   pBinArray->m_binType = type;  //BT_STRING;
   pBinArray->m_binMin = 0.0f;
   pBinArray->m_binMax = 0.0f;

   // check the type of the active field.  If it is numeric, or layer is a grid, maintain numeric info
   if ( type != BT_STRING || m_layerType == LT_GRID )  //??? need to check active field for numeric
      {
      pBinArray->m_binMin = (float) LONG_MAX;
      pBinArray->m_binMax = (float) LONG_MIN;

      for ( int i=0; i < binCount; i++ )
         {
         float value = (float) atof( valueArray[ i ] );

         if ( value < pBinArray->m_binMin )
            pBinArray->m_binMin = value;

         if ( value > pBinArray->m_binMax )
            pBinArray->m_binMax = value;
         }
      }
   
   for ( int i=0; i < binCount; i++ )
		{
		Bin &bin = pBinArray->ElementAt( i );
		bin.m_minVal  = 0.0f;
		bin.m_maxVal  = 0.0f;
      bin.m_strVal  = valueArray[ i ];
      bin.m_popSize = 0;

      if ( type != BT_STRING || m_layerType == LT_GRID )  //??? need to check active field for numeric
         {
         float value = (float) atof( valueArray[ i ] );
   		bin.m_minVal = value-0.00001f;
         bin.m_maxVal = value+0.00001f;
         }
      }

   AddNoDataBin();
   ClassifyData();

   for ( i=0; i < binCount; i++ )
      {   
		Bin &bin = pBinArray->ElementAt( i );
		BPOS binPos;
		if      ( i == 0 )          binPos = BP_FIRST;
		else if ( i == binCount-1 ) binPos = BP_LAST;
		else                        binPos = BP_MID;
      
      bin.SetDefaultLabel( BT_STRING, m_legendFormat, binPos, m_showBinCount, m_showSingleValue );

      if ( m_binColorFlag == BCF_SINGLECOLOR )
         bin.SetColor( m_fillColor );
      else
         bin.SetColor( GetColor( i, binCount, m_binColorFlag ) );
      }

   return;
   }


void MapLayer::AddNoDataBin()
   {
   Bin bin;
   bin.m_minVal = m_noDataValue - 1;
   bin.m_maxVal = m_noDataValue + 1;
   bin.m_popSize = 0;
   bin.m_color = RGB( 250, 250, 250 );    // white
   bin.m_label = "No Data";
   
   m_binArray.Add( bin );
   }
	


int MapLayer::SetUniqueBins( int col, BINTYPE type, LPCSTR units ) 
   {
   LAYER_TYPE layerType = GetType();

   if ( layerType == LT_GRID )
      col = -1;

   CStringArray values;
   GetUniqueValues( col, values );

   if ( layerType == LT_GRID )
      type = BT_NUMERIC;

   SetBins( values, NULL, type, units );

   return values.GetSize()+1;
   }

BINTYPE fBinType;
bool    fIncreasingOrder;
int SortBinProc( const void *elem1, const void *elem2 );

void MapLayer::SortBins( bool increasingOrder )
   {
   int binCount = GetBinCount();

   if ( binCount <= 0 )
      return;

   // have bins, now sort them...
   //const Bin** GetData( ) const
   Bin **newBinArray = new Bin*[ binCount ];   // an array of pointers to bins

   for ( int i=0; i < binCount; i++ )
      newBinArray[ i ] = &m_binArray[ i ];

   fIncreasingOrder =  increasingOrder;
   fBinType = m_binArray.m_binType;

   // sort the pointers
   qsort( (void*) newBinArray, binCount-1, sizeof( Bin* ), SortBinProc );  // binCount-1, because the nodata bin will be the last one, not sorted.

   // copy into new array
   BinArray tempBinArray;
   for ( i=0; i < binCount; i++ )
      tempBinArray.Add( *(newBinArray[ i ]) );  // make a copy of the binArray

   m_binArray.RemoveAll();

   for ( i=0; i < binCount; i++ )
      m_binArray.Add( tempBinArray[ i ] );

   delete [] newBinArray;
   }


int SortBinProc( const void *elem1, const void *elem2 )
   {
   Bin *pBin0 = *((Bin**) elem1);
   Bin *pBin1 = *((Bin**) elem2);

   switch( fBinType )
      {
      case BT_NUMERIC:
         {
         float dif =  pBin0->m_minVal - pBin1->m_minVal;

         if ( fIncreasingOrder )
            dif = -dif;

         if ( dif > 0 )
            return 1;
         else 
            return -1;
         }

      case BT_STRING:
         {
         int result = lstrcmpi( pBin0->m_strVal, pBin1->m_strVal );

         if ( fIncreasingOrder )
            result = -result;

         return result;
         }
      }

   return 0;
   }


//-- Map::ClassifyData() --------------------------------------------
//
//-- classifies the layers data based on passed in values.  Generates 
//   population counts for each bin only.  Assumes bins have already
//   been specified and create using SetBin() family of methods
//-------------------------------------------------------------------

bool MapLayer::ClassifyData( int fieldCol /* default=-1, ignored for grids */, int /* scheme */ )
   {
   if ( fieldCol < 0 && m_layerType != LT_GRID )
      fieldCol = GetActiveField();

   if ( fieldCol < 0 )
      return false;

   if ( m_pData == NULL )       // no data to work with!
      return false;

   // set bin counts to zero
   for ( int i=0; i < m_binArray.GetSize(); i++ )
      m_binArray[ i ].m_popSize = 0;

   // for each record (polygon/line/grid), figure out which bin it is in
   Bin *pBin = NULL;

   if ( m_binArray.GetSize() == 0 )
      {
      ASSERT( 0 );
      return false;
      }

   switch( m_layerType )
      {
      case LT_POLYGON:
      case LT_LINE:
      case LT_POINT:
         {
         TYPE type = GetFieldType( fieldCol );

         for ( i=0; i < m_polyArray.GetSize(); i++ )
            {
            Poly *pPoly = m_polyArray[ i ];    // get the polygon

            bool displayPoly = CheckDisplayThreshold( i );    // display this one?

            VData v;
            m_pData->Get( fieldCol, i, v );

            if ( displayPoly == false || v.IsNull() )
               pBin = NULL;

            else  // displayPoly is true, so get the appropriate bin
               {
               switch( type )
                  {
                  case TYPE_STRING:
                  case TYPE_DSTRING:
                     {
                     CString attribValue = m_pData->GetAsString( fieldCol, i );
                     pBin = GetDataBin( attribValue );
                     ASSERT( pBin != NULL );
                     pBin->m_popSize++;
                     }
                     break;
            
                  case TYPE_FLOAT:
                  case TYPE_DOUBLE:
                     {
                     float attribValue = m_pData->GetAsFloat( fieldCol, i );   // assumes polys are ordered consist with dataobj
                     if ( m_useNoDataBin == false && attribValue == GetNoDataValue() )
                        pBin = NULL;
                     else
                        {
                        pBin = GetDataBin( attribValue );
                        ASSERT( pBin != NULL );
                        pBin->m_popSize++;
                        }
                     }
                     break;

                  case TYPE_UINT:
                  case TYPE_INT:
                  case TYPE_ULONG:
                  case TYPE_LONG:
                  case TYPE_BOOL:
                     {
                     int attribValue = m_pData->GetAsInt( fieldCol, i );   // assumes polys are ordered consist with dataobj
                     if ( m_useNoDataBin == false && attribValue == GetNoDataValue() )
                        pBin = NULL;
                     else
                        {
                        pBin = GetDataBin( attribValue );
                        ASSERT( pBin != NULL );
                        pBin->m_popSize++;
                        }
                     }
                     break;

                  default:
                     ErrorMsg( "Invalid Type encountered in ClassifyData()" );
                     goto outside_for;

                  }  // end of: switch
               }  // end of:  // else   displayPoly == true
      
             pPoly->m_pBin = (Bin*) pBin;
            }  // end of: for
outside_for:
            ;
         }
         break;   // end of: case LT_POLYGON|LT_LINE

      case LT_GRID:
         {
         int rows = m_pData->GetRowCount();
         int cols = m_pData->GetColCount();

         for ( int row=0; row < rows; row++ )
            {
            for ( int col=0; col < cols; col++ )
               {
               float value;
               bool ok = m_pData->Get( col, row, value );
               ASSERT( ok );

               if ( value != m_noDataValue )
                  {
                  pBin = GetDataBin( value );

                  if ( pBin != NULL )     //                  ASSERT( pBin != NULL );
                     pBin->m_popSize++;
                  }
               }  // end of: for ( col < cols )
            }  // end of: for ( row < rows )
         }
         break;

      default:
         return false;
      }

   return true;
   }


bool MapLayer::ClassifyPoly( int col, int index )
   {
   if ( col < 0 )
      col = m_activeField;

   ASSERT( index >= 0 );
   ASSERT( index < m_polyArray.GetSize() );

   Poly *pPoly = m_polyArray[ index ];    // get the polygon
   ASSERT( pPoly != NULL );

   if ( pPoly->m_pBin != NULL )
      pPoly->m_pBin->m_popSize--;

   Bin *pBin = NULL;

   switch( GetFieldType( col ) )
      {
      case TYPE_STRING:
      case TYPE_DSTRING:
         {
         CString attribValue = m_pData->GetAsString( col, index );
         pBin = GetDataBin( attribValue );
         }
         break;
            
      case TYPE_FLOAT:
      case TYPE_DOUBLE:
      case TYPE_INT:
      case TYPE_LONG:
         {
         float attribValue = m_pData->GetAsFloat( col, index );   // assumes polys are ordered consist with dataobj

         if ( m_useNoDataBin == false && attribValue == GetNoDataValue() )
             pBin = NULL;
         else
            pBin = GetDataBin( attribValue );
         }
         break;
      }  // end of: switch

   if( pBin != NULL )  //   ASSERT( pBin != NULL );
       pBin->m_popSize++;

   ASSERT( pBin != NULL );

   pPoly->m_pBin = pBin; 
   return true;
   
   }


COLORREF GetColor( int count, int maxCount, BCFLAG bcFlag )
   {
   //if ( bcFlag == BCF_SINGLECOLOR ) // managed prior to GetColor calls
   //   return (COLORREF) m_fillColor;

   //if ( bcFlag == BCF_TRANSPARENT )  // covered in Map::DrawMap().
   //   return (COLORREF) -1;

   count = maxCount - count;

   float ratio = (float) count / (float) maxCount;
	long red, green, blue;

   const int darkPrimary    = 210;
   const int lightPrimary   = 250;
   const int darkSecondary  = 0;
   const int lightSecondary = 227;

   switch( bcFlag )
      {
      case BCF_MIXED:
		   {
		   float oneThird = 1.0f /  3.0f;
		   float oneNinth = 1.0f /  9.0f;
		   float one27th  = 1.0f / 27.0f;

		   red   = (long) ((fmod( ratio, oneThird ) / oneThird) * 255);
		   green = (long) ((fmod( ratio, oneNinth ) / oneNinth) * 255);
		   blue  = (long) ((fmod( ratio, one27th  ) / one27th ) * 255);

         if ( red > 250 && green > 250 && blue > 250 )
            red = green = blue = lightSecondary;
		   }
         break;

      case BCF_RED:
         red   = long( darkPrimary + ratio*(lightPrimary-darkPrimary) );
         green = blue = long( darkSecondary + ratio * (lightSecondary-darkSecondary) );
         break;

      case BCF_GREEN:
         green = long( darkPrimary + ratio*(lightPrimary-darkPrimary) );
         red = blue = long( darkSecondary + ratio * (lightSecondary-darkSecondary) );
         break;

      case BCF_BLUE:
         blue = long( darkPrimary + ratio*(lightPrimary-darkPrimary) );
         red = green = long( darkSecondary + ratio * (lightSecondary-darkSecondary) );
         break;
           
      case BCF_REDGREEN:  // bluered  grn->blue, blue->red, red-<grn
         {
         blue = 0;
         int half = maxCount/2;   // maxCount = 5, half = 2;
         count--;

         if ( count < half )   // red?  ramp between 100, 255
            {
            ratio = (float) count / (float) half;
            green = long( darkPrimary + ratio*(lightPrimary-darkPrimary) );
            blue = red = long( darkSecondary + ratio * (lightSecondary-darkSecondary) );
            }
         else if ( count == half )
            return -1L;
         else
            {
            ratio = (float) (maxCount-count-1) / (float) half; 
            red = long( darkPrimary + ratio*(lightPrimary-darkPrimary) );
            green = blue = long( darkSecondary + ratio * (lightSecondary-darkSecondary) );
            }
         }
         break;

      case BCF_BLUEGREEN:
         {
         const float k = 0.5f;
         blue  = long( 255/(1+exp(-k*(count-maxCount/3))));
         green = long(255/(1+exp(-k*((maxCount-count)-maxCount/3))));
         red   = 0;
         }
         break;

      case BCF_BLUERED:
         {
         green = 0;
         int half = maxCount/2;   // maxCount = 5, half = 2;
         count--;

         if ( count < half )   // red?  ramp between 100, 255
            {
            ratio = (float) count / (float) half;
            red   = long( darkPrimary + ratio*(lightPrimary-darkPrimary) );
            green = blue = long( darkSecondary + ratio * (lightSecondary-darkSecondary) );
            }
         else if ( count == half )
            return -1L;
         else
            {
            ratio = (float) (maxCount-count-1) / (float) half; 
            blue = long( darkPrimary + ratio*(lightPrimary-darkPrimary) );
            red = green = long( darkSecondary + ratio * (lightSecondary-darkSecondary) );
            }
         }
      break;
         
      case BCF_GRAY:
   	   int intensity = (int) (ratio * lightSecondary);
		   red = green = blue =  intensity;
         break;
		}

	return RGB( red, green, blue );
	}


//-- MapLayer::CheckDisplayThreshold() -----------------------------------
//
//  Checks to see if the value given for the specified record passes the
//  display threshold test.
//
//  returns true  if test is passed, and the poly should be displayed
//          false if test false, and the poly should have a NULL bin
//------------------------------------------------------------------------  

bool MapLayer::CheckDisplayThreshold( int rec )
   {
   // not using thresholds?
   if ( m_useDisplayThreshold == 0 )
      return true;

   int displayCol;
   bool displayOnlyLarger;

   if ( m_useDisplayThreshold > 0  )
      { 
      displayCol = m_useDisplayThreshold - 1;
      displayOnlyLarger = true;
      }
   else
      {
      displayCol = -m_useDisplayThreshold - 1;
      displayOnlyLarger = false;
      }

   float displayColValue;
   bool ok = GetData( rec, displayCol, displayColValue );

   if ( displayOnlyLarger )
      return  displayColValue >= m_displayThresholdValue ? true : false;
   else
      return displayColValue <= m_displayThresholdValue ? true : false;
   }


Bin *MapLayer::GetDataBin( float value )
	{
	int binCount = m_binArray.GetSize();
 
   if ( binCount == 0 )
      return NULL;

   if ( m_useNoDataBin && value == GetNoDataValue() )
      return &(m_binArray[ m_binArray.GetSize()-1 ]);     // nodata bin is always last one.

	for ( int i=0; i < binCount-1; i++ )
		{
      Bin *pBin = &(m_binArray[ i ]);

      switch( m_binArray.m_binType )
         {
         case BT_NUMERIC:
            if ( value >= pBin->m_minVal && value <= pBin->m_maxVal )
			      return pBin;
            break;

         case BT_STRING:
            {
            CString svalue;
            svalue.Format( "%g", value );
            if ( svalue.Compare( pBin->m_strVal ) == 0 )
               return pBin;
            }
            break;

         default:
            ASSERT( 0 );
         }  // end of switch( binCount );
		}  // end of:  for ( i < binCount )

   if ( m_useNoDataBin ) 
      return &(m_binArray[ binCount-1 ]);
   else
      return &(m_binArray[ binCount-2 ]);
	}


Bin *MapLayer::GetDataBin( int value )
	{
	int binCount = m_binArray.GetSize();

   if ( binCount == 0 )
      return NULL;

   if ( binCount == 1 )
      return &m_binArray[ 0 ];

   if ( m_useNoDataBin && value == GetNoDataValue() )
      &(m_binArray[ m_binArray.GetSize()-1 ]);     // nodata bin is always last one.

	for ( int i=0; i < binCount-1; i++ )
		{
      Bin *pBin = &(m_binArray[ i ]);

      switch( m_binArray.m_binType )
         {
         case BT_NUMERIC:
            if ( value >= pBin->m_minVal && value <= pBin->m_maxVal )
			      return pBin;
            break;

         case BT_STRING:
            {
            CString svalue;
            svalue.Format( "%i", value );
            if ( svalue.Compare( pBin->m_strVal ) == 0 )
               return pBin;
            }
            break;

         default:
            ASSERT( 0 );
         }  // end of switch( binCount );
		}  // end of:  for ( i < binCount )

   return &(m_binArray[ binCount-2 ]);
	}


Bin *MapLayer::GetDataBin( LPCTSTR value )
	{
   ASSERT(  m_binArray.m_binType == BT_STRING );

	int binCount = m_binArray.GetSize();

   if ( binCount == 0 )
      return NULL;

	for ( int i=0; i < binCount-1; i++ )
		{
      Bin *pBin = &m_binArray[ i ];

		if ( lstrcmp( value, pBin->m_strVal ) == 0 )
			return pBin;
		}

   return &m_binArray[ binCount-1 ];
	}


bool MapLayer::GetSortedPolyArray( PolyArray &polyArray, int col, bool ascending )
   {
   int polyCount = m_polyArray.GetSize();

   ASSERT( m_pData->GetRowCount() == polyCount );
   ASSERT( col < m_pData->GetColCount() );

   // now, create an arra of indexes.  This is what will 
   // actually be sorted.
   int *indexArray = new int[ polyCount ];

   int oldActiveField = m_activeField;
   m_activeField = col;
   m_ascending = ascending;

   for ( int i=0; i < polyCount; i++ )
      indexArray[ i ] = i;   

   // sort the index array
   pMapLayer = this;
   qsort( (void*) indexArray, polyCount, sizeof( int ), Compare );

   // restore previous active field;
   m_activeField = (int) oldActiveField;

   // resize and load up polyArray
   polyArray.SetSize( polyCount );

   for ( i=0; i < polyCount; i++ )
      polyArray[ i ] = m_polyArray[ indexArray[ i ] ];

   delete [] indexArray;

   return true;
   }


int Compare(const void *elem0, const void *elem1 )
   {
   int index0 = *((int*) elem0 );
   int index1 = *((int*) elem1 );

   int activeField = pMapLayer->GetActiveField();

   // get field for the indexes
   TYPE type = pMapLayer->GetFieldType( activeField );

   ASSERT( type == TYPE_FLOAT || type == TYPE_INT || type == TYPE_DOUBLE || type == TYPE_LONG );

   VData v0, v1;
   pMapLayer->GetData( index0, activeField, v0 );
   pMapLayer->GetData( index1, activeField, v1 );
   int compare = 0;
   
   switch( type )
      {
      case TYPE_FLOAT:
         compare = ( v0.val.vFloat < v1.val.vFloat ) ? -1 : 1;
         break;

      case TYPE_DOUBLE:
         compare = ( v0.val.vDouble < v1.val.vDouble ) ? -1 : 1;
         break;

      case TYPE_LONG:
         compare = v0.val.vLong - v1.val.vLong;
         break;

      case TYPE_INT:
         compare = v0.val.vInt - v1.val.vInt;
         break;

      case TYPE_UINT:
         compare = v0.val.vUInt - v1.val.vUInt;
         break;

      default:
         ASSERT( 0 );
         break;

      // case VT_?? string
      // compare = _stricmp( * ( char** ) arg1, * ( char** ) arg2 );
      // break;
      }

   if ( pMapLayer->m_ascending == false )
      compare = -compare;

   return compare;
   }


Poly *MapLayer::FindPoint( int x, int y, int *pIndex )  const  // assumes x, y are device coords
   {
   if ( m_layerType != LT_POINT )
      return NULL;

   POINT pt;
   pt.x = x;
   pt.y = y;

   m_pMap->DPtoLP( pt );              // convert to logical

   for ( int i=0; i < m_polyArray.GetSize(); i++ )
      {
      Poly *pPoly = m_polyArray[ i ];

      //if ( IsPointInPoly( pPoly->m_ptArray, pPoly->GetVertexCount(), pt ) )
      POINT &pp = pPoly->m_ptArray[ 0 ];

      if ( abs( pt.x - pp.x ) < 25 && abs( pt.y - pp.y ) < 25 )
         {
         if ( pIndex != NULL )
            *pIndex = i;

         return pPoly;
         }
      }

   if ( pIndex != NULL )
      *pIndex = -1;

   return NULL;
   }


Poly *MapLayer::FindPolygon( int x, int y, int *pIndex ) const   // assumes x, y are device coords
   {
   if ( m_layerType != LT_POLYGON && m_layerType != LT_LINE )
      return NULL;

   POINT pt;
   pt.x = x;
   pt.y = y;

   m_pMap->DPtoLP( pt );              // convert to logical

   if ( m_layerType == LT_POLYGON )
      {
      for ( int i=0; i < m_polyArray.GetSize(); i++ )
         {
         Poly *pPoly = m_polyArray[ i ];

         if ( IsPointInPoly( pPoly->m_ptArray, pPoly->GetVertexCount(), pt ) )
            {
            if ( pIndex != NULL )
               *pIndex = i;

            return pPoly;
            }
         }
      }
   else  // m_layerType == LT_LINE
      {
      for ( int i=0; i < m_polyArray.GetSize(); i++ )
         {
         Poly *pPoly = m_polyArray[ i ];

         if ( IsPointOnLine( pPoly->m_ptArray, pPoly->GetVertexCount(), pt, 20 ) )
            {
            if ( pIndex != NULL )
               *pIndex = i;

            return pPoly;
            }         
         }
      }

   if ( pIndex != NULL )
      *pIndex = -1;

   return NULL;
   }


Poly *MapLayer::GetPolygonFromCoord( float x, float y, int *pIndex /*=NULL*/ ) const
   {
   Vertex v( x, y );

   for ( int i=0; i < m_polyArray.GetSize(); i++ )
      {
      Poly *pPoly = m_polyArray[ i ];

      if ( pPoly->IsPointInPoly( v ) )
         {
         if ( pIndex != NULL )
            *pIndex = i;

         return pPoly;
         }
      }

   if ( pIndex != NULL )
      *pIndex = -1;

   return NULL;
   }



Poly* MapLayer::GetDataAtPoint( int x, int y, int col, CString &value ) const
   {
   int row;
   Poly *pPoly = FindPolygon( x, y, &row );

   if ( pPoly == NULL )
      return NULL;
   else
      {
      if ( m_pData == NULL )
         return NULL;
      else
         {
         value = m_pData->GetAsString( col, row );
         return pPoly;
         }
      }
   }


LPCTSTR MapLayer::GetFieldLabel( int col ) const
   {
   if( m_pData == NULL ) 
      return ""; 

   else 
      { 
      if ( col < 0 )
         col = m_activeField;

      LPCTSTR p = NULL;
      if ( col < 0 )    // no active field, return name of layer
         p = m_name;

      else
         p= m_pData->GetLabel( col );
         
      if ( p )
         return p;
      else
         return "";
      }
   }


void MapLayer::SetFieldLabel( int col, LPCSTR label )
   {
   if( m_pData == NULL ) 
      return;

   if ( col < 0 )
      col = m_activeField;

   m_pData->SetLabel( col, label );
   }



void MapLayer::LoadFieldStrings( CStringArray &strArray )
   {
   strArray.RemoveAll();

   for ( int i=0; i < (int) m_pData->GetColCount(); i++ )
      strArray.Add( m_pData->GetLabel( i ) );
   }



void MapLayer::UpdateExtents( bool initPolyPts /*=true*/ )
   {
   switch( m_layerType )
      {
      case LT_POLYGON:
      case LT_LINE:
      case LT_POINT:
         {
         m_vxMin = m_vyMin = (float) LONG_MAX;
         m_vxMax = m_vyMax = (float) LONG_MIN;

         for ( int i=0; i < m_polyArray.GetSize(); i++ )
            {
            Poly *pPoly = m_polyArray[ i ];

            for ( int j=0; j < pPoly->GetVertexCount(); j++ )
               {
               const Vertex &v = pPoly->GetVertex( j );

               if ( v.x > m_vxMax ) m_vxMax = v.x;     // check bounds
               if ( v.y > m_vyMax ) m_vyMax = v.y;
               if ( v.x < m_vxMin ) m_vxMin = v.x;
               if ( v.y < m_vyMin ) m_vyMin = v.y;
               }
            }

         if ( ( m_vxMax-m_vxMin ) > ( m_vyMax - m_vyMin ) )
            m_vExtent = m_vxMax - m_vxMin;
         else
            m_vExtent = m_vyMax - m_vyMin;

         if ( initPolyPts )
            InitPolyLogicalPoints();   // create logical points for this set
         }
         break;

      case LT_GRID:
         m_vxMax = m_vxMin + m_pData->GetColCount() * m_cellWidth;
         m_vyMax = m_vyMin + m_pData->GetRowCount() * m_cellHeight;
         
         if ( ( m_vxMax-m_vxMin ) > ( m_vyMax - m_vyMin ) )
            m_vExtent = m_vxMax - m_vxMin;
         else
            m_vExtent = m_vyMax - m_vyMin;

         break;

      default:
         ASSERT( 0 );
      }
   }


void MapLayer::ClearBins( bool resetPolys /*=false*/ )
   {
   //for ( int i=0; i < m_binArray.GetSize(); i++ )
   //   m_binArray[ i ].m_brush.DeleteObject();
      
  m_binArray.RemoveAll();

  if ( resetPolys )
     {
     for ( int i=0; i < m_polyArray.GetSize(); i++ )
        m_polyArray[ i ]->m_pBin = NULL;
     }
  }


bool MapLayer::GetDataMinMax( int col, float *pMin, float *pMax ) const
   {
   ASSERT( m_pData != NULL );

   if ( m_layerType == LT_GRID )
      return GetGridDataMinMax( pMin, pMax );

   int cols = m_pData->GetColCount();
   ASSERT( col < cols );

   if ( col >= 0 )
      return _GetDataMinMax( col, pMin, pMax ) ? true : false; 

   else
      {
      float minVal, maxVal;
      *pMin = (float) LONG_MAX;
      *pMax = (float) LONG_MIN;

      for ( col=0; col < cols; col++ )
         {
         _GetDataMinMax( col, &minVal, &maxVal );

         if ( minVal < *pMin ) *pMin = minVal;
         if ( maxVal > *pMax ) *pMax = maxVal;
         }
      }

   return true;
   }      


// helper function for GetDataMinMax()
bool MapLayer::_GetDataMinMax( int col, float *pMin, float *pMax ) const
   {
   *pMin = (float) LONG_MAX;
   *pMax = (float) LONG_MIN;

   int rows = m_pData->GetRowCount();

   for ( int i=0; i < rows; i++)
      {
      float val = m_pData->GetAsFloat( col, i );

      if ( val != m_noDataValue )
         {
         if ( *pMin > val )
            *pMin = val;

         if ( *pMax < val )
            *pMax = val;
         }
      }

   return TRUE;
   }


bool MapLayer::GetGridDataMinMax( float *pMin, float *pMax ) const
   {
   ASSERT( m_pData != NULL );

   *pMin = (float) 1.0e10;
   *pMax = (float) -1.0e10;

   int rows = m_pData->GetRowCount();
   int cols = m_pData->GetColCount();
   
   for ( int row=0; row < rows; row++)
      for ( int col=0; col < cols; col++)
         {
         float val = m_pData->GetAsFloat( col, row );

         if ( *pMin > val && val != m_noDataValue )
            *pMin = val;

         if ( *pMax < val && val != m_noDataValue )
            *pMax = val;
         }

   return true;
   }



// big/little endian conversion routines


WORD SwapTwoBytes(WORD w)
   {
   register WORD tmp;
   tmp =  (w & 0x00FF);
   tmp = ((w & 0xFF00) >> 0x08) | (tmp << 0x08);
   return(tmp);
   }  

DWORD SwapFourBytes(DWORD dw)
   {
   register DWORD tmp;
   tmp =  (dw & 0x000000FF);
   tmp = ((dw & 0x0000FF00) >> 0x08) | (tmp << 0x08);
   tmp = ((dw & 0x00FF0000) >> 0x10) | (tmp << 0x08);
   tmp = ((dw & 0xFF000000) >> 0x18) | (tmp << 0x08);
   return(tmp);
   }

WORD GetBigWord(FILE *fp)
   {
   register WORD w;
   w =  (WORD) (fgetc(fp) & 0xFF);
   w = ((WORD) (fgetc(fp) & 0xFF)) | (w << 0x08);
   return(w);
   }  


WORD GetLittleWord(FILE *fp)
   {
   register WORD w;
   w =  (WORD) (fgetc(fp) & 0xFF);
   w = ((WORD) (fgetc(fp) & 0xFF) << 0x08);
   return(w);
   }  


DWORD GetBigDWORD(FILE *fp)
   {
   register DWORD dw;
   dw =  (DWORD) (fgetc(fp) & 0xFF);
   dw = ((DWORD) (fgetc(fp) & 0xFF)) | (dw << 0x08);
   dw = ((DWORD) (fgetc(fp) & 0xFF)) | (dw << 0x08);
   dw = ((DWORD) (fgetc(fp) & 0xFF)) | (dw << 0x08);
   return(dw);
   }  

DWORD GetLittleDWORD(FILE *fp)
   {
   register DWORD dw;
   int count = fread( &dw, sizeof( DWORD ), 1, fp );
   //ASSERT( count == 1 );
   return ( dw );
   }  

double GetLittleDouble(FILE *fp)
   {
   ASSERT( feof( fp ) == 0 );
   double dw;
   int count = fread( &dw, sizeof( double ), 1, fp );
   ASSERT( count == 1 );
   return(dw);
   }  


void PutBigWord( WORD w, FILE *fp)
   {
   fputc((w >> 0x08) & 0xFF, fp);
   fputc(w & 0xFF, fp);
   }


void PutLittleWord( WORD w, FILE *fp )
   {
   fputc(w & 0xFF, fp);
   fputc((w >> 0x08) & 0xFF, fp);
   }

  
void PutBigDWORD( DWORD dw, FILE *fp )
   {
   fputc((dw >> 0x18) & 0xFF, fp);
   fputc((dw >> 0x10) & 0xFF, fp);
   fputc((dw >> 0x08) & 0xFF, fp);
   fputc(dw & 0xFF, fp);
   }

  
void PutLittleDWORD( DWORD dw, FILE *fp )
   {
   //fputc( dw & 0xFF, fp );
   //fputc( (dw >> 0x08) & 0xFF, fp );
   //fputc( (dw >> 0x10) & 0xFF, fp );
   //fputc( (dw >> 0x18) & 0xFF, fp );
   fwrite( (void*) &dw, sizeof( DWORD ), 1, fp );
   }

void PutLittleDouble( double d, FILE *fp )
   {
   fwrite( (void*) &d, sizeof( double ), 1, fp );
   }
   


// Get an array of unique values for the specified data column. 
// Works for any type that can be converted to string
int MapLayer::GetUniqueValues( int col, CStringArray &valueArray ) const
   {
   bool found;

   if ( col == -1 )
      {
      for ( int i=0; i < m_pData->GetColCount(); i++ )
         GetUniqueValues( i, valueArray );
      }
   else
      {
      for ( int row=0; row < m_pData->GetRowCount(); row++ )
         {
         found = false;         
         CString value = m_pData->GetAsString( col, row ) ;
   
         // check to see if it has not been found yet
         for ( int j=0; j < valueArray.GetSize(); j++ )
            {
            // string already found?
            if ( valueArray[ j ].Compare( value ) == 0 )
               {
               found = true;
               break;
               }
            }

         if ( found == false )
            valueArray.Add( value );
         }
      }

   SortStringArray( valueArray );
   
   return valueArray.GetSize();
   }


void SortStringArray( CStringArray &array )
   {
   int count = array.GetSize();
   char **strings = new char*[ count ];

   // allocate temporary strings
   for ( int i=0; i < count; i++ )
      {
      strings[ i ] = new char[ array[ i ].GetLength()+1 ];
      lstrcpy( strings[ i ], array[ i ] );
      }

   // sort the pointers
   qsort( (void*) strings, count, sizeof( char* ), SortStringProc );

   for ( i=0; i < count; i++ )
      {
      array[ i ] = strings[ i ];
      delete [] strings[ i ];
      }

   delete [] strings;
   }


int SortStringProc( const void *elem1, const void *elem2 )
   {
   char *str0 = *((char**) elem1);
   char *str1 = *((char**) elem2);
   return lstrcmp( str0, str1 );
   }


// Get an array of unique values for the specified data column. 
// Works for any type that can be converted to string
int MapLayer::GetUniqueValues( int col, CUIntArray &valueArray ) const
   {
   bool found;

   if ( col == -1 )
      {
      for ( int i=0; i < m_pData->GetColCount(); i++ )
         GetUniqueValues( i, valueArray );
      }
   else
      {
      for ( int row=0; row < m_pData->GetRowCount(); row++ )
         {
         found = false;         
         UINT value = (UINT) m_pData->GetAsInt( col, row ) ;
   
         // check to see if it has not been found yet
         for ( int j=0; j < valueArray.GetSize(); j++ )
            {
            // value already found?
            if ( valueArray[ j ] == value )
               {
               found = true;
               break;
               }
            }

         if ( found == false )
            valueArray.Add( value );
         }
      }

   return valueArray.GetSize();
   }



// Get an array of unique values for the specified data column. 
// Works for any type
int MapLayer::GetUniqueValues( int col, CArray< VData, VData& > &valueArray ) const
   {
   bool found;

   if ( col == -1 )
      {
      for ( int i=0; i < m_pData->GetColCount(); i++ )
         GetUniqueValues( i, valueArray );
      }
   else
      {
      for ( int row=0; row < m_pData->GetRowCount(); row++ )
         {
         found = false;
         VData value;
         bool ok = m_pData->Get( col, row, value ) ;
         ASSERT( ok );
   
         // check to see if it has not been found yet
         for ( int j=0; j < valueArray.GetSize(); j++ )
            {
            // value already found?
            if ( valueArray[ j ].Compare( value ) == true )
               {
               found = true;
               break;
               }
            }

         if ( found == false )
            valueArray.Add( value );
         }
      }

   //SortStringArray( valueArray );
   
   return valueArray.GetSize();
   }


int MapLayer::Project3D( MapLayer *pGrid, bool interpolate )
   {

   if ( pGrid->m_layerType != LT_GRID )
      {
      ErrorMsg( "Projection Grid is not a grid layer" );
      return -1;
      }

   if ( m_layerType != LT_LINE && m_layerType != LT_POLYGON )
      {
      ErrorMsg( "Source grid is not a polygon or line coverage" );
      return -2;
      }

   // correct layer types certified, now start converting vertex's
   for ( int i=0; i < m_polyArray.GetSize(); i++ )
      {
      Poly *pPoly = m_polyArray[ i ];

      int vertexCount = pPoly->GetVertexCount();

      for ( int j=0; j < vertexCount; j++ )
         {
         Vertex &v = (Vertex&) pPoly->GetVertex( j );
         // get the elevations of the nearest neighbors from the grid
         float elev = 0;
         int row, col;
			pGrid->GetGridCellFromCoord( v.x, v.y, row, col );
         if ( interpolate == false )
			   {
			   pGrid->GetData( row, col, elev );
			   v.z = elev;
			   }
         else
            {
            POINT map[ 9 ] = { {-1,-1}, { 0,-1}, { 1,-1},    // top row
                               {-1, 0}, { 0, 0}, { 1, 0},    // center row
                               {-1, 1}, { 0, 1}, { 1, 1} };  // bottom row
            float sum_wdz=0.0f;
            float sum_wd =0.0f;
            for ( int i=0; i < 9; i++ )
               {
               int _row = row+map[i].x;
               int _col = col+map[i].y;
               // skip over edges
               if ( _row < 0 ) continue;
               if ( _col < 0 ) continue;
               if ( _row >= pGrid->GetRowCount() ) continue;
               if ( _col >= pGrid->GetColCount() ) continue;

               // not an edge so get data and process it
               float elev, x, y;
               pGrid->GetData( _row, _col, elev );           // elevation
               if ( elev > 0.0f )
                  {
                  pGrid->GetGridCellCenter( _row, _col, x, y ); // center
                  float distance = DistancePtToPt( v.x, v.y, x, y );
                  
                  if ( distance == 0.0f )    //Special case if point falls exactly in center of grid cell
                     distance = 0.001f;     
                  
                  float wd = 1.0f/(distance*distance);
                  sum_wd  += wd;
                  sum_wdz += wd*elev;
                  }
			      }  // end of for ( i < 9 )

           if ( sum_wd == 0.0f )
               {
               int retVal = MessageBox( NULL, 
                        "Source coverage is outside the clipped DEM, Elevation will be set to zero.  Do you want to continue?", 
                        "Warning", MB_YESNO );

               if ( retVal == IDNO )
                  return -3;
               else
						v.z = 0.0f;

               }
            else
   		      v.z = sum_wdz/sum_wd;

			   }  // end of else
         
         if ( _isnan( v.z ) )
            {
            TRACE( "Invalid (NAN) z calculated - setting to 0" );
            v.z = 0.0f;
            }
         }  // end of:  for ( j < vertexCount )
      }  // end of:  for ( i < m_polyArray.GetSize() );

   
	m_3D = true;

   return m_polyArray.GetSize();  
   }



bool MapLayer::GetGridCellFromCoord( float x, float y, int &row, int &col )
   {
   // Note:  returned row is from TOP down (e.g. top of screen is row 0)
   ASSERT( m_layerType == LT_GRID );

   if ( y - m_vyMin < 0 )
      return false;

   if ( x - m_vxMin < 0 )
      return false;

   row = (int)(( y - m_vyMin ) / m_cellHeight ); // - 1; jpb, 11/4/99
   col = (int)(( x - m_vxMin ) / m_cellWidth  ); // - 1;

   row = m_pData->GetRowCount() - row - 1; // invert to be top down

   if ( row < 0 ) return false;
   if ( row >= m_pData->GetRowCount() ) return false;

   if ( col < 0 ) return false;
   if ( col >= m_pData->GetColCount() ) return false;

   return true;
   }


   
void MapLayer::GetGridCellCenter( int row, int col, float &x, float &y ) const
   {
   // Note: assumes rows are ordered from top down, vcoord system in increasing upward
   ASSERT( m_layerType == LT_GRID );
   
   x = m_vxMin + ( col * m_cellWidth  ) + m_cellWidth/2.0f;
   //y = m_vyMin + ( row * m_cellHeight ) + m_cellHeight/2.0f;
   // changed (Bolte, 7/18/02)
   int rows = GetRowCount();
   y = m_vyMin + ( (rows-row-1) * m_cellHeight) + m_cellHeight/2.0f;
   
   }


int MapLayer::CalcSlopes( int colToSet )
   {
//   ASSERT( m_layerType == LT_POLYGON );
   ASSERT( m_pData != NULL );
   ASSERT( colToSet < m_pData->GetColCount() );
   ASSERT( m_pData->GetRowCount() == m_polyArray.GetSize() );

   int polyCount = m_polyArray.GetSize();

   for ( int i=0; i < polyCount; i++ )
      {
      float slope = m_polyArray[ i ]->CalcSlope();
      m_pData->Set( colToSet, i, slope );
      m_pMap->Notify( NT_CALCSLOPE, i, 0 );
      }

   return polyCount;
   }

int MapLayer::Create3dShape( MapLayer *pDEMLayer )
	{
	int polyCount = GetPolygonCount();	
   for ( int i=0; i < polyCount-1; i++ )
	   {
		Poly *pCurrent = GetPolygon( i );
		for (int j = 0; j < pCurrent->GetVertexCount() - 1; j++ )
			{
			Vertex &vFirst = (Vertex&) pCurrent->GetVertex( j );
			int col, row;
			bool ok = pDEMLayer->GetGridCellFromCoord( vFirst.x,  vFirst.y, row, col);
			float elev;
         ok = pDEMLayer->GetData( row, col,  elev );
			vFirst.z = elev;
			}
		}
		
	return -1;	
	}


int MapLayer::PopulateElevation( int colToSet )
   {
   ASSERT( m_layerType != LT_GRID );
   ASSERT( m_pData != NULL );
   ASSERT( colToSet < m_pData->GetColCount() );
   ASSERT( m_pData->GetRowCount() == m_polyArray.GetSize() );

   int polyCount = m_polyArray.GetSize();

   for ( int i=0; i < polyCount; i++ )
      {
      Poly *pPoly = GetPolygon( i );
      float elev = pPoly->CalcElevation();
      m_pData->Set( colToSet, i, elev );
      }

   return polyCount;
   }



int MapLayer::CalcLengthSlopes( int slopeCol, int colToSet )
   {
   ASSERT( m_layerType == LT_POLYGON );
   ASSERT( m_pData != NULL );
   ASSERT( colToSet < m_pData->GetColCount() );
   ASSERT( m_pData->GetRowCount() == m_polyArray.GetSize() );

   int polyCount = m_polyArray.GetSize();

   for ( int i=0; i < polyCount; i++ )
      {
      float slope = m_pData->GetAsFloat( slopeCol, i );  // slope is nondecimal percent
      float area  = m_pData->GetAsFloat( 0, i );  // area is m2
      area = area * HA_M2;   // convert to hectares....

      float lengthSlope = m_polyArray[ i ]->CalcLengthSlope( slope, area );  // unitless
      m_pData->Set( colToSet, i, lengthSlope );
      }

   return polyCount;
   }



bool MapLayer::IsDataValid( int row, int col ) const
   {
   ASSERT( row < m_pData->GetRowCount() );
   ASSERT( col < m_pData->GetColCount() );
   ASSERT( row >= 0 );
   ASSERT( col >= 0 );

   float data;
   GetData( row, col, data );

   return ( data == m_noDataValue ? false : true );
   }


void MapLayer::SetAllData( float value, bool includeNoData )
   {
   ASSERT( m_layerType == LT_GRID );

   int rows = GetRowCount();
   int cols = GetColCount();

   for ( int row=0; row < rows; row++ )
      for ( int col=0; col < cols; col++ )
         {
         if ( includeNoData == false ) // need to check noData regions?
            {
            float currentValue;
            bool ok = m_pData->Get( col, row, currentValue );

            if ( currentValue != m_noDataValue )
               SetData( row, col, value );
            }
         else
            SetData( row, col, value );
         }
   }

bool MapLayer::SetColData( int col, float value, bool includeNoData )
   {
   int rows = GetRowCount();
   int cols = GetColCount();

   if ( col >= cols )
      return false;

   for ( int row=0; row < rows; row++ )
      {
      if ( includeNoData == false ) // need to check noData regions?
         {
         float currentValue;
         bool ok = m_pData->Get( col, row, currentValue );

         if ( currentValue != m_noDataValue )
            SetData( row, col, value );
         }
      else
         SetData( row, col, value );
      }

   return true;
   }



int MapLayer::QueryPoint( POINT pt, bool clear /* =true */)   // Note: pt is a LOGICAL coord
   {
   if ( clear )
      ClearQueryResults();
   
   // start checking polygons
   switch( m_layerType )
      {
      case LT_POLYGON:
      case LT_LINE:
         {
         int polyCount = m_polyArray.GetSize();

         for ( int i=0; i < polyCount; i++ )
            {
            Poly *pPoly = m_polyArray[ i ];
            int isInPoly = IsPointInPoly( pPoly->m_ptArray, pPoly->GetVertexCount(), pt );

            if ( isInPoly )
               {
               AddQueryResult( i );
               break;
               }
            }  // end of:  for( i < polyCount )
         }
         break;

      case LT_POINT:
      case LT_GRID:
         ASSERT( 0 );   // not implemented (yet)
         break;
      }

   return GetQueryCount();
   }


int MapLayer::QueryArea( POINT _ll, POINT _ur, bool clear /* =true */)   // Note: _llm _ur are LOGICAL coords
   {
   if ( clear )
      ClearQueryResults();
   
   POINT ll, ur;
   ll.x = min( _ll.x, _ur.x );      // make sure ponts are oriented correctly
   ll.y = min( _ll.y, _ur.y );
   ur.x = max( _ll.x, _ur.x );
   ur.y = max( _ll.y, _ur.y );

   // start checking polygons
   switch( m_layerType )
      {
      case LT_POLYGON:
      case LT_LINE:
         {
         int polyCount = m_polyArray.GetSize();

         for ( int i=0; i < polyCount; i++ )
            {
            Poly *pPoly = m_polyArray[ i ];

            for ( int j=0; j < pPoly->GetVertexCount(); j++ )
               {
               POINT *v = pPoly->m_ptArray + j;

               if ( ll.x <= v->x && v->x <= ur.x && ll.y <= v->y && v->y <= ur.y )
                  {
                  AddQueryResult( i );
                  break;
                  }
               }
            }  // end of:  for( i < polyCount )
         }
         break;

      case LT_POINT:
      case LT_GRID:
         ASSERT( 0 );
         break;
      }

   return GetQueryCount();
   }


// gets the first record number (offset) with the specified value
int MapLayer::FindIndex( int col, int value, int startIndex /*=0*/ ) const
   {
   if ( m_pData == NULL )
      return -1;

   int _value;
   for ( int i=startIndex; i < GetRecordCount(); i++ )
      {
      if ( GetData( i, col, _value ) == false )
         return -2;

      if ( _value == value )
         return i;
      }

   return -3;
   }


bool MapLayer::FindMinValue( int &minRow, int &minCol ) const
   {
   if ( m_layerType != LT_GRID )
      return false;

   ASSERT( m_pData != NULL );

   float minVal = 1.0e10f;

   int rows = m_pData->GetRowCount();
   int cols = m_pData->GetColCount();
   
   for ( int row=0; row < rows; row++)
      for ( int col=0; col < cols; col++)
         {
         float val = m_pData->GetAsFloat( col, row );

         if ( val < minVal && val != m_noDataValue )
            {
            minRow = row;
            minCol = col;
            minVal = val;
            }
         }

   return true;
   }

 
bool MapLayer::IsPolyInMap( Poly *pPoly ) const
	{
	float xMinPoly=0.0f, yMinPoly=0.0f,xMaxPoly=0.0f, yMaxPoly=0.0f;
	float vxMinLayer=0.0f,vxMaxLayer=0.0f,vyMinLayer=0.0f,vyMaxLayer=0.0f;

	pPoly->GetBoundingRect(xMinPoly, yMinPoly, xMaxPoly, yMaxPoly);

	GetExtents(vxMinLayer,vxMaxLayer,vyMinLayer,vyMaxLayer);

   if ( ( xMaxPoly < vxMinLayer )    // case 1 
     || ( xMinPoly > vxMaxLayer ) )  // case 2
      return false;  // no reason to look further

   // check y intersection next
   if ( ( yMaxPoly< vyMinLayer )    // case 1 
     || ( yMinPoly > vyMaxLayer ) )  // case 2
     return false;

   return true;
	}

//--------------------------------------------------------------------------------------------------------------------------------
// int MapLayer::CreateSpatialIndex()
//
// creates a spatial index for use prior to call GetNearbyPolys()
//
// Before creating an index, it checks to see if a valid index already exists, and loads it if possible.
//
// Parameters:
//    LPCSTR filename   - name of the index to use, or NULL.  If NULL, then the method assumes the index name is "temp.spndx"
//                        if there is no path information for the "this" MapLayer;  if path info
//                        exists, then the spatial index name is the name of "this" layer, with a ".spndx" extension.
//    int maxCount      - max number of "nearby" polys to store
//    float maxDistance - max distance to "look" for nearby polys
//    SI_METHOD method  - currently  SIM_NEAREST, SIM_CENTROID - methods for determining proximity
//    MapLayer *pToLayer-layer to look to, or NULL for internal (self-referenced) index
//---------------------------------------------------------------------------------------------------------------------------------

int MapLayer::CreateSpatialIndex( LPCSTR filename, int maxCount, float maxDistance, SI_METHOD method, MapLayer *pToLayer /*=NULL*/ )
   {
   if ( m_pSpatialIndex == NULL )
      m_pSpatialIndex = new SpatialIndex;
   else
      m_pSpatialIndex->Clear();

   // see if a spatial index currently exists on disk
   char _filename[ 128 ];

   if ( filename != NULL )
      lstrcpy( _filename, filename );
   else
      {
      if ( m_path.IsEmpty() )
         lstrcpy( _filename, "temp.spndx" );
      else
         {
         lstrcpy( _filename, (LPCSTR) m_path );
         char *dot = strrchr( _filename, '.' );

         if ( dot != NULL )
            lstrcpy( dot, ".spndx" );
         else
            lstrcat( _filename, ".spndx" );
         }
      }

   // have file name established, see if it exists
   float distance;
   int retVal = m_pSpatialIndex->GetSpatialIndexDistance( _filename, distance );

   if ( retVal > 0 ) // file exists, check size
      {
      if ( distance >= maxDistance )      // is the current spatial index okay?
         {
         retVal = m_pSpatialIndex->ReadIndex( _filename, this, pToLayer );    // yes, so just read it from the disk

         if ( retVal < 0 || m_pSpatialIndex->m_method != method )
            {
            retVal = m_pSpatialIndex->BuildIndex( this, pToLayer, maxCount, maxDistance, method );
            retVal = m_pSpatialIndex->WriteIndex( _filename );
            }
         }

      else  // otherwise, rebuild it
         {
         retVal = m_pSpatialIndex->BuildIndex( this, pToLayer, maxCount, maxDistance, method );
         retVal = m_pSpatialIndex->WriteIndex( _filename );
         }
      }

   else  // file doesn't exist, so create index and save to disk
      {
      retVal = m_pSpatialIndex->BuildIndex( this, pToLayer, maxCount, maxDistance, method );
      retVal = m_pSpatialIndex->WriteIndex( _filename );
      }

   return retVal;
   }


int MapLayer::GetNearbyPolysFromIndex( Poly *pPoly, int *neighbors, float *distances, int maxCount, float maxDistance, 
                              SI_METHOD method/*=SIM_NEAREST*/, MapLayer *pToLayer /*=NULL*/ ) const
   {
   if ( m_pSpatialIndex == NULL )
      return -1;

   int thisIndex = pPoly->m_id;

   int size = m_pSpatialIndex->GetNeighborArraySize( thisIndex );

   // make some termporary arrays
   int   *_neighbors = new int  [ size ];
   float *_distances = new float[ size ];

   // get the values from the index
   int retVal = m_pSpatialIndex->GetNeighborArray( thisIndex, _neighbors, _distances, maxCount );

   if ( retVal < 0 )
      {
      delete _neighbors;
      delete _distances;
      return retVal;
      }

   // iterate through, getting any <= maxdistance
   int count = 0;
   for ( int i=0; i < size; i++ )
      {
      if ( _distances[i] <= maxDistance )
         {
         neighbors[ count ] = _neighbors[ i ];

         if ( distances != NULL )
            distances[ count ] = _distances[ i ];

         count++;
         }

      if ( count >= maxCount )
         break;
      }

   delete [] _neighbors;
   delete [] _distances;
 
   return count;  // actual number populated
   }



int MapLayer::GetNearbyPolys( Poly *pPoly, int *neighbors, float *distances, int maxCount, float maxDistance, 
                              SI_METHOD method/*=SIM_NEAREST*/, MapLayer *pToLayer /*=NULL*/ ) const
   {
   if ( m_pSpatialIndex != NULL && m_pSpatialIndex->IsBuilt() )
      return GetNearbyPolysFromIndex( pPoly, neighbors, distances, maxCount, maxDistance, method, pToLayer );

   if ( pToLayer == NULL )
      pToLayer = (MapLayer*) this;

   int recordCount = pToLayer->GetRecordCount();
   int count = 0;

   float xMin, xMax, yMin, yMax;
   pPoly->GetBoundingRect( xMin, yMin, xMax, yMax );

   ASSERT( xMin <= xMax );
   ASSERT( yMin <= yMax );

   float xMinTo, xMaxTo, yMinTo, yMaxTo;
   float maxDistSq = maxDistance * maxDistance;

   // iterate through the poly's in the to layer
   for ( int i=0; i < recordCount; i++ )
      {
      Poly *pToPoly = pToLayer->GetPolygon( i );

      if ( pToPoly == pPoly ) // skip this polygon
         continue;

      // are we even close?
      bool ok = pToPoly->GetBoundingRect( xMinTo, yMinTo, xMaxTo, yMaxTo );
      ASSERT( ok );

      bool passX = false;
      bool passY = false;

      ASSERT( xMinTo <= xMaxTo );
      ASSERT( yMinTo <= yMaxTo );

      // fundamentally, if the closest distance between the two poly's (D) is less
      // than the max distance (M), then the poly's are neighbors, e.g. D < M 
      //
      // To speed the search, we first look at the bounding rects.  The bounding rect represents
      // the maximum size of the polygons.  Therefore, if D for the bounding rects is greater than
      // M, then we can be assured that these polygons are not neighbors.  We apply this test as a
      // first cut.  Any poly's that pass are then examined vertex by vertex to get the closest distance

      // For the bounding rect, we further simplify the analysis.  D for overlapping rects is 0,
      // and so the poly passes, Rect's overlap if:
      //   1) one rect overlaps another, or
      //   2) the "left" edge of one is within D of the right edge of the other

      // check condition 1 for both poly's
      if (  BETWEEN( xMin,   xMinTo, xMax   )    // Note: BETWEEN is inclusive ( x <= y <= z )
         || BETWEEN( xMin,   xMaxTo, xMax   )
         || BETWEEN( xMinTo, xMin,   xMaxTo ) 
         || BETWEEN( xMinTo, xMax,   xMaxTo ) )
        passX = true;

      // condition 2
      else if ( ( ( xMinTo > xMax   ) && ( xMinTo - xMax   <= maxDistance ) )
             || ( ( xMin   > xMaxTo ) && ( xMin   - xMaxTo <= maxDistance ) ) )
         passX = true;

      // repeat for y's
      if (  BETWEEN( yMin,   yMinTo, yMax   ) 
         || BETWEEN( yMin,   yMaxTo, yMax   )
         || BETWEEN( yMinTo, yMin,   yMaxTo ) 
         || BETWEEN( yMinTo, yMax,   yMaxTo ) )
        passY = true;

      // condition 2
      else if ( ( ( yMinTo > yMax   ) && ( yMinTo - yMax   <= maxDistance ) )
             || ( ( yMin   > yMaxTo ) && ( yMin   - yMaxTo <= maxDistance ) ) )
         passY = true;

      if ( passX && passY )   // passes both?
         {
         // we are close, so check more detail
         float distance;
         switch( method )
            {
            case SIM_NEAREST:
               distance = pPoly->NearestDistanceToPoly( pToPoly, 2*maxDistance );  // ???? need to check threshold useage!!!
               break;

            case SIM_CENTROID:
               distance = pPoly->CentroidDistanceToPoly( pToPoly );
               break;
            }

         if ( distance <= maxDistance )
            {
            neighbors[ count ] = i;

            if ( distances != NULL )
               distances[ count ] = distance;

            count++;

            if ( count >= maxCount )
               return count;
            }
         }
      }

   return count;
   }


int MapLayer::GetNearestPoly( float x, float y ) const
   {
   float nearestDistance = 1.0e18f;
   int   nearestIndex    = -1;

   int count = GetRecordCount();

   for ( int i=0; i < count; i++ )
      {
      Poly *pPoly = GetPolygon( i );

      int vertexCount = pPoly->GetVertexCount();

      for ( int j=0; j < vertexCount-1; j++ )
         {
         const Vertex &start = pPoly->GetVertex( j );
         const Vertex &end   = pPoly->GetVertex( j+1 );

         float distance = 0.0f;
         bool onSegment = DistancePtToLineSegment( x, y, start.x, start.y, end.x, end.y, distance );

         //if ( onSegment )
         //   return i;
         if ( distance < 0.000001f )
            return i;

         if ( fabs(distance) < nearestDistance )
            {
            nearestDistance = (float) fabs(distance);
            nearestIndex    = i;
            }
         }
      }

   return nearestIndex;
   }


int MapLayer::GetNearestPoly( Poly *pPoly, float maxDistance, SI_METHOD method/*=SIM_NEAREST*/, MapLayer *pToLayer /*=NULL*/ ) const
   {
   if ( pToLayer == NULL )
      pToLayer = (MapLayer*) this;

   int recordCount = pToLayer->GetRecordCount();
   int nearestIndex = 0;
   float nearestDistance = 1.0e12f;

   float xMin, xMax, yMin, yMax;
   pPoly->GetBoundingRect( xMin, yMin, xMax, yMax );

   ASSERT( xMin <= xMax );
   ASSERT( yMin <= yMax );

   float xMinTo, xMaxTo, yMinTo, yMaxTo;
   float maxDistSq = maxDistance * maxDistance;

   // iterate through the poly's in the to layer
   for ( int i=0; i < recordCount; i++ )
      {
      Poly *pToPoly = pToLayer->GetPolygon( i );

      if ( pToPoly == pPoly ) // skip this polygon
         continue;

      // are we even close?
      bool ok = pToPoly->GetBoundingRect( xMinTo, yMinTo, xMaxTo, yMaxTo );
      ASSERT( ok );

      bool passX = false;
      bool passY = false;

      ASSERT( xMinTo <= xMaxTo );
      ASSERT( yMinTo <= yMaxTo );

      // fundamentally, if the closest distance between the two poly's (D) is less
      // than the max distance (M), then the poly's are neighbors, e.g. D < M 
      //
      // To speed the search, we first look at the bounding rects.  The bounding rect represents
      // the maximum size of the polygons.  Therefore, if D for the bounding rects is greater than
      // M, then we can be assured that these polygons are not neighbors.  We apply this test as a
      // first cut.  Any poly's that pass are then examined vertex by vertex to get the closest distance

      // For the bounding rect, we further simplify the analysis.  D for overlapping rects is 0,
      // and so the poly passes, Rect's overlap if:
      //   1) one rect overlaps another, or
      //   2) the "left" edge of one is within D of the right edge of the other

      // check condition 1 for both poly's
      if (  BETWEEN( xMin,   xMinTo, xMax   ) 
         || BETWEEN( xMin,   xMaxTo, xMax   )
         || BETWEEN( xMinTo, xMin,   xMaxTo ) 
         || BETWEEN( xMinTo, xMax,   xMaxTo ) )
        passX = true;

      // condition 2
      else if ( ( ( xMinTo > xMax   ) && ( xMinTo - xMax   <= maxDistance ) )
             || ( ( xMin   > xMaxTo ) && ( xMin   - xMaxTo <= maxDistance ) ) )
         passX = true;

      // repeat for y's
      if (  BETWEEN( yMin,   yMinTo, yMax   ) 
         || BETWEEN( yMin,   yMaxTo, yMax   )
         || BETWEEN( yMinTo, yMin,   yMaxTo ) 
         || BETWEEN( yMinTo, yMax,   yMaxTo ) )
        passY = true;

      // condition 2
      else if ( ( ( yMinTo > yMax   ) && ( yMinTo - yMax   <= maxDistance ) )
             || ( ( yMin   > yMaxTo ) && ( yMin   - yMaxTo <= maxDistance ) ) )
         passY = true;

      if ( passX && passY )   // passes both?
         {
         // we are close, so check more detail
         float distance;
         switch( method )
            {
            case SIM_NEAREST:
               distance = pPoly->NearestDistanceToPoly( pToPoly, 2*maxDistance );  // ???? need to check threshold useage!!!
               break;

            case SIM_CENTROID:
               distance = pPoly->CentroidDistanceToPoly( pToPoly );
               break;
            }

         if ( distance < nearestDistance )
            {
            nearestIndex = i;
            nearestDistance = distance;
            }
         }
      }

   return nearestIndex;
   }




//-------------------- Watershed functions --------------//

float MapLayer::GetDownFlowPathDistance( int row, int col, MapLayer *pStreamLayer ) const
   {
   ASSERT( m_layerType == LT_GRID );

   float cellwidth = GetGridCellWidth();

   // check for stopping condition... (non-zero indicates river boundary)
   if ( pStreamLayer->IsCellInStream( row, col ) )
      return cellwidth/2;  // why cellwidth/2 ????  Note:  Cells near grid border have high values.  Why???

   // try to follow the path.  if FollowFlowPath() fails, we are at a border moving off the grid
   int newRow = row;
   int newCol = col;

   bool ok = FollowFlowPath( newRow, newCol );  // gets next cell coords along flowpath, returns false
                                                // if we new row is off a border
   if ( newRow != row && newCol != col )
      cellwidth *= 1.414f;

   if ( !ok )    // at a border?
      return GetNoDataValue();   // should probably use a large value....

   // otherwise, recurse and proceed      
   float flowPath = GetDownFlowPathDistance( newRow, newCol, pStreamLayer );

   return flowPath + cellwidth;
   }


float MapLayer::GetDownFlowPathGradient( int row, int col, MapLayer *pStreamLayer, MapLayer *pDEMLayer, int &count ) const
   {
   ASSERT( m_layerType == LT_GRID );
   float currentElev = 0;
   float downElev = 0;
   float gradAverage=0.0001f;
   float cellwidth = GetGridCellWidth();
    pDEMLayer->GetData(row,col,currentElev);
    count++;

   // check for stopping condition... (non-zero indicates river boundary)
   if ( pStreamLayer->IsCellInStream( row, col ) )
      return gradAverage;  // why cellwidth/2 ????  Note:  Cells near grid border have high values.  Why???

   // try to follow the path.  if FollowFlowPath() fails, we are at a border moving off the grid
   int newRow = row;
   int newCol = col;

   bool ok = FollowFlowPath( newRow, newCol );  // gets next cell coords along flowpath, returns false
                                                // if we new row is off a border
   if ( newRow != row && newCol != col )
      cellwidth *= 1.414f;
   pDEMLayer->GetData(newRow,newCol,downElev);
   
   float  gradient = (currentElev - downElev)/cellwidth;
   if (gradient==0.0f)
      gradient=0.0001f;

   if ( !ok )    // at a border?
      return gradient;   // should probably use a large value....

   // otherwise, recurse and proceed      
   gradAverage = GetDownFlowPathGradient( newRow, newCol, pStreamLayer, pDEMLayer , count);

   return (gradAverage + gradient );
   }




//------------------------------------------------------------------------------------------------
// Given a row and col, follow the flowpath to a stream and determine which side of the stream
// the original row and col were on
//------------------------------------------------------------------------------------------------

int MapLayer::GetSideOfStream( int row, int col, MapLayer *pStreamLayer ) const
   {
   ASSERT( pStreamLayer->m_layerType == LT_GRID );


   int newRow = row;
   int newCol = col;
	int side=-1;

   int previousRow = row;
   int previousCol = col;

	while (! pStreamLayer->IsCellInStream( row, col ) )
		{
		previousRow = row;
		previousCol = col;
	   bool ok = FollowFlowPath( row, col ); 
      if (! ok)
         break;
		}

	int landDir, streamDir;
	bool ok1 = GetData( row, col, streamDir );
   if (streamDir == GetNoDataValue()) // if the centroid of the cell is outside of the watershed area, the flowpath will move off grid...
      return side = -1;

	bool ok2 = GetData(previousRow, previousCol, landDir);

//  At this point, streamDir is the stream flow direction and landDir is the flow direction of the cell
//  flowing into the streamDir cell.

   ASSERT( ok1 == true);
   int newRowUp = row;
   int newColUp = col;

//  If the two flow directions are equal, we need the flow direction of the stream cell 
//  that is just upstream of the current stream cell.
   
   if (landDir == streamDir) 
		{
		bool ok3 = GetUpstreamCell(row, col ,pStreamLayer, newRowUp, newColUp);
      if (ok3)
		   bool ok4 = GetData(newRowUp, newColUp, streamDir);
		}

   int landPower = FlowDirToPower(landDir); // convert from flow direction into 2^power (get the power part)
   int streamPower = FlowDirToPower(streamDir);
   int relationship = -1;
   int angle = abs(landPower - streamPower);
//   bool landCardinal = false;
//   bool streamCardinal = false;

//   if (landPower == 0 || landPower == 2 || landPower == 4 || landPower == 6)
//      landCardinal = true;
//   if (streamPower == 0 || streamPower == 2 || streamPower == 4 || streamPower == 6)
//      streamCardinal = true;

// These cases occur when the landDir is a cardinal direction (N E S W) or when the landDir and
// streamDir form an acute angle.  In this case, the side of the stream is unambiguous  

   if (angle == 4 )//the two directions face each other.  This should never occur
      relationship = 0;
   if ( angle==2 || angle==6) // unambigous
      relationship = 1;
   if (angle == 0 || angle == 3 || angle == 5 || angle == 1 || angle==7 ) // ambiguous - look one stream cell upstream
      relationship = 2;

   switch ( relationship ) 
      {
      case 0:
         {
         side = -1;
         break;
         }
      case 1:
         {
         int streamTemp = streamDir;
         for ( int i=0; i<3; i++ )
            {
            streamTemp*=2;
            if (streamTemp > 128)
               streamTemp = 1;
            if (landDir == streamTemp)
               {
               side = 1; // left side of stream, looking downstream
               break;
               }
            }
         if (side == -1 )
            side = 2; // right side of the stream, looking downstream
      
         break;
         }

      case 2:     
         {

		   bool ok3 = GetUpstreamCell(row, col ,pStreamLayer, newRowUp, newColUp);
         if (ok3)
		      bool ok4 = GetData(newRowUp, newColUp, streamDir);
         int streamTemp = streamDir;
         for ( int i=0; i<3; i++ )
            {
            streamTemp*=2;
            if (streamTemp > 128)
               streamTemp = 1;
            if (landDir == streamTemp)
               {
               side = 1;
               break;
               }
            }
         if (side == -1)
            side = 2;        
         break;
         }
      break;
         

      }  // end of:  switch( change )


   return side;

	} 

int MapLayer::FlowDirToPower(int flowDirection) const
   {
   int power = -1;
   switch ( flowDirection ) 
      {
      case 1:     
         power=0;
         break;
      case 2:     
         power=1;
         break;
      case 4:     
         power=2;
         break;
      case 8:     
         power=3;
         break;
      case 16:     
         power=4;
         break;
      case 32:     
         power=5;
         break;
      case 64:     
         power=6;
         break;
      case 128:     
         power=7;
         break;
      }
   return power;
   }
         


// FollowFlowPath() takes a cell coordinate is input, and returns the coordinate 
// of the next cell down the flow path, returning true if successful, false if we're off the
// DEM grid (nodata) or if the downstream neighbor
//
// NOTE:  Assumes this is a DEM derived flowdir grid!!!

bool MapLayer::FollowFlowPath( int &row, int &col ) const
   {
   ASSERT( m_layerType == LT_GRID );
   // get the flow path direction for this cell

   int dir;
   bool ok = GetData( row, col, dir );
   //ASSERT( ok );
   if ( ! ok )
      return false;

   if ( dir == GetNoDataValue() )
//      return GetNeighborCoords(dir,row,col);
         return false;

   return GetNeighborCoords( dir, row, col );
   }


// GetNeighborCoords() - takes a direction and a cell coord, returns the neighbors' cell coord
// if the neighbor cell exists - returns false if a bad direction is passed, or if the neighbor is 
// off the grid.

bool MapLayer::GetNeighborCoords( int direction, int &row, int &col ) const
   {
   ASSERT( m_layerType == LT_GRID );

   switch( direction )
      {      
      case N:     row--;         break;
      case NE:    row--; col++;  break;
      case E:            col++;  break;
      case SE:    row++; col++;  break;
      case S:     row++;         break;
      case SW:    row++; col--;  break;
      case W:            col--;  break;
      case NW:    row--; col--;  break;

      default: return false; // no direction data, return false with original coords unchanged
      }
   
   // take care of boundaries
   bool retVal = true;
   if ( row < 0 )
      {
      row = 0;
      retVal = false;
      }
   
   else if ( row >= GetRecordCount() )
      {
      row = GetRecordCount() - 1;
      retVal =  false;
      }

   if ( col < 0 )
      {
      col = 0;
      retVal = false;
      }
   
   else if ( col >= GetFieldCount() )
      {
      col = GetFieldCount();
      retVal = false;
      }

   return retVal;
   }


// MapLayer::FindPourPoint( int &row, int &col, MapLayer *pStreamLayer, bool firstTime )
//
// Takes an initial guess, and follows down flow paths until a pour point is reached.
// pStreamLayer can be NULL, in which case the pour point is NOT checked to see if it is on the 
// river coverage
//
// NOTE:  Assumes this is a DEM coverage


bool MapLayer::FindPourPoint( int &row, int &col, MapLayer *pStreamLayer /*=NULL*/, bool firstTime /*=true*/ ) const
   {
   ASSERT( m_layerType == LT_GRID );

   // look for an existing pour point file
/*
   char file[ 128 ];
   lstrcpy( file, m_name );

   char *pExt = strrchr( file, '.' );
   if ( pExt == NULL )
      { 
      lstrcat( file, "." );
      pExt = file + lstrlen( file );
      }
   else
      pExt++;

   lstrcpy( pExt, "pp" );
   FILE *fp = fopen(  file, "rt" );
   if ( fp != NULL )
      {
      int _row, _col;
      fscanf( fp, "%i %i", &_row, &_col );
      fclose( fp );
      row = _row;
      col = _col;
      return true;
      }
*/
   // no pour point file founds, continue looking
   int counter = 0;

   // try to find a starting point
   if ( firstTime )
      {
      int rows = GetRecordCount();
      int cols = GetFieldCount();

      for ( row=rows/2; row < rows; row++ )  
         for ( col=cols/2; col < cols; col++ )
            {
            int dir;
            bool ok = GetData( row, col, dir );
            ASSERT( ok );
            
            if ( dir != GetNoDataValue() )
             goto next;
            }

      ASSERT( 0 );

next:
      ;
   }
   int lastRow = row;
   int lastCol = col;

   // loop until FollowFlowPath() fails, indicating border or no-data flow cell found
   while ( FollowFlowPath( row, col ) && counter < 3000 )
      {
      counter++;
      lastRow = row;
      lastCol = col;
      }

   if ( counter > 2999 )
      WarningMsg( "Counter exceeded in FindPourPoint(), aborting..." );

   // restore the last coordinates before failure
   row = lastRow;
   col = lastCol;

   // make sure we're on a river cell
   bool found = true;

   if ( pStreamLayer != NULL )
      {
      int river;
      bool ok = pStreamLayer->GetData( row, col, river );
      ASSERT( ok );

      found = ( river != 0 );
      }

   if ( found )
      {
      PourPointDlg dlg;
      dlg.m_row = row;
      dlg.m_col = col;
      if ( dlg.DoModal() == IDOK )
         {
         row = dlg.m_row;
         col = dlg.m_col;

         FindPourPoint( row, col, pStreamLayer, false );
         }
      else // IDCANCEL, accept pourpoint
         {
         // write to disk
         char file[ 128 ];
         lstrcpy( file, m_name );

         char *pExt = strrchr( file, '.' );
         if ( pExt == NULL )
            { 
            lstrcat( file, "." );
            pExt = file + lstrlen( file );
            }
         else
            pExt++;

         lstrcpy( pExt, "pp" );
         FILE *fp = fopen(  file, "wt" );
         fprintf( fp, "%i %i", row, col );
         fclose( fp );
         }
      }
   else
      ErrorMsg( "Unable to find pour point..."  );

   return found;
   }
  


bool MapLayer::GetStreamIntersectionPoint( int &row, int &col, MapLayer *pStreamLayer ) const
   {
   // goes downstream, looking for a branch point
   ASSERT( m_layerType == LT_GRID );

   while( pStreamLayer->IsCellInStream( row, col ) == false )
      {
      bool ok = FollowFlowPath( row, col );

      if ( ! ok ) // did we hit a border?
         return false;
      }

   return true;
   }


int MapLayer::FindUpstreamBranchPoints( int row, int col, MapLayer *pStreamLayer, RowColArray &upstreamBranchCoordArray ) const
   {
   RowColArray upstreamCoordArray;

   int count = GetUpstreamCells( row, col, pStreamLayer, upstreamCoordArray );

   switch ( count )
      {
      case 0:  // headwater, stop looking
         break;

      case 1:  // in a stream reach
         FindUpstreamBranchPoints( upstreamCoordArray[ 0 ].row, upstreamCoordArray[ 0 ].col, pStreamLayer, upstreamBranchCoordArray );
         break;

      default:
         {
         upstreamBranchCoordArray.Add( row, col );

         for ( int i=0; i < count; i++ )
            FindUpstreamBranchPoints( upstreamCoordArray[ i ].row, upstreamCoordArray[ i ].col, pStreamLayer, upstreamBranchCoordArray );
         }
         break;

      }

   return upstreamBranchCoordArray.GetSize();
   }


int MapLayer::GetUpstreamCells( int row, int col, MapLayer *pStreamLayer, RowColArray &coordArray ) const
   {
   // look in all directions for cells that are:
   //     1) upstream (flowing into this cell as determined by the directions layer)
   // and 2) are a river cell ( as determined by a non-zero, valid value in the river layer)
   coordArray.RemoveAll();

   static int dirArray[] = { NE, N, NW, W, SW, S, SE, E };

   // check cell in eac direction, add to array if necessary
   int _row, _col;

   for ( int i=0; i < 8; i++ )
      {
      _row = row;
      _col = col;
      if ( IsCellUpstream( dirArray[ i ], _row, _col, pStreamLayer ) )  // send coords of this cells, 
         coordArray.Add( _row, _col ); // return coords of neighbor cell in the specified direction
      }

   return coordArray.GetSize();
   }

bool MapLayer::GetUpstreamCell( int row, int col, MapLayer *pStreamLayer, int &rowUp, int &colUp) const
   {
   // check cell in each direction, add to array if necessary
   int _row, _col;
   static int dirArray[] = { NE, N, NW, W, SW, S, SE, E };
   for ( int i=0; i < 8; i++ )
      {
      _row = row;
      _col = col;
      if ( IsCellUpstream( dirArray[ i ], _row, _col, pStreamLayer ) )  // send coords of this cells, 
         {
			rowUp = _row;
			colUp = _col;
         return true;
			}
      
		}
   return false;
   }


bool MapLayer::IsCellUpstream( int neighborDir, int &row, int &col, MapLayer *pStreamLayer ) const
   {
   int myRow = row;
   int myCol = col;

   // get the coords of the neighbor cell in the specified direction
   if ( GetNeighborCoords( neighborDir, row, col ) == false )  // sends my coords, returns neighbor coords
      return false;  //  hit a border

   int neighborRow = row;
   int neighborCol = col;

   // next see if the neighbor cell is a river
   if ( pStreamLayer->IsCellInStream( neighborRow, neighborCol ) == false )
      return false;

   // it is in the river, check to see if the neighbor cell is flowing toward the starting cell
   row = neighborRow;
   col = neighborCol;
   FollowFlowPath( row, col );  
  
   if ( myRow == row && myCol == col )
      {
      row = neighborRow;
      col = neighborCol;
      return true;
      }
   else
      return false;
   }


bool MapLayer::IsCellInStream( int row, int col ) const
   {
   int value;
   bool ok = GetData( row, col, value );
   ASSERT( ok );

   if ( value == GetNoDataValue() )
      return false;

   return ( value > 0 );      // okay for negative nodata values
   }

//---------------------------------------------------------------------------------
// MapLayer::IsCellUpslope()
//
// Determines if a cell in the specified direction is upslope.  If pStreamLayer != NULL,
// excludes cells in a stream from being considered upslope
//---------------------------------------------------------------------------------

bool MapLayer::IsCellUpslope( int neighborDir, int &row, int &col, MapLayer *pStreamLayer /*=NULL*/ ) const
   {
   int myRow = row;
   int myCol = col;

   // get the coords of the neighbor cell in the specified direction
   if ( GetNeighborCoords( neighborDir, row, col ) == false )  // sends my coords, returns neighbor coords
      return false;  //  hit a border

   int neighborRow = row;
   int neighborCol = col;

   // if its in the river, exclude it
   if ( pStreamLayer != NULL && pStreamLayer->IsCellInStream( neighborRow, neighborCol ) )
      return false;
   
   // check to see if the neighbor cell is flowing toward the starting cell
   row = neighborRow;
   col = neighborCol;
   FollowFlowPath( row, col );  
  
   if ( myRow == row && myCol == col )
      {
      row = neighborRow;
      col = neighborCol;
      return true;
      }
   else
      return false;
   }


//-------------------------------------------------------------------------------------
// MapLayer::GetUpslopeCells()
//
// Get all upslope cell immediate adjacent to the cell at row, col.
//
// If pStreamLayer != NULL, exclude any  cells that are in a stream
//-------------------------------------------------------------------------------------

int MapLayer::GetUpslopeCells( int row, int col, RowColArray &coordArray, MapLayer *pStreamLayer /*=NULL*/ ) const
   {
   // look in all directions for cells that are:
   //     1) upstream (flowing into this cell as determined by the directions layer)
   coordArray.RemoveAll();

   static int dirArray[] = { NE, N, NW, W, SW, S, SE, E };

   // check cell in each direction, add to array if necessary
   int _row, _col;

   for ( int i=0; i < 8; i++ )
      {
      _row = row;
      _col = col;
      if ( IsCellUpslope( dirArray[ i ], _row, _col, pStreamLayer ) )  // send coords of this cells, 
         coordArray.Add( _row, _col ); // return coords of neighbor cell in the specified direction
      }

   return coordArray.GetSize();
   }


bool MapLayer::GetUpslopeCell( int row, int col,  int &rowUp, int &colUp, MapLayer *pStreamLayer /*=NULL*/ ) const
   {
   // look in all directions for cells that are:
   //     1) upstream (flowing into this cell as determined by the directions layer)

   static int dirArray[] = { NE, N, NW, W, SW, S, SE, E };

   // check cell in each direction, add to array if necessary
   int _row, _col;

   for ( int i=0; i < 8; i++ )
      {
      _row = row;
      _col = col;
      if ( IsCellUpslope( dirArray[ i ], _row, _col, pStreamLayer ) )  // send coords of this cells, 
         {
         rowUp = _row;
         colUp = _col;
         return true;
         }
      }
   return false;

   }

//--------------------------------------------------------------------------------------
// MapLayer::DelineateWatershedFromPoint()
//
// delineate the subwatersheds, based on a pour point
// Note:  "this" layer must be a DEM layer
//--------------------------------------------------------------------------------------

int MapLayer::DelineateWatershedFromPoint( int row, int col, MapLayer *pWatershedLayer, int watershedID ) const
   {
   ASSERT( pWatershedLayer->GetType() == LT_GRID );

   // start at row, col, tag all points up stream
   RowColArray coordArray;

   int count = GetUpslopeCells( row, col, coordArray );

   if ( count == 0 )
      return 0;

   else
      {
      int upCount = 0;

      for ( int i=0; i < upCount; i++ )
         {
         ROW_COL &cell = coordArray[ i ];

         pWatershedLayer->SetData( cell.row, cell.col, watershedID );

         // recurse for rest of watershed
         upCount += DelineateWatershedFromPoint( cell.row, cell.col, pWatershedLayer, watershedID );
         }

      count += upCount;
      }

   return count;
   }


//--------------------------------------------------------------------------------------
// MapLayer::DelineateWatershedsFromStream()
//
// Delineate ALL watersheds based on stream reach, as defined by branch points
//
//  returns total number of cell mapped into watersheds, with watershedID incremented
//  by the number of watersheds delineated
//--------------------------------------------------------------------------------------

int MapLayer::DelineateWatershedsFromStream( int startRow, int startCol, MapLayer *pWatershedLayer,
                                             MapLayer *pStreamLayer, int &watershedID ) const
   {
   ASSERT( pWatershedLayer != NULL );
   ASSERT( pStreamLayer != NULL );
   ASSERT( pWatershedLayer->GetType() == LT_GRID );
   ASSERT( pStreamLayer->GetType() == LT_GRID );

   // start at pourRow, pourCol, tag all points upslope (but not upstream)
   RowColArray coordArray;

   int countFromHere = 0;

   // find all points outside of stream that are upslope from this point
   int upCount = GetUpslopeCells( startRow, startCol, coordArray, pStreamLayer );

   // for each one, get all upstream cells
   for ( int i=0; i < upCount; i++ )
      countFromHere += DelineateWatershedFromPoint( coordArray[ i ].row, coordArray[ i ].col, pWatershedLayer, watershedID );

   // repeat for next upstream point, looking for terminals and branch points
   upCount = GetUpstreamCells( startRow, startCol, pStreamLayer, coordArray );

   switch( upCount )
      {
      case 0:     // terminal cell in stream
         return countFromHere;

      case 1:     // still on this stream segment, delineate with same watershedID
         {
         int count = DelineateWatershedsFromStream( coordArray[ 0 ].row, coordArray[ 0 ].col,
                              pWatershedLayer, pStreamLayer, watershedID );
         countFromHere += count;
         return countFromHere;
         }

      default: // more than one upstream cell, so we're at a branch point in the stream coverage.
               // delineate each upstream reach with an incremented watershedID
         {
         int count = 0;
         for ( i=0; i < upCount; i++ )
            {
            count += DelineateWatershedsFromStream( coordArray[ i ].row, coordArray[ i ].col,
                              pWatershedLayer, pStreamLayer, ++watershedID );
            }
         countFromHere += count;
         return countFromHere;
         }
      }
   }
            


int MapLayer::DelineateStreams( float maxUpslopeArea, MapLayer *pStreamLayer )
   {
   ASSERT( pStreamLayer != NULL );
   ASSERT( pStreamLayer->GetType() == LT_GRID );

   // first, locate the pour point.  initial guess is in middle of the grid
   int row = GetRowCount() / 2;
   int col = GetColCount() / 2;

   bool found = FindPourPoint( row, col );
   ASSERT( found );

   DelineateStreamsFromPoint( row, col, maxUpslopeArea, pStreamLayer );

   pStreamLayer->ClearBins();
   pStreamLayer->SetBinColorFlag( BCF_GRAY );
   float minVal;
   float maxVal;
   pStreamLayer->GetGridDataMinMax(&minVal, &maxVal);
   pStreamLayer->SetBins(minVal, maxVal, (int)(maxVal-minVal)+1);
   pStreamLayer->ClassifyData();

   return 1;
   }

    
int MapLayer::DelineateStreamsFromPoint( int row, int col, float maxUpslopeArea, MapLayer *pStreamLayer, 
                                        int branchCount /*=0*/ )
   {
   // set stream coverage to zero/nodata values
   int rows = GetRowCount();
   int cols = GetColCount();

   if ( branchCount == 0 )
      {
      branchCount++;
      for ( int i=0; i < rows; i++ )
         for ( int j=0; j < cols; j++ )
            {
            int value;
            GetData( i, j, value );

            if ( value == (int) GetNoDataValue() )
               pStreamLayer->SetNoData( i, j );
            else
               pStreamLayer->SetData( i, j, 0 );
            }
      }

   // have pour point, now start working upstream
   RowColArray coordArray;

   // find all points that are upslope from this point
   int upCount = GetUpslopeCells( row, col, coordArray );

   if ( upCount == 0 )
      return 0;

   CArray <int, int& > streamCells;

   for ( int i=0; i < upCount; i++ )
      {
      // iterate throught each upslope cell, looking for sufficient drainage area
      row = coordArray[ i ].row;
      col = coordArray[ i ].col;

      float upslopeArea = GetTotalUpslopeArea( row, col );

      // mark this cell as a stream if there is sufficient upstream area
      if ( upslopeArea > maxUpslopeArea )
         streamCells.Add( i );
      }

   if ( streamCells.GetSize() > 1 )
      branchCount++;

   for ( i=0; i < streamCells.GetSize(); i++ )
      {
      row = coordArray[ streamCells[ i ] ].row;
      col = coordArray[ streamCells[ i ] ].col;

      pStreamLayer->SetData( row, col, 1 );

      upCount += DelineateStreamsFromPoint( row, col, maxUpslopeArea, pStreamLayer, branchCount );
      }

   return upCount;
   }


int MapLayer::GetTotalUpslopeCells( int row, int col) const
   {
   RowColArray coordArray;

   int upCount = GetUpslopeCells( row, col, coordArray );
   int size = coordArray.GetSize();
   ASSERT( upCount == size );

   int count = upCount;

   for ( int i=0; i < upCount; i++ )
      count += GetTotalUpslopeCells( coordArray[ i ].row, coordArray[ i ].col );

   return count;
   }



float MapLayer::GetTotalUpslopeArea( int row, int col ) const
   {
   int upCount = GetTotalUpslopeCells( row, col );

   return upCount * ( m_cellWidth * m_cellHeight );
   }

/*

  //This is John's original DEMtoFlowDir - kellie manipulated it below.  To return to the old version
  //uncomment this code (and commment out the version below)

  
int MapLayer::DEMtoFlowDir( MapLayer *pFlowLayer )
   {
   ASSERT( m_layerType == LT_GRID );
   ASSERT( pFlowLayer != NULL );
   ASSERT( pFlowLayer->GetType() == LT_GRID );

   // iterate through each cell, determining flow direction based on neighbor elevations
   int rows = GetRowCount();
   int cols = GetColCount();

   for ( int row=0; row < rows; row++ )
      {
      for ( int col=0; col < cols; col++ )
         {
         int dir = GetCellFlowDir( row, col );

         if ( dir < 0 )
            pFlowLayer->SetNoData( row, col );
         else 
            pFlowLayer->SetData( row, col, dir );

         }  // end of:  for ( col < cols )
      }  // end of:  for ( row < rows )

   // entire grid computed, set the legend and go...
   const int a=30;
   pFlowLayer->ClearBins();
   pFlowLayer->AddBin( RGB( 0, 0, 0 ), "0", BT_STRING, 0.0f, 0.1f );
   pFlowLayer->AddBin( RGB( 110-a, 110-a, 170-50 ), "E", BT_STRING, 0.5f, 1.5f );
   pFlowLayer->AddBin( RGB( 130-a, 130-a, 180-50 ), "SE", BT_STRING, 1.51f, 2.5f );
   pFlowLayer->AddBin( RGB( 150-a, 150-a, 190-50 ), "S", BT_STRING, 3.0f, 5.0f );
   pFlowLayer->AddBin( RGB( 170-a, 170-a, 200-50 ), "SW", BT_STRING, 7.0f, 9.0f );
   pFlowLayer->AddBin( RGB( 190-a, 190-a, 210-50 ), "W", BT_STRING, 15.0f, 17.0f );
   pFlowLayer->AddBin( RGB( 210-a, 210-a, 220-50 ), "NW", BT_STRING, 31.0f, 33.0f );
   pFlowLayer->AddBin( RGB( 230-a, 230-a, 230-50 ), "N", BT_STRING, 63.0f, 65.0f );
   pFlowLayer->AddBin( RGB( 250-a, 250-a, 240-50 ), "NE", BT_STRING, 127.0f, 129.0f );

   return 1;
   } 
*/

//--------------------------------------------------------------------------------------
// MapLayer::DEMtoFlowDir()
//
// Calculate a flow direction grid and estimate elevationally undefined flow directions
//
//  
//--------------------------------------------------------------------------------------
int MapLayer::DEMtoFlowDir( MapLayer *pFlowLayer )
   {
   ASSERT( m_layerType == LT_GRID );
   ASSERT( pFlowLayer != NULL );
   ASSERT( pFlowLayer->GetType() == LT_GRID );

   // iterate through each cell, determining flow direction based on neighbor elevations
   int rows = GetRowCount();
   int cols = GetColCount();

   for ( int row=0; row < rows; row++ )
      {
      for ( int col=0; col < cols; col++ )
         {
         int dir = GetCellFlowDir( row, col );

         if ( dir < 0 )
            pFlowLayer->SetNoData( row, col );
         else 
            pFlowLayer->SetData( row, col, dir );
            
         }  // end of:  for ( col < cols )
      }  // end of:  for ( row < rows )


   // now go back through the newly created flow dir grid to look for 0 values (which are flat
   // areas with undefined flow direction.)  Flow directions for these flat areas are taken from
   // neighboring cells with defined flow directions.  (see FlatAreaFlowDir)
     
   
   //John - it would be nice to calculate this all in a single loop, but we need to have all
   //the flow directions calculated first, so I could not see how to do it.  Anyway, it executes
   //quickly all the same.  If you see a better way, let me know.
   
   
      int dir;
      int   dirEstimate;
      for ( int _row=0; _row < rows; _row++ )
      {
      for ( int _col=0; _col < cols; _col++ )
         {

         int  ok = pFlowLayer->GetData( _row, _col, dir ); //Get the previously calculated
                                                           //flow directions

         if ( dir == 0 )                                    //If flow dir is undefined
            {
            dirEstimate = FlatAreaFlowDir( _row, _col, pFlowLayer );  //make an estimate
            pFlowLayer->SetData( _row, _col, dirEstimate );  //and reset the undefined
            }                                                //lue to the estimate
            
         }  // end of:  for ( col < cols )
      }  // end of:  for ( row < rows )

   // entire grid computed, set the legend and go...
   const int a=30;
   pFlowLayer->ClearBins();
   pFlowLayer->AddBin( RGB( 0, 0, 0 ), "0", BT_STRING, 0.0f, 0.1f );
   pFlowLayer->AddBin( RGB( 110-a, 110-a, 170-50 ), "E", BT_STRING, 0.5f, 1.5f );
   pFlowLayer->AddBin( RGB( 130-a, 130-a, 180-50 ), "SE", BT_STRING, 1.51f, 2.5f );
   pFlowLayer->AddBin( RGB( 150-a, 150-a, 190-50 ), "S", BT_STRING, 3.0f, 5.0f );
   pFlowLayer->AddBin( RGB( 170-a, 170-a, 200-50 ), "SW", BT_STRING, 7.0f, 9.0f );
   pFlowLayer->AddBin( RGB( 190-a, 190-a, 210-50 ), "W", BT_STRING, 15.0f, 17.0f );
   pFlowLayer->AddBin( RGB( 210-a, 210-a, 220-50 ), "NW", BT_STRING, 31.0f, 33.0f );
   pFlowLayer->AddBin( RGB( 230-a, 230-a, 230-50 ), "N", BT_STRING, 63.0f, 65.0f );
   pFlowLayer->AddBin( RGB( 250-a, 250-a, 240-50 ), "NE", BT_STRING, 127.0f, 129.0f );
   pFlowLayer->AddNoDataBin();

   return 1;
   } 


// returns -1 for nodata, 0 for pit or flat area, > 0 for direction
int MapLayer::GetCellFlowDir( int row, int col ) const
   {
   float myElev, distance, test;
         
   // start out with this cell as the lowest cell
   bool ok = GetData( row, col, myElev );
   ASSERT( ok );

   if ( myElev == m_noDataValue )
      return -1;
   
   float lowestElev = myElev;
   int lowestDir = 0;
   float lowestTest = 0.0f;
   //bool onEdge = false;

   for ( int dir=E; dir <= NE; dir*=2 )
      {
      if ( dir==E || dir==S || dir==W || dir==N )
        
         distance = 1.0f;
      else
         distance = 1.41214f;
      
      int _row = row;
      int _col = col;

      ok = GetNeighborCoords( dir, _row, _col ); 

      if ( ! ok )  // off grid, ignore it
         continue;

      float neighborElev;
      ok = GetData( _row, _col, neighborElev );
      ASSERT( ok );
      test = (myElev - neighborElev) / distance;
      if ( neighborElev == m_noDataValue )
         {
         //onEdge = true;
         if ( lowestDir == 0 )
            lowestDir = dir;
         }
      else //not on edge
         //filling the DEM still leaves the cells undefined.  We need to make a guess as to where
         //the water will go when two or more adjacent cells have equal elevation (and the rest of the 
         //adjacent cells are higher) J&D provide what seems like reasonable soln.  
         //I partially implemented this as MapLayer::FlatAreaFlowDir (see below).
         if ( test > lowestTest )
            {
            // Jenson Domingue suggest a distance weighted drop as opposed to a simple drop
            // It makes sense.  Divide the drop by the distance from centroids (1 for
            // a non-corner, root2 for a corner)  This is why we are testing with 'test' rather
            // my myElev and neighborElev.

            lowestTest = test;
            lowestDir = dir;  
            
         }
         
      }  // end of:  for ( dir < 129 )  and we know the lowest elevation neighbor

   // did we find a downstream cell?
   return lowestDir;  //return the direction of the flow path (1 through 128)
   }


//--------------------------------------------------------------------------------------
// MapLayer::FlatAreaFlowDir
//
// Calculate a flow directions for cells that have no neighbors with lower elevations.  The 
// standard method cannot calculate a flow direction for this case (it's elevationally undefined)
// This method looks for neighbor cells with defined flow directions and arbitrarily assigns
// the neighbors flow direction to the undefined cell.  In this way, flow directions are calculated
// for large flat areas, moving from the borders of the areas inward.  

// It is an arbitrary solution, taken from Jenson and Domingue. The key is that water will 
// continue to move downslope.
//--------------------------------------------------------------------------------------

int MapLayer::FlatAreaFlowDir( int row, int col, MapLayer *pFlowLayer )
   {
    int myDir, dir, neighborDir;     
   // start out with this cell as the lowest cell
   bool ok = GetData( row, col, myDir );
   ASSERT( ok );

   for ( dir=E; dir <= NE; dir*=2 )
      {
      int _row = row;
      int _col = col;

      ok = pFlowLayer->GetNeighborCoords( dir, _row, _col ); 

      ok = pFlowLayer->GetData( _row, _col, neighborDir );
      ASSERT( ok );

      if ( neighborDir > 0 )
         {
         myDir = neighborDir;
         }

         
      }  // end of:  for ( dir < 129 )  and we know the lowest elevation neighbor

   // did we find a downstream cell?
   return myDir;  //return the direction of the flow path (1 through 128)
   }


//--------------------------------------------------------------------------------------
// MapLayer::DEMtoFilledDEM
//
// This method is the loop structure used to fill pits using the method FillDEMCell
//--------------------------------------------------------------------------------------

int MapLayer::DEMtoFilledDEM( MapLayer *pFilledDEMLayer )
   {
   ASSERT( m_layerType == LT_GRID );
   ASSERT( pFilledDEMLayer != NULL );
   ASSERT( pFilledDEMLayer->GetType() == LT_GRID );

   // iterate through each cell, determining flow direction based on neighbor elevations
   int rows = GetRowCount();
   int cols = GetColCount();

   for ( int row=0; row < rows; row++ )
      {
      for ( int col=0; col < cols; col++ )
         {
         float fillElev = FillDEMCell( row, col );
          pFilledDEMLayer->SetData( row, col, fillElev );
         }  // end of:  for ( col < cols )
      }  // end of:  for ( row < rows )


   return 1;
   } 


// returns -1 for nodata, 0 for pit or flat area, > 0 for direction

//--------------------------------------------------------------------------------------
// MapLayer::FillDEMCell
//
// Searches for sinks and raises them to the lowest neighboring elevation.
//  
//--------------------------------------------------------------------------------------

float MapLayer::FillDEMCell( int row, int col )
   {
   float myElev;
   float fillElev = 0;           
   // start out with this cell as the lowest cell
   bool ok = GetData( row, col, myElev );
   ASSERT( ok );

   if ( myElev == m_noDataValue )
      return m_noDataValue;

   for ( int dir=E; dir <= NE; dir*=2 )
      {
      int _row = row;
      int _col = col;

      ok = GetNeighborCoords( dir, _row, _col ); 

      if ( ! ok )  // off grid, ignore it
         continue;

      float neighborElev;
      ok = GetData( _row, _col, neighborElev );
      ASSERT( ok );

      if ( fillElev == 0)
         fillElev = neighborElev;
      
      if ( neighborElev <= myElev ) //the cell is NOT a sink
         
         return myElev;             //so return the original elevation

      else                          //The cell IS a sink
         {
         if (fillElev > neighborElev)  //Check for the LOWEST neighbor elevation.
                                          
         
         fillElev = neighborElev;// Raise cell elevation to the lowest neighboring elevation
                 
         }
     
      }  // end of:  for ( dir < 129 )  and we know the lowest elevation neighbor

   // did we find a downstream cell?
   return fillElev;  //return the direction of the flow path (1 through 128)
   }


int MapLayer::FindAdjacentRegion( int row, int col, RowColArray &coordArray ) const
   {
   ASSERT( m_layerType == LT_GRID );

   float myValue;
   bool ok = GetData( row, col, myValue );

   // no data?
   if ( myValue == m_noDataValue )
      return 0;

   for ( int dir=E; dir <= NE; dir*=2 )
      {
      int _row = row;
      int _col = col;

      ok = GetNeighborCoords( dir, _row, _col );

      if ( ! ok )  // off grid, ignore it
         continue;

      float neighborValue;
      ok = GetData( row, col, neighborValue );
      ASSERT( ok );

      if ( myValue == neighborValue )  // do the have the same attribute?
         {
         // have we already visted this cell?
         int index = coordArray.Find( _row, _col );

         if ( index < 0 )  // not in list already
            {
            coordArray.Add( _row, _col );
            FindAdjacentRegion( _row, _col, coordArray );
            }
         }
      }  // end of:  for ( all directions )

   return coordArray.GetSize();
   }


   
// NOT IMPLEMENTED!!!
int MapLayer::GridToPoly( MapLayer *pPolyLayer )
   {
/*
   ASSERT( m_layerType == LT_GRID );

   // reinitialize polygon layer
   pPolyLayer->m_layerType = LT_POLYGON;
   pPolyLayer->m_polyArray.RemoveAll();
   //pPolyLayer->m_binArray = m_binArray;

   if ( pPolyLayer->m_pDbTable != NULL )
      delete pPolyLayer->m_pDbTable;

   pPolyLayer->m_pDbTable = NULL;
   pPolyLayer->m_pData    = NULL;

   pPolyLayer->m_activeField = 0;   // column in data object
   pPolyLayer->m_isVisible = true;

   pPolyLayer->m_vxMin = m_vxMin;
   pPolyLayer->m_vxMax = m_vxMax;
   pPolyLayer->m_vyMin = m_vyMin;
   pPolyLayer->m_vyMax = m_vyMax;
   pPolyLayer->m_vzMin = m_vzMin;
   pPolyLayer->m_vzMax = m_vzMax;
   pPolyLayer->m_vExtent = m_vExtent;

   pPolyLayer->m_queryResults.RemoveAll();

   // make a temporary int matrix to designate what cells have already been vectorized
   int rows = GetRowCount();
   int cols = GetColCount();
   int id = 0;

   // NOTE:  ASSUMES INTEGER GRID!!!
   IntMatrix *visited = new IntMatrix( rows, cols, -1 );
   int value;
   int noDataValue = (int) GetNoDataValue();

   for ( int row=0; row < rows; row++ )
      {
      for ( int col=0; col < cols; col++ )
         {
         if ( visited->Get( row, col ) >= 0 )  // already visited? then skip it.
            continue;

         bool ok = GetData( row, col, value );
         ASSERT( ok );

         if ( value == noDataValue )         // nothing there? then skip it
            {
            visited->Set( row, col, 0 );
            continue;
            }

         // all set to start a new polygon....
         Poly *pPoly = new Poly;
         pPoly->m_id = id++;

         // is it a single pixel?
         int dir = 1;
         bool singlePixel = true;

         // E  = 1; S  = 4; W  = 16; N  = 64;
         do {
            int nrow = row;
            int ncol = col;
            if ( GetNeighborCoords( dir, nrow, ncol ) > 0 )
               {
               int nvalue;
               GetData( nrow, ncol, nvalue );
               if ( nvalue != value )
                  {
                  singlePixel = false;
                  break;
                  }
               }

            dir *= 4;
            } while ( dir < 128 );

         if ( singlePixel )
            {
            float x, y;
            GetGridCellCenter( row, col, x, y );
            x -= m_cellWidth/2;
            y -= m_cellHeight/2;
            pPoly->AddVertex( Vertex( x, y ) );                            // upper left?
            pPoly->AddVertex( Vertex( x+m_cellWidth, y ) );                // upper right?
            pPoly->AddVertex( Vertex( x+m_cellWidth, y+m_cellHeight ) );   // lower right?
            pPoly->AddVertex( Vertex( x, y+m_cellHeight ) );               // lower left ?
            pPoly->AddVertex( Vertex( x, y ) );                            // upper left? (closes poly)
            continue;
            }

         // not a single pixel, find the region border         
         CUIntArray chain;    // hold direction chain for border  N, S, E, W
         int p0Row = row;
         int p0Col = col;
         int p1Row, p1Col, pn1Row, pn1Col, pnRow, pnCol;

         int prevDir = 3;   // previous move direction from previous border element to the current border element
         int nextDir = prevDir;
         int count   = 0;

         // search 3x3 neighborhood in an anticlockwise direction, beginning the search in the pixel 
         // positioned in the direction (dir+3) mod 4

loop:         
         if ( count == 1 )
            {
            p1Row = row;
            p1Col = col;


         // get anticlockwise direction (based on 4-connectivity)
         for ( int i=0; i < 4; i++ )
            {
            ASSERT( i != 3 );    // don't look more than three directions
            nextDir = (nextDir+3) % 4; // move anti-clockwise

            switch( nextDir )
               {
               case 0:  dir = E; break;
               case 1:  dir = N; break;
               case 2:  dir = W; break;
               case 3:  dir = S; break;
               default: ASSERT( 0 );
               }

            int nrow = row;
            int ncol = col;
            if ( GetNeighborCoords( dir, nrow, ncol ) )
               {
               int nvalue;
               GetData( nrow, ncol, nvalue );
               if ( nvalue == value )     // found a neighbor on the border in this region
                  {
                  prevDir = nextDir;      // update direction path
                  chain.Add( dir );       // add direction to chain
                  break;
                  }
               }
            }  // end of: for ( i < 4 )

         // if the current boundary element is the same as the second border element
         

   

         ExtendGridPoly( pPoly, row, col, value );

         {
         //-- add a vertex to the polygon --//
         //pPoly->AddVertex( v );

         //if ( v.x > m_vxMax ) m_vxMax = v.x;     // check bounds
         //if ( v.y > m_vyMax ) m_vyMax = v.y;
         //if ( v.x < m_vxMin ) m_vxMin = v.x;
         //if ( v.y < m_vyMin ) m_vyMin = v.y;
         }  // end of: while buffer != 'E' (adding additional vertexes)

         m_polyArray.Add( pPoly );  // add the polygon to the Map's collection
         }
      }

   delete visited;
*/

/*  if ( ( m_vxMax-m_vxMin ) > ( m_vyMax - m_vyMin ) )
      m_vExtent = m_vxMax - m_vxMin;
   else
      m_vExtent = m_vyMax - m_vyMin;

   InitPolyLogicalPoints();   // create logical points for this set
*/

   return 0;
   }
  

void MapLayer::ExtendGridPoly( Poly *pPoly, int row, int col, int value )
   {

   // the fact that we are here implies adding the UL vertex.
   Vertex v;
   float x, y;
   GetGridCellCenter( row, col, x, y );
   v.x = x - m_cellWidth;
   v.y = y - m_cellHeight;
   pPoly->AddVertex( v );   // add UL


   }


int MapLayer::PopulateNearestDistanceToPoly( Poly *pToPoly, int col, REAL threshold, LPCSTR label /*=NULL*/ )
	{
	if ( m_layerType != LT_LINE && m_layerType != LT_POLYGON )
		return -1;

	if ( col < 0 )
		return -3;

   SetFieldType( col, TYPE_FLOAT );
   if ( label != NULL )
      SetFieldLabel( col, label );

	int count = GetRecordCount();

	for ( int i=0; i < count; i++ )
	   {
      Poly *pPoly = m_polyArray[ i ];
      float distance = pPoly->NearestDistanceToPoly( pToPoly, threshold );

      SetData( i, col, distance );
	   }

   return count;
	}
   


int MapLayer::PopulateFlowPathDistance( MapLayer *pFlowDir , MapLayer *pStreamGrid, int method)
	{
   //  The watershed layer should be a polygon.  For each flowpathdistance (a grid), we need to 1.  Determine which
      // subbasin it is in and 2. sum up that distance (and count the # of grid cells in the watershed)
   int polygonCount   = GetRecordCount();
 //  FDataObj *basinFPDistance = new FDataObj;
	for ( int i=0; i < polygonCount; i++ )
		{
      CArray <float, float& > fpDist;
		Poly *pThisPoly = m_polyArray[ i ];
      float flowPathDistance=0.0f;
      int flowPathCount=0;
      int rows = pFlowDir->GetRowCount();
      int cols = pFlowDir->GetColCount();
      for ( int row=0; row < rows; row++ )
         {
         for ( int col=0; col < cols; col++ )
            {
            float x = 0;
            float y = 0;
            pFlowDir->GetGridCellCenter(row, col, x, y);
            Vertex point;
            point.x = x;
            point.y = y;
            if (pThisPoly->IsPointInPoly(point))
               {

               float distance = pFlowDir->GetDownFlowPathDistance( row, col, pStreamGrid );
               if (method==1)// we just want the average fpdistance
                  {
                  flowPathDistance += distance;
                  flowPathCount++;
                  }
               if (method==2)// we want to maintain the entire distribution
                  {
                   fpDist.Add(distance);
                  }
               }
            }  // end of:  for ( col < cols )
         }  // end of:  for ( row < rows )
//      basinFPDistance.Add(fpDist);

      float averageFPDistance = flowPathDistance/flowPathCount;
 		m_pMap->Notify( NT_CALCDIST, i, polygonCount );   
      SetData(i,m_pData->GetCol("EXTRA_1"), averageFPDistance);
      } // end of this polygon, now go on to the next one!
//   basinFPDistance.WriteAscii("c:\\temp\\fpdist.csv");
 //  delete basinFPDistance; 
   return 1;   
   }

int MapLayer::PopulateNearestDistanceToCoverage( MapLayer *pToLayer, int col, REAL threshold, LPCSTR label /*=NULL*/ )
	{
	if ( m_layerType != LT_LINE && m_layerType != LT_POLYGON )
		return -1;

   if ( pToLayer->GetType() != LT_LINE && pToLayer->GetType() != LT_POLYGON )
      return -2;

	if ( col < 0 )
		return -3;

   SetFieldType( col, TYPE_FLOAT );
   if ( label != NULL )
      SetFieldLabel( col, label );

	int thisCount = GetRecordCount();
   int toCount   = pToLayer->GetRecordCount();

	for ( int i=0; i < thisCount; i++ )
		{
		float nearestDistance = 1.0e16f;

		Poly *pThisPoly = m_polyArray[ i ];

		for ( int j=0; j < toCount; j++ )
			{
			float distance = nearestDistance;

			Poly *pToPoly = pToLayer->GetPolygon( j );
			if ( IsPolyInMap( pToPoly )  )
				distance = pThisPoly->NearestDistanceToPoly( pToPoly, threshold );

			if ( distance < nearestDistance )
				nearestDistance = distance;
			}

		m_pMap->Notify( NT_CALCDIST, i, thisCount );

		SetData( i, col, nearestDistance );
	   }

   return thisCount;
	}


/*

int MapLayer::PopulateNearestDistanceToSelectedCoverage( MapLayer *pToLayer, int col, REAL threshold, int colToSelectFrom,LPCSTR label  )
	{
	if ( m_layerType != LT_LINE && m_layerType != LT_POLYGON )
		return -1;

   if ( pToLayer->GetType() != LT_LINE && pToLayer->GetType() != LT_POLYGON )
      return -2;

	if ( col < 0 )
		return -3;

   SetFieldType( col, TYPE_FLOAT );
   if ( label != NULL )
      SetFieldLabel( col, label );

	int thisCount = GetRecordCount();
   int toCount   = pToLayer->GetRecordCount();

	for ( int i=0; i < thisCount; i++ )
		{
		float nearestDistance = 1.0e16f;
		Poly *pThisPoly = m_polyArray[ i ];

		for ( int j=0; j < toCount; j++ )
			{
			int isTrue = 0;
			pToLayer->GetData(j,colToSelectFrom,isTrue);
			if ( isTrue )
				{
				Poly *pToPoly = pToLayer->GetPolygon( j );
				float distance = pThisPoly->NearestDistanceToPoly( pToPoly, threshold );

				if ( distance < nearestDistance )
					nearestDistance = distance;
				}
			
			}
			m_pMap->Notify( NT_CALCDIST, i, thisCount );
			SetData( i, col, nearestDistance );
	   }

   return thisCount;
	}
*/
int MapLayer::PopulateLengthOfIntersection( MapLayer *pToLayer, int col, LPCSTR label )
	{
	if ( m_layerType != LT_LINE && m_layerType != LT_POLYGON )
		return -1;

   if ( pToLayer->GetType() != LT_LINE && pToLayer->GetType() != LT_POLYGON )

      return -2;

	if ( col < 0 )
		return -3;

   SetFieldType( col, TYPE_FLOAT );

   if ( label != NULL )
      SetFieldLabel( col, label );

	int thisCount = GetRecordCount();
   int toCount   = pToLayer->GetRecordCount();

	for ( int i=0; i < thisCount; i++ )
	   {
      float length = 0.0f;

      Poly *pThisPoly = m_polyArray[ i ];

      for ( int j=0; j < toCount; j++ )
         {
         Poly *pToPoly = pToLayer->GetPolygon( j );
         float polyLength = pThisPoly->GetIntersectionLength( pToPoly );
			length += polyLength;
			}

      m_pMap->Notify( NT_CALCDIST, i, thisCount );

      SetData( i, col, length );
	   }

   return thisCount;
	}

float MapLayer::GetCentroidFlowPathDistance( Poly *pPoly, MapLayer *pFlowDirectionLayer, MapLayer *pToLayer) const
	{
	Vertex centroid = pPoly->GetCentroid();
	int row = 0;
	int col = 0;

	pFlowDirectionLayer->GetGridCellFromCoord( centroid.x, centroid.y, row, col);

	float flowPathDistance = pFlowDirectionLayer->GetDownFlowPathDistance(  row,  col, pToLayer );
	
	return flowPathDistance;
	}

int MapLayer::PopulateFlowPathDistanceFromCentroid( MapLayer *pFlowDirectionLayer, MapLayer *pToLayer, int colToSet )
	{
	if (  m_layerType == LT_POLYGON )
		{
		int thisCount = GetRecordCount();
		int toCount   = pToLayer->GetRecordCount();
		
		for ( int i=0; i < thisCount; i++ )
			{
			Poly *pPoly = GetPolygon(i);

			float distance = GetCentroidFlowPathDistance(pPoly, pFlowDirectionLayer, pToLayer);

			SetData(i, colToSet, distance);
			}
		return 1;
		}

   return 0;
	}

int MapLayer::PopulateSideOfStream( MapLayer *pFlowDirectionLayer, MapLayer *pToLayer, int colToSet)
	{
	if (  m_layerType == LT_POLYGON )
		{
		int thisCount = GetRecordCount();
		int toCount   = pToLayer->GetRecordCount();
      
      for ( int i=0; i < thisCount; i++ )
			{
			Poly *pPoly = GetPolygon(i);
			Vertex centroid = pPoly->GetCentroid();
         int colOrder = GetFieldCol("ORDER");
            
         int order = GetData(i,colOrder,order);
         int side = -1;      
         if (order == 1)
            side = 1;
         else
            {
		   	int row = 0;
		   	int col = 0;
		   	pFlowDirectionLayer->GetGridCellFromCoord( centroid.x, centroid.y, row, col);
		   	side = pFlowDirectionLayer->GetSideOfStream(row, col, pToLayer);
            }

         SetData(i, colToSet, side);
			}
      }
   return 1;
   }

int MapLayer::PopulateSelectedLengthOfIntersection( MapLayer *pToLayer, int col, int colToSelectFrom, LPCSTR label  )
	{
   return -1;
	}


int MapLayer::Translate( float dx, float dy )
   {
   // only poly and line coverages (for now)
	if ( m_layerType != LT_LINE && m_layerType != LT_POLYGON )
		return -1;

	int polyCount = GetRecordCount();

	for ( int i=0; i < polyCount; i++ )
	   {
      Poly *pPoly = m_polyArray[ i ];

      int vertexCount = pPoly->GetVertexCount();

      for ( int j=0; j < vertexCount; j++ )
         {
         Vertex &v = (Vertex&) pPoly->GetVertex( j );
         v.x += dx;
         v.y += dy;
         }

      pPoly->InitLogicalPoints( m_pMap );
      }

   return polyCount;
   }


bool MapLayer::GetZExtents( float &zMin, float &zMax )
   {
	if ( m_layerType != LT_LINE && m_layerType != LT_POLYGON )
		return false;

   if ( m_3D == false )
      {
      zMin = zMax = 0.0f;
      return false;
      }

   if ( m_vzMin != m_vzMax )
      {
      zMin = m_vzMin;
      zMax = m_vzMax;
      return true;
      }

   zMin = (float)  10e20;
   zMax = (float) -10e20;

	int polyCount = GetRecordCount();

	for ( int i=0; i < polyCount; i++ )
	   {
      Poly *pPoly = m_polyArray[ i ];

      int vertexCount = pPoly->GetVertexCount();

      for ( int j=0; j < vertexCount; j++ )
         {
         const Vertex &v = pPoly->GetVertex( j );

         if ( v.z < zMin )  zMin = v.z;
         if ( v.z > zMax )  zMax = v.z;
         }
      }

   m_vzMin = zMin;
   m_vzMax = zMax;
   return true;
   }


bool MapLayer::GetRecordLabel ( int rec, CString &label ) const
   {
   if  ( m_labelCol < 0 )
      return false;

   return GetData( rec, m_labelCol, label );
   }



// NOTE: FOLLOWING CODE IS INCOMPLETE!!!!
int MapLayer::DissolvePolys( int rec0, int rec1 )
   {
   ASSERT( rec0 < GetRecordCount() && rec0 >= 0 );
   ASSERT( rec1 < GetRecordCount() && rec1 >= 0 );

   Poly *pPoly0 = GetPolygon( rec0 );
   Poly *pPoly1 = GetPolygon( rec1 );

   // find first point in common
   int vertexCount0 = pPoly0->GetVertexCount();
   int vertexCount1 = pPoly1->GetVertexCount();
   int i,j;
   int first0 = -1;  // index of first vertex found on poly 0
   int first1 = -1;  // index of first vertex found on poly 0
   int last0 = -1;   // index of last vertex found on poly 0
   int last1 = -1;   // index of last vertex found on poly 0
   bool found = false;

   for ( i=0; i < vertexCount0; i++ )
      {
      // get ith vertex on first poly
      const Vertex &v0 = pPoly0->GetVertex( i );

      // see if jth vertex on second poly is a match
      for ( j=0; j < vertexCount1; j++ );
         {
         const Vertex &v1 = pPoly1->GetVertex( j );

         if ( v0.x == v1.x && v0.y == v1.y ) // same coord?  (Note that == operator is overloaded to look at X, Y, Z
            {
            first0 = i;
            first1 = j;
            found = true;
            break;
            }
         }  // end of: for ( j < vertexCount1 )       
      }  // end of: for ( i < vertexCount0;

   if ( ! found )
      return -1;

   // found first, keep looking for last
   //  Note:  vertices are ALWAY in clockwise order;  therefore, moving through increasing
   //  common vertex indices in poly0 will be moving through decreasing indices in poly1
   last0 = i;
   last1 = j;
   while ( found )// keep looking (found==true means we are still finding common vertices)
      {
      last0++;  // move to next vertex on poly0
      last1--;
      ASSERT( last0 < vertexCount0 );
      ASSERT( last1 >= 0 );
      const Vertex &v0 = pPoly0->GetVertex( last0 );
      const Vertex &v1 = pPoly1->GetVertex( last1 );

      if ( v0.x != v1.x || v0.y != v1.y ) // different coord?
         {
         last0--;    // back up
         last1++;
         found = false;
         break;
         }
      }

   ASSERT( found == false );

   // okay, we have the start and ending vertex.  combine into one

   // MORE NEEDED HERE!!!
   return 0;
   }


//==========================================================================

PourPointDlg::PourPointDlg(CWnd* pParent /*=NULL*/)
	: CDialog(PourPointDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(PourPointDlg)
	m_col = 0;
	m_row = 0;
	//}}AFX_DATA_INIT
}


#if !defined IDC_COL
#define IDC_COL 0
#endif

#if !defined IDC_ROW
#define IDC_ROW 0
#endif

void PourPointDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PourPointDlg)
	DDX_Text(pDX, IDC_COL, m_col);
	DDX_Text(pDX, IDC_ROW, m_row);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(PourPointDlg, CDialog)
	//{{AFX_MSG_MAP(PourPointDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PourPointDlg message handlers

BOOL PourPointDlg::OnInitDialog() 
   {
	CDialog::OnInitDialog();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
   }
