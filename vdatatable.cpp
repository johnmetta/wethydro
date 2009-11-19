// VDataTable.cpp : implementation file
//

#include "stdafx.h"
//#include "restore.h"
#include "VDataTable.h"

#include <vdataobj.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// VDataTable

VDataTable::VDataTable( VDataObj *pData )
: m_pData( pData )
{
ASSERT( m_pData != NULL );
}

VDataTable::~VDataTable()
{
}


BEGIN_MESSAGE_MAP(VDataTable, CWnd)
	//{{AFX_MSG_MAP(VDataTable)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// VDataTable message handlers

void VDataTable::LoadTable()
   {
   m_grid.DeleteAllItems();
   int cols = m_pData->GetColCount();
   int rows = m_pData->GetRowCount();

   m_grid.SetColumnCount( cols );

   m_grid.SetFixedRowCount( 1 );
   m_grid.SetRowCount( rows );

   for( int col=0; col < cols; col++ )
      {
      LPCSTR label = m_pData->GetLabel( col );
      m_grid.SetItemText( 0, col, label );
      }

   for ( int row=0; row < rows; row++ )
      {
      for ( col=0; col < cols; col++ )
         {
         VData &v = m_pData->Get( col, row );
         m_grid.SetItemText( row+1, col, v.GetAsString() );
        }
      }
   }


int VDataTable::OnCreate(LPCREATESTRUCT lpCreateStruct) 
   {
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
   RECT rect;
   GetClientRect( &rect );
   
   BOOL ok = m_grid.Create( rect, this, 100 ); //, DWORD dwStyle = WS_CHILD | WS_BORDER | WS_TABSTOP | WS_VISIBLE);
   ASSERT( ok );
   
   // get cell properties
   ASSERT( m_pData != NULL );
   LoadTable();

	return 0;
   }
   
void VDataTable::OnSize(UINT nType, int cx, int cy) 
   {
	CWnd::OnSize(nType, cx, cy);

   if ( IsWindow( m_grid.m_hWnd ) )
      {
      m_grid.MoveWindow( 0, 0, cx, cy, TRUE );
      }
   }


BOOL VDataTable::Create( LPCTSTR lpszWindowName, const RECT& rect, CWnd* pParentWnd, UINT nID ) 
   {	
	return CWnd::Create(NULL, lpszWindowName, WS_CHILD | WS_OVERLAPPEDWINDOW | WS_VISIBLE, rect, pParentWnd, nID, NULL);
   }
