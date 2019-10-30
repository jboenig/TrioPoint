//==============================================================================
// Copyright (c) 2000  -  Triopoint Systems, Inc.
//
// Module Name: Internet.h
//
// Author(s): Jeff Boenig
//
// Description:
//
//==============================================================================

#ifndef __INTERNET_H__
#define __INTERNET_H__

#pragma once

#include "ShapeShifter.h"

namespace ShapeShifter {

/////////////////////////////////////////////////////////////////////////
// ShapeShifter Internet API

extern BOOL InternetIsURL(BSTR str);
extern HRESULT InternetStartSession(BSTR strProxyName, BSTR strProxyBypass);
extern HRESULT InternetLoadTexture(LPDIRECT3DDEVICE8 pDevice,
                                   LPCTSTR lpszURL,
                                   LPDIRECT3DTEXTURE8* ppTexture);
extern HRESULT InternetEndSession();

};  // namespace ShapeShifter

#endif // #ifndef __INTERNET_H__
