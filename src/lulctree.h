// LulcTree.h: interface for the LulcTreec class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LULCTREE_H__CA0A6A7C_58E2_46A7_84D4_70C6957150CC__INCLUDED_)
#define AFX_LULCTREE_H__CA0A6A7C_58E2_46A7_84D4_70C6957150CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <afxtempl.h>

class LulcNode;
class LulcNodeArray : public CArray< LulcNode*, LulcNode* >
   { };


class LulcNode
   {
   public:
      // constructor
      LulcNode( void ) : m_pParentNode( NULL ), m_id( -1 ),  m_color( RGB( 0xFF, 0xFF, 0xFF ) ), m_pData( NULL )
            { m_name[ 0 ] = NULL; }

      // methods
      int GetNodeLevel( void )  // return 0 for root node, 1 for first level, 2 for second, etc...
         { if ( m_pParentNode == NULL ) return 0; else return m_pParentNode->GetNodeLevel() + 1; }

   public:
      int   m_id;          // id for this node
      char  m_name[ 64 ];  // associated land use name
      COLORREF m_color;    // for map legend

      void *m_pData; // pointer to application-specific data

      LulcNode     *m_pParentNode;     // pointer to parent
      LulcNodeArray m_childNodeArray;  // pointer to children
   };



class LulcTree
   {
   public:
      LulcTree( void ) : m_pRootNode( NULL ), m_pCurrentNode( NULL ) { }
     ~LulcTree( void ) { RemoveAll(); }

      void RemoveAll( void );
      void RemoveBranch( LulcNode *pRoot );

      LulcNode *GetRootNode( void ) { return m_pCurrentNode = m_pRootNode; }
      LulcNode *GetNextNode( void );   // for iterating through the tree.  returns NULL when done iterating 

      LulcNode *FindNode( int level, int id );

   public:
      LulcNode *m_pRootNode;         // roots of the tree

   protected:
      LulcNode *m_pCurrentNode;

      LulcNode *GetNextSibling( LulcNode* );
      LulcNode *GetNextNode   ( LulcNode* );
   };



#endif // !defined(AFX_LULCTREE_H__CA0A6A7C_58E2_46A7_84D4_70C6957150CC__INCLUDED_)
