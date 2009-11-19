// MapStatsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
//#include "restore.h"
#include "MapStatsDlg.h"
#include <maplayer.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MapStatsDlg dialog


MapStatsDlg::MapStatsDlg(MapLayer *pCellLayer, CWnd* pParent /*=NULL*/)
	: CDialog(MapStatsDlg::IDD, pParent)
   {
   // get total map area
   int cellCount = pCellLayer->GetRecordCount();
   int col_area = pCellLayer->GetFieldCol( "Area" );
   int col_altern = pCellLayer->GetFieldCol( "Altern" );
   int col_restoredArea = pCellLayer->GetFieldCol( "ALT_LULC" );

   float cumArea = 0;
   float cumRestoredArea = 0;
   int   cumCells = 0;

   for ( int i=0; i < cellCount; i++ )
      {
      float area;
      bool ok = pCellLayer->GetData( i, col_area, area );
      ASSERT( ok );
      cumArea += area;

      float restoredArea;
      ok = pCellLayer->GetData( i, col_restoredArea, restoredArea );
      ASSERT( ok );

      int altern;
      ok = pCellLayer->GetData( i, col_altern, altern );
      ASSERT( ok );

      if ( altern >= 0 )
         {
         if ( restoredArea > 0.0f )
            cumRestoredArea += restoredArea;
         else
            cumRestoredArea += area;

         cumCells++;
         }
      }

	//{{AFX_DATA_INIT(MapStatsDlg)
	m_restoredArea = 0;
	m_totalArea = 0;
	m_restoredCells = 0;
	m_totalCells = 0;
	//}}AFX_DATA_INIT
   m_restoredArea =  long( cumRestoredArea / (1000*1000) );    // convert to km2
   m_totalArea = long( cumArea / (1000*1000) );
   m_totalCells = cellCount;
   m_restoredCells = cumCells;
}


void MapStatsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(MapStatsDlg)
	DDX_Text(pDX, IDC_RESTOREDAREA, m_restoredArea);
	DDX_Text(pDX, IDC_TOTALAREA, m_totalArea);
	DDX_Text(pDX, IDC_RESTOREDCELLS, m_restoredCells);
	DDX_Text(pDX, IDC_TOTALCELLS, m_totalCells);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(MapStatsDlg, CDialog)
	//{{AFX_MSG_MAP(MapStatsDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MapStatsDlg message handlers
