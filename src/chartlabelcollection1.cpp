// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "chartlabelcollection1.h"

// Dispatch interfaces referenced by this interface
#include "chartlabel1.h"


/////////////////////////////////////////////////////////////////////////////
// CChartLabelCollection1 properties

LPDISPATCH CChartLabelCollection1::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x1, VT_DISPATCH, (void*)&result);
	return result;
}

long CChartLabelCollection1::GetCount()
{
	long result;
	GetProperty(0x2, VT_I4, (void*)&result);
	return result;
}

/////////////////////////////////////////////////////////////////////////////
// CChartLabelCollection1 operations

CChartLabel1 CChartLabelCollection1::Add()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0x3, DISPATCH_METHOD, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CChartLabel1(pDispatch);
}

void CChartLabelCollection1::Remove(const VARIANT& Index)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Index);
}

CChartLabel1 CChartLabelCollection1::GetItem(const VARIANT& Index)
{
	LPDISPATCH pDispatch;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, parms,
		&Index);
	return CChartLabel1(pDispatch);
}

void CChartLabelCollection1::RemoveAll()
{
	InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}
