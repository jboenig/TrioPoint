// BasicShapes.h : main header file for the BASICSHAPES application
//

#if !defined(AFX_BASICSHAPES_H__F9BC58F1_3205_44C8_8E7C_8ABF1CD0B999__INCLUDED_)
#define AFX_BASICSHAPES_H__F9BC58F1_3205_44C8_8E7C_8ABF1CD0B999__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CBasicShapesApp:
// See BasicShapes.cpp for the implementation of this class
//

class CBasicShapesApp : public CWinApp
{
public:
	CBasicShapesApp();

/////////////////////////////////////////////////////////////////////////////
// BEGIN ShapeShifter
public:
	IEnginePtr m_spEngine;
	IDevicePtr m_spDevice;
// END ShapeShifter
/////////////////////////////////////////////////////////////////////////////

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBasicShapesApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CBasicShapesApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BASICSHAPES_H__F9BC58F1_3205_44C8_8E7C_8ABF1CD0B999__INCLUDED_)
