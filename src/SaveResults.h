#if !defined(AFX_SAVERESULTS_H__990EB152_3229_475C_9093_09A04C8156B1__INCLUDED_)
#define AFX_SAVERESULTS_H__990EB152_3229_475C_9093_09A04C8156B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SaveResults.h : header file
//
#include <afxtempl.h>
#include "fdataobj.h"
//class FloatMatrix;
class DataObj;
class DataTable;

/////////////////////////////////////////////////////////////////////////////
// SaveResults dialog




class SaveResults : public CDialog
{
// Construction
public:
	SaveResults(CWnd* pParent = NULL);   // standard constructor



// Dialog Data
	//{{AFX_DATA(SaveResults)
	enum { IDD = IDD_SAVERESULTS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SaveResults)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(SaveResults)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SAVERESULTS_H__990EB152_3229_475C_9093_09A04C8156B1__INCLUDED_)
