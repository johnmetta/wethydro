#if !defined(AFX_IMAGE1_H__48F5AE4C_41C0_4E49_BB27_143D8E315AF2__INCLUDED_)
#define AFX_IMAGE1_H__48F5AE4C_41C0_4E49_BB27_143D8E315AF2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CImage1 wrapper class

class CImage1 : public COleDispatchDriver
{
public:
	CImage1() {}		// Calls COleDispatchDriver default constructor
	CImage1(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CImage1(const CImage1& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	LPDISPATCH GetParent();
	CString GetFileName();
	void SetFileName(LPCTSTR);
	long GetLayout();
	void SetLayout(long);
	BOOL GetIsEmbedded();
	void SetIsEmbedded(BOOL);

// Operations
public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGE1_H__48F5AE4C_41C0_4E49_BB27_143D8E315AF2__INCLUDED_)
