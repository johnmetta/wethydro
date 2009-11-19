// optimizeDlg.cpp : implementation file
//
#include <stdio.h>
#include <math.h>

#include "stdafx.h"
#include "wet_h.h"
#include "optimizeDlg.h"

#include <nr.h>
#include <nrutil.h>
#include "mainfrm.h"

#include "wet_hView.h"


#include "DerivedDouble.h"
#include "Axis.h"
#include "AxisCollection.h"
#include "ChartArea.h"
#include "Chart2DData.h"
#include "ChartGroup.h"
#include "ChartGroupCollection.h"
#include "SymbolStyle.h"
#include "ChartStyle.h"
#include "ChartStyleCollection.h"
#include "Label.h"
#include "ChartLabel.h"
#include "ChartLabelCollection.h"
#include "LabelCollection.h"
#include "Border.h"
#include "Interior.h"
#include "LineStyle.h"
#include "olecleg.h"
#include "Font.h"
#include "title.h"
#include "2dconst.h"


#include "DerivedDouble1.h"
#include "Axis1.h"
#include "AxisCollection1.h"
#include "ChartArea1.h"
#include "Chart3DData.h"
#include "ChartGroup1.h"
#include "ChartGroupCollection1.h"
//#include "SymbolStyle.h"
//#include "ChartStyle.h"
//#include "ChartStyleCollection.h"
#include "Label1.h"
#include "ChartLabel1.h"
#include "ChartLabelCollection1.h"
#include "LabelCollection1.h"
#include "Border1.h"
#include "Interior1.h"
#include "LineStyle1.h"
#include "legend1.h"
//#include "Font.h"
#include "title1.h"
#include "3dconst.h"

#include "Elevation.h"
#include "bar1.h"
#include "datacolorcollection.h"
#include "datacolor.h"
#include "surface.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


extern CMainFrame *gpMain;
extern WET_Hydro *gpHydroModel;

extern optimizeDlg *gpOptimize;
extern OptimizeDlg3DPlot *gpOptimizeDlg3DPlot;
extern  CWet_hView *gpView;



/////////////////////////////////////////////////////////////////////////////
// optimizeDlg dialog


optimizeDlg::optimizeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(optimizeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(optimizeDlg)
	m_maxis = 1.0f;
	m_maxks = 5.0f;
	m_maxm = 50.0f;
	m_minis = 0.1f;
	m_minks = .00001f;
	m_minm = 1.0f;
	m_minn = 0.001f;
	m_checkips = FALSE;
	m_checkksat = TRUE;
	m_checkm = TRUE;
	m_checkn = FALSE;
	m_optlength = 10.0f;  // how much time the optimization will cover - a storm? a summer?  a day?
	m_opttimestep = 1.0f; // time elapse between adjustment of the upgrade vector on the way to the param. minimum
	m_ks = 0.75f;
	m_m = 30.0f;
	m_maxn = 0.5f;
	m_n = 0.04f;
	m_is = 0.4f;
	m_isopt = 0.0f;
	m_ksopt = 0.0f;
	m_mopt = 0.0f;
	m_nopt = 0.0f;
	m_iclength = 145.0f;
	m_kdepth = 0.0001f;
	m_checket = FALSE;
	m_checkkdepth = FALSE;
	m_checksoildepth = FALSE;
	m_et = 0.001f;
	m_etopt = 0.0f;
	m_kdepthopt = 0.0f;
	m_maxet = 0.3f;
	m_maxkdepth = 0.2f;
	m_maxsoildepth = 4.0f;
	m_minet = 0.00f;
	m_minkdepth = 0.00f;
	m_minsoildepth = 0.1f;
	m_soildepth = 1.0f;
	m_soildepthopt = 0.0f;
	m_lamda = -0.5f;
	m_lamdafactor = 10.0f;
	m_checksimulationstudy = FALSE;
	m_simulationtext = _T("");
	m_perturbpercentvalue = 10.0f;
	//}}AFX_DATA_INIT
}


void optimizeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(optimizeDlg)
	DDX_Control(pDX, IDC_PERTURBPERCENT, m_perturbpercent);
	DDX_Control(pDX, IDC_PROGRESSBOX, m_progressbox);
	DDX_Text(pDX, IDC_MAXIS, m_maxis);
	DDV_MinMaxFloat(pDX, m_maxis, 1.f, 1.f);
	DDX_Text(pDX, IDC_MAXKS, m_maxks);
	DDV_MinMaxFloat(pDX, m_maxks, 5.f, 5.f);
	DDX_Text(pDX, IDC_MAXM, m_maxm);
	DDV_MinMaxFloat(pDX, m_maxm, 50.f, 50.f);
	DDX_Text(pDX, IDC_MINIS, m_minis);
	DDV_MinMaxFloat(pDX, m_minis, 0.1f, 0.1f);
	DDX_Text(pDX, IDC_MINKS, m_minks);
	DDV_MinMaxFloat(pDX, m_minks, 1.e-005f, 1.e-005f);
	DDX_Text(pDX, IDC_MINM, m_minm);
	DDV_MinMaxFloat(pDX, m_minm, 1.f, 1.f);
	DDX_Text(pDX, IDC_MINN, m_minn);
	DDV_MinMaxFloat(pDX, m_minn, 1.e-003f, 1.e-003f);
	DDX_Check(pDX, IDC_CHECKIPS, m_checkips);
	DDX_Check(pDX, IDC_CHECKKSAT, m_checkksat);
	DDX_Check(pDX, IDC_CHECKM, m_checkm);
	DDX_Check(pDX, IDC_CHECKN, m_checkn);
	DDX_Text(pDX, IDC_OPTLENGTH, m_optlength);
	DDV_MinMaxFloat(pDX, m_optlength, 1.f, 1000.f);
	DDX_Text(pDX, IDC_OPTTIMESTEP, m_opttimestep);
	DDV_MinMaxFloat(pDX, m_opttimestep, 1.38e-002f, 10.f);
	DDX_Text(pDX, IDC_KS, m_ks);
	DDV_MinMaxFloat(pDX, m_ks, 1.e-003f, 100.f);
	DDX_Text(pDX, IDC_M, m_m);
	DDV_MinMaxFloat(pDX, m_m, 1.f, 100.f);
	DDX_Text(pDX, IDC_MAXN, m_maxn);
	DDV_MinMaxFloat(pDX, m_maxn, 0.5f, 0.5f);
	DDX_Text(pDX, IDC_N, m_n);
	DDV_MinMaxFloat(pDX, m_n, 1.e-003f, 0.5f);
	DDX_Text(pDX, IDC_IS, m_is);
	DDV_MinMaxFloat(pDX, m_is, 0.1f, 0.9f);
	DDX_Text(pDX, IDC_ICLENGTH, m_iclength);
	DDX_Text(pDX, IDC_KDEPTH, m_kdepth);
	DDX_Check(pDX, IDC_CHECKET, m_checket);
	DDX_Check(pDX, IDC_CHECKKDDEPTH, m_checkkdepth);
	DDX_Check(pDX, IDC_CHECKSOILDEPTH, m_checksoildepth);
	DDX_Text(pDX, IDC_ET, m_et);
	DDX_Text(pDX, IDC_MAXET, m_maxet);
	DDV_MinMaxFloat(pDX, m_maxet, 0.3f, 0.3f);
	DDX_Text(pDX, IDC_MAXKDEPTH, m_maxkdepth);
	DDV_MinMaxFloat(pDX, m_maxkdepth, 0.2f, 0.2f);
	DDX_Text(pDX, IDC_MAXSOILDEPTH, m_maxsoildepth);
	DDV_MinMaxFloat(pDX, m_maxsoildepth, 4.f, 4.f);
	DDX_Text(pDX, IDC_MINET, m_minet);
	DDV_MinMaxFloat(pDX, m_minet, 0.f, 0.f);
	DDX_Text(pDX, IDC_MINKDEPTH, m_minkdepth);
	DDV_MinMaxFloat(pDX, m_minkdepth, 0.f, 0.f);
	DDX_Text(pDX, IDC_MINSOILDEPTH, m_minsoildepth);
	DDV_MinMaxFloat(pDX, m_minsoildepth, 0.1f, 0.1f);
	DDX_Text(pDX, IDC_SOILDEPTH, m_soildepth);
	DDX_Text(pDX, IDC_LAMDA, m_lamda);
	DDX_Text(pDX, IDC_LAMDAFACTOR, m_lamdafactor);
	DDX_Control(pDX, IDC_2DCHART, m_2dchart);
	DDX_Check(pDX, IDC_CHECKSIMULATIONSTUDY, m_checksimulationstudy);
	DDX_Text(pDX, IDC_PERTURBPERCENT, m_perturbpercentvalue);
	DDV_MinMaxFloat(pDX, m_perturbpercentvalue, 0.f, 100.f);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(optimizeDlg, CDialog)
	//{{AFX_MSG_MAP(optimizeDlg)
	ON_BN_CLICKED(IDC_OPT, OnOpt)
	ON_BN_CLICKED(IDC_RUN, OnRun)
	ON_BN_CLICKED(IDC_OTHEROUTPUT, OnOtheroutput)
	ON_BN_CLICKED(IDC_CHECKSIMULATIONSTUDY, OnChecksimulationstudy)
	ON_BN_CLICKED(IDCANCEL2, OnCancel2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

////////////////////////////////////////////
/////////////////////////////////
// optimizeDlg message handlers

void optimizeDlg::OnOK() 
{
	Optimize();
	
	CDialog::OnOK();
}

 
void optimizeDlg::Optimize() 
	{	

   gpOptimize = new optimizeDlg;

	UpdateData(true);

	static int ma = 7;  
	float *a = vector(1,ma);			// array with initial model parameters.  Optimization routine returns this array.

	a[ 1 ] = m_m;
	a[ 2 ] = m_is;
	a[ 3 ] = m_n;
	a[ 4 ] = m_ks;
	a[ 5 ] = m_soildepth;
	a[ 6 ] = m_kdepth;
	a[ 7 ] = m_et;

	float *aInit = vector(1,ma);			

	aInit[ 1 ] = m_m;
	aInit[ 2 ] = m_is;
	aInit[ 3 ] = m_n;
	aInit[ 4 ] = m_ks;
	aInit[ 5 ] = m_soildepth;
	aInit[ 6 ] = m_kdepth;
	aInit[ 7 ] = m_et;

	float *atry = vector(1,ma);
	for (int i=1; i<=ma; i++)
		atry[i]=aInit[i];

	CString msg1;
	msg1.Format("Calculating initial conditions by running the model for %i days m=%f ks=%f", (int)m_iclength, a[1], a[2]);
	gpMain->SetStatusMsg( msg1 ); 

			//  array indicating whether model parameters should be adjusted or not
	int *ia = ivector(1,ma);
	for (i=1; i<=ma; i++)
		ia[ i ] = 0;
	if (m_checkm)
		ia[ 1 ] = 1;
	if (m_checkips)
		ia[ 2 ] = 1;
	if (m_checkn)
		ia[ 3 ] = 1;
	if (m_checkksat)
		ia[ 4 ] = 1;
	if (m_checksoildepth)
		ia[ 5 ] = 1;
	if (m_checkkdepth)
		ia[ 6 ] = 1;
	if (m_checket)
		ia[ 7 ] = 1;


	float *minVal = vector(1,ma);
	minVal[ 1 ] = m_minm;
	minVal[ 2 ] = m_minis;
	minVal[ 3 ] = m_minn;
	minVal[ 4 ] = m_minks;
	minVal[ 5 ] = m_minsoildepth;
	minVal[ 6 ] = m_minkdepth;
	minVal[ 7 ] = m_minet;
	
	float *maxVal = vector(1,ma);
	maxVal[ 1 ] = m_maxm;
	maxVal[ 2 ] = m_maxis;
	maxVal[ 3 ] = m_maxn;
	maxVal[ 4 ] = m_maxks;
	maxVal[ 5 ] = m_maxsoildepth;
	maxVal[ 6 ] = m_maxkdepth;
	maxVal[ 7 ] = m_maxet;

// array to contain derivatives w/repect to parameters
	float *dyda = vector(1,ma);
	
	static int mfit=0;    // mfit is the number of params that are to be adjusted (may be less than the number of params...)

	for (int j=0; j < ma; j++) 
		if (ia[j])
			mfit++;	
		
//initialize covariance matrix and the curvature matrix
	float **covar = matrix(1,ma,1,ma);
	float **alpha = matrix(1,ma,1,ma);

	for (j=1;j<=ma;j++)
		{
		for (int k=1;k<=ma;k++) 
			{
			alpha[j][k]=0.0f;

			covar[j][k]=0.0f;
			}
		}

	float bigNumber = -10000.f, bigNumber2=-10000.f;
	float *chisq = &bigNumber;
	float *currentChisq = &bigNumber2;
	float lamdaInit = -1.0f;
	float *alamda = &lamdaInit; //initialize lambda to < 0, and it signals the routine to initialize
	float ymodel=0.0f;  //ymod is the new estimate of q based on new parameter estimates.

	int ndata = (int)( m_optlength / m_opttimestep ) - 1;// the number of points used in the optimization
																		//  This is used as the highest offset so subtract 1 and it is zero based

	ASSERT(m_optlength > m_opttimestep );

	float *x = vector(1,ndata);
	float *y = vector(1,ndata);
	float *sig = vector(1,ndata);

	float *ymod = vector(1,ndata);

	float **output = matrix(1,4,1,ndata);
	
/////???????? aInit not bool!!!!

	gpHydroModel->CalcInitialConditions( (double)m_iclength, aInit ) ; // note that end is the time to stop calculating ics, in # of days...


	if (m_opttimestep < gpHydroModel->m_rkvMaxTimeStep)
		{
		CString msg;
		msg.Format( "A suggestion: Set optimization timestep (currently %f) greater than the maximum model timestep (currently %f)", m_opttimestep, gpHydroModel->m_rkvMaxTimeStep );
		gpMain->MessageBox( msg );
		}
	
	
	double time = gpHydroModel->m_time;
	double optimizationTime = ( time - m_opttimestep) ;

	gpMain->SetStatusMsg("Finished with Initial Conditions, moving to Optimization routine" ); 

	if (m_checksimulationstudy)
		{
	
		gpHydroModel->m_stopTime=gpHydroModel->m_time + m_optlength;
		gpHydroModel->m_startTime=gpHydroModel->m_time;
		gpHydroModel->Run();
		gpHydroModel->RestoreState(2);
		float percent = 1.0f-(m_perturbpercentvalue/100);
		if (m_checkm)
			a[ 1 ] = m_m*percent;
		if (m_checkips)
			a[ 2 ] = m_is*percent;
		if (m_checkn)
			a[ 3 ] = m_n*percent;
		if (m_checkksat)
			a[ 4 ] = m_ks*percent;
		if (m_checksoildepth)
			a[ 5 ] = m_soildepth*percent;
		if (m_checkkdepth)
			a[ 6 ] = m_kdepth*percent;
		if (m_checket)
			a[ 7 ] = m_et*percent;
		}


	for (  i=1; i<=ndata; i++ )
		{
		// populate an array with the times when optimization will occur. (it won't be every model timestep)
      ///////???????????
		x[i] = optimizationTime += (float)m_opttimestep;  // TIME MUST COME FROM THE GPHYDROMODEL!!!!!!!!!!!!!!!!!!!
		if (m_checksimulationstudy) //if we are doing simulation, get measurements from the model
			y[i]=gpHydroModel->m_pInstreamData->IGet(x[i]);
		else
			y[i] = gpHydroModel->m_pMeasuredDischargeData->IGet(x[i]); // for the time, get the measured discharge;
		
		sig[i] = 1.0f; //sds are unknown, so set them equal to 1.0f;

		}
	// At this point, we have initialized conditions, set up necessary arrays, and are at our start time, which is after
	// the initial condition calculation.

	// the next step is to begin the optimization routines.

	float previousChisq = 10000.f;
	int numIterations = 0;
	int count = 0;
	int positiveStep =0;
//	FILE *fp = fopen( "d:\\schafer\\output.txt", "a" );
	bool better = false;

//	for ( i=0; i < m_paramHistoryArray.GetSize(); i++ )
//		delete m_paramHistoryArray[ i ];
//	m_paramHistoryArray.RemoveAll(); 




	while ( count <= 10)
		{
		previousChisq = *chisq;		
		CString msg;
		msg.Format("\n Iteration %i\n\n m=%f\n   mBEST=%f\n is=%f\n   isBEST=%f\n n=%f\n   nBEST=%f\n ks=%f\n   ksBEST=%f\n soilD=%f\n   soilDBEST=%f\n kDep=%f\n   kDepBEST=%f\n\n current Chisq=%f\n Chisquare to Beat=%f\n lamda=%f\n Number of Improved Vectors=%i", numIterations,atry[1],a[1],atry[2],a[2],atry[3],a[3],atry[4],a[4],atry[5],a[5],atry[6],a[6], *currentChisq, *chisq, *alamda, positiveStep);
		gpMain->SetStatusMsg( msg ); 

//		fprintf(fp, msg);




		SetDlgItemText(IDC_PROGRESSBOX,msg);
		m_progressbox.UpdateData(false);


		mrqmin(x,y,sig,ndata,a,minVal,maxVal,ia,ma,covar,alpha,chisq,gpHydroModel->EstimateParams,alamda,(long) gpHydroModel, atry, currentChisq, ymod);

		PARAM_HISTORY *pParam = new PARAM_HISTORY;
		pParam->chisq = *currentChisq;
		pParam->a1 = a[1];
		pParam->a2 = a[2];
		pParam->a3 = a[3];
		pParam->a4 = a[4];
		pParam->a5 = a[5];
		pParam->a6 = a[6];
		pParam->a7 = a[7];
		pParam->ndata = ndata;
		pParam->x=x;
		pParam->y=y;
		pParam->ymod=ymod;
		m_paramHistoryArray.Add( pParam );	
		
		
		m_chisquare = *chisq;
		float improvement=previousChisq - *chisq;

		
		if ( improvement < 0.01f && improvement > 0.0f)
			{
			count++;
			positiveStep++;
			better = true;
			}
		if ( improvement > 0.001f )
			{
			positiveStep++;
			better=true;
			}


		for (  i=1; i<=ndata; i++ )
			{
			output[1][i] = x[i];  
			output[2][i] = y[i]; 
			output[3][i] = ymod[i]; 
			output[4][i] = gpHydroModel->m_pClimateData->IGet(x[i],1,IM_LINEAR);
			}
		Create2DPlot( output, ndata, better);
		
		better=false;


		int type = 2;
		gpHydroModel->RestoreState( type );	 //As Above, back to initial conditions
		numIterations++;

		if (numIterations > 20)
			break;
		}

	// After convergence, make a last call to set the covariance matrix
	*alamda = 0.0f;
	mrqmin(x,y,sig,ndata,a,minVal,maxVal,ia,ma,covar,alpha,chisq,gpHydroModel->EstimateParams,alamda,(long) gpHydroModel, atry, currentChisq, ymod);
	
	PARAM_HISTORY *pParam = new PARAM_HISTORY;
	pParam->chisq = *currentChisq;
	pParam->a1 = a[1];
	pParam->a2 = a[2];
	pParam->a3 = a[3];
	pParam->a4 = a[4];
	pParam->a5 = a[5];
	pParam->a6 = a[6];
	pParam->a7 = a[7];
	pParam->ndata = ndata;
	pParam->x=x;
	pParam->y=y;
	pParam->ymod=ymod;
	m_paramHistoryArray.Add( pParam );	
	int test = m_paramHistoryArray.GetSize();
	
	
	
	/*	m_m        =a [ 1 ];
	m_is       =a [ 2 ];
	m_n        =a [ 3 ];
	m_ks       =a [ 4 ];
	m_soildepth=a [ 5 ];
	m_kdepth   =a [ 6 ];
	m_et       =a [ 7 ];*/
	
	for (  i=1; i<=ndata; i++ )
		{
		output[1][i] = x[i];  
		output[2][i] = y[i]; 
		output[3][i] = ymod[i]; 
		output[4][i] = gpHydroModel->m_pClimateData->IGet(x[i],1,IM_LINEAR);
		}
	
	free_ivector(ia,1,ma);
	free_vector(a,1,ma);
	free_vector(aInit,1,ma);
	free_vector(minVal,1,ma);
	free_vector(maxVal,1,ma);
	free_vector(dyda,1,ma);
	free_vector(x,1,ndata);
	free_vector(y,1,ndata);
	free_vector(sig,1,ndata);
	free_matrix(covar,1,mfit,1,ma);
	free_matrix(alpha,1,mfit,1,ma);


	CString msg2;
	msg2.Format("Optimization Complete");
	gpMain->SetStatusMsg( msg2 ); 

//	fclose(fp);

	better=true;

	Create2DPlot( output , ndata, better);
	gpHydroModel->RestoreState(2);

	}

void optimizeDlg::OnOpt() 
{
	Optimize();
	
}

void optimizeDlg::OnRun() 
	{/*
	static int ma = 7;  
	float *a = vector(1,ma);			// array with initial model parameters.  Optimization routine returns this array.

	a[ 1 ] = m_m;
	a[ 2 ] = m_is;
	a[ 3 ] = m_n;
	a[ 4 ] = m_ks;
	a[ 5 ] = m_soildepth;
	a[ 6 ] = m_kdepth;
	a[ 7 ] = m_et;

	gpHydroModel->m_m        =a [ 1 ];
	gpHydroModel->m_is       =a [ 2 ];
	gpHydroModel->m_n        =a [ 3 ];
	gpHydroModel->m_ks       =a [ 4 ];
	gpHydroModel->m_soildepth=a [ 5 ];
	gpHydroModel->m_kdepth   =a [ 6 ];
	gpHydroModel->m_et       =a [ 7 ];

	gpHydroModel->m_startTime = gpHydroModel->m_startTime + m_iclength;
	gpHydroModel->m_stopTime = gpHydroModel->m_startTime  + m_optlength;
	gpHydroModel->Run(  );	
	gpHydroModel->PlotOutput();*/
	}

int optimizeDlg::Create2DPlot(float **output , int ndata, bool better)
	{
    // set up the colors and border styles

	int lArraySize = 50;
	int lShiftSize = 40;
	int lNextPoint = 0;
	int lNextX     = 0;

	m_2dchart.GetInterior().SetBackgroundColor(RGB(245, 222, 179));
	m_2dchart.GetChartArea().GetInterior().SetBackgroundColor(RGB(247, 206, 151));
	m_2dchart.GetChartArea().GetBorder().SetType(oc2dBorderShadow);
	m_2dchart.GetLegend().GetInterior().SetBackgroundColor(RGB(210, 180, 140));
	m_2dchart.GetLegend().GetBorder().SetType(oc2dBorderShadow);
	m_2dchart.GetLegend().GetBorder().SetWidth(4);

    // set legend font point size
	CY cy;
	cy.Lo = 100000;
	cy.Hi = 0;
	m_2dchart.GetLegend().GetFont().SetSize(cy);
 	 
	CAxis axisX(m_2dchart.GetChartArea().GetAxes().GetItem(COleVariant("x")));
	CAxis axisY(m_2dchart.GetChartArea().GetAxes().GetItem(COleVariant("y")));



	CChart2DData data(m_2dchart.GetChartGroups().GetItem(COleVariant(short(1))).GetData());
	CChart2DData data2(m_2dchart.GetChartGroups().GetItem(COleVariant(short(2))).GetData());

	m_2dchart.GetChartGroups().GetItem(COleVariant(short(2))).SetChartType(oc2dTypeBar);


	CAxis axisX2(m_2dchart.GetChartArea().GetAxes().GetItem(COleVariant("x2")));
	CAxis axisY2(m_2dchart.GetChartArea().GetAxes().GetItem(COleVariant("y2")));


	CLabelCollection seriesLabels(m_2dchart.GetChartGroups().GetItem(COleVariant(short(1))).GetSeriesLabels());
	CChartLabelCollection chartLabel(m_2dchart.GetChartLabels());
	chartLabel.Add().SetName("Value");

	if (better)
		m_2dchart.GetHeader().GetText().SetText("Accepted Parameter Set");
	else
		m_2dchart.GetHeader().GetText().SetText("Rejected Parameter Set");

	VARIANT va;
	VariantInit(&va);
	va.vt = VT_I4;
    
	// Add Legend labels
	va.lVal = seriesLabels.GetCount() + 1;
	seriesLabels.Add("Measured", va);
	va.lVal = seriesLabels.GetCount() + 1;
	seriesLabels.Add("Modeled", va);
	va.lVal = seriesLabels.GetCount() + 1;
	seriesLabels.Add("Rainfall", va);


	axisX.GetMin().SetValue(output[1][1]);
	axisX.GetMax().SetValue(output[1][ndata]);
	axisY.GetMin().SetValue(0);

	float ymax = 0.0f;

	for (int series=2;series <=3; series++)
		for (int max=1; max<=ndata; max++)
			if (output[series][max] > ymax)
				ymax = output[series][max];


    axisY.GetMax().SetValue(ymax);

    // batch for efficiency

    data.SetLayout(oc2dDataArray);
    data.SetNumSeries(2);
    data.SetNumPoints(1, ndata);
    data.SetFirstSeries(1);
    data.SetLastSeries(data.GetNumSeries());

    data2.SetLayout(oc2dDataArray);
    data2.SetNumSeries(1);
    data2.SetNumPoints(1, ndata);
    data2.SetFirstSeries(1);
    data2.SetLastSeries(data2.GetNumSeries());


    for (int i = 1; i <= data.GetNumSeries(); i++) {
        CChartStyle style(m_2dchart.GetChartGroups().GetItem(COleVariant(short(1))).GetStyles().GetItem(COleVariant(short(i))));
        // clear the shape style
        style.GetSymbol().SetShape(oc2dShapeNone);
        // make the lines thicker
        style.GetLine().SetWidth(3);
		  
		  
    }

	 CChartStyle style2(m_2dchart.GetChartGroups().GetItem(COleVariant(short(2))).GetStyles().GetItem(COleVariant(short(1))));
    style2.GetLine().SetWidth(1);
	 
	 style2.GetSymbol().SetShape(oc2dShapeSquare);
	 style2.GetLine().SetColor(RGB(255, 255, 255));

    // change this color to not conflict with the legend background
    CChartStyle style(m_2dchart.GetChartGroups().GetItem(COleVariant(short(1))).GetStyles().GetItem(COleVariant(short(2))));
    style.GetLine().SetColor(RGB(200, 100, 0));


	 //First populate the x values (time)

	 for ( int k=1; k<=ndata; k++)
		 data.SetX(1,k,output[1][k]);

	 // populate y values, and note that there are separate y values for each series

	 for (int n=1; n<=2; n++)
		 for (int m=1; m<=ndata; m++)
			 data.SetY(n,m,output[n+1][m]);

	//  The second ChartGroup contains rainfall data. This code populates the X and Y values
	 for (int z=1; z<=ndata; z++)
		{
		data2.SetX(1,z,output[1][z]);
		data2.SetY(1,z,output[4][z]);
		}
	 

   CRect rect;
   m_2dchart.GetClientRect(rect);
	m_2dchart.SetRedraw(true);
	m_2dchart.BringWindowToTop();

	return 0;
	
	
	}


void optimizeDlg::OnOtheroutput() 

	{
//	gpView->m_optimizeDlg3DPlot.DoModal();


	}





void optimizeDlg::OnChecksimulationstudy() 
	{

	if (!m_checksimulationstudy)
		{
		m_perturbpercent.ShowWindow(true);
		m_checksimulationstudy=true;


		}
	else
		{
		m_perturbpercent.ShowWindow(false);
		m_checksimulationstudy=false;


		}
	
}

void optimizeDlg::OnCancel2() 
{
//if (gpOptimize != NULL)
//	delete gpOptimize;	
}

void optimizeDlg::UpdatePlot()
	{

	int last = m_paramHistoryArray.GetSize();




	}
