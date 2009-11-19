#if !defined(AFX_CHARTLABEL_H__04C676F9_B8EB_40E8_8EB7_25A8F27A7218__INCLUDED_)
#define AFX_CHARTLABEL_H__04C676F9_B8EB_40E8_8EB7_25A8F27A7218__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class CBorder;
class CInterior;
class CLabel;
class CCoord;
class CDataCoord;
class CDataIndex;
class CLocation;
class COleFont;

/////////////////////////////////////////////////////////////////////////////
// CChartLabel wrapper class

class CChartLabel : public COleDispatchDriver
{
public:
	CChartLabel() {}		// Calls COleDispatchDriver default constructor
	CChartLabel(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CChartLabel(const CChartLabel& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	CBorder GetBorder();
	CInterior GetInterior();
	CLabel GetText();
	CCoord GetAttachCoord();
	CDataCoord GetAttachDataCoord();
	CDataIndex GetAttachDataIndex();
	CString GetName();
	void SetName(LPCTSTR);
	BOOL GetIsShowing();
	void SetIsShowing(BOOL);
	BOOL GetIsConnected();
	void SetIsConnected(BOOL);
	BOOL GetIsBatched();
	void SetIsBatched(BOOL);
	long GetAttachMethod();
	void SetAttachMethod(long);
	long GetAnchor();
	void SetAnchor(long);
	long GetOffset();
	void SetOffset(long);
	long GetAdjust();
	void SetAdjust(long);
	LPDISPATCH GetParent();
	CLocation GetLocation();
	COleFont GetFont();
	void SetFont(LPDISPATCH);
	long GetRotation();
	void SetRotation(long);

// Operations
public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHARTLABEL_H__04C676F9_B8EB_40E8_8EB7_25A8F27A7218__INCLUDED_)