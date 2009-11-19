#if !defined(AFX_CONTOURLEVELS_H__2350ABC9_0782_438F_94FD_636A1F5FF7CE__INCLUDED_)
#define AFX_CONTOURLEVELS_H__2350ABC9_0782_438F_94FD_636A1F5FF7CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CContourLevels wrapper class

class CContourLevels : public COleDispatchDriver
{
public:
	CContourLevels() {}		// Calls COleDispatchDriver default constructor
	CContourLevels(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CContourLevels(const CContourLevels& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	LPDISPATCH GetParent();
	BOOL GetIsDefault();
	void SetIsDefault(BOOL);
	long GetNumLevels();
	void SetNumLevels(long);

// Operations
public:
	double GetValue(const VARIANT& Index);
	void SetValue(const VARIANT& Index, double newValue);
	void Add(double Value);
	void Remove(double Value);
	long CalcIndex(double Value);
	BOOL Load(LPCTSTR FileName);
	BOOL Save(LPCTSTR FileName);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTOURLEVELS_H__2350ABC9_0782_438F_94FD_636A1F5FF7CE__INCLUDED_)