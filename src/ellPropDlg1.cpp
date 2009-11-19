// ellPropDlg1.cpp : implementation file
//
#include "stdafx.h"
#pragma hdrstop
#include "WET_Hydro.h"

#include "wet_h.h"
#include "ellPropDlg1.h"

#include <idataobj.h>

#include <map.h>
#include <maplayer.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern WET_Hydro *gpHydroModel;

/////////////////////////////////////////////////////////////////////////////
// CellPropDlg1 dialog


CellPropDlg1::CellPropDlg1( MapLayer *pLayer, int currentCell, CWnd* pParent /*=NULL*/)
	: CDialog(CellPropDlg1::IDD, pParent),
	  m_pLayer( pLayer ),
     m_currentCell( currentCell )
{
	//{{AFX_DATA_INIT(CellPropDlg1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CellPropDlg1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CellPropDlg1)
	DDX_Control(pDX, IDC_PREVCELL, m_prevcell);
	DDX_Control(pDX, IDC_NEXTCELL, m_nextcell);
	DDX_Control(pDX, IDC_FLASHCELL, m_flashcell);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CellPropDlg1, CDialog)
	//{{AFX_MSG_MAP(CellPropDlg1)
	ON_BN_CLICKED(IDC_FLASHCELL, OnFlashcell)
	ON_BN_CLICKED(IDC_NEXTCELL, OnNextcell)
	ON_BN_CLICKED(IDC_PREVCELL, OnPrevcell)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CellPropDlg1 message handlers
/////////////////////////////////////////////////////////////////////////////
// CellPropDlg message handlers

BOOL CellPropDlg1::OnInitDialog() 
   {
	CDialog::OnInitDialog();

   RECT rect;
   GetClientRect( &rect );

   RECT btnRect;
   m_flashcell.GetWindowRect( &btnRect );

   int btnHeight = btnRect.bottom - btnRect.top;

   rect.left   += 3;
   rect.top    += 3;
   rect.right  -= 3;
   rect.bottom -= ( 3 + btnHeight );

   BOOL ok = m_grid.Create( rect, this, 100 ); //, DWORD dwStyle = WS_CHILD | WS_BORDER | WS_TABSTOP | WS_VISIBLE);
   ASSERT( ok );

   
   // get cell properties
   ASSERT( m_pLayer != NULL );
   LoadCell();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
   }


void CellPropDlg1::LoadCell( void )
   {
   m_grid.DeleteAllItems();
   int fields = m_pLayer->GetFieldCount();

   m_grid.SetRowCount( fields );
   m_grid.SetColumnCount( 2 );

   for ( int row=0; row < fields; row++ )
      {
      m_grid.SetItemText( row, 0, m_pLayer->GetFieldLabel( row ) );
      CString value;
      m_pLayer->GetData( m_currentCell, row, value );
      m_grid.SetItemText( row, 1, value );
      }
   }


void CellPropDlg1::LoadStatistics( void )
   {
/*   m_grid.DeleteAllItems();
   int fields = gpHydroModel->m_pReachStatistics->GetRowCount();
	gpHydroModel->m_pReachStatistics

   m_grid.SetRowCount( fields );
   m_grid.SetColumnCount( 8 );

   for ( int row=0; row < fields; row++ )
      {
		for (int col=0; col < 8; col++ )
			{
			m_grid.SetItemText( row, col, gpHydroModel->m_pReachStatistics->GetLabel( row, col ) );
			CString value;
			m_pReachStatistics
			m_grid.SetItemText( row, 1, value );
			}
		}*/
   }	


void CellPropDlg1::OnFlashcell() 
{
   Map *pMap  = m_pLayer->m_pMap;
   int  index = pMap->GetLayerIndex( m_pLayer );
   pMap->FlashPoly( index, m_currentCell );  // 0th layer, m_cell cell
	
}

void CellPropDlg1::OnNextcell() 
{
   m_currentCell++;

   GetDlgItem( IDC_PREVCELL )->EnableWindow( TRUE );

   if ( m_currentCell == m_pLayer->GetRecordCount() - 1 )
      GetDlgItem( IDC_NEXTCELL )->EnableWindow( false );

   LoadCell();
	
}

void CellPropDlg1::OnPrevcell() 
{
   m_currentCell--;

   GetDlgItem( IDC_NEXTCELL )->EnableWindow( TRUE );

   if ( m_currentCell == 0 )
      GetDlgItem( IDC_PREVCELL )->EnableWindow( false );

   LoadCell();
	
}
