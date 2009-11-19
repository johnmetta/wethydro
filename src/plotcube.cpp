// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "plotcube.h"

// Dispatch interfaces referenced by this interface
#include "Ceiling.h"
#include "Floor.h"
#include "Interior1.h"


/////////////////////////////////////////////////////////////////////////////
// CPlotCube properties

LPDISPATCH CPlotCube::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x1, VT_DISPATCH, (void*)&result);
	return result;
}

double CPlotCube::GetXScale()
{
	double result;
	GetProperty(0x2, VT_R8, (void*)&result);
	return result;
}

void CPlotCube::SetXScale(double propVal)
{
	SetProperty(0x2, VT_R8, propVal);
}

double CPlotCube::GetYScale()
{
	double result;
	GetProperty(0x3, VT_R8, (void*)&result);
	return result;
}

void CPlotCube::SetYScale(double propVal)
{
	SetProperty(0x3, VT_R8, propVal);
}

CCeiling CPlotCube::GetCeiling()
{
	LPDISPATCH pDispatch;
	GetProperty(0x4, VT_DISPATCH, (void*)&pDispatch);
	return CCeiling(pDispatch);
}

CFloor CPlotCube::GetFloor()
{
	LPDISPATCH pDispatch;
	GetProperty(0x5, VT_DISPATCH, (void*)&pDispatch);
	return CFloor(pDispatch);
}

CInterior1 CPlotCube::GetInterior()
{
	LPDISPATCH pDispatch;
	GetProperty(0x6, VT_DISPATCH, (void*)&pDispatch);
	return CInterior1(pDispatch);
}

/////////////////////////////////////////////////////////////////////////////
// CPlotCube operations
