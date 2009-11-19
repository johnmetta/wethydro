// ppUserInteractionPesticide.cpp : implementation file
//

#include "stdafx.h"
#include "wet_h.h"
#include "ppUserInteractionPesticide.h"
#include "WET_Hydro.h"
extern WET_Hydro *gpHydroModel;

// ppUserInteractionPesticide dialog

IMPLEMENT_DYNAMIC(ppUserInteractionPesticide, CPropertyPage)
ppUserInteractionPesticide::ppUserInteractionPesticide()
	: CPropertyPage(ppUserInteractionPesticide::IDD)
   , m_checkPesticide(gpHydroModel->m_checkPesticide)
   , m_appDatabaseName(_T(""))

{
}

ppUserInteractionPesticide::~ppUserInteractionPesticide()
{
}

void ppUserInteractionPesticide::DoDataExchange(CDataExchange* pDX)
{
   CPropertyPage::DoDataExchange(pDX);
   DDX_Check(pDX, IDC_CHECKPESTICIDE, m_checkPesticide);
   DDX_Text(pDX, IDC_EDITPESTAPPDATA, m_appDatabaseName);
   DDX_Control(pDX, IDC_LISTCHEMICAL, m_chemicalList);
}


BEGIN_MESSAGE_MAP(ppUserInteractionPesticide, CPropertyPage)
   ON_BN_CLICKED(IDC_CHECK1, OnBnClickedCheck1)
   ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
   ON_EN_CHANGE(IDC_EDIT1, OnEnChangeEdit1)
   ON_EN_CHANGE(IDC_EDITPESTAPPDATA, OnEnChangeEditpestappdata)
   ON_LBN_SELCHANGE(IDC_LISTCHEMICAL, OnLbnSelchangeListchemical)
   ON_BN_CLICKED(IDC_EDITPARAM, OnBnClickedEditparam)
   ON_BN_CLICKED(IDC_EDITAPP, OnBnClickedEditapp)
END_MESSAGE_MAP()


// ppUserInteractionPesticide message handlers

void ppUserInteractionPesticide::OnBnClickedCheck1()
{
   // TODO: Add your control notification handler code here
}

void ppUserInteractionPesticide::OnBnClickedButton1()
   {
   CFileDialog dlg( TRUE, "xls","application", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
   "Excel Files|*.xls|All files|*.*||");

   if ( dlg.DoModal() == IDOK )
      {
      CString filename( dlg.GetPathName() );
      m_appDatabaseName = filename;
 
      m_appDatabaseName.SetString(filename);

      
      for (int j=0; j< m_chemicalList.GetCount();j++)
         m_chemicalList.DeleteString(j);
      m_chemicalList.ResetContent();


      CWaitCursor c;
      CString worksheet = "Application";
      CString worksheet2 = "Parameters";
      ReadAppData(m_appDatabaseName, worksheet);
      ReadCoefData(m_appDatabaseName, worksheet2);

      m_chemicalList.SetCheckStyle( BS_AUTOCHECKBOX );
      
      for ( int i=0; i < gpHydroModel->m_chemicalParamInfoArray.GetCount(); i++ )
         {
         CHEMICAL_PARAM_INFO *pChem = gpHydroModel->m_chemicalParamInfoArray[i];
         CString name = pChem->name;
         m_chemicalList.AddString( name );
         m_chemicalList.SetCheck(i, false );
         }
      
      }
      UpdateData(false);
   }

int ppUserInteractionPesticide::ReadCoefData( LPCSTR fileName, CString worksheet)
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
   for (int k=0;k<gpHydroModel->m_chemicalParamInfoArray.GetSize(); k++)
      gpHydroModel->m_chemicalParamInfoArray.RemoveAt(k);
   
   gpHydroModel->m_chemicalParamInfoArray.RemoveAll();
  // gpHydroModel->m_chemicalParamInfoArray->SetSize( cols, 0 );  // resize matrix, statArray

   for (int row=0; row < rows; row++ )
      {
      CHEMICAL_PARAM_INFO *pChem = new CHEMICAL_PARAM_INFO;
      xls.GetData(row, 0, pChem->name); // assume the name of the pesticide is in the first row...
      xls.GetData(row, 1, pChem->ksMax); //ks minimum
      xls.GetData(row, 2, pChem->ksMin); // ksMaximum
      xls.GetData(row, 3, pChem->koc); // octanol water partioning coeff
      xls.GetData(row, 4, pChem->kf);  // foliar degradataion

      gpHydroModel->m_chemicalParamInfoArray.Add(pChem);
      
      }
      m_numberPesticides = rows-1;
