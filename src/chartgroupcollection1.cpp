// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "chartgroupcollection1.h"

// Dispatch interfaces referenced by this interface
#include "chartgroup1.h"


/////////////////////////////////////////////////////////////////////////////
// CChartGroupCollection1 properties

LPDISPATCH CChartGroupCollection1::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x1, VT_DISPATCH, (void*)&result);
	return result;
}

long CChartGroupCollection1::GetCount()
{
	long result;
	GetProperty(0x2, VT_I4, (void*)&result);
	return result;
}

/////////////////////////////////////////////////////////////////////////////
// CChartGroupCollection1 operations

CChartGroup1 CChartGroupCollection1::GetItem(const VARIANT& Index)
{
	LPDISPATCH pDispatch;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, parms,
		&Index);
	return CChartGroup1(pDispatch);
}