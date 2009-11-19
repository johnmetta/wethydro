//{{AFX_INCLUDES()
#include "chart2d.h"
#include "chart3d.h"
//}}AFX_INCLUDES
#if !defined(AFX_OPTIMIZEDLG_H__B9EDC2B1_80A7_45DC_99D2_17336ED47C10__INCLUDED_)
#define AFX_OPTIMIZEDLG_H__B9EDC2B1_80A7_45DC_99D2_17336ED47C10__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// optimizeDlg.h : header file
//
#include "resource.h"
#include "wet_hydro.h"
/////////////////////////

////////////////////////////////////////////////////
// optimizeDlg dialog


struct PARAM_INFO
   {
   char   name[ 64 ];
   float  minValue;
   float  maxValue;
   float  numIncrements;

   PARAM_INFO( void ) : name(), minValue( 0.0f ), maxValue( 0.0f ), numIncrements(0.0f) { }
   };


class ParamInfoArray : public CArray< PARAM_INFO*, PARAM_INFO* >
   {
   public:
      // destructor cleans up any allocated structures
     ~ParamInfoArray( void ) { for ( int i=0; i < GetSize(); i++ ) delete GetAt( i ); }      
   };

struct PARAM_HISTORY
   {
//   char   name[ 64 ];
   float  chisq;
   float  a1;
   float  a2;
   float  a3;
   float  a4;
   float  a5;
   float  a6;
   float  a7;
	int    ndata;
	float  *x;
	float  *y;
	float  *ymod;
   PARAM_HISTORY( void ) : chisq( 0.0f ), a1( 0.0f ), a2(0.0f), a3(0.0f), a4(0.0f), a5(0.0f), a6(0.0f), a7(0.0f), ndata(0) { }
   };


class ParamHistoryArray : public CArray< PARAM_HISTORY*, PARAM_HISTORY* >
   {
   public:
      // destructor cleans up any allocated structures
//     ~ParamHistoryArray( void ) { for ( int i=0; i < GetSize(); i++ ) delete GetAt( i ); }      
   };



class optimizeDlg : public CDialog
{
// Construction
public:
	optimizeDlg(CWnd* pParent = NULL);   // standard constructor
	void Optimize(); 
   ParamHistoryArray m_paramHistoryArray;
	void UpdatePlot();
// Dialog Data
	//{{AFX_DATA(optimizeDlg)
	enum { IDD = IDD_OPTIMIZE };
	CEdit	m_perturbpercent;
	CRichEditCtrl	m_outputfile;
	CStatic	m_progressbox;
	float	m_maxis;
	float	m_maxks;
	float	m_maxm;
	float	m_minis;
	float	m_minks;
	float	m_minm;
	float	m_minn;
	BOOL	m_checkips;
	BOOL	m_checkksat;
	BOOL	m_checkm;
	BOOL	m_checkn;
	float	m_optlength;
	float	m_opttimestep;
	float	m_ks;
	float	m_m;
	float	m_maxn;
	float	m_n;
	float	m_is;
	float	m_isopt;
	float	m_ksopt;
	float	m_mopt;
	float	m_nopt;
	float	m_iclength;
	float	m_kdepth;
	BOOL	m_checket;
	BOOL	m_checkkdepth;
	BOOL	m_checksoildepth;
	float	m_et;
	float	m_etopt;
	float	m_kdepthopt;
	float	m_maxet;
	float	m_maxkdepth;
	float	m_maxsoildepth;
	float	m_minet;
	float	m_minkdepth;
	float	m_minsoildepth;
	float	m_soildepth;
	float	m_soildepthopt;
	float	m_lamda;
	float	m_lamdafactor;
	CChart2D	m_2dchart;
	BOOL	m_checksimulationstudy;
	CString	m_simulationtext;
	float	m_perturbpercentvalue;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(optimizeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
	float m_chisquare;
	ParamInfoArray m_paramInfoArray;

	int Create2DPlot(float **output, int ndata, bool better);
	void OnTimer(UINT nIDEvent) ;

	
protected:
	//void calculateJacobianRow( float *dQda );

	// Generated message map functions
	//{{AFX_MSG(optimizeDlg)
	virtual void OnOK();
	afx_msg void OnOpt();
	afx_msg void OnRun();
	afx_msg void OnOtheroutput();
	afx_msg void OnPerturbby();
	afx_msg void OnSimulationstudy();
	afx_msg void OnChecksimulationstudy();
	afx_msg void OnCancel2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIMIZEDLG_H__B9EDC2B1_80A7_45DC_99D2_17336ED47C10__INCLUDED_)
