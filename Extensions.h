#pragma once
#include "ppUserInteractionTracer.h"
#include "ppUserInteractionPesticide.h"



// Extensions

class Extensions : public CPropertySheet
{
	DECLARE_DYNAMIC(Extensions)

public:
	Extensions(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	Extensions(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
   PPUserInteractionTracer     m_ppUserInteractionTracer ;
   ppUserInteractionPesticide  m_ppUserInteractionPesticide ;	

	virtual ~Extensions();

protected:
	DECLARE_MESSAGE_MAP()
public:
   virtual BOOL OnInitDialog();
protected:
   virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};


