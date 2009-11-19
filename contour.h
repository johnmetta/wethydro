#if !defined(AFX_CONTOUR_H__0B408C08_ECD9_493A_858A_6D669F259323__INCLUDED_)
#define AFX_CONTOUR_H__0B408C08_ECD9_493A_858A_6D669F259323__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class CContourLevels;
class CContourStyleCollection;

/////////////////////////////////////////////////////////////////////////////
// CContour wrapper class

class CContour : public COleDispatchDriver
{
public:
	CContour() {}		// Calls COleDispatchDriver default constructor
	CContour(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CContour(const CContour& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	LPDISPATCH GetParent();
	BOOL GetIsContoured();
	void SetIsContoured(BOOL);
	BOOL GetIsZoned();
	void SetIsZoned(BOOL);
	long GetZoneMethod();
	void SetZoneMethod(long);
	CContourLevels GetLevels();
	CContourStyleCollection GetStyles();

// Operations
public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTOUR_H__0B408C08_ECD9_493A_858A_6D669F259323__INCLUDED_)
