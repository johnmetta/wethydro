#if !defined(AFX_CONTOURSTYLE_H__27825E1C_1D02_44B3_AC4D_A21228C7D531__INCLUDED_)
#define AFX_CONTOURSTYLE_H__27825E1C_1D02_44B3_AC4D_A21228C7D531__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class CLineStyle1;
class CFillStyle1;

/////////////////////////////////////////////////////////////////////////////
// CContourStyle wrapper class

class CContourStyle : public COleDispatchDriver
{
public:
	CContourStyle() {}		// Calls COleDispatchDriver default constructor
	CContourStyle(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CContourStyle(const CContourStyle& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	LPDISPATCH GetParent();
	CLineStyle1 GetLineStyle();
	CFillStyle1 GetFillStyle();

// Operations
public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTOURSTYLE_H__27825E1C_1D02_44B3_AC4D_A21228C7D531__INCLUDED_)