#if !defined(AFX_VALUELABEL1_H__EA4FCD31_35A8_45DD_A3D9_91B9BE2E8B4D__INCLUDED_)
#define AFX_VALUELABEL1_H__EA4FCD31_35A8_45DD_A3D9_91B9BE2E8B4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class CLabel1;

/////////////////////////////////////////////////////////////////////////////
// CValueLabel1 wrapper class

class CValueLabel1 : public COleDispatchDriver
{
public:
	CValueLabel1() {}		// Calls COleDispatchDriver default constructor
	CValueLabel1(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CValueLabel1(const CValueLabel1& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	LPDISPATCH GetParent();
	double GetValue();
	void SetValue(double);
	CLabel1 GetLabel();

// Operations
public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VALUELABEL1_H__EA4FCD31_35A8_45DD_A3D9_91B9BE2E8B4D__INCLUDED_)
