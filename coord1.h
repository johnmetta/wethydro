#if !defined(AFX_COORD1_H__17EC297B_D49A_4BE3_AF1C_31F0294B12C4__INCLUDED_)
#define AFX_COORD1_H__17EC297B_D49A_4BE3_AF1C_31F0294B12C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CCoord1 wrapper class

class CCoord1 : public COleDispatchDriver
{
public:
	CCoord1() {}		// Calls COleDispatchDriver default constructor
	CCoord1(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CCoord1(const CCoord1& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	LPDISPATCH GetParent();
	long GetX();
	void SetX(long);
	long GetY();
	void SetY(long);

// Operations
public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COORD1_H__17EC297B_D49A_4BE3_AF1C_31F0294B12C4__INCLUDED_)
