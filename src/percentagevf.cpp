// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "percentagevf.h"

// Dispatch interfaces referenced by this interface
#include "derivedenum.h"
#include "DerivedBoolean.h"


/////////////////////////////////////////////////////////////////////////////
// CPercentageVF properties

long CPercentageVF::GetDecimalPlaces()
{
	long result;
	GetProperty(0x1, VT_I4, (void*)&result);
	return result;
}

void CPercentageVF::SetDecimalPlaces(long propVal)
{
	SetProperty(0x1, VT_I4, propVal);
}

CDerivedEnum CPercentageVF::GetOrderPercentSignValue()
{
	LPDISPATCH pDispatch;
	GetProperty(0x2, VT_DISPATCH, (void*)&pDispatch);
	return CDerivedEnum(pDispatch);
}

void CPercentageVF::SetOrderPercentSignValue(LPDISPATCH propVal)
{
	SetProperty(0x2, VT_DISPATCH, propVal);
}

CDerivedBoolean CPercentageVF::GetSpaceSeparatingSign()
{
	LPDISPATCH pDispatch;
	GetProperty(0x3, VT_DISPATCH, (void*)&pDispatch);
	return CDerivedBoolean(pDispatch);
}

void CPercentageVF::SetSpaceSeparatingSign(LPDISPATCH propVal)
{
	SetProperty(0x3, VT_DISPATCH, propVal);
}

CDerivedBoolean CPercentageVF::GetSpaceSeparatingPercent()
{
	LPDISPATCH pDispatch;
	GetProperty(0x4, VT_DISPATCH, (void*)&pDispatch);
	return CDerivedBoolean(pDispatch);
}

void CPercentageVF::SetSpaceSeparatingPercent(LPDISPATCH propVal)
{
	SetProperty(0x4, VT_DISPATCH, propVal);
}

LPDISPATCH CPercentageVF::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x5, VT_DISPATCH, (void*)&result);
	return result;
}

/////////////////////////////////////////////////////////////////////////////
// CPercentageVF operations