// ModelDefinition.cpp : implementation file
//

#include "stdafx.h"
#include "wet_h.h"
#include "ModelDefinition.h"
#include "ppModelDefHorizontal.h"
#include "ppModelDefVertical.h"
#include "ppModelDefChannel.h"
#include "WET_Hydro.h"

extern ModelDefinition *gpModelDefinition;
extern WET_Hydro       *gpHydroModel;

// ModelDefinition

IMPLEMENT_DYNAMIC(ModelDefinition, CPropertySheet)
ModelDefinition::ModelDefinition(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

ModelDefinition::ModelDefinition(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
   	gpModelDefinition = this;


	AddPage( &m_ppModelDefHorizontal);
	AddPage( &m_ppModelDefVertical );
	AddPage( &m_ppModelDefChannel );
}

ModelDefinition::~ModelDefinition()
{
}


BEGIN_MESSAGE_MAP(ModelDefinition, CPropertySheet)
END_MESSAGE_MAP()


// ModelDefinition message handlers

BOOL ModelDefinition::OnInitDialog()
{

	m_bModeless = FALSE;
	BOOL bResult = CPropertySheet::OnInitDialog();
   m_bModeless = TRUE;
	int numPages = GetPageCount();
	for (int i=0; i< numPages; i++)
		SetActivePage(i);
	return bResult;
}

BOOL ModelDefinition::OnCommand(WPARAM wParam, LPARAM lParam)
{
   switch( LOWORD( wParam ) )
      {
      case IDOK:  // map properties
			{
         m_ppModelDefVertical.UpdateData(TRUE);
         m_ppModelDefHorizontal.UpdateData(TRUE);
         m_ppModelDefChannel.UpdateData(TRUE);
         gpHydroModel->m_checkEtModel=m_ppModelDefVertical.m_checkET;
         gpHydroModel->m_checkInterception=m_ppModelDefVertical.m_simulateInterception;
         gpHydroModel->m_checkUnsaturated=m_ppModelDefVertical.m_checkUnsat;
         gpHydroModel->m_checkSnow = m_ppModelDefVertical.m_checkSnow;
         gpHydroModel->m_checkGridMet = m_ppModelDefVertical.m_checkGridMet;
         gpHydroModel->m_etMethod = m_ppModelDefVertical.m_radioET;
         gpHydroModel->m_unsatMethod = m_ppModelDefVertical.m_radioUnsat;
         gpHydroModel->m_satMethod = m_ppModelDefVertical.m_radioSat;
         gpHydroModel->m_widthBand = m_ppModelDefHorizontal.m_bandWidth;
         gpHydroModel->m_numBands = (int)m_ppModelDefHorizontal.m_numberBands;
         gpHydroModel->m_checkSideOfStream = m_ppModelDefHorizontal.m_independentSides;
         gpHydroModel->m_modelStructureMethod = m_ppModelDefHorizontal.m_radioModelStructure;
         gpHydroModel->m_modelParamMethod = m_ppModelDefHorizontal.m_radioModelParameters;
         gpHydroModel->m_radioPorosity = m_ppModelDefVertical.m_radioPorosityInt;
         //gpHydroModel->m_drainShape = m_ppModelDefVertical.m_drainShape;
         gpHydroModel->m_routeChannelWaterMethod =  m_ppModelDefChannel.m_radioInstreamRoutingMethod;
         gpHydroModel->m_routeChannelSolutesMethod= m_ppModelDefChannel.m_radioInstreamSolutes;
         
			}
			ShowWindow(SW_HIDE);

         break;

      case IDCANCEL:
			{
			ShowWindow(SW_HIDE);
			}
			break;
      case 12321:
			{


			}
			break;
	}
	
//	return CPropertySheet::OnCommand(wParam, lParam);
	return true;	

   //return CPropertySheet::OnCommand(wParam, lParam);
}
