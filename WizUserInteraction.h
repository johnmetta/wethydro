#if !defined(AFX_WIZUSERINTERACTION_H__DD469C2A_C4A6_4CE2_879F_C3EB7FDE82B1__INCLUDED_)
#define AFX_WIZUSERINTERACTION_H__DD469C2A_C4A6_4CE2_879F_C3EB7FDE82B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WizUserInteraction.h : header file
//
#include "modelinteraction.h"
//#include "ModelEvaluation.h"
#include "interfaceinteraction.h"
#include "PPUserInteractionStucture.h"
#include "PPUserInteractionSimControl.h"
#include "PPUserInteractionTracer.h"
#include "PPUserInteractionSediment.h"
#include "PPUserInteractionET.h"
#include "PPLanduseChange.h"
#include "PPUserInteractionFlowMods.h"
#include "ppUserInteractionPesticide.h"
/////////////////////////////////////////////////////////////////////////////
// WizUserInteraction

class WizUserInteraction : public CPropertySheet
{
	DECLARE_DYNAMIC(WizUserInteraction)

// Construction
public:
	WizUserInteraction(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	WizUserInteraction(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
   
  // ModelEvaluation m_modelEvaluation;
	modelinteraction m_modelInteractionPage; 
	interfaceinteraction m_interfaceInteractionPage;
	PPUserInteractionStucture m_ppUserInteractionStructure;
	PPUserInteractionSimControl m_ppUserInteractionSimControl;
	PPUserInteractionTracer m_ppUserInteractionTracer;
	PPUserInteractionET m_ppUserInteractionET;
	PPUserInteractionSediment m_ppUserInteractionSediment;
	PPLanduseChange m_ppLanduseChange;
   PPUserInteractionFlowMods m_ppUserInteractionFlowMod;
   ppUserInteractionPesticide m_ppUserInteractionPesticide;

   float m_latitude;

	float m_ac ;
	float m_airDensity ;
	float m_bc ;
	float m_bioGeoAbsorp ;
	float m_extraTerrRadiation ;
	float m_clearFraction ;
	float m_cloudFraction ;
	float m_gradientSatVapor ;
	float m_heatCond ;
	float m_heightOfHumid ;
	float m_heightOfWind ;
	float m_shortWave ;
	float m_psychrometric ;
	float m_relativeHumid ;
	float m_satVapPres ;
	float m_stephanBoltz ;
	float m_specificHeat ;
	float m_storedE ;
	float m_windSpeed ;


// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(WizUserInteraction)
	public:
	virtual BOOL OnInitDialog();
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

void OnIDOK();

// Implementation
public:
	virtual ~WizUserInteraction();

	// Generated message map functions
protected:
	//{{AFX_MSG(WizUserInteraction)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIZUSERINTERACTION_H__DD469C2A_C4A6_4CE2_879F_C3EB7FDE82B1__INCLUDED_)
