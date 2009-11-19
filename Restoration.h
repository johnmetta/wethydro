#pragma once

#include "ppUserInteractionSediment.h"
#include "ppLanduseChange.h"
#include "ppUserInteractionFlowMods.h"


// Restoration

class Restoration : public CPropertySheet
{
	DECLARE_DYNAMIC(Restoration)

public:
	Restoration(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	Restoration(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
   PPUserInteractionSediment m_ppUserInteractionSediment ;
	PPLanduseChange m_ppLanduseChange ;
	PPUserInteractionFlowMods m_ppUserInteractionFlowMod ;
	virtual ~Restoration();

protected:
	DECLARE_MESSAGE_MAP()
public:
   virtual BOOL OnInitDialog();
protected:
   virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};


