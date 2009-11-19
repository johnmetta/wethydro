// wet_hView.h : interface of the CWet_hView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WET_HVIEW_H__217EC87B_BC01_40E3_84E3_5FEA72521005__INCLUDED_)
#define AFX_WET_HVIEW_H__217EC87B_BC01_40E3_84E3_5FEA72521005__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "outputTab.h"

#include "optimizeDlg.h"
#include "optimizeDlg3DPlot.h"
#include "map.h"

#include <map3d/avigenerator.h>

struct RANDOM_NUMBER
   {
   char   dist[ 64 ];
   int    distNum;
   float  minValue;
   float  maxValue;
   float  std;
   float  mean;

   RANDOM_NUMBER( void ) : dist(), distNum( 0 ), minValue( 0.0f ), maxValue(0.0f), std(0.0f), mean(0.0f) { }
   };


class RandomNumberArray : public CArray< RANDOM_NUMBER*, RANDOM_NUMBER* >
   {
   public:
      // destructor cleans up any allocated structures
     ~RandomNumberArray( void ) { for ( int i=0; i < GetSize(); i++ ) delete GetAt( i ); }      
   };


class DataTable;

class CWet_hDoc;

#include <randgen\randunif.hpp>
#include <randgen\randLN.hpp>
#include <randgen\randnorm.hpp>

class CWet_hView : public CView
{
protected: // create from serialization only
	CWet_hView();
	DECLARE_DYNCREATE(CWet_hView)

// Attributes
public:
	CWet_hDoc* GetDocument();

   CDialogBar m_dialogBar;
	optimizeDlg m_optimizeDlg;
	OptimizeDlg3DPlot m_optimizeDlg3DPlot;
   
   OutputTab m_outputTab;
   DataTable *m_pDBTable;
   DataTable *m_pMonteCarloDBTable;
//   WindowManager m_graphManager;
	int m_mode;
   RandUniform m_randUnif1;
   RandUniform m_randUnif2;
   RandUniform m_randUnif3;
   RandUniform m_randUnif4;
   RandUniform m_randUnif5;
   RandUniform m_randUnif6;

   RandLogNormal *m_pRandLN1;
   RandLogNormal *m_pRandLN2;
   RandLogNormal *m_pRandLN3;

   RandNormal *m_pRandN1;
   RandNormal *m_pRandN2;
   RandNormal *m_pRandN3;

   // movie stuff follows
   CAVIGenerator** AVIGenerators;
   struct AuxMovieStuff* auxMovieStuff;

   int numMovies;
   int movieFrameRate;


	BOOL	m_checkIS;
	BOOL	m_checkKSat;
	BOOL	m_checkM;
	BOOL	m_checkN;
	int		m_editNumIter;
	float	m_editRandomSeed;
	int		m_isDist;
	float	m_isMax;
	float	m_isMin;
	int		m_kSatDist;
	float	m_kSatMax;
	float	m_kSatMin;
	float	m_mDist;
	float	m_mMax;
	float	m_mMin;
	float	m_nDist;
	float	m_nMax;
	float	m_nMin;
	BOOL	m_checkUpslope;
	int		m_upslopeDist;
	float	m_upslopeMax;
	float	m_upslopeMin;

   RandomNumberArray m_randomNumberArray;

   void GenerateRandomValues();


   float GenerateRandom(float minValue, float maxValue);
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWet_hView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWet_hView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CWet_hView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnEditCopy();
	afx_msg void OnZoomin();
	afx_msg void OnZoomout();
	afx_msg void OnUpdateZoomin(CCmdUI* pCmdUI);
	afx_msg void OnUpdateZoomout(CCmdUI* pCmdUI);
	afx_msg void OnSelect();
	afx_msg void OnQuerydistance();
	afx_msg void OnZoomfull();
	afx_msg void OnDbtable();
	afx_msg void OnPan();
	afx_msg void OnViewPriorityfilter();
	afx_msg void OnAnalysisRunmodel();
	afx_msg void OnModelanalysisUserinteraction();
	afx_msg void OnModelanalysisGeneratereport();
	afx_msg void OnStatistics();
	afx_msg void OnFileClose();
	afx_msg void OnAppStop();

	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:

   afx_msg void OnModelanalysisDefinehydrologymodel();
   afx_msg void OnModelanalysisModelevaluation();
   afx_msg void OnModelanalysisPesticidemodel();
   afx_msg void OnModelanalysisRestorationsensitivity();
   afx_msg void OnTopographiccalculationMeanwatershedelevation();
   afx_msg void OnTopographiccalculationSubbasinfpdistribution();
   afx_msg void OnTopographiccalculationSubbasindistancetodsoutletdistribution();
   afx_msg void OnTopographiccalculationDistancefrompolytostream();
   afx_msg void OnTopographiccalculationCalculateupstreamarea();
   afx_msg void OnAdditionalstuffLoaduaradar();
   afx_msg void OnTopographiccalculationSideofstream();
   afx_msg void OnModelanalysisSavemodelstate();
   afx_msg void OnTopographiccalculationDisttostream();
   void GetFPDistGrid();
   afx_msg void OnMenu();
   afx_msg void OnModelanalysisRunhillslopemodel();
   afx_msg void OnTopographiccalculationWillametteqmovie();
   afx_msg void OnTopographiccalculationDisgradientandquotientforgrid();
};

   struct auxMovieStuff
		{
		CDC bitmapDC;
		CBitmap bitmap;
		BITMAPINFO bi;
		LPBITMAPINFO lpbi;
		BYTE* bitAddress;
		};


#ifndef _DEBUG  // debug version in wet_hView.cpp
inline CWet_hDoc* CWet_hView::GetDocument()
   { return (CWet_hDoc*)m_pDocument; }




#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WET_HVIEW_H__217EC87B_BC01_40E3_84E3_5FEA72521005__INCLUDED_)
