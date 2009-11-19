#if !defined(AFX_VIEW3D_H__BDF5CF71_316C_4EE3_BE89_B575BD064F5E__INCLUDED_)
#define AFX_VIEW3D_H__BDF5CF71_316C_4EE3_BE89_B575BD064F5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CView3D wrapper class

class CView3D : public COleDispatchDriver
{
public:
	CView3D() {}		// Calls COleDispatchDriver default constructor
	CView3D(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CView3D(const CView3D& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	long GetDepth();
	void SetDepth(long);
	long GetElevation();
	void SetElevation(long);
	long GetRotation();
	void SetRotation(long);
	LPDISPATCH GetParent();
	short GetShading();
	void SetShading(short);

// Operations
public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEW3D_H__BDF5CF71_316C_4EE3_BE89_B575BD064F5E__INCLUDED_)
