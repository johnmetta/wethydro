// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "ceiling.h"

// Dispatch interfaces referenced by this interface
#include "projection.h"


/////////////////////////////////////////////////////////////////////////////
// CCeiling properties

LPDISPATCH CCeiling::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x1, VT_DISPATCH, (void*)&result);
	return result;
}

CProjection CCeiling::GetProjection()
{
	LPDISPATCH pDispatch;
	GetProperty(0x2, VT_DISPATCH, (void*)&pDispatch);
	return CProjection(pDispatch);
}

/////////////////////////////////////////////////////////////////////////////
// CCeiling operations
