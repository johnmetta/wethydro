// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "hiloopenclose.h"


/////////////////////////////////////////////////////////////////////////////
// CHiLoOpenClose properties

BOOL CHiLoOpenClose::GetIsShowingOpen()
{
	BOOL result;
	GetProperty(0x1, VT_BOOL, (void*)&result);
	return result;
}

void CHiLoOpenClose::SetIsShowingOpen(BOOL propVal)
{
	SetProperty(0x1, VT_BOOL, propVal);
}

BOOL CHiLoOpenClose::GetIsShowingClose()
{
	BOOL result;
	GetProperty(0x2, VT_BOOL, (void*)&result);
	return result;
}

void CHiLoOpenClose::SetIsShowingClose(BOOL propVal)
{
	SetProperty(0x2, VT_BOOL, propVal);
}

LPDISPATCH CHiLoOpenClose::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x3, VT_DISPATCH, (void*)&result);
	return result;
}

BOOL CHiLoOpenClose::GetIsOpenCloseFullWidth()
{
	BOOL result;
	GetProperty(0x4, VT_BOOL, (void*)&result);
	return result;
}

void CHiLoOpenClose::SetIsOpenCloseFullWidth(BOOL propVal)
{
	SetProperty(0x4, VT_BOOL, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// CHiLoOpenClose operations