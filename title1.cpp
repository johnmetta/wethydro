// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "title1.h"

// Dispatch interfaces referenced by this interface
#include "border1.h"
#include "Interior1.h"
#include "location1.h"
#include "font.h"
#include "label1.h"


/////////////////////////////////////////////////////////////////////////////
// CTitle1 properties

long CTitle1::GetAdjust()
{
	long result;
	GetProperty(0x1, VT_I4, (void*)&result);
	return result;
}

void CTitle1::SetAdjust(long propVal)
{
	SetProperty(0x1, VT_I4, propVal);
}

BOOL CTitle1::GetIsShowing()
{
	BOOL result;
	GetProperty(0x2, VT_BOOL, (void*)&result);
	return result;
}

void CTitle1::SetIsShowing(BOOL propVal)
{
	SetProperty(0x2, VT_BOOL, propVal);
}

LPDISPATCH CTitle1::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x3, VT_DISPATCH, (void*)&result);
	return result;
}

CBorder1 CTitle1::GetBorder()
{
	LPDISPATCH pDispatch;
	GetProperty(0x4, VT_DISPATCH, (void*)&pDispatch);
	return CBorder1(pDispatch);
}

CInterior1 CTitle1::GetInterior()
{
	LPDISPATCH pDispatch;
	GetProperty(0x5, VT_DISPATCH, (void*)&pDispatch);
	return CInterior1(pDispatch);
}

CLocation1 CTitle1::GetLocation()
{
	LPDISPATCH pDispatch;
	GetProperty(0x6, VT_DISPATCH, (void*)&pDispatch);
	return CLocation1(pDispatch);
}

COleFont CTitle1::GetFont()
{
	LPDISPATCH pDispatch;
	GetProperty(0x7, VT_DISPATCH, (void*)&pDispatch);
	return COleFont(pDispatch);
}

void CTitle1::SetFont(LPDISPATCH propVal)
{
	SetProperty(0x7, VT_DISPATCH, propVal);
}

CLabel1 CTitle1::GetText()
{
	LPDISPATCH pDispatch;
	GetProperty(0x8, VT_DISPATCH, (void*)&pDispatch);
	return CLabel1(pDispatch);
}

/////////////////////////////////////////////////////////////////////////////
// CTitle1 operations