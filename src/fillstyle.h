#if !defined(AFX_FILLSTYLE_H__47249F55_AD94_410A_9B93_43CB859EA16E__INCLUDED_)
#define AFX_FILLSTYLE_H__47249F55_AD94_410A_9B93_43CB859EA16E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CFillStyle wrapper class

class CFillStyle : public COleDispatchDriver
{
public:
	CFillStyle() {}		// Calls COleDispatchDriver default constructor
	CFillStyle(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CFillStyle(const CFillStyle& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	long GetPattern();
	void SetPattern(long);
	unsigned long GetColor();
	void SetColor(unsigned long);
	LPDISPATCH GetParent();

// Operations
public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILLSTYLE_H__47249F55_AD94_410A_9B93_43CB859EA16E__INCLUDED_)
