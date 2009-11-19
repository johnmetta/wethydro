// ppParameterValues.cpp : implementation file
//

#include "stdafx.h"
#include "wet_h.h"
#include "ppParameterValues.h"
#include "WET_Hydro.h"
#include "ModelEvaluation.h"
#include ".\ppparametervalues.h"
extern WET_Hydro *gpHydroModel;
//extern modelEvaluation *gpModelEvaluation;
extern ppParameterValues *gpParameterValues;

// ppParameterValues dialog

IMPLEMENT_DYNAMIC(ppParameterValues, CPropertyPage)
ppParameterValues::ppParameterValues()
	: CPropertyPage(ppParameterValues::IDD),
   m_currentLandscape(0),
   m_LandscapeType("Semi-Distributed")
{
   gpParameterValues = this;
}

ppParameterValues::~ppParameterValues()
{
}

void ppParameterValues::DoDataExchange(CDataExchange* pDX)
{
   CPropertyPage::DoDataExchange(pDX);
   //	DDX_Control(pDX, IDC_LIST1, m_parameterList);
   DDX_Control(pDX, IDC_LIST1, m_parameterList);

   DDX_Control(pDX, IDC_BUTTON1, m_downLandscapeList);
   DDX_Control(pDX, IDC_BUTTON2, m_upLandscapeList);
   DDX_Text(pDX, IDC_STATIC3, m_LandscapeType);
}


BEGIN_MESSAGE_MAP(ppParameterValues, CPropertyPage)
 //  ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST2, OnLvnItemchangedList2)
// ON_LBN_SELCHANGE(IDC_LIST1, OnLbnSelchangeList1)
 ON_BN_CLICKED(IDC_CHECK1, OnBnClickedCheck1)

 ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
 ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
 ON_BN_CLICKED(IDC_REREADPARAM, OnBnClickedRereadparam)
END_MESSAGE_MAP()


// ppParameterValues message handlers

void ppParameterValues::OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult)
{
   LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
   // TODO: Add your control notification handler code here
   *pResult = 0;
}

