// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "chart2d.h"

// Dispatch interfaces referenced by this interface
#include "Title.h"
#include "Legend.h"
#include "Border.h"
#include "Interior.h"
#include "ChartArea.h"
#include "ChartGroupCollection.h"
#include "ChartLabelCollection.h"
#include "ActionMapCollection.h"

/////////////////////////////////////////////////////////////////////////////
// CChart2D

IMPLEMENT_DYNCREATE(CChart2D, CWnd)

/////////////////////////////////////////////////////////////////////////////
// CChart2D properties

CTitle CChart2D::GetHeader()
{
	LPDISPATCH pDispatch;
	GetProperty(0x1, VT_DISPATCH, (void*)&pDispatch);
	return CTitle(pDispatch);
}

CTitle CChart2D::GetFooter()
{
	LPDISPATCH pDispatch;
	GetProperty(0x2, VT_DISPATCH, (void*)&pDispatch);
	return CTitle(pDispatch);
}

CLegend CChart2D::GetLegend()
{
	LPDISPATCH pDispatch;
	GetProperty(0x3, VT_DISPATCH, (void*)&pDispatch);
	return CLegend(pDispatch);
}

CBorder CChart2D::GetBorder()
{
	LPDISPATCH pDispatch;
	GetProperty(0x4, VT_DISPATCH, (void*)&pDispatch);
	return CBorder(pDispatch);
}

CInterior CChart2D::GetInterior()
{
	LPDISPATCH pDispatch;
	GetProperty(0x5, VT_DISPATCH, (void*)&pDispatch);
	return CInterior(pDispatch);
}

BOOL CChart2D::GetIsDoubleBuffered()
{
	BOOL result;
	GetProperty(0x7, VT_BOOL, (void*)&result);
	return result;
}

void CChart2D::SetIsDoubleBuffered(BOOL propVal)
{
	SetProperty(0x7, VT_BOOL, propVal);
}

CChartArea CChart2D::GetChartArea()
{
	LPDISPATCH pDispatch;
	GetProperty(0x8, VT_DISPATCH, (void*)&pDispatch);
	return CChartArea(pDispatch);
}

CChartGroupCollection CChart2D::GetChartGroups()
{
	LPDISPATCH pDispatch;
	GetProperty(0x9, VT_DISPATCH, (void*)&pDispatch);
	return CChartGroupCollection(pDispatch);
}

CChartLabelCollection CChart2D::GetChartLabels()
{
	LPDISPATCH pDispatch;
	GetProperty(0xa, VT_DISPATCH, (void*)&pDispatch);
	return CChartLabelCollection(pDispatch);
}

long CChart2D::GetErrorOffset()
{
	long result;
	GetProperty(0xb, VT_I4, (void*)&result);
	return result;
}

void CChart2D::SetErrorOffset(long propVal)
{
	SetProperty(0xb, VT_I4, propVal);
}

CActionMapCollection CChart2D::GetActionMaps()
{
	LPDISPATCH pDispatch;
	GetProperty(0xc, VT_DISPATCH, (void*)&pDispatch);
	return CActionMapCollection(pDispatch);
}

OLE_HANDLE CChart2D::GetHWnd()
{
	OLE_HANDLE result;
	GetProperty(DISPID_HWND, VT_I4, (void*)&result);
	return result;
}

void CChart2D::SetHWnd(OLE_HANDLE propVal)
{
	SetProperty(DISPID_HWND, VT_I4, propVal);
}

BOOL CChart2D::GetAllowUserChanges()
{
	BOOL result;
	GetProperty(0xd, VT_BOOL, (void*)&result);
	return result;
}

void CChart2D::SetAllowUserChanges(BOOL propVal)
{
	SetProperty(0xd, VT_BOOL, propVal);
}

LPUNKNOWN CChart2D::GetDataSource()
{
	LPUNKNOWN result;
	GetProperty(0x15, VT_UNKNOWN, (void*)&result);
	return result;
}

void CChart2D::SetDataSource(LPUNKNOWN propVal)
{
	SetProperty(0x15, VT_UNKNOWN, propVal);
}

CString CChart2D::GetDataPath()
{
	CString result;
	GetProperty(0x19, VT_BSTR, (void*)&result);
	return result;
}

void CChart2D::SetDataPath(LPCTSTR propVal)
{
	SetProperty(0x19, VT_BSTR, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// CChart2D operations

void CChart2D::Refresh()
{
	InvokeHelper(DISPID_REFRESH, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

BOOL CChart2D::Load(LPCTSTR FileName)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xe, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		FileName);
	return result;
}

BOOL CChart2D::Save(LPCTSTR FileName)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xf, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		FileName);
	return result;
}

void CChart2D::CallAction(long Action, long XCoord, long YCoord)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x10, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Action, XCoord, YCoord);
}

BOOL CChart2D::DrawToDC(long Hdc, long Format, long Scale, long Left, long Top, long Width, long Height)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x11, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		Hdc, Format, Scale, Left, Top, Width, Height);
	return result;
}

BOOL CChart2D::PrintChart(long Format, long Scale, long Left, long Top, long Width, long Height)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x12, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		Format, Scale, Left, Top, Width, Height);
	return result;
}

BOOL CChart2D::CopyToClipboard(long Format)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x13, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		Format);
	return result;
}

BOOL CChart2D::DrawToFile(LPCTSTR FileName, long Format)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_I4;
	InvokeHelper(0x14, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		FileName, Format);
	return result;
}

BOOL CChart2D::ShowChartEditorElement(short Element, BOOL Show)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I2 VTS_BOOL;
	InvokeHelper(0x16, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		Element, Show);
	return result;
}

BOOL CChart2D::IsChartEditorElementVisible(short Element)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x17, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		Element);
	return result;
}

BOOL CChart2D::LoadURL(LPCTSTR URLName)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x1a, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		URLName);
	return result;
}

void CChart2D::AboutBox()
{
	InvokeHelper(0xfffffdd8, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}