#if !defined(AFX_INTERIOR_H__34DE9324_788C_426F_8F50_59EC0B47288A__INCLUDED_)
#define AFX_INTERIOR_H__34DE9324_788C_426F_8F50_59EC0B47288A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class CImage;

/////////////////////////////////////////////////////////////////////////////
// CInterior wrapper class

class CInterior : public COleDispatchDriver
{
public:
	CInterior() {}		// Calls COleDispatchDriver default constructor
	CInterior(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CInterior(const CInterior& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	unsigned long GetBackgroundColor();
	void SetBackgroundColor(unsigned long);
	unsigned long GetForegroundColor();
	void SetForegroundColor(unsigned long);
	LPDISPATCH GetParent();
	CImage GetImage();

// Operations
public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INTERIOR_H__34DE9324_788C_426F_8F50_59EC0B47288A__INCLUDED_)