BOOL ppParameterValues::OnInitDialog()
   {
	CPropertyPage::OnInitDialog();
  

   m_parameterList.InsertColumn(0,"ParamName");
   m_parameterList.InsertColumn(1,"CurrentValue");
   m_parameterList.InsertColumn(2,"MaxValue");
   m_parameterList.InsertColumn(3,"MinValue");
   m_parameterList.InsertColumn(4,"StdValue");

   CString fileName("c:\\temp\\hydroParam.xls");
   CString a("Semi-Distributed");
   CString b("Instream");
   CString c("SHALLOW");
   CString d("MEDIUM");
   CString e("Riparian");
   CString f("Earthflows");
   CString g("Volcanics");
   CString h("Upland");
   CString i("DEEP");

   ReadCoefData( fileName, a);
   ReadCoefData( fileName, b);
   ReadCoefData( fileName, c);
   ReadCoefData( fileName, d);
   ReadCoefData( fileName, e);
   ReadCoefData( fileName, f);
   ReadCoefData( fileName, g);
   ReadCoefData( fileName, h);
   ReadCoefData( fileName, i);
   m_parameterList.SetExtendedStyle(/*LVS_EX_CHECKBOXES*/LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
   AddDataToListControl(a, 0);

  // m_LandscapeType = m_allParameterArray[0]->landscapeType ;


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void ppParameterValues::OnLbnSelchangeList1()
{
   // TODO: Add your control notification handler code here
}

//


void ppParameterValues::OnBnClickedCheck1()
{
   // TODO: Add your control notification handler code here
}

//This will be called oninit, and also when the user clicks the scroll bar to move between parameterizations.
//oninit should just get the first ALL_PARAM_INFO in m_allParameterArray!

void ppParameterValues::AddDataToListControl(CString landscapeType, int offset)
   {
   m_parameterList.DeleteAllItems();

   for ( int i=0; i < m_allParameterArray[offset]->randomParameterArray.GetSize(); i++ )
      {
      m_parameterList.InsertItem(i,m_allParameterArray[offset]->randomParameterArray[i]->paramName);
      char buffer[10];   
      _gcvt(m_allParameterArray[offset]->randomParameterArray[i]->currentValue,2, buffer );      
      m_parameterList.SetItemText(i, 1, buffer);

      m_parameterList.SetColumnWidth(1,10);
      _gcvt(m_allParameterArray[offset]->randomParameterArray[i]->minValue,2, buffer );
      m_parameterList.SetItemText(i, 2, buffer);
      _gcvt(m_allParameterArray[offset]->randomParameterArray[i]->maxValue,2, buffer); 
      m_parameterList.SetItemText(i, 3, buffer);
      _gcvt(m_allParameterArray[offset]->randomParameterArray[i]->std,2, buffer );
      m_parameterList.SetItemText(i, 4, buffer);
      }

      for (int k=0;k<5;k++)
          m_parameterList.SetColumnWidth(k,70);

 

   }
int ppParameterValues::ReadCoefData( LPCSTR fileName, CString worksheet)
   {
   bool useWideChar = VData::m_useWideChar;
   VData::m_useWideChar = false;

   DbTable xls( DOT_VDATA );     // make an empty data table
   // get corresponding dbf file
   char *xlsName = new char[ lstrlen( fileName ) + 1 ];
   lstrcpy( xlsName, fileName ); //copy string to buffer
   
 //  here collect the worksheet name
   CString a("[");
   CString b("$]");
   a += worksheet;
   a += b;
  
   CString sql( "SELECT * FROM " );
   sql += a;

   int rows = xls.Load( xlsName, sql, FALSE, TRUE, "Excel 8.0;" );

   VData::m_useWideChar = useWideChar;

   if ( rows <= 0 )
      {
      CString msg( "Error loading file: " );
      msg += fileName;
      ErrorMsg( msg );
      return -1;
      }

	rows = xls.GetRecordCount();
   //get the labels from first row
 /*  for (int k=0;k<m_allParamInfoArray.GetSize(); k++)
      m_allParamInfoArray.RemoveAt(k);
   
   gpHydroModel->m_allParamInfoArray.RemoveAll();
  // gpHydroModel->m_chemicalParamInfoArray->SetSize( cols, 0 );  // resize matrix, statArray
*/


      ALL_PARAM_INFO *pParam = new ALL_PARAM_INFO;
      pParam->landscapeType = worksheet;
      m_allParameterArray.Add(pParam);
      for (int row=0; row < rows; row++ )
         {
         RANDOM_PARAMETER *pRandom = new RANDOM_PARAMETER;
         xls.GetData(row, 0, pRandom->paramName); 
         xls.GetData(row, 1, pRandom->currentValue); 
         xls.GetData(row, 2, pRandom->minValue);
         xls.GetData(row, 3, pRandom->maxValue); 
         xls.GetData(row, 4, pRandom->std);  
         m_allParameterArray[m_allParameterArray.GetSize()-1]->randomParameterArray.Add(pRandom);
         }
      delete [] xlsName;
      return -1;
   }


void ppParameterValues::OnBnClickedButton1()
   {
   if (m_currentLandscape > 0)
      m_currentLandscape--;
   m_LandscapeType=m_allParameterArray[m_currentLandscape]->landscapeType;
    UpdateData(false);
   AddDataToListControl(m_allParameterArray[m_currentLandscape]->landscapeType, m_currentLandscape);   // TODO: Add your control notification handler code here
   }
void ppParameterValues::OnBnClickedButton2()
   {
   if (m_currentLandscape < m_allParameterArray.GetSize()-1)
      m_currentLandscape++;
   m_LandscapeType = m_allParameterArray[m_currentLandscape]->landscapeType;
   UpdateData(false);
   AddDataToListControl(m_allParameterArray[m_currentLandscape]->landscapeType, m_currentLandscape);   // TODO: Add your control notification handler code here
   }

   void ppParameterValues::OnBnClickedRereadparam()
   {
      m_allParameterArray.RemoveAll();

   CString fileName("c:\\temp\\hydroParam.xls");
   CString a("Semi-Distributed");
   CString b("Instream");
   CString c("SHALLOW");
   CString d("MEDIUM");
   CString e("Riparian");
   CString f("Earthflows");
   CString g("Volcanics");
   CString h("Upland");
   CString i("DEEP");

   ReadCoefData( fileName, a);
   ReadCoefData( fileName, b);
   ReadCoefData( fileName, c);
   ReadCoefData( fileName, d);
   ReadCoefData( fileName, e);
   ReadCoefData( fileName, f);
   ReadCoefData( fileName, g);
   ReadCoefData( fileName, h);
   ReadCoefData( fileName, i);
   m_parameterList.SetExtendedStyle(/*LVS_EX_CHECKBOXES*/LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
   AddDataToListControl(a, 0);
   UpdateData(true);
   }
