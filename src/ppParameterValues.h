#pragma once


#include <afxtempl.h>
#include "afxwin.h"
#include "afxcmn.h"
#include "spreadsheet2.h"
#include "datagrid1.h"
#include "spreadsheet3.h"

//  maybe this just contains a paramInfoArray (as in WET_HVIEW????)
struct RANDOM_PARAMETER
   {
   CString   paramName;
   bool    sampleParam;  // a 0 value means yes, sample. a 1 means no do not sample.
   float  currentValue;
   int    distNum; // from what distribution is the param sampled?
   float  minValue;
   float  maxValue;
   float  std;
   float  mean;

   RANDOM_PARAMETER( void ) : paramName(_T("")),currentValue(1.0f), sampleParam(false),distNum( 0 ), minValue( 0.0f ), maxValue(0.0f), std(0.0f), mean(0.0f) { }
   };

// An array that contains a series of RANDOM_NUMBERS, each one representing a model parameter
class RandomParameterArray : public CArray< RANDOM_PARAMETER*, RANDOM_PARAMETER* >
   {
   public:
      // destructor cleans up any allocated structures
     ~RandomParameterArray( void ) { for ( int i=0; i < GetSize(); i++ ) delete GetAt( i ); }      
   };

struct ALL_PARAM_INFO
   {
   CString  modelType;  //  semi-distributed - hillslope/riparian - geomorphic - distributed
   CString  landscapeType; //  What 'place' do these parameters represent (hillslope, riparian, etc..)
   RandomParameterArray randomParameterArray;
	  // GOOD LIST of parameters, but actually remember there is a separate struc (above) for each of them
   /*float  m;
   float  kSat;
   float  wiltPt;
   float  fieldCapac;
   float  k1;  //Seibert and McDonnell horizontal drainage model
   float  k2;  //Deep reservoir discharge coefficient
   float  initSat;
   float  n;
   float  wd; // channel width-depth ratio
   float  soilDepth;
   float  phi;
   float  vanGenN;  //unsaturated vertical rate equation (from VanGenucten, the m is based on this value)
*/
   ALL_PARAM_INFO( void ) : modelType(_T("")), landscapeType(_T("")),  randomParameterArray() { }
   };

//This class contains a set of param_info.  There is 1 for semi-distributed, 2 for hillslope riparian.
// and about 7 for the geomorphic mapping.  If it is a combo, well, you can multiply.
class AllParamInfoArray : public CArray< ALL_PARAM_INFO*, ALL_PARAM_INFO* >
   {
   public:
      // destructor cleans up any allocated structures
     ~AllParamInfoArray( void ) { for ( int i=0; i < GetSize(); i++ ) delete GetAt( i ); }      
   };



// ppParameterValues dialog

class ppParameterValues : public CPropertyPage
{
	DECLARE_DYNAMIC(ppParameterValues)

public:
	ppParameterValues();

	virtual ~ppParameterValues();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_PARAMETERVALUES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
   afx_msg void OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult);
   // This box is a checked list box that displays values of various model parameters
  // CCheckListBox m_parameterList;
   AllParamInfoArray m_allParameterArray;
   virtual BOOL OnInitDialog();
   afx_msg void OnLbnSelchangeList1();
   void GenerateRandomValues();
   afx_msg void OnBnClickedCheck1();
   int ReadCoefData( LPCSTR fileName, CString worksheet);
   CListCtrl m_parameterList;
   void AddDataToListControl(CString landscapeType, int offset);
   int m_currentLandscape;

 
;
CButton m_downLandscapeList;
CButton m_upLandscapeList;
afx_msg void OnBnClickedButton1();
afx_msg void OnBnClickedButton2();
CStatic m_landscapeName;
CString m_LandscapeType;


afx_msg void OnBnClickedRereadparam();
};
