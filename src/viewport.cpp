// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "viewport.h"


/////////////////////////////////////////////////////////////////////////////
// CViewport properties

LPDISPATCH CViewport::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x1, VT_DISPATCH, (void*)&result);
	return result;
}

BOOL CViewport::GetIsNormalized()
{
	BOOL result;
	GetProperty(0x2, VT_BOOL, (void*)&result);
	return result;
}

void CViewport::SetIsNormalized(BOOL propVal)
{
	SetProperty(0x2, VT_BOOL, propVal);
}

double CViewport::GetScaleFactor()
{
	double result;
	GetProperty(0x3, VT_R8, (void*)&result);
	return result;
}

void CViewport::SetScaleFactor(double propVal)
{
	SetProperty(0x3, VT_R8, propVal);
}

double CViewport::GetHorizontalShift()
{
	double result;
	GetProperty(0x4, VT_R8, (void*)&result);
	return result;
}

void CViewport::SetHorizontalShift(double propVal)
{
	SetProperty(0x4, VT_R8, propVal);
}

double CViewport::GetVerticalShift()
{
	double result;
	GetProperty(0x5, VT_R8, (void*)&result);
	return result;
}

void CViewport::SetVerticalShift(double propVal)
{
	SetProperty(0x5, VT_R8, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// CViewport operations
