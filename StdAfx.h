// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__1503714C_106B_4BC3_8450_64DA41FC89A4__INCLUDED_)
#define AFX_STDAFX_H__1503714C_106B_4BC3_8450_64DA41FC89A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif
#define _ATL_APARTMENT_THREADED

#include <stdio.h>
#include <comdef.h>

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>

#include "resource.h"

#include <d3d8.h>
#include <d3dx8.h>
#include <dinput.h>

#include <msxml.h>

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__1503714C_106B_4BC3_8450_64DA41FC89A4__INCLUDED)
