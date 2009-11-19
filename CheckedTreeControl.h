#if !defined(AFX_CHECKEDTREECONTROL_H__1E22595F_C2F0_4982_8D7F_76820CE0E0A2__INCLUDED_)
#define AFX_CHECKEDTREECONTROL_H__1E22595F_C2F0_4982_8D7F_76820CE0E0A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CheckedTreeControl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCheckedTreeCtrl window

class CCheckedTreeCtrl : public CTreeCtrl
{
// Construction
public:
	CCheckedTreeCtrl();

// Attributes
public:

		// Method to set the images for the tree-control
	void
	SetBitmaps(
		UINT NormalId,	// Resource-Id of the item-images
		UINT StatusId	// Resource-Id of the checkbox-images
	);

	// Method to switch the state of the checkbox in the selected item
	void ChangeItemState();	

// Operations
public:

	//imagelist for item-images
	CImageList	m_NormalImageList;

	//imagelist for checkbox-images fuer Statusbilder der Elemente
	// !!! KEEP CARE !!!
	// !!! LET IMAGE WITH INDEX 0 EMPTY AND DO NOT USE IT !!!
	CImageList	m_StatusImageList;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCheckedTreeCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCheckedTreeCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCheckedTreeCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHECKEDTREECONTROL_H__1E22595F_C2F0_4982_8D7F_76820CE0E0A2__INCLUDED_)
