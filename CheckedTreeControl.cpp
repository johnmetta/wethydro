// CheckedTreeControl.cpp : implementation file
//

#include "stdafx.h"
#include "wet_h.h"
#include "CheckedTreeControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCheckedTreeCtrl

CCheckedTreeCtrl::CCheckedTreeCtrl()
{
}

CCheckedTreeCtrl::~CCheckedTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CCheckedTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CCheckedTreeCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCheckedTreeCtrl message handlers
void CCheckedTreeCtrl::SetBitmaps( UINT NormalId, UINT StatusId)
	{
	// create Imagelists
	// 16x16 Pixel, white colour is transparent
//	m_NormalImageList.Create(IDB_BITMAP_NORMAL, 16, 2, RGB (255,255,255));
	m_StatusImageList.Create(IDB_CHECK, 16, 2, RGB (255,255,255));

	// and set the imagelists
//	SetImageList(&m_NormalImageList, TVSIL_NORMAL );
	SetImageList(&m_StatusImageList, TVSIL_STATE );
	}


void CCheckedTreeCtrl::ChangeItemState()
	{
	UINT OldState;
	HTREEITEM AItem;
	AItem = GetSelectedItem();

	if (AItem!=NULL)
		{
		// ... calculate the new state, use a bitmask
		OldState = GetItemState(AItem, TVIS_STATEIMAGEMASK);
		if ((OldState >> 12) & 1)
			SetItemState(AItem,INDEXTOSTATEIMAGEMASK (2), TVIS_STATEIMAGEMASK);
		else
			SetItemState(AItem,INDEXTOSTATEIMAGEMASK (1), TVIS_STATEIMAGEMASK);
		}
	}
