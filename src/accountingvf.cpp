// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "accountingvf.h"

// Dispatch interfaces referenced by this interface
#include "DerivedEnum.h"
#include "DerivedBoolean.h"


/////////////////////////////////////////////////////////////////////////////
// CAccountingVF properties

long CAccountingVF::GetDecimalPlaces()
{
	long result;
	GetProperty(0x1, VT_I4, (void*)&result);
	return result;
}

void CAccountingVF::SetDecimalPlaces(long propVal)
{
	SetProperty(0x1, VT_I4, propVal);
}

BOOL CAccountingVF::GetUseCurrencySymbol()
{
	BOOL result;
	GetProperty(0x2, VT_BOOL, (void*)&result);
	return result;
}

void CAccountingVF::SetUseCurrencySymbol(BOOL propVal)
{
	SetProperty(0x2, VT_BOOL, propVal);
}

BOOL CAccountingVF::GetUseSmallCents()
{
	BOOL result;
	GetProperty(0x3, VT_BOOL, (void*)&result);
	return result;
}

void CAccountingVF::SetUseSmallCents(BOOL propVal)
{
	SetProperty(0x3, VT_BOOL, propVal);
}

CDerivedEnum CAccountingVF::GetOrderCurrencySignValue()
{
	LPDISPATCH pDispatch;
	GetProperty(0x4, VT_DISPATCH, (void*)&pDispatch);
	return CDerivedEnum(pDispatch);
}

void CAccountingVF::SetOrderCurrencySignValue(LPDISPATCH propVal)
{
	SetProperty(0x4, VT_DISPATCH, propVal);
}

CDerivedBoolean CAccountingVF::GetSpaceSeparatingSign()
{
	LPDISPATCH pDispatch;
	GetProperty(0x5, VT_DISPATCH, (void*)&pDispatch);
	return CDerivedBoolean(pDispatch);
}

void CAccountingVF::SetSpaceSeparatingSign(LPDISPATCH propVal)
{
	SetProperty(0x5, VT_DISPATCH, propVal);
}

CDerivedBoolean CAccountingVF::GetSpaceSeparatingCurrency()
{
	LPDISPATCH pDispatch;
	GetProperty(0x6, VT_DISPATCH, (void*)&pDispatch);
	return CDerivedBoolean(pDispatch);
}

void CAccountingVF::SetSpaceSeparatingCurrency(LPDISPATCH propVal)
{
	SetProperty(0x6, VT_DISPATCH, propVal);
}

unsigned long CAccountingVF::GetNegativeValueColor()
{
	unsigned long result;
	GetProperty(0x7, VT_I4, (void*)&result);
	return result;
}

void CAccountingVF::SetNegativeValueColor(unsigned long propVal)
{
	SetProperty(0x7, VT_I4, propVal);
}

CDerivedBoolean CAccountingVF::GetUseBrackets()
{
	LPDISPATCH pDispatch;
	GetProperty(0x8, VT_DISPATCH, (void*)&pDispatch);
	return CDerivedBoolean(pDispatch);
}

void CAccountingVF::SetUseBrackets(LPDISPATCH propVal)
{
	SetProperty(0x8, VT_DISPATCH, propVal);
}

LPDISPATCH CAccountingVF::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x9, VT_DISPATCH, (void*)&result);
	return result;
}

/////////////////////////////////////////////////////////////////////////////
// CAccountingVF operations
