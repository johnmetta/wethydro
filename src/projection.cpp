// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "projection.h"


/////////////////////////////////////////////////////////////////////////////
// CProjection properties

LPDISPATCH CProjection::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x1, VT_DISPATCH, (void*)&result);
	return result;
}

BOOL CProjection::GetIsContoured()
{
	BOOL result;
	GetProperty(0x2, VT_BOOL, (void*)&result);
	return result;
}

void CProjection::SetIsContoured(BOOL propVal)
{
	SetProperty(0x2, VT_BOOL, propVal);
}

BOOL CProjection::GetIsZoned()
{
	BOOL result;
	GetProperty(0x3, VT_BOOL, (void*)&result);
	return result;
}

void CProjection::SetIsZoned(BOOL propVal)
{
	SetProperty(0x3, VT_BOOL, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// CProjection operations