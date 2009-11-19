// ResultsWnd.cpp : implementation file
//

#include "stdafx.h"

#include <afxcview.h>
#include "ResultsWnd.h"
#include "MapWnd.h"
#include "wet_hview.h"
//#include <Map.h>
//#include <maplayer.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//extern MapWnd     *gpMapWnd;
//extern MapLayer   *gpCellLayer;
extern ResultsWnd *gpResultsWnd;
extern CWet_hView *gpView;






/////////////////////////////////////////////////////////////////////////////
// ResultsWnd

ResultsWnd::ResultsWnd()

   {
   gpResultsWnd = this;
   }

ResultsWnd::~ResultsWnd()
   {
   RemoveAll();
   }

void ResultsWnd::RemoveAll()
   {
 //  for ( int i=0; i < m_mapWndArray.GetSize(); i++ )
 //     delete m_mapWndArray[ i ];

 //  m_mapWndArray.RemoveAll();
   }



BEGIN_MESSAGE_MAP(ResultsWnd, CWnd)
	//{{AFX_MSG_MAP(ResultsWnd)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// ResultsWnd message handlers



BOOL ResultsWnd::OnEraseBkgnd(CDC* pDC) 
   {
   RECT rect;
   GetClientRect( &rect );

   pDC->FillSolidRect( &rect, RGB( 0xFF, 0xFF, 0xFF ) );
   return TRUE; // 	return CWnd::OnEraseBkgnd(pDC);
   }



int ResultsWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
	{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
   CCreateContext context;
   context.m_pNewViewClass = RUNTIME_CLASS( CWet_hView );
   context.m_pCurrentDoc = NULL; //GetDocument();
   context.m_pNewDocTemplate = NULL;
   context.m_pLastView = NULL;

   // create the background splitter window
   m_splitWnd.CreateStatic( this, 1, 2, WS_CHILD | WS_VISIBLE, 101 );

	CRect rect;
	GetClientRect(&rect);
	CSize sz(120, rect.Height() );

   // create the left and right panes
   int retVal = m_splitWnd.CreateView( 0, 0, RUNTIME_CLASS( CListView ), sz, &context );

   sz.cx = rect.Width() - 120;
   retVal = m_splitWnd.CreateView( 0, 1, RUNTIME_CLASS( CWnd ), sz, &context );

   // store pointers to the split windows
   m_pDataListTree = (CListView*) m_splitWnd.GetPane( 0, 0 );
	
   m_pDataViewWnd = (CWnd*) m_splitWnd.GetPane( 0, 1 );

	SetTreeData();

	return 0;
	}



void ResultsWnd::SetTreeData()
	{

	CRect rect;
	GetClientRect(&rect);
	CSize sz(rect.Width()*2, rect.Height() );

	CListCtrl& ctlList = m_pDataListTree->GetListCtrl();

	DWORD dwStyle = GetWindowLong(ctlList.GetSafeHwnd(),GWL_STYLE);
	dwStyle |= LVS_LIST  ;	
	SetWindowLong(ctlList.GetSafeHwnd(), GWL_STYLE, dwStyle);
	ctlList.SetExtendedStyle(LVS_EX_CHECKBOXES);

	ctlList.InsertItem(1,"Cell Data");

}

void ResultsWnd::OnSize(UINT nType, int cx, int cy) 
   {
	CWnd::OnSize(nType, cx, cy);

   if ( IsWindow( m_splitWnd.m_hWnd ) )
      {
      m_splitWnd.MoveWindow( -2, -2, cx+2, cy+2, TRUE );
      }	
	}

void ResultsWnd::OnLButtonDblClk(UINT nFlags, CPoint point) 
	{


          

	
	CWnd::OnLButtonDblClk(nFlags, point);
}

BOOL ResultsWnd::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult) 
{
   switch( LOWORD( wParam ) )
      {
      case LVN_ITEMCHANGED :  
			{
			
			}
         break;
		}

	
	return CWnd::OnChildNotify(message, wParam, lParam, pLResult);
}
