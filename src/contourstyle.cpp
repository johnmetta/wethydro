// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "contourstyle.h"

// Dispatch interfaces referenced by this interface
#include "LineStyle1.h"
#include "fillstyle1.h"


/////////////////////////////////////////////////////////////////////////////
// CContourStyle properties

LPDISPATCH CContourStyle::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x1, VT_DISPATCH, (void*)&result);
	return result;
}

CLineStyle1 CContourStyle::GetLineStyle()
{
	LPDISPATCH pDispatch;
	GetProperty(0x2, VT_DISPATCH, (void*)&pDispatch);
	return CLineStyle1(pDispatch);
}

CFillStyle1 CContourStyle::GetFillStyle()
{
	LPDISPATCH pDispatch;
	GetProperty(0x3, VT_DISPATCH, (void*)&pDispatch);
	return CFillStyle1(pDispatch);
}

/////////////////////////////////////////////////////////////////////////////
// CContourStyle operations
