#if !defined(AFX_VALUELABEL_H__D18ADD9F_32B6_46BE_ADCB_07E0BFD74712__INCLUDED_)
#define AFX_VALUELABEL_H__D18ADD9F_32B6_46BE_ADCB_07E0BFD74712__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class CLabel;

/////////////////////////////////////////////////////////////////////////////
// CValueLabel wrapper class

class CValueLabel : public COleDispatchDriver
{
public:
	CValueLabel() {}		// Calls COleDispatchDriver default constructor
	CValueLabel(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CValueLabel(const CValueLabel& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	double GetValue();
	void SetValue(double);
	CLabel GetLabel();
	LPDISPATCH GetParent();

// Operations
public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VALUELABEL_H__D18ADD9F_32B6_46BE_ADCB_07E0BFD74712__INCLUDED_)