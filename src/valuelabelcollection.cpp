// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "valuelabelcollection.h"

// Dispatch interfaces referenced by this interface
#include "valuelabel.h"


/////////////////////////////////////////////////////////////////////////////
// CValueLabelCollection properties

long CValueLabelCollection::GetCount()
{
	long result;
	GetProperty(0x1, VT_I4, (void*)&result);
	return result;
}

LPDISPATCH CValueLabelCollection::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x2, VT_DISPATCH, (void*)&result);
	return result;
}

/////////////////////////////////////////////////////////////////////////////
// CValueLabelCollection operations

CValueLabel CValueLabelCollection::GetItem(const VARIANT& Index)
{
	LPDISPATCH pDispatch;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, parms,
		&Index);
	return CValueLabel(pDispatch);
}

CValueLabel CValueLabelCollection::Add(double Value, LPCTSTR Text)
{
	LPDISPATCH pDispatch;
	static BYTE parms[] =
		VTS_R8 VTS_BSTR;
	InvokeHelper(0x3, DISPATCH_METHOD, VT_DISPATCH, (void*)&pDispatch, parms,
		Value, Text);
	return CValueLabel(pDispatch);
}

void CValueLabelCollection::Remove(const VARIANT& Index)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Index);
}

void CValueLabelCollection::RemoveAll()
{
	InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}
