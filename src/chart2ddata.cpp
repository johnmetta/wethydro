// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "chart2ddata.h"

// Dispatch interfaces referenced by this interface
#include "Chart2DCheckNewPointsResult.h"


/////////////////////////////////////////////////////////////////////////////
// CChart2DData properties

long CChart2DData::GetLayout()
{
	long result;
	GetProperty(0x1, VT_I4, (void*)&result);
	return result;
}

void CChart2DData::SetLayout(long propVal)
{
	SetProperty(0x1, VT_I4, propVal);
}

long CChart2DData::GetNumSeries()
{
	long result;
	GetProperty(0x2, VT_I4, (void*)&result);
	return result;
}

void CChart2DData::SetNumSeries(long propVal)
{
	SetProperty(0x2, VT_I4, propVal);
}

long CChart2DData::GetFirstSeries()
{
	long result;
	GetProperty(0x3, VT_I4, (void*)&result);
	return result;
}

void CChart2DData::SetFirstSeries(long propVal)
{
	SetProperty(0x3, VT_I4, propVal);
}

long CChart2DData::GetLastSeries()
{
	long result;
	GetProperty(0x4, VT_I4, (void*)&result);
	return result;
}

void CChart2DData::SetLastSeries(long propVal)
{
	SetProperty(0x4, VT_I4, propVal);
}

double CChart2DData::GetHoleValue()
{
	double result;
	GetProperty(0x5, VT_R8, (void*)&result);
	return result;
}

void CChart2DData::SetHoleValue(double propVal)
{
	SetProperty(0x5, VT_R8, propVal);
}

BOOL CChart2DData::GetIsBatched()
{
	BOOL result;
	GetProperty(0x6, VT_BOOL, (void*)&result);
	return result;
}

void CChart2DData::SetIsBatched(BOOL propVal)
{
	SetProperty(0x6, VT_BOOL, propVal);
}

LPDISPATCH CChart2DData::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x7, VT_DISPATCH, (void*)&result);
	return result;
}

BOOL CChart2DData::GetIsSaved()
{
	BOOL result;
	GetProperty(0x8, VT_BOOL, (void*)&result);
	return result;
}

void CChart2DData::SetIsSaved(BOOL propVal)
{
	SetProperty(0x8, VT_BOOL, propVal);
}

CString CChart2DData::GetFileName()
{
	CString result;
	GetProperty(0x9, VT_BSTR, (void*)&result);
	return result;
}

void CChart2DData::SetFileName(LPCTSTR propVal)
{
	SetProperty(0x9, VT_BSTR, propVal);
}

CString CChart2DData::GetDataPath()
{
	CString result;
	GetProperty(0x1f, VT_BSTR, (void*)&result);
	return result;
}

void CChart2DData::SetDataPath(LPCTSTR propVal)
{
	SetProperty(0x1f, VT_BSTR, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// CChart2DData operations

long CChart2DData::GetNumPoints(long Series)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x10, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms,
		Series);
	return result;
}

void CChart2DData::SetNumPoints(long Series, long nNewValue)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x10, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 Series, nNewValue);
}

long CChart2DData::GetFirstPoint(long Series)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x11, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms,
		Series);
	return result;
}

void CChart2DData::SetFirstPoint(long Series, long nNewValue)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x11, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 Series, nNewValue);
}

long CChart2DData::GetLastPoint(long Series)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x12, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms,
		Series);
	return result;
}

void CChart2DData::SetLastPoint(long Series, long nNewValue)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x12, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 Series, nNewValue);
}

double CChart2DData::GetX(long Series, long Point)
{
	double result;
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x13, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, parms,
		Series, Point);
	return result;
}

void CChart2DData::SetX(long Series, long Point, double newValue)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_R8;
	InvokeHelper(0x13, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 Series, Point, newValue);
}

double CChart2DData::GetY(long Series, long Point)
{
	double result;
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x14, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, parms,
		Series, Point);
	return result;
}

void CChart2DData::SetY(long Series, long Point, double newValue)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_R8;
	InvokeHelper(0x14, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 Series, Point, newValue);
}

BOOL CChart2DData::Load(LPCTSTR FileName)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xa, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		FileName);
	return result;
}

BOOL CChart2DData::ShiftPoints(long Series, long NumPoints, long Dest, long Src)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0xb, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		Series, NumPoints, Dest, Src);
	return result;
}

BOOL CChart2DData::Sort()
{
	BOOL result;
	InvokeHelper(0xc, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL CChart2DData::Save(LPCTSTR FileName)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xd, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		FileName);
	return result;
}

BOOL CChart2DData::CheckNewPoints(long Series, long NewPoints, BOOL* IsOutsideXMin, BOOL* IsOutsideXMax, BOOL* IsOutsideYMin, BOOL* IsOutsideYMax)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_PBOOL VTS_PBOOL VTS_PBOOL VTS_PBOOL;
	InvokeHelper(0xe, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		Series, NewPoints, IsOutsideXMin, IsOutsideXMax, IsOutsideYMin, IsOutsideYMax);
	return result;
}

BOOL CChart2DData::DrawNewPoints(long Series, long NewPoints)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0xf, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		Series, NewPoints);
	return result;
}

short CChart2DData::GetSeriesDisplay(long Series)
{
	short result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x15, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, parms,
		Series);
	return result;
}

void CChart2DData::SetSeriesDisplay(long Series, short nNewValue)
{
	static BYTE parms[] =
		VTS_I4 VTS_I2;
	InvokeHelper(0x15, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 Series, nNewValue);
}

BOOL CChart2DData::CopyXVectorIn(long Series, const VARIANT& vArray)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I4 VTS_VARIANT;
	InvokeHelper(0x16, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		Series, &vArray);
	return result;
}

BOOL CChart2DData::CopyYVectorIn(long Series, const VARIANT& vArray)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I4 VTS_VARIANT;
	InvokeHelper(0x17, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		Series, &vArray);
	return result;
}

BOOL CChart2DData::CopyYArrayIn(const VARIANT& vArray)
{
	BOOL result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x18, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		&vArray);
	return result;
}

BOOL CChart2DData::CopyXVectorOut(long Series, const VARIANT& vArray)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I4 VTS_VARIANT;
	InvokeHelper(0x19, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		Series, &vArray);
	return result;
}

BOOL CChart2DData::CopyYVectorOut(long Series, const VARIANT& vArray)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I4 VTS_VARIANT;
	InvokeHelper(0x1a, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		Series, &vArray);
	return result;
}

BOOL CChart2DData::CopyYArrayOut(const VARIANT& vArray)
{
	BOOL result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x1b, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		&vArray);
	return result;
}

long CChart2DData::UpdateDataValue(double OldValue, double NewValue)
{
	long result;
	static BYTE parms[] =
		VTS_R8 VTS_R8;
	InvokeHelper(0x1c, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		OldValue, NewValue);
	return result;
}

CChart2DCheckNewPointsResult CChart2DData::CheckNewPointsObject(long Series, long NewPoints)
{
	LPDISPATCH pDispatch;
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x1d, DISPATCH_METHOD, VT_DISPATCH, (void*)&pDispatch, parms,
		Series, NewPoints);
	return CChart2DCheckNewPointsResult(pDispatch);
}

BOOL CChart2DData::LoadURL(LPCTSTR URLName)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x1e, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		URLName);
	return result;
}
