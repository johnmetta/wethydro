#if !defined(AFX_FILLSTYLE1_H__2D23BF4D_94FC_4419_9F4D_222C71E8DD5D__INCLUDED_)
#define AFX_FILLSTYLE1_H__2D23BF4D_94FC_4419_9F4D_222C71E8DD5D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CFillStyle1 wrapper class

class CFillStyle1 : public COleDispatchDriver
{
public:
	CFillStyle1() {}		// Calls COleDispatchDriver default constructor
	CFillStyle1(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CFillStyle1(const CFillStyle1& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	LPDISPATCH GetParent();
	unsigned long GetColor();
	void SetColor(unsigned long);

// Operations
public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILLSTYLE1_H__2D23BF4D_94FC_4419_9F4D_222C71E8DD5D__INCLUDED_)
