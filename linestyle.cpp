// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "linestyle.h"


/////////////////////////////////////////////////////////////////////////////
// CLineStyle properties

long CLineStyle::GetPattern()
{
	long result;
	GetProperty(0x1, VT_I4, (void*)&result);
	return result;
}

void CLineStyle::SetPattern(long propVal)
{
	SetProperty(0x1, VT_I4, propVal);
}

unsigned long CLineStyle::GetColor()
{
	unsigned long result;
	GetProperty(0x2, VT_I4, (void*)&result);
	return result;
}

void CLineStyle::SetColor(unsigned long propVal)
{
	SetProperty(0x2, VT_I4, propVal);
}

long CLineStyle::GetWidth()
{
	long result;
	GetProperty(0x3, VT_I4, (void*)&result);
	return result;
}

void CLineStyle::SetWidth(long propVal)
{
	SetProperty(0x3, VT_I4, propVal);
}

LPDISPATCH CLineStyle::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x4, VT_DISPATCH, (void*)&result);
	return result;
}

/////////////////////////////////////////////////////////////////////////////
// CLineStyle operations