/*
//  We have labels, now gather data row by row
   rows = xls.GetRecordCount();
   float str = 0;

   float *data = new float[cols];
   for (int row = 1; row < rows; row++) //first row is labels
      {
      for (col=0; col<cols; col++)
         xls.GetData(row, col, data[col]);

      AppendRow(data);
      }
   delete data;
   int t = GetColCount();
   return GetRowCount();
*/
	  delete [] xlsName;
      return -1;
   }



int ppUserInteractionPesticide::ReadAppData( LPCSTR fileName, CString worksheet)
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
   for (int k=0;k<gpHydroModel->m_chemicalAppInfoArray.GetSize(); k++)
      gpHydroModel->m_chemicalAppInfoArray.RemoveAt(k);
   
   gpHydroModel->m_chemicalAppInfoArray.RemoveAll();
  // gpHydroModel->m_chemicalParamInfoArray->SetSize( cols, 0 );  // resize matrix, statArray

   for (int row=0; row < rows; row++ )
      {
      CHEMICAL_APP_INFO *pChem = new CHEMICAL_APP_INFO;
      xls.GetData(row, 0, pChem->name); // assume the name of the pesticide is in the first row...
      xls.GetData(row, 2, pChem->lulc_c);
      xls.GetData(row, 3, pChem->lulc_b);
      xls.GetData(row, 4, pChem->rateMin);
      xls.GetData(row, 5, pChem->rateMax);
      xls.GetData(row, 6, pChem->beginDay1);
      xls.GetData(row, 7, pChem->beginMonth1);
      xls.GetData(row, 8, pChem->endDay1);
      xls.GetData(row, 9, pChem->endMonth1);
      gpHydroModel->m_chemicalAppInfoArray.Add(pChem);
      
      }	
      
      int cols = xls.GetColCount();
   // get the labels from first row
   //SetSize( cols, 0 );  // resize matrix, statArray

/*   for (int col=0; col < cols; col++ )
      {
      CString colName( xls.m_pData->GetLabel( col ) );
      colName.Trim();
      SetLabel(col,colName);
      }

//  We have labels, now gather data row by row
   rows = xls.GetRecordCount();
   float str = 0;

   float *data = new float[cols];
   for (int row = 1; row < rows; row++) //first row is labels
      {
      for (col=0; col<cols; col++)
         xls.GetData(row, col, data[col]);

      AppendRow(data);
      }
   delete data;
   int t = GetColCount();
   return GetRowCount();*/
	  delete [] xlsName;
      return -1;

   }
void ppUserInteractionPesticide::UpdateModelState()
   {
   m_numberPesticides = 0;
   for (int i=0; i<gpHydroModel->m_chemicalParamInfoArray.GetCount(); i++)
      if (m_chemicalList.GetCheck(i))
         {
         m_numberPesticides++;
         m_pesticideOffsets.Add( i );
         }
   }
void ppUserInteractionPesticide::OnEnChangeEdit1()
{
   // TODO:  If this is a RICHEDIT control, the control will not
   // send this notification unless you override the CPropertyPage::OnInitDialog()
   // function and call CRichEditCtrl().SetEventMask()
   // with the ENM_CHANGE flag ORed into the mask.

   // TODO:  Add your control notification handler code here
}

void ppUserInteractionPesticide::OnEnChangeEditpestappdata()
{
   // TODO:  If this is a RICHEDIT control, the control will not
   // send this notification unless you override the CPropertyPage::OnInitDialog()
   // function and call CRichEditCtrl().SetEventMask()
   // with the ENM_CHANGE flag ORed into the mask.

   // TODO:  Add your control notification handler code here
/*   for ( int i=0; i< gpHydroModel->m_chemicalParamInfoArray.GetCount(); i++ )
      {
      CHEMICAL_PARAM_INFO *pChem = gpHydroModel->m_chemicalParamInfoArray[i];
      CString name = pChem->name;
      m_chemicalList.AddString( name );
      m_chemicalList.SetCheck(i, true );
      }
*/
}

void ppUserInteractionPesticide::OnLbnSelchangeListchemical()
{
   // TODO: Add your control notification handler code here
}

void ppUserInteractionPesticide::OnBnClickedEditparam()
{

}

void ppUserInteractionPesticide::OnBnClickedEditapp()
{
   // TODO: Add your control notification handler code here
}
