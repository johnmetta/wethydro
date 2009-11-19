// Restoration.cpp : implementation file
//

#include "stdafx.h"
#include "wet_h.h"
#include "Restoration.h"
#include "ppUserInteractionSediment.h"
#include "ppLanduseChange.h"
#include "ppUserInteractionFlowMods.h"
extern Restoration *gpRestoration;


// Restoration

IMPLEMENT_DYNAMIC(Restoration, CPropertySheet)
Restoration::Restoration(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

Restoration::Restoration(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
   gpRestoration = this;
   AddPage( &m_ppUserInteractionSediment );
	AddPage( &m_ppLanduseChange );
	AddPage( &m_ppUserInteractionFlowMod );
}

Restoration::~Restoration()
{
}


BEGIN_MESSAGE_MAP(Restoration, CPropertySheet)
END_MESSAGE_MAP()


// Restoration message handlers

BOOL Restoration::OnInitDialog()
   {
   m_bModeless = FALSE;
	BOOL bResult = CPropertySheet::OnInitDialog();
   m_bModeless = TRUE;
	int numPages = GetPageCount();
	for (int i=0; i< numPages; i++)
		SetActivePage(i);
   return bResult;
   }

BOOL Restoration::OnCommand(WPARAM wParam, LPARAM lParam)
{
   switch( LOWORD( wParam ) )
      {
      case IDOK:  // map properties
			{

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
