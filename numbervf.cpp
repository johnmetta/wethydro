// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "numbervf.h"

// Dispatch interfaces referenced by this interface
#include "derivedenum.h"
#include "derivedboolean.h"


/////////////////////////////////////////////////////////////////////////////
// CNumberVF properties

long CNumberVF::GetDecimalPlaces()
{
	long result;
	GetProperty(0x1, VT_I4, (void*)&result);
	return result;
}

void CNumberVF::SetDecimalPlaces(long propVal)
{
	SetProperty(0x1, VT_I4, propVal);
}

BOOL CNumberVF::GetUseSeparators()
{
	BOOL result;
	GetProperty(0x2, VT_BOOL, (void*)&result);
	return result;
}

void CNumberVF::SetUseSeparators(BOOL propVal)
{
	SetProperty(0x2, VT_BOOL, propVal);
}

CDerivedEnum CNumberVF::GetOrderSignValue()
{
	LPDISPATCH pDispatch;
	GetProperty(0x3, VT_DISPATCH, (void*)&pDispatch);
	return CDerivedEnum(pDispatch);
}

void CNumberVF::SetOrderSignValue(LPDISPATCH propVal)
{
	SetProperty(0x3, VT_DISPATCH, propVal);
}

CDerivedBoolean CNumberVF::GetSpaceSeparatingSign()
{
	LPDISPATCH pDispatch;
	GetProperty(0x4, VT_DISPATCH, (void*)&pDispatch);
	return CDerivedBoolean(pDispatch);
}

void CNumberVF::SetSpaceSeparatingSign(LPDISPATCH propVal)
{
	SetProperty(0x4, VT_DISPATCH, propVal);
}

unsigned long CNumberVF::GetNegativeValueColor()
{
	unsigned long result;
	GetProperty(0x5, VT_I4, (void*)&result);
	return result;
}

void CNumberVF::SetNegativeValueColor(unsigned long propVal)
{
	SetProperty(0x5, VT_I4, propVal);
}

CDerivedBoolean CNumberVF::GetUseBrackets()
{
	LPDISPATCH pDispatch;
	GetProperty(0x6, VT_DISPATCH, (void*)&pDispatch);
	return CDerivedBoolean(pDispatch);
}

void CNumberVF::SetUseBrackets(LPDISPATCH propVal)
{
	SetProperty(0x6, VT_DISPATCH, propVal);
}

LPDISPATCH CNumberVF::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x7, VT_DISPATCH, (void*)&result);
	return result;
}

/////////////////////////////////////////////////////////////////////////////
// CNumberVF operations
