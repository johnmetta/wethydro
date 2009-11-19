#pragma once

#include "ppModelDefHorizontal.h"
#include "ppModelDefVertical.h"
#include "ppModelDefChannel.h"

// ModelDefinition

class ModelDefinition : public CPropertySheet
{
	DECLARE_DYNAMIC(ModelDefinition)

public:
	ModelDefinition(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	ModelDefinition(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	ppModelDefHorizontal m_ppModelDefHorizontal;
	ppModelDefVertical m_ppModelDefVertical;
	ppModelDefChannel m_ppModelDefChannel;
   
   virtual ~ModelDefinition();

protected:
	DECLARE_MESSAGE_MAP()
public:
   virtual BOOL OnInitDialog();
protected:
   virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};


