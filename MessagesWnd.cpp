// MessagesWnd.cpp : implementation file
//

#include "stdafx.h"
#include "wet_h.h"
#include "MessagesWnd.h"
#include "wet_hview.h"
#include "wet_hydro.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CWet_hView *gpView;
extern MessagesWnd *gpMessagesWnd;
extern WET_Hydro *gpHydroModel;



   




/////////////////////////////////////////////////////////////////////////////
// MessagesWnd

MessagesWnd::MessagesWnd()
{
	   gpMessagesWnd = this;
}

MessagesWnd::~MessagesWnd()
{
}


BEGIN_MESSAGE_MAP(MessagesWnd, CWnd)
	//{{AFX_MSG_MAP(MessagesWnd)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// MessagesWnd message handlers


int MessagesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

   CCreateContext context;
   context.m_pNewViewClass = RUNTIME_CLASS( CWet_hView );
   context.m_pCurrentDoc = NULL; //GetDocument();
   context.m_pNewDocTemplate = NULL;
   context.m_pLastView = NULL;

   // create the background splitter window
//   m_editWnd.CreateStatic( this, 1, 2, WS_CHILD | WS_VISIBLE, 101 );

	CRect rect;
	GetClientRect(&rect);
	CSize sz(rect.Width()*2, rect.Height() );

//   m_wnd->createst.CreateStatic( this, 1, 2, WS_CHILD | WS_VISIBLE, 101 );

//   int retVal = m_editWnd->Create(WS_VISIBLE,rect,this,105 );


   sz.cx *= 5;
//	SetReadOnly();

	CString msg = "Welcome to WET_Hydro.  No model has been developed during this session\n";
	CString ms2 = "yet";
	SetWindowText(msg);


	msg += ms2;

	SetWindowText(msg);
	return 0;
}

BOOL MessagesWnd::OnEraseBkgnd(CDC* pDC) 
{
   RECT rect;
   GetClientRect( &rect );

   pDC->FillSolidRect( &rect, RGB( 0xFF, 0xFF, 0xFF ) );	
	return CWnd::OnEraseBkgnd(pDC);
}

void MessagesWnd::SummarizeModel()
	{
	CString msg ;
	msg.Format(" %i m3 of water left the system as discharge.", gpHydroModel->m_runNumber);

	SetWindowText(msg);
	
	
	}
