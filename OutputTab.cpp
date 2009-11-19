// OutputTab.cpp : implementation file
//

#include "stdafx.h"
//#include "wet_hydro.h"
#include "OutputTab.h"

extern MapWnd *gpMapWnd;
extern ResultPlotWnd *gpResultPlotWnd;
extern ResultsWnd *gpResultsWnd;
extern MessagesWnd *gpMessagesWnd;

#include <afxcoll.h>   // for the CStringArray class

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// OutputTab

OutputTab::OutputTab()
   : m_font(), 
     m_mapTab(),
     m_resultsPlotTab(),
     m_resultsTab(),
	  m_messagesTab()	

   {
   //m_inputTab.Construct( "Inputs", this, 0 );
   m_font.CreatePointFont( 100, "Arial" );
   }

OutputTab::~OutputTab()
{
}


BEGIN_MESSAGE_MAP(OutputTab, CTabCtrl)
	//{{AFX_MSG_MAP(OutputTab)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// OutputTab message handlers

int OutputTab::OnCreate(LPCREATESTRUCT lpCreateStruct) 
   {
	if (CTabCtrl::OnCreate(lpCreateStruct) == -1)
   	return -1;

   SetFont( &m_font );

	CStringArray labels;

   labels.Add( CString( "Maps"          ) );
   labels.Add( CString( "Result Plots and Monte Carlo Results"         ) );
	labels.Add( CString( "Result Data"       ) );
	labels.Add( CString( "Messages"      ) );
	
	// create each tab...
	TC_ITEM item;
	item.mask = TCIF_TEXT;

	for ( int i=0; i < labels.GetSize(); i++ )
      {
      item.pszText = (LPSTR)(LPCSTR) labels[ i ];
      item.cchTextMax = labels[ i ].GetLength();
      int retVal = InsertItem( i, &item );
      ASSERT( retVal >= 0 );
      }

   //-- add individual tab windows --//
   RECT rect;
   GetClientRect( &rect );

   rect.top = 0;
   rect.bottom = 750;
   rect.right = 750;


   BOOL ok = m_mapTab.Create( NULL, "MapTab",   WS_CHILD | WS_BORDER | WS_VISIBLE , rect, this, 101 );
   ASSERT ( ok );
   DWORD result4 = SetClassLong( m_mapTab.m_hWnd, GCL_HBRBACKGROUND, (long) GetStockObject( WHITE_BRUSH ) ); 

   ok = m_resultsPlotTab.Create( NULL, "ResultPlotTab", WS_CHILD | WS_BORDER, rect, this, 102 );
   ASSERT( ok );

   DWORD result = SetClassLong( m_resultsPlotTab.m_hWnd, GCL_HBRBACKGROUND, (long) GetStockObject( WHITE_BRUSH ) ); 
   
   ok = m_resultsTab.Create( NULL, "ResultDataTab", WS_CHILD | WS_BORDER , rect, this, 104 );
   ASSERT( ok );

   DWORD result2 = SetClassLong( m_resultsTab.m_hWnd, GCL_HBRBACKGROUND, (long) GetStockObject( WHITE_BRUSH ) ); 

   ok =  m_messagesTab.Create( NULL, "Model Summary", WS_CHILD |  WS_CLIPCHILDREN | WS_BORDER, rect, this, 100 , NULL);
   ASSERT( ok );
   DWORD result3 = SetClassLong( m_messagesTab.m_hWnd, GCL_HBRBACKGROUND, (long) GetStockObject( WHITE_BRUSH ) ); 
   m_messagesTab.Navigate( "file://j:/wet_hydro/wet_h/template.htm" );

   gpMapWnd = &m_mapTab;
	gpResultPlotWnd = &m_resultsPlotTab;
	gpResultsWnd = &m_resultsTab;
	gpMessagesWnd = &m_messagesTab;
   return 0;
   }


void OutputTab::OnSize(UINT nType, int cx, int cy) 
   {
	CTabCtrl::OnSize(nType, cx, cy);

   //-- size all the children --//
   RECT itemRect;
   GetItemRect( 0, &itemRect );
   int x = 3;
   int y = 3+itemRect.bottom;
   cx -= 6;
   cy -= (itemRect.bottom + 6);

   //- have tab, now resize children -//

   m_mapTab.MoveWindow( x, y, cx, cy, TRUE );
   m_resultsPlotTab.MoveWindow( x, y, cx, cy, TRUE );
   m_resultsTab.MoveWindow( x, y, cx, cy, TRUE );
   m_messagesTab.MoveWindow( x, y, cx, cy, TRUE );

   }
