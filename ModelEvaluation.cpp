// ModelEvaluation.cpp : implementation file
//

#include "stdafx.h"
#include "wet_h.h"
#include "ModelEvaluation.h"
#include "ppParameterValues.h"
#include "ppModelEvaluation.h"
#include "WET_Hydro.h"

extern ModelEvaluation *gpModelEvaluation;
extern WET_Hydro  *gpHydroModel;

// ModelEvaluation

IMPLEMENT_DYNAMIC(ModelEvaluation, CPropertySheet)
ModelEvaluation::ModelEvaluation(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

ModelEvaluation::ModelEvaluation(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage),
   m_randUnif1(),
   m_randUnif2(),
   m_randUnif3(),
   m_randUnif4(),
   m_randUnif5(),
   m_randUnif6()

{
   gpModelEvaluation = this;
	AddPage( &m_ppModelEvalParameterValues);
	AddPage( &m_ppModelEvalModelEvaluation );
   
}

ModelEvaluation::~ModelEvaluation()
{
}


BEGIN_MESSAGE_MAP(ModelEvaluation, CPropertySheet)
END_MESSAGE_MAP()


// ModelEvaluation message handlers

BOOL ModelEvaluation::OnInitDialog()
   {
	m_bModeless = FALSE;
	BOOL bResult = CPropertySheet::OnInitDialog();
   m_bModeless = TRUE;
	int numPages = GetPageCount();
	for (int i=0; i< numPages; i++)
		SetActivePage(i);
   m_ppModelEvalModelEvaluation.m_checkKSat=gpHydroModel->m_checkCKsat;
   m_ppModelEvalModelEvaluation.m_checkFieldCapac=gpHydroModel->m_checkCFC;
   m_ppModelEvalModelEvaluation.m_checkInitSat=gpHydroModel->m_checkCInit;
   m_ppModelEvalModelEvaluation.m_checkK1=gpHydroModel->m_checkCK1;
   m_ppModelEvalModelEvaluation.m_checkKDepth=gpHydroModel->m_checkCKD;
   m_ppModelEvalModelEvaluation.m_checkPhi=gpHydroModel->m_checkCPhi;
   m_ppModelEvalModelEvaluation.m_checkSoilDepth=gpHydroModel->m_checkCSD;
   m_ppModelEvalModelEvaluation.m_checkVanGenuctN=gpHydroModel->m_checkCVan;
   m_ppModelEvalModelEvaluation.m_checkCEq.SetState(gpHydroModel->m_checkCEq);
   m_ppModelEvalModelEvaluation.m_checkWP.SetState(gpHydroModel->m_checkCWD);


   return bResult;
   }

BOOL ModelEvaluation::OnCommand(WPARAM wParam, LPARAM lParam)
   {
   switch( LOWORD( wParam ) )
      {
      case IDOK:  // map properties
         {
         m_ppModelEvalModelEvaluation.UpdateData(TRUE);
         gpHydroModel->m_numIterations = m_ppModelEvalModelEvaluation.m_numIterations;
         gpHydroModel->m_checkMonteCarlo = m_ppModelEvalModelEvaluation.m_singleModelRun;
         
         for (int i=0; i < m_ppModelEvalParameterValues.m_allParameterArray.GetSize(); i++)
            { 
            ALL_PARAM_INFO *pRandom = m_ppModelEvalParameterValues.m_allParameterArray[i];
            for (int j=0;j<pRandom->randomParameterArray.GetSize();j++) 
               {                  
               if (m_ppModelEvalModelEvaluation.m_checkCInit.GetState())
                  {
                  if (pRandom->randomParameterArray[j]->paramName=="initSat")
                     pRandom->randomParameterArray[j]->sampleParam=true;                  
                  }
               else
                  {
                  if (pRandom->randomParameterArray[j]->paramName=="initSat")
                     pRandom->randomParameterArray[j]->sampleParam=false;
                  }

               if (m_ppModelEvalModelEvaluation.m_checkCKsat.GetState())
                  {
                  if (pRandom->randomParameterArray[j]->paramName=="kSat")
                     pRandom->randomParameterArray[j]->sampleParam=true;
                  }
               else
                  {
                  if (pRandom->randomParameterArray[j]->paramName=="kSat")
                     pRandom->randomParameterArray[j]->sampleParam=false;
                  }
               if (m_ppModelEvalModelEvaluation.m_checkCM.GetState())
                  {
                  if (pRandom->randomParameterArray[j]->paramName=="m")
                     pRandom->randomParameterArray[j]->sampleParam=true;
                  }
               else
                  {
                  if (pRandom->randomParameterArray[j]->paramName=="m")
                     pRandom->randomParameterArray[j]->sampleParam=false;
                  }
               if (m_ppModelEvalModelEvaluation.m_checkCFC.GetState())
                  {
                  if (pRandom->randomParameterArray[j]->paramName=="fieldCapac")
                     pRandom->randomParameterArray[j]->sampleParam=true;
                  }
               else
                  {
                  if (pRandom->randomParameterArray[j]->paramName=="fieldCapac")
                     pRandom->randomParameterArray[j]->sampleParam=false;
                  }
               if (m_ppModelEvalModelEvaluation.m_checkCK1.GetState())
                  {
                  if (pRandom->randomParameterArray[j]->paramName=="thetaTrace")
                     pRandom->randomParameterArray[j]->sampleParam=true;
                  }
               else
                  {
                  if (pRandom->randomParameterArray[j]->paramName=="thetaTrace")
                     pRandom->randomParameterArray[j]->sampleParam=false;
                  }
               if (m_ppModelEvalModelEvaluation.m_checkCK2.GetState())
                  {
                  if (pRandom->randomParameterArray[j]->paramName=="k2")
                     pRandom->randomParameterArray[j]->sampleParam=true;
                  }
               else
                  {
                  if (pRandom->randomParameterArray[j]->paramName=="k2")
                     pRandom->randomParameterArray[j]->sampleParam=false;
                  }
               if (m_ppModelEvalModelEvaluation.m_checkCKD.GetState())
                  {
                  if (pRandom->randomParameterArray[j]->paramName=="kDepth")
                     pRandom->randomParameterArray[j]->sampleParam=true;
                  }
               else
                  {
                  if (pRandom->randomParameterArray[j]->paramName=="kDepth")
                     pRandom->randomParameterArray[j]->sampleParam=false;
                  }
               if (m_ppModelEvalModelEvaluation.m_checkCPhi.GetState())
                  {
                  if (pRandom->randomParameterArray[j]->paramName=="phi")
                     pRandom->randomParameterArray[j]->sampleParam=true;
                  }
               else
                  {
                  if (pRandom->randomParameterArray[j]->paramName=="phi")
                     pRandom->randomParameterArray[j]->sampleParam=false;
                  }
               if (m_ppModelEvalModelEvaluation.m_checkCSD.GetState())
                  {
                  if (pRandom->randomParameterArray[j]->paramName=="soilDepth")
                     pRandom->randomParameterArray[j]->sampleParam=true;
                  }
               else
                  {
                  if (pRandom->randomParameterArray[j]->paramName=="soilDepth")
                     pRandom->randomParameterArray[j]->sampleParam=false;
                  }
               if (m_ppModelEvalModelEvaluation.m_checkCVan.GetState())
                  {
                  if (pRandom->randomParameterArray[j]->paramName=="vanGenN")
                     pRandom->randomParameterArray[j]->sampleParam=true;
                  }
               else
                  {
                  if (pRandom->randomParameterArray[j]->paramName=="vanGenN")
                     pRandom->randomParameterArray[j]->sampleParam=false;
                  }
               if (m_ppModelEvalModelEvaluation.m_checkWP.GetState())
                  {
                  if (pRandom->randomParameterArray[j]->paramName=="wiltPt")
                     pRandom->randomParameterArray[j]->sampleParam=true;
                  }
               else
                  {
                  if (pRandom->randomParameterArray[j]->paramName=="wiltPt")
                     pRandom->randomParameterArray[j]->sampleParam=false;
                  }

               if (m_ppModelEvalModelEvaluation.m_checkPLE2.GetState())
                  {
                  if (pRandom->randomParameterArray[j]->paramName=="powerLExp")
                     pRandom->randomParameterArray[j]->sampleParam=true;
                  }
               else
                  {
                  if (pRandom->randomParameterArray[j]->paramName=="powerLExp")
                     pRandom->randomParameterArray[j]->sampleParam=false;
                  }
               }
            }

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

  // return CPropertySheet::OnCommand(wParam, lParam);
   return true;
}


      // Sample parameters from their distributional definitions.  The sampled value is stored in the 
      //  ALL_PARAM_INFO struc in ppParameterValues.cpp
      //  We only sample parameters that are checked in the ppModelEvaluation
int ModelEvaluation::SampleParameters(void)
   { 
   ALL_PARAM_INFO *pRandom = m_ppModelEvalParameterValues.m_allParameterArray[0];//pRandom represents a single place (shallow, medium,etc)

    for (int i=0;i<pRandom->randomParameterArray.GetSize();i++) //for each parameter
        {
        if (pRandom->randomParameterArray[i]->sampleParam) // if it is selected as participating in Monte Carlo
			{
			//if (pRandom->randomParameterArray[i]->distNum==0) //the zero indicates a uniform distribution
			pRandom->randomParameterArray[i]->currentValue = (float)m_randUnif1.RandValue(pRandom->randomParameterArray[i]->minValue,pRandom->randomParameterArray[i]->maxValue);
			// pRandom->randomParameterArray[i]->currentValue = randomValue;
			}
        else
			pRandom->randomParameterArray[i]->currentValue = pRandom->randomParameterArray[i]->currentValue;
		}


   for (int paramType=1; paramType < m_ppModelEvalParameterValues.m_allParameterArray.GetSize(); paramType++)//for all places
      { 
      ALL_PARAM_INFO *pRandomToSet = m_ppModelEvalParameterValues.m_allParameterArray[paramType]; 
      for (int i=0;i<pRandomToSet->randomParameterArray.GetSize();i++) //for each parameter
         {
         if (pRandomToSet->randomParameterArray[i]->sampleParam) // if it is selected as participating in Monte Carlo
            {
            //if (pRandom->randomParameterArray[i]->distNum==0) //the zero indicates a uniform distribution
            pRandomToSet->randomParameterArray[i]->currentValue = pRandom->randomParameterArray[i]->currentValue;
			}
         else
            pRandomToSet->randomParameterArray[i]->currentValue = pRandomToSet->randomParameterArray[i]->currentValue;
         }
      }



/*
  // uncomment above code and use this to sample every place independently...
   for (int paramType=0; paramType < m_ppModelEvalParameterValues.m_allParameterArray.GetSize(); paramType++)//for all places
      { 
      ALL_PARAM_INFO *pRandom = m_ppModelEvalParameterValues.m_allParameterArray[paramType]; 
      for (int i=0;i<pRandom->randomParameterArray.GetSize();i++) //for each parameter
         {
         if (pRandom->randomParameterArray[i]->sampleParam) // if it is selected as participating in Monte Carlo
            {
            //if (pRandom->randomParameterArray[i]->distNum==0) //the zero indicates a uniform distribution
            pRandom->randomParameterArray[i]->currentValue = (float)m_randUnif1.RandValue(pRandom->randomParameterArray[i]->minValue,pRandom->randomParameterArray[i]->maxValue);
           // pRandom->randomParameterArray[i]->currentValue = randomValue;
			}
         else
            pRandom->randomParameterArray[i]->currentValue = pRandom->randomParameterArray[i]->currentValue;
         }
      }*/
   return 0;
   }
//  The parameter database must be copied to the WET_Hydro object.  The database is maintained separately 
//  as part of the hydrology model in order to keep the interface/model independent.
//  Also, the ppParameterDatabase knows about distributional parameters for each model parameter.
//  The hydrologic model does NOT know about these distributions.  We must sample the distribution, add that values
//  to the ppParameterValue structure (as currentValue), then copy that to the hydrology model.  Only the 
//  last step occurs here.

int ModelEvaluation::CopyParamsToHydrologicModel( int runNumber)
   {
   if ( runNumber==0)
   {
   //m_hydrologicParamInfoArray is made up of stuctures.  One structure for each set of parameters.
   //eg.  HILLSLOPE / RIPARIAN areas may have separate parameterizations.
   //gpHydroModel->m_hydrologicParamInfoArray.SetSize(m_ppModelEvalParameterValues.m_allParameterArray.GetSize());
      for (int i=0;i<m_ppModelEvalParameterValues.m_allParameterArray.GetSize();i++)
         {
         ALL_PARAM_INFO *pParam = m_ppModelEvalParameterValues.m_allParameterArray.GetAt(i);
         if (pParam->landscapeType != "Instream")
            {
            HYDROLOGIC_PARAM_INFO *pParamHydro = new HYDROLOGIC_PARAM_INFO;    
            pParamHydro->landscapeType = pParam->landscapeType;
            pParamHydro->fieldCapac = pParam->randomParameterArray[0]->currentValue;
            pParamHydro->initSat    = pParam->randomParameterArray[1]->currentValue;
            pParamHydro->thetaTrace = pParam->randomParameterArray[2]->currentValue;
            pParamHydro->k2    = pParam->randomParameterArray[3]->currentValue;
            pParamHydro->kDepth = pParam->randomParameterArray[4]->currentValue;
            pParamHydro->kSat    = pParam->randomParameterArray[5]->currentValue;
            pParamHydro->m = pParam->randomParameterArray[6]->currentValue;
            pParamHydro->phi= pParam->randomParameterArray[7]->currentValue;
            pParamHydro->soilDepth    = pParam->randomParameterArray[8]->currentValue;
            pParamHydro->vanGenN = pParam->randomParameterArray[9]->currentValue;
            pParamHydro->wiltPt = pParam->randomParameterArray[10]->currentValue;
            pParamHydro->powerLExp = pParam->randomParameterArray[11]->currentValue;
            gpHydroModel->m_hydrologicParamInfoArray.Add(pParamHydro);

            }
         else
            {
            INSTREAM_HYDRO_PARAM_INFO *pParamIHydro = new INSTREAM_HYDRO_PARAM_INFO;    
            pParamIHydro->landscapeType = pParam->landscapeType;
            pParamIHydro->n    = pParam->randomParameterArray[0]->currentValue;
            pParamIHydro->wd    = pParam->randomParameterArray[1]->currentValue;
            gpHydroModel->m_instreamHydroParamInfoArray.Add(pParamIHydro);
            }

         
         }
      }
   else //we are in the middle of a monte carlo run and want to simply replace parameter values
      {
      int count=0;
      for (int i=0;i<m_ppModelEvalParameterValues.m_allParameterArray.GetSize();i++)
         {
         ALL_PARAM_INFO *pParam = m_ppModelEvalParameterValues.m_allParameterArray.GetAt(i);
         if (pParam->landscapeType != "Instream")
            {
 
            gpHydroModel->m_hydrologicParamInfoArray[count]->landscapeType = pParam->landscapeType;
            gpHydroModel->m_hydrologicParamInfoArray[count]->fieldCapac = pParam->randomParameterArray[0]->currentValue;
            gpHydroModel->m_hydrologicParamInfoArray[count]->initSat    = pParam->randomParameterArray[1]->currentValue;
            gpHydroModel->m_hydrologicParamInfoArray[count]->thetaTrace = pParam->randomParameterArray[2]->currentValue;
            gpHydroModel->m_hydrologicParamInfoArray[count]->k2    = pParam->randomParameterArray[3]->currentValue;
            gpHydroModel->m_hydrologicParamInfoArray[count]->kDepth = pParam->randomParameterArray[4]->currentValue;
            gpHydroModel->m_hydrologicParamInfoArray[count]->kSat    = pParam->randomParameterArray[5]->currentValue;
            gpHydroModel->m_hydrologicParamInfoArray[count]->m = pParam->randomParameterArray[6]->currentValue;
            gpHydroModel->m_hydrologicParamInfoArray[count]->phi= pParam->randomParameterArray[7]->currentValue;
            gpHydroModel->m_hydrologicParamInfoArray[count]->soilDepth    = pParam->randomParameterArray[8]->currentValue;
            gpHydroModel->m_hydrologicParamInfoArray[count]->vanGenN = pParam->randomParameterArray[9]->currentValue;
            gpHydroModel->m_hydrologicParamInfoArray[count]->wiltPt = pParam->randomParameterArray[10]->currentValue;
            gpHydroModel->m_hydrologicParamInfoArray[count]->powerLExp = pParam->randomParameterArray[11]->currentValue;
            float crap = gpHydroModel->m_hydrologicParamInfoArray[count]->kSat;
            count++;

            }
         else
            {
             
            gpHydroModel->m_instreamHydroParamInfoArray[0]->landscapeType = pParam->landscapeType;
            gpHydroModel->m_instreamHydroParamInfoArray[0]->n    = pParam->randomParameterArray[0]->currentValue;
            gpHydroModel->m_instreamHydroParamInfoArray[0]->wd   = pParam->randomParameterArray[1]->currentValue;
            
            } 
         }
      }


   return 0;
   }

