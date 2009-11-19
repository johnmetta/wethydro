// Extensions.cpp : implementation file
//

#include "stdafx.h"
#include "wet_h.h"
#include "Extensions.h"
#include "ppUserInteractionTracer.h"
#include "ppUserInteractionPesticide.h"
#include "WET_Hydro.h"
#include "ModelDefinition.h"

extern WET_Hydro *gpHydroModel;
extern Extensions *gpExtensions;
extern ModelDefinition *gpModelDefinition;

// Extensions

IMPLEMENT_DYNAMIC(Extensions, CPropertySheet)
Extensions::Extensions(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

Extensions::Extensions(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
   gpExtensions = this;
   AddPage( &m_ppUserInteractionTracer );
   AddPage( &m_ppUserInteractionPesticide );	



}

Extensions::~Extensions()
{
}


BEGIN_MESSAGE_MAP(Extensions, CPropertySheet)
END_MESSAGE_MAP()


// Extensions message handlers

BOOL Extensions::OnInitDialog()
   {
   m_bModeless = FALSE;
	BOOL bResult = CPropertySheet::OnInitDialog();
   m_bModeless = TRUE;
	int numPages = GetPageCount();
	for (int i=0; i< numPages; i++)
		SetActivePage(i);
   return bResult;
   }

BOOL Extensions::OnCommand(WPARAM wParam, LPARAM lParam)
   {
   switch( LOWORD( wParam ) )
      {
      case IDOK:  // map properties
			{
         m_ppUserInteractionTracer.UpdateData(TRUE);
         m_ppUserInteractionPesticide.UpdateData(TRUE);
         gpHydroModel->m_editTracerMgL = m_ppUserInteractionTracer.m_editTracerMgL;
         gpHydroModel->m_checkTracerModel = m_ppUserInteractionTracer.m_checkTracerModel;
         gpHydroModel->m_radioTracerInput = m_ppUserInteractionTracer.m_radioTracerInput;
         gpHydroModel->m_initTracerConc = m_ppUserInteractionTracer.m_initTracerConc;
         

         m_ppUserInteractionPesticide.UpdateModelState();
         gpHydroModel->m_checkPesticide = m_ppUserInteractionPesticide.m_checkPesticide;
         if (m_ppUserInteractionPesticide.m_checkPesticide)
            {
            if (gpHydroModel->m_checkUnsaturated != TRUE)
               {
               CString msg("The pesticide model requires explicit calculation of unsaturated zone water.  Resetting Now!");
               MessageBox(msg);


               gpHydroModel->m_checkUnsaturated = TRUE;
               gpModelDefinition->m_ppModelDefVertical.m_checkUnsat=TRUE;
               gpModelDefinition->m_ppModelDefVertical.UpdateData(FALSE);
               }
            }
         gpHydroModel->m_numberPesticides = m_ppUserInteractionPesticide.m_numberPesticides;

         for (int i=0; i< m_ppUserInteractionPesticide.m_pesticideOffsets.GetSize();i++)
            {
            int offset = m_ppUserInteractionPesticide.m_pesticideOffsets[i];
            gpHydroModel->m_pesticideOffsets.Add(offset);
            }

         m_ppUserInteractionTracer.SetModified(false);
         m_ppUserInteractionPesticide.SetModified(false);
			ShowWindow(SW_HIDE);
         }
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
   return true;
   // return CPropertySheet::OnNotify(wParam, lParam, pResult);
   }
