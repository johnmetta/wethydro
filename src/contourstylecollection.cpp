// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "contourstylecollection.h"

// Dispatch interfaces referenced by this interface
#include "ContourStyle.h"


/////////////////////////////////////////////////////////////////////////////
// CContourStyleCollection properties

LPDISPATCH CContourStyleCollection::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x1, VT_DISPATCH, (void*)&result);
	return result;
}

long CContourStyleCollection::GetCount()
{
	long result;
	GetProperty(0x2, VT_I4, (void*)&result);
	return result;
}

/////////////////////////////////////////////////////////////////////////////
// CContourStyleCollection operations

CContourStyle CContourStyleCollection::Add(const VARIANT& Before)
{
	LPDISPATCH pDispatch;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x3, DISPATCH_METHOD, VT_DISPATCH, (void*)&pDispatch, parms,
		&Before);
	return CContourStyle(pDispatch);
}

BOOL CContourStyleCollection::Remove(const VARIANT& Index)
{
	BOOL result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		&Index);
	return result;
}

long CContourStyleCollection::CalcIndex(const VARIANT& LevelIndex)
{
	long result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x5, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		&LevelIndex);
	return result;
}

void CContourStyleCollection::Reset()
{
	InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

BOOL CContourStyleCollection::Load(LPCTSTR FileName)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x7, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		FileName);
	return result;
}

BOOL CContourStyleCollection::Save(LPCTSTR FileName)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		FileName);
	return result;
}

CContourStyle CContourStyleCollection::GetItem(const VARIANT& Index)
{
	LPDISPATCH pDispatch;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x9, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, parms,
		&Index);
	return CContourStyle(pDispatch);
}

void CContourStyleCollection::RemoveAll()
{
	InvokeHelper(0xa, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}
