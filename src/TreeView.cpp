// TreeView.cpp : implementation file
//

#include "stdafx.h"
#include "wet_h.h"
#include "TreeView.h"
#include "ResultPlotWnd.h"
#include "wet_hydro.h"

extern WET_Hydro *gpHydroModel;
extern ResultPlotWnd *gpResultPlotWnd;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define UM_CHECKSTATECHANGE (WM_USER + 100)

/////////////////////////////////////////////////////////////////////////////
// TreeView

IMPLEMENT_DYNCREATE(TreeView, CTreeView)

TreeView::TreeView()
{
}

TreeView::~TreeView()
{
}


BEGIN_MESSAGE_MAP(TreeView, CTreeView)
	//{{AFX_MSG_MAP(TreeView)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_NOTIFY_REFLECT(NM_RDBLCLK, OnRdblclk)
	ON_WM_RBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TreeView drawing

void TreeView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();

}

/////////////////////////////////////////////////////////////////////////////
// TreeView diagnostics

#ifdef _DEBUG
void TreeView::AssertValid() const
{
	CTreeView::AssertValid();
}

void TreeView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// TreeView message handlers



void TreeView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{

}

void TreeView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{

	
	
}

void TreeView::refresh()
	{

	}


void TreeView::OnShowWindow(BOOL bShow, UINT nStatus)

{

}

void TreeView::OnActivate()
{
	



}

void TreeView::OnInitialUpdate() 
{
	CTreeView::OnInitialUpdate();
	
//	m_imageState.Create( IDB_STATE, 13, 1, RGB(255,255,255) );//
//	SetImageList( &(m_imageState), TVSIL_STATE );

	
}

void TreeView::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	*pResult = 0;
}

void TreeView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HTREEITEM hSelected = GetTreeCtrl().GetSelectedItem();
	if ( hSelected >= 0 )
		{
		CString strSelected = GetTreeCtrl().GetItemText(hSelected);
		HTREEITEM parent = GetTreeCtrl().GetParentItem(hSelected);
      HTREEITEM grandparent = GetTreeCtrl().GetParentItem(parent);
		CString runNum = GetTreeCtrl().GetItemText(grandparent);
		int runNumber = atoi(runNum);
		

		if ( strSelected == "Climate Data" )
			gpResultPlotWnd->AllocateScatter(gpHydroModel->m_resultDataArray[runNumber][0], (LPCTSTR)strSelected);

		if ( strSelected == "Stream Discharge" )
			gpResultPlotWnd->AllocateScatter(gpHydroModel->m_resultDataArray[runNumber][1], (LPCTSTR)strSelected);

		if ( strSelected == "ET Rate" )
			gpResultPlotWnd->AllocateScatter(gpHydroModel->m_resultDataArray[runNumber][2], (LPCTSTR)strSelected);

		if ( strSelected == "Average Soil Water Content" )
			gpResultPlotWnd->AllocateScatter(gpHydroModel->m_resultDataArray[runNumber][3], (LPCTSTR)strSelected);
		
      if ( strSelected == "Unsaturated Zone Depth" )
			gpResultPlotWnd->AllocateScatter(gpHydroModel->m_resultDataArray[runNumber][15], (LPCTSTR)strSelected);
     
      if ( strSelected == "Saturated Zone Depth" )
			gpResultPlotWnd->AllocateScatter(gpHydroModel->m_resultDataArray[runNumber][16], (LPCTSTR)strSelected);

      if ( strSelected == "Saturation Deficit" )
			gpResultPlotWnd->AllocateScatter(gpHydroModel->m_resultDataArray[runNumber][17], (LPCTSTR)strSelected);

      if ( strSelected == "Mass Balance" )
			gpResultPlotWnd->AllocateScatter(gpHydroModel->m_resultDataArray[runNumber][4], (LPCTSTR)strSelected);
  	
      if ( strSelected == "Tracer Mass Upslope" )
   		gpResultPlotWnd->AllocateScatter(gpHydroModel->m_resultDataArray[runNumber][5], (LPCTSTR)strSelected);
      
      if ( strSelected == "Upland Storage" )
			gpResultPlotWnd->AllocateScatter(gpHydroModel->m_resultDataArray[runNumber][6], (LPCTSTR)strSelected);

      if ( strSelected == "Time Step" )
   		gpResultPlotWnd->AllocateScatter(gpHydroModel->m_resultDataArray[runNumber][7], (LPCTSTR)strSelected);

  		if ( strSelected == "Tracer Conc" )
   		gpResultPlotWnd->AllocateScatter(gpHydroModel->m_resultDataArray[runNumber][8], (LPCTSTR)strSelected);

  	/*	if ( strSelected == "Instream Tracer Conc. (mgl)" )
   		gpResultPlotWnd->AllocateScatter(gpHydroModel->m_resultDataArray[runNumber][9], (LPCTSTR)strSelected);*/
  		if ( strSelected == "Instream Tracer Conc. (mgl) " )
   		gpResultPlotWnd->AllocateScatter(gpHydroModel->m_resultDataArray[runNumber][25], (LPCTSTR)strSelected);

  		if ( strSelected == "Upland Sediment" )
   		gpResultPlotWnd->AllocateScatter(gpHydroModel->m_resultDataArray[runNumber][11], (LPCTSTR)strSelected);

  		if ( strSelected == "Instream Tracer Conc" )
   		gpResultPlotWnd->AllocateScatter(gpHydroModel->m_resultDataArray[runNumber][10], (LPCTSTR)strSelected);

      if ( strSelected == "Cumulative Sediment Outflow" )
   		gpResultPlotWnd->AllocateScatter(gpHydroModel->m_resultDataArray[runNumber][12], (LPCTSTR)strSelected);

      if ( strSelected == "Sediment Discharge" )
   		gpResultPlotWnd->AllocateScatter(gpHydroModel->m_resultDataArray[runNumber][13], (LPCTSTR)strSelected);

      if ( strSelected == "Foliar Runoff Rate" )
   		gpResultPlotWnd->AllocateScatter(gpHydroModel->m_resultDataArray[runNumber][18], (LPCTSTR)strSelected);
      if ( strSelected == "Surface Concentration" )
   		gpResultPlotWnd->AllocateScatter(gpHydroModel->m_resultDataArray[runNumber][19], (LPCTSTR)strSelected);
      if ( strSelected == "Unsat Zone Concentration" )
   		gpResultPlotWnd->AllocateScatter(gpHydroModel->m_resultDataArray[runNumber][20], (LPCTSTR)strSelected);
      if ( strSelected == "Sat Zone Concentration (leached)" )
   		gpResultPlotWnd->AllocateScatter(gpHydroModel->m_resultDataArray[runNumber][21], (LPCTSTR)strSelected);
      if ( strSelected == "Unsat Volumes" )
   		gpResultPlotWnd->AllocateScatter(gpHydroModel->m_resultDataArray[runNumber][22], (LPCTSTR)strSelected);
      if ( strSelected == "Instream Concentration" )
   		gpResultPlotWnd->AllocateScatter(gpHydroModel->m_resultDataArray[runNumber][23], (LPCTSTR)strSelected);
      if ( strSelected == "Pest Mass Balance" )
   		gpResultPlotWnd->AllocateScatter(gpHydroModel->m_resultDataArray[runNumber][24], (LPCTSTR)strSelected);


		}
	
	*pResult = 0;
}

