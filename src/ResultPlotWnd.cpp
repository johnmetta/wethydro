// ResultPlotWnd.cpp : implementation file
//

#include "stdafx.h"

#include "ResultPlotWnd.h"
#include "MapWnd.h"
#include "wet_hydro.h"
#include "wet_hView.h"
#include "TreeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <afxcview.h>

extern WET_Hydro  *gpHydroModel;
extern ResultPlotWnd *gpResultPlotWnd;
extern CWet_hView *gpView;


/////////////////////////////////////////////////////////////////////////////
// ResultPlotWnd

ResultPlotWnd::ResultPlotWnd()
   {
   gpResultPlotWnd = this;
   }

ResultPlotWnd::~ResultPlotWnd()
   {
   RemoveAll();
   }

void ResultPlotWnd::RemoveAll()
   {
   for ( int i=0; i < m_resultInfoArray.GetSize(); i++ )
      delete m_resultInfoArray[ i ];

   m_resultInfoArray.RemoveAll();
   }

BEGIN_MESSAGE_MAP(ResultPlotWnd, CWnd)
	//{{AFX_MSG_MAP(ResultPlotWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// ResultsWnd message handlers

int ResultPlotWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
   {
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
		
   CCreateContext context;
   context.m_pNewViewClass = RUNTIME_CLASS( CWet_hView );
   context.m_pCurrentDoc = NULL; //GetDocument();
   context.m_pNewDocTemplate = NULL;
   context.m_pLastView = NULL;

   // create the background splitter window
   m_splitterWnd.CreateStatic( this, 1, 2, WS_CHILD | WS_VISIBLE, 101 );

	CRect rect;
	GetClientRect(&rect);
	CSize sz(0, rect.Height() );

   // create the left and right panes
   int retVal = m_splitterWnd.CreateView( 0, 0, RUNTIME_CLASS( TreeView ), sz, &context );

   sz.cx = rect.Width() + 500;
   retVal = m_splitterWnd.CreateView( 0, 1, RUNTIME_CLASS( CWnd ), sz, &context );

   // store pointers to the split windows
   m_pResultTree = (TreeView*) m_splitterWnd.GetPane( 0, 0 );
	
   m_pPlotWnd = (CWnd*) m_splitterWnd.GetPane( 0, 1 );
  
	return 0;
   }

BOOL ResultPlotWnd::OnEraseBkgnd(CDC* pDC) 
   {
   RECT rect;
   GetClientRect( &rect );

   pDC->FillSolidRect( &rect, RGB( 0xFF, 0xFF, 0xFF ) );
   return TRUE; // 	return CWnd::OnEraseBkgnd(pDC);
   }

ScatterWnd *ResultPlotWnd::AllocateScatter( DataObj *pScatterData, LPCTSTR title)
	{
   ScatterWnd *pScatterPlot = new ScatterWnd;
   RECT rect;
   GetClientRect( &rect );

	RECT childRect	;
   int width = rect.right/3;
   int height = rect.bottom/5;

   childRect.left = 10;
   childRect.right = childRect.left + width;

   childRect.bottom = height + 4;
   childRect.top = 4;



   pScatterPlot->Create( NULL,title,  WS_CHILD | WS_CLIPSIBLINGS | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME, childRect, m_pPlotWnd, 100 + m_resultInfoArray.GetSize() - 1 );
	pScatterPlot->SetLabelText(XAXIS, "Time");
	pScatterPlot->SetXCol( pScatterData, 0 );

	UINT count = pScatterData->GetColCount();

	if (pScatterData->GetColCount()>1)
		{
		for ( int i=1; i < (int) pScatterData->GetColCount(); i++ )
			pScatterPlot->AppendYCol( pScatterData, i );
		pScatterPlot->SetAxisDigits(YAXIS, 3);
		pScatterPlot->ShowWindow( SW_SHOW );
		}
   m_ResultPlotWndArray.Add( pScatterPlot );

   return pScatterPlot;
	}

void ResultPlotWnd::OnSize(UINT nType, int cx, int cy) 
   {
	CWnd::OnSize(nType, cx, cy);

   if ( IsWindow( m_splitterWnd.m_hWnd ) )
      {
      m_splitterWnd.MoveWindow( -2, -2, cx+2, cy+2, TRUE );
      }	
	}

	
	
