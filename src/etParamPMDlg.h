#if !defined(AFX_etParamPMDlg_H__8C325251_5F52_459E_8A09_467E7A897403__INCLUDED_)
#define AFX_etParamPMDlg_H__8C325251_5F52_459E_8A09_467E7A897403__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// etParamPMDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// etParamPMDlg dialog

class etParamPMDlg : public CDialog
{
// Construction
public:
	etParamPMDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(etParamPMDlg)
	enum { IDD = IDD_ETPARAMPMDLG };
	float	m_ac;
	float	m_airDensity;
	float	m_bc;
	float	m_bioGeoAbsorp;
	float	m_extraTerrRadiation;
	float	m_clearFraction;
	float	m_cloudFraction;
	float	m_gradientSatVapor;
	float	m_heatCond;
	float	m_heightOfHumid;
	float	m_heightOfWind;
	float	m_shortWave;
	float	m_latentHeatEvap;
	float	m_psychrometric;
	float	m_relativeHumid;
	float	m_satVapPres;
	float	m_stephanBoltz;
	float	m_specificHeat;
	float	m_storedE;
	float	m_windSpeed;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(etParamPMDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(etParamPMDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_etParamPMDlg_H__8C325251_5F52_459E_8A09_467E7A897403__INCLUDED_)
