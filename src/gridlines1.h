#if !defined(AFX_GRIDLINES1_H__5A26838D_3762_40D2_BB68_B7484CBA43CB__INCLUDED_)
#define AFX_GRIDLINES1_H__5A26838D_3762_40D2_BB68_B7484CBA43CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CGridLines1 wrapper class

class CGridLines1 : public COleDispatchDriver
{
public:
	CGridLines1() {}		// Calls COleDispatchDriver default constructor
	CGridLines1(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CGridLines1(const CGridLines1& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	LPDISPATCH GetParent();
	BOOL GetIsOnXYPlane();
	void SetIsOnXYPlane(BOOL);
	BOOL GetIsOnXZPlane();
	void SetIsOnXZPlane(BOOL);
	BOOL GetIsOnYZPlane();
	void SetIsOnYZPlane(BOOL);

// Operations
public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDLINES1_H__5A26838D_3762_40D2_BB68_B7484CBA43CB__INCLUDED_)
