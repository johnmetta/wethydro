#if !defined(AFX_ACTIONMAP1_H__B03944A8_937D_48DA_99D8_A2B84F4CD799__INCLUDED_)
#define AFX_ACTIONMAP1_H__B03944A8_937D_48DA_99D8_A2B84F4CD799__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CActionMap1 wrapper class

class CActionMap1 : public COleDispatchDriver
{
public:
	CActionMap1() {}		// Calls COleDispatchDriver default constructor
	CActionMap1(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CActionMap1(const CActionMap1& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	short GetMessage();
	short GetModifier();
	short GetKeyCode();
	short GetAction();
	void SetAction(short);
	LPDISPATCH GetParent();

// Operations
public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACTIONMAP1_H__B03944A8_937D_48DA_99D8_A2B84F4CD799__INCLUDED_)