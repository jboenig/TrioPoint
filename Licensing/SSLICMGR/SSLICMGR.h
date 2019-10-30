// SSLICMGR.h : main header file for the SSLICMGR application
//

#if !defined(AFX_SSLICMGR_H__171A48E7_BB43_41C7_8B1C_406DFA8D1651__INCLUDED_)
#define AFX_SSLICMGR_H__171A48E7_BB43_41C7_8B1C_406DFA8D1651__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSSLICMGRApp:
// See SSLICMGR.cpp for the implementation of this class
//

class CSSLICMGRApp : public CWinApp
{
public:
	CSSLICMGRApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSSLICMGRApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSSLICMGRApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SSLICMGR_H__171A48E7_BB43_41C7_8B1C_406DFA8D1651__INCLUDED_)
