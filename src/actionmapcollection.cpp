// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "actionmapcollection.h"

// Dispatch interfaces referenced by this interface
#include "actionmap.h"


/////////////////////////////////////////////////////////////////////////////
// CActionMapCollection properties

long CActionMapCollection::GetCount()
{
	long result;
	GetProperty(0x1, VT_I4, (void*)&result);
	return result;
}

LPDISPATCH CActionMapCollection::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x2, VT_DISPATCH, (void*)&result);
	return result;
}

/////////////////////////////////////////////////////////////////////////////
// CActionMapCollection operations

CActionMap CActionMapCollection::Add(short Message, short Modifier, short KeyCode, long Action)
{
	LPDISPATCH pDispatch;
	static BYTE parms[] =
		VTS_I2 VTS_I2 VTS_I2 VTS_I4;
	InvokeHelper(0x3, DISPATCH_METHOD, VT_DISPATCH, (void*)&pDispatch, parms,
		Message, Modifier, KeyCode, Action);
	return CActionMap(pDispatch);
}

void CActionMapCollection::Remove(short Message, short Modifier, short KeyCode)
{
	static BYTE parms[] =
		VTS_I2 VTS_I2 VTS_I2;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Message, Modifier, KeyCode);
}

CActionMap CActionMapCollection::GetItem(short Message, short Modifier, short KeyCode)
{
	LPDISPATCH pDispatch;
	static BYTE parms[] =
		VTS_I2 VTS_I2 VTS_I2;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, parms,
		Message, Modifier, KeyCode);
	return CActionMap(pDispatch);
}

void CActionMapCollection::RemoveAll()
{
	InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CActionMapCollection::Reset()
{
	InvokeHelper(0x7, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}
