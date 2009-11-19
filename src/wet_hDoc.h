// wet_hDoc.h : interface of the CWet_hDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WET_HDOC_H__2F532005_0498_4D11_90B3_10EB047C8FA6__INCLUDED_)
#define AFX_WET_HDOC_H__2F532005_0498_4D11_90B3_10EB047C8FA6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include <reachtree.h>
#include <MapWnd.h>
#include "lulctree.h"
#include "wet_hydro.h"

class maplayer;

struct LULC_INFO
   {
   float landValue;   // land value, $/??
  	float	soilValue;   // soil value, $/??
	float	income;      
	float	rentalRate;

   LULC_INFO( void ) : landValue( 0.0f ), soilValue( 0.0f ), income( 0.0f ), rentalRate( 0.0f ) { }
   };

class LulcInfoArray : public CArray< LULC_INFO*, LULC_INFO* >
   {
   public:
      ~LulcInfoArray( void ) { for  ( int i=0; i < GetSize(); i++ ) delete GetAt( i );  RemoveAll(); }
   };

// note:  types are  vt types as defined typedefs.hpp
// display flags :  see bin flags in maplayer.h

struct WET_FIELD_INFO
   {
   CString fieldname;
   CString label;
   int type;
   int displayFlags;
   int level;
   float minVal;
   float maxVal;

   WET_FIELD_INFO( LPCSTR _fieldname, int _level, LPCSTR _label, int _type, int _displayFlags, float _minVal, float _maxVal)
      : fieldname( _fieldname ), label( _label ), type( _type ), displayFlags( _displayFlags ), level( _level ), minVal( _minVal), maxVal( _maxVal) 
         { }
   };

class FieldInfoArray : public CArray< WET_FIELD_INFO*, WET_FIELD_INFO* >
   {
   public:
      void AddFieldInfo( LPCSTR fieldname, int level, LPCSTR label, int type, int displayFlags, float min, float max)
         { Add( new WET_FIELD_INFO( fieldname, level, label, type, displayFlags, min, max ) ); }

      ~FieldInfoArray( void ) { for ( int i=0; i < GetSize(); i++ ) delete GetAt( i );  RemoveAll(); }
   };

class CWet_hDoc : public CDocument
{
protected: // create from serialization only
	CWet_hDoc();
	DECLARE_DYNCREATE(CWet_hDoc)

// Attributes
public:

   ReachTree m_reachTree;     // store stream topology information
   LulcTree  m_lulcTree;      // store lulc heirarchy
   LulcInfoArray m_lulcInfoArray;
   CString m_name;

   FieldInfoArray m_fieldInfoArray;
   int   m_currentRun;     // number of the current run (0 means no run, 1 mean one run made, etc...)
   int   m_evalsCompleted; // flags indicating wich evaluative model results are available
// Operations
public:

   MapLayer *AddMapLayer( LPCSTR filename, int type, int includeData, int extraCols );
   int       GetFieldInfoCount( void ) { return m_fieldInfoArray.GetSize(); }
	WET_FIELD_INFO *FindFieldInfo( LPCSTR label );
   WET_FIELD_INFO *GetFieldInfo( int offset ) { return m_fieldInfoArray[ offset ]; }
   int  SelectWatershed( MapLayer *pCellLayer, int reachID, int colReachID );
   void FindUpstreamReaches( ReachNode *pNode, CArray< ReachNode*, ReachNode* > &upstreamReachArray );
   int       LoadLulcInfo( LPCSTR filename );
   bool      GetLulcInfo( int level, int landUse, LULC_INFO &info );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWet_hDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWet_hDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CWet_hDoc)
	afx_msg void OnFileLoadmap();
	afx_msg void OnProcesscellGo();

	afx_msg void OnUpdateModelanalysisOptimization(CCmdUI* pCmdUI);
	afx_msg void OnFileSaveparameters();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WET_HDOC_H__2F532005_0498_4D11_90B3_10EB047C8FA6__INCLUDED_)
