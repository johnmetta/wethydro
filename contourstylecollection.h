#if !defined(AFX_CONTOURSTYLECOLLECTION_H__FA349A87_63BC_4940_A792_8AD4A2ED7685__INCLUDED_)
#define AFX_CONTOURSTYLECOLLECTION_H__FA349A87_63BC_4940_A792_8AD4A2ED7685__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class CContourStyle;

/////////////////////////////////////////////////////////////////////////////
// CContourStyleCollection wrapper class

class CContourStyleCollection : public COleDispatchDriver
{
public:
	CContourStyleCollection() {}		// Calls COleDispatchDriver default constructor
	CContourStyleCollection(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CContourStyleCollection(const CContourStyleCollection& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	LPDISPATCH GetParent();
	long GetCount();

// Operations
public:
	CContourStyle Add(const VARIANT& Before);
	BOOL Remove(const VARIANT& Index);
	long CalcIndex(const VARIANT& LevelIndex);
	void Reset();
	BOOL Load(LPCTSTR FileName);
	BOOL Save(LPCTSTR FileName);
	CContourStyle GetItem(const VARIANT& Index);
	void RemoveAll();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTOURSTYLECOLLECTION_H__FA349A87_63BC_4940_A792_8AD4A2ED7685__INCLUDED_)
