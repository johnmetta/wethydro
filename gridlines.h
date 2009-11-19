#if !defined(AFX_GRIDLINES_H__7C4C7794_1F98_4F76_8984_E7F0BE84F415__INCLUDED_)
#define AFX_GRIDLINES_H__7C4C7794_1F98_4F76_8984_E7F0BE84F415__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class CDerivedDouble;
class CLineStyle;

/////////////////////////////////////////////////////////////////////////////
// CGridLines wrapper class

class CGridLines : public COleDispatchDriver
{
public:
	CGridLines() {}		// Calls COleDispatchDriver default constructor
	CGridLines(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CGridLines(const CGridLines& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	BOOL GetIsStyleDefault();
	void SetIsStyleDefault(BOOL);
	CDerivedDouble GetSpacing();
	CLineStyle GetStyle();
	LPDISPATCH GetParent();

// Operations
public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDLINES_H__7C4C7794_1F98_4F76_8984_E7F0BE84F415__INCLUDED_)
