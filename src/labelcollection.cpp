// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "labelcollection.h"

// Dispatch interfaces referenced by this interface
#include "label.h"


/////////////////////////////////////////////////////////////////////////////
// CLabelCollection properties

long CLabelCollection::GetCount()
{
	long result;
	GetProperty(0x1, VT_I4, (void*)&result);
	return result;
}

LPDISPATCH CLabelCollection::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x2, VT_DISPATCH, (void*)&result);
	return result;
}

/////////////////////////////////////////////////////////////////////////////
// CLabelCollection operations

CLabel CLabelCollection::GetItem(const VARIANT& Index)
{
	LPDISPATCH pDispatch;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, parms,
		&Index);
	return CLabel(pDispatch);
}

void CLabelCollection::Remove(const VARIANT& Index)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x3, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Index);
}

CLabel CLabelCollection::Add(LPCTSTR Text, const VARIANT& Before)
{
	LPDISPATCH pDispatch;
	static BYTE parms[] =
		VTS_BSTR VTS_VARIANT;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_DISPATCH, (void*)&pDispatch, parms,
		Text, &Before);
	return CLabel(pDispatch);
}

void CLabelCollection::RemoveAll()
{
	InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}