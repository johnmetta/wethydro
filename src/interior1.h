#if !defined(AFX_INTERIOR1_H__C20794D8_9FF1_4BD5_9199_4920153A7C5A__INCLUDED_)
#define AFX_INTERIOR1_H__C20794D8_9FF1_4BD5_9199_4920153A7C5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class CImage1;

/////////////////////////////////////////////////////////////////////////////
// CInterior1 wrapper class

class CInterior1 : public COleDispatchDriver
{
public:
	CInterior1() {}		// Calls COleDispatchDriver default constructor
	CInterior1(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CInterior1(const CInterior1& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	LPDISPATCH GetParent();
	unsigned long GetBackgroundColor();
	void SetBackgroundColor(unsigned long);
	unsigned long GetForegroundColor();
	void SetForegroundColor(unsigned long);
	CImage1 GetImage();

// Operations
public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INTERIOR1_H__C20794D8_9FF1_4BD5_9199_4920153A7C5A__INCLUDED_)