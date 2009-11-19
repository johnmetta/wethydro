#pragma once

#include "ppParameterValues.h"
#include "ppModelEvaluation.h"
#include <randgen\randunif.hpp>
#include <randgen\randLN.hpp>
#include <randgen\randnorm.hpp>


// ModelEvaluation

class ModelEvaluation : public CPropertySheet
{
	DECLARE_DYNAMIC(ModelEvaluation)

public:
   RandUniform m_randUnif1;
   RandUniform m_randUnif2;
   RandUniform m_randUnif3;
   RandUniform m_randUnif4;
   RandUniform m_randUnif5;
   RandUniform m_randUnif6;


	ModelEvaluation(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	ModelEvaluation(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
   ppParameterValues m_ppModelEvalParameterValues;
	ppModelEvaluation m_ppModelEvalModelEvaluation;

	virtual ~ModelEvaluation();

protected:
	DECLARE_MESSAGE_MAP()
public:
   virtual BOOL OnInitDialog();
protected:
   virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
   // Sample parameters from their distributional definitions.  The sampled value is stored in the 
   int SampleParameters(void);
   int CopyParamsToHydrologicModel(int runNumber);
};


