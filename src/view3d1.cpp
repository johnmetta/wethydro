// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "view3d1.h"


/////////////////////////////////////////////////////////////////////////////
// CView3D1 properties

LPDISPATCH CView3D1::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x1, VT_DISPATCH, (void*)&result);
	return result;
}

double CView3D1::GetPerspective()
{
	double result;
	GetProperty(0x2, VT_R8, (void*)&result);
	return result;
}

void CView3D1::SetPerspective(double propVal)
{
	SetProperty(0x2, VT_R8, propVal);
}

double CView3D1::GetXRotation()
{
	double result;
	GetProperty(0x3, VT_R8, (void*)&result);
	return result;
}

void CView3D1::SetXRotation(double propVal)
{
	SetProperty(0x3, VT_R8, propVal);
}

double CView3D1::GetYRotation()
{
	double result;
	GetProperty(0x4, VT_R8, (void*)&result);
	return result;
}

void CView3D1::SetYRotation(double propVal)
{
	SetProperty(0x4, VT_R8, propVal);
}

double CView3D1::GetZRotation()
{
	double result;
	GetProperty(0x5, VT_R8, (void*)&result);
	return result;
}

void CView3D1::SetZRotation(double propVal)
{
	SetProperty(0x5, VT_R8, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// CView3D1 operations
