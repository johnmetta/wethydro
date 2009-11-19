// LulcTree.cpp: implementation of the LulcTreec class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "restore.h"
#include "LulcTree.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

LulcNode *LulcTree::FindNode( int level, int id )
   {
   LulcNode *pNode = GetRootNode();

   while ( pNode != NULL )
      {
      if ( pNode->GetNodeLevel() == level && pNode->m_id == id )
         return pNode;

      pNode = GetNextNode();
      }

   return NULL;
   }


void LulcTree::RemoveAll( void )
   {
   if ( m_pRootNode != NULL )
      RemoveBranch( m_pRootNode );
   }


void LulcTree::RemoveBranch( LulcNode *pNode )
   {
   for ( int i=0; i < pNode->m_childNodeArray.GetSize(); i++ )
      RemoveBranch( pNode->m_childNodeArray[ i ] );   // branches of the tree

   //delete pNode->pCost;
   delete pNode;
   }


LulcNode *LulcTree::GetNextSibling( LulcNode *pNode )
   {
   LulcNode *pParent = pNode->m_pParentNode;

   if ( pParent == NULL )
      return NULL;  // no parent, no siblings

   for ( int i=0; i < pParent->m_childNodeArray.GetSize(); i++ )
      {
      if ( pNode == pParent->m_childNodeArray[ i ] )     // found self
         {
         if ( i == pParent->m_childNodeArray.GetSize()-1 )  // last root node?
            return NULL;
         else
            return pParent->m_childNodeArray[ i+1 ];   // return next root node
         }
      }  // end of:  for ( i < m_rootNodeArray )

   ASSERT( 0 );
   return NULL;
   }



LulcNode *LulcTree::GetNextNode( LulcNode *pNode )
   {
   // if a child node exists, use first one
   if ( pNode->m_childNodeArray.GetSize() > 0 )
      return pNode->m_childNodeArray[ 0 ];

   // no children, are there siblings?
   LulcNode *pSibling = GetNextSibling( pNode );

   if ( pSibling != NULL )    // if a next sibling exists, return it;
      return pSibling;

   // no sibling exists, see is a parent sibling exists.
   if ( pNode->m_pParentNode == NULL )
      return NULL;

   pSibling = GetNextSibling( pNode->m_pParentNode );

   if ( pSibling != NULL )
      return pSibling;

   // no parent sibling exists, try grandparent sibling
   if ( pNode->m_pParentNode->m_pParentNode == NULL )
      return NULL;

   pSibling = GetNextSibling( pNode->m_pParentNode->m_pParentNode );

   return pSibling;
   }


LulcNode* LulcTree::GetNextNode( void )
   {
   return m_pCurrentNode = GetNextNode( m_pCurrentNode );
   }