void TreeView::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{



}


void TreeView::SaveFile(DataObj *currentObj, CString name)
   {
   CFileDialog dlg1( FALSE, "csv",name , OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
      "CSV file|*.csv|All files|*.*||");

   if ( dlg1.DoModal() == IDOK )
      {
      CString filename( dlg1.GetPathName() );
      CWaitCursor c;
      currentObj->WriteAscii(filename);
      }
   }


void TreeView::OnRdblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{

}

void TreeView::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	HTREEITEM hSelected = GetTreeCtrl().GetSelectedItem();
	if ( hSelected >= 0 )
		{
		CString strSelected = GetTreeCtrl().GetItemText(hSelected);
		HTREEITEM parent = GetTreeCtrl().GetParentItem(hSelected);
      HTREEITEM parent2 = GetTreeCtrl().GetParentItem(parent);
		CString runNum = GetTreeCtrl().GetItemText(parent2);
		int runNumber = atoi(runNum);
		
//		RESULT_DATA *pResult = gpHydroModel->m_resultDataArray[ runNumber ];		
	
		if ( strSelected == "Climate Data" )
         {
         DataObj *currentObj = gpHydroModel->m_resultDataArray[runNumber][0];
         SaveFile(currentObj, strSelected);
         }
      if ( strSelected == "Instream Tracer Conc. (mgl)" )
         {
   		DataObj *currentObj = gpHydroModel->m_resultDataArray[runNumber][25];
         SaveFile(currentObj, strSelected);
         }

		if ( strSelected == "Stream Discharge" )
         {
			DataObj *currentObj = gpHydroModel->m_resultDataArray[runNumber][1];
         SaveFile(currentObj, strSelected);
         }

		if ( strSelected == "ET Rate" )
         {
			DataObj *currentObj = gpHydroModel->m_resultDataArray[runNumber][2];
         SaveFile(currentObj, strSelected);
         }

		if ( strSelected == "Average Soil Water Content" )
         {
			DataObj *currentObj = gpHydroModel->m_resultDataArray[runNumber][3] ;
         SaveFile(currentObj, strSelected);
         }

		if ( strSelected == "Unsaturated Zone Depth" )
         {
			DataObj *currentObj = gpHydroModel->m_resultDataArray[runNumber][15] ;
         SaveFile(currentObj, strSelected);
         }

		if ( strSelected == "Saturated Zone Depth" )
         {
			DataObj *currentObj = gpHydroModel->m_resultDataArray[runNumber][16] ;
         SaveFile(currentObj, strSelected);
         }

		if ( strSelected == "Saturation Deficit" )
         {
			DataObj *currentObj = gpHydroModel->m_resultDataArray[runNumber][17] ;
         SaveFile(currentObj, strSelected);
         }

		if ( strSelected == "Mass Balance" )
         {
			DataObj *currentObj = gpHydroModel->m_resultDataArray[runNumber][4];
         SaveFile(currentObj, strSelected);
         }

      if ( strSelected == "Tracer Mass Upslope" )
         {
   		DataObj *currentObj = gpHydroModel->m_resultDataArray[runNumber][5];
         SaveFile(currentObj, strSelected);
         }

      if ( strSelected == "Upland Storage" )
         {
			DataObj *currentObj = gpHydroModel->m_resultDataArray[runNumber][6];
         SaveFile(currentObj, strSelected);
         }

      if ( strSelected == "Time Step" )
         {
   		DataObj *currentObj = gpHydroModel->m_resultDataArray[runNumber][7];
         SaveFile(currentObj, strSelected);
         }

  		if ( strSelected == "Tracer Conc" )
         {
   		DataObj *currentObj = gpHydroModel->m_resultDataArray[runNumber][8];
         SaveFile(currentObj, strSelected);
         }

  	/*	if ( strSelected == "Instream Tracer Conc. (mgl)" )
         {
   		DataObj *currentObj = gpHydroModel->m_resultDataArray[runNumber][9];
         SaveFile(currentObj, strSelected);
         }*/

  		if ( strSelected == "Upland Sediment" )
         {
   		DataObj *currentObj = gpHydroModel->m_resultDataArray[runNumber][11];
         SaveFile(currentObj, strSelected);
         }

  		if ( strSelected == "Instream Tracer Conc" )
         {
   		DataObj *currentObj = gpHydroModel->m_resultDataArray[runNumber][10];
         SaveFile(currentObj, strSelected);
         }

      if ( strSelected == "Cumulative Sediment Outflow" )
         {
   		DataObj *currentObj = gpHydroModel->m_resultDataArray[runNumber][12];
         SaveFile(currentObj, strSelected);
         }

      if ( strSelected == "Sediment Discharge" )
         {
   		DataObj *currentObj = gpHydroModel->m_resultDataArray[runNumber][13];
         SaveFile(currentObj, strSelected);
         }
        if ( strSelected == "Foliar Runoff Rate" )
         {
   		DataObj *currentObj = gpHydroModel->m_resultDataArray[runNumber][18];
         SaveFile(currentObj, strSelected);
         }
      if ( strSelected == "Surface Concentration" )
         {
   		DataObj *currentObj = gpHydroModel->m_resultDataArray[runNumber][19];
         SaveFile(currentObj, strSelected);
         }
      if ( strSelected == "Unsat Zone Concentration" )
         {
   		DataObj *currentObj = gpHydroModel->m_resultDataArray[runNumber][20];
         SaveFile(currentObj, strSelected);
         }
      if ( strSelected == "Sat Zone Concentration (leached)" )
         {
   		DataObj *currentObj = gpHydroModel->m_resultDataArray[runNumber][21];
         SaveFile(currentObj, strSelected);
         }
      if ( strSelected == "Unsat Volumes" )
         {
   		DataObj *currentObj = gpHydroModel->m_resultDataArray[runNumber][22];
         SaveFile(currentObj, strSelected);
         }
      if ( strSelected == "Instream Concentration" )
         {
   		DataObj *currentObj = gpHydroModel->m_resultDataArray[runNumber][23];
         SaveFile(currentObj, strSelected);
         }
      if ( strSelected == "Pest Mass Balance" )
         {
   		DataObj *currentObj = gpHydroModel->m_resultDataArray[runNumber][24];
         SaveFile(currentObj, strSelected);
         }

		}
	
	
	CTreeView::OnRButtonDblClk(nFlags, point);
}
