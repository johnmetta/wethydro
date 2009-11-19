// ReportWriter.h: interface for the ReportWriter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REPORTWRITER_H__F0536EAE_D6D3_4883_93C6_06264B8735DA__INCLUDED_)
#define AFX_REPORTWRITER_H__F0536EAE_D6D3_4883_93C6_06264B8735DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxcoll.h>
#include <afxtempl.h>



class FormattedStringArray : public CArray< LPSTR, LPSTR >
{
public:
   LPSTR AddString( float value, LPCSTR format="%.0f" )
      {
      char *p = new char[ 32 ];
      sprintf( p, format, value );
      Add( p );
      
      return p;
      }

   LPSTR AddString( LPCSTR value )
      {
      char *p = new char[ lstrlen( value )+1 ];
      lstrcpy( p, value );
      Add( p );
      return p;
      }

   ~FormattedStringArray() { for ( int i=0; i < GetSize(); i++ ) delete GetAt(i);  RemoveAll(); }
};


class ReportWriter  
{
public:
	ReportWriter( LPCSTR templateName );
	virtual ~ReportWriter();

   bool GenerateReport();

   LPCSTR GetReportPath() { return m_reportPath; }

protected:
   char m_outputDir[ 256 ];     // directory to store emf's, etc (e.g. restore/source/reports/bear)
   char m_reportDir[ 256 ];  // "report" directory (contains report template" );
   char m_templatePath[ 256 ];
   char m_reportPath[ 256 ];

   FormattedStringArray m_strings;
   CMapStringToString   m_map;

   bool Substitute( LPSTR tbuffer, LPSTR buffer );
};

#endif // !defined(AFX_REPORTWRITER_H__F0536EAE_D6D3_4883_93C6_06264B8735DA__INCLUDED_)
