#if !defined(AFX_ACTIONMAPCOLLECTION1_H__BB013320_CB92_4AA7_8F3D_9AA83129DBBC__INCLUDED_)
#define AFX_ACTIONMAPCOLLECTION1_H__BB013320_CB92_4AA7_8F3D_9AA83129DBBC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class CActionMap1;

/////////////////////////////////////////////////////////////////////////////
// CActionMapCollection1 wrapper class

class CActionMapCollection1 : public COleDispatchDriver
{
public:
	CActionMapCollection1() {}		// Calls COleDispatchDriver default constructor
	CActionMapCollection1(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CActionMapCollection1(const CActionMapCollection1& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	long GetCount();
	LPDISPATCH GetParent();

// Operations
public:
	CActionMap1 Add(short Message, short Modifier, short KeyCode, short Action);
	void Remove(short Message, short Modifier, short KeyCode);
	CActionMap1 GetItem(short Message, short Modifier, short KeyCode);
	void RemoveAll();
	void Reset();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACTIONMAPCOLLECTION1_H__BB013320_CB92_4AA7_8F3D_9AA83129DBBC__INCLUDED_)
