#if !defined(AFX_BORDER1_H__F8489CC3_CAAC_4CFA_89DA_762D71537E01__INCLUDED_)
#define AFX_BORDER1_H__F8489CC3_CAAC_4CFA_89DA_762D71537E01__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CBorder1 wrapper class

class CBorder1 : public COleDispatchDriver
{
public:
	CBorder1() {}		// Calls COleDispatchDriver default constructor
	CBorder1(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CBorder1(const CBorder1& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	LPDISPATCH GetParent();
	long GetType();
	void SetType(long);
	long GetWidth();
	void SetWidth(long);

// Operations
public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BORDER1_H__F8489CC3_CAAC_4CFA_89DA_762D71537E01__INCLUDED_)