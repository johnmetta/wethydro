#if !defined(AFX_DERIVEDDOUBLE_H__54D1D541_49E5_4050_9FB6_0C3C85C603EC__INCLUDED_)
#define AFX_DERIVEDDOUBLE_H__54D1D541_49E5_4050_9FB6_0C3C85C603EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CDerivedDouble wrapper class

class CDerivedDouble : public COleDispatchDriver
{
public:
	CDerivedDouble() {}		// Calls COleDispatchDriver default constructor
	CDerivedDouble(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CDerivedDouble(const CDerivedDouble& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	double GetValue();
	void SetValue(double);
	BOOL GetIsDefault();
	void SetIsDefault(BOOL);
	LPDISPATCH GetParent();

// Operations
public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DERIVEDDOUBLE_H__54D1D541_49E5_4050_9FB6_0C3C85C603EC__INCLUDED_)
